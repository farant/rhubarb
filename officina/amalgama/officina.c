/* officina.c - GENERATUM (amalgamator) - NE MANU EDITES
 *
 * Medulla (IR registrorum) + forma textualis in plagula una
 * (SQLite modo). Fons veritatis: officina/fontes/ +
 * bibliothecae vendicatae in lib/. Regenerare:
 * officina/amalgamare.sh
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

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

#endif /* OFFICINA_H */

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

#define FILUM FILE

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
nomen structura OfficinaPiscinaNotatio {
    vacuum*        alveus_nunc;   /* Index ad alveum currentem */
    memoriae_index positus;       /* Offset in alveo */
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
 * =============================================== */

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
#ifndef CHORDA_H
#define CHORDA_H

static OfficinaChorda
officina_chorda_transcribere (
		 OfficinaChorda  s,
		OfficinaPiscina* piscina);


/* ==================================================
 * Divisio
 * ================================================= */

nomen structura {
	OfficinaChorda* elementa;
	i32     numerus;
} OfficinaChordaFissioFructus;

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
officina_xar_creare(
		OfficinaPiscina* piscina,
		 		i32  magnitudo_elementi);

/* Xar Creare Cum Vexillis
 * "Creare cum omnibus optionibus"
 */
static OfficinaXar*
officina_xar_creare_cum_vexillis(
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
officina_xar_obtinere(
		constans OfficinaXar* xar,
		         i32  index);

/* Xar Obtinere Signatum - Obtinere cum indice signato
 * "Pro iterationibus cum decrementis"
 *
 * Redde: Index ad elementum, vel NIHIL si invalidus vel negativus
 */
vacuum*
officina_xar_obtinere_s(
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
officina_xar_addere(
		OfficinaXar* xar);

/* ========================================================================
 * UTILITAS
 * ======================================================================== */

/* Xar Numerus - Obtinere numerum elementorum
 * "Obtinere numerum elementorum"
 */
i32
officina_xar_numerus(
		constans OfficinaXar* xar);

#endif /* XAR_H */

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

MedullaModulus* medulla_modulum_creare (OfficinaPiscina* piscina,
    OfficinaChorda titulus);

/* Internare per titulum: idem titulus -> idem index. Novum =
 * EXTERNUM; functionem/datum_creare genus concretum ponit. */
s32 medulla_symbolum_internare (MedullaModulus* modulus,
    OfficinaChorda titulus);
constans MedullaSymbolum* medulla_symbolum_obtinere (
    constans MedullaModulus* modulus, s32 index);

s32 medulla_causam_internare (MedullaModulus* modulus, OfficinaChorda causa);
constans OfficinaChorda* medulla_causam_obtinere (
    constans MedullaModulus* modulus, s32 index);

/* NIHIL si titulus iam definitionem concretam habet */
MedullaFunctio* medulla_functionem_creare (MedullaModulus* modulus,
    OfficinaChorda titulus, s32 typus_reditus, b32 est_variadica);

/* -> index registri novi (parametra = registra prima) */
s32 medulla_parametrum_addere (MedullaFunctio* functio,
    OfficinaChorda titulus, s32 typus);
/* titulus mensura 0 -> "tN" automaticum */
s32 medulla_registrum_novum (MedullaFunctio* functio, OfficinaChorda titulus);
constans OfficinaChorda* medulla_registrum_titulus (
    constans MedullaFunctio* functio, s32 index);

s32 medulla_bloccum_creare (MedullaFunctio* functio, OfficinaChorda titulus);
MedullaBloccus* medulla_bloccum_obtinere (
    constans MedullaFunctio* functio, s32 index);

/* argumenta vocationis in stivam -> index initii (extra_index) */
s32 medulla_operanda_addere (MedullaFunctio* functio,
    constans MedullaOperandum* operanda, s32 numerus);

/* fabricae operandorum (valore) */
MedullaOperandum medulla_op_nihil (vacuum);
MedullaOperandum medulla_op_registrum (s32 index);
MedullaOperandum medulla_op_immediatum (s64 valor);
MedullaOperandum medulla_op_immediatum_f (f64 valor);
MedullaOperandum medulla_op_symbolum (s32 index);
MedullaOperandum medulla_op_bloccum (s32 index);
MedullaOperandum medulla_op_causa (s32 index);

/* Custos disciplinae: FALSUM si bloccus invalidus/terminatus, op
 * extra fines, aut destinatio registrum ignotum. Terminator
 * bloccum claudit. Instructio VALORE copiatur. */
b32 medulla_emittere (MedullaFunctio* functio, s32 bloccus,
    constans MedullaInstructio* instructio);

b32 medulla_op_terminator (s32 op);
/* omnes blocci terminati et saltem unus */
b32 medulla_functio_terminata (constans MedullaFunctio* functio);

/* NIHIL si titulus iam definitionem concretam habet aut
 * magnitudo_octetorum < I. Imago allocatur et zephyrum-pletur. */
MedullaDatum* medulla_datum_creare (MedullaModulus* modulus,
    OfficinaChorda titulus, i32 magnitudo_octetorum, i32 ordinatio);
b32 medulla_datum_scribere (MedullaDatum* datum, i32 offset,
    constans i8* octeti, i32 numerus);
/* locellus relocationis = 8 octeti intra imaginem */
b32 medulla_relocationem_addere (MedullaDatum* datum, i32 offset,
    s32 symbolum);

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
constans character* medulla_op_titulus (s32 op);
constans character* medulla_typum_titulus (s32 typus);

OfficinaChorda medulla_textum_scribere (OfficinaPiscina* piscina,
    constans MedullaModulus* modulus);

/* NIHIL + linea_erroris (1-basis; NIHIL licet) in errore */
MedullaModulus* medulla_textum_legere (OfficinaPiscina* piscina,
    OfficinaChorda textus, i32* linea_erroris);

#endif /* OFFICINA_MEDULLA_TEXTUS_H */

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
               OfficinaPiscina* piscina,
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
    redde _allocare_interna(piscina, mensura, I, VERUM);
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

static OfficinaChorda
officina_chorda_transcribere (
     OfficinaChorda  s,
    OfficinaPiscina* piscina)
{
    OfficinaChorda  fructus;
        i8* allocatus;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)officina_piscina_allocare(piscina, s.mensura);
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

/* ================= ex lib/chorda_aedificator.c ================= */

/* ==================================================
 * Structura ChordaAedificator - Interna
 * ================================================== */

structura OfficinaChordaAedificator {
	           i8*  buffer;
	memoriae_index  capacitas;
	memoriae_index  offset;
	       OfficinaPiscina* piscina;
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

	buffer_novum = (i8*)officina_piscina_allocare(aedificator->piscina, capacitas_nova);
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
    OfficinaChordaAedificator* aedificator, 
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

interior memoriae_index
_format_integer_i32(
               i32  n, 
                i8* buffer, 
    memoriae_index  capacitas)
{
	     character cstr[CXXXII];
	           s32 mensura_signed;
	memoriae_index mensura;

	mensura_signed = snprintf(cstr, (memoriae_index)magnitudo(cstr), "%u", n);
	si (mensura_signed < ZEPHYRUM)
		redde ZEPHYRUM;

	mensura = (memoriae_index)mensura_signed;
	si (mensura >= capacitas)
		redde ZEPHYRUM;

	memcpy(buffer, cstr, mensura);
	redde mensura;
}


/* ==================================================
 * Creatio
 * ================================================== */

static OfficinaChordaAedificator*
officina_chorda_aedificator_creare(
           OfficinaPiscina* piscina, 
    memoriae_index capacitas_initialis)
{
	OfficinaChordaAedificator* aedificator;
	               i8* buffer;

	si (!piscina || capacitas_initialis == ZEPHYRUM)
		redde NIHIL;

	aedificator = (OfficinaChordaAedificator*)officina_piscina_allocare(
                                        piscina, 
                                        magnitudo(OfficinaChordaAedificator));
	si (!aedificator)
		redde NIHIL;

	buffer = (i8*)officina_piscina_allocare(piscina, capacitas_initialis);
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
officina_chorda_aedificator_appendere_character(
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
officina_chorda_aedificator_appendere_literis(
     OfficinaChordaAedificator* aedificator, 
    constans character* cstr)
{
	memoriae_index mensura;

	si (!aedificator || !cstr)
		redde FALSUM;

	mensura = strlen(cstr);
	redde _appendere_interna(aedificator, (constans i8*)cstr, mensura);
}

static b32
officina_chorda_aedificator_appendere_chorda(
    OfficinaChordaAedificator* aedificator, 
               OfficinaChorda s)
{
	si (!aedificator || !s.datum)
		redde FALSUM;

	redde _appendere_interna(aedificator, s.datum, s.mensura);
}

static b32
officina_chorda_aedificator_appendere_i32(
    OfficinaChordaAedificator* aedificator, 
                  i32  n)
{
	            i8 buffer[CXXXII];
	memoriae_index mensura;

	si (!aedificator)
		redde FALSUM;

	mensura = _format_integer_i32(n, buffer, magnitudo(buffer));
	si (mensura == ZEPHYRUM)
		redde FALSUM;

	redde _appendere_interna(aedificator, buffer, mensura);
}

static b32
officina_chorda_aedificator_appendere_repetita(
    OfficinaChordaAedificator* aedificator,
             character  c,
                   i32  numerus)
{
	i32 i;

	si (!aedificator || numerus < ZEPHYRUM)
		redde FALSUM;

	si (numerus == ZEPHYRUM)
		redde VERUM;

	per (i = ZEPHYRUM; i < numerus; i++)
	{
		si (!officina_chorda_aedificator_appendere_character(aedificator, c))
			redde FALSUM;
	}

	redde VERUM;
}

static OfficinaChorda
officina_chorda_aedificator_finire(
    OfficinaChordaAedificator* aedificator)
{
	OfficinaChorda result;

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
    OfficinaXar* xar,
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

/* ========================================================================
 * FUNCTIONES CREATIONIS
 * ======================================================================== */

/* Xar Creare */
static OfficinaXar*
officina_xar_creare(
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
officina_xar_creare_cum_vexillis(
    OfficinaPiscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi,
        i32  vexilla)
{
	OfficinaXar* xar;
	i32  i;

	si (!piscina || magnitudo_elementi == ZEPHYRUM)
    {
		redde NIHIL;
	}

	/* Allocare structuram xar ex piscina */
	xar = (OfficinaXar*)officina_piscina_allocare_ordinatum(piscina, magnitudo(OfficinaXar), magnitudo(OfficinaXar));
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
officina_xar_locare(
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
officina_xar_obtinere(
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
	redde basis + (locatio.offset_in_segmento * xar->magnitudo_elementi);
}

/* Xar Obtinere Signatum
 * "Pro iterationibus cum decrementis"
 */
vacuum*
officina_xar_obtinere_s(
    constans OfficinaXar* xar,
             s32  index)
{
	si (index < ZEPHYRUM) {
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
officina_xar_addere(
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
officina_xar_numerus(
    constans OfficinaXar* xar)
{
	redde xar ? xar->numerus_elementorum : ZEPHYRUM;
}

/* ================= ex officina/fontes/officina_medulla.c ================= */

/* ==================================================
 * Auxilia interna
 * ================================================== */

interior b32
_chordae_aequales (OfficinaChorda a, OfficinaChorda b)
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
medulla_modulum_creare (OfficinaPiscina* piscina, OfficinaChorda titulus)
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
    si (modulus->functiones == NIHIL || modulus->data == NIHIL
        || modulus->symbola == NIHIL || modulus->causae == NIHIL)
    {
        redde NIHIL;
    }
    redde modulus;
}

s32
medulla_symbolum_internare (MedullaModulus* modulus, OfficinaChorda titulus)
{
    i32 i;
    i32 m;
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

        si (symbolum != NIHIL
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
medulla_symbolum_obtinere (constans MedullaModulus* modulus,
    s32 index)
{
    si (modulus == NIHIL)
    {
        redde NIHIL;
    }
    redde (constans MedullaSymbolum*)officina_xar_obtinere_s(
        modulus->symbola, index);
}

s32
medulla_causam_internare (MedullaModulus* modulus, OfficinaChorda causa)
{
    i32 i;
    i32 m;
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
medulla_causam_obtinere (constans MedullaModulus* modulus, s32 index)
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
medulla_functionem_creare (MedullaModulus* modulus, OfficinaChorda titulus,
    s32 typus_reditus, b32 est_variadica)
{
    MedullaFunctio*  functio;
    MedullaFunctio** locus_functionis;
    MedullaSymbolum* symbolum;
    s32 index_symboli;

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
    functio->titulus = symbolum->titulus;   /* copia internata */
    functio->typus_reditus = typus_reditus;
    functio->est_variadica = est_variadica;
    functio->parametra = officina_xar_creare(modulus->piscina,
        (i32)magnitudo(MedullaParametrum));
    functio->blocci = officina_xar_creare(modulus->piscina,
        (i32)magnitudo(MedullaBloccus));
    functio->registra = officina_xar_creare(modulus->piscina,
        (i32)magnitudo(OfficinaChorda));
    functio->operanda = officina_xar_creare(modulus->piscina,
        (i32)magnitudo(MedullaOperandum));
    si (functio->parametra == NIHIL || functio->blocci == NIHIL
        || functio->registra == NIHIL || functio->operanda == NIHIL)
    {
        redde NIHIL;
    }

    locus_functionis = (MedullaFunctio**)officina_xar_addere(
        modulus->functiones);
    si (locus_functionis == NIHIL)
    {
        redde NIHIL;
    }
    *locus_functionis = functio;
    symbolum->genus = MEDULLA_SYMBOLUM_FUNCTIO;
    symbolum->index = (s32)(officina_xar_numerus(modulus->functiones) - I);
    redde functio;
}

s32
medulla_registrum_novum (MedullaFunctio* functio, OfficinaChorda titulus)
{
    OfficinaChorda* sedes;
    s32 index;

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

        temporarium.datum = (i8*)littera;
        temporarium.mensura = (i32)longitudo;
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
medulla_parametrum_addere (MedullaFunctio* functio, OfficinaChorda titulus,
    s32 typus)
{
    MedullaParametrum* parametrum;
    s32 index;

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
medulla_registrum_titulus (constans MedullaFunctio* functio,
    s32 index)
{
    si (functio == NIHIL)
    {
        redde NIHIL;
    }
    redde (constans OfficinaChorda*)officina_xar_obtinere_s(functio->registra, index);
}

s32
medulla_bloccum_creare (MedullaFunctio* functio, OfficinaChorda titulus)
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
medulla_bloccum_obtinere (constans MedullaFunctio* functio,
    s32 index)
{
    si (functio == NIHIL)
    {
        redde NIHIL;
    }
    redde (MedullaBloccus*)officina_xar_obtinere_s(functio->blocci, index);
}

s32
medulla_operanda_addere (MedullaFunctio* functio,
    constans MedullaOperandum* operanda, s32 numerus)
{
    s32 initium;
    s32 i;

    si (functio == NIHIL || numerus < ZEPHYRUM
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
_operandum (s32 genus)
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
medulla_op_registrum (s32 index)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_REGISTRUM);

    operandum.datum.index = index;
    redde operandum;
}

MedullaOperandum
medulla_op_immediatum (s64 valor)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_IMMEDIATUM);

    operandum.datum.immediatum = valor;
    redde operandum;
}

MedullaOperandum
medulla_op_immediatum_f (f64 valor)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_IMMEDIATUM_F);

    operandum.datum.immediatum_f = valor;
    redde operandum;
}

MedullaOperandum
medulla_op_symbolum (s32 index)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_SYMBOLUM);

    operandum.datum.index = index;
    redde operandum;
}

MedullaOperandum
medulla_op_bloccum (s32 index)
{
    MedullaOperandum operandum =
        _operandum(MEDULLA_OPERANDUM_BLOCCUS);

    operandum.datum.index = index;
    redde operandum;
}

MedullaOperandum
medulla_op_causa (s32 index)
{
    MedullaOperandum operandum = _operandum(MEDULLA_OPERANDUM_CAUSA);

    operandum.datum.index = index;
    redde operandum;
}

/* ==================================================
 * Emissio + disciplina terminatorum
 * ================================================== */

b32
medulla_op_terminator (s32 op)
{
    redde op == (s32)MEDULLA_OP_SALIRE
        || op == (s32)MEDULLA_OP_RAMUS
        || op == (s32)MEDULLA_OP_REDDE
        || op == (s32)MEDULLA_OP_SISTERE;
}

b32
medulla_emittere (MedullaFunctio* functio, s32 bloccus,
    constans MedullaInstructio* instructio)
{
    MedullaBloccus*    sedes_blocci;
    MedullaInstructio* sedes;

    si (functio == NIHIL || instructio == NIHIL)
    {
        redde FALSUM;
    }
    si (instructio->op < ZEPHYRUM
        || instructio->op >= (s32)MEDULLA_OP_NUMERUS)
    {
        redde FALSUM;
    }
    si (instructio->destinatio >= ZEPHYRUM
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
medulla_functio_terminata (constans MedullaFunctio* functio)
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
medulla_datum_creare (MedullaModulus* modulus, OfficinaChorda titulus,
    i32 magnitudo_octetorum, i32 ordinatio)
{
    MedullaDatum*    datum;
    MedullaDatum**   locus_dati;
    MedullaSymbolum* symbolum;
    s32 index_symboli;

    si (modulus == NIHIL || titulus.mensura == ZEPHYRUM
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
    datum->titulus = symbolum->titulus;
    datum->magnitudo_octetorum = magnitudo_octetorum;
    datum->ordinatio = ordinatio;
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
    *locus_dati = datum;
    symbolum->genus = MEDULLA_SYMBOLUM_DATUM;
    symbolum->index = (s32)(officina_xar_numerus(modulus->data) - I);
    redde datum;
}

b32
medulla_datum_scribere (MedullaDatum* datum, i32 offset,
    constans i8* octeti, i32 numerus)
{
    si (datum == NIHIL || octeti == NIHIL)
    {
        redde FALSUM;
    }
    si (offset > datum->magnitudo_octetorum
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
medulla_relocationem_addere (MedullaDatum* datum, i32 offset,
    s32 symbolum)
{
    MedullaRelocatio* relocatio;

    si (datum == NIHIL || symbolum < ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (datum->magnitudo_octetorum < VIII || offset > datum->magnitudo_octetorum - VIII)
    {
        redde FALSUM;   /* locellus 8 octetorum extra imaginem */
    }
    relocatio = (MedullaRelocatio*)officina_xar_addere(datum->relocationes);
    si (relocatio == NIHIL)
    {
        redde FALSUM;
    }
    relocatio->offset = offset;
    relocatio->symbolum = symbolum;
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
medulla_op_titulus (s32 op)
{
    si (op < ZEPHYRUM || op >= (s32)MEDULLA_OP_NUMERUS)
    {
        redde NIHIL;
    }
    redde OP_NOMINA[op];
}

constans character*
medulla_typum_titulus (s32 typus)
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
_chordam_evadere (OfficinaChordaAedificator* aed, OfficinaChorda textus)
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
_s64_scribere (OfficinaChordaAedificator* aed, s64 valor)
{
    character littera[XXXII];

    sprintf(littera, "%lld", valor);
    officina_chorda_aedificator_appendere_literis(aed, littera);
}

interior vacuum
_operandum_scribere (OfficinaChordaAedificator* aed,
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

            sprintf(littera, "%.17g",
                operandum->datum.immediatum_f);
            officina_chorda_aedificator_appendere_literis(aed, littera);
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
_instructionem_scribere (OfficinaChordaAedificator* aed,
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
        b32 primum = VERUM;
        s32 i;

        ordo[ZEPHYRUM] = &instructio->a;
        ordo[I] = &instructio->b;
        ordo[II] = &instructio->c;
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
_functionem_scribere (OfficinaChordaAedificator* aed,
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
_datum_scribere (OfficinaChordaAedificator* aed,
    constans MedullaModulus* modulus, constans MedullaDatum* datum)
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
medulla_textum_scribere (OfficinaPiscina* piscina,
    constans MedullaModulus* modulus)
{
    OfficinaChordaAedificator* aed;
    OfficinaChorda vacua;
    i32 i;
    i32 m;

    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;
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
    i32    numerus;              /* 1-basis */
} TextusLinea;

nomen structura {
    OfficinaPiscina*        piscina;
    OfficinaXar*            lineae;      /* TextusLinea valore */
    i32             positio;
    MedullaModulus* modulus;
    MedullaFunctio* functio;     /* contextus currens */
    s32             bloccus;     /* -I extra bloccum */
    MedullaDatum*   datum;
    i32             offset_dati;
    i32             linea_erroris;
} TextusLector;

interior b32
_est_littera (character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9') || c == '_' || c == '.';
}

interior b32
_est_cifra (character c)
{
    redde c >= '0' && c <= '9';
}

interior vacuum
_spatia_praeterire (OfficinaChorda linea, i32* cursor)
{
    dum (*cursor < linea.mensura
         && (character)linea.datum[*cursor] == ' ')
    {
        (*cursor)++;
    }
}

interior character
_aspicere (OfficinaChorda linea, i32 cursor)
{
    si (cursor >= linea.mensura)
    {
        redde '\0';
    }
    redde (character)linea.datum[cursor];
}

interior OfficinaChorda
_verbum_legere (OfficinaChorda linea, i32* cursor)
{
    OfficinaChorda verbum;
    i32 initium = *cursor;

    dum (*cursor < linea.mensura
         && _est_littera((character)linea.datum[*cursor]))
    {
        (*cursor)++;
    }
    verbum.datum = linea.datum + initium;
    verbum.mensura = *cursor - initium;
    redde verbum;
}

interior b32
_verbum_aequale (OfficinaChorda verbum, constans character* literis)
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
_s64_legere (OfficinaChorda linea, i32* cursor, s64* valor_out)
{
    b32 negativum = FALSUM;
    s64 valor = ZEPHYRUM;   /* accumulatio NEGATIVA (S64_MIN tuta) */
    b32 ullus = FALSUM;

    si (_aspicere(linea, *cursor) == '-')
    {
        negativum = VERUM;
        (*cursor)++;
    }
    dum (*cursor < linea.mensura
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
_chordam_citatam_legere (TextusLector* lector, OfficinaChorda linea, i32* cursor,
    OfficinaChorda* fructus)
{
    i8* datum;
    i32 scriptum = ZEPHYRUM;

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
            fructus->datum = datum;
            fructus->mensura = scriptum;
            redde VERUM;
        }
        si (c == '\\')
        {
            character f;

            (*cursor)++;
            f = _aspicere(linea, *cursor);
            si (f == 'n')       { datum[scriptum] = (i8)'\n'; }
            alioquin si (f == 't')  { datum[scriptum] = (i8)'\t'; }
            alioquin si (f == '"')  { datum[scriptum] = (i8)'"'; }
            alioquin si (f == '\\') { datum[scriptum] = (i8)'\\'; }
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
_registrum_invenire_aut_creare (MedullaFunctio* functio,
    OfficinaChorda titulus)
{
    i32 i;
    i32 m = officina_xar_numerus(functio->registra);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans OfficinaChorda* sedes =
            (constans OfficinaChorda*)officina_xar_obtinere(functio->registra, i);

        si (sedes->mensura == titulus.mensura
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
_bloccum_invenire (constans MedullaFunctio* functio, OfficinaChorda titulus)
{
    i32 i;
    i32 m = officina_xar_numerus(functio->blocci);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MedullaBloccus* bloccus =
            (constans MedullaBloccus*)officina_xar_obtinere(functio->blocci,
                i);

        si (bloccus->titulus.mensura == titulus.mensura
            && memcmp(bloccus->titulus.datum, titulus.datum,
                   (memoriae_index)titulus.mensura) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

interior s32
_op_invenire (OfficinaChorda verbum)
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
_typum_invenire (OfficinaChorda verbum)
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
_numerum_legere (OfficinaChorda linea, i32* cursor, MedullaOperandum* out)
{
    i32 finis = *cursor;
    b32 fluitat = FALSUM;

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

        si (longitudo <= ZEPHYRUM
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
_operandum_legere (TextusLector* lector, OfficinaChorda linea, i32* cursor,
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
        verbum = _verbum_legere(linea, cursor);
        index = medulla_symbolum_internare(lector->modulus, verbum);
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
_finis_lineae (OfficinaChorda linea, i32 cursor)
{
    _spatia_praeterire(linea, &cursor);
    redde cursor >= linea.mensura;
}

interior b32
_instructionem_legere (TextusLector* lector, OfficinaChorda linea)
{
    i32 cursor = ZEPHYRUM;
    MedullaInstructio instructio;
    OfficinaChorda verbum;

    memset(&instructio, ZEPHYRUM, magnitudo(MedullaInstructio));
    instructio.typus = MEDULLA_TYPUS_NIHIL;
    instructio.typus_secundus = MEDULLA_TYPUS_NIHIL;
    instructio.destinatio = -I;
    instructio.extra_index = -I;
    instructio.a = medulla_op_nihil();
    instructio.b = medulla_op_nihil();
    instructio.c = medulla_op_nihil();

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

            pars.datum = verbum.datum + punctum_a + I;
            pars.mensura = finis_partis - (i32)punctum_a - I;
            instructio.typus = _typum_invenire(pars);
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
        si (_aspicere(linea, cursor) != '-'
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
_bloccos_praescandere (TextusLector* lector)
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
            si (verbum.mensura == ZEPHYRUM
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

            si (_verbum_aequale(verbum, "functio")
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
_functionem_legere (TextusLector* lector, OfficinaChorda linea, i32 cursor)
{
    OfficinaChorda titulus;
    s32 typi_parametrorum[XXXII];
    OfficinaChorda tituli_parametrorum[XXXII];
    s32 numerus_parametrorum = ZEPHYRUM;
    b32 variadica = FALSUM;
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
            si (_aspicere(linea, cursor + I) != '.'
                || _aspicere(linea, cursor + II) != '.')
            {
                redde FALSUM;
            }
            cursor += III;
            variadica = VERUM;
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
            OfficinaChorda verbum = _verbum_legere(linea, &cursor);
            s32 typus = _typum_invenire(verbum);

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
    si (_aspicere(linea, cursor) != '-'
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
    lector->datum = NIHIL;
    lector->bloccus = -I;
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
_datum_legere (TextusLector* lector, OfficinaChorda linea, i32 cursor)
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
    si (!_s64_legere(linea, &cursor, &ordinatio)
        || !_finis_lineae(linea, cursor))
    {
        redde FALSUM;
    }
    lector->datum = medulla_datum_creare(lector->modulus, titulus,
        (i32)magnitudo_octetorum, (i32)ordinatio);
    lector->functio = NIHIL;
    lector->bloccus = -I;
    lector->offset_dati = ZEPHYRUM;
    redde lector->datum != NIHIL;
}

interior s32
_cifra_hex (character c)
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
_octetos_legere (TextusLector* lector, OfficinaChorda linea, i32 cursor)
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
        altum = _cifra_hex(_aspicere(linea, cursor));
        imum = _cifra_hex(_aspicere(linea, cursor + I));
        si (altum < ZEPHYRUM || imum < ZEPHYRUM
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
_relocationem_legere (TextusLector* lector, OfficinaChorda linea, i32 cursor)
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
_lineam_tractare (TextusLector* lector, constans TextusLinea* linea)
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
        si (_aspicere(linea->textus, cursor) != ':'
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
            si (!_chordam_citatam_legere(lector, linea->textus,
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
medulla_textum_legere (OfficinaPiscina* piscina, OfficinaChorda textus,
    i32* linea_erroris)
{
    TextusLector lector;
    i32 i;
    i32 m;
    i32 initium = ZEPHYRUM;
    i32 numerus_lineae = I;

    si (linea_erroris != NIHIL)
    {
        *linea_erroris = ZEPHYRUM;
    }
    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    memset(&lector, ZEPHYRUM, magnitudo(TextusLector));
    lector.piscina = piscina;
    lector.bloccus = -I;
    lector.lineae = officina_xar_creare(piscina, (i32)magnitudo(TextusLinea));
    si (lector.lineae == NIHIL)
    {
        redde NIHIL;
    }

    /* findere in lineas; vacuas et commenta (;) praetermittere */
    per (i = ZEPHYRUM; i <= textus.mensura; i++)
    {
        si (i == textus.mensura
            || (character)textus.datum[i] == '\n')
        {
            OfficinaChorda contenta;
            i32 cursor = ZEPHYRUM;

            contenta.datum = textus.datum + initium;
            contenta.mensura = i - initium;
            _spatia_praeterire(contenta, &cursor);
            si (cursor < contenta.mensura
                && (character)contenta.datum[cursor] != ';')
            {
                TextusLinea* sedes = (TextusLinea*)officina_xar_addere(lector.lineae);

                si (sedes == NIHIL)
                {
                    redde NIHIL;
                }
                sedes->textus = contenta;
                sedes->numerus = numerus_lineae;
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
