/* officina.c - GENERATUM (amalgamator) - NE MANU EDITES
 *
 * Medulla (IR registrorum) + forma textualis + demissio in
 * plagula una (SQLite modo). DEPENDENTIA EXTERNA UNICA:
 * silva.h (officina a silva pendet EX ARCHITECTURA) -
 * compila cum -I<radix>/silva/amalgama et conecte silva.c.
 * Fons veritatis: officina/fontes/ + bibliothecae vendicatae
 * in lib/. Regenerare: officina/amalgamare.sh
 */

#include "silva.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/mman.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

/* ================= officina.h (verbatim) ================= */
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
#include <stdio.h>   /* FILE (machinula_ansam_ponere M4b) */

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
 * sola in TU amalgamato); tabula segmentorum post caput allocata
 * (2026-09-02), sine titulo */
typedef struct OfficinaXar {
    unsigned int  numerus_elementorum;
    unsigned int  magnitudo_elementi;
    unsigned int  magnitudo_primi;
    unsigned int  numerus_segmentorum;
    unsigned int  capacitas_totalis;
    unsigned int  vexilla;
    unsigned int  segmenta_maxima;   /* mensura tabulae (XXIV | LXIV) */
    OfficinaPiscina* piscina;
    void**        segmenta;          /* tabula post caput, una allocatione */
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
    MACHINULA_PAUSA,  /* punctum tactum - resumabile (pergere) */
    MACHINULA_RECUSATIO   /* aedilis in sessione recusatus (M4b) */
} MachinulaExitusGenus;

/* vexilla recusationum (M4b sessio) */
#define MACHINULA_RECUSARE_SCRIPTURAS  1
#define MACHINULA_RECUSARE_TEMPUS      2
#define MACHINULA_RECUSARE_INITUM      4

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
void machinula_ansam_ponere(Machinula* machinula, int ansa,
    FILE* plagula);
void machinula_recusationes_ponere(Machinula* machinula,
    unsigned int vexilla);
void machinula_ansas_claudere(Machinula* machinula);
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
int machinula_anulum_inspicere(const Machinula* machinula,
    unsigned int retro_index, int* functio_index_out,
    unsigned int* instructio_out);

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
OfficinaChorda indicium_via_chorda(const IndiciumLector* lector,
    unsigned int via_index);
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

/* ================= ex include/latina.h ================= */
/* latina.h - Verba clavis C89 Latine reddita (lexicon domus) */
#ifndef LATINA_H
#define LATINA_H

#include <stddef.h>

#define character     char
#define brevis             short
#define integer         int
#define longus            long
#define fluitans        float
#define duplex            double

#define vacuum            void
#define signatus         signed
#define insignatus  unsigned
#define constans        const
#define volatilis        volatile
#define sponte            auto
#define registrum     register
#define staticus         static
#define    externus         extern

#define si                    if
#define alioquin        else
#define commutatio    switch
#define casus                case
#define ordinarius    default
#define per                    for
#define dum                 while
#define fac                 do
#define frange             break
#define perge             continue
#define salta                goto
#define redde                return

#define structura        struct
#define unio                 union
#define enumeratio     enum
#define nomen             typedef

#define magnitudo     sizeof

#define principale     main

#define NIHIL                NULL
#define VERUM             1
#define FALSUM             0

/* NUMERI ROMANI - GENERATUM ex numerus_romanus_scribere
 * (tools/latina_numeri.sh -scribere) - NE MANU MUTES; porta
 * 'generata' iudicat. Omnes ZEPHYRUM-MMMCMXCIX ordine: numerus
 * Romanus classicus ad MMMCMXCIX finit - maiores ut expressio
 * (vinculum ut '* M'): IV * M (4000), IV * MXXIV (4096); vide
 * numerus_romanus_exprimere. Omne numerale identificator
 * reservatus est: capita systematis ANTE latina.h includenda
 * (dns_util.h membra 'MD' et 'MX' habet). */
#define ZEPHYRUM         0
#define I                1
#define II               2
#define III              3
#define IV               4
#define V                5
#define VI               6
#define VII              7
#define VIII             8
#define IX               9
#define X                10
#define XI               11
#define XII              12
#define XIII             13
#define XIV              14
#define XV               15
#define XVI              16
#define XVII             17
#define XVIII            18
#define XIX              19
#define XX               20
#define XXI              21
#define XXII             22
#define XXIII            23
#define XXIV             24
#define XXV              25
#define XXVI             26
#define XXVII            27
#define XXVIII           28
#define XXIX             29
#define XXX              30
#define XXXI             31
#define XXXII            32
#define XXXIII           33
#define XXXIV            34
#define XXXV             35
#define XXXVI            36
#define XXXVII           37
#define XXXVIII          38
#define XXXIX            39
#define XL               40
#define XLI              41
#define XLII             42
#define XLIII            43
#define XLIV             44
#define XLV              45
#define XLVI             46
#define XLVII            47
#define XLVIII           48
#define XLIX             49
#define L                50
#define LI               51
#define LII              52
#define LIII             53
#define LIV              54
#define LV               55
#define LVI              56
#define LVII             57
#define LVIII            58
#define LIX              59
#define LX               60
#define LXI              61
#define LXII             62
#define LXIII            63
#define LXIV             64
#define LXV              65
#define LXVI             66
#define LXVII            67
#define LXVIII           68
#define LXIX             69
#define LXX              70
#define LXXI             71
#define LXXII            72
#define LXXIII           73
#define LXXIV            74
#define LXXV             75
#define LXXVI            76
#define LXXVII           77
#define LXXVIII          78
#define LXXIX            79
#define LXXX             80
#define LXXXI            81
#define LXXXII           82
#define LXXXIII          83
#define LXXXIV           84
#define LXXXV            85
#define LXXXVI           86
#define LXXXVII          87
#define LXXXVIII         88
#define LXXXIX           89
#define XC               90
#define XCI              91
#define XCII             92
#define XCIII            93
#define XCIV             94
#define XCV              95
#define XCVI             96
#define XCVII            97
#define XCVIII           98
#define XCIX             99
#define C                100
#define CI               101
#define CII              102
#define CIII             103
#define CIV              104
#define CV               105
#define CVI              106
#define CVII             107
#define CVIII            108
#define CIX              109
#define CX               110
#define CXI              111
#define CXII             112
#define CXIII            113
#define CXIV             114
#define CXV              115
#define CXVI             116
#define CXVII            117
#define CXVIII           118
#define CXIX             119
#define CXX              120
#define CXXI             121
#define CXXII            122
#define CXXIII           123
#define CXXIV            124
#define CXXV             125
#define CXXVI            126
#define CXXVII           127
#define CXXVIII          128
#define CXXIX            129
#define CXXX             130
#define CXXXI            131
#define CXXXII           132
#define CXXXIII          133
#define CXXXIV           134
#define CXXXV            135
#define CXXXVI           136
#define CXXXVII          137
#define CXXXVIII         138
#define CXXXIX           139
#define CXL              140
#define CXLI             141
#define CXLII            142
#define CXLIII           143
#define CXLIV            144
#define CXLV             145
#define CXLVI            146
#define CXLVII           147
#define CXLVIII          148
#define CXLIX            149
#define CL               150
#define CLI              151
#define CLII             152
#define CLIII            153
#define CLIV             154
#define CLV              155
#define CLVI             156
#define CLVII            157
#define CLVIII           158
#define CLIX             159
#define CLX              160
#define CLXI             161
#define CLXII            162
#define CLXIII           163
#define CLXIV            164
#define CLXV             165
#define CLXVI            166
#define CLXVII           167
#define CLXVIII          168
#define CLXIX            169
#define CLXX             170
#define CLXXI            171
#define CLXXII           172
#define CLXXIII          173
#define CLXXIV           174
#define CLXXV            175
#define CLXXVI           176
#define CLXXVII          177
#define CLXXVIII         178
#define CLXXIX           179
#define CLXXX            180
#define CLXXXI           181
#define CLXXXII          182
#define CLXXXIII         183
#define CLXXXIV          184
#define CLXXXV           185
#define CLXXXVI          186
#define CLXXXVII         187
#define CLXXXVIII        188
#define CLXXXIX          189
#define CXC              190
#define CXCI             191
#define CXCII            192
#define CXCIII           193
#define CXCIV            194
#define CXCV             195
#define CXCVI            196
#define CXCVII           197
#define CXCVIII          198
#define CXCIX            199
#define CC               200
#define CCI              201
#define CCII             202
#define CCIII            203
#define CCIV             204
#define CCV              205
#define CCVI             206
#define CCVII            207
#define CCVIII           208
#define CCIX             209
#define CCX              210
#define CCXI             211
#define CCXII            212
#define CCXIII           213
#define CCXIV            214
#define CCXV             215
#define CCXVI            216
#define CCXVII           217
#define CCXVIII          218
#define CCXIX            219
#define CCXX             220
#define CCXXI            221
#define CCXXII           222
#define CCXXIII          223
#define CCXXIV           224
#define CCXXV            225
#define CCXXVI           226
#define CCXXVII          227
#define CCXXVIII         228
#define CCXXIX           229
#define CCXXX            230
#define CCXXXI           231
#define CCXXXII          232
#define CCXXXIII         233
#define CCXXXIV          234
#define CCXXXV           235
#define CCXXXVI          236
#define CCXXXVII         237
#define CCXXXVIII        238
#define CCXXXIX          239
#define CCXL             240
#define CCXLI            241
#define CCXLII           242
#define CCXLIII          243
#define CCXLIV           244
#define CCXLV            245
#define CCXLVI           246
#define CCXLVII          247
#define CCXLVIII         248
#define CCXLIX           249
#define CCL              250
#define CCLI             251
#define CCLII            252
#define CCLIII           253
#define CCLIV            254
#define CCLV             255
#define CCLVI            256
#define CCLVII           257
#define CCLVIII          258
#define CCLIX            259
#define CCLX             260
#define CCLXI            261
#define CCLXII           262
#define CCLXIII          263
#define CCLXIV           264
#define CCLXV            265
#define CCLXVI           266
#define CCLXVII          267
#define CCLXVIII         268
#define CCLXIX           269
#define CCLXX            270
#define CCLXXI           271
#define CCLXXII          272
#define CCLXXIII         273
#define CCLXXIV          274
#define CCLXXV           275
#define CCLXXVI          276
#define CCLXXVII         277
#define CCLXXVIII        278
#define CCLXXIX          279
#define CCLXXX           280
#define CCLXXXI          281
#define CCLXXXII         282
#define CCLXXXIII        283
#define CCLXXXIV         284
#define CCLXXXV          285
#define CCLXXXVI         286
#define CCLXXXVII        287
#define CCLXXXVIII       288
#define CCLXXXIX         289
#define CCXC             290
#define CCXCI            291
#define CCXCII           292
#define CCXCIII          293
#define CCXCIV           294
#define CCXCV            295
#define CCXCVI           296
#define CCXCVII          297
#define CCXCVIII         298
#define CCXCIX           299
#define CCC              300
#define CCCI             301
#define CCCII            302
#define CCCIII           303
#define CCCIV            304
#define CCCV             305
#define CCCVI            306
#define CCCVII           307
#define CCCVIII          308
#define CCCIX            309
#define CCCX             310
#define CCCXI            311
#define CCCXII           312
#define CCCXIII          313
#define CCCXIV           314
#define CCCXV            315
#define CCCXVI           316
#define CCCXVII          317
#define CCCXVIII         318
#define CCCXIX           319
#define CCCXX            320
#define CCCXXI           321
#define CCCXXII          322
#define CCCXXIII         323
#define CCCXXIV          324
#define CCCXXV           325
#define CCCXXVI          326
#define CCCXXVII         327
#define CCCXXVIII        328
#define CCCXXIX          329
#define CCCXXX           330
#define CCCXXXI          331
#define CCCXXXII         332
#define CCCXXXIII        333
#define CCCXXXIV         334
#define CCCXXXV          335
#define CCCXXXVI         336
#define CCCXXXVII        337
#define CCCXXXVIII       338
#define CCCXXXIX         339
#define CCCXL            340
#define CCCXLI           341
#define CCCXLII          342
#define CCCXLIII         343
#define CCCXLIV          344
#define CCCXLV           345
#define CCCXLVI          346
#define CCCXLVII         347
#define CCCXLVIII        348
#define CCCXLIX          349
#define CCCL             350
#define CCCLI            351
#define CCCLII           352
#define CCCLIII          353
#define CCCLIV           354
#define CCCLV            355
#define CCCLVI           356
#define CCCLVII          357
#define CCCLVIII         358
#define CCCLIX           359
#define CCCLX            360
#define CCCLXI           361
#define CCCLXII          362
#define CCCLXIII         363
#define CCCLXIV          364
#define CCCLXV           365
#define CCCLXVI          366
#define CCCLXVII         367
#define CCCLXVIII        368
#define CCCLXIX          369
#define CCCLXX           370
#define CCCLXXI          371
#define CCCLXXII         372
#define CCCLXXIII        373
#define CCCLXXIV         374
#define CCCLXXV          375
#define CCCLXXVI         376
#define CCCLXXVII        377
#define CCCLXXVIII       378
#define CCCLXXIX         379
#define CCCLXXX          380
#define CCCLXXXI         381
#define CCCLXXXII        382
#define CCCLXXXIII       383
#define CCCLXXXIV        384
#define CCCLXXXV         385
#define CCCLXXXVI        386
#define CCCLXXXVII       387
#define CCCLXXXVIII      388
#define CCCLXXXIX        389
#define CCCXC            390
#define CCCXCI           391
#define CCCXCII          392
#define CCCXCIII         393
#define CCCXCIV          394
#define CCCXCV           395
#define CCCXCVI          396
#define CCCXCVII         397
#define CCCXCVIII        398
#define CCCXCIX          399
#define CD               400
#define CDI              401
#define CDII             402
#define CDIII            403
#define CDIV             404
#define CDV              405
#define CDVI             406
#define CDVII            407
#define CDVIII           408
#define CDIX             409
#define CDX              410
#define CDXI             411
#define CDXII            412
#define CDXIII           413
#define CDXIV            414
#define CDXV             415
#define CDXVI            416
#define CDXVII           417
#define CDXVIII          418
#define CDXIX            419
#define CDXX             420
#define CDXXI            421
#define CDXXII           422
#define CDXXIII          423
#define CDXXIV           424
#define CDXXV            425
#define CDXXVI           426
#define CDXXVII          427
#define CDXXVIII         428
#define CDXXIX           429
#define CDXXX            430
#define CDXXXI           431
#define CDXXXII          432
#define CDXXXIII         433
#define CDXXXIV          434
#define CDXXXV           435
#define CDXXXVI          436
#define CDXXXVII         437
#define CDXXXVIII        438
#define CDXXXIX          439
#define CDXL             440
#define CDXLI            441
#define CDXLII           442
#define CDXLIII          443
#define CDXLIV           444
#define CDXLV            445
#define CDXLVI           446
#define CDXLVII          447
#define CDXLVIII         448
#define CDXLIX           449
#define CDL              450
#define CDLI             451
#define CDLII            452
#define CDLIII           453
#define CDLIV            454
#define CDLV             455
#define CDLVI            456
#define CDLVII           457
#define CDLVIII          458
#define CDLIX            459
#define CDLX             460
#define CDLXI            461
#define CDLXII           462
#define CDLXIII          463
#define CDLXIV           464
#define CDLXV            465
#define CDLXVI           466
#define CDLXVII          467
#define CDLXVIII         468
#define CDLXIX           469
#define CDLXX            470
#define CDLXXI           471
#define CDLXXII          472
#define CDLXXIII         473
#define CDLXXIV          474
#define CDLXXV           475
#define CDLXXVI          476
#define CDLXXVII         477
#define CDLXXVIII        478
#define CDLXXIX          479
#define CDLXXX           480
#define CDLXXXI          481
#define CDLXXXII         482
#define CDLXXXIII        483
#define CDLXXXIV         484
#define CDLXXXV          485
#define CDLXXXVI         486
#define CDLXXXVII        487
#define CDLXXXVIII       488
#define CDLXXXIX         489
#define CDXC             490
#define CDXCI            491
#define CDXCII           492
#define CDXCIII          493
#define CDXCIV           494
#define CDXCV            495
#define CDXCVI           496
#define CDXCVII          497
#define CDXCVIII         498
#define CDXCIX           499
#define D                500
#define DI               501
#define DII              502
#define DIII             503
#define DIV              504
#define DV               505
#define DVI              506
#define DVII             507
#define DVIII            508
#define DIX              509
#define DX               510
#define DXI              511
#define DXII             512
#define DXIII            513
#define DXIV             514
#define DXV              515
#define DXVI             516
#define DXVII            517
#define DXVIII           518
#define DXIX             519
#define DXX              520
#define DXXI             521
#define DXXII            522
#define DXXIII           523
#define DXXIV            524
#define DXXV             525
#define DXXVI            526
#define DXXVII           527
#define DXXVIII          528
#define DXXIX            529
#define DXXX             530
#define DXXXI            531
#define DXXXII           532
#define DXXXIII          533
#define DXXXIV           534
#define DXXXV            535
#define DXXXVI           536
#define DXXXVII          537
#define DXXXVIII         538
#define DXXXIX           539
#define DXL              540
#define DXLI             541
#define DXLII            542
#define DXLIII           543
#define DXLIV            544
#define DXLV             545
#define DXLVI            546
#define DXLVII           547
#define DXLVIII          548
#define DXLIX            549
#define DL               550
#define DLI              551
#define DLII             552
#define DLIII            553
#define DLIV             554
#define DLV              555
#define DLVI             556
#define DLVII            557
#define DLVIII           558
#define DLIX             559
#define DLX              560
#define DLXI             561
#define DLXII            562
#define DLXIII           563
#define DLXIV            564
#define DLXV             565
#define DLXVI            566
#define DLXVII           567
#define DLXVIII          568
#define DLXIX            569
#define DLXX             570
#define DLXXI            571
#define DLXXII           572
#define DLXXIII          573
#define DLXXIV           574
#define DLXXV            575
#define DLXXVI           576
#define DLXXVII          577
#define DLXXVIII         578
#define DLXXIX           579
#define DLXXX            580
#define DLXXXI           581
#define DLXXXII          582
#define DLXXXIII         583
#define DLXXXIV          584
#define DLXXXV           585
#define DLXXXVI          586
#define DLXXXVII         587
#define DLXXXVIII        588
#define DLXXXIX          589
#define DXC              590
#define DXCI             591
#define DXCII            592
#define DXCIII           593
#define DXCIV            594
#define DXCV             595
#define DXCVI            596
#define DXCVII           597
#define DXCVIII          598
#define DXCIX            599
#define DC               600
#define DCI              601
#define DCII             602
#define DCIII            603
#define DCIV             604
#define DCV              605
#define DCVI             606
#define DCVII            607
#define DCVIII           608
#define DCIX             609
#define DCX              610
#define DCXI             611
#define DCXII            612
#define DCXIII           613
#define DCXIV            614
#define DCXV             615
#define DCXVI            616
#define DCXVII           617
#define DCXVIII          618
#define DCXIX            619
#define DCXX             620
#define DCXXI            621
#define DCXXII           622
#define DCXXIII          623
#define DCXXIV           624
#define DCXXV            625
#define DCXXVI           626
#define DCXXVII          627
#define DCXXVIII         628
#define DCXXIX           629
#define DCXXX            630
#define DCXXXI           631
#define DCXXXII          632
#define DCXXXIII         633
#define DCXXXIV          634
#define DCXXXV           635
#define DCXXXVI          636
#define DCXXXVII         637
#define DCXXXVIII        638
#define DCXXXIX          639
#define DCXL             640
#define DCXLI            641
#define DCXLII           642
#define DCXLIII          643
#define DCXLIV           644
#define DCXLV            645
#define DCXLVI           646
#define DCXLVII          647
#define DCXLVIII         648
#define DCXLIX           649
#define DCL              650
#define DCLI             651
#define DCLII            652
#define DCLIII           653
#define DCLIV            654
#define DCLV             655
#define DCLVI            656
#define DCLVII           657
#define DCLVIII          658
#define DCLIX            659
#define DCLX             660
#define DCLXI            661
#define DCLXII           662
#define DCLXIII          663
#define DCLXIV           664
#define DCLXV            665
#define DCLXVI           666
#define DCLXVII          667
#define DCLXVIII         668
#define DCLXIX           669
#define DCLXX            670
#define DCLXXI           671
#define DCLXXII          672
#define DCLXXIII         673
#define DCLXXIV          674
#define DCLXXV           675
#define DCLXXVI          676
#define DCLXXVII         677
#define DCLXXVIII        678
#define DCLXXIX          679
#define DCLXXX           680
#define DCLXXXI          681
#define DCLXXXII         682
#define DCLXXXIII        683
#define DCLXXXIV         684
#define DCLXXXV          685
#define DCLXXXVI         686
#define DCLXXXVII        687
#define DCLXXXVIII       688
#define DCLXXXIX         689
#define DCXC             690
#define DCXCI            691
#define DCXCII           692
#define DCXCIII          693
#define DCXCIV           694
#define DCXCV            695
#define DCXCVI           696
#define DCXCVII          697
#define DCXCVIII         698
#define DCXCIX           699
#define DCC              700
#define DCCI             701
#define DCCII            702
#define DCCIII           703
#define DCCIV            704
#define DCCV             705
#define DCCVI            706
#define DCCVII           707
#define DCCVIII          708
#define DCCIX            709
#define DCCX             710
#define DCCXI            711
#define DCCXII           712
#define DCCXIII          713
#define DCCXIV           714
#define DCCXV            715
#define DCCXVI           716
#define DCCXVII          717
#define DCCXVIII         718
#define DCCXIX           719
#define DCCXX            720
#define DCCXXI           721
#define DCCXXII          722
#define DCCXXIII         723
#define DCCXXIV          724
#define DCCXXV           725
#define DCCXXVI          726
#define DCCXXVII         727
#define DCCXXVIII        728
#define DCCXXIX          729
#define DCCXXX           730
#define DCCXXXI          731
#define DCCXXXII         732
#define DCCXXXIII        733
#define DCCXXXIV         734
#define DCCXXXV          735
#define DCCXXXVI         736
#define DCCXXXVII        737
#define DCCXXXVIII       738
#define DCCXXXIX         739
#define DCCXL            740
#define DCCXLI           741
#define DCCXLII          742
#define DCCXLIII         743
#define DCCXLIV          744
#define DCCXLV           745
#define DCCXLVI          746
#define DCCXLVII         747
#define DCCXLVIII        748
#define DCCXLIX          749
#define DCCL             750
#define DCCLI            751
#define DCCLII           752
#define DCCLIII          753
#define DCCLIV           754
#define DCCLV            755
#define DCCLVI           756
#define DCCLVII          757
#define DCCLVIII         758
#define DCCLIX           759
#define DCCLX            760
#define DCCLXI           761
#define DCCLXII          762
#define DCCLXIII         763
#define DCCLXIV          764
#define DCCLXV           765
#define DCCLXVI          766
#define DCCLXVII         767
#define DCCLXVIII        768
#define DCCLXIX          769
#define DCCLXX           770
#define DCCLXXI          771
#define DCCLXXII         772
#define DCCLXXIII        773
#define DCCLXXIV         774
#define DCCLXXV          775
#define DCCLXXVI         776
#define DCCLXXVII        777
#define DCCLXXVIII       778
#define DCCLXXIX         779
#define DCCLXXX          780
#define DCCLXXXI         781
#define DCCLXXXII        782
#define DCCLXXXIII       783
#define DCCLXXXIV        784
#define DCCLXXXV         785
#define DCCLXXXVI        786
#define DCCLXXXVII       787
#define DCCLXXXVIII      788
#define DCCLXXXIX        789
#define DCCXC            790
#define DCCXCI           791
#define DCCXCII          792
#define DCCXCIII         793
#define DCCXCIV          794
#define DCCXCV           795
#define DCCXCVI          796
#define DCCXCVII         797
#define DCCXCVIII        798
#define DCCXCIX          799
#define DCCC             800
#define DCCCI            801
#define DCCCII           802
#define DCCCIII          803
#define DCCCIV           804
#define DCCCV            805
#define DCCCVI           806
#define DCCCVII          807
#define DCCCVIII         808
#define DCCCIX           809
#define DCCCX            810
#define DCCCXI           811
#define DCCCXII          812
#define DCCCXIII         813
#define DCCCXIV          814
#define DCCCXV           815
#define DCCCXVI          816
#define DCCCXVII         817
#define DCCCXVIII        818
#define DCCCXIX          819
#define DCCCXX           820
#define DCCCXXI          821
#define DCCCXXII         822
#define DCCCXXIII        823
#define DCCCXXIV         824
#define DCCCXXV          825
#define DCCCXXVI         826
#define DCCCXXVII        827
#define DCCCXXVIII       828
#define DCCCXXIX         829
#define DCCCXXX          830
#define DCCCXXXI         831
#define DCCCXXXII        832
#define DCCCXXXIII       833
#define DCCCXXXIV        834
#define DCCCXXXV         835
#define DCCCXXXVI        836
#define DCCCXXXVII       837
#define DCCCXXXVIII      838
#define DCCCXXXIX        839
#define DCCCXL           840
#define DCCCXLI          841
#define DCCCXLII         842
#define DCCCXLIII        843
#define DCCCXLIV         844
#define DCCCXLV          845
#define DCCCXLVI         846
#define DCCCXLVII        847
#define DCCCXLVIII       848
#define DCCCXLIX         849
#define DCCCL            850
#define DCCCLI           851
#define DCCCLII          852
#define DCCCLIII         853
#define DCCCLIV          854
#define DCCCLV           855
#define DCCCLVI          856
#define DCCCLVII         857
#define DCCCLVIII        858
#define DCCCLIX          859
#define DCCCLX           860
#define DCCCLXI          861
#define DCCCLXII         862
#define DCCCLXIII        863
#define DCCCLXIV         864
#define DCCCLXV          865
#define DCCCLXVI         866
#define DCCCLXVII        867
#define DCCCLXVIII       868
#define DCCCLXIX         869
#define DCCCLXX          870
#define DCCCLXXI         871
#define DCCCLXXII        872
#define DCCCLXXIII       873
#define DCCCLXXIV        874
#define DCCCLXXV         875
#define DCCCLXXVI        876
#define DCCCLXXVII       877
#define DCCCLXXVIII      878
#define DCCCLXXIX        879
#define DCCCLXXX         880
#define DCCCLXXXI        881
#define DCCCLXXXII       882
#define DCCCLXXXIII      883
#define DCCCLXXXIV       884
#define DCCCLXXXV        885
#define DCCCLXXXVI       886
#define DCCCLXXXVII      887
#define DCCCLXXXVIII     888
#define DCCCLXXXIX       889
#define DCCCXC           890
#define DCCCXCI          891
#define DCCCXCII         892
#define DCCCXCIII        893
#define DCCCXCIV         894
#define DCCCXCV          895
#define DCCCXCVI         896
#define DCCCXCVII        897
#define DCCCXCVIII       898
#define DCCCXCIX         899
#define CM               900
#define CMI              901
#define CMII             902
#define CMIII            903
#define CMIV             904
#define CMV              905
#define CMVI             906
#define CMVII            907
#define CMVIII           908
#define CMIX             909
#define CMX              910
#define CMXI             911
#define CMXII            912
#define CMXIII           913
#define CMXIV            914
#define CMXV             915
#define CMXVI            916
#define CMXVII           917
#define CMXVIII          918
#define CMXIX            919
#define CMXX             920
#define CMXXI            921
#define CMXXII           922
#define CMXXIII          923
#define CMXXIV           924
#define CMXXV            925
#define CMXXVI           926
#define CMXXVII          927
#define CMXXVIII         928
#define CMXXIX           929
#define CMXXX            930
#define CMXXXI           931
#define CMXXXII          932
#define CMXXXIII         933
#define CMXXXIV          934
#define CMXXXV           935
#define CMXXXVI          936
#define CMXXXVII         937
#define CMXXXVIII        938
#define CMXXXIX          939
#define CMXL             940
#define CMXLI            941
#define CMXLII           942
#define CMXLIII          943
#define CMXLIV           944
#define CMXLV            945
#define CMXLVI           946
#define CMXLVII          947
#define CMXLVIII         948
#define CMXLIX           949
#define CML              950
#define CMLI             951
#define CMLII            952
#define CMLIII           953
#define CMLIV            954
#define CMLV             955
#define CMLVI            956
#define CMLVII           957
#define CMLVIII          958
#define CMLIX            959
#define CMLX             960
#define CMLXI            961
#define CMLXII           962
#define CMLXIII          963
#define CMLXIV           964
#define CMLXV            965
#define CMLXVI           966
#define CMLXVII          967
#define CMLXVIII         968
#define CMLXIX           969
#define CMLXX            970
#define CMLXXI           971
#define CMLXXII          972
#define CMLXXIII         973
#define CMLXXIV          974
#define CMLXXV           975
#define CMLXXVI          976
#define CMLXXVII         977
#define CMLXXVIII        978
#define CMLXXIX          979
#define CMLXXX           980
#define CMLXXXI          981
#define CMLXXXII         982
#define CMLXXXIII        983
#define CMLXXXIV         984
#define CMLXXXV          985
#define CMLXXXVI         986
#define CMLXXXVII        987
#define CMLXXXVIII       988
#define CMLXXXIX         989
#define CMXC             990
#define CMXCI            991
#define CMXCII           992
#define CMXCIII          993
#define CMXCIV           994
#define CMXCV            995
#define CMXCVI           996
#define CMXCVII          997
#define CMXCVIII         998
#define CMXCIX           999
#define M                1000
#define MI               1001
#define MII              1002
#define MIII             1003
#define MIV              1004
#define MV               1005
#define MVI              1006
#define MVII             1007
#define MVIII            1008
#define MIX              1009
#define MX               1010
#define MXI              1011
#define MXII             1012
#define MXIII            1013
#define MXIV             1014
#define MXV              1015
#define MXVI             1016
#define MXVII            1017
#define MXVIII           1018
#define MXIX             1019
#define MXX              1020
#define MXXI             1021
#define MXXII            1022
#define MXXIII           1023
#define MXXIV            1024
#define MXXV             1025
#define MXXVI            1026
#define MXXVII           1027
#define MXXVIII          1028
#define MXXIX            1029
#define MXXX             1030
#define MXXXI            1031
#define MXXXII           1032
#define MXXXIII          1033
#define MXXXIV           1034
#define MXXXV            1035
#define MXXXVI           1036
#define MXXXVII          1037
#define MXXXVIII         1038
#define MXXXIX           1039
#define MXL              1040
#define MXLI             1041
#define MXLII            1042
#define MXLIII           1043
#define MXLIV            1044
#define MXLV             1045
#define MXLVI            1046
#define MXLVII           1047
#define MXLVIII          1048
#define MXLIX            1049
#define ML               1050
#define MLI              1051
#define MLII             1052
#define MLIII            1053
#define MLIV             1054
#define MLV              1055
#define MLVI             1056
#define MLVII            1057
#define MLVIII           1058
#define MLIX             1059
#define MLX              1060
#define MLXI             1061
#define MLXII            1062
#define MLXIII           1063
#define MLXIV            1064
#define MLXV             1065
#define MLXVI            1066
#define MLXVII           1067
#define MLXVIII          1068
#define MLXIX            1069
#define MLXX             1070
#define MLXXI            1071
#define MLXXII           1072
#define MLXXIII          1073
#define MLXXIV           1074
#define MLXXV            1075
#define MLXXVI           1076
#define MLXXVII          1077
#define MLXXVIII         1078
#define MLXXIX           1079
#define MLXXX            1080
#define MLXXXI           1081
#define MLXXXII          1082
#define MLXXXIII         1083
#define MLXXXIV          1084
#define MLXXXV           1085
#define MLXXXVI          1086
#define MLXXXVII         1087
#define MLXXXVIII        1088
#define MLXXXIX          1089
#define MXC              1090
#define MXCI             1091
#define MXCII            1092
#define MXCIII           1093
#define MXCIV            1094
#define MXCV             1095
#define MXCVI            1096
#define MXCVII           1097
#define MXCVIII          1098
#define MXCIX            1099
#define MC               1100
#define MCI              1101
#define MCII             1102
#define MCIII            1103
#define MCIV             1104
#define MCV              1105
#define MCVI             1106
#define MCVII            1107
#define MCVIII           1108
#define MCIX             1109
#define MCX              1110
#define MCXI             1111
#define MCXII            1112
#define MCXIII           1113
#define MCXIV            1114
#define MCXV             1115
#define MCXVI            1116
#define MCXVII           1117
#define MCXVIII          1118
#define MCXIX            1119
#define MCXX             1120
#define MCXXI            1121
#define MCXXII           1122
#define MCXXIII          1123
#define MCXXIV           1124
#define MCXXV            1125
#define MCXXVI           1126
#define MCXXVII          1127
#define MCXXVIII         1128
#define MCXXIX           1129
#define MCXXX            1130
#define MCXXXI           1131
#define MCXXXII          1132
#define MCXXXIII         1133
#define MCXXXIV          1134
#define MCXXXV           1135
#define MCXXXVI          1136
#define MCXXXVII         1137
#define MCXXXVIII        1138
#define MCXXXIX          1139
#define MCXL             1140
#define MCXLI            1141
#define MCXLII           1142
#define MCXLIII          1143
#define MCXLIV           1144
#define MCXLV            1145
#define MCXLVI           1146
#define MCXLVII          1147
#define MCXLVIII         1148
#define MCXLIX           1149
#define MCL              1150
#define MCLI             1151
#define MCLII            1152
#define MCLIII           1153
#define MCLIV            1154
#define MCLV             1155
#define MCLVI            1156
#define MCLVII           1157
#define MCLVIII          1158
#define MCLIX            1159
#define MCLX             1160
#define MCLXI            1161
#define MCLXII           1162
#define MCLXIII          1163
#define MCLXIV           1164
#define MCLXV            1165
#define MCLXVI           1166
#define MCLXVII          1167
#define MCLXVIII         1168
#define MCLXIX           1169
#define MCLXX            1170
#define MCLXXI           1171
#define MCLXXII          1172
#define MCLXXIII         1173
#define MCLXXIV          1174
#define MCLXXV           1175
#define MCLXXVI          1176
#define MCLXXVII         1177
#define MCLXXVIII        1178
#define MCLXXIX          1179
#define MCLXXX           1180
#define MCLXXXI          1181
#define MCLXXXII         1182
#define MCLXXXIII        1183
#define MCLXXXIV         1184
#define MCLXXXV          1185
#define MCLXXXVI         1186
#define MCLXXXVII        1187
#define MCLXXXVIII       1188
#define MCLXXXIX         1189
#define MCXC             1190
#define MCXCI            1191
#define MCXCII           1192
#define MCXCIII          1193
#define MCXCIV           1194
#define MCXCV            1195
#define MCXCVI           1196
#define MCXCVII          1197
#define MCXCVIII         1198
#define MCXCIX           1199
#define MCC              1200
#define MCCI             1201
#define MCCII            1202
#define MCCIII           1203
#define MCCIV            1204
#define MCCV             1205
#define MCCVI            1206
#define MCCVII           1207
#define MCCVIII          1208
#define MCCIX            1209
#define MCCX             1210
#define MCCXI            1211
#define MCCXII           1212
#define MCCXIII          1213
#define MCCXIV           1214
#define MCCXV            1215
#define MCCXVI           1216
#define MCCXVII          1217
#define MCCXVIII         1218
#define MCCXIX           1219
#define MCCXX            1220
#define MCCXXI           1221
#define MCCXXII          1222
#define MCCXXIII         1223
#define MCCXXIV          1224
#define MCCXXV           1225
#define MCCXXVI          1226
#define MCCXXVII         1227
#define MCCXXVIII        1228
#define MCCXXIX          1229
#define MCCXXX           1230
#define MCCXXXI          1231
#define MCCXXXII         1232
#define MCCXXXIII        1233
#define MCCXXXIV         1234
#define MCCXXXV          1235
#define MCCXXXVI         1236
#define MCCXXXVII        1237
#define MCCXXXVIII       1238
#define MCCXXXIX         1239
#define MCCXL            1240
#define MCCXLI           1241
#define MCCXLII          1242
#define MCCXLIII         1243
#define MCCXLIV          1244
#define MCCXLV           1245
#define MCCXLVI          1246
#define MCCXLVII         1247
#define MCCXLVIII        1248
#define MCCXLIX          1249
#define MCCL             1250
#define MCCLI            1251
#define MCCLII           1252
#define MCCLIII          1253
#define MCCLIV           1254
#define MCCLV            1255
#define MCCLVI           1256
#define MCCLVII          1257
#define MCCLVIII         1258
#define MCCLIX           1259
#define MCCLX            1260
#define MCCLXI           1261
#define MCCLXII          1262
#define MCCLXIII         1263
#define MCCLXIV          1264
#define MCCLXV           1265
#define MCCLXVI          1266
#define MCCLXVII         1267
#define MCCLXVIII        1268
#define MCCLXIX          1269
#define MCCLXX           1270
#define MCCLXXI          1271
#define MCCLXXII         1272
#define MCCLXXIII        1273
#define MCCLXXIV         1274
#define MCCLXXV          1275
#define MCCLXXVI         1276
#define MCCLXXVII        1277
#define MCCLXXVIII       1278
#define MCCLXXIX         1279
#define MCCLXXX          1280
#define MCCLXXXI         1281
#define MCCLXXXII        1282
#define MCCLXXXIII       1283
#define MCCLXXXIV        1284
#define MCCLXXXV         1285
#define MCCLXXXVI        1286
#define MCCLXXXVII       1287
#define MCCLXXXVIII      1288
#define MCCLXXXIX        1289
#define MCCXC            1290
#define MCCXCI           1291
#define MCCXCII          1292
#define MCCXCIII         1293
#define MCCXCIV          1294
#define MCCXCV           1295
#define MCCXCVI          1296
#define MCCXCVII         1297
#define MCCXCVIII        1298
#define MCCXCIX          1299
#define MCCC             1300
#define MCCCI            1301
#define MCCCII           1302
#define MCCCIII          1303
#define MCCCIV           1304
#define MCCCV            1305
#define MCCCVI           1306
#define MCCCVII          1307
#define MCCCVIII         1308
#define MCCCIX           1309
#define MCCCX            1310
#define MCCCXI           1311
#define MCCCXII          1312
#define MCCCXIII         1313
#define MCCCXIV          1314
#define MCCCXV           1315
#define MCCCXVI          1316
#define MCCCXVII         1317
#define MCCCXVIII        1318
#define MCCCXIX          1319
#define MCCCXX           1320
#define MCCCXXI          1321
#define MCCCXXII         1322
#define MCCCXXIII        1323
#define MCCCXXIV         1324
#define MCCCXXV          1325
#define MCCCXXVI         1326
#define MCCCXXVII        1327
#define MCCCXXVIII       1328
#define MCCCXXIX         1329
#define MCCCXXX          1330
#define MCCCXXXI         1331
#define MCCCXXXII        1332
#define MCCCXXXIII       1333
#define MCCCXXXIV        1334
#define MCCCXXXV         1335
#define MCCCXXXVI        1336
#define MCCCXXXVII       1337
#define MCCCXXXVIII      1338
#define MCCCXXXIX        1339
#define MCCCXL           1340
#define MCCCXLI          1341
#define MCCCXLII         1342
#define MCCCXLIII        1343
#define MCCCXLIV         1344
#define MCCCXLV          1345
#define MCCCXLVI         1346
#define MCCCXLVII        1347
#define MCCCXLVIII       1348
#define MCCCXLIX         1349
#define MCCCL            1350
#define MCCCLI           1351
#define MCCCLII          1352
#define MCCCLIII         1353
#define MCCCLIV          1354
#define MCCCLV           1355
#define MCCCLVI          1356
#define MCCCLVII         1357
#define MCCCLVIII        1358
#define MCCCLIX          1359
#define MCCCLX           1360
#define MCCCLXI          1361
#define MCCCLXII         1362
#define MCCCLXIII        1363
#define MCCCLXIV         1364
#define MCCCLXV          1365
#define MCCCLXVI         1366
#define MCCCLXVII        1367
#define MCCCLXVIII       1368
#define MCCCLXIX         1369
#define MCCCLXX          1370
#define MCCCLXXI         1371
#define MCCCLXXII        1372
#define MCCCLXXIII       1373
#define MCCCLXXIV        1374
#define MCCCLXXV         1375
#define MCCCLXXVI        1376
#define MCCCLXXVII       1377
#define MCCCLXXVIII      1378
#define MCCCLXXIX        1379
#define MCCCLXXX         1380
#define MCCCLXXXI        1381
#define MCCCLXXXII       1382
#define MCCCLXXXIII      1383
#define MCCCLXXXIV       1384
#define MCCCLXXXV        1385
#define MCCCLXXXVI       1386
#define MCCCLXXXVII      1387
#define MCCCLXXXVIII     1388
#define MCCCLXXXIX       1389
#define MCCCXC           1390
#define MCCCXCI          1391
#define MCCCXCII         1392
#define MCCCXCIII        1393
#define MCCCXCIV         1394
#define MCCCXCV          1395
#define MCCCXCVI         1396
#define MCCCXCVII        1397
#define MCCCXCVIII       1398
#define MCCCXCIX         1399
#define MCD              1400
#define MCDI             1401
#define MCDII            1402
#define MCDIII           1403
#define MCDIV            1404
#define MCDV             1405
#define MCDVI            1406
#define MCDVII           1407
#define MCDVIII          1408
#define MCDIX            1409
#define MCDX             1410
#define MCDXI            1411
#define MCDXII           1412
#define MCDXIII          1413
#define MCDXIV           1414
#define MCDXV            1415
#define MCDXVI           1416
#define MCDXVII          1417
#define MCDXVIII         1418
#define MCDXIX           1419
#define MCDXX            1420
#define MCDXXI           1421
#define MCDXXII          1422
#define MCDXXIII         1423
#define MCDXXIV          1424
#define MCDXXV           1425
#define MCDXXVI          1426
#define MCDXXVII         1427
#define MCDXXVIII        1428
#define MCDXXIX          1429
#define MCDXXX           1430
#define MCDXXXI          1431
#define MCDXXXII         1432
#define MCDXXXIII        1433
#define MCDXXXIV         1434
#define MCDXXXV          1435
#define MCDXXXVI         1436
#define MCDXXXVII        1437
#define MCDXXXVIII       1438
#define MCDXXXIX         1439
#define MCDXL            1440
#define MCDXLI           1441
#define MCDXLII          1442
#define MCDXLIII         1443
#define MCDXLIV          1444
#define MCDXLV           1445
#define MCDXLVI          1446
#define MCDXLVII         1447
#define MCDXLVIII        1448
#define MCDXLIX          1449
#define MCDL             1450
#define MCDLI            1451
#define MCDLII           1452
#define MCDLIII          1453
#define MCDLIV           1454
#define MCDLV            1455
#define MCDLVI           1456
#define MCDLVII          1457
#define MCDLVIII         1458
#define MCDLIX           1459
#define MCDLX            1460
#define MCDLXI           1461
#define MCDLXII          1462
#define MCDLXIII         1463
#define MCDLXIV          1464
#define MCDLXV           1465
#define MCDLXVI          1466
#define MCDLXVII         1467
#define MCDLXVIII        1468
#define MCDLXIX          1469
#define MCDLXX           1470
#define MCDLXXI          1471
#define MCDLXXII         1472
#define MCDLXXIII        1473
#define MCDLXXIV         1474
#define MCDLXXV          1475
#define MCDLXXVI         1476
#define MCDLXXVII        1477
#define MCDLXXVIII       1478
#define MCDLXXIX         1479
#define MCDLXXX          1480
#define MCDLXXXI         1481
#define MCDLXXXII        1482
#define MCDLXXXIII       1483
#define MCDLXXXIV        1484
#define MCDLXXXV         1485
#define MCDLXXXVI        1486
#define MCDLXXXVII       1487
#define MCDLXXXVIII      1488
#define MCDLXXXIX        1489
#define MCDXC            1490
#define MCDXCI           1491
#define MCDXCII          1492
#define MCDXCIII         1493
#define MCDXCIV          1494
#define MCDXCV           1495
#define MCDXCVI          1496
#define MCDXCVII         1497
#define MCDXCVIII        1498
#define MCDXCIX          1499
#define MD               1500
#define MDI              1501
#define MDII             1502
#define MDIII            1503
#define MDIV             1504
#define MDV              1505
#define MDVI             1506
#define MDVII            1507
#define MDVIII           1508
#define MDIX             1509
#define MDX              1510
#define MDXI             1511
#define MDXII            1512
#define MDXIII           1513
#define MDXIV            1514
#define MDXV             1515
#define MDXVI            1516
#define MDXVII           1517
#define MDXVIII          1518
#define MDXIX            1519
#define MDXX             1520
#define MDXXI            1521
#define MDXXII           1522
#define MDXXIII          1523
#define MDXXIV           1524
#define MDXXV            1525
#define MDXXVI           1526
#define MDXXVII          1527
#define MDXXVIII         1528
#define MDXXIX           1529
#define MDXXX            1530
#define MDXXXI           1531
#define MDXXXII          1532
#define MDXXXIII         1533
#define MDXXXIV          1534
#define MDXXXV           1535
#define MDXXXVI          1536
#define MDXXXVII         1537
#define MDXXXVIII        1538
#define MDXXXIX          1539
#define MDXL             1540
#define MDXLI            1541
#define MDXLII           1542
#define MDXLIII          1543
#define MDXLIV           1544
#define MDXLV            1545
#define MDXLVI           1546
#define MDXLVII          1547
#define MDXLVIII         1548
#define MDXLIX           1549
#define MDL              1550
#define MDLI             1551
#define MDLII            1552
#define MDLIII           1553
#define MDLIV            1554
#define MDLV             1555
#define MDLVI            1556
#define MDLVII           1557
#define MDLVIII          1558
#define MDLIX            1559
#define MDLX             1560
#define MDLXI            1561
#define MDLXII           1562
#define MDLXIII          1563
#define MDLXIV           1564
#define MDLXV            1565
#define MDLXVI           1566
#define MDLXVII          1567
#define MDLXVIII         1568
#define MDLXIX           1569
#define MDLXX            1570
#define MDLXXI           1571
#define MDLXXII          1572
#define MDLXXIII         1573
#define MDLXXIV          1574
#define MDLXXV           1575
#define MDLXXVI          1576
#define MDLXXVII         1577
#define MDLXXVIII        1578
#define MDLXXIX          1579
#define MDLXXX           1580
#define MDLXXXI          1581
#define MDLXXXII         1582
#define MDLXXXIII        1583
#define MDLXXXIV         1584
#define MDLXXXV          1585
#define MDLXXXVI         1586
#define MDLXXXVII        1587
#define MDLXXXVIII       1588
#define MDLXXXIX         1589
#define MDXC             1590
#define MDXCI            1591
#define MDXCII           1592
#define MDXCIII          1593
#define MDXCIV           1594
#define MDXCV            1595
#define MDXCVI           1596
#define MDXCVII          1597
#define MDXCVIII         1598
#define MDXCIX           1599
#define MDC              1600
#define MDCI             1601
#define MDCII            1602
#define MDCIII           1603
#define MDCIV            1604
#define MDCV             1605
#define MDCVI            1606
#define MDCVII           1607
#define MDCVIII          1608
#define MDCIX            1609
#define MDCX             1610
#define MDCXI            1611
#define MDCXII           1612
#define MDCXIII          1613
#define MDCXIV           1614
#define MDCXV            1615
#define MDCXVI           1616
#define MDCXVII          1617
#define MDCXVIII         1618
#define MDCXIX           1619
#define MDCXX            1620
#define MDCXXI           1621
#define MDCXXII          1622
#define MDCXXIII         1623
#define MDCXXIV          1624
#define MDCXXV           1625
#define MDCXXVI          1626
#define MDCXXVII         1627
#define MDCXXVIII        1628
#define MDCXXIX          1629
#define MDCXXX           1630
#define MDCXXXI          1631
#define MDCXXXII         1632
#define MDCXXXIII        1633
#define MDCXXXIV         1634
#define MDCXXXV          1635
#define MDCXXXVI         1636
#define MDCXXXVII        1637
#define MDCXXXVIII       1638
#define MDCXXXIX         1639
#define MDCXL            1640
#define MDCXLI           1641
#define MDCXLII          1642
#define MDCXLIII         1643
#define MDCXLIV          1644
#define MDCXLV           1645
#define MDCXLVI          1646
#define MDCXLVII         1647
#define MDCXLVIII        1648
#define MDCXLIX          1649
#define MDCL             1650
#define MDCLI            1651
#define MDCLII           1652
#define MDCLIII          1653
#define MDCLIV           1654
#define MDCLV            1655
#define MDCLVI           1656
#define MDCLVII          1657
#define MDCLVIII         1658
#define MDCLIX           1659
#define MDCLX            1660
#define MDCLXI           1661
#define MDCLXII          1662
#define MDCLXIII         1663
#define MDCLXIV          1664
#define MDCLXV           1665
#define MDCLXVI          1666
#define MDCLXVII         1667
#define MDCLXVIII        1668
#define MDCLXIX          1669
#define MDCLXX           1670
#define MDCLXXI          1671
#define MDCLXXII         1672
#define MDCLXXIII        1673
#define MDCLXXIV         1674
#define MDCLXXV          1675
#define MDCLXXVI         1676
#define MDCLXXVII        1677
#define MDCLXXVIII       1678
#define MDCLXXIX         1679
#define MDCLXXX          1680
#define MDCLXXXI         1681
#define MDCLXXXII        1682
#define MDCLXXXIII       1683
#define MDCLXXXIV        1684
#define MDCLXXXV         1685
#define MDCLXXXVI        1686
#define MDCLXXXVII       1687
#define MDCLXXXVIII      1688
#define MDCLXXXIX        1689
#define MDCXC            1690
#define MDCXCI           1691
#define MDCXCII          1692
#define MDCXCIII         1693
#define MDCXCIV          1694
#define MDCXCV           1695
#define MDCXCVI          1696
#define MDCXCVII         1697
#define MDCXCVIII        1698
#define MDCXCIX          1699
#define MDCC             1700
#define MDCCI            1701
#define MDCCII           1702
#define MDCCIII          1703
#define MDCCIV           1704
#define MDCCV            1705
#define MDCCVI           1706
#define MDCCVII          1707
#define MDCCVIII         1708
#define MDCCIX           1709
#define MDCCX            1710
#define MDCCXI           1711
#define MDCCXII          1712
#define MDCCXIII         1713
#define MDCCXIV          1714
#define MDCCXV           1715
#define MDCCXVI          1716
#define MDCCXVII         1717
#define MDCCXVIII        1718
#define MDCCXIX          1719
#define MDCCXX           1720
#define MDCCXXI          1721
#define MDCCXXII         1722
#define MDCCXXIII        1723
#define MDCCXXIV         1724
#define MDCCXXV          1725
#define MDCCXXVI         1726
#define MDCCXXVII        1727
#define MDCCXXVIII       1728
#define MDCCXXIX         1729
#define MDCCXXX          1730
#define MDCCXXXI         1731
#define MDCCXXXII        1732
#define MDCCXXXIII       1733
#define MDCCXXXIV        1734
#define MDCCXXXV         1735
#define MDCCXXXVI        1736
#define MDCCXXXVII       1737
#define MDCCXXXVIII      1738
#define MDCCXXXIX        1739
#define MDCCXL           1740
#define MDCCXLI          1741
#define MDCCXLII         1742
#define MDCCXLIII        1743
#define MDCCXLIV         1744
#define MDCCXLV          1745
#define MDCCXLVI         1746
#define MDCCXLVII        1747
#define MDCCXLVIII       1748
#define MDCCXLIX         1749
#define MDCCL            1750
#define MDCCLI           1751
#define MDCCLII          1752
#define MDCCLIII         1753
#define MDCCLIV          1754
#define MDCCLV           1755
#define MDCCLVI          1756
#define MDCCLVII         1757
#define MDCCLVIII        1758
#define MDCCLIX          1759
#define MDCCLX           1760
#define MDCCLXI          1761
#define MDCCLXII         1762
#define MDCCLXIII        1763
#define MDCCLXIV         1764
#define MDCCLXV          1765
#define MDCCLXVI         1766
#define MDCCLXVII        1767
#define MDCCLXVIII       1768
#define MDCCLXIX         1769
#define MDCCLXX          1770
#define MDCCLXXI         1771
#define MDCCLXXII        1772
#define MDCCLXXIII       1773
#define MDCCLXXIV        1774
#define MDCCLXXV         1775
#define MDCCLXXVI        1776
#define MDCCLXXVII       1777
#define MDCCLXXVIII      1778
#define MDCCLXXIX        1779
#define MDCCLXXX         1780
#define MDCCLXXXI        1781
#define MDCCLXXXII       1782
#define MDCCLXXXIII      1783
#define MDCCLXXXIV       1784
#define MDCCLXXXV        1785
#define MDCCLXXXVI       1786
#define MDCCLXXXVII      1787
#define MDCCLXXXVIII     1788
#define MDCCLXXXIX       1789
#define MDCCXC           1790
#define MDCCXCI          1791
#define MDCCXCII         1792
#define MDCCXCIII        1793
#define MDCCXCIV         1794
#define MDCCXCV          1795
#define MDCCXCVI         1796
#define MDCCXCVII        1797
#define MDCCXCVIII       1798
#define MDCCXCIX         1799
#define MDCCC            1800
#define MDCCCI           1801
#define MDCCCII          1802
#define MDCCCIII         1803
#define MDCCCIV          1804
#define MDCCCV           1805
#define MDCCCVI          1806
#define MDCCCVII         1807
#define MDCCCVIII        1808
#define MDCCCIX          1809
#define MDCCCX           1810
#define MDCCCXI          1811
#define MDCCCXII         1812
#define MDCCCXIII        1813
#define MDCCCXIV         1814
#define MDCCCXV          1815
#define MDCCCXVI         1816
#define MDCCCXVII        1817
#define MDCCCXVIII       1818
#define MDCCCXIX         1819
#define MDCCCXX          1820
#define MDCCCXXI         1821
#define MDCCCXXII        1822
#define MDCCCXXIII       1823
#define MDCCCXXIV        1824
#define MDCCCXXV         1825
#define MDCCCXXVI        1826
#define MDCCCXXVII       1827
#define MDCCCXXVIII      1828
#define MDCCCXXIX        1829
#define MDCCCXXX         1830
#define MDCCCXXXI        1831
#define MDCCCXXXII       1832
#define MDCCCXXXIII      1833
#define MDCCCXXXIV       1834
#define MDCCCXXXV        1835
#define MDCCCXXXVI       1836
#define MDCCCXXXVII      1837
#define MDCCCXXXVIII     1838
#define MDCCCXXXIX       1839
#define MDCCCXL          1840
#define MDCCCXLI         1841
#define MDCCCXLII        1842
#define MDCCCXLIII       1843
#define MDCCCXLIV        1844
#define MDCCCXLV         1845
#define MDCCCXLVI        1846
#define MDCCCXLVII       1847
#define MDCCCXLVIII      1848
#define MDCCCXLIX        1849
#define MDCCCL           1850
#define MDCCCLI          1851
#define MDCCCLII         1852
#define MDCCCLIII        1853
#define MDCCCLIV         1854
#define MDCCCLV          1855
#define MDCCCLVI         1856
#define MDCCCLVII        1857
#define MDCCCLVIII       1858
#define MDCCCLIX         1859
#define MDCCCLX          1860
#define MDCCCLXI         1861
#define MDCCCLXII        1862
#define MDCCCLXIII       1863
#define MDCCCLXIV        1864
#define MDCCCLXV         1865
#define MDCCCLXVI        1866
#define MDCCCLXVII       1867
#define MDCCCLXVIII      1868
#define MDCCCLXIX        1869
#define MDCCCLXX         1870
#define MDCCCLXXI        1871
#define MDCCCLXXII       1872
#define MDCCCLXXIII      1873
#define MDCCCLXXIV       1874
#define MDCCCLXXV        1875
#define MDCCCLXXVI       1876
#define MDCCCLXXVII      1877
#define MDCCCLXXVIII     1878
#define MDCCCLXXIX       1879
#define MDCCCLXXX        1880
#define MDCCCLXXXI       1881
#define MDCCCLXXXII      1882
#define MDCCCLXXXIII     1883
#define MDCCCLXXXIV      1884
#define MDCCCLXXXV       1885
#define MDCCCLXXXVI      1886
#define MDCCCLXXXVII     1887
#define MDCCCLXXXVIII    1888
#define MDCCCLXXXIX      1889
#define MDCCCXC          1890
#define MDCCCXCI         1891
#define MDCCCXCII        1892
#define MDCCCXCIII       1893
#define MDCCCXCIV        1894
#define MDCCCXCV         1895
#define MDCCCXCVI        1896
#define MDCCCXCVII       1897
#define MDCCCXCVIII      1898
#define MDCCCXCIX        1899
#define MCM              1900
#define MCMI             1901
#define MCMII            1902
#define MCMIII           1903
#define MCMIV            1904
#define MCMV             1905
#define MCMVI            1906
#define MCMVII           1907
#define MCMVIII          1908
#define MCMIX            1909
#define MCMX             1910
#define MCMXI            1911
#define MCMXII           1912
#define MCMXIII          1913
#define MCMXIV           1914
#define MCMXV            1915
#define MCMXVI           1916
#define MCMXVII          1917
#define MCMXVIII         1918
#define MCMXIX           1919
#define MCMXX            1920
#define MCMXXI           1921
#define MCMXXII          1922
#define MCMXXIII         1923
#define MCMXXIV          1924
#define MCMXXV           1925
#define MCMXXVI          1926
#define MCMXXVII         1927
#define MCMXXVIII        1928
#define MCMXXIX          1929
#define MCMXXX           1930
#define MCMXXXI          1931
#define MCMXXXII         1932
#define MCMXXXIII        1933
#define MCMXXXIV         1934
#define MCMXXXV          1935
#define MCMXXXVI         1936
#define MCMXXXVII        1937
#define MCMXXXVIII       1938
#define MCMXXXIX         1939
#define MCMXL            1940
#define MCMXLI           1941
#define MCMXLII          1942
#define MCMXLIII         1943
#define MCMXLIV          1944
#define MCMXLV           1945
#define MCMXLVI          1946
#define MCMXLVII         1947
#define MCMXLVIII        1948
#define MCMXLIX          1949
#define MCML             1950
#define MCMLI            1951
#define MCMLII           1952
#define MCMLIII          1953
#define MCMLIV           1954
#define MCMLV            1955
#define MCMLVI           1956
#define MCMLVII          1957
#define MCMLVIII         1958
#define MCMLIX           1959
#define MCMLX            1960
#define MCMLXI           1961
#define MCMLXII          1962
#define MCMLXIII         1963
#define MCMLXIV          1964
#define MCMLXV           1965
#define MCMLXVI          1966
#define MCMLXVII         1967
#define MCMLXVIII        1968
#define MCMLXIX          1969
#define MCMLXX           1970
#define MCMLXXI          1971
#define MCMLXXII         1972
#define MCMLXXIII        1973
#define MCMLXXIV         1974
#define MCMLXXV          1975
#define MCMLXXVI         1976
#define MCMLXXVII        1977
#define MCMLXXVIII       1978
#define MCMLXXIX         1979
#define MCMLXXX          1980
#define MCMLXXXI         1981
#define MCMLXXXII        1982
#define MCMLXXXIII       1983
#define MCMLXXXIV        1984
#define MCMLXXXV         1985
#define MCMLXXXVI        1986
#define MCMLXXXVII       1987
#define MCMLXXXVIII      1988
#define MCMLXXXIX        1989
#define MCMXC            1990
#define MCMXCI           1991
#define MCMXCII          1992
#define MCMXCIII         1993
#define MCMXCIV          1994
#define MCMXCV           1995
#define MCMXCVI          1996
#define MCMXCVII         1997
#define MCMXCVIII        1998
#define MCMXCIX          1999
#define MM               2000
#define MMI              2001
#define MMII             2002
#define MMIII            2003
#define MMIV             2004
#define MMV              2005
#define MMVI             2006
#define MMVII            2007
#define MMVIII           2008
#define MMIX             2009
#define MMX              2010
#define MMXI             2011
#define MMXII            2012
#define MMXIII           2013
#define MMXIV            2014
#define MMXV             2015
#define MMXVI            2016
#define MMXVII           2017
#define MMXVIII          2018
#define MMXIX            2019
#define MMXX             2020
#define MMXXI            2021
#define MMXXII           2022
#define MMXXIII          2023
#define MMXXIV           2024
#define MMXXV            2025
#define MMXXVI           2026
#define MMXXVII          2027
#define MMXXVIII         2028
#define MMXXIX           2029
#define MMXXX            2030
#define MMXXXI           2031
#define MMXXXII          2032
#define MMXXXIII         2033
#define MMXXXIV          2034
#define MMXXXV           2035
#define MMXXXVI          2036
#define MMXXXVII         2037
#define MMXXXVIII        2038
#define MMXXXIX          2039
#define MMXL             2040
#define MMXLI            2041
#define MMXLII           2042
#define MMXLIII          2043
#define MMXLIV           2044
#define MMXLV            2045
#define MMXLVI           2046
#define MMXLVII          2047
#define MMXLVIII         2048
#define MMXLIX           2049
#define MML              2050
#define MMLI             2051
#define MMLII            2052
#define MMLIII           2053
#define MMLIV            2054
#define MMLV             2055
#define MMLVI            2056
#define MMLVII           2057
#define MMLVIII          2058
#define MMLIX            2059
#define MMLX             2060
#define MMLXI            2061
#define MMLXII           2062
#define MMLXIII          2063
#define MMLXIV           2064
#define MMLXV            2065
#define MMLXVI           2066
#define MMLXVII          2067
#define MMLXVIII         2068
#define MMLXIX           2069
#define MMLXX            2070
#define MMLXXI           2071
#define MMLXXII          2072
#define MMLXXIII         2073
#define MMLXXIV          2074
#define MMLXXV           2075
#define MMLXXVI          2076
#define MMLXXVII         2077
#define MMLXXVIII        2078
#define MMLXXIX          2079
#define MMLXXX           2080
#define MMLXXXI          2081
#define MMLXXXII         2082
#define MMLXXXIII        2083
#define MMLXXXIV         2084
#define MMLXXXV          2085
#define MMLXXXVI         2086
#define MMLXXXVII        2087
#define MMLXXXVIII       2088
#define MMLXXXIX         2089
#define MMXC             2090
#define MMXCI            2091
#define MMXCII           2092
#define MMXCIII          2093
#define MMXCIV           2094
#define MMXCV            2095
#define MMXCVI           2096
#define MMXCVII          2097
#define MMXCVIII         2098
#define MMXCIX           2099
#define MMC              2100
#define MMCI             2101
#define MMCII            2102
#define MMCIII           2103
#define MMCIV            2104
#define MMCV             2105
#define MMCVI            2106
#define MMCVII           2107
#define MMCVIII          2108
#define MMCIX            2109
#define MMCX             2110
#define MMCXI            2111
#define MMCXII           2112
#define MMCXIII          2113
#define MMCXIV           2114
#define MMCXV            2115
#define MMCXVI           2116
#define MMCXVII          2117
#define MMCXVIII         2118
#define MMCXIX           2119
#define MMCXX            2120
#define MMCXXI           2121
#define MMCXXII          2122
#define MMCXXIII         2123
#define MMCXXIV          2124
#define MMCXXV           2125
#define MMCXXVI          2126
#define MMCXXVII         2127
#define MMCXXVIII        2128
#define MMCXXIX          2129
#define MMCXXX           2130
#define MMCXXXI          2131
#define MMCXXXII         2132
#define MMCXXXIII        2133
#define MMCXXXIV         2134
#define MMCXXXV          2135
#define MMCXXXVI         2136
#define MMCXXXVII        2137
#define MMCXXXVIII       2138
#define MMCXXXIX         2139
#define MMCXL            2140
#define MMCXLI           2141
#define MMCXLII          2142
#define MMCXLIII         2143
#define MMCXLIV          2144
#define MMCXLV           2145
#define MMCXLVI          2146
#define MMCXLVII         2147
#define MMCXLVIII        2148
#define MMCXLIX          2149
#define MMCL             2150
#define MMCLI            2151
#define MMCLII           2152
#define MMCLIII          2153
#define MMCLIV           2154
#define MMCLV            2155
#define MMCLVI           2156
#define MMCLVII          2157
#define MMCLVIII         2158
#define MMCLIX           2159
#define MMCLX            2160
#define MMCLXI           2161
#define MMCLXII          2162
#define MMCLXIII         2163
#define MMCLXIV          2164
#define MMCLXV           2165
#define MMCLXVI          2166
#define MMCLXVII         2167
#define MMCLXVIII        2168
#define MMCLXIX          2169
#define MMCLXX           2170
#define MMCLXXI          2171
#define MMCLXXII         2172
#define MMCLXXIII        2173
#define MMCLXXIV         2174
#define MMCLXXV          2175
#define MMCLXXVI         2176
#define MMCLXXVII        2177
#define MMCLXXVIII       2178
#define MMCLXXIX         2179
#define MMCLXXX          2180
#define MMCLXXXI         2181
#define MMCLXXXII        2182
#define MMCLXXXIII       2183
#define MMCLXXXIV        2184
#define MMCLXXXV         2185
#define MMCLXXXVI        2186
#define MMCLXXXVII       2187
#define MMCLXXXVIII      2188
#define MMCLXXXIX        2189
#define MMCXC            2190
#define MMCXCI           2191
#define MMCXCII          2192
#define MMCXCIII         2193
#define MMCXCIV          2194
#define MMCXCV           2195
#define MMCXCVI          2196
#define MMCXCVII         2197
#define MMCXCVIII        2198
#define MMCXCIX          2199
#define MMCC             2200
#define MMCCI            2201
#define MMCCII           2202
#define MMCCIII          2203
#define MMCCIV           2204
#define MMCCV            2205
#define MMCCVI           2206
#define MMCCVII          2207
#define MMCCVIII         2208
#define MMCCIX           2209
#define MMCCX            2210
#define MMCCXI           2211
#define MMCCXII          2212
#define MMCCXIII         2213
#define MMCCXIV          2214
#define MMCCXV           2215
#define MMCCXVI          2216
#define MMCCXVII         2217
#define MMCCXVIII        2218
#define MMCCXIX          2219
#define MMCCXX           2220
#define MMCCXXI          2221
#define MMCCXXII         2222
#define MMCCXXIII        2223
#define MMCCXXIV         2224
#define MMCCXXV          2225
#define MMCCXXVI         2226
#define MMCCXXVII        2227
#define MMCCXXVIII       2228
#define MMCCXXIX         2229
#define MMCCXXX          2230
#define MMCCXXXI         2231
#define MMCCXXXII        2232
#define MMCCXXXIII       2233
#define MMCCXXXIV        2234
#define MMCCXXXV         2235
#define MMCCXXXVI        2236
#define MMCCXXXVII       2237
#define MMCCXXXVIII      2238
#define MMCCXXXIX        2239
#define MMCCXL           2240
#define MMCCXLI          2241
#define MMCCXLII         2242
#define MMCCXLIII        2243
#define MMCCXLIV         2244
#define MMCCXLV          2245
#define MMCCXLVI         2246
#define MMCCXLVII        2247
#define MMCCXLVIII       2248
#define MMCCXLIX         2249
#define MMCCL            2250
#define MMCCLI           2251
#define MMCCLII          2252
#define MMCCLIII         2253
#define MMCCLIV          2254
#define MMCCLV           2255
#define MMCCLVI          2256
#define MMCCLVII         2257
#define MMCCLVIII        2258
#define MMCCLIX          2259
#define MMCCLX           2260
#define MMCCLXI          2261
#define MMCCLXII         2262
#define MMCCLXIII        2263
#define MMCCLXIV         2264
#define MMCCLXV          2265
#define MMCCLXVI         2266
#define MMCCLXVII        2267
#define MMCCLXVIII       2268
#define MMCCLXIX         2269
#define MMCCLXX          2270
#define MMCCLXXI         2271
#define MMCCLXXII        2272
#define MMCCLXXIII       2273
#define MMCCLXXIV        2274
#define MMCCLXXV         2275
#define MMCCLXXVI        2276
#define MMCCLXXVII       2277
#define MMCCLXXVIII      2278
#define MMCCLXXIX        2279
#define MMCCLXXX         2280
#define MMCCLXXXI        2281
#define MMCCLXXXII       2282
#define MMCCLXXXIII      2283
#define MMCCLXXXIV       2284
#define MMCCLXXXV        2285
#define MMCCLXXXVI       2286
#define MMCCLXXXVII      2287
#define MMCCLXXXVIII     2288
#define MMCCLXXXIX       2289
#define MMCCXC           2290
#define MMCCXCI          2291
#define MMCCXCII         2292
#define MMCCXCIII        2293
#define MMCCXCIV         2294
#define MMCCXCV          2295
#define MMCCXCVI         2296
#define MMCCXCVII        2297
#define MMCCXCVIII       2298
#define MMCCXCIX         2299
#define MMCCC            2300
#define MMCCCI           2301
#define MMCCCII          2302
#define MMCCCIII         2303
#define MMCCCIV          2304
#define MMCCCV           2305
#define MMCCCVI          2306
#define MMCCCVII         2307
#define MMCCCVIII        2308
#define MMCCCIX          2309
#define MMCCCX           2310
#define MMCCCXI          2311
#define MMCCCXII         2312
#define MMCCCXIII        2313
#define MMCCCXIV         2314
#define MMCCCXV          2315
#define MMCCCXVI         2316
#define MMCCCXVII        2317
#define MMCCCXVIII       2318
#define MMCCCXIX         2319
#define MMCCCXX          2320
#define MMCCCXXI         2321
#define MMCCCXXII        2322
#define MMCCCXXIII       2323
#define MMCCCXXIV        2324
#define MMCCCXXV         2325
#define MMCCCXXVI        2326
#define MMCCCXXVII       2327
#define MMCCCXXVIII      2328
#define MMCCCXXIX        2329
#define MMCCCXXX         2330
#define MMCCCXXXI        2331
#define MMCCCXXXII       2332
#define MMCCCXXXIII      2333
#define MMCCCXXXIV       2334
#define MMCCCXXXV        2335
#define MMCCCXXXVI       2336
#define MMCCCXXXVII      2337
#define MMCCCXXXVIII     2338
#define MMCCCXXXIX       2339
#define MMCCCXL          2340
#define MMCCCXLI         2341
#define MMCCCXLII        2342
#define MMCCCXLIII       2343
#define MMCCCXLIV        2344
#define MMCCCXLV         2345
#define MMCCCXLVI        2346
#define MMCCCXLVII       2347
#define MMCCCXLVIII      2348
#define MMCCCXLIX        2349
#define MMCCCL           2350
#define MMCCCLI          2351
#define MMCCCLII         2352
#define MMCCCLIII        2353
#define MMCCCLIV         2354
#define MMCCCLV          2355
#define MMCCCLVI         2356
#define MMCCCLVII        2357
#define MMCCCLVIII       2358
#define MMCCCLIX         2359
#define MMCCCLX          2360
#define MMCCCLXI         2361
#define MMCCCLXII        2362
#define MMCCCLXIII       2363
#define MMCCCLXIV        2364
#define MMCCCLXV         2365
#define MMCCCLXVI        2366
#define MMCCCLXVII       2367
#define MMCCCLXVIII      2368
#define MMCCCLXIX        2369
#define MMCCCLXX         2370
#define MMCCCLXXI        2371
#define MMCCCLXXII       2372
#define MMCCCLXXIII      2373
#define MMCCCLXXIV       2374
#define MMCCCLXXV        2375
#define MMCCCLXXVI       2376
#define MMCCCLXXVII      2377
#define MMCCCLXXVIII     2378
#define MMCCCLXXIX       2379
#define MMCCCLXXX        2380
#define MMCCCLXXXI       2381
#define MMCCCLXXXII      2382
#define MMCCCLXXXIII     2383
#define MMCCCLXXXIV      2384
#define MMCCCLXXXV       2385
#define MMCCCLXXXVI      2386
#define MMCCCLXXXVII     2387
#define MMCCCLXXXVIII    2388
#define MMCCCLXXXIX      2389
#define MMCCCXC          2390
#define MMCCCXCI         2391
#define MMCCCXCII        2392
#define MMCCCXCIII       2393
#define MMCCCXCIV        2394
#define MMCCCXCV         2395
#define MMCCCXCVI        2396
#define MMCCCXCVII       2397
#define MMCCCXCVIII      2398
#define MMCCCXCIX        2399
#define MMCD             2400
#define MMCDI            2401
#define MMCDII           2402
#define MMCDIII          2403
#define MMCDIV           2404
#define MMCDV            2405
#define MMCDVI           2406
#define MMCDVII          2407
#define MMCDVIII         2408
#define MMCDIX           2409
#define MMCDX            2410
#define MMCDXI           2411
#define MMCDXII          2412
#define MMCDXIII         2413
#define MMCDXIV          2414
#define MMCDXV           2415
#define MMCDXVI          2416
#define MMCDXVII         2417
#define MMCDXVIII        2418
#define MMCDXIX          2419
#define MMCDXX           2420
#define MMCDXXI          2421
#define MMCDXXII         2422
#define MMCDXXIII        2423
#define MMCDXXIV         2424
#define MMCDXXV          2425
#define MMCDXXVI         2426
#define MMCDXXVII        2427
#define MMCDXXVIII       2428
#define MMCDXXIX         2429
#define MMCDXXX          2430
#define MMCDXXXI         2431
#define MMCDXXXII        2432
#define MMCDXXXIII       2433
#define MMCDXXXIV        2434
#define MMCDXXXV         2435
#define MMCDXXXVI        2436
#define MMCDXXXVII       2437
#define MMCDXXXVIII      2438
#define MMCDXXXIX        2439
#define MMCDXL           2440
#define MMCDXLI          2441
#define MMCDXLII         2442
#define MMCDXLIII        2443
#define MMCDXLIV         2444
#define MMCDXLV          2445
#define MMCDXLVI         2446
#define MMCDXLVII        2447
#define MMCDXLVIII       2448
#define MMCDXLIX         2449
#define MMCDL            2450
#define MMCDLI           2451
#define MMCDLII          2452
#define MMCDLIII         2453
#define MMCDLIV          2454
#define MMCDLV           2455
#define MMCDLVI          2456
#define MMCDLVII         2457
#define MMCDLVIII        2458
#define MMCDLIX          2459
#define MMCDLX           2460
#define MMCDLXI          2461
#define MMCDLXII         2462
#define MMCDLXIII        2463
#define MMCDLXIV         2464
#define MMCDLXV          2465
#define MMCDLXVI         2466
#define MMCDLXVII        2467
#define MMCDLXVIII       2468
#define MMCDLXIX         2469
#define MMCDLXX          2470
#define MMCDLXXI         2471
#define MMCDLXXII        2472
#define MMCDLXXIII       2473
#define MMCDLXXIV        2474
#define MMCDLXXV         2475
#define MMCDLXXVI        2476
#define MMCDLXXVII       2477
#define MMCDLXXVIII      2478
#define MMCDLXXIX        2479
#define MMCDLXXX         2480
#define MMCDLXXXI        2481
#define MMCDLXXXII       2482
#define MMCDLXXXIII      2483
#define MMCDLXXXIV       2484
#define MMCDLXXXV        2485
#define MMCDLXXXVI       2486
#define MMCDLXXXVII      2487
#define MMCDLXXXVIII     2488
#define MMCDLXXXIX       2489
#define MMCDXC           2490
#define MMCDXCI          2491
#define MMCDXCII         2492
#define MMCDXCIII        2493
#define MMCDXCIV         2494
#define MMCDXCV          2495
#define MMCDXCVI         2496
#define MMCDXCVII        2497
#define MMCDXCVIII       2498
#define MMCDXCIX         2499
#define MMD              2500
#define MMDI             2501
#define MMDII            2502
#define MMDIII           2503
#define MMDIV            2504
#define MMDV             2505
#define MMDVI            2506
#define MMDVII           2507
#define MMDVIII          2508
#define MMDIX            2509
#define MMDX             2510
#define MMDXI            2511
#define MMDXII           2512
#define MMDXIII          2513
#define MMDXIV           2514
#define MMDXV            2515
#define MMDXVI           2516
#define MMDXVII          2517
#define MMDXVIII         2518
#define MMDXIX           2519
#define MMDXX            2520
#define MMDXXI           2521
#define MMDXXII          2522
#define MMDXXIII         2523
#define MMDXXIV          2524
#define MMDXXV           2525
#define MMDXXVI          2526
#define MMDXXVII         2527
#define MMDXXVIII        2528
#define MMDXXIX          2529
#define MMDXXX           2530
#define MMDXXXI          2531
#define MMDXXXII         2532
#define MMDXXXIII        2533
#define MMDXXXIV         2534
#define MMDXXXV          2535
#define MMDXXXVI         2536
#define MMDXXXVII        2537
#define MMDXXXVIII       2538
#define MMDXXXIX         2539
#define MMDXL            2540
#define MMDXLI           2541
#define MMDXLII          2542
#define MMDXLIII         2543
#define MMDXLIV          2544
#define MMDXLV           2545
#define MMDXLVI          2546
#define MMDXLVII         2547
#define MMDXLVIII        2548
#define MMDXLIX          2549
#define MMDL             2550
#define MMDLI            2551
#define MMDLII           2552
#define MMDLIII          2553
#define MMDLIV           2554
#define MMDLV            2555
#define MMDLVI           2556
#define MMDLVII          2557
#define MMDLVIII         2558
#define MMDLIX           2559
#define MMDLX            2560
#define MMDLXI           2561
#define MMDLXII          2562
#define MMDLXIII         2563
#define MMDLXIV          2564
#define MMDLXV           2565
#define MMDLXVI          2566
#define MMDLXVII         2567
#define MMDLXVIII        2568
#define MMDLXIX          2569
#define MMDLXX           2570
#define MMDLXXI          2571
#define MMDLXXII         2572
#define MMDLXXIII        2573
#define MMDLXXIV         2574
#define MMDLXXV          2575
#define MMDLXXVI         2576
#define MMDLXXVII        2577
#define MMDLXXVIII       2578
#define MMDLXXIX         2579
#define MMDLXXX          2580
#define MMDLXXXI         2581
#define MMDLXXXII        2582
#define MMDLXXXIII       2583
#define MMDLXXXIV        2584
#define MMDLXXXV         2585
#define MMDLXXXVI        2586
#define MMDLXXXVII       2587
#define MMDLXXXVIII      2588
#define MMDLXXXIX        2589
#define MMDXC            2590
#define MMDXCI           2591
#define MMDXCII          2592
#define MMDXCIII         2593
#define MMDXCIV          2594
#define MMDXCV           2595
#define MMDXCVI          2596
#define MMDXCVII         2597
#define MMDXCVIII        2598
#define MMDXCIX          2599
#define MMDC             2600
#define MMDCI            2601
#define MMDCII           2602
#define MMDCIII          2603
#define MMDCIV           2604
#define MMDCV            2605
#define MMDCVI           2606
#define MMDCVII          2607
#define MMDCVIII         2608
#define MMDCIX           2609
#define MMDCX            2610
#define MMDCXI           2611
#define MMDCXII          2612
#define MMDCXIII         2613
#define MMDCXIV          2614
#define MMDCXV           2615
#define MMDCXVI          2616
#define MMDCXVII         2617
#define MMDCXVIII        2618
#define MMDCXIX          2619
#define MMDCXX           2620
#define MMDCXXI          2621
#define MMDCXXII         2622
#define MMDCXXIII        2623
#define MMDCXXIV         2624
#define MMDCXXV          2625
#define MMDCXXVI         2626
#define MMDCXXVII        2627
#define MMDCXXVIII       2628
#define MMDCXXIX         2629
#define MMDCXXX          2630
#define MMDCXXXI         2631
#define MMDCXXXII        2632
#define MMDCXXXIII       2633
#define MMDCXXXIV        2634
#define MMDCXXXV         2635
#define MMDCXXXVI        2636
#define MMDCXXXVII       2637
#define MMDCXXXVIII      2638
#define MMDCXXXIX        2639
#define MMDCXL           2640
#define MMDCXLI          2641
#define MMDCXLII         2642
#define MMDCXLIII        2643
#define MMDCXLIV         2644
#define MMDCXLV          2645
#define MMDCXLVI         2646
#define MMDCXLVII        2647
#define MMDCXLVIII       2648
#define MMDCXLIX         2649
#define MMDCL            2650
#define MMDCLI           2651
#define MMDCLII          2652
#define MMDCLIII         2653
#define MMDCLIV          2654
#define MMDCLV           2655
#define MMDCLVI          2656
#define MMDCLVII         2657
#define MMDCLVIII        2658
#define MMDCLIX          2659
#define MMDCLX           2660
#define MMDCLXI          2661
#define MMDCLXII         2662
#define MMDCLXIII        2663
#define MMDCLXIV         2664
#define MMDCLXV          2665
#define MMDCLXVI         2666
#define MMDCLXVII        2667
#define MMDCLXVIII       2668
#define MMDCLXIX         2669
#define MMDCLXX          2670
#define MMDCLXXI         2671
#define MMDCLXXII        2672
#define MMDCLXXIII       2673
#define MMDCLXXIV        2674
#define MMDCLXXV         2675
#define MMDCLXXVI        2676
#define MMDCLXXVII       2677
#define MMDCLXXVIII      2678
#define MMDCLXXIX        2679
#define MMDCLXXX         2680
#define MMDCLXXXI        2681
#define MMDCLXXXII       2682
#define MMDCLXXXIII      2683
#define MMDCLXXXIV       2684
#define MMDCLXXXV        2685
#define MMDCLXXXVI       2686
#define MMDCLXXXVII      2687
#define MMDCLXXXVIII     2688
#define MMDCLXXXIX       2689
#define MMDCXC           2690
#define MMDCXCI          2691
#define MMDCXCII         2692
#define MMDCXCIII        2693
#define MMDCXCIV         2694
#define MMDCXCV          2695
#define MMDCXCVI         2696
#define MMDCXCVII        2697
#define MMDCXCVIII       2698
#define MMDCXCIX         2699
#define MMDCC            2700
#define MMDCCI           2701
#define MMDCCII          2702
#define MMDCCIII         2703
#define MMDCCIV          2704
#define MMDCCV           2705
#define MMDCCVI          2706
#define MMDCCVII         2707
#define MMDCCVIII        2708
#define MMDCCIX          2709
#define MMDCCX           2710
#define MMDCCXI          2711
#define MMDCCXII         2712
#define MMDCCXIII        2713
#define MMDCCXIV         2714
#define MMDCCXV          2715
#define MMDCCXVI         2716
#define MMDCCXVII        2717
#define MMDCCXVIII       2718
#define MMDCCXIX         2719
#define MMDCCXX          2720
#define MMDCCXXI         2721
#define MMDCCXXII        2722
#define MMDCCXXIII       2723
#define MMDCCXXIV        2724
#define MMDCCXXV         2725
#define MMDCCXXVI        2726
#define MMDCCXXVII       2727
#define MMDCCXXVIII      2728
#define MMDCCXXIX        2729
#define MMDCCXXX         2730
#define MMDCCXXXI        2731
#define MMDCCXXXII       2732
#define MMDCCXXXIII      2733
#define MMDCCXXXIV       2734
#define MMDCCXXXV        2735
#define MMDCCXXXVI       2736
#define MMDCCXXXVII      2737
#define MMDCCXXXVIII     2738
#define MMDCCXXXIX       2739
#define MMDCCXL          2740
#define MMDCCXLI         2741
#define MMDCCXLII        2742
#define MMDCCXLIII       2743
#define MMDCCXLIV        2744
#define MMDCCXLV         2745
#define MMDCCXLVI        2746
#define MMDCCXLVII       2747
#define MMDCCXLVIII      2748
#define MMDCCXLIX        2749
#define MMDCCL           2750
#define MMDCCLI          2751
#define MMDCCLII         2752
#define MMDCCLIII        2753
#define MMDCCLIV         2754
#define MMDCCLV          2755
#define MMDCCLVI         2756
#define MMDCCLVII        2757
#define MMDCCLVIII       2758
#define MMDCCLIX         2759
#define MMDCCLX          2760
#define MMDCCLXI         2761
#define MMDCCLXII        2762
#define MMDCCLXIII       2763
#define MMDCCLXIV        2764
#define MMDCCLXV         2765
#define MMDCCLXVI        2766
#define MMDCCLXVII       2767
#define MMDCCLXVIII      2768
#define MMDCCLXIX        2769
#define MMDCCLXX         2770
#define MMDCCLXXI        2771
#define MMDCCLXXII       2772
#define MMDCCLXXIII      2773
#define MMDCCLXXIV       2774
#define MMDCCLXXV        2775
#define MMDCCLXXVI       2776
#define MMDCCLXXVII      2777
#define MMDCCLXXVIII     2778
#define MMDCCLXXIX       2779
#define MMDCCLXXX        2780
#define MMDCCLXXXI       2781
#define MMDCCLXXXII      2782
#define MMDCCLXXXIII     2783
#define MMDCCLXXXIV      2784
#define MMDCCLXXXV       2785
#define MMDCCLXXXVI      2786
#define MMDCCLXXXVII     2787
#define MMDCCLXXXVIII    2788
#define MMDCCLXXXIX      2789
#define MMDCCXC          2790
#define MMDCCXCI         2791
#define MMDCCXCII        2792
#define MMDCCXCIII       2793
#define MMDCCXCIV        2794
#define MMDCCXCV         2795
#define MMDCCXCVI        2796
#define MMDCCXCVII       2797
#define MMDCCXCVIII      2798
#define MMDCCXCIX        2799
#define MMDCCC           2800
#define MMDCCCI          2801
#define MMDCCCII         2802
#define MMDCCCIII        2803
#define MMDCCCIV         2804
#define MMDCCCV          2805
#define MMDCCCVI         2806
#define MMDCCCVII        2807
#define MMDCCCVIII       2808
#define MMDCCCIX         2809
#define MMDCCCX          2810
#define MMDCCCXI         2811
#define MMDCCCXII        2812
#define MMDCCCXIII       2813
#define MMDCCCXIV        2814
#define MMDCCCXV         2815
#define MMDCCCXVI        2816
#define MMDCCCXVII       2817
#define MMDCCCXVIII      2818
#define MMDCCCXIX        2819
#define MMDCCCXX         2820
#define MMDCCCXXI        2821
#define MMDCCCXXII       2822
#define MMDCCCXXIII      2823
#define MMDCCCXXIV       2824
#define MMDCCCXXV        2825
#define MMDCCCXXVI       2826
#define MMDCCCXXVII      2827
#define MMDCCCXXVIII     2828
#define MMDCCCXXIX       2829
#define MMDCCCXXX        2830
#define MMDCCCXXXI       2831
#define MMDCCCXXXII      2832
#define MMDCCCXXXIII     2833
#define MMDCCCXXXIV      2834
#define MMDCCCXXXV       2835
#define MMDCCCXXXVI      2836
#define MMDCCCXXXVII     2837
#define MMDCCCXXXVIII    2838
#define MMDCCCXXXIX      2839
#define MMDCCCXL         2840
#define MMDCCCXLI        2841
#define MMDCCCXLII       2842
#define MMDCCCXLIII      2843
#define MMDCCCXLIV       2844
#define MMDCCCXLV        2845
#define MMDCCCXLVI       2846
#define MMDCCCXLVII      2847
#define MMDCCCXLVIII     2848
#define MMDCCCXLIX       2849
#define MMDCCCL          2850
#define MMDCCCLI         2851
#define MMDCCCLII        2852
#define MMDCCCLIII       2853
#define MMDCCCLIV        2854
#define MMDCCCLV         2855
#define MMDCCCLVI        2856
#define MMDCCCLVII       2857
#define MMDCCCLVIII      2858
#define MMDCCCLIX        2859
#define MMDCCCLX         2860
#define MMDCCCLXI        2861
#define MMDCCCLXII       2862
#define MMDCCCLXIII      2863
#define MMDCCCLXIV       2864
#define MMDCCCLXV        2865
#define MMDCCCLXVI       2866
#define MMDCCCLXVII      2867
#define MMDCCCLXVIII     2868
#define MMDCCCLXIX       2869
#define MMDCCCLXX        2870
#define MMDCCCLXXI       2871
#define MMDCCCLXXII      2872
#define MMDCCCLXXIII     2873
#define MMDCCCLXXIV      2874
#define MMDCCCLXXV       2875
#define MMDCCCLXXVI      2876
#define MMDCCCLXXVII     2877
#define MMDCCCLXXVIII    2878
#define MMDCCCLXXIX      2879
#define MMDCCCLXXX       2880
#define MMDCCCLXXXI      2881
#define MMDCCCLXXXII     2882
#define MMDCCCLXXXIII    2883
#define MMDCCCLXXXIV     2884
#define MMDCCCLXXXV      2885
#define MMDCCCLXXXVI     2886
#define MMDCCCLXXXVII    2887
#define MMDCCCLXXXVIII   2888
#define MMDCCCLXXXIX     2889
#define MMDCCCXC         2890
#define MMDCCCXCI        2891
#define MMDCCCXCII       2892
#define MMDCCCXCIII      2893
#define MMDCCCXCIV       2894
#define MMDCCCXCV        2895
#define MMDCCCXCVI       2896
#define MMDCCCXCVII      2897
#define MMDCCCXCVIII     2898
#define MMDCCCXCIX       2899
#define MMCM             2900
#define MMCMI            2901
#define MMCMII           2902
#define MMCMIII          2903
#define MMCMIV           2904
#define MMCMV            2905
#define MMCMVI           2906
#define MMCMVII          2907
#define MMCMVIII         2908
#define MMCMIX           2909
#define MMCMX            2910
#define MMCMXI           2911
#define MMCMXII          2912
#define MMCMXIII         2913
#define MMCMXIV          2914
#define MMCMXV           2915
#define MMCMXVI          2916
#define MMCMXVII         2917
#define MMCMXVIII        2918
#define MMCMXIX          2919
#define MMCMXX           2920
#define MMCMXXI          2921
#define MMCMXXII         2922
#define MMCMXXIII        2923
#define MMCMXXIV         2924
#define MMCMXXV          2925
#define MMCMXXVI         2926
#define MMCMXXVII        2927
#define MMCMXXVIII       2928
#define MMCMXXIX         2929
#define MMCMXXX          2930
#define MMCMXXXI         2931
#define MMCMXXXII        2932
#define MMCMXXXIII       2933
#define MMCMXXXIV        2934
#define MMCMXXXV         2935
#define MMCMXXXVI        2936
#define MMCMXXXVII       2937
#define MMCMXXXVIII      2938
#define MMCMXXXIX        2939
#define MMCMXL           2940
#define MMCMXLI          2941
#define MMCMXLII         2942
#define MMCMXLIII        2943
#define MMCMXLIV         2944
#define MMCMXLV          2945
#define MMCMXLVI         2946
#define MMCMXLVII        2947
#define MMCMXLVIII       2948
#define MMCMXLIX         2949
#define MMCML            2950
#define MMCMLI           2951
#define MMCMLII          2952
#define MMCMLIII         2953
#define MMCMLIV          2954
#define MMCMLV           2955
#define MMCMLVI          2956
#define MMCMLVII         2957
#define MMCMLVIII        2958
#define MMCMLIX          2959
#define MMCMLX           2960
#define MMCMLXI          2961
#define MMCMLXII         2962
#define MMCMLXIII        2963
#define MMCMLXIV         2964
#define MMCMLXV          2965
#define MMCMLXVI         2966
#define MMCMLXVII        2967
#define MMCMLXVIII       2968
#define MMCMLXIX         2969
#define MMCMLXX          2970
#define MMCMLXXI         2971
#define MMCMLXXII        2972
#define MMCMLXXIII       2973
#define MMCMLXXIV        2974
#define MMCMLXXV         2975
#define MMCMLXXVI        2976
#define MMCMLXXVII       2977
#define MMCMLXXVIII      2978
#define MMCMLXXIX        2979
#define MMCMLXXX         2980
#define MMCMLXXXI        2981
#define MMCMLXXXII       2982
#define MMCMLXXXIII      2983
#define MMCMLXXXIV       2984
#define MMCMLXXXV        2985
#define MMCMLXXXVI       2986
#define MMCMLXXXVII      2987
#define MMCMLXXXVIII     2988
#define MMCMLXXXIX       2989
#define MMCMXC           2990
#define MMCMXCI          2991
#define MMCMXCII         2992
#define MMCMXCIII        2993
#define MMCMXCIV         2994
#define MMCMXCV          2995
#define MMCMXCVI         2996
#define MMCMXCVII        2997
#define MMCMXCVIII       2998
#define MMCMXCIX         2999
#define MMM              3000
#define MMMI             3001
#define MMMII            3002
#define MMMIII           3003
#define MMMIV            3004
#define MMMV             3005
#define MMMVI            3006
#define MMMVII           3007
#define MMMVIII          3008
#define MMMIX            3009
#define MMMX             3010
#define MMMXI            3011
#define MMMXII           3012
#define MMMXIII          3013
#define MMMXIV           3014
#define MMMXV            3015
#define MMMXVI           3016
#define MMMXVII          3017
#define MMMXVIII         3018
#define MMMXIX           3019
#define MMMXX            3020
#define MMMXXI           3021
#define MMMXXII          3022
#define MMMXXIII         3023
#define MMMXXIV          3024
#define MMMXXV           3025
#define MMMXXVI          3026
#define MMMXXVII         3027
#define MMMXXVIII        3028
#define MMMXXIX          3029
#define MMMXXX           3030
#define MMMXXXI          3031
#define MMMXXXII         3032
#define MMMXXXIII        3033
#define MMMXXXIV         3034
#define MMMXXXV          3035
#define MMMXXXVI         3036
#define MMMXXXVII        3037
#define MMMXXXVIII       3038
#define MMMXXXIX         3039
#define MMMXL            3040
#define MMMXLI           3041
#define MMMXLII          3042
#define MMMXLIII         3043
#define MMMXLIV          3044
#define MMMXLV           3045
#define MMMXLVI          3046
#define MMMXLVII         3047
#define MMMXLVIII        3048
#define MMMXLIX          3049
#define MMML             3050
#define MMMLI            3051
#define MMMLII           3052
#define MMMLIII          3053
#define MMMLIV           3054
#define MMMLV            3055
#define MMMLVI           3056
#define MMMLVII          3057
#define MMMLVIII         3058
#define MMMLIX           3059
#define MMMLX            3060
#define MMMLXI           3061
#define MMMLXII          3062
#define MMMLXIII         3063
#define MMMLXIV          3064
#define MMMLXV           3065
#define MMMLXVI          3066
#define MMMLXVII         3067
#define MMMLXVIII        3068
#define MMMLXIX          3069
#define MMMLXX           3070
#define MMMLXXI          3071
#define MMMLXXII         3072
#define MMMLXXIII        3073
#define MMMLXXIV         3074
#define MMMLXXV          3075
#define MMMLXXVI         3076
#define MMMLXXVII        3077
#define MMMLXXVIII       3078
#define MMMLXXIX         3079
#define MMMLXXX          3080
#define MMMLXXXI         3081
#define MMMLXXXII        3082
#define MMMLXXXIII       3083
#define MMMLXXXIV        3084
#define MMMLXXXV         3085
#define MMMLXXXVI        3086
#define MMMLXXXVII       3087
#define MMMLXXXVIII      3088
#define MMMLXXXIX        3089
#define MMMXC            3090
#define MMMXCI           3091
#define MMMXCII          3092
#define MMMXCIII         3093
#define MMMXCIV          3094
#define MMMXCV           3095
#define MMMXCVI          3096
#define MMMXCVII         3097
#define MMMXCVIII        3098
#define MMMXCIX          3099
#define MMMC             3100
#define MMMCI            3101
#define MMMCII           3102
#define MMMCIII          3103
#define MMMCIV           3104
#define MMMCV            3105
#define MMMCVI           3106
#define MMMCVII          3107
#define MMMCVIII         3108
#define MMMCIX           3109
#define MMMCX            3110
#define MMMCXI           3111
#define MMMCXII          3112
#define MMMCXIII         3113
#define MMMCXIV          3114
#define MMMCXV           3115
#define MMMCXVI          3116
#define MMMCXVII         3117
#define MMMCXVIII        3118
#define MMMCXIX          3119
#define MMMCXX           3120
#define MMMCXXI          3121
#define MMMCXXII         3122
#define MMMCXXIII        3123
#define MMMCXXIV         3124
#define MMMCXXV          3125
#define MMMCXXVI         3126
#define MMMCXXVII        3127
#define MMMCXXVIII       3128
#define MMMCXXIX         3129
#define MMMCXXX          3130
#define MMMCXXXI         3131
#define MMMCXXXII        3132
#define MMMCXXXIII       3133
#define MMMCXXXIV        3134
#define MMMCXXXV         3135
#define MMMCXXXVI        3136
#define MMMCXXXVII       3137
#define MMMCXXXVIII      3138
#define MMMCXXXIX        3139
#define MMMCXL           3140
#define MMMCXLI          3141
#define MMMCXLII         3142
#define MMMCXLIII        3143
#define MMMCXLIV         3144
#define MMMCXLV          3145
#define MMMCXLVI         3146
#define MMMCXLVII        3147
#define MMMCXLVIII       3148
#define MMMCXLIX         3149
#define MMMCL            3150
#define MMMCLI           3151
#define MMMCLII          3152
#define MMMCLIII         3153
#define MMMCLIV          3154
#define MMMCLV           3155
#define MMMCLVI          3156
#define MMMCLVII         3157
#define MMMCLVIII        3158
#define MMMCLIX          3159
#define MMMCLX           3160
#define MMMCLXI          3161
#define MMMCLXII         3162
#define MMMCLXIII        3163
#define MMMCLXIV         3164
#define MMMCLXV          3165
#define MMMCLXVI         3166
#define MMMCLXVII        3167
#define MMMCLXVIII       3168
#define MMMCLXIX         3169
#define MMMCLXX          3170
#define MMMCLXXI         3171
#define MMMCLXXII        3172
#define MMMCLXXIII       3173
#define MMMCLXXIV        3174
#define MMMCLXXV         3175
#define MMMCLXXVI        3176
#define MMMCLXXVII       3177
#define MMMCLXXVIII      3178
#define MMMCLXXIX        3179
#define MMMCLXXX         3180
#define MMMCLXXXI        3181
#define MMMCLXXXII       3182
#define MMMCLXXXIII      3183
#define MMMCLXXXIV       3184
#define MMMCLXXXV        3185
#define MMMCLXXXVI       3186
#define MMMCLXXXVII      3187
#define MMMCLXXXVIII     3188
#define MMMCLXXXIX       3189
#define MMMCXC           3190
#define MMMCXCI          3191
#define MMMCXCII         3192
#define MMMCXCIII        3193
#define MMMCXCIV         3194
#define MMMCXCV          3195
#define MMMCXCVI         3196
#define MMMCXCVII        3197
#define MMMCXCVIII       3198
#define MMMCXCIX         3199
#define MMMCC            3200
#define MMMCCI           3201
#define MMMCCII          3202
#define MMMCCIII         3203
#define MMMCCIV          3204
#define MMMCCV           3205
#define MMMCCVI          3206
#define MMMCCVII         3207
#define MMMCCVIII        3208
#define MMMCCIX          3209
#define MMMCCX           3210
#define MMMCCXI          3211
#define MMMCCXII         3212
#define MMMCCXIII        3213
#define MMMCCXIV         3214
#define MMMCCXV          3215
#define MMMCCXVI         3216
#define MMMCCXVII        3217
#define MMMCCXVIII       3218
#define MMMCCXIX         3219
#define MMMCCXX          3220
#define MMMCCXXI         3221
#define MMMCCXXII        3222
#define MMMCCXXIII       3223
#define MMMCCXXIV        3224
#define MMMCCXXV         3225
#define MMMCCXXVI        3226
#define MMMCCXXVII       3227
#define MMMCCXXVIII      3228
#define MMMCCXXIX        3229
#define MMMCCXXX         3230
#define MMMCCXXXI        3231
#define MMMCCXXXII       3232
#define MMMCCXXXIII      3233
#define MMMCCXXXIV       3234
#define MMMCCXXXV        3235
#define MMMCCXXXVI       3236
#define MMMCCXXXVII      3237
#define MMMCCXXXVIII     3238
#define MMMCCXXXIX       3239
#define MMMCCXL          3240
#define MMMCCXLI         3241
#define MMMCCXLII        3242
#define MMMCCXLIII       3243
#define MMMCCXLIV        3244
#define MMMCCXLV         3245
#define MMMCCXLVI        3246
#define MMMCCXLVII       3247
#define MMMCCXLVIII      3248
#define MMMCCXLIX        3249
#define MMMCCL           3250
#define MMMCCLI          3251
#define MMMCCLII         3252
#define MMMCCLIII        3253
#define MMMCCLIV         3254
#define MMMCCLV          3255
#define MMMCCLVI         3256
#define MMMCCLVII        3257
#define MMMCCLVIII       3258
#define MMMCCLIX         3259
#define MMMCCLX          3260
#define MMMCCLXI         3261
#define MMMCCLXII        3262
#define MMMCCLXIII       3263
#define MMMCCLXIV        3264
#define MMMCCLXV         3265
#define MMMCCLXVI        3266
#define MMMCCLXVII       3267
#define MMMCCLXVIII      3268
#define MMMCCLXIX        3269
#define MMMCCLXX         3270
#define MMMCCLXXI        3271
#define MMMCCLXXII       3272
#define MMMCCLXXIII      3273
#define MMMCCLXXIV       3274
#define MMMCCLXXV        3275
#define MMMCCLXXVI       3276
#define MMMCCLXXVII      3277
#define MMMCCLXXVIII     3278
#define MMMCCLXXIX       3279
#define MMMCCLXXX        3280
#define MMMCCLXXXI       3281
#define MMMCCLXXXII      3282
#define MMMCCLXXXIII     3283
#define MMMCCLXXXIV      3284
#define MMMCCLXXXV       3285
#define MMMCCLXXXVI      3286
#define MMMCCLXXXVII     3287
#define MMMCCLXXXVIII    3288
#define MMMCCLXXXIX      3289
#define MMMCCXC          3290
#define MMMCCXCI         3291
#define MMMCCXCII        3292
#define MMMCCXCIII       3293
#define MMMCCXCIV        3294
#define MMMCCXCV         3295
#define MMMCCXCVI        3296
#define MMMCCXCVII       3297
#define MMMCCXCVIII      3298
#define MMMCCXCIX        3299
#define MMMCCC           3300
#define MMMCCCI          3301
#define MMMCCCII         3302
#define MMMCCCIII        3303
#define MMMCCCIV         3304
#define MMMCCCV          3305
#define MMMCCCVI         3306
#define MMMCCCVII        3307
#define MMMCCCVIII       3308
#define MMMCCCIX         3309
#define MMMCCCX          3310
#define MMMCCCXI         3311
#define MMMCCCXII        3312
#define MMMCCCXIII       3313
#define MMMCCCXIV        3314
#define MMMCCCXV         3315
#define MMMCCCXVI        3316
#define MMMCCCXVII       3317
#define MMMCCCXVIII      3318
#define MMMCCCXIX        3319
#define MMMCCCXX         3320
#define MMMCCCXXI        3321
#define MMMCCCXXII       3322
#define MMMCCCXXIII      3323
#define MMMCCCXXIV       3324
#define MMMCCCXXV        3325
#define MMMCCCXXVI       3326
#define MMMCCCXXVII      3327
#define MMMCCCXXVIII     3328
#define MMMCCCXXIX       3329
#define MMMCCCXXX        3330
#define MMMCCCXXXI       3331
#define MMMCCCXXXII      3332
#define MMMCCCXXXIII     3333
#define MMMCCCXXXIV      3334
#define MMMCCCXXXV       3335
#define MMMCCCXXXVI      3336
#define MMMCCCXXXVII     3337
#define MMMCCCXXXVIII    3338
#define MMMCCCXXXIX      3339
#define MMMCCCXL         3340
#define MMMCCCXLI        3341
#define MMMCCCXLII       3342
#define MMMCCCXLIII      3343
#define MMMCCCXLIV       3344
#define MMMCCCXLV        3345
#define MMMCCCXLVI       3346
#define MMMCCCXLVII      3347
#define MMMCCCXLVIII     3348
#define MMMCCCXLIX       3349
#define MMMCCCL          3350
#define MMMCCCLI         3351
#define MMMCCCLII        3352
#define MMMCCCLIII       3353
#define MMMCCCLIV        3354
#define MMMCCCLV         3355
#define MMMCCCLVI        3356
#define MMMCCCLVII       3357
#define MMMCCCLVIII      3358
#define MMMCCCLIX        3359
#define MMMCCCLX         3360
#define MMMCCCLXI        3361
#define MMMCCCLXII       3362
#define MMMCCCLXIII      3363
#define MMMCCCLXIV       3364
#define MMMCCCLXV        3365
#define MMMCCCLXVI       3366
#define MMMCCCLXVII      3367
#define MMMCCCLXVIII     3368
#define MMMCCCLXIX       3369
#define MMMCCCLXX        3370
#define MMMCCCLXXI       3371
#define MMMCCCLXXII      3372
#define MMMCCCLXXIII     3373
#define MMMCCCLXXIV      3374
#define MMMCCCLXXV       3375
#define MMMCCCLXXVI      3376
#define MMMCCCLXXVII     3377
#define MMMCCCLXXVIII    3378
#define MMMCCCLXXIX      3379
#define MMMCCCLXXX       3380
#define MMMCCCLXXXI      3381
#define MMMCCCLXXXII     3382
#define MMMCCCLXXXIII    3383
#define MMMCCCLXXXIV     3384
#define MMMCCCLXXXV      3385
#define MMMCCCLXXXVI     3386
#define MMMCCCLXXXVII    3387
#define MMMCCCLXXXVIII   3388
#define MMMCCCLXXXIX     3389
#define MMMCCCXC         3390
#define MMMCCCXCI        3391
#define MMMCCCXCII       3392
#define MMMCCCXCIII      3393
#define MMMCCCXCIV       3394
#define MMMCCCXCV        3395
#define MMMCCCXCVI       3396
#define MMMCCCXCVII      3397
#define MMMCCCXCVIII     3398
#define MMMCCCXCIX       3399
#define MMMCD            3400
#define MMMCDI           3401
#define MMMCDII          3402
#define MMMCDIII         3403
#define MMMCDIV          3404
#define MMMCDV           3405
#define MMMCDVI          3406
#define MMMCDVII         3407
#define MMMCDVIII        3408
#define MMMCDIX          3409
#define MMMCDX           3410
#define MMMCDXI          3411
#define MMMCDXII         3412
#define MMMCDXIII        3413
#define MMMCDXIV         3414
#define MMMCDXV          3415
#define MMMCDXVI         3416
#define MMMCDXVII        3417
#define MMMCDXVIII       3418
#define MMMCDXIX         3419
#define MMMCDXX          3420
#define MMMCDXXI         3421
#define MMMCDXXII        3422
#define MMMCDXXIII       3423
#define MMMCDXXIV        3424
#define MMMCDXXV         3425
#define MMMCDXXVI        3426
#define MMMCDXXVII       3427
#define MMMCDXXVIII      3428
#define MMMCDXXIX        3429
#define MMMCDXXX         3430
#define MMMCDXXXI        3431
#define MMMCDXXXII       3432
#define MMMCDXXXIII      3433
#define MMMCDXXXIV       3434
#define MMMCDXXXV        3435
#define MMMCDXXXVI       3436
#define MMMCDXXXVII      3437
#define MMMCDXXXVIII     3438
#define MMMCDXXXIX       3439
#define MMMCDXL          3440
#define MMMCDXLI         3441
#define MMMCDXLII        3442
#define MMMCDXLIII       3443
#define MMMCDXLIV        3444
#define MMMCDXLV         3445
#define MMMCDXLVI        3446
#define MMMCDXLVII       3447
#define MMMCDXLVIII      3448
#define MMMCDXLIX        3449
#define MMMCDL           3450
#define MMMCDLI          3451
#define MMMCDLII         3452
#define MMMCDLIII        3453
#define MMMCDLIV         3454
#define MMMCDLV          3455
#define MMMCDLVI         3456
#define MMMCDLVII        3457
#define MMMCDLVIII       3458
#define MMMCDLIX         3459
#define MMMCDLX          3460
#define MMMCDLXI         3461
#define MMMCDLXII        3462
#define MMMCDLXIII       3463
#define MMMCDLXIV        3464
#define MMMCDLXV         3465
#define MMMCDLXVI        3466
#define MMMCDLXVII       3467
#define MMMCDLXVIII      3468
#define MMMCDLXIX        3469
#define MMMCDLXX         3470
#define MMMCDLXXI        3471
#define MMMCDLXXII       3472
#define MMMCDLXXIII      3473
#define MMMCDLXXIV       3474
#define MMMCDLXXV        3475
#define MMMCDLXXVI       3476
#define MMMCDLXXVII      3477
#define MMMCDLXXVIII     3478
#define MMMCDLXXIX       3479
#define MMMCDLXXX        3480
#define MMMCDLXXXI       3481
#define MMMCDLXXXII      3482
#define MMMCDLXXXIII     3483
#define MMMCDLXXXIV      3484
#define MMMCDLXXXV       3485
#define MMMCDLXXXVI      3486
#define MMMCDLXXXVII     3487
#define MMMCDLXXXVIII    3488
#define MMMCDLXXXIX      3489
#define MMMCDXC          3490
#define MMMCDXCI         3491
#define MMMCDXCII        3492
#define MMMCDXCIII       3493
#define MMMCDXCIV        3494
#define MMMCDXCV         3495
#define MMMCDXCVI        3496
#define MMMCDXCVII       3497
#define MMMCDXCVIII      3498
#define MMMCDXCIX        3499
#define MMMD             3500
#define MMMDI            3501
#define MMMDII           3502
#define MMMDIII          3503
#define MMMDIV           3504
#define MMMDV            3505
#define MMMDVI           3506
#define MMMDVII          3507
#define MMMDVIII         3508
#define MMMDIX           3509
#define MMMDX            3510
#define MMMDXI           3511
#define MMMDXII          3512
#define MMMDXIII         3513
#define MMMDXIV          3514
#define MMMDXV           3515
#define MMMDXVI          3516
#define MMMDXVII         3517
#define MMMDXVIII        3518
#define MMMDXIX          3519
#define MMMDXX           3520
#define MMMDXXI          3521
#define MMMDXXII         3522
#define MMMDXXIII        3523
#define MMMDXXIV         3524
#define MMMDXXV          3525
#define MMMDXXVI         3526
#define MMMDXXVII        3527
#define MMMDXXVIII       3528
#define MMMDXXIX         3529
#define MMMDXXX          3530
#define MMMDXXXI         3531
#define MMMDXXXII        3532
#define MMMDXXXIII       3533
#define MMMDXXXIV        3534
#define MMMDXXXV         3535
#define MMMDXXXVI        3536
#define MMMDXXXVII       3537
#define MMMDXXXVIII      3538
#define MMMDXXXIX        3539
#define MMMDXL           3540
#define MMMDXLI          3541
#define MMMDXLII         3542
#define MMMDXLIII        3543
#define MMMDXLIV         3544
#define MMMDXLV          3545
#define MMMDXLVI         3546
#define MMMDXLVII        3547
#define MMMDXLVIII       3548
#define MMMDXLIX         3549
#define MMMDL            3550
#define MMMDLI           3551
#define MMMDLII          3552
#define MMMDLIII         3553
#define MMMDLIV          3554
#define MMMDLV           3555
#define MMMDLVI          3556
#define MMMDLVII         3557
#define MMMDLVIII        3558
#define MMMDLIX          3559
#define MMMDLX           3560
#define MMMDLXI          3561
#define MMMDLXII         3562
#define MMMDLXIII        3563
#define MMMDLXIV         3564
#define MMMDLXV          3565
#define MMMDLXVI         3566
#define MMMDLXVII        3567
#define MMMDLXVIII       3568
#define MMMDLXIX         3569
#define MMMDLXX          3570
#define MMMDLXXI         3571
#define MMMDLXXII        3572
#define MMMDLXXIII       3573
#define MMMDLXXIV        3574
#define MMMDLXXV         3575
#define MMMDLXXVI        3576
#define MMMDLXXVII       3577
#define MMMDLXXVIII      3578
#define MMMDLXXIX        3579
#define MMMDLXXX         3580
#define MMMDLXXXI        3581
#define MMMDLXXXII       3582
#define MMMDLXXXIII      3583
#define MMMDLXXXIV       3584
#define MMMDLXXXV        3585
#define MMMDLXXXVI       3586
#define MMMDLXXXVII      3587
#define MMMDLXXXVIII     3588
#define MMMDLXXXIX       3589
#define MMMDXC           3590
#define MMMDXCI          3591
#define MMMDXCII         3592
#define MMMDXCIII        3593
#define MMMDXCIV         3594
#define MMMDXCV          3595
#define MMMDXCVI         3596
#define MMMDXCVII        3597
#define MMMDXCVIII       3598
#define MMMDXCIX         3599
#define MMMDC            3600
#define MMMDCI           3601
#define MMMDCII          3602
#define MMMDCIII         3603
#define MMMDCIV          3604
#define MMMDCV           3605
#define MMMDCVI          3606
#define MMMDCVII         3607
#define MMMDCVIII        3608
#define MMMDCIX          3609
#define MMMDCX           3610
#define MMMDCXI          3611
#define MMMDCXII         3612
#define MMMDCXIII        3613
#define MMMDCXIV         3614
#define MMMDCXV          3615
#define MMMDCXVI         3616
#define MMMDCXVII        3617
#define MMMDCXVIII       3618
#define MMMDCXIX         3619
#define MMMDCXX          3620
#define MMMDCXXI         3621
#define MMMDCXXII        3622
#define MMMDCXXIII       3623
#define MMMDCXXIV        3624
#define MMMDCXXV         3625
#define MMMDCXXVI        3626
#define MMMDCXXVII       3627
#define MMMDCXXVIII      3628
#define MMMDCXXIX        3629
#define MMMDCXXX         3630
#define MMMDCXXXI        3631
#define MMMDCXXXII       3632
#define MMMDCXXXIII      3633
#define MMMDCXXXIV       3634
#define MMMDCXXXV        3635
#define MMMDCXXXVI       3636
#define MMMDCXXXVII      3637
#define MMMDCXXXVIII     3638
#define MMMDCXXXIX       3639
#define MMMDCXL          3640
#define MMMDCXLI         3641
#define MMMDCXLII        3642
#define MMMDCXLIII       3643
#define MMMDCXLIV        3644
#define MMMDCXLV         3645
#define MMMDCXLVI        3646
#define MMMDCXLVII       3647
#define MMMDCXLVIII      3648
#define MMMDCXLIX        3649
#define MMMDCL           3650
#define MMMDCLI          3651
#define MMMDCLII         3652
#define MMMDCLIII        3653
#define MMMDCLIV         3654
#define MMMDCLV          3655
#define MMMDCLVI         3656
#define MMMDCLVII        3657
#define MMMDCLVIII       3658
#define MMMDCLIX         3659
#define MMMDCLX          3660
#define MMMDCLXI         3661
#define MMMDCLXII        3662
#define MMMDCLXIII       3663
#define MMMDCLXIV        3664
#define MMMDCLXV         3665
#define MMMDCLXVI        3666
#define MMMDCLXVII       3667
#define MMMDCLXVIII      3668
#define MMMDCLXIX        3669
#define MMMDCLXX         3670
#define MMMDCLXXI        3671
#define MMMDCLXXII       3672
#define MMMDCLXXIII      3673
#define MMMDCLXXIV       3674
#define MMMDCLXXV        3675
#define MMMDCLXXVI       3676
#define MMMDCLXXVII      3677
#define MMMDCLXXVIII     3678
#define MMMDCLXXIX       3679
#define MMMDCLXXX        3680
#define MMMDCLXXXI       3681
#define MMMDCLXXXII      3682
#define MMMDCLXXXIII     3683
#define MMMDCLXXXIV      3684
#define MMMDCLXXXV       3685
#define MMMDCLXXXVI      3686
#define MMMDCLXXXVII     3687
#define MMMDCLXXXVIII    3688
#define MMMDCLXXXIX      3689
#define MMMDCXC          3690
#define MMMDCXCI         3691
#define MMMDCXCII        3692
#define MMMDCXCIII       3693
#define MMMDCXCIV        3694
#define MMMDCXCV         3695
#define MMMDCXCVI        3696
#define MMMDCXCVII       3697
#define MMMDCXCVIII      3698
#define MMMDCXCIX        3699
#define MMMDCC           3700
#define MMMDCCI          3701
#define MMMDCCII         3702
#define MMMDCCIII        3703
#define MMMDCCIV         3704
#define MMMDCCV          3705
#define MMMDCCVI         3706
#define MMMDCCVII        3707
#define MMMDCCVIII       3708
#define MMMDCCIX         3709
#define MMMDCCX          3710
#define MMMDCCXI         3711
#define MMMDCCXII        3712
#define MMMDCCXIII       3713
#define MMMDCCXIV        3714
#define MMMDCCXV         3715
#define MMMDCCXVI        3716
#define MMMDCCXVII       3717
#define MMMDCCXVIII      3718
#define MMMDCCXIX        3719
#define MMMDCCXX         3720
#define MMMDCCXXI        3721
#define MMMDCCXXII       3722
#define MMMDCCXXIII      3723
#define MMMDCCXXIV       3724
#define MMMDCCXXV        3725
#define MMMDCCXXVI       3726
#define MMMDCCXXVII      3727
#define MMMDCCXXVIII     3728
#define MMMDCCXXIX       3729
#define MMMDCCXXX        3730
#define MMMDCCXXXI       3731
#define MMMDCCXXXII      3732
#define MMMDCCXXXIII     3733
#define MMMDCCXXXIV      3734
#define MMMDCCXXXV       3735
#define MMMDCCXXXVI      3736
#define MMMDCCXXXVII     3737
#define MMMDCCXXXVIII    3738
#define MMMDCCXXXIX      3739
#define MMMDCCXL         3740
#define MMMDCCXLI        3741
#define MMMDCCXLII       3742
#define MMMDCCXLIII      3743
#define MMMDCCXLIV       3744
#define MMMDCCXLV        3745
#define MMMDCCXLVI       3746
#define MMMDCCXLVII      3747
#define MMMDCCXLVIII     3748
#define MMMDCCXLIX       3749
#define MMMDCCL          3750
#define MMMDCCLI         3751
#define MMMDCCLII        3752
#define MMMDCCLIII       3753
#define MMMDCCLIV        3754
#define MMMDCCLV         3755
#define MMMDCCLVI        3756
#define MMMDCCLVII       3757
#define MMMDCCLVIII      3758
#define MMMDCCLIX        3759
#define MMMDCCLX         3760
#define MMMDCCLXI        3761
#define MMMDCCLXII       3762
#define MMMDCCLXIII      3763
#define MMMDCCLXIV       3764
#define MMMDCCLXV        3765
#define MMMDCCLXVI       3766
#define MMMDCCLXVII      3767
#define MMMDCCLXVIII     3768
#define MMMDCCLXIX       3769
#define MMMDCCLXX        3770
#define MMMDCCLXXI       3771
#define MMMDCCLXXII      3772
#define MMMDCCLXXIII     3773
#define MMMDCCLXXIV      3774
#define MMMDCCLXXV       3775
#define MMMDCCLXXVI      3776
#define MMMDCCLXXVII     3777
#define MMMDCCLXXVIII    3778
#define MMMDCCLXXIX      3779
#define MMMDCCLXXX       3780
#define MMMDCCLXXXI      3781
#define MMMDCCLXXXII     3782
#define MMMDCCLXXXIII    3783
#define MMMDCCLXXXIV     3784
#define MMMDCCLXXXV      3785
#define MMMDCCLXXXVI     3786
#define MMMDCCLXXXVII    3787
#define MMMDCCLXXXVIII   3788
#define MMMDCCLXXXIX     3789
#define MMMDCCXC         3790
#define MMMDCCXCI        3791
#define MMMDCCXCII       3792
#define MMMDCCXCIII      3793
#define MMMDCCXCIV       3794
#define MMMDCCXCV        3795
#define MMMDCCXCVI       3796
#define MMMDCCXCVII      3797
#define MMMDCCXCVIII     3798
#define MMMDCCXCIX       3799
#define MMMDCCC          3800
#define MMMDCCCI         3801
#define MMMDCCCII        3802
#define MMMDCCCIII       3803
#define MMMDCCCIV        3804
#define MMMDCCCV         3805
#define MMMDCCCVI        3806
#define MMMDCCCVII       3807
#define MMMDCCCVIII      3808
#define MMMDCCCIX        3809
#define MMMDCCCX         3810
#define MMMDCCCXI        3811
#define MMMDCCCXII       3812
#define MMMDCCCXIII      3813
#define MMMDCCCXIV       3814
#define MMMDCCCXV        3815
#define MMMDCCCXVI       3816
#define MMMDCCCXVII      3817
#define MMMDCCCXVIII     3818
#define MMMDCCCXIX       3819
#define MMMDCCCXX        3820
#define MMMDCCCXXI       3821
#define MMMDCCCXXII      3822
#define MMMDCCCXXIII     3823
#define MMMDCCCXXIV      3824
#define MMMDCCCXXV       3825
#define MMMDCCCXXVI      3826
#define MMMDCCCXXVII     3827
#define MMMDCCCXXVIII    3828
#define MMMDCCCXXIX      3829
#define MMMDCCCXXX       3830
#define MMMDCCCXXXI      3831
#define MMMDCCCXXXII     3832
#define MMMDCCCXXXIII    3833
#define MMMDCCCXXXIV     3834
#define MMMDCCCXXXV      3835
#define MMMDCCCXXXVI     3836
#define MMMDCCCXXXVII    3837
#define MMMDCCCXXXVIII   3838
#define MMMDCCCXXXIX     3839
#define MMMDCCCXL        3840
#define MMMDCCCXLI       3841
#define MMMDCCCXLII      3842
#define MMMDCCCXLIII     3843
#define MMMDCCCXLIV      3844
#define MMMDCCCXLV       3845
#define MMMDCCCXLVI      3846
#define MMMDCCCXLVII     3847
#define MMMDCCCXLVIII    3848
#define MMMDCCCXLIX      3849
#define MMMDCCCL         3850
#define MMMDCCCLI        3851
#define MMMDCCCLII       3852
#define MMMDCCCLIII      3853
#define MMMDCCCLIV       3854
#define MMMDCCCLV        3855
#define MMMDCCCLVI       3856
#define MMMDCCCLVII      3857
#define MMMDCCCLVIII     3858
#define MMMDCCCLIX       3859
#define MMMDCCCLX        3860
#define MMMDCCCLXI       3861
#define MMMDCCCLXII      3862
#define MMMDCCCLXIII     3863
#define MMMDCCCLXIV      3864
#define MMMDCCCLXV       3865
#define MMMDCCCLXVI      3866
#define MMMDCCCLXVII     3867
#define MMMDCCCLXVIII    3868
#define MMMDCCCLXIX      3869
#define MMMDCCCLXX       3870
#define MMMDCCCLXXI      3871
#define MMMDCCCLXXII     3872
#define MMMDCCCLXXIII    3873
#define MMMDCCCLXXIV     3874
#define MMMDCCCLXXV      3875
#define MMMDCCCLXXVI     3876
#define MMMDCCCLXXVII    3877
#define MMMDCCCLXXVIII   3878
#define MMMDCCCLXXIX     3879
#define MMMDCCCLXXX      3880
#define MMMDCCCLXXXI     3881
#define MMMDCCCLXXXII    3882
#define MMMDCCCLXXXIII   3883
#define MMMDCCCLXXXIV    3884
#define MMMDCCCLXXXV     3885
#define MMMDCCCLXXXVI    3886
#define MMMDCCCLXXXVII   3887
#define MMMDCCCLXXXVIII  3888
#define MMMDCCCLXXXIX    3889
#define MMMDCCCXC        3890
#define MMMDCCCXCI       3891
#define MMMDCCCXCII      3892
#define MMMDCCCXCIII     3893
#define MMMDCCCXCIV      3894
#define MMMDCCCXCV       3895
#define MMMDCCCXCVI      3896
#define MMMDCCCXCVII     3897
#define MMMDCCCXCVIII    3898
#define MMMDCCCXCIX      3899
#define MMMCM            3900
#define MMMCMI           3901
#define MMMCMII          3902
#define MMMCMIII         3903
#define MMMCMIV          3904
#define MMMCMV           3905
#define MMMCMVI          3906
#define MMMCMVII         3907
#define MMMCMVIII        3908
#define MMMCMIX          3909
#define MMMCMX           3910
#define MMMCMXI          3911
#define MMMCMXII         3912
#define MMMCMXIII        3913
#define MMMCMXIV         3914
#define MMMCMXV          3915
#define MMMCMXVI         3916
#define MMMCMXVII        3917
#define MMMCMXVIII       3918
#define MMMCMXIX         3919
#define MMMCMXX          3920
#define MMMCMXXI         3921
#define MMMCMXXII        3922
#define MMMCMXXIII       3923
#define MMMCMXXIV        3924
#define MMMCMXXV         3925
#define MMMCMXXVI        3926
#define MMMCMXXVII       3927
#define MMMCMXXVIII      3928
#define MMMCMXXIX        3929
#define MMMCMXXX         3930
#define MMMCMXXXI        3931
#define MMMCMXXXII       3932
#define MMMCMXXXIII      3933
#define MMMCMXXXIV       3934
#define MMMCMXXXV        3935
#define MMMCMXXXVI       3936
#define MMMCMXXXVII      3937
#define MMMCMXXXVIII     3938
#define MMMCMXXXIX       3939
#define MMMCMXL          3940
#define MMMCMXLI         3941
#define MMMCMXLII        3942
#define MMMCMXLIII       3943
#define MMMCMXLIV        3944
#define MMMCMXLV         3945
#define MMMCMXLVI        3946
#define MMMCMXLVII       3947
#define MMMCMXLVIII      3948
#define MMMCMXLIX        3949
#define MMMCML           3950
#define MMMCMLI          3951
#define MMMCMLII         3952
#define MMMCMLIII        3953
#define MMMCMLIV         3954
#define MMMCMLV          3955
#define MMMCMLVI         3956
#define MMMCMLVII        3957
#define MMMCMLVIII       3958
#define MMMCMLIX         3959
#define MMMCMLX          3960
#define MMMCMLXI         3961
#define MMMCMLXII        3962
#define MMMCMLXIII       3963
#define MMMCMLXIV        3964
#define MMMCMLXV         3965
#define MMMCMLXVI        3966
#define MMMCMLXVII       3967
#define MMMCMLXVIII      3968
#define MMMCMLXIX        3969
#define MMMCMLXX         3970
#define MMMCMLXXI        3971
#define MMMCMLXXII       3972
#define MMMCMLXXIII      3973
#define MMMCMLXXIV       3974
#define MMMCMLXXV        3975
#define MMMCMLXXVI       3976
#define MMMCMLXXVII      3977
#define MMMCMLXXVIII     3978
#define MMMCMLXXIX       3979
#define MMMCMLXXX        3980
#define MMMCMLXXXI       3981
#define MMMCMLXXXII      3982
#define MMMCMLXXXIII     3983
#define MMMCMLXXXIV      3984
#define MMMCMLXXXV       3985
#define MMMCMLXXXVI      3986
#define MMMCMLXXXVII     3987
#define MMMCMLXXXVIII    3988
#define MMMCMLXXXIX      3989
#define MMMCMXC          3990
#define MMMCMXCI         3991
#define MMMCMXCII        3992
#define MMMCMXCIII       3993
#define MMMCMXCIV        3994
#define MMMCMXCV         3995
#define MMMCMXCVI        3996
#define MMMCMXCVII       3997
#define MMMCMXCVIII      3998
#define MMMCMXCIX        3999
/* finis numerorum generatorum */

#define imprimere     printf
#define liberare         free
#define memoriae_allocare    malloc
#define exire                exit

#define interior         static
#define hic_manens     static
#define universalis static

#define FILUM FILE

nomen insignatus character    i8;
nomen insignatus brevis         i16;
nomen insignatus integer       i32;
nomen insignatus longus longus    i64;

nomen signatus character    s8;
nomen signatus brevis            s16;
nomen signatus integer         s32;
nomen signatus longus longus    s64;

nomen fluitans                  f32;
nomen duplex                         f64;

nomen integer                    b32;

nomen size_t                                 memoriae_index;

#endif /* LATINA_H */

/* ================= ex include/piscina.h ================= */
/* piscina.h - arena memoriae: liberatio tota semel (arena, pool) */
#ifndef PISCINA_H
#define PISCINA_H

/* PiscinaNotatio - nota pro mark/reset pattern
 * Captat statum piscinam ut postea reficere possit
 */
nomen structura OfficinaPiscinaNotatio {
            vacuum* alveus_nunc;   /* Index ad alveum currentem */
    memoriae_index  positus;       /* Offset in alveo */
} OfficinaPiscinaNotatio;

OfficinaPiscina*
officina_piscina_generare_dynamicum (
          constans character* piscinae_titulum,
              memoriae_index  mensura_alvei_initia);


/* ===============================================
 * Destructio
 * =============================================== */

vacuum
officina_piscina_destruere (
        OfficinaPiscina* piscina);


/* ===============================================
 * Allocatio - fatalis si fallit
 *
 * piscina_allocare ordinat ad PISCINA_ORDINATIO_ORDINARIA (VIII):
 * satis pro omni typo domus (indices, i64/s64, f64), sicut malloc.
 * Octeti soli (textus) arte stipari possunt per
 * piscina_allocare_ordinatum(piscina, mensura, I). Ante 2026-10-05
 * ordinatio ordinaria erat I: membra latiora non ordinata - mores
 * indefiniti in C, quos sanitas 'alignment' capit.
 * =============================================== */

#define PISCINA_ORDINATIO_ORDINARIA VIII

static vacuum*
officina_piscina_allocare (
                         OfficinaPiscina* piscina,
                  memoriae_index  mensura);

static vacuum*
officina_piscina_allocare_ordinatum (
                         OfficinaPiscina* piscina,
                  memoriae_index  mensura,
                  memoriae_index  ordinatio);

#endif

/* ================= ex include/chorda.h ================= */
/* chorda.h - chorda: mensura + datum, SINE NUL (string slice) */
#ifndef CHORDA_H
#define CHORDA_H


/* ==================================================
 * Constructores
 * ================================================== */

static OfficinaChorda
officina_chorda_ex_literis (
                  constans character* litterae,
                             OfficinaPiscina* piscina);

static OfficinaChorda
officina_chorda_transcribere (
         OfficinaChorda  s,
        OfficinaPiscina* piscina);


/* ==================================================
 * Divisio
 * ================================================= */

nomen structura {
    OfficinaChorda* elementa;
       i32  numerus;
} OfficinaChordaFissioFructus;

static b32
officina_chorda_aequalis_literis (
                    OfficinaChorda  s,
        constans character* cstr);

static s32
officina_chorda_comparare (
        OfficinaChorda a,
        OfficinaChorda b);

#endif /* CHORDA_H */

/* ================= ex include/chorda_aedificator.h ================= */
/* chorda_aedificator.h - chordas accumulare (string builder) */
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

nomen structura OfficinaChordaAedificator OfficinaChordaAedificator;


/* ==================================================
 * Creatio / Destructio
 * ================================================== */

static OfficinaChordaAedificator*
officina_chorda_aedificator_creare (
           OfficinaPiscina* piscina,
    memoriae_index  capacitas_initialis);


/* ==================================================
 * Appendere - Singularis Character
 * ================================================== */

static b32
officina_chorda_aedificator_appendere_character (
    OfficinaChordaAedificator* aedificator,
            character  c);


/* ==================================================
 * Appendere - Chordae (Chordae et C-chordae)
 * ================================================== */

static b32
officina_chorda_aedificator_appendere_literis (
     OfficinaChordaAedificator* aedificator,
    constans character* cstr);

static b32
officina_chorda_aedificator_appendere_chorda (
    OfficinaChordaAedificator* aedificator,
               OfficinaChorda  s);

static b32
officina_chorda_aedificator_appendere_i32 (
    OfficinaChordaAedificator* aedificator,
                  i32  n);

/* appendere_repetita: appende characterem n vicibus
 * Utile ad padding vel indentationem */
static b32
officina_chorda_aedificator_appendere_repetita (
    OfficinaChordaAedificator* aedificator,
            character  c,
                  i32  numerus);

/* finire: converte aedificatorem ad chordam
 * Transfert dominium chordae accumulatae ad vocantem.
 * Aedificator destruitur post finire.
 * Vocans debet finaliter deallocare per piscinam. */
static OfficinaChorda
officina_chorda_aedificator_finire (
    OfficinaChordaAedificator* aedificator);


/* ==================================================
 * Constantae Configurationis
 * ================================================== */

/* CHORDA_AEDIFICATOR_INDENTATIO_SPATIA
 * Numerus spatiorum per gradum indentationis (typice 2 vel 4) */
#define CHORDA_AEDIFICATOR_INDENTATIO_SPATIA II


#endif /* CHORDA_AEDIFICATOR_H */

/* ================= ex include/friatio.h ================= */
/* friatio.h - functiones friationis (hash functions) */
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
officina_friatio_fnv1a (
    OfficinaChorda clavis);


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
/* tabula_dispersa.h - tabula friationis (hash table, map) */
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
} SlotusStatus;


/* Genus functionis friationis
 * Clavis chordae capit, friationes i32 reddit */
nomen i32 (*TabulaFriatio)(OfficinaChorda clavis);


/* Genus functionis comparationis
 * 0 reddit si aequalis, <0 si a<b, >0 si a>b */
nomen s32 (*TabulaComparatio)(OfficinaChorda a, OfficinaChorda b);

nomen structura {
    SlotusStatus  status;
          OfficinaChorda  clavis;
          vacuum* valor;
             i32  hash_cachatum;
} Slotus;

/* Ipsa tabula friationis */
nomen structura {
              Slotus* sloti;
                 i32  capacitas;
                 i32  capacitas_mask;
                 i32  numerus;
                 i32  numerus_deletorum;
             OfficinaPiscina* piscina;
       TabulaFriatio  friatio;
    TabulaComparatio  comparatio;
                 f32  factor_maximus;
                 f32  factor_deletorum_maximus;
                 i32  collisiones_totales;
                 i32  distantia_maxima;
} OfficinaTabulaDispersa;

/* Iterator ad omnes introitus ambulandos */
nomen structura {
    OfficinaTabulaDispersa* tabula;
               i32  positus_currens;
} TabulaIterator;


/* ==================================================
 * Creatio
 * ================================================== */

static OfficinaTabulaDispersa*
officina_tabula_dispersa_creare (
             OfficinaPiscina* piscina,
                 i32  capacitas_initialis,
       TabulaFriatio  friatio,
    TabulaComparatio  comparatio);

/* Commoditas: crea cum comparatione chordae (signum) */
static OfficinaTabulaDispersa*
officina_tabula_dispersa_creare_chorda (
    OfficinaPiscina* piscina,
        i32  capacitas_initialis);


/* ==================================================
 * Insertio / Quaestio / Deletio
 * ================================================== */

/* Clavis-valor par inserere
 * VERUM reddit in successu, FALSUM in allocationis fractura
 * Valor existens renovat si clavis iam existit */

static b32
officina_tabula_dispersa_inserere (
    OfficinaTabulaDispersa* tabula,
            OfficinaChorda  clavis,
            vacuum* valor);


/* Clavis per quaestio
 * VERUM reddit et *valor_exitus statuit si inventum
 * FALSUM reddit si non inventum */
static b32
officina_tabula_dispersa_invenire (
    OfficinaTabulaDispersa*  tabula,
            OfficinaChorda   clavis,
            vacuum** valor_out);


/* =================================================
 * Statisticae / Diagnostica
 * ================================================== */

static f32
officina_tabula_dispersa_factor_oneris (
    OfficinaTabulaDispersa* tabula);

static f32
officina_tabula_dispersa_factor_deletorum (
    OfficinaTabulaDispersa* tabula);


/* =================================================
 * Functiones Friationis (in bibliotheca separata ideale)
 * ================================================== */

static i32
officina_tabula_friare_fnv1a (
    OfficinaChorda clavis);

#endif /* TABULA_DISPERSA_H */

/* ================= ex include/xar.h ================= */
/* xar.h - tabula crescens segmentata (growable array, vector)
 * "Codex Tabulae Exponentialis" - segmenta NON contigua: numquam
 * xar_obtinere(x, 0) ut tabulam planam lege
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

/* Constantae (RP pars II, 2026-09-02: tabula segmentorum per xar
 * ALLOCATA, non in capite inscripta - caput DLXXVI -> XLVIII octeti;
 * primum IV, non XVI: ex XI M Xar triviorum in lib/stml.c X M unum
 * elementum tenebant, CXXVIII octeti pro VIII). Segmentum k primum x
 * 2^(k-1) tenet: tabula ordinaria XXIV segmenta = cum primo IV XXXIII M
 * elementa (primum x 2^XXIII); ultra fines REFUSIO
 * nominata (stderr, xar_addere NIHIL), numquam truncatio tacita -
 * xar_creare_magnum tabulam LXIV dat. */
#define XAR_SEGMENTA_ORDINARIA      XXIV    /* tabula ordinaria */
#define XAR_MAXIMUS_SEGMENTORUM     LXIV    /* tabula magna (xar_creare_magnum) */
#define XAR_PRIMUS_SEGMENTUM        IV      /* elementa in primo segmento */

/* Vexilla */
#define XAR_VEXILLUM_ORDINARIUS     ZEPHYRUM    /* Ordinarius: memoria ad zephyrum */
#define XAR_VEXILLUM_SINE_ZEPHYRUM  I           /* Non zephyrum facere memoriam in allocatio */

/* XarIterator - Iterator pro ambulatione omnium elementorum
 */
nomen structura OfficinaXarIterator {
    constans structura OfficinaXar* xar;     /* Tabula iterata */
                              i32  index_currens;         /* Index currens */
                              i32  index_segmenti;        /* Segmentum currens */
                              i32  offset_in_segmento;    /* Offset in segmento */
                           vacuum* basis_segmenti;        /* Basis segmenti currentis */
                              i32  finis_segmenti;        /* Finis segmenti currentis */
} OfficinaXarIterator;

/* XarComparator - Index functionis pro comparationibus
 * Redde: -1 si a < b, 0 si a == b, +1 si a > b
 */
nomen s32 (*OfficinaXarComparator)(constans vacuum* a, constans vacuum* b);


/* ========================================================================
 * FUNCTIONES PRINCIPALES
 * ======================================================================== */

/* Xar Creare - Creare novum xar
 * "Genesis tabulae exponentialis"
 */
static OfficinaXar*
officina_xar_creare (
             OfficinaPiscina* piscina,
                 i32  magnitudo_elementi);

/* Xar Creare Cum Vexillis
 * "Creare cum omnibus optionibus"
 */
static OfficinaXar*
officina_xar_creare_cum_vexillis (
        OfficinaPiscina* piscina,
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
officina_xar_obtinere (
        constans OfficinaXar* xar,
                 i32  index);

/* Xar Obtinere Signatum - Obtinere cum indice signato
 * "Pro iterationibus cum decrementis"
 *
 * Redde: Index ad elementum, vel NIHIL si invalidus vel negativus
 */
vacuum*
officina_xar_obtinere_s (
        constans OfficinaXar* xar,
                 s32  index);


/* ========================================================================
 * ADDITIO
 * ======================================================================== */

/* Xar Addere - Addere elementum ad finem
 * "Addere elementum ad finem"
 *
 * Redde: Index ad elementum novum, vel NIHIL si allocatio fracta
 */
static vacuum*
officina_xar_addere (
        OfficinaXar* xar);


/* ========================================================================
 * UTILITAS
 * ======================================================================== */

/* Xar Numerus - Obtinere numerum elementorum
 * "Obtinere numerum elementorum"
 */
i32
officina_xar_numerus (
        constans OfficinaXar* xar);

/* Xar Truncare - Truncare ad magnitudinem minorem
 * "Truncare ad magnitudinem minorem"
 */
static vacuum
officina_xar_truncare (
        OfficinaXar* xar,
        i32  numerus_novus);

/* Xar Copiare Ad Tabulam - Copiare elementa ad tabulam
 * "Copiare elementa ad tabulam"
 *
 * Redde: Numerus elementorum copiatorum
 */
static i32
officina_xar_copiare_ad_tabulam (
        constans OfficinaXar* xar,
              vacuum* destinatio,
                 i32  initium,
                 i32  numerus);


/* ========================================================================
 * REMOTIO
 * ======================================================================== */

/* Xar Removere Ultimum - Removere elementum ultimum
 * "Removere ultimum - O(1)"
 *
 * Redde: VERUM si successus, FALSUM si xar vacuum
 */
static b32
officina_xar_removere_ultimum (
        OfficinaXar* xar);

/* Xar Removere Cum Ultimo - Swap-remove O(1)
 * "Removere elementum ad indicem mutando cum ultimo"
 *
 * Nota: Non servat ordinem! Utile si ordo non refert.
 * Redde: VERUM si successus, FALSUM si index invalidus
 */
static b32
officina_xar_removere_cum_ultimo (
        OfficinaXar* xar,
        i32  index);

#endif /* XAR_H */

/* ================= ex include/postulata_posix.h ================= */
/* postulata_posix.h - postulata platformae pro superficie POSIX
 *
 * SUTURA praeprocessoris pura: interfacies portabilis, mores
 * per-platformam. glibc sub -std=c89 declarationes POSIX CELAT nisi
 * macro probationis proprietatum ante caput systematis primum
 * definitur; Darwin et musl ordinarie permissivi sunt. Sine hoc
 * capite plagula quaeque POSIX-utens in Linux glibc cadit
 * (tcp_posix.c: XX errores ex radicibus IV celatis - mensuratum).
 *
 * CUR _DEFAULT_SOURCE: sonda Docker 2026-08-03 (glibc 2.35 gcc 11.4;
 * musl 1.2.5 gcc 13.2; VI plagulae x V variantes - acta in actis
 * tabularii 01KYTGNA36) mensuravit: _DEFAULT_SOURCE omnia
 * macro-sanabilia in AMBABUS libc sanat et in Darwin nihil agit.
 * Variantes strictae PEIORES sunt, non aequales: _XOPEN_SOURCE 700
 * et _POSIX_C_SOURCE usleep RE-CELANT (XPG7 sustulit). Decretum
 * 01KZ3RYZWK: caput unum, non definitiones per plagulam.
 *
 * LEX (codex examinis 85 custodit): hoc caput inclusio PRIMA
 * plagulae POSIX-utentis sit - ante caput proprium, ante latina.h.
 * features.h glibc copiam SEMEL figit, primo tactu capitis systematis
 * cuiuslibet; latina.h stddef.h trahit, ergo "prima" ad litteram.
 *
 * Nomen _DEFAULT_SOURCE classis reservatae est (C89 7.1.3) -
 * REFERIMUS interruptorem glibc documentatum, non coinamus (eadem
 * licentia qua externa systematis referuntur).
 */

#ifndef POSTULATA_POSIX_H
#define POSTULATA_POSIX_H

#define _DEFAULT_SOURCE 1

#endif /* POSTULATA_POSIX_H */

/* ================= ex officina/fontes/officina_medulla.h ================= */
/* officina_medulla.h - Medulla: IR registrorum linearis (M1a B)
 *
 * Vocabularium SIGNATUM: officina/vocabularium-medullae.md (DECISUS
 * 2026-07-09) - nomina operationum, suffixa typorum (.s/.i/.f =
 * conventio domus), sigilla (%/$/@). Consilium: officina-m1-spec.md
 * §II; complicationes simulatio-3 C2 (operanda signata + stiva
 * operandorum pro arietate vocare), C3 (origo SEMPER in memoria -
 * pignus visionis; forma textualis eam optionalem fert), C8
 * (MedullaDatum {imago, relocationes} a die primo).
 *
 * FIGURA: modulus = TU unum (conexio ad M2); functio = blocci
 * nominati (@) + registra virtualia nominata (%) + stiva
 * operandorum; instructiones VALORE in bloccis (xar segmentatum -
 * monstratores stabiles). Registra tenent verba 64-bit; TYPI IN
 * OPERATIONIBUS, non in registris. Referentiae per INDICES (blocci,
 * registra, symbola, causae) - textus eas nominibus reddit (C).
 *
 * Disciplina terminatorum: omnis bloccus uno terminatore finitur
 * (salire/ramus/redde/sistere) - medulla_emittere eam custodit.
 * Nulla probatio typorum hic: demissio annotationes semanticae
 * oboedit; lamina exsecutionis futura iudicat (sistere = sedes
 * eius exsecutionis).
 */

#ifndef OFFICINA_MEDULLA_H
#define OFFICINA_MEDULLA_H

/* provenientia - silva.h NON requiritur (tag solum; demissio
 * plena silva.h includet) */
structura SilvaNodus;

#define MEDULLA_TYPUS_NIHIL (-1)   /* operatio sine typo (salire...);
                                    * litera nuda: officina.h idem
                                    * verbatim definit (redefinitio
                                    * identica = custodia gratuita) */


/* ==================================================
 * Fabrica (interfacies prima - Eskil)
 * ================================================== */

MedullaModulus*
medulla_modulum_creare (
    OfficinaPiscina* piscina,
     OfficinaChorda  titulus);

/* Internare per titulum: idem titulus -> idem index. Novum =
 * EXTERNUM; functionem/datum_creare genus concretum ponit. */
s32
medulla_symbolum_internare (
    MedullaModulus* modulus,
            OfficinaChorda  titulus);
constans MedullaSymbolum*
medulla_symbolum_obtinere (
    constans MedullaModulus* modulus,
                        s32  index);

s32
medulla_causam_internare (
    MedullaModulus* modulus,
            OfficinaChorda  causa);
constans OfficinaChorda*
medulla_causam_obtinere (
    constans MedullaModulus* modulus,
                        s32  index);

/* NIHIL si titulus iam definitionem concretam habet */
MedullaFunctio*
medulla_functionem_creare (
    MedullaModulus* modulus,
            OfficinaChorda  titulus,
               s32  typus_reditus,
               b32  est_variadica);

/* -> index registri novi (parametra = registra prima) */
s32
medulla_parametrum_addere (
    MedullaFunctio* functio,
            OfficinaChorda  titulus,
               s32  typus);
/* titulus mensura 0 -> "tN" automaticum */
s32
medulla_registrum_novum (
    MedullaFunctio* functio,
            OfficinaChorda  titulus);
constans OfficinaChorda*
medulla_registrum_titulus (
    constans MedullaFunctio* functio,
                        s32  index);

s32
medulla_bloccum_creare (
    MedullaFunctio* functio,
            OfficinaChorda  titulus);
MedullaBloccus*
medulla_bloccum_obtinere (
    constans MedullaFunctio* functio,
                        s32  index);

/* argumenta vocationis in stivam -> index initii (extra_index) */
s32
medulla_operanda_addere (
               MedullaFunctio* functio,
    constans MedullaOperandum* operanda,
                          s32  numerus);

/* fabricae operandorum (valore) */
MedullaOperandum
medulla_op_nihil (vacuum);
MedullaOperandum
medulla_op_registrum (
    s32 index);
MedullaOperandum
medulla_op_immediatum (
    s64 valor);
MedullaOperandum
medulla_op_immediatum_f (
    f64 valor);
MedullaOperandum
medulla_op_symbolum (
    s32 index);
MedullaOperandum
medulla_op_bloccum (
    s32 index);
MedullaOperandum
medulla_op_causa (
    s32 index);

/* Custos disciplinae: FALSUM si bloccus invalidus/terminatus, op
 * extra fines, aut destinatio registrum ignotum. Terminator
 * bloccum claudit. Instructio VALORE copiatur. */
b32
medulla_emittere (
                MedullaFunctio* functio,
                           s32  bloccus,
    constans MedullaInstructio* instructio);

b32
medulla_op_terminator (
    s32 op);
/* omnes blocci terminati et saltem unus */
b32
medulla_functio_terminata (
    constans MedullaFunctio* functio);

/* NIHIL si titulus iam definitionem concretam habet aut
 * magnitudo_octetorum < I. Imago allocatur et zephyrum-pletur. */
MedullaDatum*
medulla_datum_creare (
    MedullaModulus* modulus,
            OfficinaChorda  titulus,
               i32  magnitudo_octetorum,
               i32  ordinatio);
b32
medulla_datum_scribere (
    MedullaDatum* datum,
             i32  offset,
     constans i8* octeti,
             i32  numerus);
/* locellus relocationis = 8 octeti intra imaginem */
b32
medulla_relocationem_addere (
    MedullaDatum* datum,
             i32  offset,
             s32  symbolum);

MedullaLineae*
medulla_lineas_creare (
    OfficinaPiscina* piscina);
b32
medulla_lineam_ponere (
    MedullaLineae* lineae,
    constans structura SilvaNodus* origo,
    OfficinaChorda via,
    i32 linea);
/* via_out/linea_out NIHIL licent (probatio existentiae) */
b32
medulla_lineam_quaerere (
    constans MedullaLineae* lineae,
    constans structura SilvaNodus* origo,
    OfficinaChorda* via_out,
    i32* linea_out);

#endif /* OFFICINA_MEDULLA_H */

/* ================= ex officina/fontes/officina_medulla_textus.h ================= */
/* officina_medulla_textus.h - Forma textualis medullae (M1a C)
 *
 * QUOD SCRIPTOR EVOMIT, CANON EST (DECISUS interview Q5-Q8 +
 * chunk-C: fluitantia %.17g decimalia; commenta a lectore
 * praetermissa). Vectis M1a: dump(parse(t)) == t byte-idem pro
 * plagulis canonicis; plagulae commentatae vectem idempotentiae
 * habent (parse -> dump -> parse -> dump idem).
 *
 * Sigilla: % registrum, $ symbolum, @ bloccus, "..." causa,
 * nudum immediatum. Lector STRICTUS: forma canonica sola, praeter
 * lineas vacuas et commenta (;) quae praetermittuntur (plagulae
 * manu scriptae). Indentatio: spatia praecedentia praetermissa.
 */

#ifndef OFFICINA_MEDULLA_TEXTUS_H
#define OFFICINA_MEDULLA_TEXTUS_H

/* nomina signata (vocabularium-medullae.md) - fons unicus et
 * scriptoris et lectoris; NIHIL si extra fines */
constans character*
medulla_op_titulus (
    s32 op);
constans character*
medulla_typum_titulus (
    s32 typus);

OfficinaChorda
medulla_textum_scribere (
                    OfficinaPiscina* piscina,
    constans MedullaModulus* modulus);

/* NIHIL + linea_erroris (1-basis; NIHIL licet) in errore */
MedullaModulus*
medulla_textum_legere (
    OfficinaPiscina* piscina,
     OfficinaChorda  textus,
        i32* linea_erroris);

#endif /* OFFICINA_MEDULLA_TEXTUS_H */

/* ================= ex officina/fontes/officina_regio.h ================= */
/* officina_regio.h - Regio: memoria possessa machinulae (M2a)
 *
 * DECISUS (officina-m2-interview.md Q9/Q10 + diagnostica):
 * UNA reservatio mmap magna cum BASI FIXA (determinismus: cursus
 * identici -> monstratores identici -> vestigia differibilia; later
 * primus itineris-temporis v2). Paginae pigre committuntur (macOS
 * nativum) - reservatio gratis, pretium physicum = tactum solum.
 *
 * Monstratores veri SUNT monstratores machinulae (DECISUS 6 -
 * probatio_piscina identitatem monstratorum et ordinationem XVI
 * per valores veros asserit; ansae fictae corpus fallerent).
 *
 * TRES AREAE intra reservationem:
 *   globalia - collocatio conexionis (data, descriptores, cellae
 *              externae); cumulus simplex, numquam liberatur
 *   stiva    - arcae + tabulae registrorum machinulae (cursor
 *              machinulae est; regio fines solum praebet)
 *   acervus  - pons malloc/free/realloc (aedificata); allocator
 *              indicis liberi, bases >=XVI ordinatae SEMPER
 *              (piscina %XVI == 0 per valorem verum asserit),
 *              memoria INDETERMINATA manet (numquam zephyrum -
 *              custodia venenum recognoscibile ponit, non zerum)
 *
 * CUSTODIA (vexillum diagnosticum): venenum 0xAA in allocatione,
 * 0xDD in liberatione; probationes validitatis in liberare semper
 * (viles). Machinula regio_continet ad custodes legere/scribere
 * adhibet.
 *
 * UNA regio uno tempore (basis fixa) - figura, non defectus.
 */

#ifndef OFFICINA_REGIO_H
#define OFFICINA_REGIO_H


/* ==================================================
 * Vita
 * ================================================== */

/* Reservat aream ad basim fixam; NIHIL si basis non data est
 * (numquam basim aliam tacite accipit - determinismus ante
 * commodum). Structura moderaminis ex piscina. */
Regio*
regio_generare (
    OfficinaPiscina* piscina);
vacuum
regio_destruere (
    Regio* regio);


/* ==================================================
 * Fines + custodia
 * ================================================== */

vacuum*
regio_basis (
    constans Regio* regio);
memoriae_index
regio_magnitudo_tota (
    constans Regio* regio);
b32
regio_continet (
     constans Regio* regio,
    constans vacuum* locus);

vacuum
regio_custodiam_ponere (
    Regio* regio,
      b32  custodia);
b32
regio_custodia (
    constans Regio* regio);


/* ==================================================
 * Globalia (collocatio conexionis; cumulus, sine liberatione)
 * ================================================== */

/* ordinatio = potestas duorum (0 -> I); NIHIL si spatium deest */
vacuum*
regio_globalia_allocare (
             Regio* regio,
    memoriae_index  magnitudo_octetorum,
    memoriae_index  ordinatio);
memoriae_index
regio_globalia_usus (
    constans Regio* regio);


/* ==================================================
 * Stiva (fines pro machinula)
 * ================================================== */

vacuum*
regio_stiva_initium (
    constans Regio* regio);
memoriae_index
regio_stiva_magnitudo_octetorum (
    constans Regio* regio);


/* ==================================================
 * Acervus (pons malloc/free/realloc)
 * ================================================== */

/* n rotundatur ad XVI; fructus semper XVI-ordinatus; NIHIL si
 * spatium deest aut n == 0 */
vacuum*
regio_allocare (
             Regio* regio,
    memoriae_index  n);

/* FALSUM = monstrator alienus / iam liber / caput corruptum -
 * machinula halitum honestum ex hoc facit */
b32
regio_liberare (
     Regio* regio,
    vacuum* locus);

/* locus NIHIL -> allocare; contractio -> idem monstrator;
 * amplificatio -> novus + copia + liberatio veteris */
vacuum*
regio_reallocare (
             Regio* regio,
            vacuum* locus,
    memoriae_index  n);

/* census (numeratores diagnosticorum) */
memoriae_index
regio_acervus_usus (
    constans Regio* regio);
memoriae_index
regio_acervus_apex (
    constans Regio* regio);
memoriae_index
regio_numerus_allocationum (
    constans Regio* regio);
memoriae_index
regio_numerus_liberationum (
    constans Regio* regio);

#endif /* OFFICINA_REGIO_H */

/* ================= ex officina/fontes/officina_conexio.h ================= */
/* officina_conexio.h - Conexio: nexor moduli medullae (M2a)
 *
 * DECISUS (simulatio-4 C1-C6 + interview Q2/Q3):
 * - Tabula symbolorum GLOBALIS + ordinatio translationis per modulum
 *   (index localis -> index globalis). Moduli IMMUTABILES manent -
 *   nulla instructio rescribitur (1.15M instructiones intactae).
 * - NECTERE MUNDUM: omnes moduli bibliothecae + modulus probationis
 *   (manifesta per plagulam non existunt nec computari possunt -
 *   fundus-corpus). Symbola insoluta -> DECIPULAE pigrae (ruunt
 *   SOLUM SI vocata - sedes sisterae in commissura nexus).
 * - DESCRIPTORES: 16 octeti in globalibus regionis per functionem -
 *   inscriptio descriptoris EST valor monstratoris functionis
 *   (relocationes symbola functionum petere possunt - aureum
 *   statica.medulla). signum = commissura nativa M5.
 * - CELLAE externae: stdin/stdout/stderr (ansae opacae 0/1/2) +
 *   errno (0) - symbola data externa nota; pons aedificatorum eas
 *   curat (M2b).
 * - RELOCATIO ADDITIVA: locellus = inscriptio symboli + octeti
 *   priores locelli (addendum). Unus mechanismus: plana/membrum/
 *   elementum/functio.
 *
 * Ordo: creare -> modulum_addere (iterum) -> nectere -> quaerere.
 * Conflictus (definitio duplex) = fractura honesta cum querela.
 */

#ifndef OFFICINA_CONEXIO_H
#define OFFICINA_CONEXIO_H

/* signa descriptorum - numquam 0 (memoria zephyrata numquam
 * descriptor validus); M5 addet NATIVUM. AEDIFICATUM: machinula
 * decipulas notas in aedificata IN SITU rescribit (signum solum -
 * inscriptiones descriptorum, iam in relocationibus sarcitae,
 * immotae manent) */
#define CONEXIO_SIGNUM_INTERPRETATUM 1
#define CONEXIO_SIGNUM_DECIPULA      2
#define CONEXIO_SIGNUM_AEDIFICATUM   3


/* ==================================================
 * Vita + nexus
 * ================================================== */

Conexio*
conexio_creare (
    OfficinaPiscina* piscina,
      Regio* regio);

/* FALSUM: post nectere, aut definitio duplex (querela ponitur;
 * conexio post fracturam invalida) */
b32
conexio_modulum_addere (
           Conexio* conexio,
    MedullaModulus* modulus);

/* collocat data, descriptores, cellas, decipulas; relocationes
 * ADDITIVE sarcit. Decipulae NON sunt fracturae (numeratae +
 * nominatae). FALSUM = fractura vera (querela). */
b32
conexio_nectere (
    Conexio* conexio);

constans OfficinaChorda*
conexio_querela (
    constans Conexio* conexio);
/* symbolum querelae (mensura 0 si nullum) */
constans OfficinaChorda*
conexio_querela_symbolum (
    constans Conexio* conexio);


/* ==================================================
 * Quaestio (post nectere)
 * ================================================== */

/* -I si absens */
s32
conexio_symbolum_quaerere (
    constans Conexio* conexio,
              OfficinaChorda  titulus);
constans ConexioSymbolum*
conexio_symbolum_obtinere (
    constans Conexio* conexio,
                 s32  index);
i32
conexio_numerus_symbolorum (
    constans Conexio* conexio);

/* sedes per titulum; NIHIL si absens ($main -> descriptor) */
vacuum*
conexio_sedes_quaerere (
    constans Conexio* conexio,
              OfficinaChorda  titulus);

/* (modulus, index localis) -> index globalis; -I si invalidus */
s32
conexio_symbolum_globale (
    constans Conexio* conexio,
                 s32  modulus_index,
                 s32  index_localis);

constans ConexioFunctioNexa*
conexio_functionem_obtinere (
    constans Conexio* conexio,
                 s64  index);
i32
conexio_numerus_functionum (
    constans Conexio* conexio);

i32
conexio_numerus_modulorum (
    constans Conexio* conexio);
constans MedullaModulus*
conexio_modulum_obtinere (
    constans Conexio* conexio,
                 s32  index);


/* ==================================================
 * Census (vectis M2a: decipulae numeratae + nominatae)
 * ================================================== */

i32
conexio_numerus_datorum (
    constans Conexio* conexio);
i32
conexio_numerus_cellarum (
    constans Conexio* conexio);
i32
conexio_numerus_decipularum (
    constans Conexio* conexio);
constans OfficinaChorda*
conexio_decipulam_obtinere (
    constans Conexio* conexio,
                 s32  index);

#endif /* OFFICINA_CONEXIO_H */

/* ================= ex officina/fontes/officina_machinula.h ================= */
/* officina_machinula.h - Machinula: interpres medullae (M2b)
 *
 * DECISUS (officina-m2-interview.md):
 * - LATITUDO (Q4): canonicum-in-scriptura - omnis operatio verbum
 *   PLENUM 64-bit scribit, extensum per signum suffixi (movere.s32
 *   signum extendit, legere.i8 zephyrum); f64 = figura bitorum;
 *   f32 = figura 32-bit zephyrum-extensa. Lectiones registrorum
 *   verbum UT EST sumunt (productori creditur); immediata in
 *   lectione canonicantur. Pons variadicus verba plana legit.
 * - Dispensatio commutatione (C89 -pedantic goto computatum vetat;
 *   tectum ad M5).
 * - RECORDATOR VOLATUS SEMPER ACTIVUS (Q16): anulus figurarum
 *   (functio/index planus); relatio halitus in quovis exitu
 *   abnormali: positio + stiva vocationum + cauda anuli, cum
 *   via:linea per lineas distillatas, ad stderr (registra NON in
 *   relatione - inspectio per vindicem, M3; auditum milliarii
 *   commentum mendax correxit).
 * - Probatio stivae explicita in vocare (Q10): fractura = halitus
 *   honestus.
 * - Aedificata: tabula statica interna (Q11; series publica ad M4);
 *   OMNES vocationes per CANALEM UNUM (_aedificatum_vocare - sedes
 *   memoriae/reddendi v2 nominata, Q12). Ligatio: decipulae notae
 *   in AEDIFICATUM rescribuntur (signum solum, in situ).
 * - exit = vexillum halitus (sine longjmp); redde principalis =
 *   status processus.
 * - sistere exsecutum = halitus cum causa + positione (+relatio).
 *
 * Numeratores (diagnostica #5): per-operationem, vocationes,
 * aedificata, apex stivae - statio tabularum saltus his decernitur.
 */

#ifndef OFFICINA_MACHINULA_H
#define OFFICINA_MACHINULA_H

/* vexilla recusationum (M4b sessio: determinismus replicationis).
 * custos: officina.h amalgami eadem valoribus arabicis definit */
#ifndef MACHINULA_RECUSARE_SCRIPTURAS
#define MACHINULA_RECUSARE_SCRIPTURAS  I    /* fopen w/a, remove... */
#define MACHINULA_RECUSARE_TEMPUS      II   /* time/clock/gettimeofday */
#define MACHINULA_RECUSARE_INITUM      IV   /* lectiones stdin */
#endif

/* ligat aedificata (decipulae notae -> AEDIFICATUM in situ) et
 * cellas (ansae/errno) capit; NIHIL si quid deest */
Machinula*
machinula_creare (
    OfficinaPiscina* piscina,
    Conexio* conexio,
      Regio* regio);

/* lineae distillatae pro relatione halitus (NIHIL licet - positio
 * tunc sine via:linea) */
vacuum
machinula_lineas_praebere (
                 Machinula* machinula,
                       s32  modulus_index,
    constans MedullaLineae* lineae);

/* M4b sessio: flumina norma redirigere (ansa 0/1/2; NIHIL =
 * flumen hospitis ordinarium restitutum). Sutura capturae actorum. */
vacuum
machinula_ansam_ponere (
    Machinula* machinula,
          s32  ansa,
         FILE* plagula);

/* M4b sessio: vexilla recusationum ponere (MACHINULA_RECUSARE_*;
 * 0 = omnia licita). Aedilis recusatus = halitus RECUSATIO. */
vacuum
machinula_recusationes_ponere (
    Machinula* machinula,
          i32  vexilla);

/* M4b: ansae 3+ (plagulae fopen programmatis) claudere - hygiene
 * demolitionis generationis (C6; fd non effluunt) */
vacuum
machinula_ansas_claudere (
    Machinula* machinula);

/* functionem nominatam currit (functio sine parametris aut
 * parametra zephyro implentur); status stivae/halitus purgatur -
 * currere iterabile, globalia region PERSISTUNT */
MachinulaExitus
machinula_currere (
    Machinula* machinula,
       OfficinaChorda  titulus_functionis);

/* M3 chunk 4: gressus + puncta + inspectio (superficies vindicis).
 * aperire = status purgatus + tabulatum primum (VERUM si paratum);
 * gradus = instructio UNA (VERUM = pergendum); pergere = usque ad
 * halitum aut pausam, genus halitus redditur; resumptio ex pausa
 * punctum sub cursore transit (restitue-grade-repone). currere =
 * involucrum compatibile (aperire + pergere + relatio + exitus). */
b32
machinula_aperire (
    Machinula* machinula,
       OfficinaChorda  titulus_functionis);
b32
machinula_gradus (
    Machinula* machinula);
s32
machinula_pergere (
    Machinula* machinula);

/* puncta: patching exemplaris CONGELATI (proprium machinulae - IR
 * intactum; nullum pretium in via calida). Sedes = (index functionis
 * conexionis, index instructionis planus) - indicium easdem
 * coordinatas dat. */
b32
machinula_punctum_ponere (
    Machinula* machinula,
          s32  functio_index,
          i32  instructio);
b32
machinula_punctum_tollere (
    Machinula* machinula,
          s32  functio_index,
          i32  instructio);

/* inspectio (VM pausata): tabulatum_index 0 = imum,
 * numerus - I = cacumen */
s64
machinula_halitus_codex (
    constans Machinula* machinula);
i32
machinula_tabulata_numerus (
    constans Machinula* machinula);
b32
machinula_positionem_inspicere (
    constans Machinula* machinula,
                   i32  tabulatum_index,
                   s32* functio_index_out,
                   i32* instructio_out);
b32
machinula_registrum_legere (
    constans Machinula* machinula,
                   i32  tabulatum_index,
                   i32  index_registri,
                   i64* valor_out);
/* anulus volatus: retro_index 0 = novissima figura, crescens =
 * vetustior; FALSUM ultra caudam (tabula anuli vindicis) */
b32
machinula_anulum_inspicere (
    constans Machinula* machinula,
                   i32  retro_index,
                   s32* functio_index_out,
                   i32* instructio_out);

/* census */
i64
machinula_numerus_instructionum (
    constans Machinula* machinula);
i64
machinula_numerus_op (
    constans Machinula* machinula,
                   s32  op);
/* numerus instructionum in exemplari CONGELATO functionis (index =
 * index functionis conexionis; 0 si extra fines). Probatio figens
 * pacti ambulationis canonicae (M3): numerus indicii ex ambulatione
 * collectionis == hic ex ambulatione congelationis. */
i32
machinula_numerus_instructionum_planarum (
    constans Machinula* machinula,
                   s32  functio_index);
i64
machinula_numerus_vocationum (
    constans Machinula* machinula);
i64
machinula_numerus_aedificatorum (
    constans Machinula* machinula);
memoriae_index
machinula_stiva_apex (
    constans Machinula* machinula);

#endif /* OFFICINA_MACHINULA_H */

/* ================= ex officina/fontes/officina_demissio.h ================= */
/* officina_demissio.h - Demissio: arbor typata -> medulla (M1b)
 *
 * UNICUM domicilium ubi semantica C89 explicita fit: conversiones
 * annotatae (M0b {naturalis, conversus} - contractus CONGELATUS)
 * operationes conversionum visibiles fiunt; arithmetica monstratorum
 * per formas scalatur; && / || / ternarius in fluxum linearizantur.
 * NULLA probatio typorum hic - demissio annotationibus OBOEDIT
 * (lamina exsecutionis futura iudicat); numquam ruit - inlowerabilia
 * -> sistere cum causa classificata (postura tree-sitter).
 *
 * Silva TRANS AMALGAMA SOLUM (silva.h - officina a silva pendet EX
 * ARCHITECTURA; dependentia externa unica documentata). Praesumit
 * sem POST-FLIP (typatio canonicae-relativa; fistula bis-analysans
 * VOCANTIS est - fusor percursum exscribit).
 *
 * NB sem NON-constans: forma pigra (formam_computare) et aestimator
 * tabulas per quaestionem mutant (INTENTIO const dicebat -
 * emendatio, vide worklog).
 */

#ifndef OFFICINA_DEMISSIO_H
#define OFFICINA_DEMISSIO_H

/* Modulus ex TU uno (conexio ad M2). titulus_moduli = via fontis
 * ("lib/chorda.c") - praefixum staticorum internorum ex eo. */
MedullaModulus*
demissio_currere (
                  OfficinaPiscina* piscina,
    constans SilvaParsura* parsura,
           SilvaSemantica* sem,
                   OfficinaChorda  titulus_moduli);

/* Distillatio linearum (M2a): origo -> (via, linea) DUM frons vivit
 * (parsura->expansio necessaria pro viis fontium; nulla via publica
 * post liberationem). Post hanc arbores liberari possunt - clavis
 * origo opaca manet. Consumptor: recordator volatus (M2b);
 * proto-indicium (M3). */
MedullaLineae*
demissio_lineas_colligere (
                    OfficinaPiscina* piscina,
    constans MedullaModulus* modulus,
      constans SilvaParsura* parsura);

#endif /* OFFICINA_DEMISSIO_H */

/* ================= ex lib/piscina.c ================= */

#ifndef PISCINA_DEBUG
#define PISCINA_DEBUG FALSUM /* Muta ad VERUM pro imprimere debugging,
                              * vel -DPISCINA_DEBUG=1 in linea compilandi */
#endif


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

structura OfficinaPiscina {
            Alveus* primus;
            Alveus* nunc;
    memoriae_index  mensura_alvei_initia;
         character* titulus;
               b32  est_dynamicum;
        memoriae_index  maximus_usus;
    memoriae_index  usus_currens;           /* summa offsetuum, incrementalis */
    memoriae_index  numerus_allocationum;   /* historia, numquam minuitur */
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
        imprimere("[PISCINA %s] %s: %lu bytes\n", piscinae_titulum,
                  operatio, (insignatus longus)mensura);
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

    alveus->capacitas  = capacitas;
    alveus->offset     = ZEPHYRUM;
    alveus->sequens    = NIHIL;

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
               OfficinaPiscina* piscina,
        memoriae_index  mensura,
        memoriae_index  ordinatio,
                   b32  fatalis)
{
        memoriae_index  ordinatus_offset;
        memoriae_index  necessaria;
                vacuum* ptr;

    si (!piscina || mensura == ZEPHYRUM) redde NIHIL;

    ordinatus_offset = _proxima_ordinatio(piscina->nunc->offset,
        ordinatio);
    necessaria = ordinatus_offset + mensura;

    /* Si allocatio in alveum nunc non capit, invenire vel generare alveum novum */
    dum (necessaria > piscina->nunc->capacitas)
    {
        si (piscina->nunc->sequens)
        {
            /* Transire ad alveum sequentem */
            piscina->nunc = piscina->nunc->sequens;
            ordinatus_offset = _proxima_ordinatio(piscina->nunc->offset,
                ordinatio);
            necessaria = ordinatus_offset + mensura;
        }
        alioquin si (piscina->est_dynamicum)
        {
            Alveus* alveus_novum;

            /* Generare alveum novum */
            memoriae_index capacitas_nova =
                piscina->mensura_alvei_initia * II;

            /* Petitio maior quam duplum: alveus ad mensuram petitionis
             * (+ basis), BASIS INTACTA. Olim basis ad hanc mensuram
             * ratchetabatur et numquam decrescebat: lib/stml.c alvei
             * 1, 2, 3, 6, 9, 18, 27, 54 MB, ultimo 54 MB VII tenente -
             * XXXIX% otiosum (RP §6, 2026-09-02). */
            si (necessaria > capacitas_nova)
            {
                capacitas_nova = necessaria
                    + piscina->mensura_alvei_initia;
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

            piscina->nunc->sequens  = alveus_novum;
            piscina->nunc           = alveus_novum;

            ordinatus_offset = _proxima_ordinatio(piscina->nunc->offset,
                ordinatio);
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
                imprimere("ALLOCATIO PISCINAE FRACTA: %s (indigentia %lu)\n",
                          piscina->titulus ? piscina->titulus : "nemo",
                          (insignatus longus)necessaria);
                exire(I);
            }
            redde NIHIL;
        }
    }


        /* Allocare ex alveo nunc. Apex INCREMENTALITER (2026-09-02): olim
     * omnes alvei per allocationem percurrebantur (I.II M allocationes
     * x XVII alvei in lib/stml.c = XIII% foliorum profili); summa
     * offsetuum mutatur solum hic (delta), in vacare (nihil) et in
     * reficere (recomputata semel). */
    ptr = (character*)(piscina->nunc->buffer) + ordinatus_offset;
    piscina->usus_currens += necessaria - piscina->nunc->offset;
    piscina->nunc->offset = necessaria;
    si (piscina->usus_currens > piscina->maximus_usus)
    {
        piscina->maximus_usus = piscina->usus_currens;
    }
    piscina->numerus_allocationum += I;

    _debug_imprimere(piscina->titulus ? piscina->titulus : "nemo",
        "allocare", mensura);

    redde ptr;
}


/* ===========================================================
 * GENERATIO
 * =========================================================== */

OfficinaPiscina*
officina_piscina_generare_dynamicum (
    constans character* piscinae_titulum,
        memoriae_index  mensura_alvei_initia)
{
    Alveus* alveus_primus;

    OfficinaPiscina* piscina = (OfficinaPiscina*)memoriae_allocare(magnitudo(OfficinaPiscina));
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
        piscina->est_dynamicum     = VERUM;
    piscina->maximus_usus          = ZEPHYRUM;
    piscina->usus_currens          = ZEPHYRUM;
    piscina->numerus_allocationum  = ZEPHYRUM;

    si (piscinae_titulum)
    {
        memoriae_index mensura_tituli = strlen(piscinae_titulum);
        piscina->titulus = (character*)memoriae_allocare(mensura_tituli
            + I);

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
officina_piscina_destruere (
        OfficinaPiscina* piscina)
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
officina_piscina_allocare (
           OfficinaPiscina* piscina,
    memoriae_index  mensura)
{
    redde _allocare_interna(piscina, mensura,
        PISCINA_ORDINATIO_ORDINARIA, VERUM);
}

static vacuum*
officina_piscina_allocare_ordinatum (
           OfficinaPiscina* piscina,
    memoriae_index  mensura,
    memoriae_index  ordinatio)
{
    redde _allocare_interna(piscina, mensura, ordinatio, VERUM);
}

/* ================= ex lib/chorda.c ================= */

#define CHORDA_FRIATUM_OFFSET  2166136261U
#define CHORDA_FRIATUM_PRIMUS    16777619U


/* ==================================================
 * Constructores
 * ================================================== */

static OfficinaChorda
officina_chorda_ex_literis (
    constans character* litterae,
               OfficinaPiscina* piscina)
{
    OfficinaChorda  fructus;
       i32  mensura;
        i8* allocatus;

    si (!litterae || !piscina)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

      mensura = (i32)strlen(litterae);
    allocatus = (i8*)officina_piscina_allocare(piscina, mensura);

    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    memcpy(allocatus, litterae, mensura);
    fructus.mensura  = mensura;
    fructus.datum    = allocatus;
    redde fructus;
}

static OfficinaChorda
officina_chorda_transcribere (
     OfficinaChorda  s,
    OfficinaPiscina* piscina)
{
    OfficinaChorda  fructus;
        i8* allocatus;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)officina_piscina_allocare(piscina, s.mensura);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    memcpy(allocatus, s.datum, s.mensura);

    fructus.mensura  = s.mensura;
    fructus.datum    = allocatus;

    redde fructus;
}

static b32
officina_chorda_aequalis_literis (
                OfficinaChorda  s,
    constans character* cstr)
{
    i32 len;
    i32 i;

    si (!cstr)
    {
        redde FALSUM;
    }

    /* Mensura literarum */
    len = ZEPHYRUM;
    dum (cstr[len] != '\0')
    {
        len++;
    }

    si (s.mensura != len)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < len; i++)
    {
        si ((character)s.datum[i] != cstr[i])
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

static s32
officina_chorda_comparare (
    OfficinaChorda a,
    OfficinaChorda b)
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

structura OfficinaChordaAedificator {
                i8* buffer;
    memoriae_index  capacitas;
    memoriae_index  offset;
           OfficinaPiscina* piscina;
               i32  indentatio_gradus;
};


/* ==================================================
 * ADIUTORES INTERNI
 * ================================================== */

interior memoriae_index
_proxima_capacitas (
    memoriae_index nunc)
{
    /* Duplica capacitatem donec satis habeamus */
    redde nunc > ZEPHYRUM ? nunc * II : XVI;
}

interior b32
_crescere (
    OfficinaChordaAedificator* aedificator,
       memoriae_index  necessaria)
{
    memoriae_index  capacitas_nova;
                i8* buffer_novum;

    capacitas_nova = aedificator->capacitas;
    dum (capacitas_nova < necessaria)
    {
        capacitas_nova = _proxima_capacitas(capacitas_nova);
    }

    buffer_novum = (i8*)officina_piscina_allocare(aedificator->piscina,
        capacitas_nova);
    si (!buffer_novum) redde FALSUM;

    si (aedificator->buffer && aedificator->offset > ZEPHYRUM)
    {
        memcpy(buffer_novum, aedificator->buffer, aedificator->offset);
    }

    aedificator->buffer     = buffer_novum;
    aedificator->capacitas  = capacitas_nova;

    redde VERUM;
}

interior b32
_appendere_interna (
    OfficinaChordaAedificator* aedificator,
          constans i8* datum,
       memoriae_index  mensura)
{
    memoriae_index necessaria;

    /* Appendix vacua bona est */
    si (!aedificator || !datum || mensura == ZEPHYRUM)
    {
        redde mensura == ZEPHYRUM;
    }

    necessaria = aedificator->offset + mensura;

    si (necessaria > aedificator->capacitas)
    {
        si (!_crescere(aedificator, necessaria)) redde FALSUM;
    }

    memcpy(aedificator->buffer + aedificator->offset, datum, mensura);
    aedificator->offset += mensura;

    redde VERUM;
}

interior memoriae_index
_format_integer_i32 (
               i32  n,
                i8* buffer,
    memoriae_index  capacitas)
{
         character cstr[CXXXII];
               s32 mensura_signed;
    memoriae_index mensura;

    mensura_signed = snprintf(cstr, (memoriae_index)magnitudo(cstr),
        "%u", n);
    si (mensura_signed < ZEPHYRUM) redde ZEPHYRUM;

    mensura = (memoriae_index)mensura_signed;
    si (mensura >= capacitas) redde ZEPHYRUM;

    memcpy(buffer, cstr, mensura);
    redde mensura;
}


/* ==================================================
 * Creatio
 * ================================================== */

static OfficinaChordaAedificator*
officina_chorda_aedificator_creare (
           OfficinaPiscina* piscina,
    memoriae_index  capacitas_initialis)
{
    OfficinaChordaAedificator* aedificator;
                   i8* buffer;

    si (!piscina || capacitas_initialis == ZEPHYRUM) redde NIHIL;

    aedificator = (OfficinaChordaAedificator*)officina_piscina_allocare(
                                        piscina,
                                        magnitudo(OfficinaChordaAedificator));
    si (!aedificator) redde NIHIL;

    buffer = (i8*)officina_piscina_allocare(piscina, capacitas_initialis);
    si (!buffer) redde NIHIL;

    aedificator->buffer             = buffer;
    aedificator->capacitas          = capacitas_initialis;
    aedificator->offset             = ZEPHYRUM;
    aedificator->piscina            = piscina;
    aedificator->indentatio_gradus  = ZEPHYRUM;

    redde aedificator;
}


/* ==================================================
 * Appendere - Character
 * ================================================== */

static b32
officina_chorda_aedificator_appendere_character (
    OfficinaChordaAedificator* aedificator,
            character  c)
{
    i8 ch = (i8)c;
    redde _appendere_interna(aedificator, &ch, I);
}


/* ==================================================
 * Appendere - Chordae
 * ================================================== */

static b32
officina_chorda_aedificator_appendere_literis (
     OfficinaChordaAedificator* aedificator,
    constans character* cstr)
{
    memoriae_index mensura;

    si (!aedificator || !cstr) redde FALSUM;

    mensura = strlen(cstr);
    redde _appendere_interna(aedificator, (constans i8*)cstr, mensura);
}

static b32
officina_chorda_aedificator_appendere_chorda (
    OfficinaChordaAedificator* aedificator,
               OfficinaChorda  s)
{
    si (!aedificator || !s.datum) redde FALSUM;

    redde _appendere_interna(aedificator, s.datum, s.mensura);
}

static b32
officina_chorda_aedificator_appendere_i32 (
    OfficinaChordaAedificator* aedificator,
                  i32  n)
{
                i8 buffer[CXXXII];
    memoriae_index mensura;

    si (!aedificator) redde FALSUM;

    mensura = _format_integer_i32(n, buffer, magnitudo(buffer));
    si (mensura == ZEPHYRUM) redde FALSUM;

    redde _appendere_interna(aedificator, buffer, mensura);
}

static b32
officina_chorda_aedificator_appendere_repetita (
     OfficinaChordaAedificator* aedificator,
             character  c,
                   i32  numerus)
{
    i32 i;

    /* numerus i32 insignatus - custodia negativi mortua remota
	 * (2026-07-17); familia indentationis non-negativa per push/pop */
    si (!aedificator) redde FALSUM;

    si (numerus == ZEPHYRUM) redde VERUM;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (!officina_chorda_aedificator_appendere_character(aedificator, c))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

static OfficinaChorda
officina_chorda_aedificator_finire (
    OfficinaChordaAedificator* aedificator)
{
    OfficinaChorda result;

    si (!aedificator)
    {
        result.mensura  = ZEPHYRUM;
        result.datum    = NIHIL;
        redde result;
    }

    result.mensura  = (i32)aedificator->offset;
    result.datum    = aedificator->buffer;

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
officina_friatio_fnv1a (
    OfficinaChorda clavis)
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
#define SHA1_F0(b, c, d) (((b) & (c)) | ((~(b)) & (d)))
#define SHA1_F1(b, c, d) ((b) ^ (c) ^ (d))
#define SHA1_F2(b, c, d) (((b) & (c)) | ((b) & (d)) | ((c) & (d)))
#define SHA1_F3(b, c, d) ((b) ^ (c) ^ (d))

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
#define SHA256_CH(x, y, z)  (((x) & (y)) ^ ((~(x)) & (z)))
#define SHA256_MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
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
officina_tabula_friare_fnv1a (
    OfficinaChorda clavis)
{
    redde officina_friatio_fnv1a(clavis);
}


/* ==================================================
 * Adiutor: Proxima Potentia Binaria
 * ================================================== */

interior i32
_proxima_potentia_2 (
    i32 n)
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
    OfficinaTabulaDispersa* tabula,
            OfficinaChorda  clavis,
               i32  friatum,
               b32* inventum)
{
    i32 positus;
    i32 distantia;
    i32 positus_primus_deletum;

    *inventum               = FALSUM;
    positus                 = friatum & tabula->capacitas_mask;
    distantia               = ZEPHYRUM;
    positus_primus_deletum  = (i32)-I;

    dum (tabula->sloti[positus].status != SLOT_VACUUM)
    {
        si (tabula->sloti[positus].status == SLOT_OCCUPATUM)
        {
            si (   tabula->sloti[positus].hash_cachatum == friatum
                && tabula->comparatio(
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
_tabula_dispersa_crescere (
    OfficinaTabulaDispersa* tabula)
{
    Slotus* sloti_veteres;
       i32  capacitas_vetus;
       i32  i;

    sloti_veteres    = tabula->sloti;
    capacitas_vetus  = tabula->capacitas;

    tabula->capacitas       = capacitas_vetus * II;
    tabula->capacitas_mask  = tabula->capacitas - I;

    tabula->sloti = (Slotus*)officina_piscina_allocare(
        tabula->piscina,
        tabula->capacitas * magnitudo(Slotus));

    si (!tabula->sloti)
    {
        tabula->sloti           = sloti_veteres;
        tabula->capacitas       = capacitas_vetus;
        tabula->capacitas_mask  = tabula->capacitas - I;
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < tabula->capacitas; i++)
    {
        tabula->sloti[i].status = SLOT_VACUUM;
    }

    tabula->numerus              = ZEPHYRUM;
    tabula->numerus_deletorum    = ZEPHYRUM;
    tabula->collisiones_totales  = ZEPHYRUM;
    tabula->distantia_maxima     = ZEPHYRUM;

    per (i = ZEPHYRUM; i < capacitas_vetus; i++)
    {
        si (sloti_veteres[i].status == SLOT_OCCUPATUM)
        {
            officina_tabula_dispersa_inserere(
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

static OfficinaTabulaDispersa*
officina_tabula_dispersa_creare (
             OfficinaPiscina* piscina,
                 i32  capacitas_initialis,
       TabulaFriatio  friatio,
    TabulaComparatio  comparatio)
{
    OfficinaTabulaDispersa* tabula;
               i32  capacitas;
               i32  i;

    si (!piscina || !friatio || !comparatio)
    {
        redde NIHIL;
    }

    capacitas = _proxima_potentia_2(capacitas_initialis);
    si (capacitas < XVI) capacitas = XVI;

    tabula = (OfficinaTabulaDispersa*)officina_piscina_allocare(
                                piscina,
                                magnitudo(OfficinaTabulaDispersa));
    si (!tabula) redde NIHIL;

    tabula->sloti = (Slotus*)officina_piscina_allocare(
                                piscina,
                                capacitas * magnitudo(Slotus));
    si (!tabula->sloti) redde NIHIL;

    per (i = ZEPHYRUM; i < capacitas; i++)
    {
        tabula->sloti[i].status          = SLOT_VACUUM;
        tabula->sloti[i].clavis.mensura  = ZEPHYRUM;
        tabula->sloti[i].clavis.datum    = NIHIL;
        tabula->sloti[i].valor           = NIHIL;
        tabula->sloti[i].hash_cachatum   = ZEPHYRUM;
    }

    tabula->capacitas                 = capacitas;
    tabula->capacitas_mask            = capacitas - I;
    tabula->numerus                   = ZEPHYRUM;
    tabula->numerus_deletorum         = ZEPHYRUM;
    tabula->piscina                   = piscina;
    tabula->comparatio                = comparatio;
    tabula->friatio                   = friatio;
    tabula->factor_maximus            = 0.75f;
    tabula->factor_deletorum_maximus  = 0.25f;
    tabula->collisiones_totales       = ZEPHYRUM;
    tabula->distantia_maxima          = ZEPHYRUM;

    redde tabula;
}

static OfficinaTabulaDispersa*
officina_tabula_dispersa_creare_chorda (
    OfficinaPiscina* piscina,
        i32  capacitas_initialis)
{
    redde officina_tabula_dispersa_creare(
        piscina,
        capacitas_initialis,
        officina_tabula_friare_fnv1a,
        (TabulaComparatio)officina_chorda_comparare);
}


/* ==================================================
 * Insertio
 * ================================================== */

static b32
officina_tabula_dispersa_inserere (
    OfficinaTabulaDispersa* tabula,
            OfficinaChorda  clavis,
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
    si (   officina_tabula_dispersa_factor_oneris(tabula) > tabula->factor_maximus
        || officina_tabula_dispersa_factor_deletorum(tabula) > tabula->factor_deletorum_maximus)
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

    tabula->sloti[positus].clavis         = clavis;
    tabula->sloti[positus].valor          = valor;
    tabula->sloti[positus].hash_cachatum  = friatum;
    tabula->sloti[positus].status         = SLOT_OCCUPATUM;
    tabula->numerus++;

    redde VERUM;
}


/* ==================================================
 * Quaestio
 * ================================================== */

static b32
officina_tabula_dispersa_invenire (
    OfficinaTabulaDispersa*  tabula,
            OfficinaChorda   clavis,
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


/* ==================================================
 * Statisticae
 * ================================================== */

static f32
officina_tabula_dispersa_factor_oneris (
    OfficinaTabulaDispersa* tabula)
{
    si (!tabula || tabula->capacitas == ZEPHYRUM)
    {
        redde 0.0f;
    }
    redde (f32)tabula->numerus / (f32)tabula->capacitas;
}

static f32
officina_tabula_dispersa_factor_deletorum (
    OfficinaTabulaDispersa* tabula)
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
computare_magnitudinem_segmenti (
    constans OfficinaXar* xar,
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
        imprimere("FRACTA: xar segmentum nimis altum: %d\n",
            index_segmenti);
        imprimere("        (impossibilis cum indices i32 - corruptio?)\n");
        exire(I);
    }

    /* Segmenta sequentia: duplicant */
    redde xar->magnitudo_primi << shift_amount;
}

/* Refusio nominata: tabula segmentorum exhausta (RP 7.obj.1 - finis
 * nominatus, non truncatio tacita). xar_addere NIHIL reddit. */
interior vacuum
_refusio_segmentorum (
    constans OfficinaXar* xar)
{
    fprintf(stderr, "xar: tabula segmentorum exhausta (%u segmenta,"
        " %u elementa) - xar_creare_magnum pro tabula LXIV\n",
        xar->segmenta_maxima, xar->numerus_elementorum);
}

/* Allocare Segmentum
 * "Partem allocare"
 *
 * Redde: VERUM si successus, FALSUM si error
 */
interior b32
allocare_segmentum (
    OfficinaXar* xar,
    i32  index_segmenti)
{
               i32  magnitudo_segmenti;
    memoriae_index  magnitudo_memoriae;
            vacuum* memoria;

    si (index_segmenti >= xar->segmenta_maxima)
    {
        _refusio_segmentorum(xar);
        redde FALSUM;  /* Nimis multa segmenta! */
    }

    si (xar->segmenta[index_segmenti])
    {
        redde VERUM;  /* Iam allocatus */
    }

    /* Computare magnitudinem segmenti */
    magnitudo_segmenti = computare_magnitudinem_segmenti(xar,
        index_segmenti);
    magnitudo_memoriae =
        (memoriae_index)magnitudo_segmenti * xar->magnitudo_elementi;

    /* Allocare ex piscina */
    memoria = officina_piscina_allocare(xar->piscina, magnitudo_memoriae);
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

/* Genesis: caput + tabula segmentorum UNA allocatione (tabula post
 * caput), mensura tabulae data. */
interior OfficinaXar*
_creare (
    OfficinaPiscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi,
        i32  vexilla,
        i32  segmenta_maxima)
{
               OfficinaXar* xar;
    memoriae_index  mensura;
               i32  i;

    si (!piscina || magnitudo_elementi == ZEPHYRUM)
    {
        redde NIHIL;
    }
    mensura = magnitudo(OfficinaXar)
            + (memoriae_index)segmenta_maxima * magnitudo(vacuum*);
    xar = (OfficinaXar*)officina_piscina_allocare_ordinatum(piscina, mensura, VIII);
    si (!xar)
    {
        redde NIHIL;
    }
    xar->numerus_elementorum  = ZEPHYRUM;
    xar->magnitudo_elementi   = magnitudo_elementi;
    xar->magnitudo_primi     = magnitudo_primi ? magnitudo_primi
                                               : XAR_PRIMUS_SEGMENTUM;
    xar->numerus_segmentorum  = ZEPHYRUM;
    xar->capacitas_totalis    = ZEPHYRUM;
    xar->vexilla              = vexilla;
    xar->segmenta_maxima      = segmenta_maxima;
    xar->piscina              = piscina;
    xar->segmenta             = (vacuum**)(xar + I);
    per (i = ZEPHYRUM; i < segmenta_maxima; i++)
    {
        xar->segmenta[i] = NIHIL;
    }
    redde xar;
}


/* ========================================================================
 * FUNCTIONES CREATIONIS
 * ======================================================================== */

/* Xar Creare */
static OfficinaXar*
officina_xar_creare (
    OfficinaPiscina* piscina,
        i32  magnitudo_elementi)
{
    redde officina_xar_creare_cum_vexillis(piscina,
                                   magnitudo_elementi,
                                   XAR_PRIMUS_SEGMENTUM,
                                   XAR_VEXILLUM_ORDINARIUS);
}

/* Xar Creare Cum Vexillis
 * "Genesis tabulae exponentialis - cum omnibus optionibus"
 */
static OfficinaXar*
officina_xar_creare_cum_vexillis (
    OfficinaPiscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi,
        i32  vexilla)
{
    redde _creare(piscina, magnitudo_elementi, magnitudo_primi, vexilla,
                  XAR_SEGMENTA_ORDINARIA);
}

/* Bitum summum positum (0-basatum) numeri positivi: scansio binaria
 * quinque graduum (C89 sine intrinsecis). Pro xar_locare: segmentum k
 * >= II indices [primum x 2^(k-1), primum x 2^k) tenet, ergo cum
 * q = index / primum, k = bitum_summum(q) + I - tempore constanti,
 * non gyro duplicandi (XVI% foliorum profili in lib/stml.c, 2026-09-02). */
interior i32
_bitum_summum (
    i32 q)
{
    i32 k = ZEPHYRUM;

    si (q >> XVI)
    { k += XVI; q >>= XVI;
    }
    si (q >> VIII)
    { k += VIII; q >>= VIII;
    }
    si (q >> IV)
    { k += IV;   q >>= IV;
    }
    si (q >> II)
    { k += II;   q >>= II;
    }
    si (q >> I)
    { k += I;
    }
    redde k;
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
officina_xar_locare (
    constans     OfficinaXar* xar,
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
            locatio->index_segmenti      = ZEPHYRUM;
            locatio->offset_in_segmento  = index;
            locatio->magnitudo_segmenti  = xar->magnitudo_primi;
        }
        alioquin
        {
            /* Segmentum secundum */
            locatio->index_segmenti      = I;
            locatio->offset_in_segmento  = index - xar->magnitudo_primi;
            locatio->magnitudo_segmenti  = xar->magnitudo_primi;
        }
        locatio->basis_segmenti =
            xar->segmenta[locatio->index_segmenti];
        redde VERUM;
    }

    /* Computatio directa pro segmentis exponentialibus
	 *
	 * Usans manipulationem bitorum pro quaestio exponentia
	 */
    /* Forma clausa (2026-09-02, olim gyrus duplicandi): q = index /
     * primum; segmentum = bitum_summum(q) + I; magnitudo = primum <<
     * bitum; offset = index - magnitudo. Eaedem fines ac gyrus (probatio
     * xar: numerus segmentorum et capacitas per fines contra
     * referentiam). */
    {
        i32 q = index / xar->magnitudo_primi;
        i32 b = _bitum_summum(q);

        index_segmenti      = b + I;
        magnitudo_segmenti  = xar->magnitudo_primi << b;
        index_adiustus      = index - magnitudo_segmenti;
    }

    si (index_segmenti >= xar->segmenta_maxima)
    {
        redde FALSUM;  /* Index nimis magnus (vocator additionis clamat) */
    }

    locatio->index_segmenti      = index_segmenti;
    locatio->offset_in_segmento  = index_adiustus;
    locatio->magnitudo_segmenti  = magnitudo_segmenti;
    locatio->basis_segmenti      = xar->segmenta[index_segmenti];

    redde VERUM;
}

/* Xar Obtinere
 * "Obtinere elementum"
 */
vacuum*
officina_xar_obtinere (
    constans OfficinaXar* xar,
             i32  index)
{
    XarLocatio  locatio;
            i8* basis;

    si (!xar || index >= xar->numerus_elementorum)
    {
        redde NIHIL;
    }

    si (!officina_xar_locare(xar, index, &locatio))
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
    redde basis
        + (locatio.offset_in_segmento * xar->magnitudo_elementi);
}

/* Xar Obtinere Signatum
 * "Pro iterationibus cum decrementis"
 */
vacuum*
officina_xar_obtinere_s (
    constans OfficinaXar* xar,
             s32  index)
{
    si (index < ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde officina_xar_obtinere(xar, (i32)index);
}


/* ========================================================================
 * ADDITIO
 * ======================================================================== */

/* Xar Addere
 * "Addere ad finem"
 */
static vacuum*
officina_xar_addere (
    OfficinaXar* xar)
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
    si (!officina_xar_locare(xar, index_novus, &locatio))
    {
        _refusio_segmentorum(xar);
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
    redde basis
        + (locatio.offset_in_segmento * xar->magnitudo_elementi);
}


/* ========================================================================
 * UTILITAS
 * ======================================================================== */

/* Xar Numerus */
i32
officina_xar_numerus (
    constans OfficinaXar* xar)
{
    redde xar ? xar->numerus_elementorum : ZEPHYRUM;
}

/* Xar Truncare */
static vacuum
officina_xar_truncare (
    OfficinaXar* xar,
    i32  numerus_novus)
{
    si (xar && numerus_novus < xar->numerus_elementorum)
    {
        xar->numerus_elementorum = numerus_novus;
    }
}

/* Xar Copiare Ad Tabulam */
static i32
officina_xar_copiare_ad_tabulam (
    constans OfficinaXar* xar,
          vacuum* destinatio,
             i32  initium,
             i32  numerus)
{
        i8* destinatio_bytes;
       i32  i;
    vacuum* src;

    destinatio_bytes = (i8*)destinatio;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        src = officina_xar_obtinere(xar, initium + i);
        si (!src)
        {
            redde i;  /* Reddere numerum copiatum cum successu */
        }
        memcpy(
            destinatio_bytes + (i * xar->magnitudo_elementi),
            src,
            xar->magnitudo_elementi);
    }

    redde numerus;
}


/* ========================================================================
 * REMOTIO
 * ======================================================================== */

/* Xar Removere Ultimum
 * "Removere ultimum - O(1)"
 */
static b32
officina_xar_removere_ultimum (
    OfficinaXar* xar)
{
    si (!xar || xar->numerus_elementorum == ZEPHYRUM)
    {
        redde FALSUM;
    }

    xar->numerus_elementorum--;
    redde VERUM;
}

/* Xar Removere Cum Ultimo
 * "Swap-remove - O(1)"
 */
static b32
officina_xar_removere_cum_ultimo (
    OfficinaXar* xar,
    i32  index)
{
    vacuum* elementum_ad_remotionem;
    vacuum* elementum_ultimum;
       i32  index_ultimus;

    si (!xar || xar->numerus_elementorum == ZEPHYRUM)
    {
        redde FALSUM;
    }

    si (index >= xar->numerus_elementorum)
    {
        redde FALSUM;
    }

    index_ultimus = xar->numerus_elementorum - I;

    /* Si index est ultimus, simpliciter removere */
    si (index == index_ultimus)
    {
        xar->numerus_elementorum--;
        redde VERUM;
    }

    /* Mutare cum ultimo et removere ultimum */
    elementum_ad_remotionem  = officina_xar_obtinere(xar, index);
    elementum_ultimum        = officina_xar_obtinere(xar, index_ultimus);

    si (!elementum_ad_remotionem || !elementum_ultimum)
    {
        redde FALSUM;
    }

    /* Copiare ultimum ad positionem remotionis */
    memcpy(elementum_ad_remotionem, elementum_ultimum,
        xar->magnitudo_elementi);

    /* Decrementare numerum */
    xar->numerus_elementorum--;

    redde VERUM;
}

/* ================= ex officina/fontes/officina_medulla.c ================= */


/* ==================================================
 * Auxilia interna
 * ================================================== */

interior b32
_chordae_aequales (
    OfficinaChorda a,
    OfficinaChorda b)
{
    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }
    si (a.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde memcmp(a.datum, b.datum,
        (memoriae_index)a.mensura) == ZEPHYRUM;
}


/* ==================================================
 * Modulus + symbola + causae
 * ================================================== */

MedullaModulus*
medulla_modulum_creare (
    OfficinaPiscina* piscina,
     OfficinaChorda  titulus)
{
    MedullaModulus* modulus;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    modulus = (MedullaModulus*)officina_piscina_allocare(piscina,
        (memoriae_index)magnitudo(MedullaModulus));
    si (modulus == NIHIL)
    {
        redde NIHIL;
    }
    memset(modulus, ZEPHYRUM, magnitudo(MedullaModulus));
    modulus->piscina = piscina;
    modulus->titulus = officina_chorda_transcribere(titulus, piscina);
    modulus->functiones = officina_xar_creare(piscina,
        (i32)magnitudo(MedullaFunctio*));
    modulus->data = officina_xar_creare(piscina,
        (i32)magnitudo(MedullaDatum*));
    modulus->symbola = officina_xar_creare(piscina,
        (i32)magnitudo(MedullaSymbolum));
    modulus->causae = officina_xar_creare(piscina, (i32)magnitudo(OfficinaChorda));
    si (   modulus->functiones == NIHIL || modulus->data == NIHIL
        || modulus->symbola    == NIHIL || modulus->causae == NIHIL)
    {
        redde NIHIL;
    }
    redde modulus;
}

s32
medulla_symbolum_internare (
    MedullaModulus* modulus,
            OfficinaChorda  titulus)
{
                i32  i;
                i32  m;
    MedullaSymbolum* symbolum;

    si (modulus == NIHIL || titulus.mensura == ZEPHYRUM)
    {
        redde -I;
    }
    m = officina_xar_numerus(modulus->symbola);
    per (i = ZEPHYRUM; i < m; i++)
    {
        symbolum = (MedullaSymbolum*)officina_xar_obtinere(modulus->symbola,
            i);

        si (   symbolum != NIHIL
            && _chordae_aequales(symbolum->titulus, titulus))
        {
            redde (s32)i;
        }
    }
    symbolum = (MedullaSymbolum*)officina_xar_addere(modulus->symbola);
    si (symbolum == NIHIL)
    {
        redde -I;
    }
    symbolum->titulus = officina_chorda_transcribere(titulus,
        modulus->piscina);
    symbolum->genus = MEDULLA_SYMBOLUM_EXTERNUM;
    symbolum->index = -I;
    redde (s32)(officina_xar_numerus(modulus->symbola) - I);
}

constans MedullaSymbolum*
medulla_symbolum_obtinere (
    constans MedullaModulus* modulus,
                        s32  index)
{
    si (modulus == NIHIL)
    {
        redde NIHIL;
    }
    redde (constans MedullaSymbolum*)officina_xar_obtinere_s(
        modulus->symbola, index);
}

s32
medulla_causam_internare (
    MedullaModulus* modulus,
            OfficinaChorda  causa)
{
       i32  i;
       i32  m;
    OfficinaChorda* sedes;

    si (modulus == NIHIL)
    {
        redde -I;
    }
    m = officina_xar_numerus(modulus->causae);
    per (i = ZEPHYRUM; i < m; i++)
    {
        sedes = (OfficinaChorda*)officina_xar_obtinere(modulus->causae, i);

        si (sedes != NIHIL && _chordae_aequales(*sedes, causa))
        {
            redde (s32)i;
        }
    }
    sedes = (OfficinaChorda*)officina_xar_addere(modulus->causae);
    si (sedes == NIHIL)
    {
        redde -I;
    }
    *sedes = officina_chorda_transcribere(causa, modulus->piscina);
    redde (s32)(officina_xar_numerus(modulus->causae) - I);
}

constans OfficinaChorda*
medulla_causam_obtinere (
    constans MedullaModulus* modulus,
                        s32  index)
{
    si (modulus == NIHIL)
    {
        redde NIHIL;
    }
    redde (constans OfficinaChorda*)officina_xar_obtinere_s(modulus->causae, index);
}


/* ==================================================
 * Functio + registra + blocci
 * ================================================== */

MedullaFunctio*
medulla_functionem_creare (
    MedullaModulus* modulus,
            OfficinaChorda  titulus,
               s32  typus_reditus,
               b32  est_variadica)
{
     MedullaFunctio*  functio;
     MedullaFunctio** locus_functionis;
    MedullaSymbolum*  symbolum;
                s32   index_symboli;

    si (modulus == NIHIL || titulus.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    index_symboli = medulla_symbolum_internare(modulus, titulus);
    si (index_symboli < ZEPHYRUM)
    {
        redde NIHIL;
    }
    symbolum = (MedullaSymbolum*)officina_xar_obtinere_s(modulus->symbola,
        index_symboli);
    si (symbolum->genus != (s32)MEDULLA_SYMBOLUM_EXTERNUM)
    {
        redde NIHIL;   /* definitio duplex */
    }

    functio = (MedullaFunctio*)officina_piscina_allocare(modulus->piscina,
        (memoriae_index)magnitudo(MedullaFunctio));
    si (functio == NIHIL)
    {
        redde NIHIL;
    }
    memset(functio, ZEPHYRUM, magnitudo(MedullaFunctio));
    functio->titulus        = symbolum->titulus;   /* copia internata */
    functio->typus_reditus  = typus_reditus;
    functio->est_variadica  = est_variadica;
    functio->parametra = officina_xar_creare(modulus->piscina,
        (i32)magnitudo(MedullaParametrum));
    functio->blocci = officina_xar_creare(modulus->piscina,
        (i32)magnitudo(MedullaBloccus));
    functio->registra = officina_xar_creare(modulus->piscina,
        (i32)magnitudo(OfficinaChorda));
    functio->operanda = officina_xar_creare(modulus->piscina,
        (i32)magnitudo(MedullaOperandum));
    si (   functio->parametra == NIHIL || functio->blocci == NIHIL
        || functio->registra  == NIHIL || functio->operanda == NIHIL)
    {
        redde NIHIL;
    }

    locus_functionis = (MedullaFunctio**)officina_xar_addere(
        modulus->functiones);
    si (locus_functionis == NIHIL)
    {
        redde NIHIL;
    }
    *locus_functionis  = functio;
    symbolum->genus    = MEDULLA_SYMBOLUM_FUNCTIO;
    symbolum->index    = (s32)(officina_xar_numerus(modulus->functiones) - I);
    redde functio;
}

s32
medulla_registrum_novum (
    MedullaFunctio* functio,
            OfficinaChorda  titulus)
{
    OfficinaChorda* sedes;
       s32  index;

    si (functio == NIHIL)
    {
        redde -I;
    }
    index = (s32)officina_xar_numerus(functio->registra);
    sedes = (OfficinaChorda*)officina_xar_addere(functio->registra);
    si (sedes == NIHIL)
    {
        redde -I;
    }
    si (titulus.mensura == ZEPHYRUM)
    {
        /* nomen automaticum tN (N = index) */
        character littera[XXXII];
           OfficinaChorda temporarium;
              s32 longitudo = (s32)sprintf(littera, "t%d", (int)index);

        temporarium.datum    = (i8*)littera;
        temporarium.mensura  = (i32)longitudo;
        *sedes = officina_chorda_transcribere(temporarium,
            functio->registra->piscina);
    }
    alioquin
    {
        *sedes = officina_chorda_transcribere(titulus,
            functio->registra->piscina);
    }
    redde index;
}

s32
medulla_parametrum_addere (
    MedullaFunctio* functio,
            OfficinaChorda  titulus,
               s32  typus)
{
    MedullaParametrum* parametrum;
                  s32  index;

    si (functio == NIHIL)
    {
        redde -I;
    }
    index = medulla_registrum_novum(functio, titulus);
    si (index < ZEPHYRUM)
    {
        redde -I;
    }
    parametrum = (MedullaParametrum*)officina_xar_addere(functio->parametra);
    si (parametrum == NIHIL)
    {
        redde -I;
    }
    parametrum->index = index;
    parametrum->typus = typus;
    redde index;
}

constans OfficinaChorda*
medulla_registrum_titulus (
    constans MedullaFunctio* functio,
                        s32  index)
{
    si (functio == NIHIL)
    {
        redde NIHIL;
    }
    redde (constans OfficinaChorda*)officina_xar_obtinere_s(functio->registra, index);
}

s32
medulla_bloccum_creare (
    MedullaFunctio* functio,
            OfficinaChorda  titulus)
{
    MedullaBloccus* bloccus;

    si (functio == NIHIL || titulus.mensura == ZEPHYRUM)
    {
        redde -I;
    }
    bloccus = (MedullaBloccus*)officina_xar_addere(functio->blocci);
    si (bloccus == NIHIL)
    {
        redde -I;
    }
    bloccus->titulus = officina_chorda_transcribere(titulus,
        functio->blocci->piscina);
    bloccus->instructiones = officina_xar_creare(functio->blocci->piscina,
        (i32)magnitudo(MedullaInstructio));
    bloccus->terminatus = FALSUM;
    si (bloccus->instructiones == NIHIL)
    {
        redde -I;
    }
    redde (s32)(officina_xar_numerus(functio->blocci) - I);
}

MedullaBloccus*
medulla_bloccum_obtinere (
    constans MedullaFunctio* functio,
                        s32  index)
{
    si (functio == NIHIL)
    {
        redde NIHIL;
    }
    redde (MedullaBloccus*)officina_xar_obtinere_s(functio->blocci, index);
}

s32
medulla_operanda_addere (
               MedullaFunctio* functio,
    constans MedullaOperandum* operanda,
                          s32  numerus)
{
    s32 initium;
    s32 i;

    si (   functio == NIHIL || numerus < ZEPHYRUM
        || (numerus > ZEPHYRUM && operanda == NIHIL))
    {
        redde -I;
    }
    initium = (s32)officina_xar_numerus(functio->operanda);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        MedullaOperandum* sedes =
            (MedullaOperandum*)officina_xar_addere(functio->operanda);

        si (sedes == NIHIL)
        {
            redde -I;
        }
        *sedes = operanda[i];
    }
    redde initium;
}


/* ==================================================
 * Fabricae operandorum
 * ================================================== */

interior MedullaOperandum
_operandum (
    s32 genus)
{
    MedullaOperandum operandum;

    memset(&operandum, ZEPHYRUM, magnitudo(MedullaOperandum));
    operandum.genus = genus;
    redde operandum;
}

MedullaOperandum
medulla_op_nihil (vacuum)
{
    redde _operandum(MEDULLA_OPERANDUM_NIHIL);
}

MedullaOperandum
medulla_op_registrum (
    s32 index)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_REGISTRUM);

    operandum.datum.index = index;
    redde operandum;
}

MedullaOperandum
medulla_op_immediatum (
    s64 valor)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_IMMEDIATUM);

    operandum.datum.immediatum = valor;
    redde operandum;
}

MedullaOperandum
medulla_op_immediatum_f (
    f64 valor)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_IMMEDIATUM_F);

    operandum.datum.immediatum_f = valor;
    redde operandum;
}

MedullaOperandum
medulla_op_symbolum (
    s32 index)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_SYMBOLUM);

    operandum.datum.index = index;
    redde operandum;
}

MedullaOperandum
medulla_op_bloccum (
    s32 index)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_BLOCCUS);

    operandum.datum.index = index;
    redde operandum;
}

MedullaOperandum
medulla_op_causa (
    s32 index)
{
    MedullaOperandum operandum = _operandum(MEDULLA_OPERANDUM_CAUSA);

    operandum.datum.index = index;
    redde operandum;
}


/* ==================================================
 * Emissio + disciplina terminatorum
 * ================================================== */

b32
medulla_op_terminator (
    s32 op)
{
    redde op == (s32)MEDULLA_OP_SALIRE
        || op == (s32)MEDULLA_OP_RAMUS
        || op == (s32)MEDULLA_OP_REDDE
        || op == (s32)MEDULLA_OP_SISTERE;
}

b32
medulla_emittere (
                MedullaFunctio* functio,
                           s32  bloccus,
    constans MedullaInstructio* instructio)
{
       MedullaBloccus* sedes_blocci;
    MedullaInstructio* sedes;

    si (functio == NIHIL || instructio == NIHIL)
    {
        redde FALSUM;
    }
    si (   instructio->op < ZEPHYRUM
        || instructio->op >= (s32)MEDULLA_OP_NUMERUS)
    {
        redde FALSUM;
    }
    si (   instructio->destinatio >= ZEPHYRUM
        && instructio->destinatio
            >= (s32)officina_xar_numerus(functio->registra))
    {
        redde FALSUM;   /* registrum ignotum */
    }
    sedes_blocci = (MedullaBloccus*)officina_xar_obtinere_s(functio->blocci,
        bloccus);
    si (sedes_blocci == NIHIL || sedes_blocci->terminatus)
    {
        redde FALSUM;
    }
    sedes = (MedullaInstructio*)officina_xar_addere(
        sedes_blocci->instructiones);
    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    *sedes = *instructio;
    si (medulla_op_terminator(instructio->op))
    {
        sedes_blocci->terminatus = VERUM;
    }
    redde VERUM;
}

b32
medulla_functio_terminata (
    constans MedullaFunctio* functio)
{
    i32 i;
    i32 m;

    si (functio == NIHIL)
    {
        redde FALSUM;
    }
    m = officina_xar_numerus(functio->blocci);
    si (m == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MedullaBloccus* bloccus =
            (constans MedullaBloccus*)officina_xar_obtinere(functio->blocci,
                i);

        si (bloccus == NIHIL || !bloccus->terminatus)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Datum + relocationes (C8: imago + relocationes a die primo)
 * ================================================== */

MedullaDatum*
medulla_datum_creare (
    MedullaModulus* modulus,
            OfficinaChorda  titulus,
               i32  magnitudo_octetorum,
               i32  ordinatio)
{
       MedullaDatum*  datum;
       MedullaDatum** locus_dati;
    MedullaSymbolum*  symbolum;
                s32   index_symboli;

    si (   modulus == NIHIL || titulus.mensura == ZEPHYRUM
        || magnitudo_octetorum < I || ordinatio < I)
    {
        redde NIHIL;
    }
    index_symboli = medulla_symbolum_internare(modulus, titulus);
    si (index_symboli < ZEPHYRUM)
    {
        redde NIHIL;
    }
    symbolum = (MedullaSymbolum*)officina_xar_obtinere_s(modulus->symbola,
        index_symboli);
    si (symbolum->genus != (s32)MEDULLA_SYMBOLUM_EXTERNUM)
    {
        redde NIHIL;   /* definitio duplex */
    }

    datum = (MedullaDatum*)officina_piscina_allocare(modulus->piscina,
        (memoriae_index)magnitudo(MedullaDatum));
    si (datum == NIHIL)
    {
        redde NIHIL;
    }
    memset(datum, ZEPHYRUM, magnitudo(MedullaDatum));
    datum->titulus              = symbolum->titulus;
    datum->magnitudo_octetorum  = magnitudo_octetorum;
    datum->ordinatio            = ordinatio;
    datum->imago = (i8*)officina_piscina_allocare(modulus->piscina,
        (memoriae_index)magnitudo_octetorum);
    datum->relocationes = officina_xar_creare(modulus->piscina,
        (i32)magnitudo(MedullaRelocatio));
    si (datum->imago == NIHIL || datum->relocationes == NIHIL)
    {
        redde NIHIL;
    }
    memset(datum->imago, ZEPHYRUM,
        (memoriae_index)magnitudo_octetorum);

    locus_dati = (MedullaDatum**)officina_xar_addere(modulus->data);
    si (locus_dati == NIHIL)
    {
        redde NIHIL;
    }
    *locus_dati      = datum;
    symbolum->genus  = MEDULLA_SYMBOLUM_DATUM;
    symbolum->index  = (s32)(officina_xar_numerus(modulus->data) - I);
    redde datum;
}

b32
medulla_datum_scribere (
    MedullaDatum* datum,
             i32  offset,
     constans i8* octeti,
             i32  numerus)
{
    si (datum == NIHIL || octeti == NIHIL)
    {
        redde FALSUM;
    }
    si (   offset > datum->magnitudo_octetorum
        || numerus > datum->magnitudo_octetorum - offset)
    {
        redde FALSUM;
    }
    si (numerus > ZEPHYRUM)
    {
        memcpy(datum->imago + offset, octeti,
            (memoriae_index)numerus);
    }
    redde VERUM;
}

b32
medulla_relocationem_addere (
    MedullaDatum* datum,
             i32  offset,
             s32  symbolum)
{
    MedullaRelocatio* relocatio;

    si (datum == NIHIL || symbolum < ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (   datum->magnitudo_octetorum < VIII
        || offset > datum->magnitudo_octetorum - VIII)
    {
        redde FALSUM;   /* locellus 8 octetorum extra imaginem */
    }
    relocatio = (MedullaRelocatio*)officina_xar_addere(datum->relocationes);
    si (relocatio == NIHIL)
    {
        redde FALSUM;
    }
    relocatio->offset    = offset;
    relocatio->symbolum  = symbolum;
    redde VERUM;
}


/* ==================================================
 * Lineae distillatae (M2a) - clavis = octeti monstratoris origo
 * (numquam dereferendus - pattern DemissioIntroitus)
 * ================================================== */

nomen structura {
     i8 clavis_octeti[magnitudo(vacuum*)];
    s32 via_index;
    i32 linea;
} LineaIntroitus;

MedullaLineae*
medulla_lineas_creare (
    OfficinaPiscina* piscina)
{
    MedullaLineae* lineae;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    lineae = (MedullaLineae*)officina_piscina_allocare(piscina,
        magnitudo(MedullaLineae));
    si (lineae == NIHIL)
    {
        redde NIHIL;
    }
    lineae->piscina  = piscina;
    lineae->viae     = officina_xar_creare(piscina, (i32)magnitudo(OfficinaChorda));
    lineae->lineae   = officina_tabula_dispersa_creare_chorda(piscina, CCLVI);
    si (lineae->viae == NIHIL || lineae->lineae == NIHIL)
    {
        redde NIHIL;
    }
    redde lineae;
}

interior s32
_viam_internare (
    MedullaLineae* lineae,
           OfficinaChorda  via)
{
       i32  i;
       i32  numerus = officina_xar_numerus(lineae->viae);
    OfficinaChorda* sedes;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        constans OfficinaChorda* nota = (constans OfficinaChorda*)officina_xar_obtinere(
            lineae->viae, i);

        si (   nota->mensura == via.mensura
            && (via.mensura == ZEPHYRUM
                || memcmp(nota->datum, via.datum,
                       (memoriae_index)via.mensura) == ZEPHYRUM))
        {
            redde (s32)i;
        }
    }
    sedes = (OfficinaChorda*)officina_xar_addere(lineae->viae);
    si (sedes == NIHIL)
    {
        redde -I;
    }
    *sedes = officina_chorda_transcribere(via, lineae->piscina);
    redde (s32)numerus;
}

b32
medulla_lineam_ponere (
    MedullaLineae* lineae,
    constans structura SilvaNodus* origo,
    OfficinaChorda via,
    i32 linea)
{
    LineaIntroitus* introitus;
            OfficinaChorda  clavis;
               s32  via_index;

    si (lineae == NIHIL || origo == NIHIL)
    {
        redde FALSUM;
    }
    si (medulla_lineam_quaerere(lineae, origo, NIHIL, NIHIL))
    {
        redde VERUM;   /* iam distillata */
    }
    via_index = _viam_internare(lineae, via);
    si (via_index < ZEPHYRUM)
    {
        redde FALSUM;
    }
    introitus = (LineaIntroitus*)officina_piscina_allocare(lineae->piscina,
        magnitudo(LineaIntroitus));
    si (introitus == NIHIL)
    {
        redde FALSUM;
    }
    memcpy(introitus->clavis_octeti, &origo,
        magnitudo(introitus->clavis_octeti));
    introitus->via_index  = via_index;
    introitus->linea      = linea;
    clavis.datum          = introitus->clavis_octeti;
    clavis.mensura        = (i32)magnitudo(introitus->clavis_octeti);
    redde officina_tabula_dispersa_inserere(lineae->lineae, clavis,
        (vacuum*)introitus);
}

b32
medulla_lineam_quaerere (
    constans MedullaLineae* lineae,
    constans structura SilvaNodus* origo,
    OfficinaChorda* via_out,
    i32* linea_out)
{
                         i8  octeti[magnitudo(vacuum*)];
                     OfficinaChorda  clavis;
                     vacuum* valor = NIHIL;
    constans LineaIntroitus* introitus;

    si (lineae == NIHIL || origo == NIHIL)
    {
        redde FALSUM;
    }
    memcpy(octeti, &origo, magnitudo(octeti));
    clavis.datum    = octeti;
    clavis.mensura  = (i32)magnitudo(octeti);
    si (!officina_tabula_dispersa_invenire(lineae->lineae, clavis, &valor))
    {
        redde FALSUM;
    }
    introitus = (constans LineaIntroitus*)valor;
    si (via_out != NIHIL)
    {
        constans OfficinaChorda* via = (constans OfficinaChorda*)officina_xar_obtinere(
            lineae->viae, (i32)introitus->via_index);

        si (via != NIHIL)
        {
            *via_out = *via;
        }
        alioquin
        {
            via_out->datum    = NIHIL;
            via_out->mensura  = ZEPHYRUM;
        }
    }
    si (linea_out != NIHIL)
    {
        *linea_out = introitus->linea;
    }
    redde VERUM;
}

/* ================= ex officina/fontes/officina_medulla_textus.c ================= */


/* ==================================================
 * Nomina signata (ORDO = ordo enumerationum!)
 * ================================================== */

interior constans character* OP_NOMINA[MEDULLA_OP_NUMERUS] = {
    "movere",
    "addere", "subtrahere", "multiplicare", "dividere", "residuum",
    "negare",
    "et", "vel", "aut", "complementum", "sinistrorsum",
    "dextrorsum",
    "aequalis", "inaequalis", "minor", "minor_aequalis", "maior",
    "maior_aequalis",
    "extendere", "amputare", "fluitare", "integrare", "dilatare",
    "artare",
    "legere", "scribere", "arca", "locus", "copia",
    "salire", "ramus", "redde", "vocare", "sistere"
};

interior constans character* TYPI_NOMINA[MEDULLA_TYPUS_NUMERUS] = {
    "i8", "i16", "i32", "i64",
    "s8", "s16", "s32", "s64",
    "f32", "f64"
};

constans character*
medulla_op_titulus (
    s32 op)
{
    si (op < ZEPHYRUM || op >= (s32)MEDULLA_OP_NUMERUS)
    {
        redde NIHIL;
    }
    redde OP_NOMINA[op];
}

constans character*
medulla_typum_titulus (
    s32 typus)
{
    si (typus < ZEPHYRUM || typus >= (s32)MEDULLA_TYPUS_NUMERUS)
    {
        redde NIHIL;
    }
    redde TYPI_NOMINA[typus];
}


/* ==================================================
 * SCRIPTOR - quod evomit, canon est
 * ================================================== */

interior vacuum
_chordam_evadere (
    OfficinaChordaAedificator* aed,
               OfficinaChorda  textus)
{
    i32 i;

    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        character c = (character)textus.datum[i];

        si (c == '\\')
        {
            officina_chorda_aedificator_appendere_literis(aed, "\\\\");
        }
        alioquin si (c == '"')
        {
            officina_chorda_aedificator_appendere_literis(aed, "\\\"");
        }
        alioquin si (c == '\n')
        {
            officina_chorda_aedificator_appendere_literis(aed, "\\n");
        }
        alioquin si (c == '\t')
        {
            officina_chorda_aedificator_appendere_literis(aed, "\\t");
        }
        alioquin
        {
            officina_chorda_aedificator_appendere_character(aed, c);
        }
    }
}

interior vacuum
_s64_scribere (
    OfficinaChordaAedificator* aed,
                  s64  valor)
{
    character littera[XXXII];

    sprintf(littera, "%lld", valor);
    officina_chorda_aedificator_appendere_literis(aed, littera);
}

interior vacuum
_operandum_scribere (
            OfficinaChordaAedificator* aed,
      constans MedullaModulus* modulus,
      constans MedullaFunctio* functio,
    constans MedullaOperandum* operandum)
{
    commutatio (operandum->genus)
    {
        casus (s32)MEDULLA_OPERANDUM_REGISTRUM:
        {
            constans OfficinaChorda* titulus = medulla_registrum_titulus(
                functio, operandum->datum.index);

            officina_chorda_aedificator_appendere_character(aed, '%');
            si (titulus != NIHIL)
            {
                officina_chorda_aedificator_appendere_chorda(aed, *titulus);
            }
            frange;
        }
        casus (s32)MEDULLA_OPERANDUM_IMMEDIATUM:
        {
            _s64_scribere(aed, operandum->datum.immediatum);
            frange;
        }
        casus (s32)MEDULLA_OPERANDUM_IMMEDIATUM_F:
        {
            character littera[XL];
                  i32 k;
                  b32 signatum_fluitantis = FALSUM;

            sprintf(littera, "%.17g",
                operandum->datum.immediatum_f);
            /* integri toti (%.17g "100") signum fluitantis
             * servandum - aliter lector immediatum INTEGRALE
             * relegit (genus mutatur, textus byte-idem manet:
             * cautio subdola) */
            per (k = ZEPHYRUM; littera[k] != '\0'; k++)
            {
                si (   littera[k] == '.' || littera[k] == 'e'
                    || littera[k] == 'E')
                {
                    signatum_fluitantis = VERUM;
                }
            }
            officina_chorda_aedificator_appendere_literis(aed, littera);
            si (!signatum_fluitantis)
            {
                officina_chorda_aedificator_appendere_literis(aed, ".0");
            }
            frange;
        }
        casus (s32)MEDULLA_OPERANDUM_SYMBOLUM:
        {
            constans MedullaSymbolum* symbolum =
                medulla_symbolum_obtinere(modulus,
                    operandum->datum.index);

            officina_chorda_aedificator_appendere_character(aed, '$');
            si (symbolum != NIHIL)
            {
                officina_chorda_aedificator_appendere_chorda(aed,
                    symbolum->titulus);
            }
            frange;
        }
        casus (s32)MEDULLA_OPERANDUM_BLOCCUS:
        {
            constans MedullaBloccus* bloccus =
                medulla_bloccum_obtinere(functio,
                    operandum->datum.index);

            officina_chorda_aedificator_appendere_character(aed, '@');
            si (bloccus != NIHIL)
            {
                officina_chorda_aedificator_appendere_chorda(aed,
                    bloccus->titulus);
            }
            frange;
        }
        casus (s32)MEDULLA_OPERANDUM_CAUSA:
        {
            constans OfficinaChorda* causa = medulla_causam_obtinere(
                modulus, operandum->datum.index);

            officina_chorda_aedificator_appendere_character(aed, '"');
            si (causa != NIHIL)
            {
                _chordam_evadere(aed, *causa);
            }
            officina_chorda_aedificator_appendere_character(aed, '"');
            frange;
        }
        ordinarius:
        {
            frange;   /* NIHIL numquam scribitur */
        }
    }
}

interior vacuum
_instructionem_scribere (
             OfficinaChordaAedificator* aed,
       constans MedullaModulus* modulus,
       constans MedullaFunctio* functio,
    constans MedullaInstructio* instructio)
{
    officina_chorda_aedificator_appendere_repetita(aed, ' ', IV);
    si (instructio->destinatio >= ZEPHYRUM)
    {
        constans OfficinaChorda* titulus = medulla_registrum_titulus(
            functio, instructio->destinatio);

        officina_chorda_aedificator_appendere_character(aed, '%');
        si (titulus != NIHIL)
        {
            officina_chorda_aedificator_appendere_chorda(aed, *titulus);
        }
        officina_chorda_aedificator_appendere_literis(aed, " = ");
    }
    officina_chorda_aedificator_appendere_literis(aed,
        medulla_op_titulus(instructio->op));
    si (instructio->typus != MEDULLA_TYPUS_NIHIL)
    {
        officina_chorda_aedificator_appendere_character(aed, '.');
        officina_chorda_aedificator_appendere_literis(aed,
            medulla_typum_titulus(instructio->typus));
    }
    si (instructio->typus_secundus != MEDULLA_TYPUS_NIHIL)
    {
        officina_chorda_aedificator_appendere_character(aed, '.');
        officina_chorda_aedificator_appendere_literis(aed,
            medulla_typum_titulus(instructio->typus_secundus));
    }

    si (instructio->op == (s32)MEDULLA_OP_RAMUS)
    {
        officina_chorda_aedificator_appendere_character(aed, ' ');
        _operandum_scribere(aed, modulus, functio, &instructio->a);
        officina_chorda_aedificator_appendere_literis(aed, " -> ");
        _operandum_scribere(aed, modulus, functio, &instructio->b);
        officina_chorda_aedificator_appendere_literis(aed, " | ");
        _operandum_scribere(aed, modulus, functio, &instructio->c);
    }
    alioquin si (instructio->op == (s32)MEDULLA_OP_VOCARE)
    {
        s32 i;

        officina_chorda_aedificator_appendere_character(aed, ' ');
        _operandum_scribere(aed, modulus, functio, &instructio->a);
        per (i = ZEPHYRUM; i < instructio->extra_numerus; i++)
        {
            constans MedullaOperandum* argumentum =
                (constans MedullaOperandum*)officina_xar_obtinere_s(
                    functio->operanda,
                    instructio->extra_index + i);

            officina_chorda_aedificator_appendere_literis(aed, ", ");
            si (argumentum != NIHIL)
            {
                _operandum_scribere(aed, modulus, functio,
                    argumentum);
            }
        }
    }
    alioquin
    {
        constans MedullaOperandum* ordo[III];
                              b32  primum = VERUM;
                              s32  i;

        ordo[ZEPHYRUM]  = &instructio->a;
        ordo[I]         = &instructio->b;
        ordo[II]        = &instructio->c;
        per (i = ZEPHYRUM; i < III; i++)
        {
            si (ordo[i]->genus == (s32)MEDULLA_OPERANDUM_NIHIL)
            {
                perge;
            }
            officina_chorda_aedificator_appendere_literis(aed,
                primum ? " " : ", ");
            primum = FALSUM;
            _operandum_scribere(aed, modulus, functio, ordo[i]);
        }
    }
    officina_chorda_aedificator_appendere_character(aed, '\n');
}

interior vacuum
_functionem_scribere (
          OfficinaChordaAedificator* aed,
    constans MedullaModulus* modulus,
    constans MedullaFunctio* functio)
{
    i32 i;
    i32 m;

    officina_chorda_aedificator_appendere_literis(aed, "\nfunctio $");
    officina_chorda_aedificator_appendere_chorda(aed, functio->titulus);
    officina_chorda_aedificator_appendere_literis(aed, " (");
    m = officina_xar_numerus(functio->parametra);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MedullaParametrum* parametrum =
            (constans MedullaParametrum*)officina_xar_obtinere(
                functio->parametra, i);
        constans OfficinaChorda* titulus;

        si (i > ZEPHYRUM)
        {
            officina_chorda_aedificator_appendere_literis(aed, ", ");
        }
        officina_chorda_aedificator_appendere_literis(aed,
            medulla_typum_titulus(parametrum->typus));
        officina_chorda_aedificator_appendere_literis(aed, " %");
        titulus = medulla_registrum_titulus(functio,
            parametrum->index);
        si (titulus != NIHIL)
        {
            officina_chorda_aedificator_appendere_chorda(aed, *titulus);
        }
    }
    si (functio->est_variadica)
    {
        officina_chorda_aedificator_appendere_literis(aed,
            (m > ZEPHYRUM) ? ", ..." : "...");
    }
    officina_chorda_aedificator_appendere_literis(aed, ") -> ");
    si (functio->typus_reditus == MEDULLA_TYPUS_NIHIL)
    {
        officina_chorda_aedificator_appendere_literis(aed, "vacuum");
    }
    alioquin
    {
        officina_chorda_aedificator_appendere_literis(aed,
            medulla_typum_titulus(functio->typus_reditus));
    }
    officina_chorda_aedificator_appendere_character(aed, '\n');

    m = officina_xar_numerus(functio->blocci);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MedullaBloccus* bloccus =
            (constans MedullaBloccus*)officina_xar_obtinere(functio->blocci,
                i);
        i32 j;
        i32 n = officina_xar_numerus(bloccus->instructiones);

        officina_chorda_aedificator_appendere_character(aed, '@');
        officina_chorda_aedificator_appendere_chorda(aed, bloccus->titulus);
        officina_chorda_aedificator_appendere_literis(aed, ":\n");
        per (j = ZEPHYRUM; j < n; j++)
        {
            constans MedullaInstructio* instructio =
                (constans MedullaInstructio*)officina_xar_obtinere(
                    bloccus->instructiones, j);

            _instructionem_scribere(aed, modulus, functio,
                instructio);
        }
    }
}

interior vacuum
_datum_scribere (
          OfficinaChordaAedificator* aed,
    constans MedullaModulus* modulus,
      constans MedullaDatum* datum)
{
    i32 i;
    i32 m;

    officina_chorda_aedificator_appendere_literis(aed, "\ndatum $");
    officina_chorda_aedificator_appendere_chorda(aed, datum->titulus);
    officina_chorda_aedificator_appendere_literis(aed, " magnitudo ");
    officina_chorda_aedificator_appendere_i32(aed, datum->magnitudo_octetorum);
    officina_chorda_aedificator_appendere_literis(aed, " ordinatio ");
    officina_chorda_aedificator_appendere_i32(aed, datum->ordinatio);
    officina_chorda_aedificator_appendere_character(aed, '\n');

    per (i = ZEPHYRUM; i < datum->magnitudo_octetorum; i++)
    {
        character littera[IV];

        si (i % XVI == ZEPHYRUM)
        {
            officina_chorda_aedificator_appendere_literis(aed,
                "    octeti ");
        }
        alioquin si (i % IV == ZEPHYRUM)
        {
            officina_chorda_aedificator_appendere_character(aed, ' ');
        }
        sprintf(littera, "%02x", (insignatus integer)datum->imago[i]);
        officina_chorda_aedificator_appendere_literis(aed, littera);
        si (i % XVI == XV || i == datum->magnitudo_octetorum - I)
        {
            officina_chorda_aedificator_appendere_character(aed, '\n');
        }
    }

    m = officina_xar_numerus(datum->relocationes);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MedullaRelocatio* relocatio =
            (constans MedullaRelocatio*)officina_xar_obtinere(
                datum->relocationes, i);
        constans MedullaSymbolum* symbolum =
            medulla_symbolum_obtinere(modulus, relocatio->symbolum);

        officina_chorda_aedificator_appendere_literis(aed, "    relocatio ");
        officina_chorda_aedificator_appendere_i32(aed, relocatio->offset);
        officina_chorda_aedificator_appendere_literis(aed, " $");
        si (symbolum != NIHIL)
        {
            officina_chorda_aedificator_appendere_chorda(aed,
                symbolum->titulus);
        }
        officina_chorda_aedificator_appendere_character(aed, '\n');
    }
}

OfficinaChorda
medulla_textum_scribere (
                    OfficinaPiscina* piscina,
    constans MedullaModulus* modulus)
{
    OfficinaChordaAedificator* aed;
               OfficinaChorda  vacua;
                  i32  i;
                  i32  m;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (piscina == NIHIL || modulus == NIHIL)
    {
        redde vacua;
    }
    aed = officina_chorda_aedificator_creare(piscina, 4096);
    si (aed == NIHIL)
    {
        redde vacua;
    }

    officina_chorda_aedificator_appendere_literis(aed, "modulus \"");
    _chordam_evadere(aed, modulus->titulus);
    officina_chorda_aedificator_appendere_literis(aed, "\"\n");

    m = officina_xar_numerus(modulus->functiones);
    per (i = ZEPHYRUM; i < m; i++)
    {
        MedullaFunctio** functio =
            (MedullaFunctio**)officina_xar_obtinere(modulus->functiones, i);

        _functionem_scribere(aed, modulus, *functio);
    }
    m = officina_xar_numerus(modulus->data);
    per (i = ZEPHYRUM; i < m; i++)
    {
        MedullaDatum** datum =
            (MedullaDatum**)officina_xar_obtinere(modulus->data, i);

        _datum_scribere(aed, modulus, *datum);
    }
    redde officina_chorda_aedificator_finire(aed);
}


/* ==================================================
 * LECTOR - strictus praeter commenta et lineas vacuas
 * ================================================== */

nomen structura {
    OfficinaChorda textus;
       i32 numerus;              /* 1-basis */
} TextusLinea;

nomen structura {
           OfficinaPiscina* piscina;
               OfficinaXar* lineae;      /* TextusLinea valore */
               i32  positio;
    MedullaModulus* modulus;
    MedullaFunctio* functio;     /* contextus currens */
               s32  bloccus;     /* -I extra bloccum */
      MedullaDatum* datum;
               i32  offset_dati;
               i32  linea_erroris;
} TextusLector;

interior b32
_est_littera (
    character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9') || c == '_' || c == '.';
}

interior b32
_est_cifra (
    character c)
{
    redde c >= '0' && c <= '9';
}

interior vacuum
_spatia_praeterire (
    OfficinaChorda  linea,
       i32* cursor)
{
    dum (   *cursor < linea.mensura
         && (character)linea.datum[*cursor] == ' ')
    {
        (*cursor)++;
    }
}

interior character
_aspicere (
    OfficinaChorda linea,
       i32 cursor)
{
    si (cursor >= linea.mensura)
    {
        redde '\0';
    }
    redde (character)linea.datum[cursor];
}

interior OfficinaChorda
_verbum_legere (
    OfficinaChorda  linea,
       i32* cursor)
{
    OfficinaChorda verbum;
       i32 initium = *cursor;

    dum (   *cursor < linea.mensura
         && _est_littera((character)linea.datum[*cursor]))
    {
        (*cursor)++;
    }
    verbum.datum    = linea.datum + initium;
    verbum.mensura  = *cursor - initium;
    redde verbum;
}

interior b32
_verbum_aequale (
                OfficinaChorda  verbum,
    constans character* literis)
{
    i32 longitudo = (i32)strlen(literis);

    si (verbum.mensura != longitudo)
    {
        redde FALSUM;
    }
    si (longitudo == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde memcmp(verbum.datum, literis,
        (memoriae_index)longitudo) == ZEPHYRUM;
}

interior b32
_s64_legere (
    OfficinaChorda  linea,
       i32* cursor,
       s64* valor_out)
{
    b32 negativum  = FALSUM;
    s64 valor      = ZEPHYRUM;   /* accumulatio NEGATIVA (S64_MIN tuta) */
    b32 ullus      = FALSUM;

    si (_aspicere(linea, *cursor) == '-')
    {
        negativum = VERUM;
        (*cursor)++;
    }
    dum (   *cursor < linea.mensura
         && _est_cifra((character)linea.datum[*cursor]))
    {
        valor = valor * X
            - (s64)((character)linea.datum[*cursor] - '0');
        (*cursor)++;
        ullus = VERUM;
    }
    si (!ullus)
    {
        redde FALSUM;
    }
    *valor_out = negativum ? valor : -valor;
    redde VERUM;
}

/* fructus in piscinam (fugae solutae) */
interior b32
_chordam_citatam_legere (
    TextusLector* lector,
          OfficinaChorda  linea,
             i32* cursor,
          OfficinaChorda* fructus)
{
     i8* datum;
    i32  scriptum = ZEPHYRUM;

    si (_aspicere(linea, *cursor) != '"')
    {
        redde FALSUM;
    }
    (*cursor)++;
    datum = (i8*)officina_piscina_allocare(lector->piscina,
        (memoriae_index)(linea.mensura > ZEPHYRUM
                         ? linea.mensura : I));
    si (datum == NIHIL)
    {
        redde FALSUM;
    }
    dum (*cursor < linea.mensura)
    {
        character c = (character)linea.datum[*cursor];

        si (c == '"')
        {
            (*cursor)++;
            fructus->datum    = datum;
            fructus->mensura  = scriptum;
            redde VERUM;
        }
        si (c == '\\')
        {
            character f;

            (*cursor)++;
            f = _aspicere(linea, *cursor);
            si (f == 'n')
            { datum[scriptum] = (i8)'\n';
            }
            alioquin si (f == 't')
            { datum[scriptum] = (i8)'\t';
            }
            alioquin si (f == '"')
            { datum[scriptum] = (i8)'"';
            }
            alioquin si (f == '\\')
            { datum[scriptum] = (i8)'\\';
            }
            alioquin
            {
                redde FALSUM;   /* fuga ignota */
            }
        }
        alioquin
        {
            datum[scriptum] = (i8)c;
        }
        (*cursor)++;
        scriptum++;
    }
    redde FALSUM;   /* citatio non clausa */
}

interior s32
_registrum_invenire_aut_creare (
    MedullaFunctio* functio,
            OfficinaChorda  titulus)
{
    i32 i;
    i32 m = officina_xar_numerus(functio->registra);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans OfficinaChorda* sedes =
            (constans OfficinaChorda*)officina_xar_obtinere(functio->registra, i);

        si (   sedes->mensura == titulus.mensura
            && (titulus.mensura == ZEPHYRUM
                || memcmp(sedes->datum, titulus.datum,
                       (memoriae_index)titulus.mensura) == ZEPHYRUM))
        {
            redde (s32)i;
        }
    }
    redde medulla_registrum_novum(functio, titulus);
}

interior s32
_bloccum_invenire (
    constans MedullaFunctio* functio,
                     OfficinaChorda  titulus)
{
    i32 i;
    i32 m = officina_xar_numerus(functio->blocci);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MedullaBloccus* bloccus =
            (constans MedullaBloccus*)officina_xar_obtinere(functio->blocci,
                i);

        si (   bloccus->titulus.mensura == titulus.mensura
            && memcmp(bloccus->titulus.datum, titulus.datum,
                   (memoriae_index)titulus.mensura) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

interior s32
_op_invenire (
    OfficinaChorda verbum)
{
    s32 i;

    per (i = ZEPHYRUM; i < (s32)MEDULLA_OP_NUMERUS; i++)
    {
        si (_verbum_aequale(verbum, OP_NOMINA[i]))
        {
            redde i;
        }
    }
    redde -I;
}

interior s32
_typum_invenire (
    OfficinaChorda verbum)
{
    s32 i;

    per (i = ZEPHYRUM; i < (s32)MEDULLA_TYPUS_NUMERUS; i++)
    {
        si (_verbum_aequale(verbum, TYPI_NOMINA[i]))
        {
            redde i;
        }
    }
    redde -I;
}

/* numerus: fluitans si spatium characterum '.'/'e'/'E' continet */
interior b32
_numerum_legere (
              OfficinaChorda  linea,
                 i32* cursor,
    MedullaOperandum* out)
{
    i32 finis    = *cursor;
    b32 fluitat  = FALSUM;

    si (_aspicere(linea, finis) == '-')
    {
        finis++;
    }
    dum (finis < linea.mensura)
    {
        character c = (character)linea.datum[finis];

        si (c == ' ' || c == ',')
        {
            frange;
        }
        si (c == '.' || c == 'e' || c == 'E' || c == '+')
        {
            fluitat = VERUM;
        }
        finis++;
    }
    si (fluitat)
    {
        character littera[XLVIII];
              i32 longitudo = finis - *cursor;

        /* <tolera codex="SUBTRACTIO_COMPARATA" (>cursor numquam ultra finem (custos ansae vocantis) */
        si (   longitudo <= ZEPHYRUM
            || longitudo >= (i32)magnitudo(littera))
        {
            redde FALSUM;
        }
        memcpy(littera, linea.datum + *cursor,
            (memoriae_index)longitudo);
        littera[longitudo] = '\0';
        *out = medulla_op_immediatum_f(strtod(littera, NIHIL));
        *cursor = finis;
        redde VERUM;
    }
    {
        s64 valor;

        si (!_s64_legere(linea, cursor, &valor))
        {
            redde FALSUM;
        }
        *out = medulla_op_immediatum(valor);
        redde VERUM;
    }
}

interior b32
_operandum_legere (
        TextusLector* lector,
              OfficinaChorda  linea,
                 i32* cursor,
    MedullaOperandum* out)
{
    character c;

    _spatia_praeterire(linea, cursor);
    c = _aspicere(linea, *cursor);
    si (c == '%')
    {
        OfficinaChorda verbum;
           s32 index;

        (*cursor)++;
        verbum = _verbum_legere(linea, cursor);
        si (verbum.mensura == ZEPHYRUM || lector->functio == NIHIL)
        {
            redde FALSUM;
        }
        index = _registrum_invenire_aut_creare(lector->functio,
            verbum);
        si (index < ZEPHYRUM)
        {
            redde FALSUM;
        }
        *out = medulla_op_registrum(index);
        redde VERUM;
    }
    si (c == '$')
    {
        OfficinaChorda verbum;
           s32 index;

        (*cursor)++;
        verbum  = _verbum_legere(linea, cursor);
        index   = medulla_symbolum_internare(lector->modulus, verbum);
        si (index < ZEPHYRUM)
        {
            redde FALSUM;
        }
        *out = medulla_op_symbolum(index);
        redde VERUM;
    }
    si (c == '@')
    {
        OfficinaChorda verbum;
           s32 index;

        (*cursor)++;
        verbum = _verbum_legere(linea, cursor);
        si (lector->functio == NIHIL)
        {
            redde FALSUM;
        }
        index = _bloccum_invenire(lector->functio, verbum);
        si (index < ZEPHYRUM)
        {
            redde FALSUM;   /* bloccus ignotus */
        }
        *out = medulla_op_bloccum(index);
        redde VERUM;
    }
    si (c == '"')
    {
        OfficinaChorda causa;
           s32 index;

        si (!_chordam_citatam_legere(lector, linea, cursor, &causa))
        {
            redde FALSUM;
        }
        index = medulla_causam_internare(lector->modulus, causa);
        si (index < ZEPHYRUM)
        {
            redde FALSUM;
        }
        *out = medulla_op_causa(index);
        redde VERUM;
    }
    redde _numerum_legere(linea, cursor, out);
}

interior b32
_finis_lineae (
    OfficinaChorda linea,
       i32 cursor)
{
    _spatia_praeterire(linea, &cursor);
    redde cursor >= linea.mensura;
}

interior b32
_instructionem_legere (
    TextusLector* lector,
          OfficinaChorda  linea)
{
                  i32 cursor = ZEPHYRUM;
    MedullaInstructio instructio;
               OfficinaChorda verbum;

    memset(&instructio, ZEPHYRUM, magnitudo(MedullaInstructio));
    instructio.typus           = MEDULLA_TYPUS_NIHIL;
    instructio.typus_secundus  = MEDULLA_TYPUS_NIHIL;
    instructio.destinatio      = -I;
    instructio.extra_index     = -I;
    instructio.a               = medulla_op_nihil();
    instructio.b               = medulla_op_nihil();
    instructio.c               = medulla_op_nihil();

    _spatia_praeterire(linea, &cursor);
    si (_aspicere(linea, cursor) == '%')
    {
        cursor++;
        verbum = _verbum_legere(linea, &cursor);
        si (verbum.mensura == ZEPHYRUM)
        {
            redde FALSUM;
        }
        instructio.destinatio = _registrum_invenire_aut_creare(
            lector->functio, verbum);
        si (instructio.destinatio < ZEPHYRUM)
        {
            redde FALSUM;
        }
        _spatia_praeterire(linea, &cursor);
        si (_aspicere(linea, cursor) != '=')
        {
            redde FALSUM;
        }
        cursor++;
        _spatia_praeterire(linea, &cursor);
    }

    /* op[.typus[.typus]] */
    verbum = _verbum_legere(linea, &cursor);
    {
           s32 punctum_a = -I;   /* index puncti; -I absens (s32!) */
           s32 punctum_b = -I;
           i32 k;
        OfficinaChorda pars;

        per (k = ZEPHYRUM; k < verbum.mensura; k++)
        {
            si ((character)verbum.datum[k] == '.')
            {
                si (punctum_a < ZEPHYRUM)
                {
                    punctum_a = (s32)k;
                }
                alioquin si (punctum_b < ZEPHYRUM)
                {
                    punctum_b = (s32)k;
                }
                alioquin
                {
                    redde FALSUM;
                }
            }
        }
        pars.datum = verbum.datum;
        pars.mensura = (punctum_a >= ZEPHYRUM) ? (i32)punctum_a
                                               : verbum.mensura;
        instructio.op = _op_invenire(pars);
        si (instructio.op < ZEPHYRUM)
        {
            redde FALSUM;
        }
        si (punctum_a >= ZEPHYRUM)
        {
            i32 finis_partis = (punctum_b >= ZEPHYRUM)
                ? (i32)punctum_b : verbum.mensura;

            pars.datum        = verbum.datum + punctum_a + I;
            pars.mensura      = finis_partis - (i32)punctum_a - I;
            instructio.typus  = _typum_invenire(pars);
            si (instructio.typus < ZEPHYRUM)
            {
                redde FALSUM;
            }
        }
        si (punctum_b >= ZEPHYRUM)
        {
            pars.datum = verbum.datum + punctum_b + I;
            pars.mensura = verbum.mensura - (i32)punctum_b - I;
            instructio.typus_secundus = _typum_invenire(pars);
            si (instructio.typus_secundus < ZEPHYRUM)
            {
                redde FALSUM;
            }
        }
    }

    si (instructio.op == (s32)MEDULLA_OP_RAMUS)
    {
        si (!_operandum_legere(lector, linea, &cursor,
                &instructio.a))
        {
            redde FALSUM;
        }
        _spatia_praeterire(linea, &cursor);
        si (   _aspicere(linea, cursor)     != '-'
            || _aspicere(linea, cursor + I) != '>')
        {
            redde FALSUM;
        }
        cursor += II;
        si (!_operandum_legere(lector, linea, &cursor,
                &instructio.b))
        {
            redde FALSUM;
        }
        _spatia_praeterire(linea, &cursor);
        si (_aspicere(linea, cursor) != '|')
        {
            redde FALSUM;
        }
        cursor++;
        si (!_operandum_legere(lector, linea, &cursor,
                &instructio.c))
        {
            redde FALSUM;
        }
    }
    alioquin si (instructio.op == (s32)MEDULLA_OP_VOCARE)
    {
        si (!_operandum_legere(lector, linea, &cursor,
                &instructio.a))
        {
            redde FALSUM;
        }
        instructio.extra_numerus = ZEPHYRUM;
        _spatia_praeterire(linea, &cursor);
        dum (_aspicere(linea, cursor) == ',')
        {
            MedullaOperandum argumentum;
                         s32 index;

            cursor++;
            si (!_operandum_legere(lector, linea, &cursor,
                    &argumentum))
            {
                redde FALSUM;
            }
            index = medulla_operanda_addere(lector->functio,
                &argumentum, I);
            si (index < ZEPHYRUM)
            {
                redde FALSUM;
            }
            si (instructio.extra_numerus == ZEPHYRUM)
            {
                instructio.extra_index = index;
            }
            instructio.extra_numerus++;
            _spatia_praeterire(linea, &cursor);
        }
    }
    alioquin si (!_finis_lineae(linea, cursor))
    {
        /* usque ad tres operanda virgulis separata */
        si (!_operandum_legere(lector, linea, &cursor,
                &instructio.a))
        {
            redde FALSUM;
        }
        _spatia_praeterire(linea, &cursor);
        si (_aspicere(linea, cursor) == ',')
        {
            cursor++;
            si (!_operandum_legere(lector, linea, &cursor,
                    &instructio.b))
            {
                redde FALSUM;
            }
            _spatia_praeterire(linea, &cursor);
            si (_aspicere(linea, cursor) == ',')
            {
                cursor++;
                si (!_operandum_legere(lector, linea, &cursor,
                        &instructio.c))
                {
                    redde FALSUM;
                }
            }
        }
    }

    si (!_finis_lineae(linea, cursor))
    {
        redde FALSUM;   /* reliquiae post instructionem */
    }
    redde medulla_emittere(lector->functio, lector->bloccus,
        &instructio);
}

/* praescandere tituli bloccorum usque ad terminum functionis */
interior b32
_bloccos_praescandere (
    TextusLector* lector)
{
    i32 i;
    i32 m = officina_xar_numerus(lector->lineae);

    per (i = lector->positio + I; i < m; i++)
    {
        constans TextusLinea* linea =
            (constans TextusLinea*)officina_xar_obtinere(lector->lineae, i);
              i32 cursor = ZEPHYRUM;
        character c;

        _spatia_praeterire(linea->textus, &cursor);
        c = _aspicere(linea->textus, cursor);
        si (c == '@')
        {
            OfficinaChorda verbum;

            cursor++;
            verbum = _verbum_legere(linea->textus, &cursor);
            si (   verbum.mensura                   == ZEPHYRUM
                || _aspicere(linea->textus, cursor) != ':'
                || medulla_bloccum_creare(lector->functio, verbum)
                    < ZEPHYRUM)
            {
                lector->linea_erroris = linea->numerus;
                redde FALSUM;
            }
        }
        alioquin si (c != '%')
        {
            OfficinaChorda verbum = _verbum_legere(linea->textus, &cursor);

            si (   _verbum_aequale(verbum, "functio")
                || _verbum_aequale(verbum, "datum")
                || _verbum_aequale(verbum, "modulus"))
            {
                frange;
            }
        }
    }
    redde VERUM;
}

interior b32
_functionem_legere (
    TextusLector* lector,
          OfficinaChorda  linea,
             i32  cursor)
{
    OfficinaChorda titulus;
       s32 typi_parametrorum[XXXII];
    OfficinaChorda tituli_parametrorum[XXXII];
       s32 numerus_parametrorum  = ZEPHYRUM;
       b32 variadica             = FALSUM;
       s32 typus_reditus;
       s32 i;

    _spatia_praeterire(linea, &cursor);
    si (_aspicere(linea, cursor) != '$')
    {
        redde FALSUM;
    }
    cursor++;
    titulus = _verbum_legere(linea, &cursor);
    _spatia_praeterire(linea, &cursor);
    si (_aspicere(linea, cursor) != '(')
    {
        redde FALSUM;
    }
    cursor++;
    _spatia_praeterire(linea, &cursor);
    dum (_aspicere(linea, cursor) != ')')
    {
        si (numerus_parametrorum > ZEPHYRUM || variadica)
        {
            si (_aspicere(linea, cursor) != ',')
            {
                redde FALSUM;
            }
            cursor++;
            _spatia_praeterire(linea, &cursor);
        }
        si (_aspicere(linea, cursor) == '.')
        {
            /* "..." */
            si (   _aspicere(linea, cursor + I)  != '.'
                || _aspicere(linea, cursor + II) != '.')
            {
                redde FALSUM;
            }
            cursor     += III;
            variadica  = VERUM;
            _spatia_praeterire(linea, &cursor);
            perge;
        }
        si (numerus_parametrorum
            >= (s32)(magnitudo(typi_parametrorum)
                     / magnitudo(typi_parametrorum[ZEPHYRUM])))
        {
            redde FALSUM;
        }
        {
            OfficinaChorda verbum  = _verbum_legere(linea, &cursor);
               s32 typus   = _typum_invenire(verbum);

            si (typus < ZEPHYRUM)
            {
                redde FALSUM;
            }
            _spatia_praeterire(linea, &cursor);
            si (_aspicere(linea, cursor) != '%')
            {
                redde FALSUM;
            }
            cursor++;
            typi_parametrorum[numerus_parametrorum] = typus;
            tituli_parametrorum[numerus_parametrorum] =
                _verbum_legere(linea, &cursor);
            numerus_parametrorum++;
            _spatia_praeterire(linea, &cursor);
        }
    }
    cursor++;   /* ')' */
    _spatia_praeterire(linea, &cursor);
    si (   _aspicere(linea, cursor)     != '-'
        || _aspicere(linea, cursor + I) != '>')
    {
        redde FALSUM;
    }
    cursor += II;
    _spatia_praeterire(linea, &cursor);
    {
        OfficinaChorda verbum = _verbum_legere(linea, &cursor);

        si (_verbum_aequale(verbum, "vacuum"))
        {
            typus_reditus = MEDULLA_TYPUS_NIHIL;
        }
        alioquin
        {
            typus_reditus = _typum_invenire(verbum);
            si (typus_reditus < ZEPHYRUM)
            {
                redde FALSUM;
            }
        }
    }
    si (!_finis_lineae(linea, cursor))
    {
        redde FALSUM;
    }

    lector->functio = medulla_functionem_creare(lector->modulus,
        titulus, typus_reditus, variadica);
    lector->datum    = NIHIL;
    lector->bloccus  = -I;
    si (lector->functio == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus_parametrorum; i++)
    {
        si (medulla_parametrum_addere(lector->functio,
                tituli_parametrorum[i], typi_parametrorum[i])
            < ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde _bloccos_praescandere(lector);
}

interior b32
_datum_legere (
    TextusLector* lector,
          OfficinaChorda  linea,
             i32  cursor)
{
    OfficinaChorda titulus;
       s64 magnitudo_octetorum;
       s64 ordinatio;
    OfficinaChorda verbum;

    _spatia_praeterire(linea, &cursor);
    si (_aspicere(linea, cursor) != '$')
    {
        redde FALSUM;
    }
    cursor++;
    titulus = _verbum_legere(linea, &cursor);
    _spatia_praeterire(linea, &cursor);
    verbum = _verbum_legere(linea, &cursor);
    si (!_verbum_aequale(verbum, "magnitudo"))
    {
        redde FALSUM;
    }
    _spatia_praeterire(linea, &cursor);
    si (!_s64_legere(linea, &cursor, &magnitudo_octetorum))
    {
        redde FALSUM;
    }
    _spatia_praeterire(linea, &cursor);
    verbum = _verbum_legere(linea, &cursor);
    si (!_verbum_aequale(verbum, "ordinatio"))
    {
        redde FALSUM;
    }
    _spatia_praeterire(linea, &cursor);
    si (   !_s64_legere(linea, &cursor, &ordinatio)
        || !_finis_lineae(linea, cursor))
    {
        redde FALSUM;
    }
    lector->datum = medulla_datum_creare(lector->modulus, titulus,
        (i32)magnitudo_octetorum, (i32)ordinatio);
    lector->functio      = NIHIL;
    lector->bloccus      = -I;
    lector->offset_dati  = ZEPHYRUM;
    redde lector->datum != NIHIL;
}

interior s32
_cifra_hex (
    character c)
{
    si (c >= '0' && c <= '9')
    {
        redde (s32)(c - '0');
    }
    si (c >= 'a' && c <= 'f')
    {
        redde (s32)(c - 'a') + X;
    }
    redde -I;
}

interior b32
_octetos_legere (
    TextusLector* lector,
          OfficinaChorda  linea,
             i32  cursor)
{
     i8 octeti[LXIV];
    i32 numerus = ZEPHYRUM;

    per (;;)
    {
        s32 altum;
        s32 imum;

        _spatia_praeterire(linea, &cursor);
        si (cursor >= linea.mensura)
        {
            frange;
        }
        altum  = _cifra_hex(_aspicere(linea, cursor));
        imum   = _cifra_hex(_aspicere(linea, cursor + I));
        si (   altum < ZEPHYRUM || imum < ZEPHYRUM
            || numerus >= (i32)magnitudo(octeti))
        {
            redde FALSUM;
        }
        octeti[numerus] = (i8)(altum * XVI + imum);
        numerus++;
        cursor += II;
    }
    si (numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (!medulla_datum_scribere(lector->datum, lector->offset_dati,
            octeti, (i32)numerus))
    {
        redde FALSUM;
    }
    lector->offset_dati += (i32)numerus;
    redde VERUM;
}

interior b32
_relocationem_legere (
    TextusLector* lector,
          OfficinaChorda  linea,
             i32  cursor)
{
       s64 offset;
    OfficinaChorda titulus;
       s32 symbolum;

    _spatia_praeterire(linea, &cursor);
    si (!_s64_legere(linea, &cursor, &offset))
    {
        redde FALSUM;
    }
    _spatia_praeterire(linea, &cursor);
    si (_aspicere(linea, cursor) != '$')
    {
        redde FALSUM;
    }
    cursor++;
    titulus = _verbum_legere(linea, &cursor);
    si (!_finis_lineae(linea, cursor))
    {
        redde FALSUM;
    }
    symbolum = medulla_symbolum_internare(lector->modulus, titulus);
    si (symbolum < ZEPHYRUM || offset < ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde medulla_relocationem_addere(lector->datum, (i32)offset,
        symbolum);
}

interior b32
_lineam_tractare (
            TextusLector* lector,
    constans TextusLinea* linea)
{
          i32 cursor = ZEPHYRUM;
    character c;
       OfficinaChorda verbum;

    _spatia_praeterire(linea->textus, &cursor);
    c = _aspicere(linea->textus, cursor);

    si (c == '@')
    {
        s32 index;

        si (lector->functio == NIHIL)
        {
            redde FALSUM;
        }
        cursor++;
        verbum = _verbum_legere(linea->textus, &cursor);
        si (   _aspicere(linea->textus, cursor) != ':'
            || !_finis_lineae(linea->textus, cursor + I))
        {
            redde FALSUM;
        }
        index = _bloccum_invenire(lector->functio, verbum);
        si (index < ZEPHYRUM)
        {
            redde FALSUM;
        }
        lector->bloccus = index;
        redde VERUM;
    }
    si (c == '%')
    {
        si (lector->functio == NIHIL || lector->bloccus < ZEPHYRUM)
        {
            redde FALSUM;
        }
        redde _instructionem_legere(lector, linea->textus);
    }

    {
        i32 cursor_verbi = cursor;

        verbum = _verbum_legere(linea->textus, &cursor_verbi);
        si (_verbum_aequale(verbum, "modulus"))
        {
            OfficinaChorda titulus;

            si (lector->modulus != NIHIL)
            {
                redde FALSUM;
            }
            _spatia_praeterire(linea->textus, &cursor_verbi);
            si (   !_chordam_citatam_legere(lector, linea->textus,
                    &cursor_verbi, &titulus)
                || !_finis_lineae(linea->textus, cursor_verbi))
            {
                redde FALSUM;
            }
            lector->modulus = medulla_modulum_creare(
                lector->piscina, titulus);
            redde lector->modulus != NIHIL;
        }
        si (lector->modulus == NIHIL)
        {
            redde FALSUM;
        }
        si (_verbum_aequale(verbum, "functio"))
        {
            redde _functionem_legere(lector, linea->textus,
                cursor_verbi);
        }
        si (_verbum_aequale(verbum, "datum"))
        {
            redde _datum_legere(lector, linea->textus,
                cursor_verbi);
        }
        si (_verbum_aequale(verbum, "octeti"))
        {
            si (lector->datum == NIHIL)
            {
                redde FALSUM;
            }
            redde _octetos_legere(lector, linea->textus,
                cursor_verbi);
        }
        si (_verbum_aequale(verbum, "relocatio"))
        {
            si (lector->datum == NIHIL)
            {
                redde FALSUM;
            }
            redde _relocationem_legere(lector, linea->textus,
                cursor_verbi);
        }
        /* instructio sine destinatione (salire/redde/...) */
        si (lector->functio == NIHIL || lector->bloccus < ZEPHYRUM)
        {
            redde FALSUM;
        }
        redde _instructionem_legere(lector, linea->textus);
    }
}

MedullaModulus*
medulla_textum_legere (
    OfficinaPiscina* piscina,
     OfficinaChorda  textus,
        i32* linea_erroris)
{
    TextusLector lector;
             i32 i;
             i32 m;
             i32 initium         = ZEPHYRUM;
             i32 numerus_lineae  = I;

    si (linea_erroris != NIHIL)
    {
        *linea_erroris = ZEPHYRUM;
    }
    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    memset(&lector, ZEPHYRUM, magnitudo(TextusLector));
    lector.piscina  = piscina;
    lector.bloccus  = -I;
    lector.lineae   = officina_xar_creare(piscina, (i32)magnitudo(TextusLinea));
    si (lector.lineae == NIHIL)
    {
        redde NIHIL;
    }

    /* findere in lineas; vacuas et commenta (;) praetermittere */
    per (i = ZEPHYRUM; i <= textus.mensura; i++)
    {
        si (   i                          == textus.mensura
            || (character)textus.datum[i] == '\n')
        {
            OfficinaChorda contenta;
               i32 cursor = ZEPHYRUM;

            contenta.datum    = textus.datum + initium;
            contenta.mensura  = i - initium;
            _spatia_praeterire(contenta, &cursor);
            si (   cursor < contenta.mensura
                && (character)contenta.datum[cursor] != ';')
            {
                TextusLinea* sedes =
                    (TextusLinea*)officina_xar_addere(lector.lineae);

                si (sedes == NIHIL)
                {
                    redde NIHIL;
                }
                sedes->textus   = contenta;
                sedes->numerus  = numerus_lineae;
            }
            initium = i + I;
            numerus_lineae++;
        }
    }

    m = officina_xar_numerus(lector.lineae);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans TextusLinea* linea =
            (constans TextusLinea*)officina_xar_obtinere(lector.lineae, i);

        lector.positio = i;
        si (!_lineam_tractare(&lector, linea))
        {
            si (linea_erroris != NIHIL)
            {
                *linea_erroris = (lector.linea_erroris > ZEPHYRUM)
                    ? lector.linea_erroris : linea->numerus;
            }
            redde NIHIL;
        }
    }
    si (lector.modulus == NIHIL && linea_erroris != NIHIL)
    {
        *linea_erroris = numerus_lineae - I;
    }
    redde lector.modulus;
}

/* ================= ex officina/fontes/officina_regio.c ================= */

/* basis fixa: determinismus (interview Q9). 0x300000000000 =
 * spatium altum, liberum in macOS arm64; si mmap eam non dat,
 * regio_generare NIHIL reddit (numquam basim aliam tacite). */
#define REGIO_BASIS_VALOR   ((memoriae_index)0x300000000000UL)
#define REGIO_TOTA          ((memoriae_index)0x1000000000UL)  /* 64GB */
#define REGIO_GLOBALIA_MAG  ((memoriae_index)0x200000000UL)   /*  8GB */
#define REGIO_STIVA_MAG     ((memoriae_index)0x100000000UL)   /*  4GB */

#define ACERVUS_ORDINATIO   ((memoriae_index)16)
#define ACERVUS_VENENUM_ALLOCATIONIS  0xAA
#define ACERVUS_VENENUM_LIBERATIONIS  0xDD

/* caput ante quemque locellum acervi; XVI octeti - fructus manet
 * XVI-ordinatus */
nomen structura CaputAcervi CaputAcervi;
structura CaputAcervi {
    memoriae_index  magnitudo_cum_vexillo;  /* payload; bit 0 = liber */
       CaputAcervi* sequens_liber;          /* index liber (si liber) */
};

structura Regio {
    OfficinaPiscina* piscina;
         i8* basis;
        b32  custodia;

    /* globalia */
                i8* globalia_initium;
    memoriae_index  globalia_cursor;       /* offset intra aream */

    /* stiva */
    i8* stiva_initium;

    /* acervus */
                i8* acervus_initium;
    memoriae_index  acervus_magnitudo;
    memoriae_index  acervus_cursor;        /* offset (fines cumuli) */
       CaputAcervi* index_liber;

    /* census */
    memoriae_index acervus_octeti_usi;
    memoriae_index acervus_apex;
    memoriae_index numerus_allocationum;
    memoriae_index numerus_liberationum;
};


/* ==================================================
 * Vita
 * ================================================== */

Regio*
regio_generare (
    OfficinaPiscina* piscina)
{
     Regio* regio;
    vacuum* datus;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }

    datus = mmap((vacuum*)REGIO_BASIS_VALOR, REGIO_TOTA,
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    si (datus == MAP_FAILED)
    {
        redde NIHIL;
    }
    si (datus != (vacuum*)REGIO_BASIS_VALOR)
    {
        /* basis aliena: determinismus periret - honeste recusa */
        munmap(datus, REGIO_TOTA);
        redde NIHIL;
    }

    regio = officina_piscina_allocare(piscina, magnitudo(Regio));
    si (regio == NIHIL)
    {
        munmap(datus, REGIO_TOTA);
        redde NIHIL;
    }
    memset(regio, ZEPHYRUM, magnitudo(Regio));

    regio->piscina   = piscina;
    regio->basis     = (i8*)datus;
    regio->custodia  = FALSUM;

    regio->globalia_initium  = regio->basis;
    regio->globalia_cursor   = ZEPHYRUM;

    regio->stiva_initium = regio->basis + REGIO_GLOBALIA_MAG;

    regio->acervus_initium = regio->stiva_initium + REGIO_STIVA_MAG;
    regio->acervus_magnitudo = REGIO_TOTA - REGIO_GLOBALIA_MAG
        - REGIO_STIVA_MAG;
    regio->acervus_cursor  = ZEPHYRUM;
    regio->index_liber     = NIHIL;

    redde regio;
}

vacuum
regio_destruere (
    Regio* regio)
{
    si (regio == NIHIL)
    {
        redde;
    }
    munmap((vacuum*)regio->basis, REGIO_TOTA);
    regio->basis = NIHIL;
}


/* ==================================================
 * Fines + custodia
 * ================================================== */

vacuum*
regio_basis (
    constans Regio* regio)
{
    redde (vacuum*)regio->basis;
}

memoriae_index
regio_magnitudo_tota (
    constans Regio* regio)
{
    (vacuum)regio;
    redde REGIO_TOTA;
}

b32
regio_continet (
     constans Regio* regio,
    constans vacuum* locus)
{
    constans i8* p = (constans i8*)locus;

    si (regio == NIHIL || regio->basis == NIHIL)
    {
        redde FALSUM;
    }
    redde (b32)(p >= regio->basis && p < regio->basis + REGIO_TOTA);
}

vacuum
regio_custodiam_ponere (
    Regio* regio,
      b32  custodia)
{
    regio->custodia = custodia;
}

b32
regio_custodia (
    constans Regio* regio)
{
    redde regio->custodia;
}


/* ==================================================
 * Globalia
 * ================================================== */

vacuum*
regio_globalia_allocare (
             Regio* regio,
    memoriae_index  magnitudo_octetorum,
    memoriae_index  ordinatio)
{
    memoriae_index  cursor;
                i8* fructus;

    si (regio == NIHIL || magnitudo_octetorum == ZEPHYRUM)
    {
        redde NIHIL;
    }
    si (ordinatio == ZEPHYRUM)
    {
        ordinatio = I;
    }

    /* rotundare cursorem ad ordinationem (potestas duorum) */
    cursor = (regio->globalia_cursor + ordinatio - I)
        & ~(ordinatio - I);
    si (cursor + magnitudo_octetorum > REGIO_GLOBALIA_MAG)
    {
        redde NIHIL;
    }

    fructus                 = regio->globalia_initium + cursor;
    regio->globalia_cursor  = cursor + magnitudo_octetorum;
    redde (vacuum*)fructus;
}

memoriae_index
regio_globalia_usus (
    constans Regio* regio)
{
    redde regio->globalia_cursor;
}


/* ==================================================
 * Stiva
 * ================================================== */

vacuum*
regio_stiva_initium (
    constans Regio* regio)
{
    redde (vacuum*)regio->stiva_initium;
}

memoriae_index
regio_stiva_magnitudo_octetorum (
    constans Regio* regio)
{
    (vacuum)regio;
    redde REGIO_STIVA_MAG;
}


/* ==================================================
 * Acervus
 * ================================================== */

interior memoriae_index
_rotundare_xvi (
    memoriae_index n)
{
    redde (n + ACERVUS_ORDINATIO - I) & ~(ACERVUS_ORDINATIO - I);
}

interior CaputAcervi*
_caput_de_loco (
    vacuum* locus)
{
    redde (CaputAcervi*)(vacuum*)((i8*)locus
        - magnitudo(CaputAcervi));
}

interior vacuum*
_locus_de_capite (
    CaputAcervi* caput)
{
    redde (vacuum*)((i8*)(vacuum*)caput
        + magnitudo(CaputAcervi));
}

interior memoriae_index
_caput_magnitudo (
    constans CaputAcervi* caput)
{
    redde caput->magnitudo_cum_vexillo & ~(memoriae_index)I;
}

interior b32
_caput_liber (
    constans CaputAcervi* caput)
{
    redde (b32)(caput->magnitudo_cum_vexillo & (memoriae_index)I);
}

/* validitas capitis: intra acervum usum, magnitudo sana */
interior b32
_caput_validum (
          constans Regio* regio,
    constans CaputAcervi* caput)
{
       constans i8* p = (constans i8*)(constans vacuum*)caput;
    memoriae_index  m;

    si (   p < regio->acervus_initium
        || p >= regio->acervus_initium + regio->acervus_cursor)
    {
        redde FALSUM;
    }
    si ((memoriae_index)(p - regio->acervus_initium)
        % ACERVUS_ORDINATIO != ZEPHYRUM)
    {
        redde FALSUM;
    }
    m = _caput_magnitudo(caput);
    si (   m == ZEPHYRUM || m % ACERVUS_ORDINATIO != ZEPHYRUM
        || m > regio->acervus_cursor)
    {
        redde FALSUM;
    }
    redde VERUM;
}

vacuum*
regio_allocare (
             Regio* regio,
    memoriae_index  n)
{
       CaputAcervi* caput;
       CaputAcervi* prior;
            vacuum* fructus;
    memoriae_index  rotundata;

    si (regio == NIHIL || n == ZEPHYRUM)
    {
        redde NIHIL;
    }
    rotundata = _rotundare_xvi(n);

    /* primus-aptus in indice libero */
    prior = NIHIL;
    caput = regio->index_liber;
    dum (caput != NIHIL)
    {
        si (_caput_magnitudo(caput) >= rotundata)
        {
            frange;
        }
        prior = caput;
        caput = caput->sequens_liber;
    }

    si (caput != NIHIL)
    {
        /* evellere de indice */
        si (prior == NIHIL)
        {
            regio->index_liber = caput->sequens_liber;
        }
        alioquin
        {
            prior->sequens_liber = caput->sequens_liber;
        }

        /* scindere si residuum locellum integrum fert */
        si (_caput_magnitudo(caput) >= rotundata
            + magnitudo(CaputAcervi) + ACERVUS_ORDINATIO)
        {
               CaputAcervi* residuum;
            memoriae_index  magnitudo_residui;

            magnitudo_residui = _caput_magnitudo(caput) - rotundata
                - magnitudo(CaputAcervi);
            residuum =
                (CaputAcervi*)(vacuum*)((i8*)_locus_de_capite(caput)
                + rotundata);
            residuum->magnitudo_cum_vexillo = magnitudo_residui
                | (memoriae_index)I;
            residuum->sequens_liber  = regio->index_liber;
            regio->index_liber       = residuum;

            caput->magnitudo_cum_vexillo = rotundata;
        }
        alioquin
        {
            caput->magnitudo_cum_vexillo = _caput_magnitudo(caput);
        }
        caput->sequens_liber = NIHIL;
    }
    alioquin
    {
        /* cumulus crescit */
        memoriae_index opus = magnitudo(CaputAcervi) + rotundata;

        si (regio->acervus_cursor + opus > regio->acervus_magnitudo)
        {
            redde NIHIL;
        }
        caput = (CaputAcervi*)(vacuum*)(regio->acervus_initium
            + regio->acervus_cursor);
        regio->acervus_cursor         += opus;
        caput->magnitudo_cum_vexillo  = rotundata;
        caput->sequens_liber          = NIHIL;
    }

    fructus                    = _locus_de_capite(caput);
    regio->acervus_octeti_usi  += _caput_magnitudo(caput);
    si (regio->acervus_octeti_usi > regio->acervus_apex)
    {
        regio->acervus_apex = regio->acervus_octeti_usi;
    }
    regio->numerus_allocationum += I;

    si (regio->custodia)
    {
        memset(fructus, ACERVUS_VENENUM_ALLOCATIONIS,
            _caput_magnitudo(caput));
    }
    redde fructus;
}

b32
regio_liberare (
     Regio* regio,
    vacuum* locus)
{
    CaputAcervi* caput;

    si (regio == NIHIL || locus == NIHIL)
    {
        redde FALSUM;
    }
    caput = _caput_de_loco(locus);
    si (!_caput_validum(regio, caput) || _caput_liber(caput))
    {
        redde FALSUM;
    }

    si (regio->custodia)
    {
        memset(locus, ACERVUS_VENENUM_LIBERATIONIS,
            _caput_magnitudo(caput));
    }

    regio->acervus_octeti_usi    -= _caput_magnitudo(caput);
    regio->numerus_liberationum  += I;

    caput->magnitudo_cum_vexillo  |= (memoriae_index)I;
    caput->sequens_liber          = regio->index_liber;
    regio->index_liber            = caput;
    redde VERUM;
}

vacuum*
regio_reallocare (
             Regio* regio,
            vacuum* locus,
    memoriae_index  n)
{
       CaputAcervi* caput;
    memoriae_index  vetus;
            vacuum* novus;

    si (regio == NIHIL)
    {
        redde NIHIL;
    }
    si (locus == NIHIL)
    {
        redde regio_allocare(regio, n);
    }
    si (n == ZEPHYRUM)
    {
        redde NIHIL;
    }

    caput = _caput_de_loco(locus);
    si (!_caput_validum(regio, caput) || _caput_liber(caput))
    {
        redde NIHIL;
    }
    vetus = _caput_magnitudo(caput);

    si (_rotundare_xvi(n) <= vetus)
    {
        /* contractio: idem monstrator (pinnatur in probatione) */
        redde locus;
    }

    novus = regio_allocare(regio, n);
    si (novus == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(novus, locus, vetus);
    regio_liberare(regio, locus);
    redde novus;
}

memoriae_index
regio_acervus_usus (
    constans Regio* regio)
{
    redde regio->acervus_octeti_usi;
}

memoriae_index
regio_acervus_apex (
    constans Regio* regio)
{
    redde regio->acervus_apex;
}

memoriae_index
regio_numerus_allocationum (
    constans Regio* regio)
{
    redde regio->numerus_allocationum;
}

memoriae_index
regio_numerus_liberationum (
    constans Regio* regio)
{
    redde regio->numerus_liberationum;
}

/* ================= ex officina/fontes/officina_conexio.c ================= */

nomen structura {
    constans character* titulus;
                   s64  valor;
} CellaNota;

/* symbola data externa nota (M2a; M2c/M2d addent) - ansae opacae
 * (interview Q1): stdin 0, stdout 1, stderr 2; errno cella 0 */
interior constans CellaNota CELLAE_NOTAE[] = {
    { "stdin",  0 },
    { "stdout", 1 },
    { "stderr", 2 },
    { "errno",  0 }
};
#define CELLAE_NOTAE_NUMERUS 4

structura Conexio {
           OfficinaPiscina* piscina;
             Regio* regio;
               OfficinaXar* moduli;            /* MedullaModulus* */
               OfficinaXar* symbola;           /* ConexioSymbolum valore */
    OfficinaTabulaDispersa* index_titulorum;   /* titulus -> index+1 */
               OfficinaXar* translationes;     /* s32* (una per modulum) */
               OfficinaXar* functiones_nexae;  /* ConexioFunctioNexa valore */
               OfficinaXar* decipulae;         /* chorda valore (tituli) */
               b32  nexum;
            OfficinaChorda  querela;
            OfficinaChorda  querela_symbolum;
               i32  numerus_fracturarum;
               i32  numerus_datorum;
               i32  numerus_cellarum;
};


/* ==================================================
 * Auxilia
 * ================================================== */

interior OfficinaChorda
_titulum_copiare (
    Conexio* conexio,
     OfficinaChorda  titulus)
{
    OfficinaChorda  copia;
        i8* datum;

    datum = officina_piscina_allocare(conexio->piscina,
        (memoriae_index)titulus.mensura);
    memcpy(datum, titulus.datum, (memoriae_index)titulus.mensura);
    copia.datum    = datum;
    copia.mensura  = titulus.mensura;
    redde copia;
}

interior vacuum
_querelam_ponere (
               Conexio* conexio,
    constans character* nuntius)
{
    conexio->querela = officina_chorda_ex_literis(nuntius, conexio->piscina);
}

/* invenire aut creare; -I in fractura allocationis */
interior s32
_globale_internare (
    Conexio* conexio,
     OfficinaChorda  titulus)
{
             vacuum* valor;
    ConexioSymbolum* symbolum;
                s32  index;

    si (officina_tabula_dispersa_invenire(conexio->index_titulorum, titulus,
        &valor))
    {
        redde (s32)(memoriae_index)valor - I;
    }

    index     = (s32)officina_xar_numerus(conexio->symbola);
    symbolum  = officina_xar_addere(conexio->symbola);
    si (symbolum == NIHIL)
    {
        redde -I;
    }
    symbolum->titulus        = _titulum_copiare(conexio, titulus);
    symbolum->genus          = CONEXIO_SYMBOLUM_IGNOTUM;
    symbolum->modulus_index  = -I;
    symbolum->index_localis  = -I;
    symbolum->sedes          = NIHIL;

    si (!officina_tabula_dispersa_inserere(conexio->index_titulorum,
        symbolum->titulus,
        (vacuum*)(memoriae_index)(index + I)))
    {
        redde -I;
    }
    redde index;
}


/* ==================================================
 * Vita + nexus
 * ================================================== */

Conexio*
conexio_creare (
    OfficinaPiscina* piscina,
      Regio* regio)
{
    Conexio* conexio;

    si (piscina == NIHIL || regio == NIHIL)
    {
        redde NIHIL;
    }
    conexio = officina_piscina_allocare(piscina, magnitudo(Conexio));
    memset(conexio, ZEPHYRUM, magnitudo(Conexio));

    conexio->piscina  = piscina;
    conexio->regio    = regio;
    conexio->moduli = officina_xar_creare(piscina,
        (i32)magnitudo(MedullaModulus*));
    conexio->symbola = officina_xar_creare(piscina,
        (i32)magnitudo(ConexioSymbolum));
    conexio->index_titulorum = officina_tabula_dispersa_creare_chorda(piscina,
        256);
    conexio->translationes = officina_xar_creare(piscina,
        (i32)magnitudo(s32*));
    conexio->functiones_nexae = officina_xar_creare(piscina,
        (i32)magnitudo(ConexioFunctioNexa));
    conexio->decipulae = officina_xar_creare(piscina, (i32)magnitudo(OfficinaChorda));

    si (   conexio->moduli == NIHIL || conexio->symbola == NIHIL
        || conexio->index_titulorum == NIHIL
        || conexio->translationes == NIHIL
        || conexio->functiones_nexae == NIHIL
        || conexio->decipulae == NIHIL)
    {
        redde NIHIL;
    }
    redde conexio;
}

b32
conexio_modulum_addere (
           Conexio* conexio,
    MedullaModulus* modulus)
{
               s32   modulus_index;
               i32   numerus;
               s32*  translatio;
               i32   i;
    MedullaModulus** locellus;
               s32** locellus_translationis;

    si (conexio == NIHIL || modulus == NIHIL || conexio->nexum)
    {
        redde FALSUM;
    }

    modulus_index  = (s32)officina_xar_numerus(conexio->moduli);
    locellus       = officina_xar_addere(conexio->moduli);
    si (locellus == NIHIL)
    {
        redde FALSUM;
    }
    *locellus = modulus;

    numerus     = officina_xar_numerus(modulus->symbola);
    translatio  = NIHIL;
    si (numerus > ZEPHYRUM)
    {
        translatio = officina_piscina_allocare(conexio->piscina,
            (memoriae_index)numerus * magnitudo(s32));
    }

    /* fractura statum NON corrumpit: omnes duplicationes colliguntur
     * (census instrumenti), translatio semper impletur et ponitur -
     * nectere postea recusat si fracturae > 0 */
    {
        b32 fractum = FALSUM;

        per (i = ZEPHYRUM; i < numerus; i += I)
        {
            constans MedullaSymbolum* localis =
                medulla_symbolum_obtinere(modulus, (s32)i);
            s32 globale = _globale_internare(conexio,
                localis->titulus);
            ConexioSymbolum* symbolum;

            si (globale < ZEPHYRUM)
            {
                _querelam_ponere(conexio,
                    "allocatio symboli fracta");
                conexio->numerus_fracturarum += I;
                redde FALSUM;
            }
            translatio[i] = globale;

            si (localis->genus == MEDULLA_SYMBOLUM_EXTERNUM)
            {
                perge;
            }

            symbolum = officina_xar_obtinere(conexio->symbola, (i32)globale);
            si (symbolum->genus != CONEXIO_SYMBOLUM_IGNOTUM)
            {
                _querelam_ponere(conexio,
                    "definitio duplex symboli");
                conexio->querela_symbolum     = symbolum->titulus;
                conexio->numerus_fracturarum  += I;
                fractum                       = VERUM;
                perge;
            }
            symbolum->genus =
                (localis->genus == MEDULLA_SYMBOLUM_FUNCTIO)
                ? CONEXIO_SYMBOLUM_FUNCTIO
                : CONEXIO_SYMBOLUM_DATUM;
            symbolum->modulus_index = modulus_index;
            symbolum->index_localis = localis->index;
        }

        locellus_translationis = officina_xar_addere(conexio->translationes);
        si (locellus_translationis == NIHIL)
        {
            redde FALSUM;
        }
        *locellus_translationis = translatio;
        redde !fractum;
    }
}

interior b32
_cellam_notam_quaerere (
    OfficinaChorda  titulus,
       s64* valor_out)
{
    i32 i;

    per (i = ZEPHYRUM; i < CELLAE_NOTAE_NUMERUS; i += I)
    {
        si (officina_chorda_aequalis_literis(titulus, CELLAE_NOTAE[i].titulus))
        {
            *valor_out = CELLAE_NOTAE[i].valor;
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
conexio_nectere (
    Conexio* conexio)
{
    i32 numerus;
    i32 i;

    si (   conexio == NIHIL || conexio->nexum
        || conexio->numerus_fracturarum > ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* gradus I: collocatio - omne symbolum sedem accipit */
    numerus = officina_xar_numerus(conexio->symbola);
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        ConexioSymbolum* symbolum = officina_xar_obtinere(conexio->symbola, i);
         MedullaModulus* modulus;

        si (symbolum->genus == CONEXIO_SYMBOLUM_FUNCTIO)
        {
             ConexioDescriptor* descriptor;
            ConexioFunctioNexa* nexa;
                           s64  index_nexae;

            modulus = *(MedullaModulus**)officina_xar_obtinere(conexio->moduli,
                (i32)symbolum->modulus_index);
            index_nexae  = (s64)officina_xar_numerus(conexio->functiones_nexae);
            nexa         = officina_xar_addere(conexio->functiones_nexae);
            nexa->functio = *(MedullaFunctio**)officina_xar_obtinere(
                modulus->functiones, (i32)symbolum->index_localis);
            nexa->modulus_index = symbolum->modulus_index;

            descriptor = regio_globalia_allocare(conexio->regio,
                magnitudo(ConexioDescriptor), XVI);
            si (descriptor == NIHIL)
            {
                _querelam_ponere(conexio, "globalia exhausta");
                redde FALSUM;
            }
            descriptor->signum  = CONEXIO_SIGNUM_INTERPRETATUM;
            descriptor->index   = index_nexae;
            symbolum->sedes     = descriptor;
        }
        alioquin si (symbolum->genus == CONEXIO_SYMBOLUM_DATUM)
        {
            constans MedullaDatum* datum;

            modulus = *(MedullaModulus**)officina_xar_obtinere(conexio->moduli,
                (i32)symbolum->modulus_index);
            datum = *(MedullaDatum**)officina_xar_obtinere(modulus->data,
                (i32)symbolum->index_localis);
            symbolum->sedes = regio_globalia_allocare(conexio->regio,
                (memoriae_index)datum->magnitudo_octetorum,
                (memoriae_index)datum->ordinatio);
            si (symbolum->sedes == NIHIL)
            {
                _querelam_ponere(conexio, "globalia exhausta");
                redde FALSUM;
            }
            memcpy(symbolum->sedes, datum->imago,
                (memoriae_index)datum->magnitudo_octetorum);
            conexio->numerus_datorum += I;
        }
        alioquin
        {
            s64 valor_cellae;

            si (_cellam_notam_quaerere(symbolum->titulus,
                &valor_cellae))
            {
                s64* cella = regio_globalia_allocare(conexio->regio,
                    magnitudo(s64), VIII);
                si (cella == NIHIL)
                {
                    _querelam_ponere(conexio, "globalia exhausta");
                    redde FALSUM;
                }
                *cella                     = valor_cellae;
                symbolum->genus            = CONEXIO_SYMBOLUM_CELLA;
                symbolum->sedes            = cella;
                conexio->numerus_cellarum  += I;
            }
            alioquin
            {
                ConexioDescriptor* descriptor;
                           OfficinaChorda* titulus_decipulae;
                              s64  index_decipulae;

                index_decipulae = (s64)officina_xar_numerus(conexio->decipulae);
                titulus_decipulae = officina_xar_addere(conexio->decipulae);
                *titulus_decipulae = symbolum->titulus;

                descriptor = regio_globalia_allocare(conexio->regio,
                    magnitudo(ConexioDescriptor), XVI);
                si (descriptor == NIHIL)
                {
                    _querelam_ponere(conexio, "globalia exhausta");
                    redde FALSUM;
                }
                descriptor->signum  = CONEXIO_SIGNUM_DECIPULA;
                descriptor->index   = index_decipulae;
                symbolum->genus     = CONEXIO_SYMBOLUM_DECIPULA;
                symbolum->sedes     = descriptor;
            }
        }
    }

    /* gradus II: relocationes ADDITIVE sarcire (locellus = sedes
     * symboli + addendum prius; memcpy - nulla assumptio
     * ordinationis) */
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        constans ConexioSymbolum* symbolum =
            officina_xar_obtinere(conexio->symbola, i);
               MedullaModulus* modulus;
        constans MedullaDatum* datum;
                          s32* translatio;
                          i32  r;

        si (symbolum->genus != CONEXIO_SYMBOLUM_DATUM)
        {
            perge;
        }
        modulus = *(MedullaModulus**)officina_xar_obtinere(conexio->moduli,
            (i32)symbolum->modulus_index);
        datum = *(MedullaDatum**)officina_xar_obtinere(modulus->data,
            (i32)symbolum->index_localis);
        translatio = *(s32**)officina_xar_obtinere(conexio->translationes,
            (i32)symbolum->modulus_index);

        per (r = ZEPHYRUM; r < officina_xar_numerus(datum->relocationes);
            r += I)
        {
            constans MedullaRelocatio* relocatio =
                officina_xar_obtinere(datum->relocationes, r);
            s32 globale = translatio[relocatio->symbolum];
            constans ConexioSymbolum* petitum =
                officina_xar_obtinere(conexio->symbola, (i32)globale);
             i8* locellus = (i8*)symbolum->sedes + relocatio->offset;
            s64  addendum;
            s64  valor;

            memcpy(&addendum, locellus, magnitudo(s64));
            valor = (s64)(memoriae_index)petitum->sedes + addendum;
            memcpy(locellus, &valor, magnitudo(s64));
        }
    }

    conexio->nexum = VERUM;
    redde VERUM;
}

constans OfficinaChorda*
conexio_querela (
    constans Conexio* conexio)
{
    redde &conexio->querela;
}

constans OfficinaChorda*
conexio_querela_symbolum (
    constans Conexio* conexio)
{
    redde &conexio->querela_symbolum;
}


/* ==================================================
 * Quaestio
 * ================================================== */

s32
conexio_symbolum_quaerere (
    constans Conexio* conexio,
              OfficinaChorda  titulus)
{
    vacuum* valor;

    si (officina_tabula_dispersa_invenire(conexio->index_titulorum, titulus,
        &valor))
    {
        redde (s32)(memoriae_index)valor - I;
    }
    redde -I;
}

constans ConexioSymbolum*
conexio_symbolum_obtinere (
    constans Conexio* conexio,
                 s32  index)
{
    redde officina_xar_obtinere_s(conexio->symbola, index);
}

i32
conexio_numerus_symbolorum (
    constans Conexio* conexio)
{
    redde officina_xar_numerus(conexio->symbola);
}

vacuum*
conexio_sedes_quaerere (
    constans Conexio* conexio,
              OfficinaChorda  titulus)
{
    s32 index = conexio_symbolum_quaerere(conexio, titulus);
    constans ConexioSymbolum* symbolum;

    si (index < ZEPHYRUM)
    {
        redde NIHIL;
    }
    symbolum = conexio_symbolum_obtinere(conexio, index);
    redde symbolum->sedes;
}

s32
conexio_symbolum_globale (
    constans Conexio* conexio,
                 s32  modulus_index,
                 s32  index_localis)
{
    constans MedullaModulus* modulus;
                        s32* translatio;

    si (   modulus_index < ZEPHYRUM
        || modulus_index >= (s32)officina_xar_numerus(conexio->moduli))
    {
        redde -I;
    }
    modulus = *(MedullaModulus**)officina_xar_obtinere(conexio->moduli,
        (i32)modulus_index);
    si (   index_localis < ZEPHYRUM
        || index_localis >= (s32)officina_xar_numerus(modulus->symbola))
    {
        redde -I;
    }
    translatio = *(s32**)officina_xar_obtinere(conexio->translationes,
        (i32)modulus_index);
    redde translatio[index_localis];
}

constans ConexioFunctioNexa*
conexio_functionem_obtinere (
    constans Conexio* conexio,
                 s64  index)
{
    redde officina_xar_obtinere_s(conexio->functiones_nexae, (s32)index);
}

i32
conexio_numerus_functionum (
    constans Conexio* conexio)
{
    redde officina_xar_numerus(conexio->functiones_nexae);
}

i32
conexio_numerus_modulorum (
    constans Conexio* conexio)
{
    redde officina_xar_numerus(conexio->moduli);
}

constans MedullaModulus*
conexio_modulum_obtinere (
    constans Conexio* conexio,
                 s32  index)
{
    MedullaModulus** locellus = officina_xar_obtinere_s(conexio->moduli,
        index);

    redde (locellus == NIHIL) ? NIHIL : *locellus;
}


/* ==================================================
 * Census
 * ================================================== */

i32
conexio_numerus_datorum (
    constans Conexio* conexio)
{
    redde conexio->numerus_datorum;
}

i32
conexio_numerus_cellarum (
    constans Conexio* conexio)
{
    redde conexio->numerus_cellarum;
}

i32
conexio_numerus_decipularum (
    constans Conexio* conexio)
{
    redde officina_xar_numerus(conexio->decipulae);
}

constans OfficinaChorda*
conexio_decipulam_obtinere (
    constans Conexio* conexio,
                 s32  index)
{
    redde officina_xar_obtinere_s(conexio->decipulae, index);
}

/* ================= ex officina/fontes/officina_machinula.c ================= */

#define ANULUS_MENSURA        4096
#define ANULUS_LARVA          (ANULUS_MENSURA - 1)
#define ARGUMENTA_MAXIMA      32
#define STIVA_MARGO           256   /* octeti custodiae in vocare */
#define ANSAE_MAXIMAE         64    /* plagulae apertae simul */

/* ops privatae machinulae: valores post MEDULLA_OP_NUMERUS, solum
 * in exemplari congelato (numquam textuales, numquam in IR ipso).
 * Sedes numeratorum pro his in acie numeri_op reservantur. */
#define MACHINULA_OP_FLUXUS_CUSTOS  (MEDULLA_OP_NUMERUS)
#define MACHINULA_OP_PAUSA          (MEDULLA_OP_NUMERUS + 1)
#define MACHINULA_OPS_PRIVATAE      2


/* ==================================================
 * Typi interni
 * ================================================== */

/* congelatio plana (M3 chunk I): exemplar machinulae proprium
 * instructionum functionis - IR immutabile manet (DECISUS
 * conexionis); ordo = ambulatio canonica medullae (PACTUM in
 * officina_medulla.h). blocci_initia: blocci_numerus + I introitus,
 * stricte crescentia (ultimus = numerus totus); scopi ramorum per
 * eam transferuntur. Post bloccum non-terminatum custos fluxus
 * inseritur - semantica "fluxus extra bloccum" hodierna servata
 * (corpus semper terminatum manet 1:1 cum ambulatione canonica). */
nomen structura {
    MedullaInstructio* instructiones;   /* contiguae; NON-constans:
                                         * puncta hoc exemplar
                                         * patchant (M3 chunk 4) */
                          i32  numerus;
                 constans i32* blocci_initia;
                          i32  blocci_numerus;
    constans MedullaOperandum* operanda;        /* acies plana */
                          i32  operanda_numerus;
} FunctioPlana;

nomen structura {
    s32 functio_index;
    i32 instructio;                /* index planus */
    s32 op_originalis;
} MachinulaPunctum;

nomen structura {
    constans MedullaFunctio* functio;
    constans FunctioPlana*   plana;
    s32            modulus_index;
    i32            instructio;      /* index PLANUS proximae */
    i64*           registra;
    memoriae_index basis_stivae;    /* cursor restaurandus in redde */
    s32            destinatio_vocantis;   /* registrum vocantis; -I */
} Tabulatum;

nomen structura {
    constans MedullaFunctio* functio;
    constans FunctioPlana*   plana;
    s32 modulus_index;
    i32 instructio;                 /* index planus */
} AnulusFigura;

nomen b32 (*MachinulaPons)(Machinula* machinula,
    constans i64* argumenta, s32 numerus, i64* fructus);

structura Machinula {
      OfficinaPiscina* piscina;
      Conexio* conexio;
        Regio* regio;
          OfficinaXar* tabulata;             /* Tabulatum valore */
    Tabulatum* tabulatum_summum;    /* cacumen tabulatorum (monstra-
                                     * tores Xar stabiles trans
                                     * appends - Correctio 07-02) */
    FunctioPlana* planae;           /* parallelae tabulae functionum
                                     * conexionis (index descriptoris) */
    i32       planae_numerus;
    OfficinaXar*      puncta;               /* MachinulaPunctum valore (M3) */
    OfficinaXar*      lineae_modulorum;     /* MedullaLineae* (NIHIL licet) */
    i8*       stiva_basis;
    memoriae_index stiva_magnitudo;
    memoriae_index stiva_cursor;
    /* halitus */
       b32 currens;
       s32 halitus_genus;
       s64 halitus_codex;
    OfficinaChorda halitus_nuntius;
    /* anulus volatus (SEMPER activus) */
    AnulusFigura anulus[ANULUS_MENSURA];
             i64 anulus_cursor;
    /* numeratores */
    i64       numeri_op[MEDULLA_OP_NUMERUS + MACHINULA_OPS_PRIVATAE];
    i64       summa_instructionum;
    i64       numerus_vocationum;
    i64       numerus_aedificatorum;
    memoriae_index apex_stivae;
    /* cellae externae captae */
    s64* cella_errno;          /* NIHIL licet */
    /* ansae plagularum (M2d): 0/1/2 = flumina norma (DECISUS Q1),
     * 3+ = plagulae fopen; locelli NIHIL liberi (fclose vacat).
     * M4b: locelli 0/1/2 redirectiones facultativae (ansam_ponere -
     * sutura capturae); NIHIL = flumen hospitis ordinarium. */
    FILE* ansae[ANSAE_MAXIMAE];
    /* M4b: vexilla recusationum sessionis (0 = omnia licita) */
    i32 recusationes;
};

interior FILE*
_ansam_solvere (
    Machinula* m,
          i64  ansa);
interior vacuum
_recusare (
             Machinula* m,
    constans character* nuntius);
interior MachinulaPunctum*
_punctum_invenire (
    constans Machinula* m,
                   s32  functio_index,
                   i32  instructio);


/* ==================================================
 * Canonicum + figurae fluitantes
 * ================================================== */

interior i64
_canonicum (
    i64 verbum,
    s32 typus)
{
    commutatio (typus)
    {
    casus MEDULLA_TYPUS_I8:
        redde verbum & (i64)0xFF;
    casus MEDULLA_TYPUS_I16:
        redde verbum & (i64)0xFFFF;
    casus MEDULLA_TYPUS_I32:
    casus MEDULLA_TYPUS_F32:
        redde verbum & (i64)0xFFFFFFFF;
    casus MEDULLA_TYPUS_S8:
        verbum &= (i64)0xFF;
        redde (verbum & (i64)0x80) ? (verbum | ~(i64)0xFF) : verbum;
    casus MEDULLA_TYPUS_S16:
        verbum &= (i64)0xFFFF;
        redde (verbum & (i64)0x8000)
            ? (verbum | ~(i64)0xFFFF) : verbum;
    casus MEDULLA_TYPUS_S32:
        verbum &= (i64)0xFFFFFFFF;
        redde (verbum & (i64)0x80000000)
            ? (verbum | ~(i64)0xFFFFFFFF) : verbum;
    ordinarius:
        redde verbum;   /* i64/s64/f64/nihil */
    }
}

interior f64
_f64_de_verbo (
    i64 verbum)
{
    unio { i64 v; f64 f; } u;

    u.v = verbum;
    redde u.f;
}

interior i64
_verbum_de_f64 (
    f64 fluitans_valor)
{
    unio { i64 v; f64 f; } u;

    u.f = fluitans_valor;
    redde u.v;
}

interior f32
_f32_de_verbo (
    i64 verbum)
{
    unio { i32 v; f32 f; } u;

    u.v = (i32)(verbum & (i64)0xFFFFFFFF);
    redde u.f;
}

interior i64
_verbum_de_f32 (
    f32 fluitans_valor)
{
    unio { i32 v; f32 f; } u;

    u.f = fluitans_valor;
    redde (i64)u.v;
}

interior b32
_typus_signatus (
    s32 typus)
{
    redde (b32)(typus >= MEDULLA_TYPUS_S8
        && typus <= MEDULLA_TYPUS_S64);
}

interior s32
_latitudo_typi (
    s32 typus)
{
    commutatio (typus)
    {
    casus MEDULLA_TYPUS_I8:
    casus MEDULLA_TYPUS_S8:
        redde 8;
    casus MEDULLA_TYPUS_I16:
    casus MEDULLA_TYPUS_S16:
        redde 16;
    casus MEDULLA_TYPUS_I32:
    casus MEDULLA_TYPUS_S32:
    casus MEDULLA_TYPUS_F32:
        redde 32;
    ordinarius:
        redde 64;
    }
}


/* ==================================================
 * Halitus + relatio
 * ================================================== */

interior vacuum
_halitum_ponere (
             Machinula* m,
                   s32  genus,
                   s64  codex,
    constans character* nuntius)
{
    m->currens        = FALSUM;
    m->halitus_genus  = genus;
    m->halitus_codex  = codex;
    si (nuntius != NIHIL)
    {
        m->halitus_nuntius = officina_chorda_ex_literis(nuntius, m->piscina);
    }
}

interior vacuum
_vitium (
             Machinula* m,
    constans character* nuntius)
{
    _halitum_ponere(m, MACHINULA_VITIUM, I, nuntius);
}

interior constans MedullaModulus*
_modulum (
    constans Machinula* m,
                   s32  modulus_index)
{
    redde conexio_modulum_obtinere(m->conexio, modulus_index);
}

interior vacuum
_positionem_imprimere (
         constans Machinula* m,
    constans MedullaFunctio* functio,
                        s32  modulus_index,
      constans FunctioPlana* plana,
                        i32  instructio_plana)
{
    constans MedullaInstructio* instructio = NIHIL;
    FILE* fl = m->ansae[II] != NIHIL ? m->ansae[II] : stderr;

    fprintf(fl, "    %.*s",
        (int)functio->titulus.mensura,
        (constans character*)functio->titulus.datum);
    si (   plana != NIHIL && plana->blocci_numerus > ZEPHYRUM
        && instructio_plana < plana->numerus)
    {
        /* index planus -> (bloccus, intra): quaestio binaria in
         * blocci_initia (stricte crescentia) - via imprimendi sola,
         * numquam in ansa calida */
                            i32  imus    = ZEPHYRUM;
                            i32  summus  = plana->blocci_numerus - I;
        constans MedullaBloccus* bloccus;

        dum (imus < summus)
        {
            i32 medius = imus + (summus - imus + I) / II;

            si (plana->blocci_initia[medius] <= instructio_plana)
            {
                imus = medius;
            }
            alioquin
            {
                summus = medius - I;
            }
        }
        bloccus = medulla_bloccum_obtinere(functio, (s32)imus);
        si (bloccus != NIHIL)
        {
            fprintf(fl, " @%.*s+%d",
                (int)bloccus->titulus.mensura,
                (constans character*)bloccus->titulus.datum,
                (int)(instructio_plana - plana->blocci_initia[imus]));
        }
        instructio = &plana->instructiones[instructio_plana];
    }
    si (   instructio          != NIHIL && instructio->origo != NIHIL
        && m->lineae_modulorum != NIHIL)
    {
        MedullaLineae** lineae = (MedullaLineae**)officina_xar_obtinere_s(
            m->lineae_modulorum, modulus_index);
        OfficinaChorda via;
           i32 linea = ZEPHYRUM;

        si (   lineae != NIHIL && *lineae != NIHIL
            && medulla_lineam_quaerere(*lineae, instructio->origo,
                   &via, &linea))
        {
            fprintf(fl, "  [%.*s:%u]", (int)via.mensura,
                (constans character*)via.datum,
                (insignatus integer)linea);
        }
    }
    fprintf(fl, "\n");
}

/* relatio halitus (recordator volatus): positio + stiva vocationum
 * + cauda anuli - ad stderr (ruinas superstat) */
interior vacuum
_relationem_imprimere (constans Machinula* m)
{
    /* M4b interludium: relatio per ansae[2] (captura sessionis
     * absorbat; NIHIL = stderr hospitis ut prius - cursor/vindex
     * immoti) */
    FILE* fl = m->ansae[II] != NIHIL ? m->ansae[II] : stderr;
    constans character* genera[] = {
        "BENE", "SISTERE", "DECIPULA", "VITIUM", "PAUSA",
        "RECUSATIO"
    };
    i32 numerus_tabulatorum = officina_xar_numerus(m->tabulata);
    i32 i;

    fprintf(fl, "\n=== MACHINULA HALITUS: %s ===\n",
        genera[m->halitus_genus]);
    si (m->halitus_nuntius.mensura > ZEPHYRUM)
    {
        fprintf(fl, "nuntius: %.*s\n",
            (int)m->halitus_nuntius.mensura,
            (constans character*)m->halitus_nuntius.datum);
    }
    fprintf(fl, "stiva vocationum (%ld tabulata, cacumen "
        "primum):\n", (long)numerus_tabulatorum);
    per (i = (i32)numerus_tabulatorum; i > ZEPHYRUM; i--)
    {
        constans Tabulatum* t = (constans Tabulatum*)officina_xar_obtinere(
            m->tabulata, i - I);

        si ((i32)numerus_tabulatorum - i >= XXXII)
        {
            fprintf(fl, "    ... (%ld reliqua)\n", (long)i);
            frange;
        }
        _positionem_imprimere(m, t->functio, t->modulus_index,
            t->plana, t->instructio);
    }
    fprintf(fl, "cauda anuli (novissima prima):\n");
    per (i = ZEPHYRUM; i < XVI; i++)
    {
                          i64  index = m->anulus_cursor - I - (i64)i;
        constans AnulusFigura* figura;

        si ((s64)index < ZEPHYRUM)
        {
            frange;
        }
        figura = &m->anulus[index & (i64)ANULUS_LARVA];
        si (figura->functio == NIHIL)
        {
            frange;
        }
        _positionem_imprimere(m, figura->functio,
            figura->modulus_index, figura->plana,
            figura->instructio);
    }
    fprintf(fl, "instructiones exsecutae: %llu\n",
        (insignatus longus longus)m->summa_instructionum);
}


/* ==================================================
 * Formator (pons printf-familiae)
 * ================================================== */

nomen structura {
                i8* regio_buf;   /* NIHIL -> fluxus */
    memoriae_index  cap;
    memoriae_index  scriptum;    /* longitudo vera (etiam ultra cap) */
              FILE* fluxus;
} Formatio;

interior vacuum
_f_emittere (
              Formatio* f,
    constans character* octeti,
        memoriae_index  numerus)
{
    si (f->fluxus != NIHIL)
    {
        (vacuum)fwrite(octeti, I, numerus, f->fluxus);
    }
    alioquin si (f->regio_buf != NIHIL && f->scriptum < f->cap)
    {
        memoriae_index spatium = f->cap - f->scriptum;
        memoriae_index n = (numerus < spatium) ? numerus : spatium;

        memcpy(f->regio_buf + f->scriptum, octeti, n);
    }
    f->scriptum += numerus;
}

/* -I = vitium (halitus iam positus) */
interior s64
_formare (
             Machinula* m,
              Formatio* f,
    constans character* forma,
          constans i64* argumenta,
                   s32  numerus)
{
    memoriae_index i                 = ZEPHYRUM;
    memoriae_index longitudo_formae  = strlen(forma);
               s32 arg               = ZEPHYRUM;
         character dirbuf[XLVIII];
         character tempus[DXII];

    dum (i < longitudo_formae)
    {
        /* cursus litteralis */
        memoriae_index initium = i;

        dum (i < longitudo_formae && forma[i] != '%')
        {
            i++;
        }
        si (i > initium)
        {
            _f_emittere(f, forma + initium, i - initium);
        }
        si (i >= longitudo_formae)
        {
            frange;
        }
        /* directiva */
        {
                  i32 d = ZEPHYRUM;
            character conversio;
            character longitudo_mod  = '\0';
                  b32 longitudo_ll   = FALSUM;
                  b32 sinistra       = FALSUM;
                  s32 latitudo       = -I;
                  s32 praecisio      = -I;

            dirbuf[d] = '%';
            d++;
            i++;   /* post % */
            /* vexilla */
            dum (   i < longitudo_formae && (forma[i] == '-'
                || forma[i] == '+' || forma[i] == ' '
                || forma[i] == '0' || forma[i] == '#'))
            {
                si (forma[i] == '-')
                {
                    sinistra = VERUM;
                }
                si (d < (i32)XL)
                {
                    dirbuf[d] = forma[i];
                    d++;
                }
                i++;
            }
            /* latitudo */
            si (i < longitudo_formae && forma[i] == '*')
            {
                si (arg >= numerus)
                {
                    _vitium(m, "forma: argumenta exhausta");
                    redde -I;
                }
                latitudo = (s32)_canonicum(argumenta[arg],
                    MEDULLA_TYPUS_S32);
                arg++;
                si (latitudo < ZEPHYRUM)
                {
                    sinistra = VERUM;
                    latitudo = -latitudo;
                }
                d += (i32)sprintf(dirbuf + d, "%s%d",
                    sinistra ? "-" : "", (int)latitudo);
                i++;
            }
            alioquin
            {
                dum (   i < longitudo_formae && forma[i] >= '0'
                     && forma[i] <= '9')
                {
                    latitudo = ((latitudo < ZEPHYRUM)
                        ? ZEPHYRUM : latitudo * X)
                        + (s32)(forma[i] - '0');
                    si (d < (i32)XL)
                    {
                        dirbuf[d] = forma[i];
                        d++;
                    }
                    i++;
                }
            }
            /* praecisio */
            si (i < longitudo_formae && forma[i] == '.')
            {
                i++;
                praecisio = ZEPHYRUM;
                si (i < longitudo_formae && forma[i] == '*')
                {
                    si (arg >= numerus)
                    {
                        _vitium(m, "forma: argumenta exhausta");
                        redde -I;
                    }
                    praecisio = (s32)_canonicum(argumenta[arg],
                        MEDULLA_TYPUS_S32);
                    arg++;
                    si (praecisio < ZEPHYRUM)
                    {
                        praecisio = -I;
                    }
                    i++;
                }
                alioquin
                {
                    dum (   i < longitudo_formae && forma[i] >= '0'
                         && forma[i] <= '9')
                    {
                        praecisio = praecisio * X
                            + (s32)(forma[i] - '0');
                        i++;
                    }
                }
                si (praecisio >= ZEPHYRUM)
                {
                    d += (i32)sprintf(dirbuf + d, ".%d",
                        (int)praecisio);
                }
            }
            /* modificator longitudinis */
            dum (   i < longitudo_formae && (forma[i] == 'l'
                || forma[i] == 'h' || forma[i] == 'z'))
            {
                si (forma[i] == 'l' && longitudo_mod == 'l')
                {
                    longitudo_ll = VERUM;
                }
                longitudo_mod = forma[i];
                i++;
            }
            si (i >= longitudo_formae)
            {
                _vitium(m, "forma trunca");
                redde -I;
            }
            conversio = forma[i];
            i++;

            si (conversio == '%')
            {
                _f_emittere(f, "%", I);
                perge;
            }
            si (conversio == 's')
            {
                constans character* littera;
                    memoriae_index  longitudo_litterae;

                si (arg >= numerus)
                {
                    _vitium(m, "forma: argumenta exhausta");
                    redde -I;
                }
                littera = (constans character*)(memoriae_index)
                    argumenta[arg];
                arg++;
                si (littera == NIHIL)
                {
                    littera = "(nihil)";
                }
                longitudo_litterae = strlen(littera);
                si (   praecisio >= ZEPHYRUM
                    && (memoriae_index)praecisio
                        < longitudo_litterae)
                {
                    longitudo_litterae = (memoriae_index)praecisio;
                }
                si (   latitudo > ZEPHYRUM && !sinistra
                    && (memoriae_index)latitudo
                        > longitudo_litterae)
                {
                    memoriae_index k = (memoriae_index)latitudo
                        - longitudo_litterae;

                    dum (k > ZEPHYRUM)
                    {
                        _f_emittere(f, " ", I);
                        k--;
                    }
                }
                _f_emittere(f, littera, longitudo_litterae);
                si (   latitudo > ZEPHYRUM && sinistra
                    && (memoriae_index)latitudo
                        > longitudo_litterae)
                {
                    memoriae_index k = (memoriae_index)latitudo
                        - longitudo_litterae;

                    dum (k > ZEPHYRUM)
                    {
                        _f_emittere(f, " ", I);
                        k--;
                    }
                }
                perge;
            }
            /* conversiones numericae: per snprintf hospitis */
            {
                i64 verbum  = ZEPHYRUM;
                int n       = ZEPHYRUM;
                b32 sumit   = (b32)(conversio != '%');

                si (sumit)
                {
                    si (arg >= numerus)
                    {
                        _vitium(m, "forma: argumenta exhausta");
                        redde -I;
                    }
                    verbum = argumenta[arg];
                    arg++;
                }
                /* modificator in dirbuf */
                si (longitudo_ll)
                {
                    dirbuf[d] = 'l';
                    d++;
                    dirbuf[d] = 'l';
                    d++;
                }
                alioquin si (longitudo_mod == 'l')
                {
                    dirbuf[d] = 'l';
                    d++;
                }
                alioquin si (longitudo_mod == 'z')
                {
                    dirbuf[d] = 'z';
                    d++;
                }
                dirbuf[d] = conversio;
                d++;
                dirbuf[d] = '\0';

                commutatio (conversio)
                {
                casus 'd':
                casus 'i':
                    si (longitudo_ll)
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (signatus longus longus)(s64)verbum);
                    }
                    alioquin si (longitudo_mod == 'l')
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (longus)(s64)verbum);
                    }
                    alioquin si (longitudo_mod == 'z')
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (memoriae_index)verbum);
                    }
                    alioquin
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (int)(s64)verbum);
                    }
                    frange;
                casus 'u':
                casus 'o':
                casus 'x':
                casus 'X':
                    si (longitudo_ll)
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (insignatus longus longus)verbum);
                    }
                    alioquin si (longitudo_mod == 'l')
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (insignatus longus)verbum);
                    }
                    alioquin si (longitudo_mod == 'z')
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (memoriae_index)verbum);
                    }
                    alioquin
                    {
                        n = snprintf(tempus, DXII, dirbuf,
                            (insignatus integer)(verbum
                                & (i64)0xFFFFFFFF));
                    }
                    frange;
                casus 'c':
                    n = snprintf(tempus, DXII, dirbuf,
                        (int)(s64)_canonicum(verbum,
                            MEDULLA_TYPUS_S32));
                    frange;
                casus 'f':
                casus 'e':
                casus 'E':
                casus 'g':
                casus 'G':
                    n = snprintf(tempus, DXII, dirbuf,
                        _f64_de_verbo(verbum));
                    frange;
                casus 'p':
                    n = snprintf(tempus, DXII, dirbuf,
                        (vacuum*)(memoriae_index)verbum);
                    frange;
                ordinarius:
                    _vitium(m, "forma: conversio ignota");
                    redde -I;
                }
                si (n > ZEPHYRUM)
                {
                    _f_emittere(f, tempus,
                        (n < (int)(DXII - I))
                            ? (memoriae_index)n
                            : (memoriae_index)(DXII - I));
                }
            }
        }
    }
    /* NUL snprintf-modi */
    si (f->regio_buf != NIHIL && f->cap > ZEPHYRUM)
    {
        memoriae_index finis = (f->scriptum < f->cap - I)
            ? f->scriptum : f->cap - I;

        f->regio_buf[finis] = ZEPHYRUM;
    }
    redde (s64)f->scriptum;
}


/* ==================================================
 * Aedificata (pavimentum Undae 0)
 * ================================================== */

interior b32
_aed_malloc (
       Machinula* m,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    (vacuum)numerus;
    *fructus = (i64)(memoriae_index)regio_allocare(m->regio,
        (memoriae_index)argumenta[ZEPHYRUM]);
    redde VERUM;
}

interior b32
_aed_free (
       Machinula* m,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    (vacuum)numerus;
    *fructus = ZEPHYRUM;
    si (argumenta[ZEPHYRUM] == ZEPHYRUM)
    {
        redde VERUM;   /* free(NULL) licet */
    }
    si (!regio_liberare(m->regio,
            (vacuum*)(memoriae_index)argumenta[ZEPHYRUM]))
    {
        _vitium(m, "free: monstrator insanus");
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_aed_realloc (
       Machinula* m,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    (vacuum)numerus;
    *fructus = (i64)(memoriae_index)regio_reallocare(m->regio,
        (vacuum*)(memoriae_index)argumenta[ZEPHYRUM],
        (memoriae_index)argumenta[I]);
    redde VERUM;
}

interior b32
_aed_strlen (
       Machinula* m,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    (vacuum)m;
    (vacuum)numerus;
    *fructus = (i64)strlen(
        (constans character*)(memoriae_index)argumenta[ZEPHYRUM]);
    redde VERUM;
}

interior b32
_aed_strcpy (
       Machinula* m,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    (vacuum)m;
    (vacuum)numerus;
    strcpy((character*)(memoriae_index)argumenta[ZEPHYRUM],
        (constans character*)(memoriae_index)argumenta[I]);
    *fructus = argumenta[ZEPHYRUM];
    redde VERUM;
}

interior b32
_aed_memcpy (
       Machinula* m,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    (vacuum)m;
    (vacuum)numerus;
    memcpy((vacuum*)(memoriae_index)argumenta[ZEPHYRUM],
        (constans vacuum*)(memoriae_index)argumenta[I],
        (memoriae_index)argumenta[II]);
    *fructus = argumenta[ZEPHYRUM];
    redde VERUM;
}

interior b32
_aed_memset (
       Machinula* m,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    (vacuum)m;
    (vacuum)numerus;
    memset((vacuum*)(memoriae_index)argumenta[ZEPHYRUM],
        (int)(s64)_canonicum(argumenta[I], MEDULLA_TYPUS_S32),
        (memoriae_index)argumenta[II]);
    *fructus = argumenta[ZEPHYRUM];
    redde VERUM;
}

interior b32
_aed_memcmp (
       Machinula* m,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    (vacuum)m;
    (vacuum)numerus;
    *fructus = (i64)(s64)memcmp(
        (constans vacuum*)(memoriae_index)argumenta[ZEPHYRUM],
        (constans vacuum*)(memoriae_index)argumenta[I],
        (memoriae_index)argumenta[II]);
    redde VERUM;
}

interior b32
_aed_fabs (
       Machinula* m,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    f64 valor = _f64_de_verbo(argumenta[ZEPHYRUM]);

    (vacuum)m;
    (vacuum)numerus;
    *fructus = _verbum_de_f64((valor < 0.0) ? -valor : valor);
    redde VERUM;
}

interior b32
_aed_exit (
       Machinula* m,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    (vacuum)numerus;
    *fructus = ZEPHYRUM;
    _halitum_ponere(m, MACHINULA_BENE,
        (s64)_canonicum(argumenta[ZEPHYRUM], MEDULLA_TYPUS_S32),
        NIHIL);
    redde FALSUM;   /* ansa frangitur; BENE - sine relatione */
}

interior b32
_aed_printf (
       Machinula* m,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    Formatio f;
         s64 n;

    f.regio_buf  = NIHIL;
    f.cap        = ZEPHYRUM;
    f.scriptum   = ZEPHYRUM;
    f.fluxus     = _ansam_solvere(m, I);   /* M4b: captura per ansae[1] */
    n = _formare(m, &f,
        (constans character*)(memoriae_index)argumenta[ZEPHYRUM],
        argumenta + I, numerus - I);
    *fructus = (i64)n;
    redde (b32)(n >= ZEPHYRUM);
}

interior b32
_aed_fprintf (
       Machinula* m,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    Formatio f;
         s64 n;

    f.regio_buf  = NIHIL;
    f.cap        = ZEPHYRUM;
    f.scriptum   = ZEPHYRUM;
    f.fluxus     = _ansam_solvere(m, argumenta[ZEPHYRUM]);
    si (f.fluxus == NIHIL)
    {
        _vitium(m, "fprintf: ansa ignota");
        redde FALSUM;
    }
    n = _formare(m, &f,
        (constans character*)(memoriae_index)argumenta[I],
        argumenta + II, numerus - II);
    *fructus = (i64)n;
    redde (b32)(n >= ZEPHYRUM);
}

interior b32
_aed_snprintf (
       Machinula* m,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    Formatio f;
         s64 n;

    f.regio_buf  = (i8*)(memoriae_index)argumenta[ZEPHYRUM];
    f.cap        = (memoriae_index)argumenta[I];
    f.scriptum   = ZEPHYRUM;
    f.fluxus     = NIHIL;
    n = _formare(m, &f,
        (constans character*)(memoriae_index)argumenta[II],
        argumenta + III, numerus - III);
    *fructus = (i64)n;
    redde (b32)(n >= ZEPHYRUM);
}

interior b32
_aed_sprintf (
       Machinula* m,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    Formatio f;
         s64 n;

    f.regio_buf  = (i8*)(memoriae_index)argumenta[ZEPHYRUM];
    f.cap        = (memoriae_index)0x7FFFFFFF;
    f.scriptum   = ZEPHYRUM;
    f.fluxus     = NIHIL;
    n = _formare(m, &f,
        (constans character*)(memoriae_index)argumenta[I],
        argumenta + II, numerus - II);
    *fructus = (i64)n;
    redde (b32)(n >= ZEPHYRUM);
}

/* scriptura-retro errno (interview Q1/M2b): post aedificata quae
 * errno hospitis ponunt, cella VM renovatur */
interior vacuum
_errno_retro (
    Machinula* m)
{
    si (m->cella_errno != NIHIL)
    {
        *m->cella_errno = (s64)errno;
    }
}

interior b32
_aed_strcmp (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = (i64)(s64)strcmp(
        (constans character*)(memoriae_index)a[ZEPHYRUM],
        (constans character*)(memoriae_index)a[I]);
    redde VERUM;
}

interior b32
_aed_strncmp (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = (i64)(s64)strncmp(
        (constans character*)(memoriae_index)a[ZEPHYRUM],
        (constans character*)(memoriae_index)a[I],
        (memoriae_index)a[II]);
    redde VERUM;
}

interior b32
_aed_strchr (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = (i64)(memoriae_index)strchr(
        (constans character*)(memoriae_index)a[ZEPHYRUM],
        (int)(s64)_canonicum(a[I], MEDULLA_TYPUS_S32));
    redde VERUM;
}

interior b32
_aed_strstr (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = (i64)(memoriae_index)strstr(
        (constans character*)(memoriae_index)a[ZEPHYRUM],
        (constans character*)(memoriae_index)a[I]);
    redde VERUM;
}

interior b32
_aed_strncpy (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)m; (vacuum)n;
    strncpy((character*)(memoriae_index)a[ZEPHYRUM],
        (constans character*)(memoriae_index)a[I],
        (memoriae_index)a[II]);
    *fr = a[ZEPHYRUM];
    redde VERUM;
}

interior b32
_aed_memmove (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)m; (vacuum)n;
    memmove((vacuum*)(memoriae_index)a[ZEPHYRUM],
        (constans vacuum*)(memoriae_index)a[I],
        (memoriae_index)a[II]);
    *fr = a[ZEPHYRUM];
    redde VERUM;
}

interior b32
_aed_atoi (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = (i64)(s64)atoi(
        (constans character*)(memoriae_index)a[ZEPHYRUM]);
    redde VERUM;
}

interior b32
_aed_atof (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = _verbum_de_f64(atof(
        (constans character*)(memoriae_index)a[ZEPHYRUM]));
    redde VERUM;
}

interior b32
_aed_abs (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    s64 v = (s64)_canonicum(a[ZEPHYRUM], MEDULLA_TYPUS_S32);

    (vacuum)m; (vacuum)n;
    *fr = _canonicum((i64)((v < ZEPHYRUM) ? -v : v),
        MEDULLA_TYPUS_S32);
    redde VERUM;
}

interior b32
_aed_strtod (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    character* finis = NIHIL;
          f64  v;

    (vacuum)n;
    errno = ZEPHYRUM;
    v = strtod((constans character*)(memoriae_index)a[ZEPHYRUM],
        &finis);
    si (a[I] != ZEPHYRUM)
    {
        i64 verbum = (i64)(memoriae_index)finis;

        memcpy((vacuum*)(memoriae_index)a[I], &verbum,
            magnitudo(i64));
    }
    _errno_retro(m);
    *fr = _verbum_de_f64(v);
    redde VERUM;
}

interior b32
_aed_strtol (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    character* finis = NIHIL;
       longus  v;

    (vacuum)n;
    errno = ZEPHYRUM;
    v = strtol((constans character*)(memoriae_index)a[ZEPHYRUM],
        &finis, (int)(s64)_canonicum(a[II], MEDULLA_TYPUS_S32));
    si (a[I] != ZEPHYRUM)
    {
        i64 verbum = (i64)(memoriae_index)finis;

        memcpy((vacuum*)(memoriae_index)a[I], &verbum,
            magnitudo(i64));
    }
    _errno_retro(m);
    *fr = (i64)(s64)v;
    redde VERUM;
}

interior b32
_aed_strtoul (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
            character* finis = NIHIL;
    insignatus longus  v;

    (vacuum)n;
    errno = ZEPHYRUM;
    v = strtoul((constans character*)(memoriae_index)a[ZEPHYRUM],
        &finis, (int)(s64)_canonicum(a[II], MEDULLA_TYPUS_S32));
    si (a[I] != ZEPHYRUM)
    {
        i64 verbum = (i64)(memoriae_index)finis;

        memcpy((vacuum*)(memoriae_index)a[I], &verbum,
            magnitudo(i64));
    }
    _errno_retro(m);
    *fr = (i64)v;
    redde VERUM;
}

#define AED_CTYPE(TITULUS, FUNCTIO) \
interior b32 \
TITULUS (Machinula* m, constans i64* a, s32 n, i64* fr) \
{ \
    (vacuum)m; (vacuum)n; \
    *fr = (i64)(s64)FUNCTIO( \
        (int)(s64)_canonicum(a[ZEPHYRUM], MEDULLA_TYPUS_S32)); \
    redde VERUM; \
}

AED_CTYPE(_aed_isalpha, isalpha)
AED_CTYPE(_aed_isdigit, isdigit)
AED_CTYPE(_aed_islower, islower)
AED_CTYPE(_aed_isupper, isupper)
AED_CTYPE(_aed_isspace, isspace)
AED_CTYPE(_aed_iscntrl, iscntrl)
AED_CTYPE(_aed_tolower, tolower)
AED_CTYPE(_aed_toupper, toupper)

#define AED_MATH1(TITULUS, FUNCTIO) \
interior b32 \
TITULUS (Machinula* m, constans i64* a, s32 n, i64* fr) \
{ \
    (vacuum)m; (vacuum)n; \
    *fr = _verbum_de_f64(FUNCTIO(_f64_de_verbo(a[ZEPHYRUM]))); \
    redde VERUM; \
}

AED_MATH1(_aed_sin, sin)
AED_MATH1(_aed_cos, cos)
AED_MATH1(_aed_sqrt, sqrt)
AED_MATH1(_aed_floor, floor)

interior b32
_aed_pow (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = _verbum_de_f64(pow(_f64_de_verbo(a[ZEPHYRUM]),
        _f64_de_verbo(a[I])));
    redde VERUM;
}

interior b32
_aed_fmod (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = _verbum_de_f64(fmod(_f64_de_verbo(a[ZEPHYRUM]),
        _f64_de_verbo(a[I])));
    redde VERUM;
}

interior b32
_aed_ldexp (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = _verbum_de_f64(ldexp(_f64_de_verbo(a[ZEPHYRUM]),
        (int)(s64)_canonicum(a[I], MEDULLA_TYPUS_S32)));
    redde VERUM;
}

interior b32
_aed_fflush (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    *fr = ZEPHYRUM;
    si (n < I || a[ZEPHYRUM] == ZEPHYRUM)
    {
        /* 0 = NIHIL (omnia effundere); ambiguitas cum stdin
         * consulta - fflush(stdin) indefinitum, nemo perdit */
        (vacuum)fflush(NIHIL);
    }
    alioquin
    {
        FILE* pl = _ansam_solvere(m, a[ZEPHYRUM]);

        si (pl != NIHIL)
        {
            (vacuum)fflush(pl);
        }
    }
    redde VERUM;
}

/* time_t hospitis = longus (8 octeti) - verbum integrum;
 * scriptura per monstratorem eodem valore ac fructus (vocatio UNA) */
interior b32
_aed_time (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    time_t v;

    si (m->recusationes & MACHINULA_RECUSARE_TEMPUS)
    {
        _recusare(m, "time in sessione recusatum"
            " - determinismus replicationis");
        redde FALSUM;
    }
    v = time(NIHIL);
    si (n >= I && a[ZEPHYRUM] != ZEPHYRUM)
    {
        i64 verbum = (i64)(s64)v;

        memcpy((vacuum*)(memoriae_index)a[ZEPHYRUM], &verbum,
            magnitudo(i64));
    }
    *fr = (i64)(s64)v;
    redde VERUM;
}


/* ==================================================
 * Ansae plagularum (M2d) - FILE* = ansa opaca (DECISUS Q1)
 * ================================================== */

interior FILE*
_ansam_solvere (
    Machinula* m,
          i64  ansa)
{
    /* M4b: locelli 0/1/2 redirectiones facultativae (captura) */
    si (ansa == ZEPHYRUM)
    {
        redde m->ansae[ZEPHYRUM] != NIHIL ? m->ansae[ZEPHYRUM]
            : stdin;
    }
    si (ansa == I)
    {
        redde m->ansae[I] != NIHIL ? m->ansae[I] : stdout;
    }
    si (ansa == II)
    {
        redde m->ansae[II] != NIHIL ? m->ansae[II] : stderr;
    }
    si (ansa >= III && ansa < (i64)ANSAE_MAXIMAE)
    {
        redde m->ansae[ansa];
    }
    redde NIHIL;
}

/* M4b: halitus recusationis (politica sessionis, non defectus) */
interior vacuum
_recusare (
             Machinula* m,
    constans character* nuntius)
{
    _halitum_ponere(m, MACHINULA_RECUSATIO, I, nuntius);
}

interior b32
_aed_fopen (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    FILE* pl;
    i64 ansa = ZEPHYRUM;

    (vacuum)n;
    si (m->recusationes & MACHINULA_RECUSARE_SCRIPTURAS)
    {
        constans character* modus =
            (constans character*)(memoriae_index)a[I];

        si (   modus == NIHIL || strchr(modus, 'w') != NIHIL
            || strchr(modus, 'a') != NIHIL
            || strchr(modus, '+') != NIHIL)
        {
            _recusare(m, "fopen scribens in sessione recusatus"
                " - dilatio nominata (umbra)");
            redde FALSUM;
        }
    }
    errno = ZEPHYRUM;
    pl = fopen((constans character*)(memoriae_index)a[ZEPHYRUM],
        (constans character*)(memoriae_index)a[I]);
    si (pl != NIHIL)
    {
        i64 k;

        per (k = III; k < (i64)ANSAE_MAXIMAE; k++)
        {
            si (m->ansae[k] == NIHIL)
            {
                m->ansae[k]  = pl;
                ansa         = k;
                frange;
            }
        }
        si (ansa == ZEPHYRUM)
        {
            fclose(pl);
            _vitium(m, "ansae exhaustae");
            redde FALSUM;
        }
    }
    _errno_retro(m);
    *fr = ansa;
    redde VERUM;
}

interior b32
_aed_fclose (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
     i64  ansa  = a[ZEPHYRUM];
    FILE* pl    = _ansam_solvere(m, ansa);

    (vacuum)n;
    si (pl == NIHIL || ansa < III)
    {
        _vitium(m, "fclose: ansa ignota");
        redde FALSUM;
    }
    *fr             = (i64)(s64)fclose(pl);
    m->ansae[ansa]  = NIHIL;
    redde VERUM;
}

interior b32
_aed_fread (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    FILE* pl = _ansam_solvere(m, a[III]);

    (vacuum)n;
    si (   a[III] == ZEPHYRUM
        && (m->recusationes & MACHINULA_RECUSARE_INITUM))
    {
        _recusare(m, "lectio stdin in sessione recusata"
            " - dilatio nominata (acta)");
        redde FALSUM;
    }
    si (pl == NIHIL)
    {
        _vitium(m, "fread: ansa ignota");
        redde FALSUM;
    }
    *fr = (i64)fread((vacuum*)(memoriae_index)a[ZEPHYRUM],
        (memoriae_index)a[I], (memoriae_index)a[II], pl);
    redde VERUM;
}

interior b32
_aed_fwrite (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    FILE* pl = _ansam_solvere(m, a[III]);

    (vacuum)n;
    si (pl == NIHIL)
    {
        _vitium(m, "fwrite: ansa ignota");
        redde FALSUM;
    }
    *fr = (i64)fwrite((constans vacuum*)(memoriae_index)a[ZEPHYRUM],
        (memoriae_index)a[I], (memoriae_index)a[II], pl);
    redde VERUM;
}

interior b32
_aed_fseek (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    FILE* pl = _ansam_solvere(m, a[ZEPHYRUM]);

    (vacuum)n;
    si (pl == NIHIL)
    {
        _vitium(m, "fseek: ansa ignota");
        redde FALSUM;
    }
    *fr = (i64)(s64)fseek(pl, (long)(s64)a[I],
        (int)(s64)_canonicum(a[II], MEDULLA_TYPUS_S32));
    redde VERUM;
}

interior b32
_aed_ftell (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    FILE* pl = _ansam_solvere(m, a[ZEPHYRUM]);

    (vacuum)n;
    si (pl == NIHIL)
    {
        _vitium(m, "ftell: ansa ignota");
        redde FALSUM;
    }
    *fr = (i64)(s64)ftell(pl);
    redde VERUM;
}

interior b32
_aed_fgets (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
         FILE* pl = _ansam_solvere(m, a[II]);
    character* fructus_hospitis;

    (vacuum)n;
    si (   a[II] == ZEPHYRUM
        && (m->recusationes & MACHINULA_RECUSARE_INITUM))
    {
        _recusare(m, "lectio stdin in sessione recusata"
            " - dilatio nominata (acta)");
        redde FALSUM;
    }
    si (pl == NIHIL)
    {
        _vitium(m, "fgets: ansa ignota");
        redde FALSUM;
    }
    fructus_hospitis = fgets((character*)(memoriae_index)a[ZEPHYRUM],
        (int)(s64)_canonicum(a[I], MEDULLA_TYPUS_S32), pl);
    /* fructus = monstrator HOSPITIS (= hospes verus) aut NIHIL */
    *fr = (fructus_hospitis != NIHIL) ? a[ZEPHYRUM] : ZEPHYRUM;
    redde VERUM;
}

interior b32
_aed_fputc (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    FILE* pl = _ansam_solvere(m, a[I]);

    (vacuum)n;
    si (pl == NIHIL)
    {
        _vitium(m, "fputc: ansa ignota");
        redde FALSUM;
    }
    *fr = (i64)(s64)fputc(
        (int)(s64)_canonicum(a[ZEPHYRUM], MEDULLA_TYPUS_S32), pl);
    redde VERUM;
}

interior b32
_aed_remove (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)n;
    si (m->recusationes & MACHINULA_RECUSARE_SCRIPTURAS)
    {
        _recusare(m, "remove in sessione recusatum");
        redde FALSUM;
    }
    errno = ZEPHYRUM;
    *fr = (i64)(s64)remove(
        (constans character*)(memoriae_index)a[ZEPHYRUM]);
    _errno_retro(m);
    redde VERUM;
}

interior b32
_aed_rename (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)n;
    si (m->recusationes & MACHINULA_RECUSARE_SCRIPTURAS)
    {
        _recusare(m, "rename in sessione recusatum");
        redde FALSUM;
    }
    errno = ZEPHYRUM;
    *fr = (i64)(s64)rename(
        (constans character*)(memoriae_index)a[ZEPHYRUM],
        (constans character*)(memoriae_index)a[I]);
    _errno_retro(m);
    redde VERUM;
}

/* stat/gettimeofday: forma nostra ≡ formae hospitis CERTIFICATA
 * (auspex_posix.sh) - scriptura DIRECTA per monstratorem hospitis */
interior b32
_aed_stat (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)n;
    errno = ZEPHYRUM;
    *fr = (i64)(s64)stat(
        (constans character*)(memoriae_index)a[ZEPHYRUM],
        (structura stat*)(memoriae_index)a[I]);
    _errno_retro(m);
    redde VERUM;
}

interior b32
_aed_mkdir (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)n;
    si (m->recusationes & MACHINULA_RECUSARE_SCRIPTURAS)
    {
        _recusare(m, "mkdir in sessione recusatum");
        redde FALSUM;
    }
    errno = ZEPHYRUM;
    *fr = (i64)(s64)mkdir(
        (constans character*)(memoriae_index)a[ZEPHYRUM],
        (mode_t)(s64)_canonicum(a[I], MEDULLA_TYPUS_S32));
    _errno_retro(m);
    redde VERUM;
}

interior b32
_aed_getcwd (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    character* fructus_hospitis;

    (vacuum)n;
    errno = ZEPHYRUM;
    fructus_hospitis = getcwd(
        (character*)(memoriae_index)a[ZEPHYRUM],
        (memoriae_index)a[I]);
    _errno_retro(m);
    *fr = (fructus_hospitis != NIHIL) ? a[ZEPHYRUM] : ZEPHYRUM;
    redde VERUM;
}

interior b32
_aed_gettimeofday (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)n;
    si (m->recusationes & MACHINULA_RECUSARE_TEMPUS)
    {
        _recusare(m, "gettimeofday in sessione recusatum"
            " - determinismus replicationis");
        redde FALSUM;
    }
    *fr = (i64)(s64)gettimeofday(
        (structura timeval*)(memoriae_index)a[ZEPHYRUM], NIHIL);
    redde VERUM;
}

interior b32
_aed_clock (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)a; (vacuum)n;
    si (m->recusationes & MACHINULA_RECUSARE_TEMPUS)
    {
        _recusare(m, "clock in sessione recusatum"
            " - determinismus replicationis");
        redde FALSUM;
    }
    *fr = (i64)(s64)clock();
    redde VERUM;
}

interior b32
_aed_unlink (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)n;
    si (m->recusationes & MACHINULA_RECUSARE_SCRIPTURAS)
    {
        _recusare(m, "unlink in sessione recusatum");
        redde FALSUM;
    }
    errno = ZEPHYRUM;
    *fr = (i64)(s64)unlink(
        (constans character*)(memoriae_index)a[ZEPHYRUM]);
    _errno_retro(m);
    redde VERUM;
}

/* tm hospitis staticum EXTRA regionem redditur - lectio sola per
 * novem campos normae (praefixum idem); mktime cum tm hospitis
 * PERICULUM nominatum (tm_gmtoff ultra formam systematis) - corpus
 * mktime non vocat (auditum M2d) */
interior b32
_aed_localtime (
       Machinula* m,
    constans i64* a,
             s32  n,
             i64* fr)
{
    (vacuum)m; (vacuum)n;
    *fr = (i64)(memoriae_index)localtime(
        (constans time_t*)(memoriae_index)a[ZEPHYRUM]);
    redde VERUM;
}

nomen structura {
    constans character* titulus;
         MachinulaPons  pons;
} AedificatumNota;

interior constans AedificatumNota AEDIFICATA[] = {
    { "malloc",   _aed_malloc },
    { "free",     _aed_free },
    { "realloc",  _aed_realloc },
    { "strlen",   _aed_strlen },
    { "strcpy",   _aed_strcpy },
    { "memcpy",   _aed_memcpy },
    { "memset",   _aed_memset },
    { "memcmp",   _aed_memcmp },
    { "fabs",     _aed_fabs },
    { "exit",     _aed_exit },
    { "printf",   _aed_printf },
    { "fprintf",  _aed_fprintf },
    { "snprintf", _aed_snprintf },
    { "sprintf",  _aed_sprintf },
    { "strcmp",   _aed_strcmp },
    { "strncmp",  _aed_strncmp },
    { "strchr",   _aed_strchr },
    { "strstr",   _aed_strstr },
    { "strncpy",  _aed_strncpy },
    { "memmove",  _aed_memmove },
    { "atoi",     _aed_atoi },
    { "atof",     _aed_atof },
    { "abs",      _aed_abs },
    { "strtod",   _aed_strtod },
    { "strtol",   _aed_strtol },
    { "strtoul",  _aed_strtoul },
    { "isalpha",  _aed_isalpha },
    { "isdigit",  _aed_isdigit },
    { "islower",  _aed_islower },
    { "isupper",  _aed_isupper },
    { "isspace",  _aed_isspace },
    { "iscntrl",  _aed_iscntrl },
    { "tolower",  _aed_tolower },
    { "toupper",  _aed_toupper },
    { "sin",      _aed_sin },
    { "cos",      _aed_cos },
    { "sqrt",     _aed_sqrt },
    { "floor",    _aed_floor },
    { "pow",      _aed_pow },
    { "fmod",     _aed_fmod },
    { "ldexp",    _aed_ldexp },
    { "fflush",   _aed_fflush },
    { "time",     _aed_time },
    /* M2d: plagulae (ansae) + POSIX */
    { "fopen",        _aed_fopen },
    { "fclose",       _aed_fclose },
    { "fread",        _aed_fread },
    { "fwrite",       _aed_fwrite },
    { "fseek",        _aed_fseek },
    { "ftell",        _aed_ftell },
    { "fgets",        _aed_fgets },
    { "fputc",        _aed_fputc },
    { "remove",       _aed_remove },
    { "rename",       _aed_rename },
    { "stat",         _aed_stat },
    { "mkdir",        _aed_mkdir },
    { "getcwd",       _aed_getcwd },
    { "gettimeofday", _aed_gettimeofday },
    { "localtime",    _aed_localtime },
    { "unlink",       _aed_unlink },
    { "clock",        _aed_clock }
};
/* structuralis - numerus falsus (44 vs 42 verae) lectiones extra
 * tabulam in ansa ligationis fecit; magnitudo drift vetat */
#define AEDIFICATA_NUMERUS \
    ((i32)(magnitudo(AEDIFICATA) / magnitudo(AEDIFICATA[ZEPHYRUM])))

/* CANALIS UNUS aedificatorum - sedes memoriae/reddendi v2 (Q12) */
interior b32
_aedificatum_vocare (
       Machinula* m,
             s64  index,
    constans i64* argumenta,
             s32  numerus,
             i64* fructus)
{
    m->numerus_aedificatorum += I;
    redde AEDIFICATA[index].pons(m, argumenta, numerus, fructus);
}


/* ==================================================
 * Vita + ligatio
 * ================================================== */

Machinula*
machinula_creare (OfficinaPiscina* piscina, Conexio* conexio, Regio* regio)
{
    Machinula* m;
          i32  numerus_symbolorum;
          i32  numerus_modulorum;
          i32  i;

    si (piscina == NIHIL || conexio == NIHIL || regio == NIHIL)
    {
        redde NIHIL;
    }
    m = officina_piscina_allocare(piscina, magnitudo(Machinula));
    si (m == NIHIL)
    {
        redde NIHIL;
    }
    memset(m, ZEPHYRUM, magnitudo(Machinula));
    m->piscina   = piscina;
    m->conexio   = conexio;
    m->regio     = regio;
    m->tabulata  = officina_xar_creare(piscina, (i32)magnitudo(Tabulatum));
    m->puncta = officina_xar_creare(piscina,
        (i32)magnitudo(MachinulaPunctum));
    si (m->puncta == NIHIL)
    {
        redde NIHIL;
    }
    m->lineae_modulorum = officina_xar_creare(piscina,
        (i32)magnitudo(MedullaLineae*));
    m->stiva_basis      = (i8*)regio_stiva_initium(regio);
    m->stiva_magnitudo  = regio_stiva_magnitudo_octetorum(regio);
    si (m->tabulata == NIHIL || m->lineae_modulorum == NIHIL)
    {
        redde NIHIL;
    }
    numerus_modulorum = (i32)conexio_numerus_modulorum(conexio);
    per (i = ZEPHYRUM; i < numerus_modulorum; i++)
    {
        MedullaLineae** locellus = officina_xar_addere(m->lineae_modulorum);

        si (locellus != NIHIL)
        {
            *locellus = NIHIL;
        }
    }

    /* ligatio: decipulae notae -> AEDIFICATUM (signum in situ) */
    numerus_symbolorum = (i32)conexio_numerus_symbolorum(conexio);
    per (i = ZEPHYRUM; i < numerus_symbolorum; i++)
    {
        constans ConexioSymbolum* symbolum =
            conexio_symbolum_obtinere(conexio, (s32)i);
        i32 a;

        si (symbolum->genus != CONEXIO_SYMBOLUM_DECIPULA)
        {
            perge;
        }
        per (a = ZEPHYRUM; a < (i32)AEDIFICATA_NUMERUS; a++)
        {
            si (officina_chorda_aequalis_literis(symbolum->titulus,
                    AEDIFICATA[a].titulus))
            {
                ConexioDescriptor* descriptor =
                    (ConexioDescriptor*)symbolum->sedes;

                descriptor->signum  = CONEXIO_SIGNUM_AEDIFICATUM;
                descriptor->index   = (s64)a;
                frange;
            }
        }
    }

    /* congelatio plana (M3): instructiones + operanda cuiusque
     * functionis in acies contiguas - POST ligationem decipularum
     * (exemplar effigiem ligatam capit), ante exsecutionem. IR
     * intactum manet: exemplar machinulae proprium est. */
    {
        i32 numerus_functionum =
            (i32)conexio_numerus_functionum(conexio);
        i32 f;

        m->planae_numerus = numerus_functionum;
        si (numerus_functionum > ZEPHYRUM)
        {
            m->planae = officina_piscina_allocare(piscina,
                (memoriae_index)numerus_functionum
                    * magnitudo(FunctioPlana));
            si (m->planae == NIHIL)
            {
                redde NIHIL;
            }
            memset(m->planae, ZEPHYRUM,
                (memoriae_index)numerus_functionum
                    * magnitudo(FunctioPlana));
        }
        per (f = ZEPHYRUM; f < numerus_functionum; f++)
        {
            constans ConexioFunctioNexa* nexa =
                conexio_functionem_obtinere(conexio, (s32)f);
            constans MedullaFunctio* functio        = nexa->functio;
                       FunctioPlana* plana          =
                           &m->planae[f];
                  MedullaInstructio* instructiones  = NIHIL;
                                i32* initia;
                                i32  blocci_numerus;
                                i32  summa = ZEPHYRUM;
                                i32  b;

            si (functio == NIHIL || functio->blocci == NIHIL)
            {
                perge;   /* plana vacua: introitus -> "fluxus extra
                          * bloccum" (honestas hodierna servata) */
            }
            blocci_numerus = officina_xar_numerus(functio->blocci);
            per (b = ZEPHYRUM; b < blocci_numerus; b++)
            {
                constans MedullaBloccus* bloccus =
                    medulla_bloccum_obtinere(functio, (s32)b);

                summa += officina_xar_numerus(bloccus->instructiones);
                si (!bloccus->terminatus)
                {
                    summa += I;   /* custos fluxus */
                }
            }
            initia = officina_piscina_allocare(piscina,
                ((memoriae_index)blocci_numerus + I)
                    * magnitudo(i32));
            si (initia == NIHIL)
            {
                redde NIHIL;
            }
            si (summa > ZEPHYRUM)
            {
                instructiones = officina_piscina_allocare(piscina,
                    (memoriae_index)summa
                        * magnitudo(MedullaInstructio));
                si (instructiones == NIHIL)
                {
                    redde NIHIL;
                }
            }
            summa = ZEPHYRUM;
            per (b = ZEPHYRUM; b < blocci_numerus; b++)
            {
                constans MedullaBloccus* bloccus =
                    medulla_bloccum_obtinere(functio, (s32)b);
                i32 n = officina_xar_numerus(bloccus->instructiones);

                initia[b] = summa;
                si (n > ZEPHYRUM)
                {
                    (vacuum)officina_xar_copiare_ad_tabulam(
                        bloccus->instructiones,
                        &instructiones[summa], ZEPHYRUM, n);
                }
                summa += n;
                si (!bloccus->terminatus)
                {
                    memset(&instructiones[summa], ZEPHYRUM,
                        magnitudo(MedullaInstructio));
                    instructiones[summa].op =
                        (s32)MACHINULA_OP_FLUXUS_CUSTOS;
                    instructiones[summa].destinatio   = -I;
                    instructiones[summa].extra_index  = -I;
                    summa                             += I;
                }
            }
            initia[blocci_numerus]  = summa;
            plana->instructiones    = instructiones;
            plana->numerus          = summa;
            plana->blocci_initia    = initia;
            plana->blocci_numerus   = blocci_numerus;
            si (functio->operanda != NIHIL)
            {
                plana->operanda_numerus =
                    officina_xar_numerus(functio->operanda);
            }
            si (plana->operanda_numerus > ZEPHYRUM)
            {
                MedullaOperandum* operanda = officina_piscina_allocare(
                    piscina,
                    (memoriae_index)plana->operanda_numerus
                        * magnitudo(MedullaOperandum));

                si (operanda == NIHIL)
                {
                    redde NIHIL;
                }
                (vacuum)officina_xar_copiare_ad_tabulam(functio->operanda,
                    operanda, ZEPHYRUM, plana->operanda_numerus);
                plana->operanda = operanda;
            }
        }
    }

    /* cellae captae */
    {
        OfficinaChorda titulus;
        unio { constans character* c; i8* m; } u;

        u.c              = "errno";
        titulus.datum    = u.m;
        titulus.mensura  = V;
        m->cella_errno   = conexio_sedes_quaerere(conexio, titulus);
    }
    redde m;
}

vacuum
machinula_lineas_praebere (
                 Machinula* machinula,
                       s32  modulus_index,
    constans MedullaLineae* lineae)
{
    MedullaLineae** locellus = officina_xar_obtinere_s(
        machinula->lineae_modulorum, modulus_index);

    si (locellus != NIHIL)
    {
        unio { constans MedullaLineae* c; MedullaLineae* m; } u;

        u.c        = lineae;
        *locellus  = u.m;
    }
}

vacuum
machinula_ansam_ponere (
    Machinula* machinula,
          s32  ansa,
         FILE* plagula)
{
    si (machinula == NIHIL || ansa < ZEPHYRUM || ansa > (s32)II)
    {
        redde;   /* 0/1/2 solum - flumina norma redirigenda */
    }
    machinula->ansae[ansa] = plagula;
}

vacuum
machinula_recusationes_ponere (
    Machinula* machinula,
          i32  vexilla)
{
    si (machinula == NIHIL)
    {
        redde;
    }
    machinula->recusationes = vexilla;
}

vacuum
machinula_ansas_claudere (
    Machinula* machinula)
{
    i64 k;

    si (machinula == NIHIL)
    {
        redde;
    }
    per (k = III; k < (i64)ANSAE_MAXIMAE; k++)
    {
        si (machinula->ansae[k] != NIHIL)
        {
            fclose(machinula->ansae[k]);
            machinula->ansae[k] = NIHIL;
        }
    }
}


/* ==================================================
 * Tabulata
 * ================================================== */

interior b32
_tabulatum_impellere (
                  Machinula* m,
    constans MedullaFunctio* functio,
      constans FunctioPlana* plana,
                        s32  modulus_index,
                        s32  destinatio_vocantis,
               constans i64* argumenta,
                        s32  numerus)
{
    Tabulatum* t;
    i32 numerus_registrorum = officina_xar_numerus(functio->registra);
    memoriae_index opus = ((memoriae_index)numerus_registrorum
        * magnitudo(i64) + XV) & ~(memoriae_index)XV;
    i32 i;
    i32 numerus_parametrorum = officina_xar_numerus(functio->parametra);

    si (m->stiva_cursor + opus + STIVA_MARGO > m->stiva_magnitudo)
    {
        _vitium(m, "stiva exhausta");
        redde FALSUM;
    }
    si ((i32)officina_xar_numerus(m->tabulata) >= (i32)1048576)
    {
        _vitium(m, "stiva vocationum exhausta (recursio?)");
        redde FALSUM;
    }
    t = officina_xar_addere(m->tabulata);
    si (t == NIHIL)
    {
        _vitium(m, "tabulata exhausta");
        redde FALSUM;
    }
    t->functio = functio;
    t->plana = plana;
    t->modulus_index = modulus_index;
    t->instructio = ZEPHYRUM;
    t->registra = (i64*)(vacuum*)(m->stiva_basis + m->stiva_cursor);
    t->basis_stivae = m->stiva_cursor;
    t->destinatio_vocantis = destinatio_vocantis;
    m->stiva_cursor += opus;
    si (m->stiva_cursor > m->apex_stivae)
    {
        m->apex_stivae = m->stiva_cursor;
    }
    memset(t->registra, ZEPHYRUM,
        (memoriae_index)numerus_registrorum * magnitudo(i64));
    per (i = ZEPHYRUM; i < numerus_parametrorum
        && i < (i32)numerus; i++)
    {
        constans MedullaParametrum* parametrum =
            (constans MedullaParametrum*)officina_xar_obtinere(
                functio->parametra, i);

        t->registra[parametrum->index] = _canonicum(argumenta[i],
            parametrum->typus);
    }
    m->numerus_vocationum  += I;
    m->tabulatum_summum    = t;
    redde VERUM;
}


/* ==================================================
 * Exsecutio
 * ================================================== */

interior i64
_valor_operandi (
                    Machinula* m,
           constans Tabulatum* t,
    constans MedullaOperandum* operandum,
                          s32  typus)
{
    commutatio (operandum->genus)
    {
    casus MEDULLA_OPERANDUM_REGISTRUM:
        redde t->registra[operandum->datum.index];
    casus MEDULLA_OPERANDUM_IMMEDIATUM:
        redde _canonicum((i64)operandum->datum.immediatum, typus);
    casus MEDULLA_OPERANDUM_IMMEDIATUM_F:
        si (typus == MEDULLA_TYPUS_F32)
        {
            redde _verbum_de_f32(
                (f32)operandum->datum.immediatum_f);
        }
        redde _verbum_de_f64(operandum->datum.immediatum_f);
    casus MEDULLA_OPERANDUM_SYMBOLUM:
    {
        /* symbolum ut valor = inscriptio sedis (argumenta
         * vocationum manu scripta: $forma) */
        s32 globale = conexio_symbolum_globale(m->conexio,
            t->modulus_index, operandum->datum.index);

        si (globale < ZEPHYRUM)
        {
            _vitium(m, "operandum: symbolum insanum");
            redde ZEPHYRUM;
        }
        redde (i64)(memoriae_index)conexio_symbolum_obtinere(
            m->conexio, globale)->sedes;
    }
    ordinarius:
        _vitium(m, "operandum insanum");
        redde ZEPHYRUM;
    }
}

/* lectura canonica: suffixus operationis IPSE auctoritas est.
 * Operationes amputantes (addere/multiplicare/et...) registra
 * ut-sunt legere possunt (bits inferiores solum fluunt); operationes
 * latitudini sensibiles (comparationes, divisio, dextrorsum,
 * fluitare, redde) valorem ad latitudinem suffixi reducere DEBENT -
 * aliter semantica instructionis ab historia scriptoris pendet
 * (inventum muri M2c: inaequalis.i32 inter registrum s32-canonicum
 * et i32-canonicum verba plena comparabat - ramus falsus). */
interior i64
_valor_canonicus (
                    Machinula* m,
           constans Tabulatum* t,
    constans MedullaOperandum* operandum,
                          s32  typus)
{
    redde _canonicum(_valor_operandi(m, t, operandum, typus),
        typus);
}

interior b32
_memoriam_probare (
         Machinula* m,
               i64  inscriptio,
    memoriae_index  numerus)
{
    si (inscriptio < (i64)4096)
    {
        _vitium(m, "memoria: monstrator nullus/humilis");
        redde FALSUM;
    }
    si (   regio_custodia(m->regio)
        && (!regio_continet(m->regio,
                (constans vacuum*)(memoriae_index)inscriptio)
            || !regio_continet(m->regio,
                (constans vacuum*)((memoriae_index)inscriptio
                    + numerus - I))))
    {
        _vitium(m, "memoria: extra regionem");
        redde FALSUM;
    }
    redde VERUM;
}

b32
machinula_aperire (
    Machinula* m,
       OfficinaChorda  titulus_functionis)
{
    /* status purgatus - aperire iterabile */
    officina_xar_truncare(m->tabulata, ZEPHYRUM);
    m->tabulatum_summum         = NIHIL;
    m->stiva_cursor             = ZEPHYRUM;
    m->currens                  = VERUM;
    m->halitus_genus            = MACHINULA_BENE;
    m->halitus_codex            = ZEPHYRUM;
    m->halitus_nuntius.datum    = NIHIL;
    m->halitus_nuntius.mensura  = ZEPHYRUM;

    /* initium */
    {
        s32 index = conexio_symbolum_quaerere(m->conexio,
            titulus_functionis);
           constans ConexioSymbolum* symbolum;
         constans ConexioDescriptor* descriptor;
        constans ConexioFunctioNexa* nexa;

        si (index < ZEPHYRUM)
        {
            _vitium(m, "initium: functio non inventa");
        }
        alioquin
        {
            symbolum = conexio_symbolum_obtinere(m->conexio, index);
            si (symbolum->genus != CONEXIO_SYMBOLUM_FUNCTIO)
            {
                _vitium(m, "initium: non functio");
            }
            alioquin
            {
                descriptor = symbolum->sedes;
                nexa = conexio_functionem_obtinere(m->conexio,
                    descriptor->index);
                (vacuum)_tabulatum_impellere(m, nexa->functio,
                    &m->planae[descriptor->index],
                    nexa->modulus_index, -I, NIHIL, ZEPHYRUM);
            }
        }
    }

    redde m->currens;
}

/* instructio UNA - corpus ansae dispensationis extractum (M3 chunk
 * 4; indentatio corporis ex ansa originali servata). VERUM =
 * pergendum. Pausa (punctum tactum) currens exstinguit positione
 * INTACTA - resumptio per pergere. */
b32
machinula_gradus (Machinula* m)
{
    i64 argumenta[ARGUMENTA_MAXIMA];

    si (m == NIHIL)
    {
        redde FALSUM;
    }
    /* resumptio ex pausa IN GRADU (inventum vectis M3): gressus a
     * statu pausato = exsecutio instructionis sub puncto
     * (restitue-grade-repone), tum status currens. pergere = ansa
     * super gradus - sedes resumptionis UNA. */
    si (   m->halitus_genus == MACHINULA_PAUSA && !m->currens
        && officina_xar_numerus(m->tabulata) > ZEPHYRUM)
    {
               Tabulatum* t              = m->tabulatum_summum;
                     s32  functio_index  = (s32)(t->plana - m->planae);
                     i32  sedes          = t->instructio;
        MachinulaPunctum* punctum = _punctum_invenire(m,
            functio_index, sedes);

        m->currens        = VERUM;
        m->halitus_genus  = MACHINULA_BENE;
        m->halitus_codex  = ZEPHYRUM;
        si (punctum != NIHIL)
        {
            b32 pergendum;

            m->planae[functio_index].instructiones[sedes].op =
                punctum->op_originalis;
            pergendum = machinula_gradus(m);
            m->planae[functio_index].instructiones[sedes].op =
                (s32)MACHINULA_OP_PAUSA;
            redde pergendum;
        }
    }
    si (!m->currens)
    {
        redde FALSUM;
    }
    {
                         Tabulatum* t      =
                             m->tabulatum_summum;
             constans FunctioPlana* plana  = t->plana;
        constans MedullaInstructio* instructio;
                               s32  op;

        si (t->instructio >= plana->numerus)
        {
            _vitium(m, "fluxus extra bloccum");
            redde FALSUM;
        }
        instructio  = &plana->instructiones[t->instructio];
        op          = instructio->op;

        /* anulus + numeratores (semper) */
        {
            AnulusFigura* figura =
                &m->anulus[m->anulus_cursor & (i64)ANULUS_LARVA];

            figura->functio        = t->functio;
            figura->plana          = plana;
            figura->modulus_index  = t->modulus_index;
            figura->instructio     = t->instructio;
            m->anulus_cursor       += I;
        }
        m->numeri_op[op]        += I;
        m->summa_instructionum  += I;

        commutatio (op)
        {
        casus MEDULLA_OP_MOVERE:
            t->registra[instructio->destinatio] = _canonicum(
                _valor_operandi(m, t, &instructio->a,
                    instructio->typus), instructio->typus);
            t->instructio += I;
            frange;

        casus MEDULLA_OP_ADDERE:
        casus MEDULLA_OP_SUBTRAHERE:
        casus MEDULLA_OP_MULTIPLICARE:
        casus MEDULLA_OP_DIVIDERE:
        casus MEDULLA_OP_RESIDUUM:
        {
            s32 typus = instructio->typus;
            i64 a = _valor_canonicus(m, t, &instructio->a, typus);
            i64 b = _valor_canonicus(m, t, &instructio->b, typus);
            i64 fructus = ZEPHYRUM;

            si (typus == MEDULLA_TYPUS_F64)
            {
                f64 fa = _f64_de_verbo(a);
                f64 fb = _f64_de_verbo(b);
                f64 fr = 0.0;

                commutatio (op)
                {
                casus MEDULLA_OP_ADDERE:       fr = fa + fb; frange;
                casus MEDULLA_OP_SUBTRAHERE:   fr = fa - fb; frange;
                casus MEDULLA_OP_MULTIPLICARE: fr = fa * fb; frange;
                ordinarius:                    fr = fa / fb; frange;
                }
                fructus = _verbum_de_f64(fr);
            }
            alioquin si (typus == MEDULLA_TYPUS_F32)
            {
                f32 fa = _f32_de_verbo(a);
                f32 fb = _f32_de_verbo(b);
                f32 fr = 0.0f;

                commutatio (op)
                {
                casus MEDULLA_OP_ADDERE:       fr = fa + fb; frange;
                casus MEDULLA_OP_SUBTRAHERE:   fr = fa - fb; frange;
                casus MEDULLA_OP_MULTIPLICARE: fr = fa * fb; frange;
                ordinarius:                    fr = fa / fb; frange;
                }
                fructus = _verbum_de_f32(fr);
            }
            alioquin si (   op == MEDULLA_OP_DIVIDERE
                         || op == MEDULLA_OP_RESIDUUM)
            {
                si (b == ZEPHYRUM)
                {
                    _vitium(m, "divisio per zephyrum");
                    frange;
                }
                si (_typus_signatus(typus))
                {
                    s64 sa = (s64)a;
                    s64 sb = (s64)b;

                    si (   sb == (s64)-I
                        && sa == (s64)((i64)I << 63))
                    {
                        _vitium(m, "divisio: superfluxus");
                        frange;
                    }
                    fructus = (op == MEDULLA_OP_DIVIDERE)
                        ? (i64)(sa / sb) : (i64)(sa % sb);
                }
                alioquin
                {
                    fructus = (op == MEDULLA_OP_DIVIDERE)
                        ? a / b : a % b;
                }
                fructus = _canonicum(fructus, typus);
            }
            alioquin
            {
                commutatio (op)
                {
                casus MEDULLA_OP_ADDERE:       fructus = a + b; frange;
                casus MEDULLA_OP_SUBTRAHERE:   fructus = a - b; frange;
                ordinarius:                    fructus = a * b; frange;
                }
                fructus = _canonicum(fructus, typus);
            }
            si (m->currens)
            {
                t->registra[instructio->destinatio]  = fructus;
                t->instructio                        += I;
            }
            frange;
        }

        casus MEDULLA_OP_NEGARE:
        {
            s32 typus  = instructio->typus;
            i64 a      = _valor_operandi(m, t, &instructio->a, typus);

            si (typus == MEDULLA_TYPUS_F64)
            {
                t->registra[instructio->destinatio] =
                    _verbum_de_f64(-_f64_de_verbo(a));
            }
            alioquin si (typus == MEDULLA_TYPUS_F32)
            {
                t->registra[instructio->destinatio] =
                    _verbum_de_f32(-_f32_de_verbo(a));
            }
            alioquin
            {
                t->registra[instructio->destinatio] = _canonicum(
                    (i64)ZEPHYRUM - a, typus);
            }
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_ET:
        casus MEDULLA_OP_VEL:
        casus MEDULLA_OP_AUT:
        {
            s32 typus  = instructio->typus;
            i64 a      = _valor_operandi(m, t, &instructio->a, typus);
            i64 b      = _valor_operandi(m, t, &instructio->b, typus);
            i64 fructus;

            si (op == MEDULLA_OP_ET)
            {
                fructus = a & b;
            }
            alioquin si (op == MEDULLA_OP_VEL)
            {
                fructus = a | b;
            }
            alioquin
            {
                fructus = a ^ b;
            }
            t->registra[instructio->destinatio] = _canonicum(
                fructus, typus);
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_COMPLEMENTUM:
        {
            s32 typus  = instructio->typus;
            i64 a      = _valor_operandi(m, t, &instructio->a, typus);

            t->registra[instructio->destinatio] = _canonicum(~a,
                typus);
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_SINISTRORSUM:
        casus MEDULLA_OP_DEXTRORSUM:
        {
            s32 typus  = instructio->typus;
            i64 a      = _valor_canonicus(m, t, &instructio->a, typus);
            i64 b = _valor_operandi(m, t, &instructio->b,
                MEDULLA_TYPUS_S32);
            i64 n = b & (i64)(_latitudo_typi(typus) - I);
            i64 fructus;

            si (op == MEDULLA_OP_SINISTRORSUM)
            {
                fructus = a << n;
            }
            alioquin si (_typus_signatus(typus))
            {
                fructus = (i64)((s64)a >> n);   /* arithmeticum */
            }
            alioquin
            {
                fructus = a >> n;               /* logicum */
            }
            t->registra[instructio->destinatio] = _canonicum(
                fructus, typus);
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_AEQUALIS:
        casus MEDULLA_OP_INAEQUALIS:
        casus MEDULLA_OP_MINOR:
        casus MEDULLA_OP_MINOR_AEQUALIS:
        casus MEDULLA_OP_MAIOR:
        casus MEDULLA_OP_MAIOR_AEQUALIS:
        {
            s32 typus = instructio->typus;
            i64 a = _valor_canonicus(m, t, &instructio->a, typus);
            i64 b = _valor_canonicus(m, t, &instructio->b, typus);
            b32 verum_est = FALSUM;

            si (   typus == MEDULLA_TYPUS_F64
                || typus == MEDULLA_TYPUS_F32)
            {
                f64 fa = (typus == MEDULLA_TYPUS_F32)
                    ? (f64)_f32_de_verbo(a) : _f64_de_verbo(a);
                f64 fb = (typus == MEDULLA_TYPUS_F32)
                    ? (f64)_f32_de_verbo(b) : _f64_de_verbo(b);

                commutatio (op)
                {
                casus MEDULLA_OP_AEQUALIS:
                    verum_est = (b32)(fa == fb); frange;
                casus MEDULLA_OP_INAEQUALIS:
                    verum_est = (b32)(fa != fb); frange;
                casus MEDULLA_OP_MINOR:
                    verum_est = (b32)(fa < fb); frange;
                casus MEDULLA_OP_MINOR_AEQUALIS:
                    verum_est = (b32)(fa <= fb); frange;
                casus MEDULLA_OP_MAIOR:
                    verum_est = (b32)(fa > fb); frange;
                ordinarius:
                    verum_est = (b32)(fa >= fb); frange;
                }
            }
            alioquin si (_typus_signatus(typus))
            {
                s64 sa = (s64)a;
                s64 sb = (s64)b;

                commutatio (op)
                {
                casus MEDULLA_OP_AEQUALIS:
                    verum_est = (b32)(sa == sb); frange;
                casus MEDULLA_OP_INAEQUALIS:
                    verum_est = (b32)(sa != sb); frange;
                casus MEDULLA_OP_MINOR:
                    verum_est = (b32)(sa < sb); frange;
                casus MEDULLA_OP_MINOR_AEQUALIS:
                    verum_est = (b32)(sa <= sb); frange;
                casus MEDULLA_OP_MAIOR:
                    verum_est = (b32)(sa > sb); frange;
                ordinarius:
                    verum_est = (b32)(sa >= sb); frange;
                }
            }
            alioquin
            {
                commutatio (op)
                {
                casus MEDULLA_OP_AEQUALIS:
                    verum_est = (b32)(a == b); frange;
                casus MEDULLA_OP_INAEQUALIS:
                    verum_est = (b32)(a != b); frange;
                casus MEDULLA_OP_MINOR:
                    verum_est = (b32)(a < b); frange;
                casus MEDULLA_OP_MINOR_AEQUALIS:
                    verum_est = (b32)(a <= b); frange;
                casus MEDULLA_OP_MAIOR:
                    verum_est = (b32)(a > b); frange;
                ordinarius:
                    verum_est = (b32)(a >= b); frange;
                }
            }
            t->registra[instructio->destinatio] =
                verum_est ? (i64)I : (i64)ZEPHYRUM;
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_EXTENDERE:
            t->registra[instructio->destinatio] = _canonicum(
                _valor_operandi(m, t, &instructio->a,
                    instructio->typus), instructio->typus);
            t->instructio += I;
            frange;

        casus MEDULLA_OP_AMPUTARE:
            t->registra[instructio->destinatio] = _canonicum(
                _valor_operandi(m, t, &instructio->a,
                    instructio->typus), instructio->typus_secundus);
            t->instructio += I;
            frange;

        casus MEDULLA_OP_FLUITARE:
        {
            i64 a = _valor_canonicus(m, t, &instructio->a,
                instructio->typus);
            f64 valor;

            si (_typus_signatus(instructio->typus))
            {
                valor = (f64)(s64)a;
            }
            alioquin
            {
                valor = (f64)a;
            }
            si (instructio->typus_secundus == MEDULLA_TYPUS_F32)
            {
                t->registra[instructio->destinatio] =
                    _verbum_de_f32((f32)valor);
            }
            alioquin
            {
                t->registra[instructio->destinatio] =
                    _verbum_de_f64(valor);
            }
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_INTEGRARE:
        {
            i64 a = _valor_operandi(m, t, &instructio->a,
                instructio->typus);
            f64 valor = (instructio->typus == MEDULLA_TYPUS_F32)
                ? (f64)_f32_de_verbo(a) : _f64_de_verbo(a);
            i64 fructus;

            si (_typus_signatus(instructio->typus_secundus))
            {
                fructus = (i64)(s64)valor;
            }
            alioquin
            {
                fructus = (i64)valor;
            }
            t->registra[instructio->destinatio] = _canonicum(
                fructus, instructio->typus_secundus);
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_DILATARE:
            t->registra[instructio->destinatio] = _verbum_de_f64(
                (f64)_f32_de_verbo(_valor_operandi(m, t,
                    &instructio->a, MEDULLA_TYPUS_F32)));
            t->instructio += I;
            frange;

        casus MEDULLA_OP_ARTARE:
            t->registra[instructio->destinatio] = _verbum_de_f32(
                (f32)_f64_de_verbo(_valor_operandi(m, t,
                    &instructio->a, MEDULLA_TYPUS_F64)));
            t->instructio += I;
            frange;

        casus MEDULLA_OP_LEGERE:
        {
            i64 inscriptio = _valor_operandi(m, t, &instructio->a,
                MEDULLA_TYPUS_I64);
                       s32 typus = instructio->typus;
            memoriae_index latitudo = (memoriae_index)
                (_latitudo_typi(typus) / 8);
            i64 verbum = ZEPHYRUM;

            si (!_memoriam_probare(m, inscriptio, latitudo))
            {
                frange;
            }
            memcpy(&verbum, (constans vacuum*)(memoriae_index)
                inscriptio, latitudo);
            t->registra[instructio->destinatio] = _canonicum(
                verbum, typus);
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_SCRIBERE:
        {
            i64 inscriptio = _valor_operandi(m, t, &instructio->a,
                MEDULLA_TYPUS_I64);
                       s32 typus = instructio->typus;
            memoriae_index latitudo = (memoriae_index)
                (_latitudo_typi(typus) / 8);
            i64 verbum = _valor_operandi(m, t, &instructio->b,
                typus);

            si (!_memoriam_probare(m, inscriptio, latitudo))
            {
                frange;
            }
            memcpy((vacuum*)(memoriae_index)inscriptio, &verbum,
                latitudo);
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_ARCA:
        {
            memoriae_index mag = (memoriae_index)_valor_operandi(m,
                t, &instructio->a, MEDULLA_TYPUS_I64);
            memoriae_index ordinatio = (memoriae_index)
                _valor_operandi(m, t, &instructio->b,
                    MEDULLA_TYPUS_I64);
            memoriae_index cursor;

            si (ordinatio == ZEPHYRUM)
            {
                ordinatio = I;
            }
            si (ordinatio < XVI)
            {
                ordinatio = XVI;   /* arcae XVI-ordinatae semper */
            }
            cursor = (m->stiva_cursor + ordinatio - I)
                & ~(ordinatio - I);
            si (cursor + mag + STIVA_MARGO > m->stiva_magnitudo)
            {
                _vitium(m, "stiva exhausta (arca)");
                frange;
            }
            t->registra[instructio->destinatio] =
                (i64)(memoriae_index)(m->stiva_basis + cursor);
            m->stiva_cursor = cursor + mag;
            si (m->stiva_cursor > m->apex_stivae)
            {
                m->apex_stivae = m->stiva_cursor;
            }
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_LOCUS:
        {
            s32 globale = conexio_symbolum_globale(m->conexio,
                t->modulus_index, instructio->a.datum.index);
            constans ConexioSymbolum* symbolum;

            si (globale < ZEPHYRUM)
            {
                _vitium(m, "locus: symbolum insanum");
                frange;
            }
            symbolum = conexio_symbolum_obtinere(m->conexio,
                globale);
            t->registra[instructio->destinatio] =
                (i64)(memoriae_index)symbolum->sedes;
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_COPIA:
        {
            i64 finis = _valor_operandi(m, t, &instructio->a,
                MEDULLA_TYPUS_I64);
            i64 fons = _valor_operandi(m, t, &instructio->b,
                MEDULLA_TYPUS_I64);
            memoriae_index mag = (memoriae_index)_valor_operandi(m,
                t, &instructio->c, MEDULLA_TYPUS_I64);

            si (   !_memoriam_probare(m, finis, mag)
                || !_memoriam_probare(m, fons, mag))
            {
                frange;
            }
            memmove((vacuum*)(memoriae_index)finis,
                (constans vacuum*)(memoriae_index)fons, mag);
            t->instructio += I;
            frange;
        }

        casus MEDULLA_OP_SALIRE:
            si ((i32)instructio->a.datum.index
                >= plana->blocci_numerus)
            {
                _vitium(m, "fluxus extra bloccum");
                frange;
            }
            t->instructio =
                plana->blocci_initia[instructio->a.datum.index];
            frange;

        casus MEDULLA_OP_RAMUS:
        {
            i64 conditio = _valor_operandi(m, t, &instructio->a,
                MEDULLA_TYPUS_I64);
            s32 scopus = (conditio != ZEPHYRUM)
                ? instructio->b.datum.index
                : instructio->c.datum.index;

            si ((i32)scopus >= plana->blocci_numerus)
            {
                _vitium(m, "fluxus extra bloccum");
                frange;
            }
            t->instructio = plana->blocci_initia[scopus];
            frange;
        }

        casus MEDULLA_OP_REDDE:
        {
            i64 verbum      = ZEPHYRUM;
            s32 destinatio  = t->destinatio_vocantis;
            b32 valorem_habet =
                (b32)(instructio->a.genus
                    != MEDULLA_OPERANDUM_NIHIL);

            si (valorem_habet)
            {
                /* valor reditus canonicus per typum signaturae -
                 * symmetria cum parametris (vide _tabulatum_addere) */
                verbum = _valor_canonicus(m, t, &instructio->a,
                    t->functio->typus_reditus);
            }
            /* tabulatum solvere */
            m->stiva_cursor = t->basis_stivae;
            si (officina_xar_numerus(m->tabulata) == I)
            {
                _halitum_ponere(m, MACHINULA_BENE, (s64)verbum,
                    NIHIL);
                frange;
            }
            (vacuum)officina_xar_removere_ultimum(m->tabulata);
            m->tabulatum_summum = officina_xar_obtinere(m->tabulata,
                (i32)officina_xar_numerus(m->tabulata) - I);
            si (destinatio >= ZEPHYRUM && valorem_habet)
            {
                m->tabulatum_summum->registra[destinatio] = verbum;
            }
            frange;
        }

        casus MEDULLA_OP_VOCARE:
        {
            constans ConexioDescriptor* descriptor = NIHIL;
            s32 numerus_argumentorum = instructio->extra_numerus;
            s32 k;

            si (instructio->a.genus == MEDULLA_OPERANDUM_SYMBOLUM)
            {
                s32 globale = conexio_symbolum_globale(m->conexio,
                    t->modulus_index, instructio->a.datum.index);
                constans ConexioSymbolum* symbolum;

                si (globale < ZEPHYRUM)
                {
                    _vitium(m, "vocatio: symbolum insanum");
                    frange;
                }
                symbolum = conexio_symbolum_obtinere(m->conexio,
                    globale);
                si (   symbolum->genus != CONEXIO_SYMBOLUM_FUNCTIO
                    && symbolum->genus != CONEXIO_SYMBOLUM_DECIPULA)
                {
                    _vitium(m, "vocatio: non functio");
                    frange;
                }
                descriptor = symbolum->sedes;
            }
            alioquin
            {
                i64 inscriptio = _valor_operandi(m, t,
                    &instructio->a, MEDULLA_TYPUS_I64);

                si (!_memoriam_probare(m, inscriptio,
                        magnitudo(ConexioDescriptor)))
                {
                    frange;
                }
                descriptor = (constans ConexioDescriptor*)
                    (memoriae_index)inscriptio;
                si (   descriptor->signum
                        != CONEXIO_SIGNUM_INTERPRETATUM
                    && descriptor->signum != CONEXIO_SIGNUM_DECIPULA
                    && descriptor->signum
                        != CONEXIO_SIGNUM_AEDIFICATUM)
                {
                    _vitium(m,
                        "vocatio per monstratorem insanum");
                    frange;
                }
            }

            si (numerus_argumentorum > (s32)ARGUMENTA_MAXIMA)
            {
                _vitium(m, "vocatio: argumenta nimia");
                frange;
            }
            {
                /* signatura ante argumenta: argumentum per typum
                 * PARAMETRI aestimatur ubi nota (litera f32 ut
                 * IMMEDIATUM_F sub S64 figuram f64 pareret -
                 * dimidium falsum in parametro f32; inventum
                 * nuntium EXITUS-1). Ignota (aedificata/decipulae/
                 * argumenta supra parametra) -> S64 ut ante
                 * (promotiones variadicae: f64 recte). */
                constans MedullaFunctio* functio_nexae = NIHIL;

                si (descriptor->signum
                    == CONEXIO_SIGNUM_INTERPRETATUM)
                {
                    functio_nexae = conexio_functionem_obtinere(
                        m->conexio, descriptor->index)->functio;
                }
                per (k = ZEPHYRUM; k < numerus_argumentorum; k++)
                {
                    constans MedullaOperandum* operandum =
                        &plana->operanda[instructio->extra_index
                            + k];
                    s32 typus_argumenti = MEDULLA_TYPUS_S64;

                    si (   functio_nexae != NIHIL
                        && k < (s32)officina_xar_numerus(
                               functio_nexae->parametra))
                    {
                        typus_argumenti =
                            ((constans MedullaParametrum*)
                                officina_xar_obtinere(
                                    functio_nexae->parametra,
                                    (i32)k))->typus;
                    }
                    argumenta[k] = _valor_operandi(m, t, operandum,
                        typus_argumenti);
                }
            }
            si (!m->currens)
            {
                frange;   /* vitium in argumentis */
            }

            si (descriptor->signum == CONEXIO_SIGNUM_INTERPRETATUM)
            {
                constans ConexioFunctioNexa* nexa =
                    conexio_functionem_obtinere(m->conexio,
                        descriptor->index);

                t->instructio += I;   /* resumptio post redde */
                si (!_tabulatum_impellere(m, nexa->functio,
                        &m->planae[descriptor->index],
                        nexa->modulus_index, instructio->destinatio,
                        argumenta, numerus_argumentorum))
                {
                    frange;
                }
            }
            alioquin si (descriptor->signum
                         == CONEXIO_SIGNUM_AEDIFICATUM)
            {
                i64 fructus = ZEPHYRUM;

                si (!_aedificatum_vocare(m, descriptor->index,
                        argumenta, numerus_argumentorum, &fructus))
                {
                    frange;   /* halitus iam positus */
                }
                si (instructio->destinatio >= ZEPHYRUM)
                {
                    t->registra[instructio->destinatio] =
                        _canonicum(fructus, instructio->typus);
                }
                t->instructio += I;
            }
            alioquin
            {
                constans OfficinaChorda* titulus = conexio_decipulam_obtinere(
                    m->conexio, (s32)descriptor->index);
                character nuntius[CXXVIII];

                sprintf(nuntius, "decipula vocata: %.*s",
                    (titulus != NIHIL && titulus->mensura < C)
                        ? (int)titulus->mensura : ZEPHYRUM,
                    (titulus != NIHIL)
                        ? (constans character*)titulus->datum : "");
                _vitium(m, nuntius);
                m->halitus_genus = MACHINULA_DECIPULA;
            }
            frange;
        }

        casus MEDULLA_OP_SISTERE:
        {
            constans MedullaModulus* modulus = _modulum(m,
                t->modulus_index);
            constans OfficinaChorda* causa = medulla_causam_obtinere(
                modulus, instructio->a.datum.index);

            _halitum_ponere(m, MACHINULA_SISTERE, I, NIHIL);
            si (causa != NIHIL)
            {
                m->halitus_nuntius = *causa;
            }
            frange;
        }

        casus MACHINULA_OP_FLUXUS_CUSTOS:
            /* sedes post bloccum non-terminatum (congelatio) -
             * semantica hodierna: cadere de fine blocci = vitium */
            _vitium(m, "fluxus extra bloccum");
            frange;

        casus MACHINULA_OP_PAUSA:
            /* punctum tactum - ANTE exsecutionem; positio manet;
             * resumptio per pergere (restitue-grade-repone) */
            _halitum_ponere(m, MACHINULA_PAUSA, ZEPHYRUM, NIHIL);
            frange;

        ordinarius:
            _vitium(m, "operatio ignota");
            frange;
        }
    }
    redde m->currens;
}

interior MachinulaPunctum*
_punctum_invenire (
    constans Machinula* m,
                   s32  functio_index,
                   i32  instructio)
{
    i32 numerus = officina_xar_numerus(m->puncta);
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        MachinulaPunctum* punctum = officina_xar_obtinere(m->puncta, i);

        si (   punctum->functio_index == functio_index
            && punctum->instructio    == instructio)
        {
            redde punctum;
        }
    }
    redde NIHIL;
}

b32
machinula_punctum_ponere (Machinula* m, s32 functio_index,
    i32 instructio)
{
    FunctioPlana* plana;

    si (   m                  == NIHIL || functio_index < ZEPHYRUM
        || (i32)functio_index >= m->planae_numerus)
    {
        redde FALSUM;
    }
    plana = &m->planae[functio_index];
    si (instructio >= plana->numerus)
    {
        redde FALSUM;
    }
    si (_punctum_invenire(m, functio_index, instructio) != NIHIL)
    {
        redde VERUM;   /* iam positum */
    }
    {
        MachinulaPunctum* punctum = officina_xar_addere(m->puncta);

        si (punctum == NIHIL)
        {
            redde FALSUM;
        }
        punctum->functio_index  = functio_index;
        punctum->instructio     = instructio;
        punctum->op_originalis  = plana->instructiones[instructio].op;
        plana->instructiones[instructio].op =
            (s32)MACHINULA_OP_PAUSA;
    }
    redde VERUM;
}

b32
machinula_punctum_tollere (
    Machinula* m,
          s32  functio_index,
          i32  instructio)
{
    i32 numerus;
    i32 i;

    si (m == NIHIL)
    {
        redde FALSUM;
    }
    numerus = officina_xar_numerus(m->puncta);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        MachinulaPunctum* punctum = officina_xar_obtinere(m->puncta, i);

        si (   punctum->functio_index == functio_index
            && punctum->instructio    == instructio)
        {
            m->planae[functio_index].instructiones[instructio].op =
                punctum->op_originalis;
            (vacuum)officina_xar_removere_cum_ultimo(m->puncta, i);
            redde VERUM;
        }
    }
    redde FALSUM;
}

s32
machinula_pergere (
    Machinula* m)
{
    si (m == NIHIL)
    {
        redde MACHINULA_VITIUM;
    }
    /* resumptio ex pausa in gradu ipso vivit - ansa nuda */
    dum (machinula_gradus(m))
    {
    }
    redde m->halitus_genus;
}

s64
machinula_halitus_codex (
    constans Machinula* machinula)
{
    si (machinula == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde machinula->halitus_codex;
}

i32
machinula_tabulata_numerus (
    constans Machinula* machinula)
{
    si (machinula == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde officina_xar_numerus(machinula->tabulata);
}

b32
machinula_positionem_inspicere (
    constans Machinula* machinula,
                   i32  tabulatum_index,
                   s32* functio_index_out,
                   i32* instructio_out)
{
    constans Tabulatum* t;

    si (machinula == NIHIL)
    {
        redde FALSUM;
    }
    t = (constans Tabulatum*)officina_xar_obtinere_s(machinula->tabulata,
        (s32)tabulatum_index);
    si (t == NIHIL)
    {
        redde FALSUM;
    }
    si (functio_index_out != NIHIL)
    {
        *functio_index_out = (s32)(t->plana - machinula->planae);
    }
    si (instructio_out != NIHIL)
    {
        *instructio_out = t->instructio;
    }
    redde VERUM;
}

b32
machinula_registrum_legere (
    constans Machinula* machinula,
                   i32  tabulatum_index,
                   i32  index_registri,
                   i64* valor_out)
{
    constans Tabulatum* t;

    si (machinula == NIHIL || valor_out == NIHIL)
    {
        redde FALSUM;
    }
    t = (constans Tabulatum*)officina_xar_obtinere_s(machinula->tabulata,
        (s32)tabulatum_index);
    si (   t              == NIHIL
        || index_registri >= officina_xar_numerus(t->functio->registra))
    {
        redde FALSUM;
    }
    *valor_out = t->registra[index_registri];
    redde VERUM;
}

b32
machinula_anulum_inspicere (constans Machinula* machinula,
    i32 retro_index, s32* functio_index_out, i32* instructio_out)
{
    /* index s64 (2026-07-17): retro ultra initium historiae in i64
     * volvebatur - custodia "< 0" mortua, salvatio fortuita per
     * ordines anuli zephyratos */
                      s64  index;
    constans AnulusFigura* figura;

    si (   machinula   == NIHIL
        || retro_index >= (i32)ANULUS_MENSURA)
    {
        redde FALSUM;
    }
    index = (s64)machinula->anulus_cursor - I - (s64)retro_index;
    si (index < ZEPHYRUM)
    {
        redde FALSUM;
    }
    figura = &machinula->anulus[(i64)index & (i64)ANULUS_LARVA];
    si (figura->functio == NIHIL)
    {
        redde FALSUM;
    }
    si (functio_index_out != NIHIL)
    {
        *functio_index_out =
            (s32)(figura->plana - machinula->planae);
    }
    si (instructio_out != NIHIL)
    {
        *instructio_out = figura->instructio;
    }
    redde VERUM;
}

MachinulaExitus
machinula_currere (
    Machinula* m,
       OfficinaChorda  titulus_functionis)
{
    MachinulaExitus exitus;

    memset(&exitus, ZEPHYRUM, magnitudo(MachinulaExitus));
    exitus.genus = MACHINULA_VITIUM;
    si (machinula_aperire(m, titulus_functionis))
    {
        (vacuum)machinula_pergere(m);
    }
    si (m->halitus_genus != MACHINULA_BENE)
    {
        _relationem_imprimere(m);
    }
    exitus.genus    = m->halitus_genus;
    exitus.codex    = m->halitus_codex;
    exitus.nuntius  = m->halitus_nuntius;
    redde exitus;
}


/* ==================================================
 * Census
 * ================================================== */

i64
machinula_numerus_instructionum (
    constans Machinula* machinula)
{
    redde machinula->summa_instructionum;
}

i64
machinula_numerus_op (
    constans Machinula* machinula,
                   s32  op)
{
    si (op < ZEPHYRUM || op >= (s32)MEDULLA_OP_NUMERUS)
    {
        redde ZEPHYRUM;
    }
    redde machinula->numeri_op[op];
}

i32
machinula_numerus_instructionum_planarum (
    constans Machinula* machinula,
                   s32  functio_index)
{
    si (   machinula          == NIHIL || functio_index < ZEPHYRUM
        || (i32)functio_index >= machinula->planae_numerus)
    {
        redde ZEPHYRUM;
    }
    redde machinula->planae[functio_index].numerus;
}

i64
machinula_numerus_vocationum (
    constans Machinula* machinula)
{
    redde machinula->numerus_vocationum;
}

i64
machinula_numerus_aedificatorum (
    constans Machinula* machinula)
{
    redde machinula->numerus_aedificatorum;
}

memoriae_index
machinula_stiva_apex (
    constans Machinula* machinula)
{
    redde machinula->apex_stivae;
}

/* ================= ex officina/fontes/officina_demissio.c ================= */


/* ==================================================
 * Structurae
 * ================================================== */

nomen structura {
        i8  clavis_octeti[magnitudo(vacuum*)];
    vacuum* valor;
} DemissioIntroitus;

nomen structura {
    constans SemanticaSymbolum* symbolum;
                           s32  index_registri;  /* valor AUT
                                                  * inscriptio arcae */
    b32 est_arca;
    s32 index_symboli_moduli;  /* staticum
                                                        * locale ->
                                                        * datum; -I */
} DemissioSedes;

nomen structura {
    b32 directum;               /* VERUM: index = registrum valoris */
    s32 index;                  /* aliter registrum inscriptionis */
    b32 valida;
} DemissioLocus;

nomen structura {
    constans SemanticaSymbolum* symbolum;
           constans SilvaNodus* initiator;   /* NIHIL licet (mensura
                                              * completa acierum) */
} DemissioLocale;

nomen structura {
           OfficinaPiscina* piscina;
    SilvaSemantica* sem;
    MedullaModulus* modulus;
    MedullaFunctio* functio;
               s32  bloccus;
               s32  numerator;    /* nomina bloccorum */
               s32  frange_finis; /* -I extra */
               s32  perge_finis;
               s32  reditus_index;   /* registrum parametri
                                              * reditus aggregati
                                              * (conventio C7); -I */
    s32 reditus_mensura;
    s32 numerator_datorum;  /* chorda_N/zeri_N/
                                                 * congeries_N */
    SilvaPiscina* officina_piscina_silvae;  /* transitoria (decodere;
                                              * octeti statim copiati) */
    OfficinaTabulaDispersa* lexemata;     /* SilvaToken* -> symbolum */
    OfficinaTabulaDispersa* sedes;        /* symbolum -> DemissioSedes* */
    OfficinaTabulaDispersa* capti;        /* symbola inscriptione capta */
            OfficinaChorda  stirps;       /* "lib/chorda.c" ->
                                           * "chorda_c" (praefixum
                                           * staticorum plagulae) */
} Demissio;

/* prototypa mutuae recursionis */
interior s32
_mensura_elementi_monstratoris (
             Demissio* d,
    constans TypusC89* typus);
interior s32
_expressionem (
               Demissio* d,
    constans SilvaNodus* nodus);
interior MedullaOperandum
_ut_operandum (
               Demissio* d,
    constans SilvaNodus* nodus);
interior s32
_ut_valor (
               Demissio* d,
    constans SilvaNodus* nodus);
interior DemissioLocus
_ut_locum (
               Demissio* d,
    constans SilvaNodus* nodus);
interior vacuum
_sententiam (
               Demissio* d,
    constans SilvaNodus* nodus);


/* ==================================================
 * Auxilia parva
 * ================================================== */

interior OfficinaChorda
_ch_de_silva (
    SilvaChorda s)
{
    OfficinaChorda c;

    c.datum    = s.datum;
    c.mensura  = s.mensura;
    redde c;
}

interior OfficinaChorda
_ch_literis (
    constans character* literis)
{
    OfficinaChorda c;
    unio { constans character* c; i8* m; } u;

    u.c        = literis;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(literis);
    redde c;
}

interior vacuum*
_sine_constante (
    constans vacuum* monstrator)
{
    unio { constans vacuum* c; vacuum* m; } u;

    u.c = monstrator;
    redde u.m;
}

interior constans SilvaNodus*
_canonicus (
    constans SilvaNodus* nodus)
{
    dum (   nodus        != NIHIL
         && nodus->genus == (s32)SILVA_C89_GENUS_AMBIGUUS)
    {
        SilvaValor v = silva_c89_ambiguus_canonica(nodus);

        si (v.genus == SILVA_VALOR_NODUS)
        {
            nodus = v.datum.nodus;
            perge;
        }
        si (v.genus == SILVA_VALOR_INDEX)
        {
            /* canonica = INDEX in interpretationes (species loci
             * INDEX - inventum fusoris: genus 54 residuum) */
            SilvaValor interpretationes =
                silva_c89_ambiguus_interpretationes(nodus);
            SilvaValor* electa;

            si (v.datum.index < ZEPHYRUM)
            {
                frange;
            }
            electa = silva_valor_lista_obtinere(interpretationes,
                (unsigned int)v.datum.index);
            si (   electa        != NIHIL
                && electa->genus == SILVA_VALOR_NODUS)
            {
                nodus = electa->datum.nodus;
                perge;
            }
        }
        frange;
    }
    redde nodus;
}

interior SilvaChorda
_tok_textus (
    SilvaValor v)
{
    SilvaChorda vacua;

    si (v.genus == SILVA_VALOR_TOKEN && v.datum.token != NIHIL)
    {
        redde v.datum.token->valor;
    }
    vacua.mensura  = 0;
    vacua.datum    = NIHIL;
    redde vacua;
}

interior b32
_op_est (
           SilvaChorda  textus,
    constans character* literis)
{
    i32 longitudo = (i32)strlen(literis);

    redde textus.mensura == longitudo
        && memcmp(textus.datum, literis,
               (memoriae_index)longitudo) == ZEPHYRUM;
}


/* ==================================================
 * Tabulae monstratorum (exemplar typationis M0b)
 * ================================================== */

interior vacuum*
_tabulam_invenire (
     OfficinaTabulaDispersa* tabula,
    constans vacuum* monstrator)
{
        i8  octeti[magnitudo(vacuum*)];
    OfficinaChorda  clavis;
    vacuum* valor = NIHIL;

    memcpy(octeti, &monstrator, magnitudo(octeti));
    clavis.datum    = octeti;
    clavis.mensura  = (i32)magnitudo(octeti);
    si (officina_tabula_dispersa_invenire(tabula, clavis, &valor))
    {
        redde ((DemissioIntroitus*)valor)->valor;
    }
    redde NIHIL;
}

interior vacuum
_tabulam_ponere (
           Demissio* d,
     OfficinaTabulaDispersa* tabula,
    constans vacuum* monstrator,
             vacuum* valor)
{
    DemissioIntroitus* introitus;
               OfficinaChorda  clavis;
                   i8  octeti[magnitudo(vacuum*)];
               vacuum* prior = NIHIL;

    memcpy(octeti, &monstrator, magnitudo(octeti));
    clavis.datum    = octeti;
    clavis.mensura  = (i32)magnitudo(octeti);
    si (officina_tabula_dispersa_invenire(tabula, clavis, &prior))
    {
        ((DemissioIntroitus*)prior)->valor = valor;
        redde;
    }
    introitus = (DemissioIntroitus*)officina_piscina_allocare(d->piscina,
        (memoriae_index)magnitudo(DemissioIntroitus));
    si (introitus == NIHIL)
    {
        redde;
    }
    memcpy(introitus->clavis_octeti, &monstrator,
        magnitudo(introitus->clavis_octeti));
    introitus->valor  = valor;
    clavis.datum      = introitus->clavis_octeti;
    clavis.mensura    = (i32)magnitudo(introitus->clavis_octeti);
    (vacuum)officina_tabula_dispersa_inserere(tabula, clavis,
        (vacuum*)introitus);
}


/* ==================================================
 * Typi
 * ================================================== */

interior constans TypusC89*
_exutus (
    constans TypusC89* typus)
{
    dum (typus != NIHIL && typus->genus == TYPUS_C89_QUALIFICATUS)
    {
        typus = typus->datum.qualificatus.internum;
    }
    redde typus;
}

/* MEDULLA_TYPUS_* aut -I (non scalaris) */
interior s32
_typus_medullae (
    constans TypusC89* typus)
{
    typus = _exutus(typus);
    si (typus == NIHIL)
    {
        redde -I;
    }
    commutatio (typus->genus)
    {
        casus TYPUS_C89_PRIMITIVUS:
        {
            commutatio (typus->datum.primitivum)
            {
                casus PRIMITIVUM_CHARACTER:           redde MEDULLA_TYPUS_S8;
                casus PRIMITIVUM_CHARACTER_SIGNATUM:  redde MEDULLA_TYPUS_S8;
                casus PRIMITIVUM_CHARACTER_INSIGNATUM: redde MEDULLA_TYPUS_I8;
                casus PRIMITIVUM_BREVIS:              redde MEDULLA_TYPUS_S16;
                casus PRIMITIVUM_BREVIS_INSIGNATUM:   redde MEDULLA_TYPUS_I16;
                casus PRIMITIVUM_INTEGER:             redde MEDULLA_TYPUS_S32;
                casus PRIMITIVUM_INTEGER_INSIGNATUM:  redde MEDULLA_TYPUS_I32;
                casus PRIMITIVUM_LONGUS:              redde MEDULLA_TYPUS_S64;
                casus PRIMITIVUM_LONGUS_INSIGNATUM:   redde MEDULLA_TYPUS_I64;
                casus PRIMITIVUM_LONGUS_LONGUS:       redde MEDULLA_TYPUS_S64;
                casus PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM:
                    redde MEDULLA_TYPUS_I64;
                casus PRIMITIVUM_FLUITANS:            redde MEDULLA_TYPUS_F32;
                casus PRIMITIVUM_DUPLEX:              redde MEDULLA_TYPUS_F64;
                casus PRIMITIVUM_DUPLEX_LONGUS:       redde MEDULLA_TYPUS_F64;
                ordinarius:                           redde -I;   /* vacuum */
            }
        }
        casus TYPUS_C89_MONSTRATOR:  redde MEDULLA_TYPUS_I64;
        casus TYPUS_C89_ACIES:       redde MEDULLA_TYPUS_I64;
        casus TYPUS_C89_FUNCTIO:     redde MEDULLA_TYPUS_I64;
        casus TYPUS_C89_ENUMERATUS:  redde MEDULLA_TYPUS_S32;
        ordinarius:                  redde -I;
    }
}

interior constans TypusC89*
_typus_finalis (
      constans Demissio* d,
    constans SilvaNodus* nodus)
{
    constans TypusC89* conversus = silva_c89_conversio_expressionis(
        d->sem, nodus);

    si (conversus != NIHIL)
    {
        redde conversus;
    }
    redde silva_c89_typus_expressionis(d->sem, nodus);
}

/* magnitudo per formam; -I si forma invalida */
interior s32
_mensura_typi (
             Demissio* d,
    constans TypusC89* typus)
{
    TypusC89* mutabilis = (TypusC89*)_sine_constante(
        (constans vacuum*)typus);

    si (   typus == NIHIL
        || !silva_c89_formam_computare(d->sem, mutabilis))
    {
        redde -I;
    }
    redde (s32)typus->magnitudo_octetorum;
}

/* octeti LE (deterministici - machinula LE) */
interior vacuum
_octetos_integri (
    s64  valor,
     i8* octeti,
    s32  mensura)
{
    s32 k;

    per (k = ZEPHYRUM; k < mensura; k++)
    {
        octeti[k] = (i8)((valor >> (k * VIII)) & 0xff);
    }
}

/* aestimator fluitantium staticus (aestimator publicus integralis
 * solum est): folium fluitans/integer, -, +, parenthesis, conversio */
interior b32
_fluitantem_aestimare (
               Demissio* d,
    constans SilvaNodus* nodus,
                    f64* valor_fructus)
{
    s64 integrale = 0;

    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_FOLIUM_FLUITANS)
    {
        SilvaChorda textus = _tok_textus(
            silva_c89_folium_fluitans_tok_valor(nodus));
        character littera[XL];

        si (textus.mensura == ZEPHYRUM || textus.mensura >= XL)
        {
            redde FALSUM;
        }
        memcpy(littera, textus.datum,
            (memoriae_index)textus.mensura);
        littera[textus.mensura]  = '\0';
        *valor_fructus           = strtod(littera, NIHIL);
        redde VERUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        SilvaValor v = silva_c89_parenthesis_internum(nodus);

        redde v.genus == SILVA_VALOR_NODUS
            && _fluitantem_aestimare(d, v.datum.nodus,
                   valor_fructus);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_CONVERSIO)
    {
        SilvaValor v = silva_c89_conversio_internum(nodus);

        redde v.genus == SILVA_VALOR_NODUS
            && _fluitantem_aestimare(d, v.datum.nodus,
                   valor_fructus);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_UNARIUM)
    {
        SilvaChorda operator = _tok_textus(
            silva_c89_unarium_tok_operator(nodus));
        SilvaValor v = silva_c89_unarium_internum(nodus);
               f64 internum;

        si (   v.genus != SILVA_VALOR_NODUS
            || !_fluitantem_aestimare(d, v.datum.nodus, &internum))
        {
            redde FALSUM;
        }
        si (_op_est(operator, "-"))
        {
            *valor_fructus = -internum;
            redde VERUM;
        }
        si (_op_est(operator, "+"))
        {
            *valor_fructus = internum;
            redde VERUM;
        }
        redde FALSUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_BINARIUM)
    {
        /* initiatores tabularum fluitantium (1.0/3.0 - inventum
         * fusoris, 47 moduli) */
         SilvaValor sin_v = silva_c89_binarium_sinister(nodus);
         SilvaValor dex_v = silva_c89_binarium_dexter(nodus);
        SilvaChorda operator = _tok_textus(
            silva_c89_binarium_tok_operator(nodus));
        f64 a;
        f64 b;

        si (   sin_v.genus != SILVA_VALOR_NODUS
            || dex_v.genus != SILVA_VALOR_NODUS
            || !_fluitantem_aestimare(d, sin_v.datum.nodus, &a)
            || !_fluitantem_aestimare(d, dex_v.datum.nodus, &b))
        {
            redde FALSUM;
        }
        si (_op_est(operator, "+"))
        {
            *valor_fructus = a + b;
            redde VERUM;
        }
        si (_op_est(operator, "-"))
        {
            *valor_fructus = a - b;
            redde VERUM;
        }
        si (_op_est(operator, "*"))
        {
            *valor_fructus = a * b;
            redde VERUM;
        }
        si (_op_est(operator, "/") && b != 0.0)
        {
            *valor_fructus = a / b;
            redde VERUM;
        }
        redde FALSUM;
    }
    si (silva_c89_constans_aestimare(d->sem, nodus, &integrale))
    {
        *valor_fructus = (f64)integrale;
        redde VERUM;
    }
    redde FALSUM;
}


/* ==================================================
 * Nomina + blocci + registra
 * ================================================== */

interior OfficinaChorda
_titulum_fingere (
              Demissio* d,
    constans character* basis)
{
    character littera[XLVIII];
       OfficinaChorda temporarium;
          s32 longitudo = (s32)sprintf(littera, "%s_%d", basis,
              (int)d->numerator);

    d->numerator++;
    temporarium.datum    = (i8*)littera;
    temporarium.mensura  = (i32)longitudo;
    redde officina_chorda_transcribere(temporarium, d->piscina);
}

interior s32
_bloccum_novum (
              Demissio* d,
    constans character* basis)
{
    redde medulla_bloccum_creare(d->functio,
        _titulum_fingere(d, basis));
}

/* stirps moduli: "lib/chorda.c" -> "chorda_c" (vocabularium §II).
 * Inventum M2a in primo contactu nexus mundi: sine praefixo statica
 * plagulae + data anonyma trans modulos collidunt (M1b eas videre
 * non potuit - demissio solitaria numquam collidit). */
interior OfficinaChorda
_stirpem_computare (
    OfficinaPiscina* piscina,
     OfficinaChorda  titulus_moduli)
{
        i8 littera[LXIV];
       i32 initium   = ZEPHYRUM;
       i32 scriptum  = ZEPHYRUM;
       i32 i;
    OfficinaChorda s;

    per (i = ZEPHYRUM; i < titulus_moduli.mensura; i++)
    {
        si (titulus_moduli.datum[i] == (i8)'/')
        {
            initium = i + I;
        }
    }
    per (i = initium; i < titulus_moduli.mensura
        && scriptum < (i32)(LXIV - I); i++)
    {
         i8 c = titulus_moduli.datum[i];
        b32 idoneus = (b32)((c >= (i8)'a' && c <= (i8)'z')
            || (c >= (i8)'A' && c <= (i8)'Z')
            || (c >= (i8)'0' && c <= (i8)'9')
            || c == (i8)'_');

        littera[scriptum] = idoneus ? c : (i8)'_';
        scriptum++;
    }
    s.datum    = littera;
    s.mensura  = scriptum;
    redde officina_chorda_transcribere(s, piscina);
}

/* titulus symboli globalis - CANALIS UNICUS definitionis ET
 * referentiarum (aliter nexus frangitur): statica plagulae
 * (profunditas 0 + REPOSITIO_STATICA) praefixum stirpis accipiunt
 * ($stirps.titulus); externa titulum nudum servant. */
interior OfficinaChorda
_titulum_symboli (
                      Demissio* d,
    constans SemanticaSymbolum* symbolum)
{
    OfficinaChorda titulus = _ch_de_silva(symbolum->titulus);

    si (   symbolum->profunditas > ZEPHYRUM
        || (symbolum->repositio & REPOSITIO_STATICA) == ZEPHYRUM)
    {
        redde titulus;
    }
    {
            i8 littera[CXXVIII];
        OfficinaChorda plenus;
           i32 caput_s = (d->stirps.mensura < XL)
               ? d->stirps.mensura : XL;
        i32 caput_t = (titulus.mensura < LXXX)
            ? titulus.mensura : LXXX;

        memcpy(littera, d->stirps.datum, (memoriae_index)caput_s);
        littera[caput_s] = (i8)'.';
        memcpy(littera + caput_s + I, titulus.datum,
            (memoriae_index)caput_t);
        plenus.datum    = littera;
        plenus.mensura  = caput_s + I + caput_t;
        redde officina_chorda_transcribere(plenus, d->piscina);
    }
}

interior b32
_bloccus_vivus (
    constans Demissio* d)
{
    constans MedullaBloccus* bloccus = medulla_bloccum_obtinere(
        d->functio, d->bloccus);

    redde bloccus != NIHIL && !bloccus->terminatus;
}

interior vacuum
_vivum (
    Demissio* d)
{
    si (!_bloccus_vivus(d))
    {
        d->bloccus = _bloccum_novum(d, "insequens");
    }
}

interior b32
_registrum_captum (
    constans Demissio* d,
               OfficinaChorda  titulus)
{
    i32 i;
    i32 m = officina_xar_numerus(d->functio->registra);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans OfficinaChorda* sedes = (constans OfficinaChorda*)officina_xar_obtinere(
            d->functio->registra, i);

        si (   sedes->mensura == titulus.mensura
            && titulus.mensura > ZEPHYRUM
            && memcmp(sedes->datum, titulus.datum,
                   (memoriae_index)titulus.mensura) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior s32
_registrum_unicum (
       Demissio* d,
    SilvaChorda  titulus_silva)
{
    OfficinaChorda titulus = _ch_de_silva(titulus_silva);

    si (   titulus.mensura == ZEPHYRUM
        || !_registrum_captum(d, titulus))
    {
        redde medulla_registrum_novum(d->functio, titulus);
    }
    {
        character littera[LXIV];
           OfficinaChorda alterum;
              i32 scriptum = ZEPHYRUM;
              i32 caput = (titulus.mensura
                  < XL) ? titulus.mensura : XL;
              s32 n;

        memcpy(littera, titulus.datum, (memoriae_index)caput);
        scriptum = caput;
        n = (s32)sprintf(littera + scriptum, "_%d",
            (int)d->numerator);
        d->numerator++;
        alterum.datum    = (i8*)littera;
        alterum.mensura  = scriptum + (i32)n;
        redde medulla_registrum_novum(d->functio,
            officina_chorda_transcribere(alterum, d->piscina));
    }
}

interior s32
_registrum_temporarium (
    Demissio* d)
{
    OfficinaChorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde medulla_registrum_novum(d->functio, vacua);
}


/* ==================================================
 * Emissio
 * ================================================== */

interior s32
_em (
               Demissio* d,
    constans SilvaNodus* origo,
                    s32  op,
                    s32  typus,
                    s32  typus_secundus,
                    s32  destinatio,
       MedullaOperandum  a,
       MedullaOperandum  b,
       MedullaOperandum  c)
{
    MedullaInstructio instructio;

    _vivum(d);
    memset(&instructio, ZEPHYRUM, magnitudo(MedullaInstructio));
    instructio.op              = op;
    instructio.typus           = typus;
    instructio.typus_secundus  = typus_secundus;
    instructio.destinatio      = destinatio;
    instructio.extra_index     = -I;
    instructio.a               = a;
    instructio.b               = b;
    instructio.c               = c;
    instructio.origo           = origo;
    (vacuum)medulla_emittere(d->functio, d->bloccus, &instructio);
    redde destinatio;
}

interior vacuum
_sistere (
               Demissio* d,
    constans SilvaNodus* origo,
     constans character* causa)
{
    s32 index = medulla_causam_internare(d->modulus,
        _ch_literis(causa));

    (vacuum)_em(d, origo, MEDULLA_OP_SISTERE, MEDULLA_TYPUS_NIHIL,
        MEDULLA_TYPUS_NIHIL, -I, medulla_op_causa(index),
        medulla_op_nihil(), medulla_op_nihil());
}

interior vacuum
_salire_si_vivus (
    Demissio* d,
         s32  finis)
{
    si (_bloccus_vivus(d))
    {
        (vacuum)_em(d, NIHIL, MEDULLA_OP_SALIRE, MEDULLA_TYPUS_NIHIL,
            MEDULLA_TYPUS_NIHIL, -I, medulla_op_bloccum(finis),
            medulla_op_nihil(), medulla_op_nihil());
    }
}

interior vacuum
_ramus (
    Demissio* d,
    constans SilvaNodus* origo,
    MedullaOperandum
    conditio,
    s32 verum,
    s32 falsum)
{
    (vacuum)_em(d, origo, MEDULLA_OP_RAMUS, MEDULLA_TYPUS_NIHIL,
        MEDULLA_TYPUS_NIHIL, -I, conditio,
        medulla_op_bloccum(verum), medulla_op_bloccum(falsum));
}

/* movere operandi in registrum novum (typus datur) */
interior s32
_movere (
               Demissio* d,
    constans SilvaNodus* origo,
                    s32  typus,
                    s32  destinatio,
       MedullaOperandum  a)
{
    redde _em(d, origo, MEDULLA_OP_MOVERE,
        (typus >= ZEPHYRUM) ? typus : MEDULLA_TYPUS_S64,
        MEDULLA_TYPUS_NIHIL, destinatio, a, medulla_op_nihil(),
        medulla_op_nihil());
}

/* mensurae typorum medullae (octeti) */
interior s32
_octeti_typi_m (
    s32 typus)
{
    commutatio (typus)
    {
        casus MEDULLA_TYPUS_I8:  redde I;
        casus MEDULLA_TYPUS_S8:  redde I;
        casus MEDULLA_TYPUS_I16: redde II;
        casus MEDULLA_TYPUS_S16: redde II;
        casus MEDULLA_TYPUS_I32: redde IV;
        casus MEDULLA_TYPUS_S32: redde IV;
        casus MEDULLA_TYPUS_F32: redde IV;
        ordinarius:              redde VIII;
    }
}

interior b32
_fluitans_m (
    s32 typus)
{
    redde typus == (s32)MEDULLA_TYPUS_F32
        || typus == (s32)MEDULLA_TYPUS_F64;
}

/* conversio inter typos C annotatos - familia operationum
 * nominatarum; idem aut solum-signum = registrum immotum
 * (typi in operationibus, non in registris) */
interior s32
_conversionem (
    Demissio* d,
    constans SilvaNodus* origo,
    s32
    registrum_valoris,
    constans TypusC89* de,
    constans TypusC89* ad)
{
    s32 mt_de = _typus_medullae(de);
    s32 mt_ad = _typus_medullae(ad);
    s32 op;

    si (mt_de < ZEPHYRUM || mt_ad < ZEPHYRUM)
    {
        redde registrum_valoris;   /* vacuum/aggregatum: immotum */
    }
    si (mt_de == mt_ad)
    {
        redde registrum_valoris;
    }
    si (_fluitans_m(mt_de) && _fluitans_m(mt_ad))
    {
        op = (mt_de == (s32)MEDULLA_TYPUS_F32)
            ? (s32)MEDULLA_OP_DILATARE : (s32)MEDULLA_OP_ARTARE;
    }
    alioquin si (_fluitans_m(mt_de))
    {
        op = MEDULLA_OP_INTEGRARE;
    }
    alioquin si (_fluitans_m(mt_ad))
    {
        op = MEDULLA_OP_FLUITARE;
    }
    alioquin
    {
        s32 o_de = _octeti_typi_m(mt_de);
        s32 o_ad = _octeti_typi_m(mt_ad);

        si (o_de == o_ad)
        {
            redde registrum_valoris;   /* signum solum: octeti idem */
        }
        op = (o_ad > o_de) ? (s32)MEDULLA_OP_EXTENDERE
                           : (s32)MEDULLA_OP_AMPUTARE;
    }
    redde _em(d, origo, op, mt_de, mt_ad, _registrum_temporarium(d),
        medulla_op_registrum(registrum_valoris), medulla_op_nihil(),
        medulla_op_nihil());
}

/* membrum tag per titulum -> offset + typus (forma computata) */
interior b32
_membrum_invenire (
             Demissio*  d,
    constans TypusC89*  typus,
          SilvaChorda   titulus,
                  s32*  offset_fructus,
    constans TypusC89** typus_fructus)
{
    constans TypusC89* exutus = _exutus(typus);
                  i32  i;

    si (   exutus == NIHIL
        || (exutus->genus != TYPUS_C89_STRUCTURA
            && exutus->genus != TYPUS_C89_UNIO)
        || !exutus->datum.tag.completa)
    {
        redde FALSUM;
    }
    si (_mensura_typi(d, exutus) < ZEPHYRUM)
    {
        redde FALSUM;   /* forma incomputabilis */
    }
    per (i = ZEPHYRUM; i < (i32)exutus->datum.tag.numerus_membrorum;
         i++)
    {
        constans TypusC89Membrum* membrum =
            &exutus->datum.tag.membra[i];

        si (   membrum->titulus.mensura == titulus.mensura
            && titulus.mensura > ZEPHYRUM
            && memcmp(membrum->titulus.datum, titulus.datum,
                   (memoriae_index)titulus.mensura) == ZEPHYRUM)
        {
            *offset_fructus  = (s32)membrum->offset;
            *typus_fructus   = membrum->typus;
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_est_aggregatum (
    constans TypusC89* typus)
{
    constans TypusC89* exutus = _exutus(typus);

    redde exutus != NIHIL
        && (exutus->genus == TYPUS_C89_STRUCTURA
            || exutus->genus == TYPUS_C89_UNIO);
}


/* ==================================================
 * Sedes symbolorum
 * ================================================== */

interior DemissioSedes*
_sedem_symboli (
             constans Demissio* d,
    constans SemanticaSymbolum* symbolum)
{
    redde (DemissioSedes*)_tabulam_invenire(d->sedes,
        (constans vacuum*)symbolum);
}

interior DemissioSedes*
_sedem_creare (
                      Demissio* d,
    constans SemanticaSymbolum* symbolum,
                           s32  index_registri,
                           b32  est_arca)
{
    DemissioSedes* sedes = (DemissioSedes*)officina_piscina_allocare(
        d->piscina, (memoriae_index)magnitudo(DemissioSedes));

    si (sedes == NIHIL)
    {
        redde NIHIL;
    }
    sedes->symbolum              = symbolum;
    sedes->index_registri        = index_registri;
    sedes->est_arca              = est_arca;
    sedes->index_symboli_moduli  = -I;
    _tabulam_ponere(d, d->sedes, (constans vacuum*)symbolum,
        (vacuum*)sedes);
    redde sedes;
}


/* ==================================================
 * Data statica (M1b C): imagines + relocationes.
 * Relocatio ADDITIVA: locellus = inscriptio symboli + octeti
 * priores (addendum in imagine).
 * ================================================== */

/* mensura completa: acies incompleta (x[] = ...) ab initiatore
 * completur - parca semanticae "initiatores non probantur"
 * (inventum fusoris: hic_manens i8 lit[] = "...") */
interior s32
_mensura_completa (
               Demissio* d,
      constans TypusC89* typus,
    constans SilvaNodus* initiator)
{
    constans TypusC89* exutus   = _exutus(typus);
                  s32  mensura  = _mensura_typi(d, typus);

    si (   mensura > ZEPHYRUM || exutus == NIHIL
        || exutus->genus               != TYPUS_C89_ACIES
        || exutus->datum.acies.numerus >= ZEPHYRUM
        || initiator                   == NIHIL)
    {
        redde mensura;
    }
    initiator = _canonicus(initiator);
    {
        s32 mensura_elementi = _mensura_typi(d,
            exutus->datum.acies.elementum);

        si (mensura_elementi <= ZEPHYRUM || initiator == NIHIL)
        {
            redde -I;
        }
        si (initiator->genus == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA)
        {
            SilvaChorda octeti;

            si (   d->officina_piscina_silvae == NIHIL
                || !silva_c89_chorda_decodere(d->officina_piscina_silvae,
                       initiator, &octeti))
            {
                redde -I;
            }
            redde ((s32)octeti.mensura + I) * mensura_elementi;
        }
        si (initiator->genus == (s32)SILVA_C89_GENUS_CONGERIES)
        {
            SilvaValor elementa = silva_c89_congeries_elementa(
                initiator);
            i32 i;
            i32 m        = (i32)silva_valor_lista_numerus(elementa);
            s32 numerus  = ZEPHYRUM;

            per (i = ZEPHYRUM; i < m; i++)
            {
                SilvaValor* v = silva_valor_lista_obtinere(elementa,
                    (unsigned int)i);

                si (v != NIHIL && v->genus == SILVA_VALOR_NODUS)
                {
                    numerus++;
                }
            }
            si (numerus == ZEPHYRUM)
            {
                redde -I;
            }
            redde numerus * mensura_elementi;
        }
    }
    redde -I;
}

/* ordinatio (acies incompleta: elementi) */
interior s32
_ordinatio_typi (
             Demissio* d,
    constans TypusC89* typus)
{
    constans TypusC89* exutus = _exutus(typus);

    si (exutus == NIHIL)
    {
        redde I;
    }
    si (   exutus->genus == TYPUS_C89_ACIES
        && exutus->datum.acies.numerus < ZEPHYRUM)
    {
        exutus = _exutus(exutus->datum.acies.elementum);
        si (exutus == NIHIL)
        {
            redde I;
        }
    }
    si (_mensura_typi(d, exutus) <= ZEPHYRUM)
    {
        redde I;
    }
    redde (s32)exutus->ordinatio;
}

/* nota sine contextu functionis: causa internata = signum
 * classificatum moduli (fusor eas numerat) */
interior vacuum
_notare (
               Demissio* d,
    constans SilvaNodus* nodus,
     constans character* causa)
{
    si (d->functio != NIHIL)
    {
        _sistere(d, nodus, causa);
    }
    alioquin
    {
        (vacuum)medulla_causam_internare(d->modulus,
            _ch_literis(causa));
    }
}

interior MedullaDatum*
_datum_moduli (
    constans Demissio* d,
                  s32  index_symboli)
{
    constans MedullaSymbolum* symbolum = medulla_symbolum_obtinere(
        d->modulus, index_symboli);
    MedullaDatum** locus_dati;

    si (   symbolum        == NIHIL
        || symbolum->genus != (s32)MEDULLA_SYMBOLUM_DATUM)
    {
        redde NIHIL;
    }
    locus_dati = (MedullaDatum**)officina_xar_obtinere_s(d->modulus->data,
        symbolum->index);
    redde (locus_dati != NIHIL) ? *locus_dati : NIHIL;
}

interior MedullaDatum*
_datum_invenire_aut_creare (
    Demissio* d,
      OfficinaChorda  titulus,
         s32  mensura,
         s32  ordinatio)
{
    s32 index_symboli = medulla_symbolum_internare(d->modulus,
        titulus);
    constans MedullaSymbolum* symbolum;

    si (index_symboli < ZEPHYRUM)
    {
        redde NIHIL;
    }
    symbolum = medulla_symbolum_obtinere(d->modulus, index_symboli);
    si (symbolum->genus == (s32)MEDULLA_SYMBOLUM_DATUM)
    {
        redde _datum_moduli(d, index_symboli);
    }
    si (symbolum->genus != (s32)MEDULLA_SYMBOLUM_EXTERNUM)
    {
        redde NIHIL;   /* functio eodem titulo */
    }
    redde medulla_datum_creare(d->modulus, titulus, (i32)mensura,
        (i32)ordinatio);
}

/* data anonyma per se privata moduli sunt -> praefixum stirpis
 * semper ($stirps.chorda_N; inventum M2a: sine eo chorda_0 in
 * omnibus modulis collidit) */
interior OfficinaChorda
_titulum_dati (
              Demissio* d,
    constans character* basis)
{
    character littera[CXXVIII];
       OfficinaChorda temporarium;
          s32 longitudo = (s32)sprintf(littera, "%.*s.%s_%d",
              (int)((d->stirps.mensura < XL) ? d->stirps.mensura : XL),
              (constans character*)d->stirps.datum, basis,
              (int)d->numerator_datorum);

    d->numerator_datorum++;
    temporarium.datum    = (i8*)littera;
    temporarium.mensura  = (i32)longitudo;
    redde officina_chorda_transcribere(temporarium, d->piscina);
}

/* chorda litteralis -> datum anonymum (octeti decodati + NUL;
 * mensura data si maior). -I in errore. */
interior s32
_datum_chordae_mensurae (
               Demissio* d,
    constans SilvaNodus* nodus,
                    s32  mensura_data)
{
     SilvaChorda  octeti;
          OfficinaChorda  titulus;
    MedullaDatum* datum;
             s32  mensura;
          OfficinaChorda  visus;

    si (   d->officina_piscina_silvae == NIHIL
        || !silva_c89_chorda_decodere(d->officina_piscina_silvae, nodus,
               &octeti))
    {
        redde -I;
    }
    mensura = (mensura_data > (s32)octeti.mensura)
        ? mensura_data : (s32)octeti.mensura + I;   /* + NUL */
    titulus = _titulum_dati(d, "chorda");
    datum = medulla_datum_creare(d->modulus, titulus, (i32)mensura,
        I);
    si (datum == NIHIL)
    {
        redde -I;
    }
    visus.datum    = octeti.datum;
    visus.mensura  = octeti.mensura;
    si (visus.mensura > ZEPHYRUM)
    {
        (vacuum)medulla_datum_scribere(datum, ZEPHYRUM, visus.datum,
            visus.mensura);
    }
    redde medulla_symbolum_internare(d->modulus, titulus);
}

interior s32
_datum_chordae (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    redde _datum_chordae_mensurae(d, nodus, ZEPHYRUM);
}

/* locus staticus: identificator globalis / functio, cum catena
 * accessus/subscriptionis constantis -> symbolum + addendum */
interior b32
_locum_staticum (
               Demissio* d,
    constans SilvaNodus* nodus,
                    s32* symbolum_fructus,
                    s64* addendum_fructus)
{
    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        SilvaValor v = silva_c89_parenthesis_internum(nodus);

        redde v.genus == SILVA_VALOR_NODUS
            && _locum_staticum(d, v.datum.nodus, symbolum_fructus,
                   addendum_fructus);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
    {
        constans SemanticaSymbolum* symbolum =
            silva_c89_symbolum_nodi(d->sem, nodus);

        si (symbolum == NIHIL || symbolum->profunditas > ZEPHYRUM)
        {
            redde FALSUM;   /* localia non statice */
        }
        *symbolum_fructus = medulla_symbolum_internare(d->modulus,
            _titulum_symboli(d, symbolum));
        *addendum_fructus = 0;
        redde *symbolum_fructus >= ZEPHYRUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_ACCESSUS)
    {
         SilvaValor basis_v = silva_c89_accessus_basis(nodus);
        SilvaChorda operator = _tok_textus(
            silva_c89_accessus_tok_operator(nodus));
        SilvaChorda titulus = _tok_textus(
            silva_c89_accessus_tok_titulus(nodus));
                      s32  offset        = ZEPHYRUM;
        constans TypusC89* typus_membri  = NIHIL;

        si (   basis_v.genus != SILVA_VALOR_NODUS
            || !_op_est(operator, "."))
        {
            redde FALSUM;
        }
        si (!_locum_staticum(d, basis_v.datum.nodus,
                symbolum_fructus, addendum_fructus))
        {
            redde FALSUM;
        }
        si (!_membrum_invenire(d, silva_c89_typus_expressionis(
                d->sem, _canonicus(basis_v.datum.nodus)), titulus,
                &offset, &typus_membri))
        {
            redde FALSUM;
        }
        *addendum_fructus += (s64)offset;
        redde VERUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_SUBSCRIPTIO)
    {
        SilvaValor basis_v  = silva_c89_subscriptio_basis(nodus);
        SilvaValor index_v  = silva_c89_subscriptio_index(nodus);
               s64 index    = 0;
               s32 mensura;

        si (   basis_v.genus != SILVA_VALOR_NODUS
            || index_v.genus != SILVA_VALOR_NODUS
            || !silva_c89_constans_aestimare(d->sem,
                   index_v.datum.nodus, &index))
        {
            redde FALSUM;
        }
        si (!_locum_staticum(d, basis_v.datum.nodus,
                symbolum_fructus, addendum_fructus))
        {
            redde FALSUM;
        }
        mensura = _mensura_elementi_monstratoris(d,
            _typus_finalis(d, _canonicus(basis_v.datum.nodus)));
        si (mensura <= ZEPHYRUM)
        {
            redde FALSUM;
        }
        *addendum_fructus += index * (s64)mensura;
        redde VERUM;
    }
    redde FALSUM;
}

/* constans inscriptionis pro imagine statica: &..., functio,
 * acies (lapsus), chorda litteralis */
interior b32
_inscriptio_statica (
               Demissio* d,
    constans SilvaNodus* nodus,
                    s32* symbolum_fructus,
                    s64* addendum_fructus)
{
    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        SilvaValor v = silva_c89_parenthesis_internum(nodus);

        redde v.genus == SILVA_VALOR_NODUS
            && _inscriptio_statica(d, v.datum.nodus,
                   symbolum_fructus, addendum_fructus);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_CONVERSIO)
    {
        SilvaValor v = silva_c89_conversio_internum(nodus);

        redde v.genus == SILVA_VALOR_NODUS
            && _inscriptio_statica(d, v.datum.nodus,
                   symbolum_fructus, addendum_fructus);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA)
    {
        s32 index_symboli = _datum_chordae(d, nodus);

        si (index_symboli < ZEPHYRUM)
        {
            redde FALSUM;
        }
        *symbolum_fructus = index_symboli;
        *addendum_fructus = 0;
        redde VERUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_UNARIUM)
    {
        SilvaChorda operator = _tok_textus(
            silva_c89_unarium_tok_operator(nodus));
        SilvaValor v = silva_c89_unarium_internum(nodus);

        redde _op_est(operator, "&")
            && v.genus == SILVA_VALOR_NODUS
            && _locum_staticum(d, v.datum.nodus, symbolum_fructus,
                   addendum_fructus);
    }
    /* identificator functionis / aciei (lapsus) */
    redde _locum_staticum(d, nodus, symbolum_fructus,
        addendum_fructus);
}

/* imaginem staticam scribere (recursiva); FALSUM = inaestimabilis
 * (vocans notat) */
interior b32
_imaginem_scribere (
               Demissio* d,
           MedullaDatum* datum,
                    s32  offset,
      constans TypusC89* typus,
    constans SilvaNodus* initiator)
{
    constans TypusC89* exutus = _exutus(typus);

    initiator = _canonicus(initiator);
    si (exutus == NIHIL || initiator == NIHIL)
    {
        redde FALSUM;
    }
    si (initiator->genus == (s32)SILVA_C89_GENUS_CONGERIES)
    {
        SilvaValor elementa = silva_c89_congeries_elementa(
            initiator);
        i32 i;
        i32 m     = (i32)silva_valor_lista_numerus(elementa);
        s32 a     = ZEPHYRUM;
        b32 bene  = VERUM;

        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(elementa,
                (unsigned int)i);
            constans TypusC89* finis            = NIHIL;
                          s32  offset_elementi  = offset;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            si (exutus->genus == TYPUS_C89_ACIES)
            {
                s32 mensura = _mensura_typi(d,
                    exutus->datum.acies.elementum);

                si (mensura <= ZEPHYRUM)
                {
                    redde FALSUM;
                }
                finis            = exutus->datum.acies.elementum;
                offset_elementi  = offset + a * mensura;
            }
            alioquin si (exutus->genus == TYPUS_C89_STRUCTURA)
            {
                si (   _mensura_typi(d, exutus) < ZEPHYRUM
                    || a >= (s32)exutus->datum.tag.numerus_membrorum)
                {
                    redde FALSUM;
                }
                finis = exutus->datum.tag.membra[a].typus;
                offset_elementi = offset
                    + (s32)exutus->datum.tag.membra[a].offset;
            }
            alioquin si (exutus->genus == TYPUS_C89_UNIO)
            {
                si (a > ZEPHYRUM)
                {
                    frange;   /* membrum primum solum */
                }
                si (   _mensura_typi(d, exutus) < ZEPHYRUM
                    || exutus->datum.tag.numerus_membrorum
                        == ZEPHYRUM)
                {
                    redde FALSUM;
                }
                finis = exutus->datum.tag.membra[ZEPHYRUM].typus;
            }
            alioquin
            {
                finis = exutus;   /* scalaris uncis vestitus */
            }
            si (!_imaginem_scribere(d, datum, offset_elementi,
                    finis, v->datum.nodus))
            {
                bene = FALSUM;
            }
            a++;
        }
        redde bene;
    }
    /* chorda in aciem characterum */
    si (   initiator->genus == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA
        && exutus->genus    == TYPUS_C89_ACIES)
    {
        SilvaChorda octeti;

        si (   d->officina_piscina_silvae == NIHIL
            || !silva_c89_chorda_decodere(d->officina_piscina_silvae,
                   initiator, &octeti))
        {
            redde FALSUM;
        }
        si (octeti.mensura > ZEPHYRUM)
        {
            redde medulla_datum_scribere(datum, (i32)offset,
                octeti.datum, octeti.mensura);
        }
        redde VERUM;
    }
    /* folium scalare */
    {
        s32 mt       = _typus_medullae(exutus);
        s32 mensura  = _mensura_typi(d, exutus);
         i8 octeti[VIII];
        s64 valor = 0;

        si (   mt < ZEPHYRUM || mensura <= ZEPHYRUM
            || mensura > VIII)
        {
            redde FALSUM;
        }
        si (_fluitans_m(mt))
        {
            f64 fluitans_valor = 0.0;

            si (!_fluitantem_aestimare(d, initiator,
                    &fluitans_valor))
            {
                redde FALSUM;
            }
            si (mt == (s32)MEDULLA_TYPUS_F32)
            {
                unio { f32 v; i8 o[IV]; } u;

                u.v = (f32)fluitans_valor;
                memcpy(octeti, u.o, IV);
            }
            alioquin
            {
                unio { f64 v; i8 o[VIII]; } u;

                u.v = fluitans_valor;
                memcpy(octeti, u.o, VIII);
            }
            redde medulla_datum_scribere(datum, (i32)offset, octeti,
                (i32)mensura);
        }
        si (silva_c89_constans_aestimare(d->sem, initiator, &valor))
        {
            _octetos_integri(valor, octeti, mensura);
            redde medulla_datum_scribere(datum, (i32)offset, octeti,
                (i32)mensura);
        }
        /* constans inscriptionis (relocatio additiva) */
        si (   exutus->genus == TYPUS_C89_MONSTRATOR
            || mensura       == VIII)
        {
            s32 index_symboli  = -I;
            s64 addendum       = 0;

            si (_inscriptio_statica(d, initiator, &index_symboli,
                    &addendum))
            {
                _octetos_integri(addendum, octeti, VIII);
                (vacuum)medulla_datum_scribere(datum, (i32)offset,
                    octeti, VIII);
                redde medulla_relocationem_addere(datum,
                    (i32)offset, index_symboli);
            }
        }
        /* constans nulla monstratoris: NULL = ((void*)0) - aestimator
         * publicus conversiones non-integrales recusat (inventum
         * fusoris, 46 moduli). Involucra exuta, nucleus aestimatur. */
        {
            constans SilvaNodus* nucleus  = initiator;
                            s32  custos   = ZEPHYRUM;

            dum (nucleus != NIHIL && custos < XVI)
            {
                custos++;
                nucleus = _canonicus(nucleus);
                si (nucleus->genus == (s32)SILVA_C89_GENUS_CONVERSIO)
                {
                    SilvaValor v = silva_c89_conversio_internum(
                        nucleus);

                    nucleus = (v.genus == SILVA_VALOR_NODUS)
                        ? v.datum.nodus : NIHIL;
                    perge;
                }
                si (nucleus->genus
                    == (s32)SILVA_C89_GENUS_PARENTHESIS)
                {
                    SilvaValor v = silva_c89_parenthesis_internum(
                        nucleus);

                    nucleus = (v.genus == SILVA_VALOR_NODUS)
                        ? v.datum.nodus : NIHIL;
                    perge;
                }
                frange;
            }
            si (   nucleus != NIHIL
                && silva_c89_constans_aestimare(d->sem, nucleus,
                       &valor))
            {
                _octetos_integri(valor, octeti, mensura);
                redde medulla_datum_scribere(datum, (i32)offset,
                    octeti, (i32)mensura);
            }
        }
        redde FALSUM;
    }
}

/* probatio puritatis: initiator totus statice aestimabilis? (sine
 * effectibus - chordae aestimabiles censentur) */
interior b32
_constans_est (
               Demissio* d,
      constans TypusC89* typus,
    constans SilvaNodus* initiator)
{
    constans TypusC89* exutus = _exutus(typus);

    initiator = _canonicus(initiator);
    si (exutus == NIHIL || initiator == NIHIL)
    {
        redde FALSUM;
    }
    si (initiator->genus == (s32)SILVA_C89_GENUS_CONGERIES)
    {
        SilvaValor elementa = silva_c89_congeries_elementa(
            initiator);
        i32 i;
        i32 m = (i32)silva_valor_lista_numerus(elementa);
        s32 a = ZEPHYRUM;

        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(elementa,
                (unsigned int)i);
            constans TypusC89* finis = NIHIL;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            si (exutus->genus == TYPUS_C89_ACIES)
            {
                finis = exutus->datum.acies.elementum;
            }
            alioquin si (exutus->genus == TYPUS_C89_STRUCTURA)
            {
                si (   _mensura_typi(d, exutus) < ZEPHYRUM
                    || a >= (s32)exutus->datum.tag.numerus_membrorum)
                {
                    redde FALSUM;
                }
                finis = exutus->datum.tag.membra[a].typus;
            }
            alioquin si (exutus->genus == TYPUS_C89_UNIO)
            {
                si (a > ZEPHYRUM)
                {
                    frange;
                }
                si (exutus->datum.tag.numerus_membrorum == ZEPHYRUM)
                {
                    redde FALSUM;
                }
                finis = exutus->datum.tag.membra[ZEPHYRUM].typus;
            }
            alioquin
            {
                finis = exutus;
            }
            si (!_constans_est(d, finis, v->datum.nodus))
            {
                redde FALSUM;
            }
            a++;
        }
        redde VERUM;
    }
    si (initiator->genus == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA)
    {
        redde VERUM;
    }
    {
        s32 mt              = _typus_medullae(exutus);
        s64 valor           = 0;
        f64 fluitans_valor  = 0.0;

        si (mt < ZEPHYRUM)
        {
            redde FALSUM;
        }
        si (_fluitans_m(mt))
        {
            redde _fluitantem_aestimare(d, initiator,
                &fluitans_valor);
        }
        si (silva_c89_constans_aestimare(d->sem, initiator, &valor))
        {
            redde VERUM;
        }
        /* &globalis, functio, chorda - resolutio sine creatione
         * TANTUM per probationem locorum: identificator/&-catena
         * (chordae supra iam VERUM) */
        si (exutus->genus == TYPUS_C89_MONSTRATOR)
        {
            constans SilvaNodus* probandus = initiator;

            si (probandus->genus == (s32)SILVA_C89_GENUS_UNARIUM)
            {
                SilvaValor v = silva_c89_unarium_internum(
                    probandus);

                si (   _op_est(_tok_textus(
                        silva_c89_unarium_tok_operator(probandus)),
                        "&")
                    && v.genus == SILVA_VALOR_NODUS)
                {
                    s32 ignotum_s = -I;
                    s64 ignotum_a = 0;

                    redde _locum_staticum(d, v.datum.nodus,
                        &ignotum_s, &ignotum_a);
                }
                redde FALSUM;
            }
            {
                s32 ignotum_s = -I;
                s64 ignotum_a = 0;

                redde _locum_staticum(d, probandus, &ignotum_s,
                    &ignotum_a);
            }
        }
        redde FALSUM;
    }
}


/* ==================================================
 * Expressiones
 * ================================================== */

/* valor naturalis identificatoris (ante conversionem nodi) */
interior s32
_identificatorem (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    constans SemanticaSymbolum* symbolum = silva_c89_symbolum_nodi(
        d->sem, nodus);
    constans TypusC89* naturalis = silva_c89_typus_expressionis(
        d->sem, nodus);
    DemissioSedes* sedes;

    si (symbolum == NIHIL)
    {
        _sistere(d, nodus, "identificator sine symbolo");
        redde _registrum_temporarium(d);
    }
    si (symbolum->genus == (int)SYMBOLUM_CONSTANS)
    {
        redde _movere(d, nodus, MEDULLA_TYPUS_S32,
            _registrum_temporarium(d),
            medulla_op_immediatum((s64)symbolum->valor));
    }
    sedes = _sedem_symboli(d, symbolum);
    si (sedes != NIHIL && sedes->index_symboli_moduli >= ZEPHYRUM)
    {
        /* staticum locale: datum moduli */
        s32 inscriptio = _em(d, nodus, MEDULLA_OP_LOCUS,
            MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
            _registrum_temporarium(d),
            medulla_op_symbolum(sedes->index_symboli_moduli),
            medulla_op_nihil(), medulla_op_nihil());
        s32 mt = _typus_medullae(naturalis);

        si (   mt                        >= ZEPHYRUM
            && _exutus(naturalis)->genus != TYPUS_C89_ACIES)
        {
            redde _em(d, nodus, MEDULLA_OP_LEGERE, mt,
                MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                medulla_op_registrum(inscriptio),
                medulla_op_nihil(), medulla_op_nihil());
        }
        redde inscriptio;
    }
    si (sedes != NIHIL)
    {
        si (!sedes->est_arca)
        {
            redde sedes->index_registri;
        }
        /* arca: inscriptio; onus solum si typus scalaris */
        {
            s32 mt = _typus_medullae(naturalis);

            si (   mt                        >= ZEPHYRUM
                && _exutus(naturalis)->genus != TYPUS_C89_ACIES)
            {
                redde _em(d, nodus, MEDULLA_OP_LEGERE, mt,
                    MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                    medulla_op_registrum(sedes->index_registri),
                    medulla_op_nihil(), medulla_op_nihil());
            }
            redde sedes->index_registri;   /* inscriptio (acies/
                                            * aggregatum) */
        }
    }
    /* globale aut functio: locus $symbolum */
    si (   symbolum->profunditas > ZEPHYRUM
        && (symbolum->repositio & REPOSITIO_STATICA) != ZEPHYRUM)
    {
        _sistere(d, nodus, "staticum ante declarationem");
        redde _registrum_temporarium(d);
    }
    {
        s32 index_symboli = medulla_symbolum_internare(d->modulus,
            _titulum_symboli(d, symbolum));
        s32 inscriptio = _em(d, nodus, MEDULLA_OP_LOCUS,
            MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
            _registrum_temporarium(d),
            medulla_op_symbolum(index_symboli), medulla_op_nihil(),
            medulla_op_nihil());
        constans TypusC89* exutus  = _exutus(naturalis);
                      s32  mt       =
                          _typus_medullae(naturalis);

        si (   mt            >= ZEPHYRUM && exutus != NIHIL
            && exutus->genus != TYPUS_C89_ACIES
            && exutus->genus != TYPUS_C89_FUNCTIO)
        {
            redde _em(d, nodus, MEDULLA_OP_LEGERE, mt,
                MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                medulla_op_registrum(inscriptio),
                medulla_op_nihil(), medulla_op_nihil());
        }
        redde inscriptio;
    }
}

/* index (int) -> inscriptio scalata (i64): extendere + multiplicare */
interior s32
_indicem_scalare (
               Demissio* d,
    constans SilvaNodus* origo,
       MedullaOperandum  index_op,
                    s32  mt_indicis,
                    s32  mensura_elementi)
{
    s32 registrum_indicis;
    s32 latus;

    si (index_op.genus == (s32)MEDULLA_OPERANDUM_IMMEDIATUM)
    {
        /* scalatio in tempore compilationis */
        redde _movere(d, origo, MEDULLA_TYPUS_I64,
            _registrum_temporarium(d),
            medulla_op_immediatum(index_op.datum.immediatum
                * (s64)mensura_elementi));
    }
    registrum_indicis = (index_op.genus
        == (s32)MEDULLA_OPERANDUM_REGISTRUM)
        ? index_op.datum.index
        : _movere(d, origo, mt_indicis, _registrum_temporarium(d),
              index_op);
    latus = registrum_indicis;
    si (_octeti_typi_m(mt_indicis) < VIII)
    {
        latus = _em(d, origo, MEDULLA_OP_EXTENDERE, mt_indicis,
            (mt_indicis == (s32)MEDULLA_TYPUS_I32)
                ? (s32)MEDULLA_TYPUS_I64 : (s32)MEDULLA_TYPUS_S64,
            _registrum_temporarium(d),
            medulla_op_registrum(registrum_indicis),
            medulla_op_nihil(), medulla_op_nihil());
    }
    redde _em(d, origo, MEDULLA_OP_MULTIPLICARE, MEDULLA_TYPUS_I64,
        MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
        medulla_op_registrum(latus),
        medulla_op_immediatum((s64)mensura_elementi),
        medulla_op_nihil());
}

interior b32
_est_monstratorius (
    constans TypusC89* typus)
{
    constans TypusC89* exutus = _exutus(typus);

    redde exutus != NIHIL
        && (exutus->genus == TYPUS_C89_MONSTRATOR
            || exutus->genus == TYPUS_C89_ACIES);
}

interior s32
_mensura_elementi_monstratoris (
             Demissio* d,
    constans TypusC89* typus)
{
    constans TypusC89* exutus     = _exutus(typus);
    constans TypusC89* elementum  = NIHIL;

    si (exutus == NIHIL)
    {
        redde -I;
    }
    si (exutus->genus == TYPUS_C89_MONSTRATOR)
    {
        elementum = exutus->datum.monstrator.internum;
    }
    alioquin si (exutus->genus == TYPUS_C89_ACIES)
    {
        elementum = exutus->datum.acies.elementum;
    }
    redde _mensura_typi(d, elementum);
}

/* && / || : materializatio ingenua per bloccos (0/1) */
interior s32
_brevem (
               Demissio* d,
    constans SilvaNodus* nodus,
                    b32  est_et)
{
    SilvaValor sin_v = silva_c89_binarium_sinister(nodus);
    SilvaValor dex_v = silva_c89_binarium_dexter(nodus);
    s32 fructus = _registrum_temporarium(d);
    s32 b_alter = _bloccum_novum(d, est_et ? "et_alter" : "vel_alter");
    s32 b_muta = _bloccum_novum(d,
        est_et ? "et_verum" : "vel_zephyrum");
    s32 b_finis = _bloccum_novum(d, est_et ? "et_finis" : "vel_finis");
    s32 primus;

    (vacuum)_movere(d, nodus, MEDULLA_TYPUS_S32, fructus,
        medulla_op_immediatum(est_et ? 0 : 1));
    primus = _ut_valor(d, sin_v.datum.nodus);
    si (est_et)
    {
        _ramus(d, nodus, medulla_op_registrum(primus), b_alter,
            b_finis);
    }
    alioquin
    {
        _ramus(d, nodus, medulla_op_registrum(primus), b_finis,
            b_alter);
    }
    d->bloccus = b_alter;
    {
        s32 secundus = _ut_valor(d, dex_v.datum.nodus);

        si (est_et)
        {
            _ramus(d, nodus, medulla_op_registrum(secundus), b_muta,
                b_finis);
        }
        alioquin
        {
            _ramus(d, nodus, medulla_op_registrum(secundus), b_finis,
                b_muta);
        }
    }
    d->bloccus = b_muta;
    (vacuum)_movere(d, nodus, MEDULLA_TYPUS_S32, fructus,
        medulla_op_immediatum(est_et ? 1 : 0));
    _salire_si_vivus(d, b_finis);
    d->bloccus = b_finis;
    redde fructus;
}

interior s32
_binarium (
               Demissio* d,
    constans SilvaNodus* nodus)
{
     SilvaValor sin_v = silva_c89_binarium_sinister(nodus);
     SilvaValor dex_v = silva_c89_binarium_dexter(nodus);
    SilvaChorda operator = _tok_textus(
        silva_c89_binarium_tok_operator(nodus));
    constans TypusC89* fructus_t = silva_c89_typus_expressionis(
        d->sem, nodus);
    s32 op          = -I;
    b32 comparatio  = FALSUM;

    si (   sin_v.genus != SILVA_VALOR_NODUS
        || dex_v.genus != SILVA_VALOR_NODUS)
    {
        _sistere(d, nodus, "binarium mancum");
        redde _registrum_temporarium(d);
    }
    si (_op_est(operator, "&&"))
    {
        redde _brevem(d, nodus, VERUM);
    }
    si (_op_est(operator, "||"))
    {
        redde _brevem(d, nodus, FALSUM);
    }
    si (_op_est(operator, "+"))
    { op = MEDULLA_OP_ADDERE;
    }
    alioquin si (_op_est(operator, "-"))
    { op = MEDULLA_OP_SUBTRAHERE;
    }
    alioquin si (_op_est(operator, "*"))
    { op = MEDULLA_OP_MULTIPLICARE;
    }
    alioquin si (_op_est(operator, "/"))
    { op = MEDULLA_OP_DIVIDERE;
    }
    alioquin si (_op_est(operator, "%"))
    { op = MEDULLA_OP_RESIDUUM;
    }
    alioquin si (_op_est(operator, "&"))
    { op = MEDULLA_OP_ET;
    }
    alioquin si (_op_est(operator, "|"))
    { op = MEDULLA_OP_VEL;
    }
    alioquin si (_op_est(operator, "^"))
    { op = MEDULLA_OP_AUT;
    }
    alioquin si (_op_est(operator, "<<"))
    { op = MEDULLA_OP_SINISTRORSUM;
    }
    alioquin si (_op_est(operator, ">>"))
    { op = MEDULLA_OP_DEXTRORSUM;
    }
    alioquin si (_op_est(operator, "=="))
    { op = MEDULLA_OP_AEQUALIS; comparatio = VERUM;
    }
    alioquin si (_op_est(operator, "!="))
    { op = MEDULLA_OP_INAEQUALIS; comparatio = VERUM;
    }
    alioquin si (_op_est(operator, "<"))
    { op = MEDULLA_OP_MINOR; comparatio = VERUM;
    }
    alioquin si (_op_est(operator, "<="))
    { op = MEDULLA_OP_MINOR_AEQUALIS; comparatio = VERUM;
    }
    alioquin si (_op_est(operator, ">"))
    { op = MEDULLA_OP_MAIOR; comparatio = VERUM;
    }
    alioquin si (_op_est(operator, ">="))
    { op = MEDULLA_OP_MAIOR_AEQUALIS; comparatio = VERUM;
    }
    alioquin
    {
        _sistere(d, nodus, "operator binarii ignotus");
        redde _registrum_temporarium(d);
    }

    /* arithmetica monstratorum: scalatio per formam */
    si (   (op == (s32)MEDULLA_OP_ADDERE
         || op == (s32)MEDULLA_OP_SUBTRAHERE) && !comparatio)
    {
        constans TypusC89* sin_ft = _typus_finalis(d,
            _canonicus(sin_v.datum.nodus));
        constans TypusC89* dex_ft = _typus_finalis(d,
            _canonicus(dex_v.datum.nodus));
        b32 sin_m = _est_monstratorius(sin_ft);
        b32 dex_m = _est_monstratorius(dex_ft);

        si (sin_m && dex_m && op == (s32)MEDULLA_OP_SUBTRAHERE)
        {
            /* monstrator - monstrator -> differentia elementorum */
            s32 mensura  = _mensura_elementi_monstratoris(d, sin_ft);
            s32 a        = _ut_valor(d, sin_v.datum.nodus);
            s32 b        = _ut_valor(d, dex_v.datum.nodus);
            s32 crudum;

            si (mensura <= ZEPHYRUM)
            {
                _sistere(d, nodus, "forma elementi ignota");
                redde _registrum_temporarium(d);
            }
            crudum = _em(d, nodus, MEDULLA_OP_SUBTRAHERE,
                MEDULLA_TYPUS_I64, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d), medulla_op_registrum(a),
                medulla_op_registrum(b), medulla_op_nihil());
            redde _em(d, nodus, MEDULLA_OP_DIVIDERE,
                MEDULLA_TYPUS_S64, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d),
                medulla_op_registrum(crudum),
                medulla_op_immediatum((s64)mensura),
                medulla_op_nihil());
        }
        si (sin_m != dex_m)
        {
            /* monstrator ± integer (aut integer + monstrator) */
            constans SilvaNodus* n_monstratoris = sin_m
                ? sin_v.datum.nodus : dex_v.datum.nodus;
            constans SilvaNodus* n_indicis = sin_m
                ? dex_v.datum.nodus : sin_v.datum.nodus;
            constans TypusC89* t_monstratoris = sin_m ? sin_ft
                                                      : dex_ft;
            s32 mensura = _mensura_elementi_monstratoris(d,
                t_monstratoris);
                         s32 basis;
            MedullaOperandum index_op;
                         s32 scalatum;

            si (mensura <= ZEPHYRUM)
            {
                _sistere(d, nodus, "forma elementi ignota");
                redde _registrum_temporarium(d);
            }
            basis     = _ut_valor(d, n_monstratoris);
            index_op  = _ut_operandum(d, n_indicis);
            scalatum = _indicem_scalare(d, nodus, index_op,
                _typus_medullae(_typus_finalis(d,
                    _canonicus(n_indicis))), mensura);
            redde _em(d, nodus, op, MEDULLA_TYPUS_I64,
                MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                medulla_op_registrum(basis),
                medulla_op_registrum(scalatum), medulla_op_nihil());
        }
    }

    {
        MedullaOperandum a = _ut_operandum(d, sin_v.datum.nodus);
        MedullaOperandum b = _ut_operandum(d, dex_v.datum.nodus);
                     s32 suffixus;

        si (comparatio)
        {
            suffixus = _typus_medullae(_typus_finalis(d,
                _canonicus(sin_v.datum.nodus)));
        }
        alioquin
        {
            suffixus = _typus_medullae(fructus_t);
        }
        si (suffixus < ZEPHYRUM)
        {
            _sistere(d, nodus, "typus binarii non scalaris");
            redde _registrum_temporarium(d);
        }
        redde _em(d, nodus, op, suffixus, MEDULLA_TYPUS_NIHIL,
            _registrum_temporarium(d), a, b, medulla_op_nihil());
    }
}

interior s32
_incrementum (
               Demissio* d,
    constans SilvaNodus* nodus,
    constans SilvaNodus* internum,
                    b32  est_additio,
                    b32  est_praefixum)
{
        DemissioLocus  locus = _ut_locum(d, internum);
    constans TypusC89* naturalis = silva_c89_typus_expressionis(
        d->sem, _canonicus(internum));
    s32 mt      = _typus_medullae(naturalis);
    s64 passus  = I;
    s32 currens;
    s32 novus;

    si (!locus.valida || mt < ZEPHYRUM)
    {
        _sistere(d, nodus, "incrementum sine loco");
        redde _registrum_temporarium(d);
    }
    si (_est_monstratorius(naturalis))
    {
        s32 mensura = _mensura_elementi_monstratoris(d, naturalis);

        si (mensura <= ZEPHYRUM)
        {
            _sistere(d, nodus, "forma elementi ignota");
            redde _registrum_temporarium(d);
        }
        passus = (s64)mensura;
    }
    si (locus.directum)
    {
        currens = locus.index;
    }
    alioquin
    {
        currens = _em(d, nodus, MEDULLA_OP_LEGERE, mt,
            MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
            medulla_op_registrum(locus.index), medulla_op_nihil(),
            medulla_op_nihil());
    }
    /* valor vetus servandus pro postfixo */
    si (!est_praefixum)
    {
        s32 vetus = _movere(d, nodus, mt, _registrum_temporarium(d),
            medulla_op_registrum(currens));

        novus = _em(d, nodus, est_additio ? (s32)MEDULLA_OP_ADDERE
                                          : (s32)MEDULLA_OP_SUBTRAHERE,
            mt, MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
            medulla_op_registrum(currens),
            medulla_op_immediatum(passus), medulla_op_nihil());
        si (locus.directum)
        {
            (vacuum)_movere(d, nodus, mt, locus.index,
                medulla_op_registrum(novus));
        }
        alioquin
        {
            (vacuum)_em(d, nodus, MEDULLA_OP_SCRIBERE, mt,
                MEDULLA_TYPUS_NIHIL, -I,
                medulla_op_registrum(locus.index),
                medulla_op_registrum(novus), medulla_op_nihil());
        }
        redde vetus;
    }
    novus = _em(d, nodus, est_additio ? (s32)MEDULLA_OP_ADDERE
                                      : (s32)MEDULLA_OP_SUBTRAHERE,
        mt, MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
        medulla_op_registrum(currens), medulla_op_immediatum(passus),
        medulla_op_nihil());
    si (locus.directum)
    {
        (vacuum)_movere(d, nodus, mt, locus.index,
            medulla_op_registrum(novus));
    }
    alioquin
    {
        (vacuum)_em(d, nodus, MEDULLA_OP_SCRIBERE, mt,
            MEDULLA_TYPUS_NIHIL, -I,
            medulla_op_registrum(locus.index),
            medulla_op_registrum(novus), medulla_op_nihil());
    }
    redde novus;
}

interior s32
_unarium (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    SilvaChorda operator = _tok_textus(
        silva_c89_unarium_tok_operator(nodus));
             SilvaValor  internum_v = silva_c89_unarium_internum(nodus);
    constans SilvaNodus* internum;

    si (internum_v.genus != SILVA_VALOR_NODUS)
    {
        _sistere(d, nodus, "unarium mancum");
        redde _registrum_temporarium(d);
    }
    internum = internum_v.datum.nodus;

    si (_op_est(operator, "-"))
    {
        s32 mt = _typus_medullae(silva_c89_typus_expressionis(
            d->sem, nodus));
        MedullaOperandum a = _ut_operandum(d, internum);

        redde _em(d, nodus, MEDULLA_OP_NEGARE,
            (mt >= ZEPHYRUM) ? mt : MEDULLA_TYPUS_S32,
            MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d), a,
            medulla_op_nihil(), medulla_op_nihil());
    }
    si (_op_est(operator, "+"))
    {
        redde _ut_valor(d, internum);
    }
    si (_op_est(operator, "~"))
    {
        s32 mt = _typus_medullae(silva_c89_typus_expressionis(
            d->sem, nodus));
        MedullaOperandum a = _ut_operandum(d, internum);

        redde _em(d, nodus, MEDULLA_OP_COMPLEMENTUM,
            (mt >= ZEPHYRUM) ? mt : MEDULLA_TYPUS_S32,
            MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d), a,
            medulla_op_nihil(), medulla_op_nihil());
    }
    si (_op_est(operator, "!"))
    {
        s32 mt = _typus_medullae(_typus_finalis(d,
            _canonicus(internum)));
        MedullaOperandum a = _ut_operandum(d, internum);

        redde _em(d, nodus, MEDULLA_OP_AEQUALIS,
            (mt >= ZEPHYRUM) ? mt : MEDULLA_TYPUS_S32,
            MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d), a,
            medulla_op_immediatum(0), medulla_op_nihil());
    }
    si (_op_est(operator, "&"))
    {
        DemissioLocus locus = _ut_locum(d, internum);

        si (!locus.valida || locus.directum)
        {
            _sistere(d, nodus, "inscriptio sine loco");
            redde _registrum_temporarium(d);
        }
        redde locus.index;
    }
    si (_op_est(operator, "*"))
    {
                      s32  inscriptio = _ut_valor(d, internum);
        constans TypusC89* naturalis = silva_c89_typus_expressionis(
            d->sem, nodus);
                      s32 mt       =
                          _typus_medullae(naturalis);
        constans TypusC89* exutus  = _exutus(naturalis);

        si (   mt            >= ZEPHYRUM && exutus != NIHIL
            && exutus->genus != TYPUS_C89_ACIES
            && exutus->genus != TYPUS_C89_FUNCTIO)
        {
            redde _em(d, nodus, MEDULLA_OP_LEGERE, mt,
                MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                medulla_op_registrum(inscriptio),
                medulla_op_nihil(), medulla_op_nihil());
        }
        redde inscriptio;   /* aggregatum/acies/functio: inscriptio */
    }
    si (_op_est(operator, "++"))
    {
        redde _incrementum(d, nodus, internum, VERUM, VERUM);
    }
    si (_op_est(operator, "--"))
    {
        redde _incrementum(d, nodus, internum, FALSUM, VERUM);
    }
    _sistere(d, nodus, "operator unarii ignotus");
    redde _registrum_temporarium(d);
}

interior s32
_assignationem (
               Demissio* d,
    constans SilvaNodus* nodus)
{
     SilvaValor sin_v = silva_c89_assignatio_sinister(nodus);
     SilvaValor dex_v = silva_c89_assignatio_dexter(nodus);
    SilvaChorda operator = _tok_textus(
        silva_c89_assignatio_tok_operator(nodus));
          DemissioLocus  locus;
    constans SilvaNodus* sinister;
      constans TypusC89* sin_naturalis;
                    s32  mt;
                    s32  fons;

    si (   sin_v.genus != SILVA_VALOR_NODUS
        || dex_v.genus != SILVA_VALOR_NODUS)
    {
        _sistere(d, nodus, "assignatio manca");
        redde _registrum_temporarium(d);
    }
    sinister       = _canonicus(sin_v.datum.nodus);
    sin_naturalis  = silva_c89_typus_expressionis(d->sem, sinister);
    mt             = _typus_medullae(sin_naturalis);
    locus          = _ut_locum(d, sin_v.datum.nodus);
    si (   locus.valida && !locus.directum && mt < ZEPHYRUM
        && _est_aggregatum(sin_naturalis) && _op_est(operator, "="))
    {
        /* assignatio aggregati -> copia (valor = inscriptio) */
        s32 mensura = _mensura_typi(d, sin_naturalis);
        s32 fons;

        si (mensura <= ZEPHYRUM)
        {
            _sistere(d, nodus, "forma aggregati ignota");
            redde _registrum_temporarium(d);
        }
        fons = _ut_valor(d, dex_v.datum.nodus);
        (vacuum)_em(d, nodus, MEDULLA_OP_COPIA, MEDULLA_TYPUS_NIHIL,
            MEDULLA_TYPUS_NIHIL, -I,
            medulla_op_registrum(locus.index),
            medulla_op_registrum(fons),
            medulla_op_immediatum((s64)mensura));
        redde locus.index;
    }
    si (!locus.valida || mt < ZEPHYRUM)
    {
        _sistere(d, nodus, "assignatio sine loco scalari");
        (vacuum)_ut_operandum(d, dex_v.datum.nodus);
        redde _registrum_temporarium(d);
    }

    si (_op_est(operator, "="))
    {
        fons = _ut_valor(d, dex_v.datum.nodus);
    }
    alioquin
    {
        /* compositum: conversiones implicatae iam annotatae in
         * utroque operando (M0b) */
                      s32  op = -I;
                      s32  currens;
                      s32  commune_reg;
        constans TypusC89* commune_t = _typus_finalis(d, sinister);
                      s32  mt_commune =
                          _typus_medullae(commune_t);

        si (_op_est(operator, "+="))
        { op = MEDULLA_OP_ADDERE;
        }
        alioquin si (_op_est(operator, "-="))
        { op = MEDULLA_OP_SUBTRAHERE;
        }
        alioquin si (_op_est(operator, "*="))
        { op = MEDULLA_OP_MULTIPLICARE;
        }
        alioquin si (_op_est(operator, "/="))
        { op = MEDULLA_OP_DIVIDERE;
        }
        alioquin si (_op_est(operator, "%="))
        { op = MEDULLA_OP_RESIDUUM;
        }
        alioquin si (_op_est(operator, "&="))
        { op = MEDULLA_OP_ET;
        }
        alioquin si (_op_est(operator, "|="))
        { op = MEDULLA_OP_VEL;
        }
        alioquin si (_op_est(operator, "^="))
        { op = MEDULLA_OP_AUT;
        }
        alioquin si (_op_est(operator, "<<="))
        { op = MEDULLA_OP_SINISTRORSUM;
        }
        alioquin si (_op_est(operator, ">>="))
        { op = MEDULLA_OP_DEXTRORSUM;
        }
        alioquin
        {
            _sistere(d, nodus, "operator assignationis ignotus");
            redde _registrum_temporarium(d);
        }
        si (locus.directum)
        {
            currens = locus.index;
        }
        alioquin
        {
            currens = _em(d, nodus, MEDULLA_OP_LEGERE, mt,
                MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                medulla_op_registrum(locus.index),
                medulla_op_nihil(), medulla_op_nihil());
        }
        commune_reg = _conversionem(d, nodus, currens,
            sin_naturalis, commune_t);
        si (   _est_monstratorius(sin_naturalis)
            && (op == (s32)MEDULLA_OP_ADDERE
                || op == (s32)MEDULLA_OP_SUBTRAHERE))
        {
            /* p += n : scalatio */
            s32 mensura = _mensura_elementi_monstratoris(d,
                sin_naturalis);
            MedullaOperandum index_op = _ut_operandum(d,
                dex_v.datum.nodus);
            s32 scalatum;

            si (mensura <= ZEPHYRUM)
            {
                _sistere(d, nodus, "forma elementi ignota");
                redde _registrum_temporarium(d);
            }
            scalatum = _indicem_scalare(d, nodus, index_op,
                _typus_medullae(_typus_finalis(d,
                    _canonicus(dex_v.datum.nodus))), mensura);
            fons = _em(d, nodus, op, MEDULLA_TYPUS_I64,
                MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                medulla_op_registrum(commune_reg),
                medulla_op_registrum(scalatum), medulla_op_nihil());
        }
        alioquin
        {
            MedullaOperandum b = _ut_operandum(d, dex_v.datum.nodus);
                         s32 crudus = _em(d, nodus, op,
                             (mt_commune >= ZEPHYRUM) ? mt_commune : mt,
                             MEDULLA_TYPUS_NIHIL,
                             _registrum_temporarium(d),
                             medulla_op_registrum(commune_reg), b,
                             medulla_op_nihil());

            fons = _conversionem(d, nodus, crudus, commune_t,
                sin_naturalis);
        }
    }

    si (locus.directum)
    {
        (vacuum)_movere(d, nodus, mt, locus.index,
            medulla_op_registrum(fons));
    }
    alioquin
    {
        (vacuum)_em(d, nodus, MEDULLA_OP_SCRIBERE, mt,
            MEDULLA_TYPUS_NIHIL, -I,
            medulla_op_registrum(locus.index),
            medulla_op_registrum(fons), medulla_op_nihil());
    }
    redde fons;
}

interior s32
_ternarium (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    SilvaValor cond_v    = silva_c89_ternarius_conditio(nodus);
    SilvaValor verum_v   = silva_c89_ternarius_verum(nodus);
    SilvaValor falsum_v  = silva_c89_ternarius_falsum(nodus);
           s32 mt = _typus_medullae(silva_c89_typus_expressionis(d->sem,
               nodus));
    s32 fructus   = _registrum_temporarium(d);
    s32 b_verum   = _bloccum_novum(d, "tern_verum");
    s32 b_falsum  = _bloccum_novum(d, "tern_falsum");
    s32 b_finis   = _bloccum_novum(d, "tern_finis");
    s32 conditio;

    si (mt < ZEPHYRUM)
    {
        mt = MEDULLA_TYPUS_S64;   /* monstratores compositi etc. */
    }
    conditio = _ut_valor(d, cond_v.datum.nodus);
    _ramus(d, nodus, medulla_op_registrum(conditio), b_verum,
        b_falsum);
    d->bloccus = b_verum;
    (vacuum)_movere(d, nodus, mt, fructus,
        _ut_operandum(d, verum_v.datum.nodus));
    _salire_si_vivus(d, b_finis);
    d->bloccus = b_falsum;
    (vacuum)_movere(d, nodus, mt, fructus,
        _ut_operandum(d, falsum_v.datum.nodus));
    _salire_si_vivus(d, b_finis);
    d->bloccus = b_finis;
    redde fructus;
}

interior s32
_vocationem (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    SilvaValor functio_v    = silva_c89_vocatio_functio(nodus);
    SilvaValor argumenta_v  = silva_c89_vocatio_argumenta(nodus);
    constans SilvaNodus* nf;
    constans TypusC89* typus_vocati;
    constans TypusC89* reditus;
    MedullaOperandum vocatus;
    MedullaOperandum argumenta[LXIV];
    s32 numerus_argumentorum  = ZEPHYRUM;
    b32 reditus_aggregatus    = FALSUM;
    s32 arca_reditus          = -I;
    s32 mt_reditus;

    si (functio_v.genus != SILVA_VALOR_NODUS)
    {
        _sistere(d, nodus, "vocatio sine vocato");
        redde _registrum_temporarium(d);
    }
    nf = _canonicus(functio_v.datum.nodus);

    /* typus functionis (monstrator ad functionem exuitur) */
    typus_vocati = _exutus(_typus_finalis(d, nf));
    si (   typus_vocati        != NIHIL
        && typus_vocati->genus == TYPUS_C89_MONSTRATOR)
    {
        typus_vocati = _exutus(
            typus_vocati->datum.monstrator.internum);
    }
    si (   typus_vocati        == NIHIL
        || typus_vocati->genus != TYPUS_C89_FUNCTIO)
    {
        _sistere(d, nodus, "vocatio sine typo functionis");
        redde _registrum_temporarium(d);
    }
    reditus             = _exutus(typus_vocati->datum.functio.reditus);
    mt_reditus          = _typus_medullae(reditus);
    reditus_aggregatus  = _est_aggregatum(reditus);

    /* vocatus: $symbolum directum si identificator functionis;
     * aliter valor (monstrator functionis) */
    vocatus = medulla_op_nihil();
    si (nf->genus == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
    {
        constans SemanticaSymbolum* symbolum =
            silva_c89_symbolum_nodi(d->sem, nf);
        constans TypusC89* typus_symboli = (symbolum != NIHIL)
            ? _exutus(symbolum->typus) : NIHIL;

        si (   typus_symboli        != NIHIL
            && typus_symboli->genus == TYPUS_C89_FUNCTIO)
        {
            vocatus = medulla_op_symbolum(
                medulla_symbolum_internare(d->modulus,
                    _titulum_symboli(d, symbolum)));
        }
    }
    si (vocatus.genus == (s32)MEDULLA_OPERANDUM_NIHIL)
    {
        vocatus = medulla_op_registrum(_ut_valor(d,
            functio_v.datum.nodus));
    }

    /* reditus aggregatus: arca destinationis = argumentum primum
     * EXPLICITUM (conventio C7 - demissio semantica visibilia
     * facit) */
    si (reditus_aggregatus)
    {
        s32 mensura = _mensura_typi(d, reditus);

        si (mensura <= ZEPHYRUM)
        {
            _sistere(d, nodus, "forma reditus ignota");
            redde _registrum_temporarium(d);
        }
        arca_reditus = _em(d, nodus, MEDULLA_OP_ARCA,
            MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
            _registrum_temporarium(d),
            medulla_op_immediatum((s64)mensura),
            medulla_op_immediatum(
                (s64)_exutus(reditus)->ordinatio),
            medulla_op_nihil());
        argumenta[numerus_argumentorum] =
            medulla_op_registrum(arca_reditus);
        numerus_argumentorum++;
    }

    /* argumenta (conversiones/promotiones variadicae iam
     * annotatae; virgulae intersertae praetermissae) */
    {
        i32 i;
        i32 m = (i32)silva_valor_lista_numerus(argumenta_v);

        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(argumenta_v,
                (unsigned int)i);
            constans SilvaNodus* argumentum;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            si (numerus_argumentorum >= (s32)LXIV)
            {
                _sistere(d, nodus, "argumenta nimia");
                frange;
            }
            argumentum = v->datum.nodus;
            si (_est_aggregatum(_typus_finalis(d,
                    _canonicus(argumentum))))
            {
                /* valore: copia in arcam temporariam vocantis */
                constans TypusC89* typus_argumenti = _exutus(
                    _typus_finalis(d, _canonicus(argumentum)));
                s32 mensura = _mensura_typi(d, typus_argumenti);
                s32 fons;
                s32 temporaria;

                si (mensura <= ZEPHYRUM)
                {
                    _sistere(d, nodus, "forma argumenti ignota");
                    perge;
                }
                fons = _ut_valor(d, argumentum);
                temporaria = _em(d, nodus, MEDULLA_OP_ARCA,
                    MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                    _registrum_temporarium(d),
                    medulla_op_immediatum((s64)mensura),
                    medulla_op_immediatum(
                        (s64)typus_argumenti->ordinatio),
                    medulla_op_nihil());
                (vacuum)_em(d, nodus, MEDULLA_OP_COPIA,
                    MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I,
                    medulla_op_registrum(temporaria),
                    medulla_op_registrum(fons),
                    medulla_op_immediatum((s64)mensura));
                argumenta[numerus_argumentorum] =
                    medulla_op_registrum(temporaria);
            }
            alioquin
            {
                argumenta[numerus_argumentorum] = _ut_operandum(d,
                    argumentum);
            }
            numerus_argumentorum++;
        }
    }

    /* emissio (stiva UNO ictu appensa - vocationes nidificatae in
     * argumentis stivam propriam iam appenderunt) */
    {
        MedullaInstructio instructio;
                      s32 destinatio  = -I;
                      s32 typus       = MEDULLA_TYPUS_NIHIL;

        si (!reditus_aggregatus && mt_reditus >= ZEPHYRUM)
        {
            destinatio  = _registrum_temporarium(d);
            typus       = mt_reditus;
        }
        _vivum(d);
        memset(&instructio, ZEPHYRUM, magnitudo(MedullaInstructio));
        instructio.op              = MEDULLA_OP_VOCARE;
        instructio.typus           = typus;
        instructio.typus_secundus  = MEDULLA_TYPUS_NIHIL;
        instructio.destinatio      = destinatio;
        instructio.a               = vocatus;
        instructio.b               = medulla_op_nihil();
        instructio.c               = medulla_op_nihil();
        instructio.extra_index = medulla_operanda_addere(d->functio,
            argumenta, numerus_argumentorum);
        instructio.extra_numerus  = numerus_argumentorum;
        instructio.origo          = nodus;
        (vacuum)medulla_emittere(d->functio, d->bloccus,
            &instructio);
        si (reditus_aggregatus)
        {
            redde arca_reditus;
        }
        si (destinatio >= ZEPHYRUM)
        {
            redde destinatio;
        }
        redde _registrum_temporarium(d);   /* vacuum */
    }
}

/* dispensator: valor naturalis + conversio annotata applicata */
interior s32
_expressionem (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    constans TypusC89* naturalis;
    constans TypusC89* conversus;
                  s32  fructus;

    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde _registrum_temporarium(d);
    }
    naturalis = silva_c89_typus_expressionis(d->sem, nodus);
    conversus = silva_c89_conversio_expressionis(d->sem, nodus);

    commutatio (nodus->genus)
    {
        casus (s32)SILVA_C89_GENUS_PARENTHESIS:
        {
            SilvaValor v = silva_c89_parenthesis_internum(nodus);

            /* NON redde directum: conversio annotata HUIUS nodi
             * (cauda functionis) applicanda - (s64)(a*b) casus
             * pecuniae sputnik (inventum M2d) */
            fructus = _expressionem(d, v.datum.nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR:
        {
            fructus = _identificatorem(d, nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_BINARIUM:
        {
            fructus = _binarium(d, nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_UNARIUM:
        {
            fructus = _unarium(d, nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_POSTCREMENTUM:
        {
            SilvaValor internum_v = silva_c89_postcrementum_internum(
                nodus);
            SilvaChorda operator = _tok_textus(
                silva_c89_postcrementum_tok_operator(nodus));

            fructus = _incrementum(d, nodus, internum_v.datum.nodus,
                _op_est(operator, "++"), FALSUM);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_ASSIGNATIO:
        {
            fructus = _assignationem(d, nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_TERNARIUS:
        {
            fructus = _ternarium(d, nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_VIRGULA:
        {
            SilvaValor sin_v = silva_c89_virgula_sinister(nodus);
            SilvaValor dex_v = silva_c89_virgula_dexter(nodus);

            (vacuum)_ut_operandum(d, sin_v.datum.nodus);
            fructus = _ut_valor(d, dex_v.datum.nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_CONVERSIO:
        {
            SilvaValor internum_v = silva_c89_conversio_internum(
                nodus);
            constans SilvaNodus* internum = _canonicus(
                internum_v.datum.nodus);
            s32 valor = _ut_valor(d, internum);

            fructus = _conversionem(d, nodus, valor,
                _typus_finalis(d, internum), naturalis);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS:
        {
            /* NON per aestimatorem: scopi post analysim clausi -
             * expressiones locales ibi non solvuntur (inventum
             * fusoris). Typus internae in typationibus UNdecayed
             * (constructione naturalis) -> mensura per formam. */
            SilvaValor internum_v =
                silva_c89_magnitudo_expressionis_internum(nodus);
            constans TypusC89* typus_interni =
                (internum_v.genus == SILVA_VALOR_NODUS)
                ? silva_c89_typus_expressionis(d->sem,
                      _canonicus(internum_v.datum.nodus)) : NIHIL;
            s32 mensura = _mensura_typi(d, typus_interni);

            si (mensura > ZEPHYRUM)
            {
                fructus = _movere(d, nodus, MEDULLA_TYPUS_I64,
                    _registrum_temporarium(d),
                    medulla_op_immediatum((s64)mensura));
            }
            alioquin
            {
                _sistere(d, nodus, "magnitudo inaestimabilis");
                fructus = _registrum_temporarium(d);
            }
            frange;
        }
        casus (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI:
        {
            s64 valor = 0;

            si (silva_c89_constans_aestimare(d->sem, nodus, &valor))
            {
                fructus = _movere(d, nodus, MEDULLA_TYPUS_I64,
                    _registrum_temporarium(d),
                    medulla_op_immediatum(valor));
            }
            alioquin
            {
                _sistere(d, nodus, "magnitudo inaestimabilis");
                fructus = _registrum_temporarium(d);
            }
            frange;
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_INTEGER:
        casus (s32)SILVA_C89_GENUS_FOLIUM_CHARACTER:
        casus (s32)SILVA_C89_GENUS_FOLIUM_FLUITANS:
        {
            MedullaOperandum op = _ut_operandum(d, nodus);
            s32 mt = _typus_medullae(_typus_finalis(d, nodus));

            redde _movere(d, nodus, mt, _registrum_temporarium(d),
                op);   /* conversio iam in valore */
        }
        casus (s32)SILVA_C89_GENUS_VOCATIO:
        {
            fructus = _vocationem(d, nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_ACCESSUS:
        casus (s32)SILVA_C89_GENUS_SUBSCRIPTIO:
        {
            DemissioLocus locus = _ut_locum(d, nodus);

            si (!locus.valida)
            {
                redde _registrum_temporarium(d);   /* iam sistit */
            }
            {
                constans TypusC89* exutus  = _exutus(naturalis);
                              s32  mt       =
                                  _typus_medullae(naturalis);

                si (   mt            >= ZEPHYRUM && exutus != NIHIL
                    && exutus->genus != TYPUS_C89_ACIES
                    && exutus->genus != TYPUS_C89_FUNCTIO)
                {
                    fructus = _em(d, nodus, MEDULLA_OP_LEGERE, mt,
                        MEDULLA_TYPUS_NIHIL,
                        _registrum_temporarium(d),
                        medulla_op_registrum(locus.index),
                        medulla_op_nihil(), medulla_op_nihil());
                }
                alioquin
                {
                    fructus = locus.index;   /* inscriptio */
                }
            }
            frange;
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_CHORDA:
        {
            s32 index_symboli = _datum_chordae(d, nodus);

            si (index_symboli < ZEPHYRUM)
            {
                _sistere(d, nodus, "chorda indecodabilis");
                redde _registrum_temporarium(d);
            }
            fructus = _em(d, nodus, MEDULLA_OP_LOCUS,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d),
                medulla_op_symbolum(index_symboli),
                medulla_op_nihil(), medulla_op_nihil());
            frange;
        }
        casus (s32)SILVA_C89_GENUS_CONGERIES:
        {
            _sistere(d, nodus, "nondum congeries (M1b C)");
            redde _registrum_temporarium(d);
        }
        ordinarius:
        {
            character causa[LXIV];

            sprintf(causa, "expressio generis ignoti (%d)",
                (int)nodus->genus);
            _sistere(d, nodus, causa);   /* internatur - copia */
            redde _registrum_temporarium(d);
        }
    }
    /* conversio scopi annotata (lapsus/UAC/assignatio/...) */
    si (conversus != NIHIL)
    {
        fructus = _conversionem(d, nodus, fructus, naturalis,
            conversus);
    }
    redde fructus;
}

interior MedullaOperandum
_ut_operandum (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    constans SilvaNodus* canonicus = _canonicus(nodus);

    si (canonicus == NIHIL)
    {
        redde medulla_op_immediatum(0);
    }
    si (   canonicus->genus == (s32)SILVA_C89_GENUS_FOLIUM_INTEGER
        || canonicus->genus == (s32)SILVA_C89_GENUS_FOLIUM_CHARACTER)
    {
        s64 valor = 0;

        si (silva_c89_constans_aestimare(d->sem, canonicus, &valor))
        {
            constans TypusC89* finalis = _typus_finalis(d,
                canonicus);
            s32 mt = _typus_medullae(finalis);

            si (_fluitans_m(mt))
            {
                redde medulla_op_immediatum_f((f64)valor);
            }
            redde medulla_op_immediatum(valor);
        }
    }
    si (canonicus->genus == (s32)SILVA_C89_GENUS_FOLIUM_FLUITANS)
    {
        SilvaValor tok_v = silva_c89_folium_fluitans_tok_valor(
            canonicus);
        SilvaChorda textus = _tok_textus(tok_v);

        si (   textus.mensura > ZEPHYRUM
            && textus.mensura < XL)
        {
            character littera[XL];

            memcpy(littera, textus.datum,
                (memoriae_index)textus.mensura);
            littera[textus.mensura] = '\0';
            redde medulla_op_immediatum_f(strtod(littera, NIHIL));
        }
    }
    redde medulla_op_registrum(_expressionem(d, nodus));
}

interior s32
_ut_valor (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    MedullaOperandum op = _ut_operandum(d, nodus);

    si (op.genus == (s32)MEDULLA_OPERANDUM_REGISTRUM)
    {
        redde op.datum.index;
    }
    redde _movere(d, _canonicus(nodus),
        _typus_medullae(_typus_finalis(d, _canonicus(nodus))),
        _registrum_temporarium(d), op);
}

interior DemissioLocus
_ut_locum (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    DemissioLocus locus;

    locus.directum  = FALSUM;
    locus.index     = -I;
    locus.valida    = FALSUM;
    nodus           = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde locus;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        SilvaValor v = silva_c89_parenthesis_internum(nodus);

        redde _ut_locum(d, v.datum.nodus);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
    {
        constans SemanticaSymbolum* symbolum =
            silva_c89_symbolum_nodi(d->sem, nodus);
        DemissioSedes* sedes;

        si (symbolum == NIHIL)
        {
            redde locus;
        }
        sedes = _sedem_symboli(d, symbolum);
        si (   sedes                       != NIHIL
            && sedes->index_symboli_moduli >= ZEPHYRUM)
        {
            /* staticum locale */
            locus.directum = FALSUM;
            locus.index = _em(d, nodus, MEDULLA_OP_LOCUS,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d),
                medulla_op_symbolum(sedes->index_symboli_moduli),
                medulla_op_nihil(), medulla_op_nihil());
            locus.valida = VERUM;
            redde locus;
        }
        si (sedes != NIHIL)
        {
            locus.directum  = !sedes->est_arca;
            locus.index     = sedes->index_registri;
            locus.valida    = VERUM;
            redde locus;
        }
        si (   symbolum->profunditas > ZEPHYRUM
            && (symbolum->repositio & REPOSITIO_STATICA)
                != ZEPHYRUM)
        {
            redde locus;   /* staticum ante declarationem? */
        }
        /* globale */
        {
            s32 index_symboli = medulla_symbolum_internare(
                d->modulus, _titulum_symboli(d, symbolum));

            locus.directum = FALSUM;
            locus.index = _em(d, nodus, MEDULLA_OP_LOCUS,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d),
                medulla_op_symbolum(index_symboli),
                medulla_op_nihil(), medulla_op_nihil());
            locus.valida = VERUM;
            redde locus;
        }
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_UNARIUM)
    {
        SilvaChorda operator = _tok_textus(
            silva_c89_unarium_tok_operator(nodus));

        si (_op_est(operator, "*"))
        {
            SilvaValor internum_v = silva_c89_unarium_internum(
                nodus);

            locus.directum  = FALSUM;
            locus.index     = _ut_valor(d, internum_v.datum.nodus);
            locus.valida    = VERUM;
            redde locus;
        }
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_SUBSCRIPTIO)
    {
        SilvaValor basis_v = silva_c89_subscriptio_basis(nodus);
        SilvaValor index_v = silva_c89_subscriptio_index(nodus);
        constans SilvaNodus* basis_n;
        constans SilvaNodus* index_n;
        constans TypusC89* basis_ft;
        s32 mensura;
        s32 basis_registrum;
        MedullaOperandum index_op;
        s32 scalatum;

        si (   basis_v.genus != SILVA_VALOR_NODUS
            || index_v.genus != SILVA_VALOR_NODUS)
        {
            redde locus;
        }
        basis_n   = basis_v.datum.nodus;
        index_n   = index_v.datum.nodus;
        basis_ft  = _typus_finalis(d, _canonicus(basis_n));
        si (!_est_monstratorius(basis_ft))
        {
            /* subscriptio commutativa: i[a] */
            constans SilvaNodus* commutatum = basis_n;

            basis_n   = index_n;
            index_n   = commutatum;
            basis_ft  = _typus_finalis(d, _canonicus(basis_n));
        }
        mensura = _mensura_elementi_monstratoris(d, basis_ft);
        si (mensura <= ZEPHYRUM)
        {
            _sistere(d, nodus, "forma elementi ignota");
            redde locus;
        }
        basis_registrum  = _ut_valor(d, basis_n);
        index_op         = _ut_operandum(d, index_n);
        scalatum = _indicem_scalare(d, nodus, index_op,
            _typus_medullae(_typus_finalis(d, _canonicus(index_n))),
            mensura);
        locus.directum = FALSUM;
        locus.index = _em(d, nodus, MEDULLA_OP_ADDERE,
            MEDULLA_TYPUS_I64, MEDULLA_TYPUS_NIHIL,
            _registrum_temporarium(d),
            medulla_op_registrum(basis_registrum),
            medulla_op_registrum(scalatum), medulla_op_nihil());
        locus.valida = VERUM;
        redde locus;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_ACCESSUS)
    {
         SilvaValor basis_v = silva_c89_accessus_basis(nodus);
        SilvaChorda operator = _tok_textus(
            silva_c89_accessus_tok_operator(nodus));
        SilvaChorda titulus = _tok_textus(
            silva_c89_accessus_tok_titulus(nodus));
        constans TypusC89* typus_tag;
                      s32  inscriptio_basis;
                      s32  offset        = ZEPHYRUM;
        constans TypusC89* typus_membri  = NIHIL;

        si (basis_v.genus != SILVA_VALOR_NODUS)
        {
            redde locus;
        }
        si (_op_est(operator, "->"))
        {
            constans TypusC89* basis_ft = _exutus(_typus_finalis(d,
                _canonicus(basis_v.datum.nodus)));

            si (   basis_ft        == NIHIL
                || basis_ft->genus != TYPUS_C89_MONSTRATOR)
            {
                _sistere(d, nodus,
                    "accessus sagittae sine monstratore");
                redde locus;
            }
            typus_tag         = basis_ft->datum.monstrator.internum;
            inscriptio_basis  = _ut_valor(d, basis_v.datum.nodus);
        }
        alioquin
        {
            DemissioLocus locus_basis = _ut_locum(d,
                basis_v.datum.nodus);

            si (locus_basis.valida && !locus_basis.directum)
            {
                inscriptio_basis = locus_basis.index;
            }
            alioquin si (_est_aggregatum(_typus_finalis(d,
                         _canonicus(basis_v.datum.nodus))))
            {
                /* rvalor aggregatus (fructus vocationis, ...):
                 * valor aggregati EST inscriptio eius (conventio -
                 * vide _vocationem, _assignationem) */
                inscriptio_basis = _ut_valor(d,
                    basis_v.datum.nodus);
            }
            alioquin
            {
                _sistere(d, nodus,
                    "accessus sine inscriptione basis");
                redde locus;
            }
            typus_tag = silva_c89_typus_expressionis(d->sem,
                _canonicus(basis_v.datum.nodus));
        }
        si (!_membrum_invenire(d, typus_tag, titulus, &offset,
                &typus_membri))
        {
            _sistere(d, nodus, "accessus structurae incompletae");
            redde locus;
        }
        (vacuum)typus_membri;
        locus.directum = FALSUM;
        locus.index = (offset == ZEPHYRUM) ? inscriptio_basis
            : _em(d, nodus, MEDULLA_OP_ADDERE, MEDULLA_TYPUS_I64,
                  MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                  medulla_op_registrum(inscriptio_basis),
                  medulla_op_immediatum((s64)offset),
                  medulla_op_nihil());
        locus.valida = VERUM;
        redde locus;
    }
    redde locus;
}


/* ==================================================
 * Sententiae
 * ================================================== */

interior vacuum
_lista_sententiarum (
      Demissio* d,
    SilvaValor  lista)
{
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(lista);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(lista, i);

        si (v != NIHIL && v->genus == SILVA_VALOR_NODUS)
        {
            _sententiam(d, v->datum.nodus);
        }
    }
}

interior constans SemanticaSymbolum*
_symbolum_lexematis (
    constans Demissio* d,
           SilvaToken* lexema)
{
    redde (constans SemanticaSymbolum*)_tabulam_invenire(
        d->lexemata, (constans vacuum*)lexema);
}

/* congeries localis: imagine statica + copia si constans;
 * aliter zeri + copia + scripturae elementorum */
interior vacuum
_congeriem_localem (
               Demissio* d,
                    s32  inscriptio,
      constans TypusC89* typus,
    constans SilvaNodus* congeries)
{
    constans TypusC89* exutus = _exutus(typus);
    SilvaValor elementa = silva_c89_congeries_elementa(congeries);
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(elementa);
    s32 a = ZEPHYRUM;

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(elementa,
            (unsigned int)i);
          constans TypusC89* finis   = NIHIL;
                        s32  offset  = ZEPHYRUM;
        constans SilvaNodus* elementum;

        si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        elementum = _canonicus(v->datum.nodus);
        si (exutus->genus == TYPUS_C89_ACIES)
        {
            s32 mensura = _mensura_typi(d,
                exutus->datum.acies.elementum);

            si (mensura <= ZEPHYRUM)
            {
                _sistere(d, congeries, "forma elementi ignota");
                redde;
            }
            finis   = exutus->datum.acies.elementum;
            offset  = a * mensura;
        }
        alioquin si (exutus->genus == TYPUS_C89_STRUCTURA)
        {
            si (   _mensura_typi(d, exutus) < ZEPHYRUM
                || a >= (s32)exutus->datum.tag.numerus_membrorum)
            {
                _sistere(d, congeries, "congeries ultra membra");
                redde;
            }
            finis   = exutus->datum.tag.membra[a].typus;
            offset  = (s32)exutus->datum.tag.membra[a].offset;
        }
        alioquin si (exutus->genus == TYPUS_C89_UNIO)
        {
            si (a > ZEPHYRUM)
            {
                frange;
            }
            finis = exutus->datum.tag.membra[ZEPHYRUM].typus;
        }
        alioquin
        {
            finis = exutus;
        }
        {
            s32 sedes_elementi = (offset == ZEPHYRUM) ? inscriptio
                : _em(d, elementum, MEDULLA_OP_ADDERE,
                      MEDULLA_TYPUS_I64, MEDULLA_TYPUS_NIHIL,
                      _registrum_temporarium(d),
                      medulla_op_registrum(inscriptio),
                      medulla_op_immediatum((s64)offset),
                      medulla_op_nihil());

            si (elementum->genus == (s32)SILVA_C89_GENUS_CONGERIES)
            {
                _congeriem_localem(d, sedes_elementi, finis,
                    elementum);
            }
            alioquin
            {
                s32 mt = _typus_medullae(finis);

                si (mt < ZEPHYRUM)
                {
                    _sistere(d, elementum,
                        "elementum congeriei non scalare");
                }
                alioquin
                {
                    MedullaOperandum fons = _ut_operandum(d,
                        elementum);

                    (vacuum)_em(d, elementum, MEDULLA_OP_SCRIBERE,
                        mt, MEDULLA_TYPUS_NIHIL, -I,
                        medulla_op_registrum(sedes_elementi), fons,
                        medulla_op_nihil());
                }
            }
        }
        a++;
    }
}

/* initiator localis non scalaris (congeries / chorda in aciem) */
interior vacuum
_initiatorem_aggregatum (
                      Demissio* d,
                 DemissioSedes* sedes,
    constans SemanticaSymbolum* symbolum,
           constans SilvaNodus* initiator)
{
    s32 mensura = _mensura_completa(d, symbolum->typus, initiator);
    constans SilvaNodus* canonicus = _canonicus(initiator);

    si (mensura <= ZEPHYRUM || !sedes->est_arca)
    {
        _sistere(d, initiator, "forma aggregati ignota");
        redde;
    }
    /* chorda in aciem characterum: datum plenum + copia */
    si (canonicus->genus == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA)
    {
        s32 index_symboli = _datum_chordae_mensurae(d, canonicus,
            mensura);

        si (index_symboli < ZEPHYRUM)
        {
            _sistere(d, initiator, "chorda indecodabilis");
            redde;
        }
        {
            s32 fons = _em(d, initiator, MEDULLA_OP_LOCUS,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d),
                medulla_op_symbolum(index_symboli),
                medulla_op_nihil(), medulla_op_nihil());

            (vacuum)_em(d, initiator, MEDULLA_OP_COPIA,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I,
                medulla_op_registrum(sedes->index_registri),
                medulla_op_registrum(fons),
                medulla_op_immediatum((s64)mensura));
        }
        redde;
    }
    si (canonicus->genus != (s32)SILVA_C89_GENUS_CONGERIES)
    {
        _sistere(d, initiator, "initiator aggregati ignotus");
        redde;
    }
    si (_constans_est(d, symbolum->typus, canonicus))
    {
        /* imago statica integra + copia una */
              OfficinaChorda  titulus = _titulum_dati(d, "congeries");
        MedullaDatum* datum = medulla_datum_creare(d->modulus,
            titulus, (i32)mensura,
            (i32)_exutus(symbolum->typus)->ordinatio);

        si (   datum != NIHIL
            && _imaginem_scribere(d, datum, ZEPHYRUM,
                   symbolum->typus, canonicus))
        {
            s32 index_symboli = medulla_symbolum_internare(
                d->modulus, titulus);
            s32 fons = _em(d, initiator, MEDULLA_OP_LOCUS,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d),
                medulla_op_symbolum(index_symboli),
                medulla_op_nihil(), medulla_op_nihil());

            (vacuum)_em(d, initiator, MEDULLA_OP_COPIA,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I,
                medulla_op_registrum(sedes->index_registri),
                medulla_op_registrum(fons),
                medulla_op_immediatum((s64)mensura));
            redde;
        }
        /* cadit ad viam infra */
    }
    /* zeri + copia (C89: membra intacta zephyrum) + scripturae */
    {
              OfficinaChorda  titulus = _titulum_dati(d, "zeri");
        MedullaDatum* datum = medulla_datum_creare(d->modulus,
            titulus, (i32)mensura, I);

        si (datum != NIHIL)
        {
            s32 index_symboli = medulla_symbolum_internare(
                d->modulus, titulus);
            s32 fons = _em(d, initiator, MEDULLA_OP_LOCUS,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d),
                medulla_op_symbolum(index_symboli),
                medulla_op_nihil(), medulla_op_nihil());

            (vacuum)_em(d, initiator, MEDULLA_OP_COPIA,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I,
                medulla_op_registrum(sedes->index_registri),
                medulla_op_registrum(fons),
                medulla_op_immediatum((s64)mensura));
        }
    }
    _congeriem_localem(d, sedes->index_registri, symbolum->typus,
        canonicus);
}

/* staticum locale: datum moduli nomine presso $functio.titulus */
interior vacuum
_staticum_locale (
                      Demissio* d,
    constans SemanticaSymbolum* symbolum,
           constans SilvaNodus* nodus,
           constans SilvaNodus* initiator)
{
    character littera[CXXVIII];
    OfficinaChorda titulus;
    i32 scriptum = ZEPHYRUM;
    i32 caput_f;
    i32 caput_s;
    MedullaDatum* datum;
    s32 mensura = _mensura_completa(d, symbolum->typus, initiator);
    s32 index_symboli;

    si (mensura <= ZEPHYRUM)
    {
        _sistere(d, nodus, "forma statici localis ignota");
        redde;
    }
    caput_f = (d->functio->titulus.mensura < XL)
        ? (i32)d->functio->titulus.mensura : XL;
    caput_s = (symbolum->titulus.mensura < XL)
        ? (i32)symbolum->titulus.mensura : XL;
    memcpy(littera, d->functio->titulus.datum,
        (memoriae_index)caput_f);
    scriptum           = caput_f;
    littera[scriptum]  = '.';
    scriptum++;
    memcpy(littera + scriptum, symbolum->titulus.datum,
        (memoriae_index)caput_s);
    scriptum         += caput_s;
    titulus.datum    = (i8*)littera;
    titulus.mensura  = scriptum;
    titulus          = officina_chorda_transcribere(titulus, d->piscina);

    /* collisio (statica eiusdem nominis in scopis fratribus) ->
     * suffixum */
    index_symboli = medulla_symbolum_internare(d->modulus, titulus);
    si (medulla_symbolum_obtinere(d->modulus, index_symboli)->genus
        != (s32)MEDULLA_SYMBOLUM_EXTERNUM)
    {
        character alterum[CXXVIII];
           OfficinaChorda secunda;
              s32 n = (s32)sprintf(alterum, "%.*s_%d", (int)scriptum,
                  littera, (int)d->numerator_datorum);

        d->numerator_datorum++;
        secunda.datum    = (i8*)alterum;
        secunda.mensura  = (i32)n;
        titulus          = officina_chorda_transcribere(secunda, d->piscina);
        index_symboli = medulla_symbolum_internare(d->modulus,
            titulus);
    }
    datum = medulla_datum_creare(d->modulus, titulus, (i32)mensura,
        (i32)_ordinatio_typi(d, symbolum->typus));
    si (datum == NIHIL)
    {
        _sistere(d, nodus, "staticum locale sine dato");
        redde;
    }
    si (   initiator != NIHIL
        && !_imaginem_scribere(d, datum, ZEPHYRUM, symbolum->typus,
               initiator))
    {
        _notare(d, nodus, "initiator staticus inaestimabilis");
    }
    {
        DemissioSedes* sedes = _sedem_creare(d, symbolum, -I,
            VERUM);

        si (sedes != NIHIL)
        {
            sedes->index_symboli_moduli = index_symboli;
        }
    }
}

interior vacuum
_declarationem (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    SilvaValor declaratores = silva_c89_declaratio_declaratores(
        nodus);
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(declaratores);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(declaratores, i);
        constans SilvaNodus* elementum;
        constans SilvaNodus* initiator = NIHIL;
        SilvaToken* lexema;
        constans SemanticaSymbolum* symbolum;
        DemissioSedes* sedes;

        si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        elementum = _canonicus(v->datum.nodus);
        si (elementum->genus
            == (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS)
        {
            SilvaValor init_v =
                silva_c89_declarator_initiatus_initiator(
                elementum);

            si (init_v.genus == SILVA_VALOR_NODUS)
            {
                initiator = init_v.datum.nodus;
            }
        }
        lexema = silva_c89_declaratoris_titulus(elementum);
        si (lexema == NIHIL)
        {
            perge;
        }
        symbolum = _symbolum_lexematis(d, lexema);
        si (   symbolum        == NIHIL
            || symbolum->genus == (int)SYMBOLUM_TYPEDEF)
        {
            perge;
        }
        si ((symbolum->repositio & REPOSITIO_STATICA) != ZEPHYRUM)
        {
            _staticum_locale(d, symbolum, nodus, initiator);
            perge;
        }
        sedes = _sedem_symboli(d, symbolum);
        si (sedes == NIHIL || initiator == NIHIL)
        {
            perge;
        }
        si (   _canonicus(initiator)->genus
                == (s32)SILVA_C89_GENUS_CONGERIES
            || (_canonicus(initiator)->genus
                    == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA
                && _exutus(symbolum->typus) != NIHIL
                && _exutus(symbolum->typus)->genus
                    == TYPUS_C89_ACIES))
        {
            _initiatorem_aggregatum(d, sedes, symbolum, initiator);
            perge;
        }
        {
                         s32 mt = _typus_medullae(symbolum->typus);
            MedullaOperandum fons;

            si (   mt < ZEPHYRUM && _est_aggregatum(symbolum->typus)
                && sedes->est_arca)
            {
                /* initiator aggregati: copia ex inscriptione */
                s32 mensura = _mensura_typi(d, symbolum->typus);
                s32 origo_valoris;

                si (mensura <= ZEPHYRUM)
                {
                    _sistere(d, nodus, "forma aggregati ignota");
                    perge;
                }
                origo_valoris = _ut_valor(d, initiator);
                (vacuum)_em(d, nodus, MEDULLA_OP_COPIA,
                    MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I,
                    medulla_op_registrum(sedes->index_registri),
                    medulla_op_registrum(origo_valoris),
                    medulla_op_immediatum((s64)mensura));
                perge;
            }
            fons = _ut_operandum(d, initiator);
            si (mt < ZEPHYRUM)
            {
                _sistere(d, nodus,
                    "initiator aggregati sine congerie");
                perge;
            }
            si (sedes->est_arca)
            {
                (vacuum)_em(d, nodus, MEDULLA_OP_SCRIBERE, mt,
                    MEDULLA_TYPUS_NIHIL, -I,
                    medulla_op_registrum(sedes->index_registri),
                    fons, medulla_op_nihil());
            }
            alioquin
            {
                (vacuum)_movere(d, nodus, mt, sedes->index_registri,
                    fons);
            }
        }
    }
}

interior s32
_bloccum_tituli (
       Demissio* d,
    SilvaChorda  titulus)
{
    character littera[LXIV];
       OfficinaChorda quaesitum;
          i32 caput = (titulus.mensura < LX) ? titulus.mensura : LX;
          i32 i;
          i32 m;

    littera[ZEPHYRUM]  = 'l';
    littera[I]         = '_';
    memcpy(littera + II, titulus.datum, (memoriae_index)caput);
    quaesitum.datum    = (i8*)littera;
    quaesitum.mensura  = caput + II;

    m = officina_xar_numerus(d->functio->blocci);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MedullaBloccus* bloccus =
            (constans MedullaBloccus*)officina_xar_obtinere(
                d->functio->blocci, i);

        si (   bloccus->titulus.mensura == quaesitum.mensura
            && memcmp(bloccus->titulus.datum, quaesitum.datum,
                   (memoriae_index)quaesitum.mensura) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde medulla_bloccum_creare(d->functio,
        officina_chorda_transcribere(quaesitum, d->piscina));
}

interior vacuum
_commutationem (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    SilvaValor discrimen_v  = silva_c89_commutatio_discrimen(nodus);
    SilvaValor corpus_v     = silva_c89_commutatio_corpus(nodus);
    constans SilvaNodus* corpus;
    SilvaValor elementa;
    s32 discrimen;
    s32 mt = _typus_medullae(_typus_finalis(d,
        _canonicus(discrimen_v.datum.nodus)));
    s32 b_finis;
    s32 b_ordinarius = -I;
    s32 frange_prior = d->frange_finis;
    i32 i;
    i32 m;

    si (corpus_v.genus != SILVA_VALOR_NODUS)
    {
        _sistere(d, nodus, "commutatio sine corpore");
        redde;
    }
    corpus = _canonicus(corpus_v.datum.nodus);
    si (corpus->genus != (s32)SILVA_C89_GENUS_CORPUS)
    {
        _sistere(d, nodus, "corpus commutationis non compositum");
        redde;
    }
    elementa         = silva_c89_corpus_elementa(corpus);
    m                = (i32)silva_valor_lista_numerus(elementa);
    discrimen        = _ut_valor(d, discrimen_v.datum.nodus);
    b_finis          = _bloccum_novum(d, "comm_finis");
    d->frange_finis  = b_finis;

    /* transitus primus: bloccos casuum creare + dispensationem
     * emittere (catena comparationum - tabula saliendi parcata) */
    {
        OfficinaXar* blocci_casuum = officina_xar_creare(d->piscina,
            (i32)magnitudo(s32));

        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(elementa, i);
            constans SilvaNodus* elementum;
            s32* sedes_blocci;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            elementum = _canonicus(v->datum.nodus);
            si (elementum->genus == (s32)SILVA_C89_GENUS_CASUS)
            {
                       s64 valor    = 0;
                       s32 b_casus  = _bloccum_novum(d, "comm_casus");
                SilvaValor valor_v = silva_c89_casus_valor(
                    elementum);
                s32 probatum;

                sedes_blocci   = (s32*)officina_xar_addere(blocci_casuum);
                *sedes_blocci  = b_casus;
                si (!silva_c89_constans_aestimare(d->sem,
                        valor_v.datum.nodus, &valor))
                {
                    _sistere(d, elementum, "casus inaestimabilis");
                    perge;
                }
                probatum = _em(d, elementum, MEDULLA_OP_AEQUALIS,
                    (mt >= ZEPHYRUM) ? mt : MEDULLA_TYPUS_S32,
                    MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                    medulla_op_registrum(discrimen),
                    medulla_op_immediatum(valor),
                    medulla_op_nihil());
                {
                    s32 b_proximus = _bloccum_novum(d,
                        "comm_proba");

                    _ramus(d, elementum,
                        medulla_op_registrum(probatum), b_casus,
                        b_proximus);
                    d->bloccus = b_proximus;
                }
            }
            alioquin si (elementum->genus
                         == (s32)SILVA_C89_GENUS_ORDINARIUS)
            {
                b_ordinarius   = _bloccum_novum(d, "comm_ordinarius");
                sedes_blocci   = (s32*)officina_xar_addere(blocci_casuum);
                *sedes_blocci  = b_ordinarius;
            }
            alioquin
            {
                sedes_blocci   = (s32*)officina_xar_addere(blocci_casuum);
                *sedes_blocci  = -I;
            }
        }
        _salire_si_vivus(d, (b_ordinarius >= ZEPHYRUM)
            ? b_ordinarius : b_finis);

        /* transitus alter: corpora ordine fontis (perlapsus) */
        {
            i32 a = ZEPHYRUM;

            per (i = ZEPHYRUM; i < m; i++)
            {
                SilvaValor* v = silva_valor_lista_obtinere(elementa,
                    i);
                constans SilvaNodus* elementum;

                si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
                {
                    perge;
                }
                elementum = _canonicus(v->datum.nodus);
                si (   elementum->genus == (s32)SILVA_C89_GENUS_CASUS
                    || elementum->genus
                        == (s32)SILVA_C89_GENUS_ORDINARIUS)
                {
                    s32* b = (s32*)officina_xar_obtinere(blocci_casuum,
                        (i32)a);

                    a++;
                    si (b == NIHIL || *b < ZEPHYRUM)
                    {
                        perge;
                    }
                    _salire_si_vivus(d, *b);   /* perlapsus */
                    d->bloccus = *b;
                    si (elementum->genus
                        == (s32)SILVA_C89_GENUS_CASUS)
                    {
                        _lista_sententiarum(d,
                            silva_c89_casus_sententiae(elementum));
                    }
                    alioquin
                    {
                        _lista_sententiarum(d,
                            silva_c89_ordinarius_sententiae(
                                elementum));
                    }
                }
                alioquin
                {
                    a++;
                    _sententiam(d, elementum);
                }
            }
        }
    }
    _salire_si_vivus(d, b_finis);
    d->bloccus       = b_finis;
    d->frange_finis  = frange_prior;
}

interior vacuum
_sententiam (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde;
    }
    commutatio (nodus->genus)
    {
        casus (s32)SILVA_C89_GENUS_CORPUS:
        {
            _lista_sententiarum(d, silva_c89_corpus_elementa(nodus));
            redde;
        }
        casus (s32)SILVA_C89_GENUS_SENTENTIA_VACUA:
        {
            redde;
        }
        casus (s32)SILVA_C89_GENUS_DECLARATIO:
        {
            _declarationem(d, nodus);
            redde;
        }
        casus (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS:
        {
            SilvaValor v = silva_c89_sententia_expressionis_expressio(
                nodus);

            si (v.genus == SILVA_VALOR_NODUS)
            {
                (vacuum)_ut_operandum(d, v.datum.nodus);
            }
            redde;
        }
        casus (s32)SILVA_C89_GENUS_SI:
        {
            SilvaValor cond_v   = silva_c89_si_conditio(nodus);
            SilvaValor cons_v   = silva_c89_si_consequens(nodus);
            SilvaValor alio_v   = silva_c89_si_alioquin(nodus);
            b32 habet_alioquin  = (alio_v.genus == SILVA_VALOR_NODUS);
            s32 b_verum         = _bloccum_novum(d, "si_verum");
            s32 b_falsum = habet_alioquin
                ? _bloccum_novum(d, "si_falsum") : -I;
            s32 b_finis   = _bloccum_novum(d, "si_finis");
            s32 conditio  = _ut_valor(d, cond_v.datum.nodus);

            _ramus(d, nodus, medulla_op_registrum(conditio),
                b_verum, habet_alioquin ? b_falsum : b_finis);
            d->bloccus = b_verum;
            si (cons_v.genus == SILVA_VALOR_NODUS)
            {
                _sententiam(d, cons_v.datum.nodus);
            }
            _salire_si_vivus(d, b_finis);
            si (habet_alioquin)
            {
                d->bloccus = b_falsum;
                _sententiam(d, alio_v.datum.nodus);
                _salire_si_vivus(d, b_finis);
            }
            d->bloccus = b_finis;
            redde;
        }
        casus (s32)SILVA_C89_GENUS_DUM:
        {
            s32 b_proba       = _bloccum_novum(d, "dum_proba");
            s32 b_corpus      = _bloccum_novum(d, "dum_corpus");
            s32 b_finis       = _bloccum_novum(d, "dum_finis");
            s32 frange_prior  = d->frange_finis;
            s32 perge_prior   = d->perge_finis;

            _salire_si_vivus(d, b_proba);
            d->bloccus = b_proba;
            {
                SilvaValor cond_v    = silva_c89_dum_conditio(nodus);
                       s32 conditio  = _ut_valor(d,
                           cond_v.datum.nodus);

                _ramus(d, nodus, medulla_op_registrum(conditio),
                    b_corpus, b_finis);
            }
            d->bloccus       = b_corpus;
            d->frange_finis  = b_finis;
            d->perge_finis   = b_proba;
            {
                SilvaValor corpus_v = silva_c89_dum_corpus(nodus);

                si (corpus_v.genus == SILVA_VALOR_NODUS)
                {
                    _sententiam(d, corpus_v.datum.nodus);
                }
            }
            _salire_si_vivus(d, b_proba);
            d->frange_finis  = frange_prior;
            d->perge_finis   = perge_prior;
            d->bloccus       = b_finis;
            redde;
        }
        casus (s32)SILVA_C89_GENUS_FAC_DUM:
        {
            s32 b_corpus      = _bloccum_novum(d, "fac_corpus");
            s32 b_proba       = _bloccum_novum(d, "fac_proba");
            s32 b_finis       = _bloccum_novum(d, "fac_finis");
            s32 frange_prior  = d->frange_finis;
            s32 perge_prior   = d->perge_finis;

            _salire_si_vivus(d, b_corpus);
            d->bloccus       = b_corpus;
            d->frange_finis  = b_finis;
            d->perge_finis   = b_proba;
            {
                SilvaValor corpus_v = silva_c89_fac_dum_corpus(
                    nodus);

                si (corpus_v.genus == SILVA_VALOR_NODUS)
                {
                    _sententiam(d, corpus_v.datum.nodus);
                }
            }
            _salire_si_vivus(d, b_proba);
            d->bloccus = b_proba;
            {
                SilvaValor cond_v = silva_c89_fac_dum_conditio(
                    nodus);
                s32 conditio = _ut_valor(d, cond_v.datum.nodus);

                _ramus(d, nodus, medulla_op_registrum(conditio),
                    b_corpus, b_finis);
            }
            d->frange_finis  = frange_prior;
            d->perge_finis   = perge_prior;
            d->bloccus       = b_finis;
            redde;
        }
        casus (s32)SILVA_C89_GENUS_PER:
        {
            SilvaValor clausula_v = silva_c89_per_clausula(nodus);
            constans SilvaNodus* clausula = (clausula_v.genus
                == SILVA_VALOR_NODUS)
                ? _canonicus(clausula_v.datum.nodus) : NIHIL;
            s32 b_proba       = _bloccum_novum(d, "per_proba");
            s32 b_corpus      = _bloccum_novum(d, "per_corpus");
            s32 b_passus      = _bloccum_novum(d, "per_passus");
            s32 b_finis       = _bloccum_novum(d, "per_finis");
            s32 frange_prior  = d->frange_finis;
            s32 perge_prior   = d->perge_finis;

            si (clausula != NIHIL)
            {
                SilvaValor initium_v = silva_c89_per_clausula_initium(
                    clausula);

                si (initium_v.genus == SILVA_VALOR_NODUS)
                {
                    constans SilvaNodus* initium = _canonicus(
                        initium_v.datum.nodus);

                    si (initium->genus
                        == (s32)SILVA_C89_GENUS_DECLARATIO)
                    {
                        _declarationem(d, initium);   /* C99-forma
                                                       * tolerata */
                    }
                    alioquin
                    {
                        (vacuum)_ut_operandum(d, initium);
                    }
                }
            }
            _salire_si_vivus(d, b_proba);
            d->bloccus = b_proba;
            {
                SilvaValor cond_v;

                cond_v.genus = SILVA_VALOR_NIHIL;
                si (clausula != NIHIL)
                {
                    cond_v = silva_c89_per_clausula_conditio(
                        clausula);
                }
                si (cond_v.genus == SILVA_VALOR_NODUS)
                {
                    s32 conditio = _ut_valor(d, cond_v.datum.nodus);

                    _ramus(d, nodus, medulla_op_registrum(conditio),
                        b_corpus, b_finis);
                }
                alioquin
                {
                    _salire_si_vivus(d, b_corpus);
                }
            }
            d->bloccus       = b_corpus;
            d->frange_finis  = b_finis;
            d->perge_finis   = b_passus;
            {
                SilvaValor corpus_v = silva_c89_per_corpus(nodus);

                si (corpus_v.genus == SILVA_VALOR_NODUS)
                {
                    _sententiam(d, corpus_v.datum.nodus);
                }
            }
            _salire_si_vivus(d, b_passus);
            d->bloccus = b_passus;
            si (clausula != NIHIL)
            {
                SilvaValor passus_v = silva_c89_per_clausula_passus(
                    clausula);

                si (passus_v.genus == SILVA_VALOR_NODUS)
                {
                    (vacuum)_ut_operandum(d, passus_v.datum.nodus);
                }
            }
            _salire_si_vivus(d, b_proba);
            d->frange_finis  = frange_prior;
            d->perge_finis   = perge_prior;
            d->bloccus       = b_finis;
            redde;
        }
        casus (s32)SILVA_C89_GENUS_COMMUTATIO:
        {
            _commutationem(d, nodus);
            redde;
        }
        casus (s32)SILVA_C89_GENUS_TITULATUM:
        {
            SilvaChorda titulus = _tok_textus(
                silva_c89_titulatum_tok_titulus(nodus));
                   s32 b = _bloccum_tituli(d, titulus);
            SilvaValor sententia_v = silva_c89_titulatum_sententia(
                nodus);

            _salire_si_vivus(d, b);
            d->bloccus = b;
            si (sententia_v.genus == SILVA_VALOR_NODUS)
            {
                _sententiam(d, sententia_v.datum.nodus);
            }
            redde;
        }
        casus (s32)SILVA_C89_GENUS_SALTA:
        {
            SilvaChorda titulus = _tok_textus(
                silva_c89_salta_tok_destinatio(nodus));
            s32 b = _bloccum_tituli(d, titulus);

            _salire_si_vivus(d, b);
            redde;
        }
        casus (s32)SILVA_C89_GENUS_FRANGE:
        {
            si (d->frange_finis >= ZEPHYRUM)
            {
                _salire_si_vivus(d, d->frange_finis);
            }
            alioquin
            {
                _sistere(d, nodus, "frange extra ansam");
            }
            redde;
        }
        casus (s32)SILVA_C89_GENUS_PERGE:
        {
            si (d->perge_finis >= ZEPHYRUM)
            {
                _salire_si_vivus(d, d->perge_finis);
            }
            alioquin
            {
                _sistere(d, nodus, "perge extra ansam");
            }
            redde;
        }
        casus (s32)SILVA_C89_GENUS_REDDE:
        {
                  SilvaValor valor_v  = silva_c89_redde_valor(nodus);
            MedullaOperandum a        = medulla_op_nihil();

            si (   valor_v.genus    == SILVA_VALOR_NODUS
                && d->reditus_index >= ZEPHYRUM)
            {
                /* reditus aggregatus: copia in destinationem
                 * (conventio C7), redde sine valore */
                s32 fons = _ut_valor(d, valor_v.datum.nodus);

                si (d->reditus_mensura > ZEPHYRUM)
                {
                    (vacuum)_em(d, nodus, MEDULLA_OP_COPIA,
                        MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                        -I,
                        medulla_op_registrum(d->reditus_index),
                        medulla_op_registrum(fons),
                        medulla_op_immediatum(
                            (s64)d->reditus_mensura));
                }
                alioquin
                {
                    _sistere(d, nodus, "forma reditus ignota");
                }
            }
            alioquin si (valor_v.genus == SILVA_VALOR_NODUS)
            {
                a = _ut_operandum(d, valor_v.datum.nodus);
            }
            (vacuum)_em(d, nodus, MEDULLA_OP_REDDE,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I, a,
                medulla_op_nihil(), medulla_op_nihil());
            redde;
        }
        casus (s32)SILVA_C89_GENUS_CONDITIONALIS:
        {
            SilvaValor rami = silva_c89_conditionalis_rami(nodus);
                   i32 i;
                   i32 m = (i32)silva_valor_lista_numerus(rami);

            per (i = ZEPHYRUM; i < m; i++)
            {
                SilvaValor* v = silva_valor_lista_obtinere(rami, i);

                si (   v != NIHIL && v->genus == SILVA_VALOR_NODUS
                    && v->datum.nodus->genus
                        == (s32)SILVA_C89_GENUS_RAMUS_SUMPTUS)
                {
                    _lista_sententiarum(d,
                        silva_c89_ramus_sumptus_contentum(
                            v->datum.nodus));
                }
            }
            redde;
        }
        casus (s32)SILVA_C89_GENUS_ERROR:
        {
            redde;   /* corpus sine erroribus; robustitas */
        }
        ordinarius:
        {
            character causa[LXIV];

            sprintf(causa, "sententia generis ignoti (%d)",
                (int)nodus->genus);
            _sistere(d, nodus, causa);
            redde;
        }
    }
}


/* ==================================================
 * Praecursus: symbola inscriptione capta + locales colligere
 * ================================================== */

interior vacuum
_praecursum (
               Demissio* d,
    constans SilvaNodus* nodus,
                    OfficinaXar* locales)
{
    i32 i;

    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_DECLARATIO)
    {
        SilvaValor declaratores = silva_c89_declaratio_declaratores(
            nodus);
        i32 m = (i32)silva_valor_lista_numerus(declaratores);
        i32 k;

        per (k = ZEPHYRUM; k < m; k++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(declaratores,
                k);
            constans SilvaNodus* elementum;
            constans SilvaNodus* initiator = NIHIL;
            SilvaToken* lexema;
            constans SemanticaSymbolum* symbolum;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            elementum = _canonicus(v->datum.nodus);
            si (elementum->genus
                == (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS)
            {
                SilvaValor init_v =
                    silva_c89_declarator_initiatus_initiator(
                        elementum);

                si (init_v.genus == SILVA_VALOR_NODUS)
                {
                    initiator = init_v.datum.nodus;
                }
            }
            lexema = silva_c89_declaratoris_titulus(elementum);
            si (lexema == NIHIL)
            {
                perge;
            }
            symbolum = _symbolum_lexematis(d, lexema);
            si (   symbolum        != NIHIL
                && symbolum->genus != (int)SYMBOLUM_TYPEDEF
                && (symbolum->repositio & REPOSITIO_STATICA)
                    == ZEPHYRUM
                && symbolum->profunditas > ZEPHYRUM)
            {
                DemissioLocale* locale =
                    (DemissioLocale*)officina_xar_addere(locales);

                si (locale != NIHIL)
                {
                    locale->symbolum   = symbolum;
                    locale->initiator  = initiator;
                }
            }
        }
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_UNARIUM)
    {
        SilvaChorda operator = _tok_textus(
            silva_c89_unarium_tok_operator(nodus));

        si (_op_est(operator, "&"))
        {
            constans SilvaNodus* basis = NIHIL;
                     SilvaValor  v = silva_c89_unarium_internum(nodus);

            si (v.genus == SILVA_VALOR_NODUS)
            {
                basis = _canonicus(v.datum.nodus);
            }
            dum (basis != NIHIL)
            {
                si (basis->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
                {
                    v = silva_c89_parenthesis_internum(basis);
                }
                alioquin si (basis->genus
                             == (s32)SILVA_C89_GENUS_SUBSCRIPTIO)
                {
                    v = silva_c89_subscriptio_basis(basis);
                }
                alioquin si (   basis->genus
                             == (s32)SILVA_C89_GENUS_ACCESSUS
                             && _op_est(_tok_textus(
                             silva_c89_accessus_tok_operator(basis)),
                             "."))
                {
                    v = silva_c89_accessus_basis(basis);
                }
                alioquin
                {
                    frange;
                }
                basis = (v.genus == SILVA_VALOR_NODUS)
                    ? _canonicus(v.datum.nodus) : NIHIL;
            }
            si (   basis != NIHIL && basis->genus
                == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
            {
                constans SemanticaSymbolum* symbolum =
                    silva_c89_symbolum_nodi(d->sem, basis);

                si (   symbolum != NIHIL
                    && symbolum->profunditas > ZEPHYRUM)
                {
                    _tabulam_ponere(d, d->capti,
                        (constans vacuum*)symbolum,
                        _sine_constante((constans vacuum*)symbolum));
                }
            }
        }
    }
    /* recursio generalis per loci (NODUS + elementa listarum) */
    per (i = ZEPHYRUM; i < (i32)nodus->numerus_locorum; i++)
    {
        constans SilvaValor* v = &nodus->loci[i];

        si (v->genus == SILVA_VALOR_NODUS)
        {
            _praecursum(d, v->datum.nodus, locales);
        }
        alioquin si (v->genus == SILVA_VALOR_LISTA)
        {
            i32 k;
            i32 m = (i32)silva_valor_lista_numerus(*v);

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* e = silva_valor_lista_obtinere(*v,
                    (unsigned int)k);

                si (e != NIHIL && e->genus == SILVA_VALOR_NODUS)
                {
                    _praecursum(d, e->datum.nodus, locales);
                }
            }
        }
    }
}


/* ==================================================
 * Functio
 * ================================================== */

interior constans SilvaNodus*
_declarator_functionis_invenire (
    constans SilvaNodus* declarator)
{
    s32 custos = ZEPHYRUM;

    declarator = _canonicus(declarator);
    dum (declarator != NIHIL && custos < XXXII)
    {
        SilvaValor v;

        custos++;
        commutatio (declarator->genus)
        {
            casus (s32)SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS:
            {
                redde declarator;
            }
            casus (s32)SILVA_C89_GENUS_DECLARATOR_MONSTRATOR:
            {
                v = silva_c89_declarator_monstrator_internum(
                    declarator);
                frange;
            }
            casus (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS:
            {
                v = silva_c89_declarator_initiatus_declarator(
                    declarator);
                frange;
            }
            casus (s32)SILVA_C89_GENUS_PARENTHESIS:
            {
                v = silva_c89_parenthesis_internum(declarator);
                frange;
            }
            casus (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI:
            {
                v = silva_c89_declarator_aciei_internum(declarator);
                frange;
            }
            ordinarius:
            {
                redde NIHIL;
            }
        }
        declarator = (v.genus == SILVA_VALOR_NODUS)
            ? _canonicus(v.datum.nodus) : NIHIL;
    }
    redde NIHIL;
}

interior b32
_arcam_symboli (
                      Demissio* d,
    constans SemanticaSymbolum* symbolum)
{
    constans TypusC89* exutus  = _exutus(symbolum->typus);
                  s32  mt       =
                      _typus_medullae(symbolum->typus);

    si (_tabulam_invenire(d->capti, (constans vacuum*)symbolum)
        != NIHIL)
    {
        redde VERUM;
    }
    si (mt < ZEPHYRUM)
    {
        redde VERUM;   /* aggregatum */
    }
    si (exutus != NIHIL && exutus->genus == TYPUS_C89_ACIES)
    {
        redde VERUM;
    }
    redde FALSUM;
}

interior vacuum
_functionem (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    SilvaValor declarator_v = silva_c89_definitio_functionis_declarator(
        nodus);
    SilvaValor corpus_v = silva_c89_definitio_functionis_corpus(nodus);
    SilvaToken* lexema;
    constans SemanticaSymbolum* symbolum;
    constans TypusC89* typus_functionis;
    constans TypusC89* reditus = NIHIL;
    s32 mt_reditus;
    b32 reditus_aggregatus = FALSUM;
    OfficinaXar* locales;
    i32 i;
    i32 m;

    si (   declarator_v.genus != SILVA_VALOR_NODUS
        || corpus_v.genus     != SILVA_VALOR_NODUS)
    {
        redde;
    }
    lexema = silva_c89_declaratoris_titulus(
        _canonicus(declarator_v.datum.nodus));
    symbolum = (lexema != NIHIL) ? _symbolum_lexematis(d, lexema)
                                 : NIHIL;
    si (symbolum == NIHIL)
    {
        redde;
    }
    typus_functionis = _exutus(symbolum->typus);
    si (   typus_functionis        == NIHIL
        || typus_functionis->genus != TYPUS_C89_FUNCTIO)
    {
        redde;
    }
    reditus     = _exutus(typus_functionis->datum.functio.reditus);
    mt_reditus  = _typus_medullae(reditus);
    si (   mt_reditus < ZEPHYRUM && reditus != NIHIL
        && (reditus->genus == TYPUS_C89_STRUCTURA
            || reditus->genus == TYPUS_C89_UNIO))
    {
        reditus_aggregatus = VERUM;
    }
    d->functio = medulla_functionem_creare(d->modulus,
        _titulum_symboli(d, symbolum),
        (mt_reditus >= ZEPHYRUM) ? mt_reditus : MEDULLA_TYPUS_NIHIL,
        typus_functionis->datum.functio.est_variadica);
    si (d->functio == NIHIL)
    {
        redde;   /* definitio duplex */
    }
    /* provenientia functionis (M3): locellus designatus sed numquam
     * impletus ante indicium - probatio ancestriae variabilium eum
     * poscit (et _lineam_colligere eo meliorescit) */
    d->functio->origo = nodus;
    d->numerator = ZEPHYRUM;
    d->frange_finis = -I;
    d->perge_finis = -I;
    d->reditus_index = -I;
    d->reditus_mensura = -I;
    d->sedes = officina_tabula_dispersa_creare_chorda(d->piscina, CCLVI);
    d->capti = officina_tabula_dispersa_creare_chorda(d->piscina, CCLVI);
    locales = officina_xar_creare(d->piscina,
        (i32)magnitudo(DemissioLocale));

    /* praecursus: capti + locales (ante residentiam) */
    _praecursum(d, corpus_v.datum.nodus, locales);

    /* reditus aggregatus: parametrum destinationis EXPLICITUM
     * primum (conventio C7) */
    si (reditus_aggregatus)
    {
        s32 mensura = _mensura_typi(d, reditus);

        d->reditus_index = medulla_parametrum_addere(d->functio,
            _ch_literis("reditus"), MEDULLA_TYPUS_I64);
        d->reditus_mensura = mensura;
    }

    /* parametra: registra prima (ordo signaturae); symbola
     * servata pro arca post initium */
    {
        constans SilvaNodus* df = _declarator_functionis_invenire(
            declarator_v.datum.nodus);
        SilvaValor  parametra;
               OfficinaXar* symbola_parametrorum = officina_xar_creare(d->piscina,
                   (i32)magnitudo(SemanticaSymbolum*));

        si (df != NIHIL)
        {
            parametra = silva_c89_declarator_functionis_parametra(
                df);
            m = (i32)silva_valor_lista_numerus(parametra);
            per (i = ZEPHYRUM; i < m; i++)
            {
                SilvaValor* v = silva_valor_lista_obtinere(
                    parametra, i);
                SilvaValor pd;
                SilvaToken* p_lexema;
                constans SemanticaSymbolum* p_symbolum;
                s32 index;

                si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
                {
                    perge;
                }
                pd = silva_c89_parametrum_declarator(
                    _canonicus(v->datum.nodus));
                si (pd.genus != SILVA_VALOR_NODUS)
                {
                    perge;   /* (void) aut abstractum */
                }
                p_lexema = silva_c89_declaratoris_titulus(
                    _canonicus(pd.datum.nodus));
                p_symbolum = (p_lexema != NIHIL)
                    ? _symbolum_lexematis(d, p_lexema) : NIHIL;
                si (p_symbolum == NIHIL)
                {
                    perge;
                }
                {
                    s32 mt = _typus_medullae(p_symbolum->typus);
                    constans SemanticaSymbolum** sedes_p;

                    index = medulla_parametrum_addere(d->functio,
                        _ch_de_silva(p_symbolum->titulus),
                        (mt >= ZEPHYRUM) ? mt : MEDULLA_TYPUS_I64);
                    (vacuum)_sedem_creare(d, p_symbolum, index,
                        FALSUM);
                    sedes_p = (constans SemanticaSymbolum**)
                        officina_xar_addere(symbola_parametrorum);
                    si (sedes_p != NIHIL)
                    {
                        *sedes_p = p_symbolum;
                    }
                }
            }
        }

        /* bloccus primus */
        d->bloccus = _bloccum_novum(d, "initium");

        /* parametra capta inscriptione aut aggregata -> arca +
         * scribere valoris incipientis */
        m = officina_xar_numerus(symbola_parametrorum);
        per (i = ZEPHYRUM; i < m; i++)
        {
            constans SemanticaSymbolum** sedes_p =
                (constans SemanticaSymbolum**)officina_xar_obtinere(
                    symbola_parametrorum, i);
            constans SemanticaSymbolum* p_symbolum;
                         DemissioSedes* sedes;
                                   s32  mt;

            si (sedes_p == NIHIL)
            {
                perge;
            }
            p_symbolum = *sedes_p;
            si (!_arcam_symboli(d, p_symbolum))
            {
                perge;
            }
            mt = _typus_medullae(p_symbolum->typus);
            si (mt < ZEPHYRUM)
            {
                /* aggregatum valore: registrum parametri IAM
                 * inscriptionem copiae vocantis tenet (conventio) -
                 * sedes vertitur, nulla arca nova */
                sedes = _sedem_symboli(d, p_symbolum);
                si (sedes != NIHIL)
                {
                    sedes->est_arca = VERUM;
                }
                perge;
            }
            sedes = _sedem_symboli(d, p_symbolum);
            {
                s32 mensura     = _mensura_typi(d, p_symbolum->typus);
                s32 inscriptio  = _registrum_temporarium(d);

                si (mensura <= ZEPHYRUM || sedes == NIHIL)
                {
                    perge;
                }
                (vacuum)_em(d, NIHIL, MEDULLA_OP_ARCA,
                    MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                    inscriptio,
                    medulla_op_immediatum((s64)mensura),
                    medulla_op_immediatum((s64)_exutus(
                        p_symbolum->typus)->ordinatio),
                    medulla_op_nihil());
                (vacuum)_em(d, NIHIL, MEDULLA_OP_SCRIBERE, mt,
                    MEDULLA_TYPUS_NIHIL, -I,
                    medulla_op_registrum(inscriptio),
                    medulla_op_registrum(sedes->index_registri),
                    medulla_op_nihil());
                sedes->index_registri  = inscriptio;
                sedes->est_arca        = VERUM;
            }
        }
    }

    /* locales: sedes + arca in initio */
    m = officina_xar_numerus(locales);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans DemissioLocale* locale =
            (constans DemissioLocale*)officina_xar_obtinere(locales, i);
        constans SemanticaSymbolum* symbolum_locale;

        si (locale == NIHIL)
        {
            perge;
        }
        symbolum_locale = locale->symbolum;
        si (_sedem_symboli(d, symbolum_locale) != NIHIL)
        {
            perge;   /* iam creatum */
        }
        si (_arcam_symboli(d, symbolum_locale))
        {
            s32 mensura = _mensura_completa(d,
                symbolum_locale->typus, locale->initiator);
            s32 ordinatio;
            s32 inscriptio;

            si (mensura <= ZEPHYRUM)
            {
                _sistere(d, nodus, "forma localis ignota");
                perge;
            }
            ordinatio = _ordinatio_typi(d, symbolum_locale->typus);
            inscriptio = _registrum_unicum(d,
                symbolum_locale->titulus);
            (vacuum)_em(d, NIHIL, MEDULLA_OP_ARCA,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                inscriptio, medulla_op_immediatum((s64)mensura),
                medulla_op_immediatum((s64)ordinatio),
                medulla_op_nihil());
            (vacuum)_sedem_creare(d, symbolum_locale, inscriptio,
                VERUM);
        }
        alioquin
        {
            s32 index = _registrum_unicum(d,
                symbolum_locale->titulus);

            (vacuum)_sedem_creare(d, symbolum_locale, index,
                FALSUM);
        }
    }

    /* parametra capta inscriptione: arca + copia valoris */
    {
        i32 n = officina_xar_numerus(d->functio->parametra);

        per (i = ZEPHYRUM; i < (i32)n; i++)
        {
            /* invenire symbolum parametri cuius sedes directa est
             * sed captum est */
            SilvaValor pd_ignotum;

            pd_ignotum.genus = SILVA_VALOR_NIHIL;
            (vacuum)pd_ignotum;
        }
    }

    /* corpus */
    _sententiam(d, corpus_v.datum.nodus);

    /* casus decidens: reditus implicitus */
    si (_bloccus_vivus(d))
    {
        si (mt_reditus >= ZEPHYRUM)
        {
            (vacuum)_em(d, NIHIL, MEDULLA_OP_REDDE,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I,
                medulla_op_immediatum(0), medulla_op_nihil(),
                medulla_op_nihil());
        }
        alioquin
        {
            (vacuum)_em(d, NIHIL, MEDULLA_OP_REDDE,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I,
                medulla_op_nihil(), medulla_op_nihil(),
                medulla_op_nihil());
        }
    }
    d->functio = NIHIL;
}


/* ==================================================
 * Radix
 * ================================================== */

/* data globalia: definitiones (initiatae aut tentativae) ->
 * MedullaDatum; declarationes purae (extern sine initiatore) et
 * prototypa praetermissa */
interior vacuum
_data_globalia (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    SilvaValor declaratores = silva_c89_declaratio_declaratores(
        nodus);
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(declaratores);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(declaratores,
            (unsigned int)i);
        constans SilvaNodus* elementum;
        constans SilvaNodus* initiator = NIHIL;
        SilvaToken* lexema;
        constans SemanticaSymbolum* symbolum;
        MedullaDatum* datum;
        s32 mensura;

        si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        elementum = _canonicus(v->datum.nodus);
        si (elementum->genus
            == (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS)
        {
            SilvaValor init_v =
                silva_c89_declarator_initiatus_initiator(elementum);

            si (init_v.genus == SILVA_VALOR_NODUS)
            {
                initiator = init_v.datum.nodus;
            }
        }
        lexema = silva_c89_declaratoris_titulus(elementum);
        symbolum = (lexema != NIHIL)
            ? _symbolum_lexematis(d, lexema) : NIHIL;
        si (   symbolum        == NIHIL
            || symbolum->genus != (int)SYMBOLUM_VARIABILE
            || symbolum->profunditas > ZEPHYRUM)
        {
            perge;   /* typedef/functio/enumerator/prototypum */
        }
        si (   initiator == NIHIL
            && (symbolum->repositio & REPOSITIO_EXTERNA)
                != ZEPHYRUM)
        {
            perge;   /* declaratio pura */
        }
        mensura = _mensura_completa(d, symbolum->typus, initiator);
        si (mensura <= ZEPHYRUM)
        {
            _notare(d, nodus, "forma dati globalis ignota");
            perge;
        }
        datum = _datum_invenire_aut_creare(d,
            _titulum_symboli(d, symbolum), mensura,
            _ordinatio_typi(d, symbolum->typus));
        si (datum == NIHIL)
        {
            _notare(d, nodus, "datum globale non creatum");
            perge;
        }
        si (   initiator != NIHIL
            && !_imaginem_scribere(d, datum, ZEPHYRUM,
                   symbolum->typus, initiator))
        {
            _notare(d, nodus, "initiator staticus inaestimabilis");
        }
    }
}

interior vacuum
_radicis_elementum (
               Demissio* d,
    constans SilvaNodus* nodus)
{
    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS)
    {
        _functionem(d, nodus);
        redde;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_DECLARATIO)
    {
        _data_globalia(d, nodus);
        redde;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_CONDITIONALIS)
    {
        SilvaValor rami = silva_c89_conditionalis_rami(nodus);
               i32 i;
               i32 m = (i32)silva_valor_lista_numerus(rami);

        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(rami, i);

            si (   v != NIHIL && v->genus == SILVA_VALOR_NODUS
                && v->datum.nodus->genus
                    == (s32)SILVA_C89_GENUS_RAMUS_SUMPTUS)
            {
                SilvaValor contentum =
                    silva_c89_ramus_sumptus_contentum(
                        v->datum.nodus);
                i32 k;
                i32 n = (i32)silva_valor_lista_numerus(contentum);

                per (k = ZEPHYRUM; k < n; k++)
                {
                    SilvaValor* e = silva_valor_lista_obtinere(
                        contentum, (unsigned int)k);

                    si (   e        != NIHIL
                        && e->genus == SILVA_VALOR_NODUS)
                    {
                        _radicis_elementum(d, e->datum.nodus);
                    }
                }
            }
        }
        redde;
    }
    /* declarationes (data statica M1b C), cetera: praetermissa */
}

MedullaModulus*
demissio_currere (
                  OfficinaPiscina* piscina,
    constans SilvaParsura* parsura,
           SilvaSemantica* sem,
                   OfficinaChorda  titulus_moduli)
{
    Demissio d;
         i32 i;
         i32 m;

    si (   piscina == NIHIL || parsura == NIHIL || sem == NIHIL
        || parsura->commissio == NIHIL)
    {
        redde NIHIL;
    }
    memset(&d, ZEPHYRUM, magnitudo(Demissio));
    d.piscina = piscina;
    d.sem = sem;
    d.stirps = _stirpem_computare(piscina, titulus_moduli);
    d.modulus = medulla_modulum_creare(piscina, titulus_moduli);
    d.bloccus = -I;
    d.frange_finis = -I;
    d.perge_finis = -I;
    d.reditus_index = -I;
    d.reditus_mensura = -I;
    si (d.modulus == NIHIL)
    {
        redde NIHIL;
    }
    /* piscina transitoria pro decodere (octeti statim in imagines
     * copiantur) */
    d.officina_piscina_silvae = silva_piscina_generare_dynamicum(
        "demissio_chordae", 8388608);
    /* tabula lexematum: lexema declarationis -> symbolum (sedes
     * declaratorum; usus per symbolum_nodi) */
    d.lexemata = officina_tabula_dispersa_creare_chorda(piscina, CCLVI);
    si (d.lexemata == NIHIL)
    {
        redde NIHIL;
    }
    m = (i32)silva_c89_symbola_numerus(sem);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaSymbolum* symbolum =
            silva_c89_symbolum_per_indicem(sem, (unsigned int)i);

        si (symbolum != NIHIL && symbolum->lexema != NIHIL)
        {
            _tabulam_ponere(&d, d.lexemata,
                (constans vacuum*)symbolum->lexema,
                _sine_constante((constans vacuum*)symbolum));
        }
    }

    /* radix */
    {
        SilvaValor radix  = parsura->commissio->radix;
               i32 n      =
                   (i32)silva_valor_lista_numerus(radix);

        per (i = ZEPHYRUM; i < n; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(radix,
                (unsigned int)i);

            si (v != NIHIL && v->genus == SILVA_VALOR_NODUS)
            {
                _radicis_elementum(&d, v->datum.nodus);
            }
        }
    }
    si (d.officina_piscina_silvae != NIHIL)
    {
        silva_piscina_destruere(d.officina_piscina_silvae);
    }
    redde d.modulus;
}


/* ==================================================
 * Distillatio linearum (M2a) - vide caput
 * ================================================== */

/* lexema primum verum subarboris (byte_offset -1 = syntheticum -
 * praetermittitur; fusor _linea_nodi cognatum) */
interior constans SilvaToken*
_lexema_primum (
    constans SilvaNodus* nodus,
                    i32  profunditas)
{
    i32 i;

    si (nodus == NIHIL || profunditas > XXXII)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < (i32)nodus->numerus_locorum; i++)
    {
        constans SilvaValor* v = &nodus->loci[i];

        si (   v->genus == SILVA_VALOR_TOKEN && v->datum.token != NIHIL
            && v->datum.token->byte_offset != -I)
        {
            redde v->datum.token;
        }
        si (v->genus == SILVA_VALOR_NODUS)
        {
            constans SilvaToken* lexema = _lexema_primum(
                v->datum.nodus, profunditas + I);

            si (lexema != NIHIL)
            {
                redde lexema;
            }
        }
    }
    redde NIHIL;
}

interior vacuum
_lineam_colligere (
    MedullaLineae* lineae,
    constans SilvaParsura* parsura,
    constans structura SilvaNodus* origo)
{
    constans SilvaToken* lexema;
                 OfficinaChorda  via;

    si (   origo == NIHIL
        || medulla_lineam_quaerere(lineae, origo, NIHIL, NIHIL))
    {
        redde;
    }
    lexema = _lexema_primum((constans SilvaNodus*)origo, ZEPHYRUM);
    si (lexema == NIHIL)
    {
        redde;
    }
    /* sedes RADICIS (invocationis), non scripturae (M3 chunk 2):
     * lexema expansum campos lexicales E CORPORE macronis fert -
     * linea definitionis. Radix catenae originis = invocatio in
     * plagula usoris; ea est linea quam relatio halitus et gressus
     * macro-conscius monstrare debent. Catena PLENA (acies
     * expansionis) ad indicium (chunk 3). */
    {
        unio { constans SilvaToken* c; SilvaToken* m; } ul;
        constans SilvaToken* radix;

        ul.c   = lexema;
        radix  = silva_token_radix(ul.m);
        si (radix != NIHIL)
        {
            lexema = radix;
        }
    }
    via.datum    = NIHIL;
    via.mensura  = ZEPHYRUM;
    si (parsura->expansio != NIHIL)
    {
        constans SilvaChorda* via_s = silva_fons_via(
            parsura->expansio, lexema->fons_index);

        si (via_s != NIHIL)
        {
            via = _ch_de_silva(*via_s);
        }
    }
    (vacuum)medulla_lineam_ponere(lineae, origo, via,
        (i32)lexema->linea);
}

MedullaLineae*
demissio_lineas_colligere (
                    OfficinaPiscina* piscina,
    constans MedullaModulus* modulus,
      constans SilvaParsura* parsura)
{
    MedullaLineae* lineae;
              i32  f;
              i32  numerus_functionum;
              i32  numerus_datorum;

    si (piscina == NIHIL || modulus == NIHIL || parsura == NIHIL)
    {
        redde NIHIL;
    }
    lineae = medulla_lineas_creare(piscina);
    si (lineae == NIHIL)
    {
        redde NIHIL;
    }

    numerus_functionum = officina_xar_numerus(modulus->functiones);
    per (f = ZEPHYRUM; f < numerus_functionum; f++)
    {
        constans MedullaFunctio* functio =
            *(MedullaFunctio**)officina_xar_obtinere(modulus->functiones,
                (i32)f);
        i32 b;
        i32 numerus_bloccorum = officina_xar_numerus(functio->blocci);

        _lineam_colligere(lineae, parsura, functio->origo);
        per (b = ZEPHYRUM; b < numerus_bloccorum; b++)
        {
            constans MedullaBloccus* bloccus =
                (constans MedullaBloccus*)officina_xar_obtinere(
                    functio->blocci, (i32)b);
            i32 n = officina_xar_numerus(bloccus->instructiones);
            i32 k;

            per (k = ZEPHYRUM; k < n; k++)
            {
                constans MedullaInstructio* instructio =
                    (constans MedullaInstructio*)officina_xar_obtinere(
                        bloccus->instructiones, (i32)k);

                _lineam_colligere(lineae, parsura,
                    instructio->origo);
            }
        }
    }
    numerus_datorum = officina_xar_numerus(modulus->data);
    per (f = ZEPHYRUM; f < numerus_datorum; f++)
    {
        constans MedullaDatum* datum =
            *(MedullaDatum**)officina_xar_obtinere(modulus->data, (i32)f);

        _lineam_colligere(lineae, parsura, datum->origo);
    }
    redde lineae;
}

/* ================= ex officina/fontes/officina_indicium.c ================= */


/* ==================================================
 * Typi scriptoris (privati)
 * ================================================== */

nomen structura {
    i32 instructio;                /* index planus intra functionem */
    i32 via;
    i32 linea;
    i32 profunditas;
    i32 nomen_macro;
} ScriptorLinea;

nomen structura {
    constans MedullaFunctio* functio;   /* clavis iuncturae */
    i32  titulus;                  /* chorda interna */
    OfficinaXar* lineae;                   /* ScriptorLinea valore */
    OfficinaXar* variabilia;               /* IndiciumVariabile valore */
    i32  instructiones_numerus;
} ScriptorFunctio;

structura IndiciumScriptor {
    OfficinaPiscina* piscina;
    OfficinaXar* chordae_datum;            /* i8 */
    OfficinaXar* chordae_offseta;          /* i32; N+1 introitus */
    OfficinaTabulaDispersa* chordae_tabula;
    OfficinaXar* viae;                     /* i32 (chorda) */
    OfficinaTabulaDispersa* viae_tabula;
    OfficinaXar* functiones;               /* ScriptorFunctio valore */
    OfficinaTabulaDispersa* functiones_tabula;
};

/* magnitudines elementorum - ORDO == IndiciumSectioGenus EXACTE
 * (asserta magnitudinum probationis derivam figunt) */
interior constans memoriae_index _elementi_magnitudines[] = {
    magnitudo(i8),                 /* CHORDAE_DATA */
    magnitudo(i32),                /* CHORDAE_OFFSETA */
    magnitudo(IndiciumVia),        /* VIAE */
    magnitudo(IndiciumModulus),    /* MODULI */
    magnitudo(IndiciumFunctio),    /* FUNCTIONES */
    magnitudo(i32),                /* LINEAE_INDICES */
    magnitudo(IndiciumLinea),      /* LINEAE_RES */
    magnitudo(i32),                /* RETRO_LINEAE */
    magnitudo(i32),                /* RETRO_INTERVALLA */
    magnitudo(IndiciumSitus),      /* RETRO_SITUS */
    magnitudo(IndiciumVariabile)   /* VARIABILIA */
};


/* ==================================================
 * Auxilia chordarum
 * ================================================== */

interior OfficinaChorda
_ch_vacua (vacuum)
{
    OfficinaChorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior OfficinaChorda
_ind_ch_de_silva (
    constans SilvaChorda* s)
{
    OfficinaChorda c;

    si (s == NIHIL || s->datum == NIHIL)
    {
        redde _ch_vacua();
    }
    c.datum    = (i8*)s->datum;
    c.mensura  = (i32)s->mensura;
    redde c;
}

interior OfficinaChorda
_ch_de_literis (
    constans character* literis)
{
    OfficinaChorda c;
    unio { constans character* c; i8* m; } u;

    u.c        = literis;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(literis);
    redde c;
}

interior b32
_ind_chordae_aequales (
    OfficinaChorda a,
    OfficinaChorda b)
{
    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }
    si (a.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (b32)(memcmp(a.datum, b.datum,
        (memoriae_index)a.mensura) == 0);
}

/* copia clavis in piscinam (tabula clavem non copiat) */
interior OfficinaChorda
_clavem_copiare (
    OfficinaPiscina* piscina,
     OfficinaChorda  c)
{
    OfficinaChorda copia;

    copia.datum = (i8*)officina_piscina_allocare(piscina,
        (memoriae_index)(c.mensura > ZEPHYRUM ? c.mensura : I));
    copia.mensura = ZEPHYRUM;
    si (copia.datum == NIHIL)
    {
        copia.datum = NIHIL;
        redde copia;
    }
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(copia.datum, c.datum, (memoriae_index)c.mensura);
    }
    copia.mensura = c.mensura;
    redde copia;
}


/* ==================================================
 * Internamentum chordarum (blob + offseta + dedup)
 * ================================================== */

interior i32
_internare (
    IndiciumScriptor* s,
              OfficinaChorda  c)
{
    vacuum* valor = NIHIL;
       i32  index;
       i32  i;

    si (c.mensura == ZEPHYRUM || c.datum == NIHIL)
    {
        redde ZEPHYRUM;   /* chorda nihili */
    }
    si (officina_tabula_dispersa_invenire(s->chordae_tabula, c, &valor))
    {
        redde (i32)(memoriae_index)valor;
    }
    index = officina_xar_numerus(s->chordae_offseta) - I;   /* chordae adhuc */
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        i8* locellus = officina_xar_addere(s->chordae_datum);

        si (locellus == NIHIL)
        {
            redde ZEPHYRUM;
        }
        *locellus = c.datum[i];
    }
    {
        i32* finis = officina_xar_addere(s->chordae_offseta);

        si (finis == NIHIL)
        {
            redde ZEPHYRUM;
        }
        *finis = officina_xar_numerus(s->chordae_datum);
    }
    (vacuum)officina_tabula_dispersa_inserere(s->chordae_tabula,
        _clavem_copiare(s->piscina, c),
        (vacuum*)(memoriae_index)index);
    redde index;
}

interior i32
_ind_viam_internare (
    IndiciumScriptor* s,
              OfficinaChorda  via)
{
    vacuum* valor = NIHIL;
       i32  index;

    si (via.mensura == ZEPHYRUM || via.datum == NIHIL)
    {
        redde ZEPHYRUM;
    }
    si (officina_tabula_dispersa_invenire(s->viae_tabula, via, &valor))
    {
        redde (i32)(memoriae_index)valor;
    }
    index = officina_xar_numerus(s->viae);
    {
        i32* locellus = officina_xar_addere(s->viae);

        si (locellus == NIHIL)
        {
            redde ZEPHYRUM;
        }
        *locellus = _internare(s, via);
    }
    (vacuum)officina_tabula_dispersa_inserere(s->viae_tabula,
        _clavem_copiare(s->piscina, via),
        (vacuum*)(memoriae_index)index);
    redde index;
}


/* ==================================================
 * Scriptor: creatio
 * ================================================== */

IndiciumScriptor*
indicium_scriptor_creare (
    OfficinaPiscina* piscina)
{
    IndiciumScriptor* s;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    s = officina_piscina_allocare(piscina, magnitudo(IndiciumScriptor));
    si (s == NIHIL)
    {
        redde NIHIL;
    }
    memset(s, ZEPHYRUM, magnitudo(IndiciumScriptor));
    s->piscina          = piscina;
    s->chordae_datum    = officina_xar_creare(piscina, (i32)magnitudo(i8));
    s->chordae_offseta  = officina_xar_creare(piscina, (i32)magnitudo(i32));
    s->chordae_tabula   = officina_tabula_dispersa_creare_chorda(piscina, 512);
    s->viae             = officina_xar_creare(piscina, (i32)magnitudo(i32));
    s->viae_tabula      = officina_tabula_dispersa_creare_chorda(piscina, 64);
    s->functiones = officina_xar_creare(piscina,
        (i32)magnitudo(ScriptorFunctio));
    s->functiones_tabula = officina_tabula_dispersa_creare_chorda(piscina,
        512);
    si (   s->chordae_datum     == NIHIL || s->chordae_offseta == NIHIL
        || s->chordae_tabula    == NIHIL || s->viae == NIHIL
        || s->viae_tabula       == NIHIL || s->functiones == NIHIL
        || s->functiones_tabula == NIHIL)
    {
        redde NIHIL;
    }
    /* chorda 0 = nihili: offseta {0, 0} */
    {
        i32* nulla = officina_xar_addere(s->chordae_offseta);
        i32* prima = officina_xar_addere(s->chordae_offseta);

        si (nulla == NIHIL || prima == NIHIL)
        {
            redde NIHIL;
        }
        *nulla = ZEPHYRUM;
        *prima = ZEPHYRUM;
    }
    redde s;
}


/* ==================================================
 * Collectio: acies expansionis (catena originis)
 * ================================================== */

/* lexema primum verum subarboris (par demissionis _ind_lexema_primum -
 * exscriptum consulto: demissio internum tenet; nota in worklog) */
interior constans SilvaToken*
_ind_lexema_primum (
    constans SilvaNodus* nodus,
                    i32  profunditas)
{
    i32 i;

    si (nodus == NIHIL || profunditas > XXXII)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < (i32)nodus->numerus_locorum; i++)
    {
        constans SilvaValor* v = &nodus->loci[i];

        si (   v->genus == SILVA_VALOR_TOKEN && v->datum.token != NIHIL
            && v->datum.token->byte_offset != -I)
        {
            redde v->datum.token;
        }
        si (v->genus == SILVA_VALOR_NODUS)
        {
            constans SilvaToken* lexema = _ind_lexema_primum(
                v->datum.nodus, profunditas + I);

            si (lexema != NIHIL)
            {
                redde lexema;
            }
        }
    }
    redde NIHIL;
}

interior constans SilvaToken*
_praedecessor_sedis_usus (
    constans SilvaToken* t)
{
    commutatio (silva_token_origo(t)->genus)
    {
    casus SILVA_ORIGO_EXPANSIO:
        redde silva_token_origo(t)->datum.expansio.invocatio;
    casus SILVA_ORIGO_PASTA:
        redde silva_token_origo(t)->datum.pasta.sinister;
    casus SILVA_ORIGO_CHORDA:
        redde silva_token_origo(t)->datum.stringificatio.primus;
    ordinarius:
        redde NIHIL;   /* FONS / API = radix */
    }
}

interior constans SilvaChorda*
_nomen_brachii (
    constans SilvaToken* t)
{
    commutatio (silva_token_origo(t)->genus)
    {
    casus SILVA_ORIGO_EXPANSIO:
        redde silva_token_origo(t)->datum.expansio.nomen_macro;
    casus SILVA_ORIGO_PASTA:
        redde silva_token_origo(t)->datum.pasta.nomen_macro;
    casus SILVA_ORIGO_CHORDA:
        redde silva_token_origo(t)->datum.stringificatio.nomen_macro;
    casus SILVA_ORIGO_API:
        redde silva_token_origo(t)->datum.api.nomen_macro;
    ordinarius:
        redde NIHIL;
    }
}

#define CATENA_MAXIMA 16

nomen structura {
    i32 initium;                   /* in sf->lineae */
    i32 numerus;
} AciesPrior;

interior vacuum
_aciem_colligere (
    IndiciumScriptor* s,
    ScriptorFunctio* sf,
    constans SilvaParsura* parsura,
    i32 index_planus,
    constans structura SilvaNodus* origo,
    AciesPrior* prior)
{
    constans SilvaToken* catena[CATENA_MAXIMA];
          ScriptorLinea  novae[CATENA_MAXIMA];
                    i32  numerus = ZEPHYRUM;
                    i32  k;
    constans SilvaToken* t;

    t = _ind_lexema_primum(origo, ZEPHYRUM);
    si (t == NIHIL || parsura->expansio == NIHIL)
    {
        redde;
    }
    dum (t != NIHIL && numerus < (i32)CATENA_MAXIMA)
    {
        catena[numerus] = t;
        numerus++;
        t = _praedecessor_sedis_usus(t);
    }
    /* radix prima (profunditas 0 = invocatio) */
    per (k = ZEPHYRUM; k < numerus; k++)
    {
         constans SilvaToken* gradus = catena[numerus - I - k];
        constans SilvaChorda* via_s = silva_fons_via(
            parsura->expansio, gradus->fons_index);

        novae[k].instructio = index_planus;
        novae[k].via = _ind_viam_internare(s, _ind_ch_de_silva(via_s));
        novae[k].linea = (i32)gradus->linea;
        novae[k].profunditas = k;
        novae[k].nomen_macro = _internare(s,
            _ind_ch_de_silva(_nomen_brachii(gradus)));
    }
    /* dedup contra aciem emissam priorem: eadem acies (praeter
     * instructio) = intervallum extenditur, nihil emittitur */
    si (prior->numerus == numerus && numerus > ZEPHYRUM)
    {
        b32 eadem = VERUM;

        per (k = ZEPHYRUM; k < numerus; k++)
        {
            constans ScriptorLinea* p = (constans ScriptorLinea*)
                officina_xar_obtinere(sf->lineae, prior->initium + k);

            si (   p->via != novae[k].via || p->linea != novae[k].linea
                || p->profunditas != novae[k].profunditas
                || p->nomen_macro != novae[k].nomen_macro)
            {
                eadem = FALSUM;
                frange;
            }
        }
        si (eadem)
        {
            redde;
        }
    }
    prior->initium = officina_xar_numerus(sf->lineae);
    prior->numerus = numerus;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        ScriptorLinea* locellus = officina_xar_addere(sf->lineae);

        si (locellus == NIHIL)
        {
            redde;
        }
        *locellus = novae[k];
    }
}


/* ==================================================
 * Collectio: variabilia (registra nominata x symbola semanticae)
 * ================================================== */

interior b32
_temporarium (
    OfficinaChorda titulus)
{
    i32 i;

    si (titulus.mensura < II || titulus.datum[ZEPHYRUM] != 't')
    {
        redde FALSUM;
    }
    per (i = I; i < titulus.mensura; i++)
    {
        si (titulus.datum[i] < '0' || titulus.datum[i] > '9')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_intra_nodum (
    constans SilvaNodus* nodus,
    constans structura SilvaNodus* maior)
{
    dum (nodus != NIHIL)
    {
        si (nodus == maior)
        {
            redde VERUM;
        }
        nodus = nodus->pater;
    }
    redde FALSUM;
}

interior constans character*
_primitivi_nomen (
    integer primitivum)
{
    interior constans character* constans NOMINA[] = {
        "void", "char", "signed char", "unsigned char",
        "short", "unsigned short", "int", "unsigned int",
        "long", "unsigned long", "long long",
        "unsigned long long", "float", "double", "long double"
    };

    si (   primitivum < 0
        || primitivum >= (integer)(magnitudo(NOMINA)
               / magnitudo(NOMINA[0])))
    {
        redde "?";
    }
    redde NOMINA[primitivum];
}

interior vacuum
_scripturae_adde (
             character* cella,
                   i32* cursor,
                   i32  capacitas,
    constans character* textus,
                   i32  mensura)
{
    i32 i;

    per (i = ZEPHYRUM; i < mensura && *cursor + I < capacitas; i++)
    {
        cella[*cursor] = textus[i];
        (*cursor)++;
    }
    cella[*cursor] = '\0';
}

interior vacuum
_typum_scribere (
    constans TypusC89* typus,
            character* cella,
                  i32* cursor,
                  i32  capacitas,
                  i32  profunditas)
{
    si (typus == NIHIL || profunditas > VIII)
    {
        _scripturae_adde(cella, cursor, capacitas, "?", I);
        redde;
    }
    commutatio (typus->genus)
    {
    casus TYPUS_C89_PRIMITIVUS:
    {
        constans character* n = _primitivi_nomen(
            typus->datum.primitivum);

        _scripturae_adde(cella, cursor, capacitas, n,
            (i32)strlen(n));
        frange;
    }
    casus TYPUS_C89_MONSTRATOR:
        _typum_scribere(typus->datum.monstrator.internum, cella,
            cursor, capacitas, profunditas + I);
        _scripturae_adde(cella, cursor, capacitas, "*", I);
        frange;
    casus TYPUS_C89_ACIES:
    {
        character numeri[XXXII];

        _typum_scribere(typus->datum.acies.elementum, cella,
            cursor, capacitas, profunditas + I);
        si (typus->datum.acies.numerus >= 0)
        {
            sprintf(numeri, "[%d]",
                (integer)typus->datum.acies.numerus);
        }
        alioquin
        {
            sprintf(numeri, "[]");
        }
        _scripturae_adde(cella, cursor, capacitas, numeri,
            (i32)strlen(numeri));
        frange;
    }
    casus TYPUS_C89_FUNCTIO:
        _typum_scribere(typus->datum.functio.reditus, cella,
            cursor, capacitas, profunditas + I);
        _scripturae_adde(cella, cursor, capacitas, " ()", III);
        frange;
    casus TYPUS_C89_STRUCTURA:
    casus TYPUS_C89_UNIO:
    {
        constans character* genus_nomen =
            (typus->genus == (integer)TYPUS_C89_STRUCTURA)
                ? "struct " : "union ";

        _scripturae_adde(cella, cursor, capacitas, genus_nomen,
            (i32)strlen(genus_nomen));
        si (typus->datum.tag.titulus.mensura > 0U)
        {
            _scripturae_adde(cella, cursor, capacitas,
                (constans character*)typus->datum.tag.titulus.datum,
                (i32)typus->datum.tag.titulus.mensura);
        }
        alioquin
        {
            _scripturae_adde(cella, cursor, capacitas, "?", I);
        }
        frange;
    }
    casus TYPUS_C89_ENUMERATUS:
        _scripturae_adde(cella, cursor, capacitas, "enum ", V);
        si (typus->datum.enumeratus.titulus.mensura > 0U)
        {
            _scripturae_adde(cella, cursor, capacitas,
                (constans character*)
                    typus->datum.enumeratus.titulus.datum,
                (i32)typus->datum.enumeratus.titulus.mensura);
        }
        alioquin
        {
            _scripturae_adde(cella, cursor, capacitas, "?", I);
        }
        frange;
    casus TYPUS_C89_QUALIFICATUS:
        si (typus->datum.qualificatus.quales
            & (insignatus integer)QUALIS_CONSTANS)
        {
            _scripturae_adde(cella, cursor, capacitas, "const ",
                VI);
        }
        si (typus->datum.qualificatus.quales
            & (insignatus integer)QUALIS_VOLATILIS)
        {
            _scripturae_adde(cella, cursor, capacitas,
                "volatile ", IX);
        }
        _typum_scribere(typus->datum.qualificatus.internum, cella,
            cursor, capacitas, profunditas + I);
        frange;
    ordinarius:
        _scripturae_adde(cella, cursor, capacitas, "?", I);
        frange;
    }
}

interior s32
_tag_de_typo (
    constans TypusC89* typus)
{
    si (typus == NIHIL)
    {
        redde (s32)INDICIUM_TYPUS_IGNOTUS;
    }
    si (typus->genus == (integer)TYPUS_C89_QUALIFICATUS)
    {
        redde _tag_de_typo(typus->datum.qualificatus.internum);
    }
    commutatio (typus->genus)
    {
    casus TYPUS_C89_MONSTRATOR:
    casus TYPUS_C89_ACIES:
    casus TYPUS_C89_FUNCTIO:
        redde MEDULLA_TYPUS_I64;   /* inscriptiones = verba */
    casus TYPUS_C89_ENUMERATUS:
        redde MEDULLA_TYPUS_S32;
    casus TYPUS_C89_PRIMITIVUS:
        commutatio (typus->datum.primitivum)
        {
        casus PRIMITIVUM_CHARACTER:            /* arm64 Apple:
                                                * char signatus */
        casus PRIMITIVUM_CHARACTER_SIGNATUM:
            redde MEDULLA_TYPUS_S8;
        casus PRIMITIVUM_CHARACTER_INSIGNATUM:
            redde MEDULLA_TYPUS_I8;
        casus PRIMITIVUM_BREVIS:
            redde MEDULLA_TYPUS_S16;
        casus PRIMITIVUM_BREVIS_INSIGNATUM:
            redde MEDULLA_TYPUS_I16;
        casus PRIMITIVUM_INTEGER:
            redde MEDULLA_TYPUS_S32;
        casus PRIMITIVUM_INTEGER_INSIGNATUM:
            redde MEDULLA_TYPUS_I32;
        casus PRIMITIVUM_LONGUS:
        casus PRIMITIVUM_LONGUS_LONGUS:
            redde MEDULLA_TYPUS_S64;
        casus PRIMITIVUM_LONGUS_INSIGNATUM:
        casus PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM:
            redde MEDULLA_TYPUS_I64;
        casus PRIMITIVUM_FLUITANS:
            redde MEDULLA_TYPUS_F32;
        casus PRIMITIVUM_DUPLEX:
        casus PRIMITIVUM_DUPLEX_LONGUS:
            redde MEDULLA_TYPUS_F64;
        ordinarius:
            redde (s32)INDICIUM_TYPUS_IGNOTUS;
        }
    ordinarius:
        redde (s32)INDICIUM_TYPUS_IGNOTUS;
    }
}

interior constans SemanticaSymbolum*
_symbolum_functionis (
    SilvaSemantica* sem,
    constans structura SilvaNodus* functionis_nodus,
    OfficinaChorda titulus)
{
    insignatus integer numerus;
    insignatus integer i;

    si (sem == NIHIL || functionis_nodus == NIHIL)
    {
        redde NIHIL;
    }
    numerus = silva_c89_symbola_numerus(sem);
    per (i = 0U; i < numerus; i++)
    {
        constans SemanticaSymbolum* symbolum =
            silva_c89_symbolum_per_indicem(sem, i);

        si (symbolum == NIHIL)
        {
            perge;
        }
        si (   symbolum->genus != (integer)SYMBOLUM_VARIABILE
            && symbolum->genus != (integer)SYMBOLUM_PARAMETRUM)
        {
            perge;
        }
        si (symbolum->profunditas == 0U)
        {
            perge;   /* scopus fili = datum, non registrum */
        }
        si (!_ind_chordae_aequales(titulus,
                _ind_ch_de_silva(&symbolum->titulus)))
        {
            perge;
        }
        si (!_intra_nodum(symbolum->declarans, functionis_nodus))
        {
            perge;
        }
        redde symbolum;   /* congruentia prima (umbrae: lacuna
                           * honesta documentata) */
    }
    redde NIHIL;
}

interior vacuum
_variabilia_colligere (
           IndiciumScriptor* s,
            ScriptorFunctio* sf,
    constans MedullaFunctio* functio,
             SilvaSemantica* sem)
{
    i32 numerus_registrorum   = officina_xar_numerus(functio->registra);
    i32 numerus_parametrorum  = officina_xar_numerus(functio->parametra);
    i32 r;

    per (r = ZEPHYRUM; r < numerus_registrorum; r++)
    {
        constans OfficinaChorda* titulus = (constans OfficinaChorda*)officina_xar_obtinere(
            functio->registra, r);
        IndiciumVariabile v;
                      i32 p;

        si (_temporarium(*titulus))
        {
            perge;
        }
        v.titulus         = _internare(s, *titulus);
        v.index_registri  = r;
        v.typus_medulla   = (s32)INDICIUM_TYPUS_IGNOTUS;
        v.typus_scriptus  = ZEPHYRUM;
        per (p = ZEPHYRUM; p < numerus_parametrorum; p++)
        {
            constans MedullaParametrum* parametrum =
                (constans MedullaParametrum*)officina_xar_obtinere(
                    functio->parametra, p);

            si (parametrum->index == (s32)r)
            {
                v.typus_medulla = parametrum->typus;
                frange;
            }
        }
        {
            constans SemanticaSymbolum* symbolum =
                _symbolum_functionis(sem, functio->origo,
                    *titulus);

            si (symbolum != NIHIL && symbolum->typus != NIHIL)
            {
                character littera[CXXVIII];
                      i32 cursor = ZEPHYRUM;

                littera[ZEPHYRUM] = '\0';
                _typum_scribere(symbolum->typus, littera, &cursor,
                    (i32)magnitudo(littera), ZEPHYRUM);
                v.typus_scriptus = _internare(s,
                    _ch_de_literis(littera));
                si (v.typus_medulla == (s32)INDICIUM_TYPUS_IGNOTUS)
                {
                    v.typus_medulla = _tag_de_typo(symbolum->typus);
                }
            }
        }
        {
            IndiciumVariabile* locellus = officina_xar_addere(sf->variabilia);

            si (locellus != NIHIL)
            {
                *locellus = v;
            }
        }
    }
}


/* ==================================================
 * Collectio: modulus
 * ================================================== */

b32
indicium_modulum_colligere (
           IndiciumScriptor* s,
    constans MedullaModulus* modulus,
      constans SilvaParsura* parsura,
             SilvaSemantica* sem)
{
    i32 numerus_functionum;
    i32 f;

    si (s == NIHIL || modulus == NIHIL || parsura == NIHIL)
    {
        redde FALSUM;
    }
    numerus_functionum = officina_xar_numerus(modulus->functiones);
    per (f = ZEPHYRUM; f < numerus_functionum; f++)
    {
        constans MedullaFunctio* functio =
            *(MedullaFunctio**)officina_xar_obtinere(modulus->functiones,
                (i32)f);
        ScriptorFunctio* sf;
             AciesPrior  prior;
                    i32  index_planus = ZEPHYRUM;
                    i32  b;
                    i32  numerus_bloccorum;

        /* aperire */
        {
            i8* clavis_octeti = officina_piscina_allocare(s->piscina,
                magnitudo(vacuum*));
            OfficinaChorda clavis;
               i32 sf_index = officina_xar_numerus(s->functiones);

            sf = officina_xar_addere(s->functiones);
            si (sf == NIHIL || clavis_octeti == NIHIL)
            {
                redde FALSUM;
            }
            memset(sf, ZEPHYRUM, magnitudo(ScriptorFunctio));
            sf->functio = functio;
            sf->titulus = _internare(s, functio->titulus);
            sf->lineae = officina_xar_creare(s->piscina,
                (i32)magnitudo(ScriptorLinea));
            sf->variabilia = officina_xar_creare(s->piscina,
                (i32)magnitudo(IndiciumVariabile));
            si (sf->lineae == NIHIL || sf->variabilia == NIHIL)
            {
                redde FALSUM;
            }
            memcpy(clavis_octeti, &functio, magnitudo(vacuum*));
            clavis.datum    = clavis_octeti;
            clavis.mensura  = (i32)magnitudo(vacuum*);
            (vacuum)officina_tabula_dispersa_inserere(s->functiones_tabula,
                clavis, (vacuum*)(memoriae_index)sf_index);
        }
        prior.initium      = ZEPHYRUM;
        prior.numerus      = ZEPHYRUM;
        numerus_bloccorum  = officina_xar_numerus(functio->blocci);
        per (b = ZEPHYRUM; b < numerus_bloccorum; b++)
        {
            constans MedullaBloccus* bloccus =
                medulla_bloccum_obtinere(functio, (s32)b);
            i32 n = officina_xar_numerus(bloccus->instructiones);
            i32 k;

            per (k = ZEPHYRUM; k < n; k++)
            {
                constans MedullaInstructio* instructio =
                    (constans MedullaInstructio*)officina_xar_obtinere(
                        bloccus->instructiones, (i32)k);

                si (instructio->origo != NIHIL)
                {
                    _aciem_colligere(s, sf, parsura, index_planus,
                        instructio->origo, &prior);
                }
                index_planus++;
            }
        }
        sf->instructiones_numerus = index_planus;
        si (sem != NIHIL)
        {
            _variabilia_colligere(s, sf, functio, sem);
        }
    }
    redde VERUM;
}


/* ==================================================
 * Scribere: iunctura ordine conexionis + retro + plagula
 * ================================================== */

nomen structura {
    i32 linea;
    i32 functio;
    i32 instructio;
} RetroTriplum;

interior integer
_tripla_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans RetroTriplum* ta = (constans RetroTriplum*)a;
    constans RetroTriplum* tb = (constans RetroTriplum*)b;

    si (ta->linea != tb->linea)
    {
        redde (ta->linea < tb->linea) ? -1 : 1;
    }
    si (ta->functio != tb->functio)
    {
        redde (ta->functio < tb->functio) ? -1 : 1;
    }
    si (ta->instructio != tb->instructio)
    {
        redde (ta->instructio < tb->instructio) ? -1 : 1;
    }
    redde 0;
}

interior i32
_instructiones_numerare (
    constans MedullaFunctio* functio)
{
    i32 summa = ZEPHYRUM;
    i32 b;
    i32 numerus_bloccorum;

    si (functio == NIHIL || functio->blocci == NIHIL)
    {
        redde ZEPHYRUM;
    }
    numerus_bloccorum = officina_xar_numerus(functio->blocci);
    per (b = ZEPHYRUM; b < numerus_bloccorum; b++)
    {
        constans MedullaBloccus* bloccus =
            medulla_bloccum_obtinere(functio, (s32)b);

        summa += officina_xar_numerus(bloccus->instructiones);
    }
    redde summa;
}

interior vacuum*
_xar_in_tabulam (
           OfficinaPiscina* piscina,
      constans OfficinaXar* xar,
    memoriae_index  elementi_octeti,
               i32* numerus_out)
{
       i32  numerus = officina_xar_numerus(xar);
    vacuum* tabula;

    *numerus_out = numerus;
    si (numerus == ZEPHYRUM)
    {
        redde NIHIL;
    }
    tabula = officina_piscina_allocare(piscina,
        (memoriae_index)numerus * elementi_octeti);
    si (tabula == NIHIL)
    {
        *numerus_out = ZEPHYRUM;
        redde NIHIL;
    }
    (vacuum)officina_xar_copiare_ad_tabulam(xar, tabula, ZEPHYRUM, numerus);
    redde tabula;
}

b32
indicium_scribere (
      IndiciumScriptor* s,
      constans Conexio* conexio,
    constans character* via)
{
    OfficinaXar* moduli_res;
    OfficinaXar* functiones_res;
    OfficinaXar* lineae_indices;
    OfficinaXar* lineae_res;
    OfficinaXar* variabilia_res;
    OfficinaXar* viae_res;
    OfficinaXar* retro_lineae;
    OfficinaXar* retro_intervalla;
    OfficinaXar* retro_situs;
    OfficinaXar* tripla_per_viam;          /* Xar* valore, per viam */
    i32  numerus_functionum;
    i32  numerus_modulorum;
    i32  numerus_viarum;
    i32  f;
    i32  m;
    i32  v;

    si (s == NIHIL || conexio == NIHIL || via == NIHIL)
    {
        redde FALSUM;
    }
    moduli_res = officina_xar_creare(s->piscina,
        (i32)magnitudo(IndiciumModulus));
    functiones_res = officina_xar_creare(s->piscina,
        (i32)magnitudo(IndiciumFunctio));
    lineae_indices = officina_xar_creare(s->piscina, (i32)magnitudo(i32));
    lineae_res = officina_xar_creare(s->piscina,
        (i32)magnitudo(IndiciumLinea));
    variabilia_res = officina_xar_creare(s->piscina,
        (i32)magnitudo(IndiciumVariabile));
    viae_res = officina_xar_creare(s->piscina, (i32)magnitudo(IndiciumVia));
    retro_lineae = officina_xar_creare(s->piscina, (i32)magnitudo(i32));
    retro_intervalla = officina_xar_creare(s->piscina, (i32)magnitudo(i32));
    retro_situs = officina_xar_creare(s->piscina,
        (i32)magnitudo(IndiciumSitus));
    tripla_per_viam = officina_xar_creare(s->piscina, (i32)magnitudo(OfficinaXar*));
    si (   moduli_res     == NIHIL || functiones_res == NIHIL
        || lineae_indices == NIHIL || lineae_res == NIHIL
        || variabilia_res == NIHIL || viae_res == NIHIL
        || retro_lineae   == NIHIL || retro_intervalla == NIHIL
        || retro_situs    == NIHIL || tripla_per_viam == NIHIL)
    {
        redde FALSUM;
    }

    /* moduli (ordo conexionis) */
    numerus_modulorum = (i32)conexio_numerus_modulorum(conexio);
    per (m = ZEPHYRUM; m < numerus_modulorum; m++)
    {
        constans MedullaModulus* modulus =
            conexio_modulum_obtinere(conexio, (s32)m);
        IndiciumModulus* locellus = officina_xar_addere(moduli_res);

        si (locellus == NIHIL)
        {
            redde FALSUM;
        }
        locellus->titulus = (modulus != NIHIL)
            ? _internare(s, modulus->titulus) : ZEPHYRUM;
    }

    /* sinus triplorum per viam (parati) */
    numerus_viarum = officina_xar_numerus(s->viae);
    per (v = ZEPHYRUM; v < numerus_viarum; v++)
    {
        OfficinaXar** locellus = officina_xar_addere(tripla_per_viam);

        si (locellus == NIHIL)
        {
            redde FALSUM;
        }
        *locellus = officina_xar_creare(s->piscina,
            (i32)magnitudo(RetroTriplum));
        si (*locellus == NIHIL)
        {
            redde FALSUM;
        }
    }

    /* functiones (ordo conexionis; iunctura per monstratorem) */
    numerus_functionum = (i32)conexio_numerus_functionum(conexio);
    per (f = ZEPHYRUM; f < numerus_functionum; f++)
    {
        constans ConexioFunctioNexa* nexa =
            conexio_functionem_obtinere(conexio, (s32)f);
                 IndiciumFunctio  ifu;
        constans ScriptorFunctio* sf = NIHIL;

        memset(&ifu, ZEPHYRUM, magnitudo(IndiciumFunctio));
        ifu.modulus = (i32)nexa->modulus_index;
        {
                i8  octeti[magnitudo(vacuum*)];
            OfficinaChorda  clavis;
            vacuum* valor = NIHIL;

            memcpy(octeti, &nexa->functio, magnitudo(octeti));
            clavis.datum    = octeti;
            clavis.mensura  = (i32)magnitudo(octeti);
            si (officina_tabula_dispersa_invenire(s->functiones_tabula,
                    clavis, &valor))
            {
                sf = (constans ScriptorFunctio*)officina_xar_obtinere(
                    s->functiones, (i32)(memoriae_index)valor);
            }
        }
        ifu.lineae_indices_primus  = officina_xar_numerus(lineae_indices);
        ifu.lineae_res_primus      = officina_xar_numerus(lineae_res);
        ifu.variabilia_prima       = officina_xar_numerus(variabilia_res);
        si (sf != NIHIL)
        {
            i32 n = officina_xar_numerus(sf->lineae);
            i32 k;

            ifu.titulus                = sf->titulus;
            ifu.instructiones_numerus  = sf->instructiones_numerus;
            ifu.lineae_numerus         = n;
            per (k = ZEPHYRUM; k < n; k++)
            {
                constans ScriptorLinea* sl =
                    (constans ScriptorLinea*)officina_xar_obtinere(
                        sf->lineae, k);
                i32* index_locellus = officina_xar_addere(lineae_indices);
                IndiciumLinea* res_locellus = officina_xar_addere(lineae_res);

                si (   index_locellus == NIHIL
                    || res_locellus   == NIHIL)
                {
                    redde FALSUM;
                }
                *index_locellus            = sl->instructio;
                res_locellus->via          = sl->via;
                res_locellus->linea        = sl->linea;
                res_locellus->profunditas  = sl->profunditas;
                res_locellus->nomen_macro  = sl->nomen_macro;
                /* radix -> triplum retro */
                si (sl->profunditas == ZEPHYRUM)
                {
                    OfficinaXar** sinus = (OfficinaXar**)officina_xar_obtinere(
                        tripla_per_viam, sl->via);
                    RetroTriplum* triplum = officina_xar_addere(*sinus);

                    si (triplum == NIHIL)
                    {
                        redde FALSUM;
                    }
                    triplum->linea       = sl->linea;
                    triplum->functio     = f;
                    triplum->instructio  = sl->instructio;
                }
            }
            {
                i32 k2 = officina_xar_numerus(sf->variabilia);
                i32 j;

                ifu.variabilia_numerus = k2;
                per (j = ZEPHYRUM; j < k2; j++)
                {
                    IndiciumVariabile* locellus =
                        officina_xar_addere(variabilia_res);

                    si (locellus == NIHIL)
                    {
                        redde FALSUM;
                    }
                    *locellus = *(IndiciumVariabile*)officina_xar_obtinere(
                        sf->variabilia, j);
                }
            }
        }
        alioquin
        {
            ifu.titulus = _internare(s, nexa->functio->titulus);
            ifu.instructiones_numerus =
                _instructiones_numerare(nexa->functio);
            ifu.lineae_numerus = ZEPHYRUM;
        }
        /* introitus claudens (semper) */
        {
            i32* claudens = officina_xar_addere(lineae_indices);

            si (claudens == NIHIL)
            {
                redde FALSUM;
            }
            *claudens = ifu.instructiones_numerus;
        }
        {
            IndiciumFunctio* locellus = officina_xar_addere(functiones_res);

            si (locellus == NIHIL)
            {
                redde FALSUM;
            }
            *locellus = ifu;
        }
    }

    /* retro: per viam, ordinata */
    per (v = ZEPHYRUM; v < numerus_viarum; v++)
    {
                 OfficinaXar** sinus = (OfficinaXar**)officina_xar_obtinere(tripla_per_viam, v);
                 i32   n;
        RetroTriplum*  tabula;
         IndiciumVia   iv;
                 i32   k;
                 i32   linea_prior = (i32)-I;   /* valor impossibilis */

        iv.titulus                  = *(i32*)officina_xar_obtinere(s->viae, v);
        iv.retro_lineae_primus      = officina_xar_numerus(retro_lineae);
        iv.retro_intervalla_primus  = officina_xar_numerus(retro_intervalla);
        iv.retro_numerus            = ZEPHYRUM;
        tabula = (RetroTriplum*)_xar_in_tabulam(s->piscina, *sinus,
            magnitudo(RetroTriplum), &n);
        si (n > ZEPHYRUM)
        {
            qsort(tabula, (memoriae_index)n,
                magnitudo(RetroTriplum), _tripla_comparare);
        }
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (tabula[k].linea != linea_prior)
            {
                i32* linea_locellus  = officina_xar_addere(retro_lineae);
                i32* intervallum     = officina_xar_addere(retro_intervalla);

                si (linea_locellus == NIHIL || intervallum == NIHIL)
                {
                    redde FALSUM;
                }
                *linea_locellus  = tabula[k].linea;
                *intervallum     = officina_xar_numerus(retro_situs);
                iv.retro_numerus++;
                linea_prior = tabula[k].linea;
            }
            {
                IndiciumSitus* situs = officina_xar_addere(retro_situs);

                si (situs == NIHIL)
                {
                    redde FALSUM;
                }
                situs->functio     = tabula[k].functio;
                situs->instructio  = tabula[k].instructio;
            }
        }
        {
            i32* clausura = officina_xar_addere(retro_intervalla);

            si (clausura == NIHIL)
            {
                redde FALSUM;
            }
            *clausura = officina_xar_numerus(retro_situs);
        }
        {
            IndiciumVia* locellus = officina_xar_addere(viae_res);

            si (locellus == NIHIL)
            {
                redde FALSUM;
            }
            *locellus = iv;
        }
    }

    /* plagula: caput + directorium + sectiones (8-ordinatae) */
    {
                vacuum* sectio_data[INDICIUM_SECTIO_NUMERUS];
                   i32  sectio_numeri[INDICIUM_SECTIO_NUMERUS];
         IndiciumCaput  caput;
        IndiciumSectio  directorium[INDICIUM_SECTIO_NUMERUS];
                   i32  cursor;
                   i32  g;
        FILE* plagula;

        sectio_data[INDICIUM_SECTIO_CHORDAE_DATA] =
            _xar_in_tabulam(s->piscina, s->chordae_datum,
                magnitudo(i8),
                &sectio_numeri[INDICIUM_SECTIO_CHORDAE_DATA]);
        sectio_data[INDICIUM_SECTIO_CHORDAE_OFFSETA] =
            _xar_in_tabulam(s->piscina, s->chordae_offseta,
                magnitudo(i32),
                &sectio_numeri[INDICIUM_SECTIO_CHORDAE_OFFSETA]);
        sectio_data[INDICIUM_SECTIO_VIAE] =
            _xar_in_tabulam(s->piscina, viae_res,
                magnitudo(IndiciumVia),
                &sectio_numeri[INDICIUM_SECTIO_VIAE]);
        sectio_data[INDICIUM_SECTIO_MODULI] =
            _xar_in_tabulam(s->piscina, moduli_res,
                magnitudo(IndiciumModulus),
                &sectio_numeri[INDICIUM_SECTIO_MODULI]);
        sectio_data[INDICIUM_SECTIO_FUNCTIONES] =
            _xar_in_tabulam(s->piscina, functiones_res,
                magnitudo(IndiciumFunctio),
                &sectio_numeri[INDICIUM_SECTIO_FUNCTIONES]);
        sectio_data[INDICIUM_SECTIO_LINEAE_INDICES] =
            _xar_in_tabulam(s->piscina, lineae_indices,
                magnitudo(i32),
                &sectio_numeri[INDICIUM_SECTIO_LINEAE_INDICES]);
        sectio_data[INDICIUM_SECTIO_LINEAE_RES] =
            _xar_in_tabulam(s->piscina, lineae_res,
                magnitudo(IndiciumLinea),
                &sectio_numeri[INDICIUM_SECTIO_LINEAE_RES]);
        sectio_data[INDICIUM_SECTIO_RETRO_LINEAE] =
            _xar_in_tabulam(s->piscina, retro_lineae,
                magnitudo(i32),
                &sectio_numeri[INDICIUM_SECTIO_RETRO_LINEAE]);
        sectio_data[INDICIUM_SECTIO_RETRO_INTERVALLA] =
            _xar_in_tabulam(s->piscina, retro_intervalla,
                magnitudo(i32),
                &sectio_numeri[INDICIUM_SECTIO_RETRO_INTERVALLA]);
        sectio_data[INDICIUM_SECTIO_RETRO_SITUS] =
            _xar_in_tabulam(s->piscina, retro_situs,
                magnitudo(IndiciumSitus),
                &sectio_numeri[INDICIUM_SECTIO_RETRO_SITUS]);
        sectio_data[INDICIUM_SECTIO_VARIABILIA] =
            _xar_in_tabulam(s->piscina, variabilia_res,
                magnitudo(IndiciumVariabile),
                &sectio_numeri[INDICIUM_SECTIO_VARIABILIA]);

        cursor = (i32)(magnitudo(IndiciumCaput)
            + (memoriae_index)INDICIUM_SECTIO_NUMERUS
                * magnitudo(IndiciumSectio));
        cursor = (cursor + VII) & ~(i32)VII;
        per (g = ZEPHYRUM; g < (i32)INDICIUM_SECTIO_NUMERUS; g++)
        {
            i32 octeti = (i32)((memoriae_index)sectio_numeri[g]
                * _elementi_magnitudines[g]);

            directorium[g].offset = cursor;
            directorium[g].magnitudo_octetorum = octeti;
            cursor = (cursor + octeti + VII) & ~(i32)VII;
        }
        memcpy(caput.magica, INDICIUM_MAGICA,
            magnitudo(caput.magica));
        caput.versio             = (i32)INDICIUM_VERSIO;
        caput.numerus_sectionum  = (i32)INDICIUM_SECTIO_NUMERUS;

        plagula = fopen(via, "wb");
        si (plagula == NIHIL)
        {
            redde FALSUM;
        }
        {
            b32 bene = VERUM;
            i32 scriptum = (i32)magnitudo(IndiciumCaput)
                + (i32)((memoriae_index)INDICIUM_SECTIO_NUMERUS
                    * magnitudo(IndiciumSectio));
            interior constans i8 ZERI[VIII] = { 0, 0, 0, 0,
                0, 0, 0, 0 };

            bene = bene && fwrite(&caput, magnitudo(IndiciumCaput),
                I, plagula) == (memoriae_index)I;
            bene = bene && fwrite(directorium,
                magnitudo(IndiciumSectio),
                (memoriae_index)INDICIUM_SECTIO_NUMERUS, plagula)
                    == (memoriae_index)INDICIUM_SECTIO_NUMERUS;
            per (g = ZEPHYRUM; bene
                && g < (i32)INDICIUM_SECTIO_NUMERUS; g++)
            {
                /* solum ordinatio: zeri usque ad offset */
                dum (bene && scriptum < directorium[g].offset)
                {
                    i32 restat = directorium[g].offset - scriptum;
                    i32 gradus = (restat < (i32)VIII)
                        ? restat : (i32)VIII;

                    bene = fwrite(ZERI, I, (memoriae_index)gradus,
                        plagula) == (memoriae_index)gradus;
                    scriptum += gradus;
                }
                si (   bene
                    && directorium[g].magnitudo_octetorum
                        > ZEPHYRUM)
                {
                    bene = fwrite(sectio_data[g], I,
                        (memoriae_index)
                            directorium[g].magnitudo_octetorum,
                        plagula) == (memoriae_index)
                            directorium[g].magnitudo_octetorum;
                    scriptum += directorium[g].magnitudo_octetorum;
                }
            }
            si (fclose(plagula) != 0)
            {
                bene = FALSUM;
            }
            redde bene;
        }
    }
}


/* ==================================================
 * Lector
 * ================================================== */

structura IndiciumLector {
    constans i8* datum;
            i32  mensura;
    constans i8* sectio_datum[INDICIUM_SECTIO_NUMERUS];
            i32  sectio_numerus[INDICIUM_SECTIO_NUMERUS];
};

IndiciumLector*
indicium_aperire (
               OfficinaPiscina* piscina,
    constans character* via)
{
    FILE* plagula;
    long mensura_l;
    i32 mensura;
    i8* datum;
    IndiciumLector* lector;
    constans IndiciumCaput* caput;
    constans IndiciumSectio* directorium;
    i32 g;

    si (piscina == NIHIL || via == NIHIL)
    {
        redde NIHIL;
    }
    plagula = fopen(via, "rb");
    si (plagula == NIHIL)
    {
        redde NIHIL;
    }
    fseek(plagula, 0L, SEEK_END);
    mensura_l = ftell(plagula);
    fseek(plagula, 0L, SEEK_SET);
    si (mensura_l < (long)(magnitudo(IndiciumCaput)
            + (memoriae_index)INDICIUM_SECTIO_NUMERUS
                * magnitudo(IndiciumSectio)))
    {
        fclose(plagula);
        redde NIHIL;
    }
    mensura  = (i32)mensura_l;
    datum    = (i8*)officina_piscina_allocare(piscina, (memoriae_index)mensura);
    si (   datum == NIHIL
        || fread(datum, I, (memoriae_index)mensura, plagula)
            != (memoriae_index)mensura)
    {
        fclose(plagula);
        redde NIHIL;
    }
    fclose(plagula);

    caput = (constans IndiciumCaput*)(constans vacuum*)datum;
    si (   memcmp(caput->magica, INDICIUM_MAGICA,
            magnitudo(caput->magica)) != 0
        || caput->versio != (i32)INDICIUM_VERSIO
        || caput->numerus_sectionum
            != (i32)INDICIUM_SECTIO_NUMERUS)
    {
        redde NIHIL;   /* magica aliena aut versio - dure reiecta */
    }
    directorium = (constans IndiciumSectio*)(constans vacuum*)
        (datum + magnitudo(IndiciumCaput));
    lector = officina_piscina_allocare(piscina, magnitudo(IndiciumLector));
    si (lector == NIHIL)
    {
        redde NIHIL;
    }
    lector->datum    = datum;
    lector->mensura  = mensura;
    per (g = ZEPHYRUM; g < (i32)INDICIUM_SECTIO_NUMERUS; g++)
    {
        constans IndiciumSectio* sectio = &directorium[g];

        /* clausulae "< 0" mortuae remotae (2026-07-17): agri i32
         * formati; summa in memoriae_index (64-bit) sine volutione
         * - cohibitio summae contra mensuram valores QUOSVIS capit */
        si (   (memoriae_index)sectio->offset
                + (memoriae_index)sectio->magnitudo_octetorum
                > (memoriae_index)mensura
            || (_elementi_magnitudines[g] > 0U
                && (memoriae_index)sectio->magnitudo_octetorum
                    % _elementi_magnitudines[g] != 0U))
        {
            redde NIHIL;
        }
        lector->sectio_datum[g] = datum + sectio->offset;
        lector->sectio_numerus[g] =
            (i32)((memoriae_index)sectio->magnitudo_octetorum
                / _elementi_magnitudines[g]);
    }
    redde lector;
}

OfficinaChorda
indicium_chorda (
    constans IndiciumLector* lector,
                        i32  index)
{
    constans i32* offseta;
             i32  numerus;

    si (lector == NIHIL)
    {
        redde _ch_vacua();
    }
    offseta = (constans i32*)(constans vacuum*)
        lector->sectio_datum[INDICIUM_SECTIO_CHORDAE_OFFSETA];
    numerus = lector->sectio_numerus[
        INDICIUM_SECTIO_CHORDAE_OFFSETA] - I;
    si (index >= numerus)
    {
        redde _ch_vacua();
    }
    {
        OfficinaChorda c;
        unio { constans i8* c; i8* m; } u;

        u.c = lector->sectio_datum[INDICIUM_SECTIO_CHORDAE_DATA]
            + offseta[index];
        c.datum    = u.m;
        c.mensura  = offseta[index + I] - offseta[index];
        redde c;
    }
}

OfficinaChorda
indicium_via_chorda (
    constans IndiciumLector* lector,
                        i32  via_index)
{
    constans IndiciumVia* viae;

    si (   lector    == NIHIL
        || via_index >= lector->sectio_numerus[INDICIUM_SECTIO_VIAE])
    {
        redde _ch_vacua();
    }
    viae = (constans IndiciumVia*)(constans vacuum*)
        lector->sectio_datum[INDICIUM_SECTIO_VIAE];
    redde indicium_chorda(lector, viae[via_index].titulus);
}

i32
indicium_functiones_numerus (
    constans IndiciumLector* lector)
{
    si (lector == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde lector->sectio_numerus[INDICIUM_SECTIO_FUNCTIONES];
}

constans IndiciumFunctio*
indicium_functio (
    constans IndiciumLector* lector,
                        i32  index)
{
    si (   lector == NIHIL || index
        >= lector->sectio_numerus[INDICIUM_SECTIO_FUNCTIONES])
    {
        redde NIHIL;
    }
    redde (constans IndiciumFunctio*)(constans vacuum*)
        lector->sectio_datum[INDICIUM_SECTIO_FUNCTIONES] + index;
}

s32
indicium_functionem_quaerere (
    constans IndiciumLector* lector,
                     OfficinaChorda  titulus)
{
    i32 numerus;
    i32 f;

    si (lector == NIHIL)
    {
        redde -I;
    }
    numerus = lector->sectio_numerus[INDICIUM_SECTIO_FUNCTIONES];
    per (f = ZEPHYRUM; f < numerus; f++)
    {
        constans IndiciumFunctio* ifu = indicium_functio(lector, f);

        si (_ind_chordae_aequales(titulus,
                indicium_chorda(lector, ifu->titulus)))
        {
            redde (s32)f;
        }
    }
    redde -I;
}

i32
indicium_lineas_de_instructione (
    constans IndiciumLector*  lector,
                        i32   functio_index,
                        i32   instructio,
     constans IndiciumLinea** acies_out)
{
    constans IndiciumFunctio* ifu;
                constans i32* indices;
      constans IndiciumLinea* res;
                         i32  n;
                         i32  imus;
                         i32  summus;
                         i32  primus;
                         i32  numerus;

    *acies_out  = NIHIL;
    ifu         = indicium_functio(lector, functio_index);
    si (ifu == NIHIL || ifu->lineae_numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    indices = (constans i32*)(constans vacuum*)
        lector->sectio_datum[INDICIUM_SECTIO_LINEAE_INDICES]
        + ifu->lineae_indices_primus;
    res = (constans IndiciumLinea*)(constans vacuum*)
        lector->sectio_datum[INDICIUM_SECTIO_LINEAE_RES]
        + ifu->lineae_res_primus;
    n = ifu->lineae_numerus;
    si (   instructio < indices[ZEPHYRUM]
        || instructio >= indices[n])   /* [n] = claudens */
    {
        redde ZEPHYRUM;
    }
    /* quaestio binaria: maximus i cum indices[i] <= instructio */
    imus    = ZEPHYRUM;
    summus  = n - I;
    dum (imus < summus)
    {
        i32 medius = imus + (summus - imus + I) / II;

        si (indices[medius] <= instructio)
        {
            imus = medius;
        }
        alioquin
        {
            summus = medius - I;
        }
    }
    /* acies acervata: introitus consecutivi eodem indice */
    primus = imus;
    dum (   primus > ZEPHYRUM
         && indices[primus - I] == indices[imus])
    {
        primus--;
    }
    numerus = I;
    dum (   primus + numerus < n
         && indices[primus + numerus] == indices[primus])
    {
        numerus++;
    }
    *acies_out = res + primus;
    redde numerus;
}

i32
indicium_situs_de_linea (
    constans IndiciumLector*  lector,
                     OfficinaChorda   via,
                        i32   linea,
     constans IndiciumSitus** acies_out)
{
    constans IndiciumVia* viae;
                     i32  numerus_viarum;
                     i32  v;

    *acies_out = NIHIL;
    si (lector == NIHIL)
    {
        redde ZEPHYRUM;
    }
    viae = (constans IndiciumVia*)(constans vacuum*)
        lector->sectio_datum[INDICIUM_SECTIO_VIAE];
    numerus_viarum = lector->sectio_numerus[INDICIUM_SECTIO_VIAE];
    per (v = ZEPHYRUM; v < numerus_viarum; v++)
    {
        constans i32* lineae;
        constans i32* intervalla;
                 i32  imus;
                 i32  summus;

        si (!_ind_chordae_aequales(via,
                indicium_chorda(lector, viae[v].titulus)))
        {
            perge;
        }
        si (viae[v].retro_numerus == ZEPHYRUM)
        {
            redde ZEPHYRUM;
        }
        lineae = (constans i32*)(constans vacuum*)
            lector->sectio_datum[INDICIUM_SECTIO_RETRO_LINEAE]
            + viae[v].retro_lineae_primus;
        intervalla = (constans i32*)(constans vacuum*)
            lector->sectio_datum[INDICIUM_SECTIO_RETRO_INTERVALLA]
            + viae[v].retro_intervalla_primus;
        imus    = ZEPHYRUM;
        summus  = viae[v].retro_numerus - I;
        dum (imus < summus)
        {
            i32 medius = imus + (summus - imus) / II;

            si (lineae[medius] < linea)
            {
                imus = medius + I;
            }
            alioquin
            {
                summus = medius;
            }
        }
        si (lineae[imus] != linea)
        {
            redde ZEPHYRUM;   /* congruentia exacta v0 */
        }
        *acies_out = (constans IndiciumSitus*)(constans vacuum*)
            lector->sectio_datum[INDICIUM_SECTIO_RETRO_SITUS]
            + intervalla[imus];
        redde intervalla[imus + I] - intervalla[imus];
    }
    redde ZEPHYRUM;
}

i32
indicium_variabilia_functionis (
       constans IndiciumLector*  lector,
                           i32   functio_index,
    constans IndiciumVariabile** acies_out)
{
    constans IndiciumFunctio* ifu;

    *acies_out  = NIHIL;
    ifu         = indicium_functio(lector, functio_index);
    si (ifu == NIHIL || ifu->variabilia_numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    *acies_out = (constans IndiciumVariabile*)(constans vacuum*)
        lector->sectio_datum[INDICIUM_SECTIO_VARIABILIA]
        + ifu->variabilia_prima;
    redde ifu->variabilia_numerus;
}
