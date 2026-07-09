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

static s32
officina_chorda_comparare (
		OfficinaChorda a,
		OfficinaChorda b);

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
officina_friatio_fnv1a(
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
MedullaModulus* demissio_currere (OfficinaPiscina* piscina,
    constans SilvaParsura* parsura, SilvaSemantica* sem,
    OfficinaChorda titulus_moduli);

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

/* ================= ex lib/friatio.c ================= */

/* ==================================================
 * FNV-1a (Fowler-Noll-Vo)
 * ================================================== */

/* FNV-1a constants (32-bit) */
#define FNV_OFFSET_BASIS 2166136261U
#define FNV_PRIME        16777619U

static i32
officina_friatio_fnv1a(
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
officina_tabula_friare_fnv1a(OfficinaChorda clavis)
{
    redde officina_friatio_fnv1a(clavis);
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
    OfficinaTabulaDispersa* tabula,
            OfficinaChorda  clavis,
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
_tabula_dispersa_crescere(OfficinaTabulaDispersa* tabula)
{
    Slotus* sloti_veteres;
       i32  capacitas_vetus;
       i32  i;

    sloti_veteres   = tabula->sloti;
    capacitas_vetus = tabula->capacitas;

    tabula->capacitas      = capacitas_vetus * II;
    tabula->capacitas_mask = tabula->capacitas - I;

    tabula->sloti = (Slotus*)officina_piscina_allocare(
        tabula->piscina,
        tabula->capacitas * magnitudo(Slotus));

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
officina_tabula_dispersa_creare(
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


static OfficinaTabulaDispersa*
officina_tabula_dispersa_creare_chorda(
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
officina_tabula_dispersa_inserere(
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
    si (officina_tabula_dispersa_factor_oneris(tabula) > tabula->factor_maximus ||
        officina_tabula_dispersa_factor_deletorum(tabula) > tabula->factor_deletorum_maximus)
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
officina_tabula_dispersa_invenire(
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
officina_tabula_dispersa_factor_oneris(OfficinaTabulaDispersa* tabula)
{
    si (!tabula || tabula->capacitas == ZEPHYRUM)
    {
        redde 0.0f;
    }
    redde (f32)tabula->numerus / (f32)tabula->capacitas;
}

static f32
officina_tabula_dispersa_factor_deletorum(OfficinaTabulaDispersa* tabula)
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
                si (littera[k] == '.' || littera[k] == 'e'
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

/* ================= ex officina/fontes/officina_demissio.c ================= */

/* ==================================================
 * Structurae
 * ================================================== */

nomen structura {
    i8                           clavis_octeti[magnitudo(vacuum*)];
    vacuum*                      valor;
} DemissioIntroitus;

nomen structura {
    constans SemanticaSymbolum* symbolum;
    s32                         index_registri;  /* valor AUT
                                                  * inscriptio arcae */
    b32                         est_arca;
} DemissioSedes;

nomen structura {
    b32 directum;               /* VERUM: index = registrum valoris */
    s32 index;                  /* aliter registrum inscriptionis */
    b32 valida;
} DemissioLocus;

nomen structura {
    OfficinaPiscina*                piscina;
    SilvaSemantica*         sem;
    MedullaModulus*         modulus;
    MedullaFunctio*         functio;
    s32                     bloccus;
    s32                     numerator;    /* nomina bloccorum */
    s32                     frange_finis; /* -I extra */
    s32                     perge_finis;
    OfficinaTabulaDispersa*         lexemata;     /* SilvaToken* -> symbolum */
    OfficinaTabulaDispersa*         sedes;        /* symbolum -> DemissioSedes* */
    OfficinaTabulaDispersa*         capti;        /* symbola inscriptione capta */
} Demissio;

/* prototypa mutuae recursionis */
interior s32 _expressionem (Demissio* d, constans SilvaNodus* nodus);
interior MedullaOperandum _ut_operandum (Demissio* d,
    constans SilvaNodus* nodus);
interior s32 _ut_valor (Demissio* d, constans SilvaNodus* nodus);
interior DemissioLocus _ut_locum (Demissio* d,
    constans SilvaNodus* nodus);
interior vacuum _sententiam (Demissio* d, constans SilvaNodus* nodus);

/* ==================================================
 * Auxilia parva
 * ================================================== */

interior OfficinaChorda
_ch_de_silva (SilvaChorda s)
{
    OfficinaChorda c;

    c.datum = s.datum;
    c.mensura = s.mensura;
    redde c;
}

interior OfficinaChorda
_ch_literis (constans character* literis)
{
    OfficinaChorda c;
    unio { constans character* c; i8* m; } u;

    u.c = literis;
    c.datum = u.m;
    c.mensura = (i32)strlen(literis);
    redde c;
}

interior vacuum*
_sine_constante (constans vacuum* monstrator)
{
    unio { constans vacuum* c; vacuum* m; } u;

    u.c = monstrator;
    redde u.m;
}

interior constans SilvaNodus*
_canonicus (constans SilvaNodus* nodus)
{
    dum (nodus != NIHIL
         && nodus->genus == (s32)SILVA_C89_GENUS_AMBIGUUS)
    {
        SilvaValor v = silva_c89_ambiguus_canonica(nodus);

        si (v.genus != SILVA_VALOR_NODUS)
        {
            frange;
        }
        nodus = v.datum.nodus;
    }
    redde nodus;
}

interior SilvaChorda
_tok_textus (SilvaValor v)
{
    SilvaChorda vacua;

    si (v.genus == SILVA_VALOR_TOKEN && v.datum.token != NIHIL)
    {
        redde v.datum.token->valor;
    }
    vacua.mensura = 0;
    vacua.datum = NIHIL;
    redde vacua;
}

interior b32
_op_est (SilvaChorda textus, constans character* literis)
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
_tabulam_invenire (OfficinaTabulaDispersa* tabula,
    constans vacuum* monstrator)
{
    i8 octeti[magnitudo(vacuum*)];
    OfficinaChorda clavis;
    vacuum* valor = NIHIL;

    memcpy(octeti, &monstrator, magnitudo(octeti));
    clavis.datum = octeti;
    clavis.mensura = (i32)magnitudo(octeti);
    si (officina_tabula_dispersa_invenire(tabula, clavis, &valor))
    {
        redde ((DemissioIntroitus*)valor)->valor;
    }
    redde NIHIL;
}

interior vacuum
_tabulam_ponere (Demissio* d, OfficinaTabulaDispersa* tabula,
    constans vacuum* monstrator, vacuum* valor)
{
    DemissioIntroitus* introitus;
    OfficinaChorda clavis;
    i8 octeti[magnitudo(vacuum*)];
    vacuum* prior = NIHIL;

    memcpy(octeti, &monstrator, magnitudo(octeti));
    clavis.datum = octeti;
    clavis.mensura = (i32)magnitudo(octeti);
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
    introitus->valor = valor;
    clavis.datum = introitus->clavis_octeti;
    clavis.mensura = (i32)magnitudo(introitus->clavis_octeti);
    (vacuum)officina_tabula_dispersa_inserere(tabula, clavis,
        (vacuum*)introitus);
}

/* ==================================================
 * Typi
 * ================================================== */

interior constans TypusC89*
_exutus (constans TypusC89* typus)
{
    dum (typus != NIHIL && typus->genus == TYPUS_C89_QUALIFICATUS)
    {
        typus = typus->datum.qualificatus.internum;
    }
    redde typus;
}

/* MEDULLA_TYPUS_* aut -I (non scalaris) */
interior s32
_typus_medullae (constans TypusC89* typus)
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
_typus_finalis (constans Demissio* d, constans SilvaNodus* nodus)
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
_mensura_typi (Demissio* d, constans TypusC89* typus)
{
    TypusC89* mutabilis = (TypusC89*)_sine_constante(
        (constans vacuum*)typus);

    si (typus == NIHIL
        || !silva_c89_formam_computare(d->sem, mutabilis))
    {
        redde -I;
    }
    redde (s32)typus->magnitudo_octetorum;
}

/* ==================================================
 * Nomina + blocci + registra
 * ================================================== */

interior OfficinaChorda
_titulum_fingere (Demissio* d, constans character* basis)
{
    character littera[XLVIII];
    OfficinaChorda temporarium;
    s32 longitudo = (s32)sprintf(littera, "%s_%d", basis,
        (int)d->numerator);

    d->numerator++;
    temporarium.datum = (i8*)littera;
    temporarium.mensura = (i32)longitudo;
    redde officina_chorda_transcribere(temporarium, d->piscina);
}

interior s32
_bloccum_novum (Demissio* d, constans character* basis)
{
    redde medulla_bloccum_creare(d->functio,
        _titulum_fingere(d, basis));
}

interior b32
_bloccus_vivus (constans Demissio* d)
{
    constans MedullaBloccus* bloccus = medulla_bloccum_obtinere(
        d->functio, d->bloccus);

    redde bloccus != NIHIL && !bloccus->terminatus;
}

interior vacuum
_vivum (Demissio* d)
{
    si (!_bloccus_vivus(d))
    {
        d->bloccus = _bloccum_novum(d, "insequens");
    }
}

interior b32
_registrum_captum (constans Demissio* d, OfficinaChorda titulus)
{
    i32 i;
    i32 m = officina_xar_numerus(d->functio->registra);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans OfficinaChorda* sedes = (constans OfficinaChorda*)officina_xar_obtinere(
            d->functio->registra, i);

        si (sedes->mensura == titulus.mensura
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
_registrum_unicum (Demissio* d, SilvaChorda titulus_silva)
{
    OfficinaChorda titulus = _ch_de_silva(titulus_silva);

    si (titulus.mensura == ZEPHYRUM
        || !_registrum_captum(d, titulus))
    {
        redde medulla_registrum_novum(d->functio, titulus);
    }
    {
        character littera[LXIV];
        OfficinaChorda alterum;
        i32 scriptum = ZEPHYRUM;
        i32 caput = (titulus.mensura < XL) ? titulus.mensura : XL;
        s32 n;

        memcpy(littera, titulus.datum, (memoriae_index)caput);
        scriptum = caput;
        n = (s32)sprintf(littera + scriptum, "_%d",
            (int)d->numerator);
        d->numerator++;
        alterum.datum = (i8*)littera;
        alterum.mensura = scriptum + (i32)n;
        redde medulla_registrum_novum(d->functio,
            officina_chorda_transcribere(alterum, d->piscina));
    }
}

interior s32
_registrum_temporarium (Demissio* d)
{
    OfficinaChorda vacua;

    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;
    redde medulla_registrum_novum(d->functio, vacua);
}

/* ==================================================
 * Emissio
 * ================================================== */

interior s32
_em (Demissio* d, constans SilvaNodus* origo, s32 op, s32 typus,
    s32 typus_secundus, s32 destinatio, MedullaOperandum a,
    MedullaOperandum b, MedullaOperandum c)
{
    MedullaInstructio instructio;

    _vivum(d);
    memset(&instructio, ZEPHYRUM, magnitudo(MedullaInstructio));
    instructio.op = op;
    instructio.typus = typus;
    instructio.typus_secundus = typus_secundus;
    instructio.destinatio = destinatio;
    instructio.extra_index = -I;
    instructio.a = a;
    instructio.b = b;
    instructio.c = c;
    instructio.origo = origo;
    (vacuum)medulla_emittere(d->functio, d->bloccus, &instructio);
    redde destinatio;
}

interior vacuum
_sistere (Demissio* d, constans SilvaNodus* origo,
    constans character* causa)
{
    s32 index = medulla_causam_internare(d->modulus,
        _ch_literis(causa));

    (vacuum)_em(d, origo, MEDULLA_OP_SISTERE, MEDULLA_TYPUS_NIHIL,
        MEDULLA_TYPUS_NIHIL, -I, medulla_op_causa(index),
        medulla_op_nihil(), medulla_op_nihil());
}

interior vacuum
_salire_si_vivus (Demissio* d, s32 finis)
{
    si (_bloccus_vivus(d))
    {
        (vacuum)_em(d, NIHIL, MEDULLA_OP_SALIRE, MEDULLA_TYPUS_NIHIL,
            MEDULLA_TYPUS_NIHIL, -I, medulla_op_bloccum(finis),
            medulla_op_nihil(), medulla_op_nihil());
    }
}

interior vacuum
_ramus (Demissio* d, constans SilvaNodus* origo, MedullaOperandum
    conditio, s32 verum, s32 falsum)
{
    (vacuum)_em(d, origo, MEDULLA_OP_RAMUS, MEDULLA_TYPUS_NIHIL,
        MEDULLA_TYPUS_NIHIL, -I, conditio,
        medulla_op_bloccum(verum), medulla_op_bloccum(falsum));
}

/* movere operandi in registrum novum (typus datur) */
interior s32
_movere (Demissio* d, constans SilvaNodus* origo, s32 typus,
    s32 destinatio, MedullaOperandum a)
{
    redde _em(d, origo, MEDULLA_OP_MOVERE,
        (typus >= ZEPHYRUM) ? typus : MEDULLA_TYPUS_S64,
        MEDULLA_TYPUS_NIHIL, destinatio, a, medulla_op_nihil(),
        medulla_op_nihil());
}

/* mensurae typorum medullae (octeti) */
interior s32
_octeti_typi_m (s32 typus)
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
_fluitans_m (s32 typus)
{
    redde typus == (s32)MEDULLA_TYPUS_F32
        || typus == (s32)MEDULLA_TYPUS_F64;
}

/* conversio inter typos C annotatos - familia operationum
 * nominatarum; idem aut solum-signum = registrum immotum
 * (typi in operationibus, non in registris) */
interior s32
_conversionem (Demissio* d, constans SilvaNodus* origo, s32
    registrum_valoris, constans TypusC89* de, constans TypusC89* ad)
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

/* ==================================================
 * Sedes symbolorum
 * ================================================== */

interior DemissioSedes*
_sedem_symboli (constans Demissio* d,
    constans SemanticaSymbolum* symbolum)
{
    redde (DemissioSedes*)_tabulam_invenire(d->sedes,
        (constans vacuum*)symbolum);
}

interior DemissioSedes*
_sedem_creare (Demissio* d, constans SemanticaSymbolum* symbolum,
    s32 index_registri, b32 est_arca)
{
    DemissioSedes* sedes = (DemissioSedes*)officina_piscina_allocare(
        d->piscina, (memoriae_index)magnitudo(DemissioSedes));

    si (sedes == NIHIL)
    {
        redde NIHIL;
    }
    sedes->symbolum = symbolum;
    sedes->index_registri = index_registri;
    sedes->est_arca = est_arca;
    _tabulam_ponere(d, d->sedes, (constans vacuum*)symbolum,
        (vacuum*)sedes);
    redde sedes;
}

/* ==================================================
 * Expressiones
 * ================================================== */

/* valor naturalis identificatoris (ante conversionem nodi) */
interior s32
_identificatorem (Demissio* d, constans SilvaNodus* nodus)
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
    si (sedes != NIHIL)
    {
        si (!sedes->est_arca)
        {
            redde sedes->index_registri;
        }
        /* arca: inscriptio; onus solum si typus scalaris */
        {
            s32 mt = _typus_medullae(naturalis);

            si (mt >= ZEPHYRUM
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
    si (symbolum->profunditas > ZEPHYRUM
        && (symbolum->repositio & REPOSITIO_STATICA) != ZEPHYRUM)
    {
        _sistere(d, nodus, "nondum staticum locale (M1b C)");
        redde _registrum_temporarium(d);
    }
    {
        s32 index_symboli = medulla_symbolum_internare(d->modulus,
            _ch_de_silva(symbolum->titulus));
        s32 inscriptio = _em(d, nodus, MEDULLA_OP_LOCUS,
            MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
            _registrum_temporarium(d),
            medulla_op_symbolum(index_symboli), medulla_op_nihil(),
            medulla_op_nihil());
        constans TypusC89* exutus = _exutus(naturalis);
        s32 mt = _typus_medullae(naturalis);

        si (mt >= ZEPHYRUM && exutus != NIHIL
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
_indicem_scalare (Demissio* d, constans SilvaNodus* origo,
    MedullaOperandum index_op, s32 mt_indicis, s32 mensura_elementi)
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
_est_monstratorius (constans TypusC89* typus)
{
    constans TypusC89* exutus = _exutus(typus);

    redde exutus != NIHIL
        && (exutus->genus == TYPUS_C89_MONSTRATOR
            || exutus->genus == TYPUS_C89_ACIES);
}

interior s32
_mensura_elementi_monstratoris (Demissio* d,
    constans TypusC89* typus)
{
    constans TypusC89* exutus = _exutus(typus);
    constans TypusC89* elementum = NIHIL;

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
_brevem (Demissio* d, constans SilvaNodus* nodus, b32 est_et)
{
    SilvaValor sin_v = silva_c89_binarium_sinister(nodus);
    SilvaValor dex_v = silva_c89_binarium_dexter(nodus);
    s32 fructus = _registrum_temporarium(d);
    s32 b_alter = _bloccum_novum(d, est_et ? "et_alter" : "vel_alter");
    s32 b_muta  = _bloccum_novum(d, est_et ? "et_verum" : "vel_zephyrum");
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
_binarium (Demissio* d, constans SilvaNodus* nodus)
{
    SilvaValor sin_v = silva_c89_binarium_sinister(nodus);
    SilvaValor dex_v = silva_c89_binarium_dexter(nodus);
    SilvaChorda operator = _tok_textus(
        silva_c89_binarium_tok_operator(nodus));
    constans TypusC89* fructus_t = silva_c89_typus_expressionis(
        d->sem, nodus);
    s32 op = -I;
    b32 comparatio = FALSUM;

    si (sin_v.genus != SILVA_VALOR_NODUS
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
    si      (_op_est(operator, "+"))  { op = MEDULLA_OP_ADDERE; }
    alioquin si (_op_est(operator, "-"))  { op = MEDULLA_OP_SUBTRAHERE; }
    alioquin si (_op_est(operator, "*"))  { op = MEDULLA_OP_MULTIPLICARE; }
    alioquin si (_op_est(operator, "/"))  { op = MEDULLA_OP_DIVIDERE; }
    alioquin si (_op_est(operator, "%"))  { op = MEDULLA_OP_RESIDUUM; }
    alioquin si (_op_est(operator, "&"))  { op = MEDULLA_OP_ET; }
    alioquin si (_op_est(operator, "|"))  { op = MEDULLA_OP_VEL; }
    alioquin si (_op_est(operator, "^"))  { op = MEDULLA_OP_AUT; }
    alioquin si (_op_est(operator, "<<")) { op = MEDULLA_OP_SINISTRORSUM; }
    alioquin si (_op_est(operator, ">>")) { op = MEDULLA_OP_DEXTRORSUM; }
    alioquin si (_op_est(operator, "==")) { op = MEDULLA_OP_AEQUALIS; comparatio = VERUM; }
    alioquin si (_op_est(operator, "!=")) { op = MEDULLA_OP_INAEQUALIS; comparatio = VERUM; }
    alioquin si (_op_est(operator, "<"))  { op = MEDULLA_OP_MINOR; comparatio = VERUM; }
    alioquin si (_op_est(operator, "<=")) { op = MEDULLA_OP_MINOR_AEQUALIS; comparatio = VERUM; }
    alioquin si (_op_est(operator, ">"))  { op = MEDULLA_OP_MAIOR; comparatio = VERUM; }
    alioquin si (_op_est(operator, ">=")) { op = MEDULLA_OP_MAIOR_AEQUALIS; comparatio = VERUM; }
    alioquin
    {
        _sistere(d, nodus, "operator binarii ignotus");
        redde _registrum_temporarium(d);
    }

    /* arithmetica monstratorum: scalatio per formam */
    si ((op == (s32)MEDULLA_OP_ADDERE
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
            s32 mensura = _mensura_elementi_monstratoris(d, sin_ft);
            s32 a = _ut_valor(d, sin_v.datum.nodus);
            s32 b = _ut_valor(d, dex_v.datum.nodus);
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
            basis = _ut_valor(d, n_monstratoris);
            index_op = _ut_operandum(d, n_indicis);
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
_incrementum (Demissio* d, constans SilvaNodus* nodus,
    constans SilvaNodus* internum, b32 est_additio, b32 est_praefixum)
{
    DemissioLocus locus = _ut_locum(d, internum);
    constans TypusC89* naturalis = silva_c89_typus_expressionis(
        d->sem, _canonicus(internum));
    s32 mt = _typus_medullae(naturalis);
    s64 passus = I;
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
_unarium (Demissio* d, constans SilvaNodus* nodus)
{
    SilvaChorda operator = _tok_textus(
        silva_c89_unarium_tok_operator(nodus));
    SilvaValor internum_v = silva_c89_unarium_internum(nodus);
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
        s32 inscriptio = _ut_valor(d, internum);
        constans TypusC89* naturalis = silva_c89_typus_expressionis(
            d->sem, nodus);
        s32 mt = _typus_medullae(naturalis);
        constans TypusC89* exutus = _exutus(naturalis);

        si (mt >= ZEPHYRUM && exutus != NIHIL
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
_assignationem (Demissio* d, constans SilvaNodus* nodus)
{
    SilvaValor sin_v = silva_c89_assignatio_sinister(nodus);
    SilvaValor dex_v = silva_c89_assignatio_dexter(nodus);
    SilvaChorda operator = _tok_textus(
        silva_c89_assignatio_tok_operator(nodus));
    DemissioLocus locus;
    constans SilvaNodus* sinister;
    constans TypusC89* sin_naturalis;
    s32 mt;
    s32 fons;

    si (sin_v.genus != SILVA_VALOR_NODUS
        || dex_v.genus != SILVA_VALOR_NODUS)
    {
        _sistere(d, nodus, "assignatio manca");
        redde _registrum_temporarium(d);
    }
    sinister = _canonicus(sin_v.datum.nodus);
    sin_naturalis = silva_c89_typus_expressionis(d->sem, sinister);
    mt = _typus_medullae(sin_naturalis);
    locus = _ut_locum(d, sin_v.datum.nodus);
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
        s32 op = -I;
        s32 currens;
        s32 commune_reg;
        constans TypusC89* commune_t = _typus_finalis(d, sinister);
        s32 mt_commune = _typus_medullae(commune_t);

        si      (_op_est(operator, "+="))  { op = MEDULLA_OP_ADDERE; }
        alioquin si (_op_est(operator, "-="))  { op = MEDULLA_OP_SUBTRAHERE; }
        alioquin si (_op_est(operator, "*="))  { op = MEDULLA_OP_MULTIPLICARE; }
        alioquin si (_op_est(operator, "/="))  { op = MEDULLA_OP_DIVIDERE; }
        alioquin si (_op_est(operator, "%="))  { op = MEDULLA_OP_RESIDUUM; }
        alioquin si (_op_est(operator, "&="))  { op = MEDULLA_OP_ET; }
        alioquin si (_op_est(operator, "|="))  { op = MEDULLA_OP_VEL; }
        alioquin si (_op_est(operator, "^="))  { op = MEDULLA_OP_AUT; }
        alioquin si (_op_est(operator, "<<=")) { op = MEDULLA_OP_SINISTRORSUM; }
        alioquin si (_op_est(operator, ">>=")) { op = MEDULLA_OP_DEXTRORSUM; }
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
        si (_est_monstratorius(sin_naturalis)
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
                MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
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
_ternarium (Demissio* d, constans SilvaNodus* nodus)
{
    SilvaValor cond_v = silva_c89_ternarius_conditio(nodus);
    SilvaValor verum_v = silva_c89_ternarius_verum(nodus);
    SilvaValor falsum_v = silva_c89_ternarius_falsum(nodus);
    s32 mt = _typus_medullae(silva_c89_typus_expressionis(d->sem,
        nodus));
    s32 fructus = _registrum_temporarium(d);
    s32 b_verum = _bloccum_novum(d, "tern_verum");
    s32 b_falsum = _bloccum_novum(d, "tern_falsum");
    s32 b_finis = _bloccum_novum(d, "tern_finis");
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

/* dispensator: valor naturalis + conversio annotata applicata */
interior s32
_expressionem (Demissio* d, constans SilvaNodus* nodus)
{
    constans TypusC89* naturalis;
    constans TypusC89* conversus;
    s32 fructus;

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

            redde _expressionem(d, v.datum.nodus);
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
            _sistere(d, nodus, "nondum vocatio (M1b B)");
            redde _registrum_temporarium(d);
        }
        casus (s32)SILVA_C89_GENUS_ACCESSUS:
        {
            _sistere(d, nodus, "nondum accessus (M1b B)");
            redde _registrum_temporarium(d);
        }
        casus (s32)SILVA_C89_GENUS_SUBSCRIPTIO:
        {
            _sistere(d, nodus, "nondum subscriptio (M1b B)");
            redde _registrum_temporarium(d);
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_CHORDA:
        {
            _sistere(d, nodus, "nondum chorda litteralis (M1b C)");
            redde _registrum_temporarium(d);
        }
        casus (s32)SILVA_C89_GENUS_CONGERIES:
        {
            _sistere(d, nodus, "nondum congeries (M1b C)");
            redde _registrum_temporarium(d);
        }
        ordinarius:
        {
            _sistere(d, nodus, "expressio generis ignoti");
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
_ut_operandum (Demissio* d, constans SilvaNodus* nodus)
{
    constans SilvaNodus* canonicus = _canonicus(nodus);

    si (canonicus == NIHIL)
    {
        redde medulla_op_immediatum(0);
    }
    si (canonicus->genus == (s32)SILVA_C89_GENUS_FOLIUM_INTEGER
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

        si (textus.mensura > ZEPHYRUM
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
_ut_valor (Demissio* d, constans SilvaNodus* nodus)
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
_ut_locum (Demissio* d, constans SilvaNodus* nodus)
{
    DemissioLocus locus;

    locus.directum = FALSUM;
    locus.index = -I;
    locus.valida = FALSUM;
    nodus = _canonicus(nodus);
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
        si (sedes != NIHIL)
        {
            locus.directum = !sedes->est_arca;
            locus.index = sedes->index_registri;
            locus.valida = VERUM;
            redde locus;
        }
        si (symbolum->profunditas > ZEPHYRUM
            && (symbolum->repositio & REPOSITIO_STATICA)
                != ZEPHYRUM)
        {
            redde locus;   /* staticum locale: M1b C */
        }
        /* globale */
        {
            s32 index_symboli = medulla_symbolum_internare(
                d->modulus, _ch_de_silva(symbolum->titulus));

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

            locus.directum = FALSUM;
            locus.index = _ut_valor(d, internum_v.datum.nodus);
            locus.valida = VERUM;
            redde locus;
        }
    }
    redde locus;   /* accessus/subscriptio -> M1b B */
}

/* ==================================================
 * Sententiae
 * ================================================== */

interior vacuum
_lista_sententiarum (Demissio* d, SilvaValor lista)
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
_symbolum_lexematis (constans Demissio* d, SilvaToken* lexema)
{
    redde (constans SemanticaSymbolum*)_tabulam_invenire(
        d->lexemata, (constans vacuum*)lexema);
}

interior vacuum
_declarationem (Demissio* d, constans SilvaNodus* nodus)
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
            SilvaValor init_v = silva_c89_declarator_initiatus_initiator(
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
        si (symbolum == NIHIL
            || symbolum->genus == (int)SYMBOLUM_TYPEDEF)
        {
            perge;
        }
        si ((symbolum->repositio & REPOSITIO_STATICA) != ZEPHYRUM)
        {
            _sistere(d, nodus, "nondum staticum locale (M1b C)");
            perge;
        }
        sedes = _sedem_symboli(d, symbolum);
        si (sedes == NIHIL || initiator == NIHIL)
        {
            perge;
        }
        si (_canonicus(initiator)->genus
            == (s32)SILVA_C89_GENUS_CONGERIES)
        {
            _sistere(d, nodus, "nondum congeries localis (M1b C)");
            perge;
        }
        {
            s32 mt = _typus_medullae(symbolum->typus);
            MedullaOperandum fons = _ut_operandum(d, initiator);

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
_bloccum_tituli (Demissio* d, SilvaChorda titulus)
{
    character littera[LXIV];
    OfficinaChorda quaesitum;
    i32 caput = (titulus.mensura < LX) ? titulus.mensura : LX;
    i32 i;
    i32 m;

    littera[ZEPHYRUM] = 'l';
    littera[I] = '_';
    memcpy(littera + II, titulus.datum, (memoriae_index)caput);
    quaesitum.datum = (i8*)littera;
    quaesitum.mensura = caput + II;

    m = officina_xar_numerus(d->functio->blocci);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MedullaBloccus* bloccus =
            (constans MedullaBloccus*)officina_xar_obtinere(
                d->functio->blocci, i);

        si (bloccus->titulus.mensura == quaesitum.mensura
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
_commutationem (Demissio* d, constans SilvaNodus* nodus)
{
    SilvaValor discrimen_v = silva_c89_commutatio_discrimen(nodus);
    SilvaValor corpus_v = silva_c89_commutatio_corpus(nodus);
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
    elementa = silva_c89_corpus_elementa(corpus);
    m = (i32)silva_valor_lista_numerus(elementa);
    discrimen = _ut_valor(d, discrimen_v.datum.nodus);
    b_finis = _bloccum_novum(d, "comm_finis");
    d->frange_finis = b_finis;

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
                s64 valor = 0;
                s32 b_casus = _bloccum_novum(d, "comm_casus");
                SilvaValor valor_v = silva_c89_casus_valor(
                    elementum);
                s32 probatum;

                sedes_blocci = (s32*)officina_xar_addere(blocci_casuum);
                *sedes_blocci = b_casus;
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
                b_ordinarius = _bloccum_novum(d, "comm_ordinarius");
                sedes_blocci = (s32*)officina_xar_addere(blocci_casuum);
                *sedes_blocci = b_ordinarius;
            }
            alioquin
            {
                sedes_blocci = (s32*)officina_xar_addere(blocci_casuum);
                *sedes_blocci = -I;
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
                si (elementum->genus == (s32)SILVA_C89_GENUS_CASUS
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
    d->bloccus = b_finis;
    d->frange_finis = frange_prior;
}

interior vacuum
_sententiam (Demissio* d, constans SilvaNodus* nodus)
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
            SilvaValor cond_v = silva_c89_si_conditio(nodus);
            SilvaValor cons_v = silva_c89_si_consequens(nodus);
            SilvaValor alio_v = silva_c89_si_alioquin(nodus);
            b32 habet_alioquin = (alio_v.genus == SILVA_VALOR_NODUS);
            s32 b_verum = _bloccum_novum(d, "si_verum");
            s32 b_falsum = habet_alioquin
                ? _bloccum_novum(d, "si_falsum") : -I;
            s32 b_finis = _bloccum_novum(d, "si_finis");
            s32 conditio = _ut_valor(d, cond_v.datum.nodus);

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
            s32 b_proba = _bloccum_novum(d, "dum_proba");
            s32 b_corpus = _bloccum_novum(d, "dum_corpus");
            s32 b_finis = _bloccum_novum(d, "dum_finis");
            s32 frange_prior = d->frange_finis;
            s32 perge_prior = d->perge_finis;

            _salire_si_vivus(d, b_proba);
            d->bloccus = b_proba;
            {
                SilvaValor cond_v = silva_c89_dum_conditio(nodus);
                s32 conditio = _ut_valor(d, cond_v.datum.nodus);

                _ramus(d, nodus, medulla_op_registrum(conditio),
                    b_corpus, b_finis);
            }
            d->bloccus = b_corpus;
            d->frange_finis = b_finis;
            d->perge_finis = b_proba;
            {
                SilvaValor corpus_v = silva_c89_dum_corpus(nodus);

                si (corpus_v.genus == SILVA_VALOR_NODUS)
                {
                    _sententiam(d, corpus_v.datum.nodus);
                }
            }
            _salire_si_vivus(d, b_proba);
            d->frange_finis = frange_prior;
            d->perge_finis = perge_prior;
            d->bloccus = b_finis;
            redde;
        }
        casus (s32)SILVA_C89_GENUS_FAC_DUM:
        {
            s32 b_corpus = _bloccum_novum(d, "fac_corpus");
            s32 b_proba = _bloccum_novum(d, "fac_proba");
            s32 b_finis = _bloccum_novum(d, "fac_finis");
            s32 frange_prior = d->frange_finis;
            s32 perge_prior = d->perge_finis;

            _salire_si_vivus(d, b_corpus);
            d->bloccus = b_corpus;
            d->frange_finis = b_finis;
            d->perge_finis = b_proba;
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
            d->frange_finis = frange_prior;
            d->perge_finis = perge_prior;
            d->bloccus = b_finis;
            redde;
        }
        casus (s32)SILVA_C89_GENUS_PER:
        {
            SilvaValor clausula_v = silva_c89_per_clausula(nodus);
            constans SilvaNodus* clausula = (clausula_v.genus
                == SILVA_VALOR_NODUS)
                ? _canonicus(clausula_v.datum.nodus) : NIHIL;
            s32 b_proba = _bloccum_novum(d, "per_proba");
            s32 b_corpus = _bloccum_novum(d, "per_corpus");
            s32 b_passus = _bloccum_novum(d, "per_passus");
            s32 b_finis = _bloccum_novum(d, "per_finis");
            s32 frange_prior = d->frange_finis;
            s32 perge_prior = d->perge_finis;

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
            d->bloccus = b_corpus;
            d->frange_finis = b_finis;
            d->perge_finis = b_passus;
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
            d->frange_finis = frange_prior;
            d->perge_finis = perge_prior;
            d->bloccus = b_finis;
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
            SilvaValor valor_v = silva_c89_redde_valor(nodus);
            MedullaOperandum a = medulla_op_nihil();

            si (valor_v.genus == SILVA_VALOR_NODUS)
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

                si (v != NIHIL && v->genus == SILVA_VALOR_NODUS
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
            _sistere(d, nodus, "sententia generis ignoti");
            redde;
        }
    }
}

/* ==================================================
 * Praecursus: symbola inscriptione capta + locales colligere
 * ================================================== */

interior vacuum
_praecursum (Demissio* d, constans SilvaNodus* nodus, OfficinaXar* locales)
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
            SilvaToken* lexema;
            constans SemanticaSymbolum* symbolum;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            lexema = silva_c89_declaratoris_titulus(
                _canonicus(v->datum.nodus));
            si (lexema == NIHIL)
            {
                perge;
            }
            symbolum = _symbolum_lexematis(d, lexema);
            si (symbolum != NIHIL
                && symbolum->genus != (int)SYMBOLUM_TYPEDEF
                && (symbolum->repositio & REPOSITIO_STATICA)
                    == ZEPHYRUM
                && symbolum->profunditas > ZEPHYRUM)
            {
                constans SemanticaSymbolum** sedes =
                    (constans SemanticaSymbolum**)officina_xar_addere(
                        locales);

                si (sedes != NIHIL)
                {
                    *sedes = symbolum;
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
            SilvaValor v = silva_c89_unarium_internum(nodus);

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
                alioquin si (basis->genus
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
            si (basis != NIHIL && basis->genus
                == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
            {
                constans SemanticaSymbolum* symbolum =
                    silva_c89_symbolum_nodi(d->sem, basis);

                si (symbolum != NIHIL
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
_declarator_functionis_invenire (constans SilvaNodus* declarator)
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
_arcam_symboli (Demissio* d, constans SemanticaSymbolum* symbolum)
{
    constans TypusC89* exutus = _exutus(symbolum->typus);
    s32 mt = _typus_medullae(symbolum->typus);

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
_functionem (Demissio* d, constans SilvaNodus* nodus)
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

    si (declarator_v.genus != SILVA_VALOR_NODUS
        || corpus_v.genus != SILVA_VALOR_NODUS)
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
    si (typus_functionis == NIHIL
        || typus_functionis->genus != TYPUS_C89_FUNCTIO)
    {
        redde;
    }
    reditus = _exutus(typus_functionis->datum.functio.reditus);
    mt_reditus = _typus_medullae(reditus);
    si (mt_reditus < ZEPHYRUM && reditus != NIHIL
        && (reditus->genus == TYPUS_C89_STRUCTURA
            || reditus->genus == TYPUS_C89_UNIO))
    {
        reditus_aggregatus = VERUM;
    }
    d->functio = medulla_functionem_creare(d->modulus,
        _ch_de_silva(symbolum->titulus),
        (mt_reditus >= ZEPHYRUM) ? mt_reditus : MEDULLA_TYPUS_NIHIL,
        typus_functionis->datum.functio.est_variadica);
    si (d->functio == NIHIL)
    {
        redde;   /* definitio duplex */
    }
    d->numerator = ZEPHYRUM;
    d->frange_finis = -I;
    d->perge_finis = -I;
    d->sedes = officina_tabula_dispersa_creare_chorda(d->piscina, CCLVI);
    d->capti = officina_tabula_dispersa_creare_chorda(d->piscina, CCLVI);
    locales = officina_xar_creare(d->piscina,
        (i32)magnitudo(SemanticaSymbolum*));

    /* praecursus: capti + locales (ante residentiam) */
    _praecursum(d, corpus_v.datum.nodus, locales);

    /* parametra: registra prima (ordo signaturae); symbola
     * servata pro arca post initium */
    {
        constans SilvaNodus* df = _declarator_functionis_invenire(
            declarator_v.datum.nodus);
        SilvaValor parametra;
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
        si (reditus_aggregatus)
        {
            _sistere(d, nodus, "nondum reditus aggregatus (M1b B)");
        }

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
            s32 mt;

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
                _sistere(d, nodus,
                    "nondum parametrum aggregatum (M1b B)");
                perge;
            }
            sedes = _sedem_symboli(d, p_symbolum);
            {
                s32 mensura = _mensura_typi(d, p_symbolum->typus);
                s32 inscriptio = _registrum_temporarium(d);

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
                sedes->index_registri = inscriptio;
                sedes->est_arca = VERUM;
            }
        }
    }

    /* locales: sedes + arca in initio */
    m = officina_xar_numerus(locales);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaSymbolum** sedes_symboli =
            (constans SemanticaSymbolum**)officina_xar_obtinere(locales, i);
        constans SemanticaSymbolum* symbolum_locale;

        si (sedes_symboli == NIHIL)
        {
            perge;
        }
        symbolum_locale = *sedes_symboli;
        si (_sedem_symboli(d, symbolum_locale) != NIHIL)
        {
            perge;   /* iam creatum */
        }
        si (_arcam_symboli(d, symbolum_locale))
        {
            s32 mensura = _mensura_typi(d, symbolum_locale->typus);
            s32 ordinatio;
            s32 inscriptio;

            si (mensura <= ZEPHYRUM)
            {
                _sistere(d, nodus, "forma localis ignota");
                perge;
            }
            ordinatio = (s32)_exutus(symbolum_locale->typus)
                ->ordinatio;
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

interior vacuum
_radicis_elementum (Demissio* d, constans SilvaNodus* nodus)
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
    si (nodus->genus == (s32)SILVA_C89_GENUS_CONDITIONALIS)
    {
        SilvaValor rami = silva_c89_conditionalis_rami(nodus);
        i32 i;
        i32 m = (i32)silva_valor_lista_numerus(rami);

        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(rami, i);

            si (v != NIHIL && v->genus == SILVA_VALOR_NODUS
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

                    si (e != NIHIL
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
demissio_currere (OfficinaPiscina* piscina, constans SilvaParsura* parsura,
    SilvaSemantica* sem, OfficinaChorda titulus_moduli)
{
    Demissio d;
    i32 i;
    i32 m;

    si (piscina == NIHIL || parsura == NIHIL || sem == NIHIL
        || parsura->commissio == NIHIL)
    {
        redde NIHIL;
    }
    memset(&d, ZEPHYRUM, magnitudo(Demissio));
    d.piscina = piscina;
    d.sem = sem;
    d.modulus = medulla_modulum_creare(piscina, titulus_moduli);
    d.bloccus = -I;
    d.frange_finis = -I;
    d.perge_finis = -I;
    si (d.modulus == NIHIL)
    {
        redde NIHIL;
    }
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
        SilvaValor radix = parsura->commissio->radix;
        i32 n = (i32)silva_valor_lista_numerus(radix);

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
    redde d.modulus;
}
