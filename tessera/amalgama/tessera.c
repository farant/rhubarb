/* tessera.c - GENERATUM (amalgamator) - NE MANU EDITES
 *
 * Bibliotheca terminalis tessellata in plagula una (SQLite
 * modo). Capita POSIX (termios etc.) sublata infra - tessera
 * bibliotheca terminalis EST. Fons veritatis: tessera/fontes/
 * + bibliothecae vendicatae in lib/. Regenerare:
 * tessera/amalgamare.sh
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

/* ================= tessera.h (verbatim) ================= */
/* tessera.h - Bibliotheca terminalis tessellata (interfacies publica)
 *
 * MANU SCRIPTUM, C89 vanilla - definitiones latinae numquam trans
 * limitem publicum transeunt (hospes variabiles si/per/nomen habere
 * potest). Structurae PELLUCIDAE: campi ordine EXACTO fontium
 * (definitiones hae solae in TU amalgamatis - deriva compilationem
 * frangit). Macra eadem nomina ac interna gerunt: redefinitio
 * identica LEGALIS est et deriva valoris redefinitio DIVERSA fit =
 * error compilationis (custodia gratuita).
 *
 * NB: amalgama tessera.c capita POSIX (termios etc.) sublata ad
 * initium fert - tessera bibliotheca terminalis EST; pons memoriae
 * (probationes) et pons posix (machina vera) ambo insunt.
 *
 * Cellulae signum = octeti UTF-8 COMPACTI in u32 (LSB primus; ASCII
 * pellucidum: compactum == codepoint < 0x80). Eventi runa =
 * codepoint DECODITUS. Nomina diversa consulto.
 */

#ifndef TESSERA_H
#define TESSERA_H

#include <stddef.h>

/* ==================================================
 * Piscina (arena vendicata) - creare et destruere solum
 * ================================================== */

typedef struct TesseraPiscina TesseraPiscina;

TesseraPiscina* tessera_piscina_generare_dynamicum(
    const char* titulum, size_t mensura_alvei_initia);
void tessera_piscina_destruere(TesseraPiscina* piscina);

/* ==================================================
 * Chorda (visus octetorum, NON NUL-terminatus)
 * ================================================== */

typedef struct TesseraChorda {
    unsigned int   mensura;
    unsigned char* datum;
} TesseraChorda;

typedef struct TesseraChordaAedificator TesseraChordaAedificator;

/* ==================================================
 * Cellula + stilus + colores + ornamenta + signa
 * ================================================== */

#define TESSERA_COLOR_NATIVUS 0xFF000000

#define TESSERA_ORNAMENTUM_CRASSUM     0x01
#define TESSERA_ORNAMENTUM_OBSCURUM    0x02
#define TESSERA_ORNAMENTUM_CURSIVUM    0x04
#define TESSERA_ORNAMENTUM_SUBLINEATUM 0x08
#define TESSERA_ORNAMENTUM_INVERSUM    0x10
#define TESSERA_ORNAMENTUM_TRANSFIXUM  0x20

typedef struct TesseraCellula {
    unsigned int signum;          /* UTF-8 compactum; 0 = vacuum */
    unsigned int color_litterae;  /* 0x00RRGGBB aut NATIVUS */
    unsigned int color_fundi;
    unsigned int ornamenta;
} TesseraCellula;

typedef struct TesseraStilus {
    unsigned int color_litterae;
    unsigned int color_fundi;
    unsigned int ornamenta;
} TesseraStilus;

TesseraStilus tessera_stilus(unsigned int color_litterae,
    unsigned int color_fundi, unsigned int ornamenta);
TesseraStilus tessera_stilus_nativus(void);
int tessera_stilus_aequalis(TesseraStilus a, TesseraStilus b);

unsigned int tessera_signum_ex_octetis(const unsigned char* octeti,
    unsigned int numerus);
unsigned int tessera_signum_mensura(unsigned int signum);

#define TESSERA_SIGNUM_SIMPLEX_H  0x8094E2
#define TESSERA_SIGNUM_SIMPLEX_V  0x8294E2
#define TESSERA_SIGNUM_SIMPLEX_SS 0x8C94E2
#define TESSERA_SIGNUM_SIMPLEX_SD 0x9094E2
#define TESSERA_SIGNUM_SIMPLEX_IS 0x9494E2
#define TESSERA_SIGNUM_SIMPLEX_ID 0x9894E2
#define TESSERA_SIGNUM_DUPLEX_H   0x9095E2
#define TESSERA_SIGNUM_DUPLEX_V   0x9195E2
#define TESSERA_SIGNUM_DUPLEX_SS  0x9495E2
#define TESSERA_SIGNUM_DUPLEX_SD  0x9795E2
#define TESSERA_SIGNUM_DUPLEX_IS  0x9A95E2
#define TESSERA_SIGNUM_DUPLEX_ID  0x9D95E2
#define TESSERA_SIGNUM_ROTUNDATUM_SS 0xAD95E2
#define TESSERA_SIGNUM_ROTUNDATUM_SD 0xAE95E2
#define TESSERA_SIGNUM_ROTUNDATUM_ID 0xAF95E2
#define TESSERA_SIGNUM_ROTUNDATUM_IS 0xB095E2

typedef enum {
    TESSERA_LINEA_SIMPLEX = 0,
    TESSERA_LINEA_DUPLEX,
    TESSERA_LINEA_ROTUNDATA
} TesseraLineaGenus;

/* ==================================================
 * Pons - tabula functionum machinae (sutura probationum)
 * ================================================== */

typedef struct TesseraPons TesseraPons;

struct TesseraPons {
    void* datum;
    int  (*legere)    (void* datum, unsigned char* buffer,
                       unsigned int capacitas, int mora_ms);
    int  (*scribere)  (void* datum, const unsigned char* octeti,
                       unsigned int numerus);
    int  (*amplitudo) (void* datum, unsigned int* latitudo_out,
                       unsigned int* altitudo_out);
    int  (*intrare)   (void* datum);
    int  (*egredi)     (void* datum);
    int  (*resumptum) (void* datum);   /* NULL licet */
};

/* Pons memoriae (probationes: scriptum intro, captum foras) */
typedef struct TesseraPonsMemoriae {
    TesseraPons               pons;
    TesseraPiscina*           piscina;
    const unsigned char*      initus;
    unsigned int              initus_mensura;
    unsigned int              initus_cursor;
    TesseraChordaAedificator* captum;
    unsigned int              latitudo;
    unsigned int              altitudo;
    int                       intratum;
    int                       resumendum;
    unsigned int              numerus_intratum;
    unsigned int              numerus_exitum;
} TesseraPonsMemoriae;

TesseraPonsMemoriae* tessera_pons_memoriae_creare(
    TesseraPiscina* piscina, unsigned int latitudo,
    unsigned int altitudo);
int tessera_pons_memoriae_initum(TesseraPonsMemoriae* pm,
    const unsigned char* octeti, unsigned int mensura);
TesseraChorda tessera_pons_memoriae_captum(TesseraPonsMemoriae* pm);
void tessera_pons_memoriae_purgare(TesseraPonsMemoriae* pm);
void tessera_pons_memoriae_amplitudo(TesseraPonsMemoriae* pm,
    unsigned int latitudo, unsigned int altitudo);

/* Pons posix (machina vera; NULL si stdin/stdout non terminal) */
TesseraPons* tessera_pons_posix_creare(TesseraPiscina* piscina);

/* ==================================================
 * Opus - contextus, pictura, praesentatio
 * ================================================== */

#define TESSERA_LATITUDO_MAXIMA 512
#define TESSERA_ALTITUDO_MAXIMA 256

typedef struct TesseraFructus {
    unsigned int cellulae_collatae;
    unsigned int cellulae_mutatae;
    unsigned int octeti_emissi;
    unsigned int praesentationes;
    double       tempus_praesentandi_ms;
} TesseraFructus;

typedef struct TesseraOpus TesseraOpus;

struct TesseraOpus {
    TesseraPiscina*           piscina;
    TesseraPons*              pons;
    TesseraCellula*           frons;
    TesseraCellula*           tergum;
    unsigned int              latitudo;
    unsigned int              altitudo;
    TesseraChordaAedificator* aed;
    int                       cursor_x;   /* -1 = celatus */
    int                       cursor_y;
    int                       cursor_x_actus;
    int                       cursor_y_actus;
    int                       cursor_visibilis_actus;
    int                       primum;
    TesseraFructus            fructus;
};

TesseraOpus* tessera_aperire(TesseraPiscina* piscina,
    TesseraPons* pons);
void tessera_claudere(TesseraOpus* opus);
void tessera_intermittere(TesseraOpus* opus);
void tessera_resumere(TesseraOpus* opus);

unsigned int tessera_latitudo(const TesseraOpus* opus);
unsigned int tessera_altitudo(const TesseraOpus* opus);

void tessera_purgare(TesseraOpus* opus, TesseraStilus stilus);
void tessera_cellulam_ponere(TesseraOpus* opus, int x, int y,
    unsigned int signum, TesseraStilus stilus);
TesseraCellula tessera_cellulam_legere(const TesseraOpus* opus,
    int x, int y);
void tessera_scribere(TesseraOpus* opus, int x, int y,
    TesseraChorda textus, TesseraStilus stilus);
void tessera_scribere_literis(TesseraOpus* opus, int x, int y,
    const char* textus, TesseraStilus stilus);
void tessera_quadrum_pingere(TesseraOpus* opus, int x, int y,
    int latitudo, int altitudo, TesseraLineaGenus genus,
    TesseraStilus stilus);
void tessera_lineam_pingere(TesseraOpus* opus, int x, int y,
    int longitudo, int verticalis, TesseraLineaGenus genus,
    TesseraStilus stilus);
void tessera_replere(TesseraOpus* opus, int x, int y,
    int latitudo, int altitudo, unsigned int signum,
    TesseraStilus stilus);
void tessera_cursorem_ponere(TesseraOpus* opus, int x, int y);
int tessera_praesentare(TesseraOpus* opus);
int tessera_magnitudinem_renovare(TesseraOpus* opus);

/* ==================================================
 * Eventa initus + lector
 * ================================================== */

#define TESSERA_LECTOR_BUFFER 64
#define TESSERA_MORA_FUGAE_MS 25

typedef enum {
    TESSERA_EVENTUM_NIHIL = 0,
    TESSERA_EVENTUM_CLAVIS,
    TESSERA_EVENTUM_MUS,
    TESSERA_EVENTUM_AMPLITUDO,
    TESSERA_EVENTUM_RESUMPTUM
} TesseraEventumGenus;

typedef enum {
    TESSERA_CLAVIS_NULLA = 0,
    TESSERA_CLAVIS_SURSUM,
    TESSERA_CLAVIS_DEORSUM,
    TESSERA_CLAVIS_DEXTRA,
    TESSERA_CLAVIS_SINISTRA,
    TESSERA_CLAVIS_DOMUS,
    TESSERA_CLAVIS_FINIS,
    TESSERA_CLAVIS_PAGINA_SURSUM,
    TESSERA_CLAVIS_PAGINA_DEORSUM,
    TESSERA_CLAVIS_INSERTIO,
    TESSERA_CLAVIS_DELETIO,
    TESSERA_CLAVIS_FUGA,
    TESSERA_CLAVIS_REDITUS,
    TESSERA_CLAVIS_TABULA,
    TESSERA_CLAVIS_RETRORSUM,
    TESSERA_CLAVIS_FUNCTIO
} TesseraClavis;

#define TESSERA_MODIFICATOR_IMPERIUM  0x01
#define TESSERA_MODIFICATOR_ALTERUM   0x02
#define TESSERA_MODIFICATOR_MAIUSCULA 0x04

typedef enum {
    TESSERA_MUS_PRESSUS = 0,
    TESSERA_MUS_SOLUTUS,
    TESSERA_MUS_ROTA_SURSUM,
    TESSERA_MUS_ROTA_DEORSUM
} TesseraMusGenus;

typedef struct TesseraEventum {
    TesseraEventumGenus genus;
    int                 runa;      /* codepoint; 0 si specialis */
    TesseraClavis       clavis;
    unsigned int        modificatores;
    unsigned int        numerus;   /* FUNCTIO: 1-12 */
    TesseraMusGenus     mus_genus;
    int                 mus_x;
    int                 mus_y;
    unsigned int        mus_pulsus;
    unsigned int        latitudo;  /* AMPLITUDO */
    unsigned int        altitudo;
} TesseraEventum;

typedef struct TesseraLector {
    TesseraPons*  pons;
    unsigned char buffer[TESSERA_LECTOR_BUFFER];
    unsigned int  mensura;
    unsigned int  latitudo_nota;
    unsigned int  altitudo_nota;
} TesseraLector;

TesseraLector* tessera_lector_creare(TesseraPiscina* piscina,
    TesseraPons* pons);
TesseraEventumGenus tessera_eventum_expectare(TesseraLector* lector,
    TesseraEventum* eventum, int mora_ms);

#endif /* TESSERA_H */

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
nomen structura TesseraPiscinaNotatio {
            vacuum* alveus_nunc;   /* Index ad alveum currentem */
    memoriae_index  positus;       /* Offset in alveo */
} TesseraPiscinaNotatio;

TesseraPiscina*
tessera_piscina_generare_dynamicum (
          constans character* piscinae_titulum,
              memoriae_index  mensura_alvei_initia);


/* ===============================================
 * Destructio
 * =============================================== */

vacuum
tessera_piscina_destruere (
        TesseraPiscina* piscina);


/* ===============================================
 * Allocatio - fatalis si fallit
 * =============================================== */

static vacuum*
tessera_piscina_allocare (
                         TesseraPiscina* piscina,
                  memoriae_index  mensura);

static vacuum*
tessera_piscina_allocare_ordinatum (
                         TesseraPiscina* piscina,
                  memoriae_index  mensura,
                  memoriae_index  ordinatio);

#endif

/* ================= ex include/chorda_aedificator.h ================= */
#ifndef CHORDA_AEDIFICATOR_H
#define CHORDA_AEDIFICATOR_H


/* ==================================================
 * Creatio / Destructio
 * ================================================== */

static TesseraChordaAedificator*
tessera_chorda_aedificator_creare (
           TesseraPiscina* piscina,
    memoriae_index  capacitas_initialis);


/* ==================================================
 * Appendere - Singularis Character
 * ================================================== */

static b32
tessera_chorda_aedificator_appendere_character (
    TesseraChordaAedificator* aedificator,
            character  c);


/* ==================================================
 * Appendere - Chordae (Chordae et C-chordae)
 * ================================================== */

static b32
tessera_chorda_aedificator_appendere_literis (
     TesseraChordaAedificator* aedificator,
    constans character* cstr);

static b32
tessera_chorda_aedificator_appendere_i32 (
    TesseraChordaAedificator* aedificator,
                  i32  n);

/* spectare: vide contentum currentem sine finiendo
 * Reddit chordam spectationem buffer currenti.
 * Validus solum usque ad proximam mutationem. */
static TesseraChorda
tessera_chorda_aedificator_spectare (
    TesseraChordaAedificator* aedificator);


/* ==================================================
 * Cyclus Vitae
 * ================================================== */

/* reset: purga contentum, serva capacitatem allocatam
 * Utile ad reutilizandum aedificatorem pro chordis multiplicibus */
static vacuum
tessera_chorda_aedificator_reset (
    TesseraChordaAedificator* aedificator);


/* ==================================================
 * Constantae Configurationis
 * ================================================== */

/* CHORDA_AEDIFICATOR_INDENTATIO_SPATIA
 * Numerus spatiorum per gradum indentationis (typice 2 vel 4) */
#define CHORDA_AEDIFICATOR_INDENTATIO_SPATIA II


#endif /* CHORDA_AEDIFICATOR_H */

/* ================= ex include/utf8.h ================= */
/*
 * utf8.h - Bibliotheca UTF-8 decodendi
 *
 * Functiones purae pro decodendo UTF-8 ad codepoints.
 * Nulla allocatio, nulla dependentia praeter latina.h
 */

#ifndef UTF8_H
#define UTF8_H

/*
 * utf8_decodere - Decodere unam runam ex sequentia UTF-8
 *
 * @ptr: Indicator ad indicatorem currentem (promovebitur)
 * @finis: Finis buffer (non legendum)
 *
 * Redde: Codepoint (0-0x10FFFF), vel -1 si invalidum/incompletum
 *
 * Nota: Indicator promovetur ad proximam runam post decodificationem
 */
static s32
tessera_utf8_decodere (
    constans i8** ptr,
    constans i8*  finis);

/*
 * utf8_longitudo_byte - Quot bytes hic byte principalis indicat?
 *
 * @byte: Primus byte sequentiae
 *
 * Redde: 1-4 pro validis, 0 pro invalidis
 *
 * Exempla:
 *   0xxxxxxx -> 1 (ASCII)
 *   110xxxxx -> 2
 *   1110xxxx -> 3
 *   11110xxx -> 4
 *   10xxxxxx -> 0 (continuatio, non principalis)
 *   11111xxx -> 0 (invalidum)
 */
static s32
tessera_utf8_longitudo_byte (
    i8 byte);

/*
 * utf8_est_continuatio - An hic byte est continuatio? (10xxxxxx)
 *
 * @byte: Byte examinandus
 *
 * Redde: VERUM si continuatio, FALSUM aliter
 */
static b32
tessera_utf8_est_continuatio (
    i8 byte);

#endif /* UTF8_H */

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

/* ================= ex tessera/fontes/tessera_cellula.h ================= */
/* tessera_cellula.h - Cellula, stilus, colores, signa (Phase A)
 *
 * SIGNUM = octeti UTF-8 COMPACTI in i32 (spec-v2 par 1.3): octetus
 * primus in LSB, 1-4 octeti, 0 = cellula vacua (spatium emittitur).
 * ASCII: valor compactus == codepoint (< 0x80) - hospiti pellucidum.
 * Sine codificatore, sine decodificatore: scriptio limites runarum
 * ambulat (utf8_proxima_runa), emissio octetos effundit.
 *
 * COLOR = 0x00RRGGBB; TESSERA_COLOR_NATIVUS = defalta terminalis
 * (SGR reditio nuda eam dat - emissio nihil addit).
 *
 * ORNAMENTA = sex tuta (SGR singuli): crassum 1, obscurum 2,
 * cursivum 3, sublineatum 4, inversum 7, transfixum 9.
 */

#ifndef TESSERA_CELLULA_H
#define TESSERA_CELLULA_H


/* ==================================================
 * Colores
 * ================================================== */

#define TESSERA_COLOR_NATIVUS 0xFF000000


/* ==================================================
 * Ornamenta (fasciculus bitorum - sex tuta)
 * ================================================== */

#define TESSERA_ORNAMENTUM_CRASSUM     0x01
#define TESSERA_ORNAMENTUM_OBSCURUM    0x02
#define TESSERA_ORNAMENTUM_CURSIVUM    0x04
#define TESSERA_ORNAMENTUM_SUBLINEATUM 0x08
#define TESSERA_ORNAMENTUM_INVERSUM    0x10
#define TESSERA_ORNAMENTUM_TRANSFIXUM  0x20

TesseraStilus
tessera_stilus (
    i32 color_litterae,
    i32 color_fundi,
    i32 ornamenta);
TesseraStilus
tessera_stilus_nativus (vacuum);
b32
tessera_stilus_aequalis (
    TesseraStilus a,
    TesseraStilus b);


/* ==================================================
 * Signum compactum
 * ================================================== */

/* Octetos 1-4 compingere (LSB primus); 0 octeti aut nimis -> 0 */
i32
tessera_signum_ex_octetis (
    constans i8* octeti,
            i32  numerus);

/* Numerus octetorum signi; 0 pro vacuo */
i32
tessera_signum_mensura (
    i32 signum);

/* Octetos signi in aedificatorem effundere; vacuum -> ' ' */
vacuum
tessera_signum_scribere (
    TesseraChordaAedificator* aed,
                  i32  signum);


/* ==================================================
 * Signa linearum (constanta compacta)
 * H/V horizontale/verticale; anguli SS/SD/IS/ID =
 * superior sinister/dexter, inferior sinister/dexter
 * ================================================== */

#define TESSERA_SIGNUM_SIMPLEX_H  0x8094E2  /* U+2500 */
#define TESSERA_SIGNUM_SIMPLEX_V  0x8294E2  /* U+2502 */
#define TESSERA_SIGNUM_SIMPLEX_SS 0x8C94E2  /* U+250C */
#define TESSERA_SIGNUM_SIMPLEX_SD 0x9094E2  /* U+2510 */
#define TESSERA_SIGNUM_SIMPLEX_IS 0x9494E2  /* U+2514 */
#define TESSERA_SIGNUM_SIMPLEX_ID 0x9894E2  /* U+2518 */

#define TESSERA_SIGNUM_DUPLEX_H   0x9095E2  /* U+2550 */
#define TESSERA_SIGNUM_DUPLEX_V   0x9195E2  /* U+2551 */
#define TESSERA_SIGNUM_DUPLEX_SS  0x9495E2  /* U+2554 */
#define TESSERA_SIGNUM_DUPLEX_SD  0x9795E2  /* U+2557 */
#define TESSERA_SIGNUM_DUPLEX_IS  0x9A95E2  /* U+255A */
#define TESSERA_SIGNUM_DUPLEX_ID  0x9D95E2  /* U+255D */

#define TESSERA_SIGNUM_ROTUNDATUM_SS 0xAD95E2  /* U+256D */
#define TESSERA_SIGNUM_ROTUNDATUM_SD 0xAE95E2  /* U+256E */
#define TESSERA_SIGNUM_ROTUNDATUM_ID 0xAF95E2  /* U+256F */
#define TESSERA_SIGNUM_ROTUNDATUM_IS 0xB095E2  /* U+2570 */

#endif /* TESSERA_CELLULA_H */

/* ================= ex tessera/fontes/tessera_pons.h ================= */
/* tessera_pons.h - Pons machinae: tabula functionum (Phase A)
 *
 * PONS EST SUTURA PROBATIONUM (spec-v2 par 1.6, CLAUDE.md pin):
 * omnia supra pontem per pontem memoriae probantur (octeti scripti
 * intro, effugia capta foras). Capita systematis in
 * tessera_pons_posix.c SOLO vivunt (Phase B) - numquam in capitibus
 * publicis (mos tcp.h).
 *
 * Contractus:
 *   legere    - octetos usque ad capacitatem intra moram (ms);
 *               reddit numerum lectorum, 0 = mora exacta, -1 = error
 *   scribere  - octetos effundere (totos; FALSUM in errore)
 *   amplitudo - mensura scrinii in cellulis
 *   intrare   - modus crudus + scrinium alternum + al. (status
 *               machinae; pons memoriae solum numerat)
 *   egredi     - omnia restituere
 */

#ifndef TESSERA_PONS_H
#define TESSERA_PONS_H

#endif /* TESSERA_PONS_H */

/* ================= ex tessera/fontes/tessera_pons_memoriae.h ================= */
/* tessera_pons_memoriae.h - Pons memoriae: sutura probationum
 * (Phase A)
 *
 * Machina ficta: initus = octeti scripti (probatio eos ponit),
 * exitus = effugia capta (probatio ea legit et asserit), amplitudo
 * mutabilis (semita magnitudinis renovandae probabilis sine
 * SIGWINCH). intrare/exire numerantur - status crudi observabilis.
 */

#ifndef TESSERA_PONS_MEMORIAE_H
#define TESSERA_PONS_MEMORIAE_H

TesseraPonsMemoriae*
tessera_pons_memoriae_creare (
    TesseraPiscina* piscina,
        i32  latitudo,
        i32  altitudo);

/* Scriptum initus ponere (copiatur); cursor ad initium redit */
b32
tessera_pons_memoriae_initum (
    TesseraPonsMemoriae* pm,
            constans i8* octeti,
                    i32  mensura);

/* Visus exitus capti (validus usque ad mutationem proximam) */
TesseraChorda
tessera_pons_memoriae_captum (
    TesseraPonsMemoriae* pm);

/* Exitum captum vacare (buffer manet - exemplar reset) */
vacuum
tessera_pons_memoriae_purgare (
    TesseraPonsMemoriae* pm);

/* Amplitudinem mutare (semita renovationis probanda) */
vacuum
tessera_pons_memoriae_amplitudo (
    TesseraPonsMemoriae* pm,
                    i32  latitudo,
                    i32  altitudo);

#endif /* TESSERA_PONS_MEMORIAE_H */

/* ================= ex tessera/fontes/tessera_pons_posix.h ================= */
/* tessera_pons_posix.h - Pons POSIX (Phase B)
 *
 * Machina vera: termios crudus + scrinium alternum + mus SGR +
 * TIOCGWINSZ + signa (WINCH interrumpit, TSTP/CONT intermittunt et
 * resumunt, fatalia restituunt et remoriuntur) + atexit. CAPITA
 * SYSTEMATIS IN .c SOLO (mos tcp.h; probatum sub vexillis plenis
 * sine macris capacitatum - spec-v2 par 1.6).
 *
 * NIHIL redditur si stdin non terminal est (sine capite = pons
 * memoriae, expressus).
 *
 * STATICUM UNUM SANCTUM (CLAUDE.md pin): termios servatus + chorda
 * restitutionis praefixa pro tractatoribus signorum fatalium
 * (write + tcsetattr - ambo async-signal-tuta).
 */

#ifndef TESSERA_PONS_POSIX_H
#define TESSERA_PONS_POSIX_H

TesseraPons*
tessera_pons_posix_creare (
    TesseraPiscina* piscina);

#endif /* TESSERA_PONS_POSIX_H */

/* ================= ex tessera/fontes/tessera_eventum.h ================= */
/* tessera_eventum.h - Eventa initus + lector (Phase B)
 *
 * Exemplar clavium XTERM VETUS ET DEPERDITUM (interview: classic
 * lossy) - ambiguitates documentatae, non celatae:
 *   \r ET \n -> REDITUS; 0x08 ET 0x7F -> RETRORSUM;
 *   Ctrl+I == TABULA; Ctrl+littera -> runa + IMPERIUM;
 *   maiuscula in litteris invisibilis (runa ipsa eam fert).
 * NB sub ponte posix Ctrl-Z clavem NON dat - SIGTSTP verum generat
 * (VSUSP activum, VINTR/VQUIT vetita: Ctrl-C clavis 0x03 manet);
 * 0x1A ut clavis solum per pontes sine ISIG (memoriae) advenit.
 * Series agnitae: CSI (frecce A-D, H/F, Z = tabula retro, ~-codices:
 * 2 insertio, 3 deletio, 5/6 paginae, 11-15/17-21/23/24 = F1-F12,
 * modificatores 1;m), SS3 (ESC O: frecce + F1-F4), ESC+clavis =
 * ALTERUM, mus SGR (ESC [ < btn;x;y M/m; rota = btn&64; coordinatae
 * 1-basatae -> 0-basatae). CSI ignota TACITE consumuntur (strepitus
 * regiminis clavem phantasma fieri non debet).
 *
 * RUNA = codepoint DECODITUS (non compactus!): initus comparationes
 * et mathematicam casus vult; cellula.signum compactus manet -
 * nomina diversa confusionem vetant.
 *
 * LECTOR: buffer gestationis 64 octetorum (series trans lectiones
 * scissae accumulantur); ESC solum per moram sequentem ~25ms
 * disambiguatur; amplitudo pontis quaque exspectatione rogatur
 * (AMPLITUDO eventum - SIGWINCH select solum interrumpit);
 * resumptum pontis rogatur (RESUMPTUM eventum) - tractator
 * utriusque = tessera_magnitudinem_renovare + pictura.
 */

#ifndef TESSERA_EVENTUM_H
#define TESSERA_EVENTUM_H

#define TESSERA_LECTOR_BUFFER 64
#define TESSERA_MORA_FUGAE_MS 25

#define TESSERA_MODIFICATOR_IMPERIUM  0x01  /* ctrl */
#define TESSERA_MODIFICATOR_ALTERUM   0x02  /* alt/meta */
#define TESSERA_MODIFICATOR_MAIUSCULA 0x04  /* shift (ubi noscibile) */

TesseraLector*
tessera_lector_creare (
        TesseraPiscina* piscina,
    TesseraPons* pons);

/* Eventum proximum intra moram (ms); mora < 0 = sine fine.
 * NIHIL genus = mora exacta. */
TesseraEventumGenus
tessera_eventum_expectare (
     TesseraLector* lector,
    TesseraEventum* eventum,
               s32  mora_ms);

#endif /* TESSERA_EVENTUM_H */

/* ================= ex tessera/fontes/tessera_opus.h ================= */
/* tessera_opus.h - Opus tessellatum: contextus, pictura, praesentatio
 * (Phase A)
 *
 * OPUS = scrinium ut musivum. Exemplar quadri: pinge totum tergum,
 * praesentare confert tergum cum fronte et effugia minima emittit
 * (differentia IPSA est persecutio damni - termbox modo). Crates
 * passu MAXIMO indexatae (cellulae numquam moventur, renovatio
 * magnitudinis nihil reallocat); aedificator praedimensus et per
 * spectare+reset reusatus - status stabilis NIHIL allocat (assertio
 * apicis in probationibus).
 *
 * Cursor machinae celatus defalta; tessera_cursorem_ponere optatum
 * ponit, praesentare in FINE quadri applicat.
 */

#ifndef TESSERA_OPUS_H
#define TESSERA_OPUS_H

#define TESSERA_LATITUDO_MAXIMA 512
#define TESSERA_ALTITUDO_MAXIMA 256

/* Pons REQUISITUS in Phase A (defalta posix = Phase B) */
TesseraOpus*
tessera_aperire (
        TesseraPiscina* piscina,
    TesseraPons* pons);
vacuum
tessera_claudere (
    TesseraOpus* opus);

/* Intermissio (effusio ad $EDITOR etc.): scrinium restituitur
 * (reditio SGR + cursor + exire); resumere intrat et picturam
 * plenam cogit. Per pontem solum - probabile contra memoriam. */
vacuum
tessera_intermittere (
    TesseraOpus* opus);
vacuum
tessera_resumere (
    TesseraOpus* opus);

i32
tessera_latitudo (
    constans TesseraOpus* opus);
i32
tessera_altitudo (
    constans TesseraOpus* opus);

/* Regionem activam implere (signum 0 = vacuum, stilus datus) */
vacuum
tessera_purgare (
      TesseraOpus* opus,
    TesseraStilus  stilus);

/* Cellulam ponere/legere (extra fines: taciturne praecisum /
 * cellula vacua redditur) */
vacuum
tessera_cellulam_ponere (
      TesseraOpus* opus,
              s32  x,
              s32  y,
              i32  signum,
    TesseraStilus  stilus);
TesseraCellula
tessera_cellulam_legere (
    constans TesseraOpus* opus,
                     s32  x,
                     s32  y);

/* Textum scribere: limites runarum UTF-8 ambulantur, quaeque runa
 * cellulam unam (latitudo 1 praesumpta); octeti regiminis et series
 * invalidae -> '?' */
vacuum
tessera_scribere (
      TesseraOpus* opus,
              s32  x,
              s32  y,
           TesseraChorda  textus,
    TesseraStilus  stilus);
vacuum
tessera_scribere_literis (
           TesseraOpus* opus,
                   s32  x,
                   s32  y,
    constans character* textus,
         TesseraStilus  stilus);

/* Ars linearis: quadrum (margo solum) + linea */
vacuum
tessera_quadrum_pingere (
          TesseraOpus* opus,
                  s32  x,
                  s32  y,
                  s32  latitudo,
                  s32  altitudo,
    TesseraLineaGenus  genus,
        TesseraStilus  stilus);
vacuum
tessera_lineam_pingere (
          TesseraOpus* opus,
                  s32  x,
                  s32  y,
                  s32  longitudo,
                  b32  verticalis,
    TesseraLineaGenus  genus,
        TesseraStilus  stilus);

/* Rectangulum replere (signum uniforme + stilus; fines tacite) -
 * 1.1: signatura a primo hospite vero confirmata (saltuarius
 * Phase A: vectis selectionis = replere alt I; interior tabellae
 * = casus rectanguli, Phase C) */
vacuum
tessera_replere (
      TesseraOpus* opus,
              s32  x,
              s32  y,
              s32  latitudo,
              s32  altitudo,
              i32  signum,
    TesseraStilus  stilus);

vacuum
tessera_cursorem_ponere (
    TesseraOpus* opus,
            s32  x,
            s32  y);

/* Differentia + emissio (una scriptio per pontem); FALSUM in
 * fractura scriptionis */
b32
tessera_praesentare (
    TesseraOpus* opus);

/* Amplitudinem ex ponte renovare (SIGWINCH Phase B hoc vocat);
 * pictura plena sequitur */
b32
tessera_magnitudinem_renovare (
    TesseraOpus* opus);

#endif /* TESSERA_OPUS_H */

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

structura TesseraPiscina {
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
               TesseraPiscina* piscina,
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

TesseraPiscina*
tessera_piscina_generare_dynamicum (
    constans character* piscinae_titulum,
        memoriae_index  mensura_alvei_initia)
{
    Alveus* alveus_primus;

    TesseraPiscina* piscina = (TesseraPiscina*)memoriae_allocare(magnitudo(TesseraPiscina));
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
tessera_piscina_destruere (
        TesseraPiscina* piscina)
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
tessera_piscina_allocare (
           TesseraPiscina* piscina,
    memoriae_index  mensura)
{
    redde _allocare_interna(piscina, mensura, I, VERUM);
}

static vacuum*
tessera_piscina_allocare_ordinatum (
           TesseraPiscina* piscina,
    memoriae_index  mensura,
    memoriae_index  ordinatio)
{
    redde _allocare_interna(piscina, mensura, ordinatio, VERUM);
}

/* ================= ex lib/chorda_aedificator.c ================= */


/* ==================================================
 * Structura ChordaAedificator - Interna
 * ================================================== */

structura TesseraChordaAedificator {
                i8* buffer;
    memoriae_index  capacitas;
    memoriae_index  offset;
           TesseraPiscina* piscina;
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
    TesseraChordaAedificator* aedificator,
       memoriae_index  necessaria)
{
    memoriae_index  capacitas_nova;
                i8* buffer_novum;

    capacitas_nova = aedificator->capacitas;
    dum (capacitas_nova < necessaria)
    {
        capacitas_nova = _proxima_capacitas(capacitas_nova);
    }

    buffer_novum = (i8*)tessera_piscina_allocare(aedificator->piscina, capacitas_nova);
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
    TesseraChordaAedificator* aedificator,
          constans i8* datum,
       memoriae_index  mensura)
{
    memoriae_index necessaria;

    si (!aedificator || !datum || mensura == ZEPHYRUM) redde mensura == ZEPHYRUM; /* Appendix vacua bona est */

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

    mensura_signed = snprintf(cstr, (memoriae_index)magnitudo(cstr), "%u", n);
    si (mensura_signed < ZEPHYRUM) redde ZEPHYRUM;

    mensura = (memoriae_index)mensura_signed;
    si (mensura >= capacitas) redde ZEPHYRUM;

    memcpy(buffer, cstr, mensura);
    redde mensura;
}


/* ==================================================
 * Creatio
 * ================================================== */

static TesseraChordaAedificator*
tessera_chorda_aedificator_creare (
           TesseraPiscina* piscina,
    memoriae_index  capacitas_initialis)
{
    TesseraChordaAedificator* aedificator;
                   i8* buffer;

    si (!piscina || capacitas_initialis == ZEPHYRUM) redde NIHIL;

    aedificator = (TesseraChordaAedificator*)tessera_piscina_allocare(
                                        piscina,
                                        magnitudo(TesseraChordaAedificator));
    si (!aedificator) redde NIHIL;

    buffer = (i8*)tessera_piscina_allocare(piscina, capacitas_initialis);
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
tessera_chorda_aedificator_appendere_character (
    TesseraChordaAedificator* aedificator,
            character  c)
{
    i8 ch = (i8)c;
    redde _appendere_interna(aedificator, &ch, I);
}


/* ==================================================
 * Appendere - Chordae
 * ================================================== */

static b32
tessera_chorda_aedificator_appendere_literis (
     TesseraChordaAedificator* aedificator,
    constans character* cstr)
{
    memoriae_index mensura;

    si (!aedificator || !cstr) redde FALSUM;

    mensura = strlen(cstr);
    redde _appendere_interna(aedificator, (constans i8*)cstr, mensura);
}

static b32
tessera_chorda_aedificator_appendere_i32 (
    TesseraChordaAedificator* aedificator,
                  i32  n)
{
                i8 buffer[CXXXII];
    memoriae_index mensura;

    si (!aedificator) redde FALSUM;

    mensura = _format_integer_i32(n, buffer, magnitudo(buffer));
    si (mensura == ZEPHYRUM) redde FALSUM;

    redde _appendere_interna(aedificator, buffer, mensura);
}

static TesseraChorda
tessera_chorda_aedificator_spectare (
    TesseraChordaAedificator* aedificator)
{
    TesseraChorda result;

    si (!aedificator || !aedificator->buffer)
    {
        result.mensura  = ZEPHYRUM;
        result.datum    = NIHIL;
    }
    alioquin
    {
        result.mensura  = (i32)aedificator->offset;
        result.datum    = aedificator->buffer;
    }

    redde result;
}


/* ==================================================
 * Cyclus Vitae
 * ================================================== */

static vacuum
tessera_chorda_aedificator_reset (
    TesseraChordaAedificator* aedificator)
{
    si (!aedificator) redde;

    aedificator->offset             = ZEPHYRUM;
    aedificator->indentatio_gradus  = ZEPHYRUM;
}

/* ================= ex lib/utf8.c ================= */

/* Mascherae pro decodendo */
#define MASCA_ASCII       0x80u  /* 10000000 */
#define MASCA_CONT        0xC0u  /* 11000000 */
#define MASCA_2BYTE       0xE0u  /* 11100000 */
#define MASCA_3BYTE       0xF0u  /* 11110000 */
#define MASCA_4BYTE       0xF8u  /* 11111000 */

#define VALOR_CONT        0x80u  /* 10xxxxxx */
#define VALOR_2BYTE       0xC0u  /* 110xxxxx */
#define VALOR_3BYTE       0xE0u  /* 1110xxxx */
#define VALOR_4BYTE       0xF0u  /* 11110xxx */

/* Codepoint maximus validus */
#define CODEPOINT_MAXIMUS 0x10FFFF

/* Surrogates (invalidi in UTF-8) */
#define SURROGATUM_INITIUM 0xD800
#define SURROGATUM_FINIS   0xDFFF

static s32
tessera_utf8_longitudo_byte (
    i8 byte)
{
    i8 b = byte;

    /* ASCII: 0xxxxxxx */
    si ((b & MASCA_ASCII) == 0)
    {
        redde 1;
    }

    /* Continuatio: 10xxxxxx - non est principalis */
    si ((b & MASCA_CONT) == VALOR_CONT)
    {
        redde 0;
    }

    /* 2 bytes: 110xxxxx */
    si ((b & MASCA_2BYTE) == VALOR_2BYTE)
    {
        redde 2;
    }

    /* 3 bytes: 1110xxxx */
    si ((b & MASCA_3BYTE) == VALOR_3BYTE)
    {
        redde 3;
    }

    /* 4 bytes: 11110xxx */
    si ((b & MASCA_4BYTE) == VALOR_4BYTE)
    {
        redde 4;
    }

    /* Invalidum: 11111xxx vel aliud */
    redde 0;
}

static b32
tessera_utf8_est_continuatio (
    i8 byte)
{
    redde ((byte & MASCA_CONT) == VALOR_CONT);
}

static s32
tessera_utf8_decodere (
    constans i8** ptr,
    constans i8*  finis)
{
    constans i8* p;
             i8  primus;
            s32  longitudo;
            s32  codepoint;
            s32  i;

    si (ptr == NIHIL || *ptr == NIHIL || *ptr >= finis)
    {
        redde -1;
    }

    p          = *ptr;
    primus     = *p;
    longitudo  = tessera_utf8_longitudo_byte(primus);

    /* Byte invalidus vel continuatio orphana */
    si (longitudo == 0)
    {
        (*ptr)++;
        redde -1;
    }

    /* Verifica satis bytes */
    si (p + longitudo > finis)
    {
        (*ptr)++;
        redde -1;
    }

    /* Decodere secundum longitudinem */
    commutatio (longitudo)
    {
        casus 1:
            /* ASCII directum */
            codepoint = (s32)primus;
            frange;

        casus 2:
            /* 110xxxxx 10xxxxxx */
            si (!tessera_utf8_est_continuatio(p[1]))
            {
                (*ptr)++;
                redde -1;
            }
            codepoint = ((s32)(primus & 0x1F) << 6)
                | ((s32)(p[1] & 0x3F));
            /* Verifica non-overlong (minimum 0x80) */
            si (codepoint < 0x80)
            {
                (*ptr) += 2;
                redde -1;
            }
            frange;

        casus 3:
            /* 1110xxxx 10xxxxxx 10xxxxxx */
            per (i = 1; i < 3; i++)
            {
                si (!tessera_utf8_est_continuatio(p[i]))
                {
                    (*ptr)++;
                    redde -1;
                }
            }
            codepoint = ((s32)(primus & 0x0F) << 12)
                | ((s32)(p[1] & 0x3F) << 6)
                | ((s32)(p[2] & 0x3F));
            /* Verifica non-overlong (minimum 0x800) */
            si (codepoint < 0x800)
            {
                (*ptr) += 3;
                redde -1;
            }
            /* Verifica non-surrogatum */
            si (codepoint >= SURROGATUM_INITIUM && codepoint <= SURROGATUM_FINIS)
            {
                (*ptr) += 3;
                redde -1;
            }
            frange;

        casus 4:
            /* 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
            per (i = 1; i < 4; i++)
            {
                si (!tessera_utf8_est_continuatio(p[i]))
                {
                    (*ptr)++;
                    redde -1;
                }
            }
            codepoint = ((s32)(primus & 0x07) << 18)
                | ((s32)(p[1] & 0x3F) << 12)
                | ((s32)(p[2] & 0x3F) << 6)
                | ((s32)(p[3] & 0x3F));
            /* Verifica non-overlong (minimum 0x10000) */
            si (codepoint < 0x10000)
            {
                (*ptr) += 4;
                redde -1;
            }
            /* Verifica intra limites Unicode */
            si (codepoint > CODEPOINT_MAXIMUS)
            {
                (*ptr) += 4;
                redde -1;
            }
            frange;

        ordinarius:
            (*ptr)++;
            redde -1;
    }

    /* Promove indicator */
    (*ptr) += longitudo;
    redde codepoint;
}

/* ================= ex tessera/fontes/tessera_cellula.c ================= */

TesseraStilus
tessera_stilus (
    i32 color_litterae,
    i32 color_fundi,
    i32 ornamenta)
{
    TesseraStilus stilus;

    stilus.color_litterae  = color_litterae;
    stilus.color_fundi     = color_fundi;
    stilus.ornamenta       = ornamenta;
    redde stilus;
}

TesseraStilus
tessera_stilus_nativus (vacuum)
{
    redde tessera_stilus(TESSERA_COLOR_NATIVUS, TESSERA_COLOR_NATIVUS,
        ZEPHYRUM);
}

b32
tessera_stilus_aequalis (
    TesseraStilus a,
    TesseraStilus b)
{
    redde (a.color_litterae == b.color_litterae
        && a.color_fundi == b.color_fundi
        && a.ornamenta == b.ornamenta) ? VERUM : FALSUM;
}

i32
tessera_signum_ex_octetis (
    constans i8* octeti,
            i32  numerus)
{
    i32 signum = ZEPHYRUM;
    i32 k;

    si (octeti == NIHIL || numerus == ZEPHYRUM || numerus > IV)
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        signum |= ((i32)octeti[k]) << (VIII * k);
    }
    redde signum;
}

i32
tessera_signum_mensura (
    i32 signum)
{
    si (signum == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si ((signum >> VIII) == ZEPHYRUM)
    {
        redde I;
    }
    si ((signum >> XVI) == ZEPHYRUM)
    {
        redde II;
    }
    si ((signum >> XXIV) == ZEPHYRUM)
    {
        redde III;
    }
    redde IV;
}

vacuum
tessera_signum_scribere (
    TesseraChordaAedificator* aed,
                  i32  signum)
{
    i32 mensura;
    i32 k;

    mensura = tessera_signum_mensura(signum);
    si (mensura == ZEPHYRUM)
    {
        tessera_chorda_aedificator_appendere_character(aed, ' ');
        redde;
    }
    per (k = ZEPHYRUM; k < mensura; k++)
    {
        tessera_chorda_aedificator_appendere_character(aed,
            (character)((signum >> (VIII * k)) & 0xFF));
    }
}

/* ================= ex tessera/fontes/tessera_pons_memoriae.c ================= */

interior s32
_legere (
    vacuum* datum,
        i8* buffer,
       i32  capacitas,
       s32  mora_ms)
{
    TesseraPonsMemoriae* pm = (TesseraPonsMemoriae*)datum;
                    i32  reliqui;
                    i32  n;

    (vacuum)mora_ms;  /* scriptum: mora numquam expectatur */
    si (pm == NIHIL || buffer == NIHIL || capacitas == ZEPHYRUM)
    {
        redde -I;
    }
    reliqui = pm->initus_mensura - pm->initus_cursor;
    si (reliqui == ZEPHYRUM)
    {
        redde ZEPHYRUM;  /* exhaustum = mora exacta */
    }
    n = (reliqui < capacitas) ? reliqui : capacitas;
    memcpy(buffer, pm->initus + pm->initus_cursor, (memoriae_index)n);
    pm->initus_cursor += n;
    redde (s32)n;
}

interior b32
_scribere (
         vacuum* datum,
    constans i8* octeti,
            i32  numerus)
{
    TesseraPonsMemoriae* pm = (TesseraPonsMemoriae*)datum;
                    i32  k;

    si (pm == NIHIL || octeti == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        tessera_chorda_aedificator_appendere_character(pm->captum,
            (character)octeti[k]);
    }
    redde VERUM;
}

interior b32
_amplitudo (
    vacuum* datum,
       i32* latitudo_out,
       i32* altitudo_out)
{
    TesseraPonsMemoriae* pm = (TesseraPonsMemoriae*)datum;

    si (pm == NIHIL || latitudo_out == NIHIL || altitudo_out == NIHIL)
    {
        redde FALSUM;
    }
    *latitudo_out = pm->latitudo;
    *altitudo_out = pm->altitudo;
    redde VERUM;
}

interior b32
_intrare (
    vacuum* datum)
{
    TesseraPonsMemoriae* pm = (TesseraPonsMemoriae*)datum;

    si (pm == NIHIL)
    {
        redde FALSUM;
    }
    pm->intratum = VERUM;
    pm->numerus_intratum++;
    redde VERUM;
}

interior b32
_resumptum (
    vacuum* datum)
{
    TesseraPonsMemoriae* pm = (TesseraPonsMemoriae*)datum;

    si (pm == NIHIL || !pm->resumendum)
    {
        redde FALSUM;
    }
    pm->resumendum = FALSUM;
    redde VERUM;
}

interior b32
_egredi (
    vacuum* datum)
{
    TesseraPonsMemoriae* pm = (TesseraPonsMemoriae*)datum;

    si (pm == NIHIL)
    {
        redde FALSUM;
    }
    pm->intratum = FALSUM;
    pm->numerus_exitum++;
    redde VERUM;
}

TesseraPonsMemoriae*
tessera_pons_memoriae_creare (
    TesseraPiscina* piscina,
        i32  latitudo,
        i32  altitudo)
{
    TesseraPonsMemoriae* pm;

    si (   piscina  == NIHIL || latitudo == ZEPHYRUM
        || altitudo == ZEPHYRUM)
    {
        redde NIHIL;
    }
    pm = (TesseraPonsMemoriae*)tessera_piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(TesseraPonsMemoriae), IV);
    si (pm == NIHIL)
    {
        redde NIHIL;
    }
    pm->piscina           = piscina;
    pm->initus            = NIHIL;
    pm->initus_mensura    = ZEPHYRUM;
    pm->initus_cursor     = ZEPHYRUM;
    pm->captum            = tessera_chorda_aedificator_creare(piscina, 16384);
    pm->latitudo          = latitudo;
    pm->altitudo          = altitudo;
    pm->intratum          = FALSUM;
    pm->resumendum        = FALSUM;
    pm->numerus_intratum  = ZEPHYRUM;
    pm->numerus_exitum    = ZEPHYRUM;
    si (pm->captum == NIHIL)
    {
        redde NIHIL;
    }
    pm->pons.datum      = pm;
    pm->pons.legere     = _legere;
    pm->pons.scribere   = _scribere;
    pm->pons.amplitudo  = _amplitudo;
    pm->pons.intrare    = _intrare;
    pm->pons.egredi     = _egredi;
    pm->pons.resumptum  = _resumptum;
    redde pm;
}

b32
tessera_pons_memoriae_initum (
    TesseraPonsMemoriae* pm,
            constans i8* octeti,
                    i32  mensura)
{
    i8* copia;

    si (pm == NIHIL || (octeti == NIHIL && mensura > ZEPHYRUM))
    {
        redde FALSUM;
    }
    copia = (i8*)tessera_piscina_allocare(pm->piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (copia == NIHIL)
    {
        redde FALSUM;
    }
    si (mensura > ZEPHYRUM)
    {
        memcpy(copia, octeti, (memoriae_index)mensura);
    }
    pm->initus          = copia;
    pm->initus_mensura  = mensura;
    pm->initus_cursor   = ZEPHYRUM;
    redde VERUM;
}

TesseraChorda
tessera_pons_memoriae_captum (
    TesseraPonsMemoriae* pm)
{
    redde tessera_chorda_aedificator_spectare(pm->captum);
}

vacuum
tessera_pons_memoriae_purgare (
    TesseraPonsMemoriae* pm)
{
    tessera_chorda_aedificator_reset(pm->captum);
}

vacuum
tessera_pons_memoriae_amplitudo (
    TesseraPonsMemoriae* pm,
                    i32  latitudo,
                    i32  altitudo)
{
    si (pm == NIHIL)
    {
        redde;
    }
    pm->latitudo = latitudo;
    pm->altitudo = altitudo;
}

/* ================= ex tessera/fontes/tessera_pons_posix.c ================= */

/* Effugia intrandi/exeundi: scrinium alternum + mus SGR */
#define INTRANDI "\033[?1049h\033[?1000h\033[?1006h"
#define EXEUNDI  "\033[?1006l\033[?1000l\033[?1049l\033[0m\033[?25h"


/* ==================================================
 * STATICUM SANCTUM: status restitutionis pro tractatoribus
 * signorum (async-signal-tuta: write + tcsetattr solum)
 * ================================================== */

hic_manens structura termios modus_pristinus;
hic_manens structura termios modus_crudus;
hic_manens volatilis sig_atomic_t vexillum_intratum = 0;
hic_manens volatilis sig_atomic_t vexillum_resumptum = 0;
hic_manens b32 tractatores_instituti = FALSUM;

interior vacuum
_restituere_cruda (vacuum)
{
    si (vexillum_intratum)
    {
        /* async-signal-tuta ambo */
        (vacuum)!write(I, EXEUNDI, magnitudo(EXEUNDI) - I);
        (vacuum)tcsetattr(ZEPHYRUM, TCSAFLUSH, &modus_pristinus);
        vexillum_intratum = 0;
    }
}

interior vacuum
_tractator_fatalis (
    signatus numerus)
{
    _restituere_cruda();
    (vacuum)signal(numerus, SIG_DFL);
    (vacuum)raise(numerus);
}

interior vacuum
_tractator_winch (
    signatus numerus)
{
    (vacuum)numerus;
    /* corpus vacuum: select EINTR accipit; lector amplitudinem
     * rogat */
}

interior vacuum
_tractator_tstp (
    signatus numerus)
{
    (vacuum)numerus;
    _restituere_cruda();
    (vacuum)signal(SIGTSTP, SIG_DFL);
    (vacuum)raise(SIGTSTP);
}

interior vacuum
_tractator_cont (
    signatus numerus)
{
    (vacuum)numerus;
    /* in crudum redire + tractatorem TSTP reinstituere */
    (vacuum)tcsetattr(ZEPHYRUM, TCSAFLUSH, &modus_crudus);
    (vacuum)!write(I, INTRANDI, magnitudo(INTRANDI) - I);
    vexillum_intratum = 1;
    (vacuum)signal(SIGTSTP, _tractator_tstp);
    vexillum_resumptum = 1;
}

interior vacuum
_ad_exitum (vacuum)
{
    _restituere_cruda();
}

interior vacuum
_tractatores_instituere (vacuum)
{
    structura sigaction actio;

    si (tractatores_instituti)
    {
        redde;
    }
    /* WINCH sine SA_RESTART: select EINTR reddit */
    actio.sa_handler = _tractator_winch;
    sigemptyset(&actio.sa_mask);
    actio.sa_flags = ZEPHYRUM;
    (vacuum)sigaction(SIGWINCH, &actio, (structura sigaction*)NIHIL);

    (vacuum)signal(SIGTSTP, _tractator_tstp);
    (vacuum)signal(SIGCONT, _tractator_cont);
    (vacuum)signal(SIGSEGV, _tractator_fatalis);
    (vacuum)signal(SIGBUS, _tractator_fatalis);
    (vacuum)signal(SIGFPE, _tractator_fatalis);
    (vacuum)signal(SIGABRT, _tractator_fatalis);
    (vacuum)signal(SIGTERM, _tractator_fatalis);
    (vacuum)signal(SIGINT, _tractator_fatalis);
    (vacuum)atexit(_ad_exitum);
    tractatores_instituti = VERUM;
}


/* ==================================================
 * Tabula functionum
 * ================================================== */

interior s32
_legere_posix (
    vacuum* datum,
        i8* buffer,
       i32  capacitas,
       s32  mora_ms)
{
    fd_set legenda;
    structura timeval mora;
    signatus fructus;

    (vacuum)datum;
    si (buffer == NIHIL || capacitas == ZEPHYRUM)
    {
        redde -I;
    }
    FD_ZERO(&legenda);
    FD_SET(ZEPHYRUM, &legenda);
    si (mora_ms >= ZEPHYRUM)
    {
        mora.tv_sec   = mora_ms / 1000;
        mora.tv_usec  = (mora_ms % 1000) * 1000;
        fructus = select(I, &legenda, (fd_set*)NIHIL, (fd_set*)NIHIL,
            &mora);
    }
    alioquin
    {
        fructus = select(I, &legenda, (fd_set*)NIHIL, (fd_set*)NIHIL,
            (structura timeval*)NIHIL);
    }
    si (fructus < ZEPHYRUM)
    {
        redde (errno == EINTR) ? ZEPHYRUM : -I;  /* WINCH = mora */
    }
    si (fructus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    {
        ssize_t n = read(ZEPHYRUM, buffer, (memoriae_index)capacitas);

        si (n < ZEPHYRUM)
        {
            redde (errno == EINTR) ? ZEPHYRUM : -I;
        }
        si (n == ZEPHYRUM)
        {
            redde -I;  /* EOF: terminal abiit */
        }
        redde (s32)n;
    }
}

interior b32
_scribere_posix (
         vacuum* datum,
    constans i8* octeti,
            i32  numerus)
{
    i32 scripti = ZEPHYRUM;

    (vacuum)datum;
    si (octeti == NIHIL)
    {
        redde FALSUM;
    }
    dum (scripti < numerus)
    {
        ssize_t n = write(I, octeti + scripti,
            (memoriae_index)(numerus - scripti));

        si (n < ZEPHYRUM)
        {
            si (errno == EINTR)
            {
                perge;
            }
            redde FALSUM;
        }
        scripti += (i32)n;
    }
    redde VERUM;
}

interior b32
_amplitudo_posix (
    vacuum* datum,
       i32* latitudo_out,
       i32* altitudo_out)
{
    structura winsize fenestra;  /* NB "magnitudo" = macro latina! */

    (vacuum)datum;
    si (latitudo_out == NIHIL || altitudo_out == NIHIL)
    {
        redde FALSUM;
    }
    si (   ioctl(I, TIOCGWINSZ, &fenestra) != ZEPHYRUM
        || fenestra.ws_col                 == ZEPHYRUM
        || fenestra.ws_row                 == ZEPHYRUM)
    {
        *latitudo_out = LXXX;   /* 80x24 refugium */
        *altitudo_out = XXIV;
        redde VERUM;
    }
    *latitudo_out = (i32)fenestra.ws_col;
    *altitudo_out = (i32)fenestra.ws_row;
    redde VERUM;
}

interior b32
_intrare_posix (
    vacuum* datum)
{
    structura termios modus;

    (vacuum)datum;
    si (tcgetattr(ZEPHYRUM, &modus) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    modus_pristinus = modus;
    cfmakeraw(&modus);
    modus.c_cc[VMIN]   = I;   /* select moram dat; read saltem unum */
    modus.c_cc[VTIME]  = ZEPHYRUM;
    /* ISIG redux sed SUSP SOLUM: Ctrl-Z SIGTSTP verum generat
     * (interview: "Ctrl-Z numquam frangit"), Ctrl-C et Ctrl-\
     * claves ordinariae manent (VINTR/VQUIT singulatim vetita -
     * cfmakeraw ISIG totum stinxerat, quo Ctrl-Z octetus 0x1A
     * merus fiebat: oculi Fran in spectaculo id invenerunt) */
    modus.c_lflag      |= (insignatus longus)ISIG;
    modus.c_cc[VINTR]  = _POSIX_VDISABLE;
    modus.c_cc[VQUIT]  = _POSIX_VDISABLE;
    si (tcsetattr(ZEPHYRUM, TCSAFLUSH, &modus) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    modus_crudus = modus;
    _tractatores_instituere();
    vexillum_intratum = 1;
    redde _scribere_posix(NIHIL, (constans i8*)INTRANDI,
        (i32)(magnitudo(INTRANDI) - I));
}

interior b32
_egredi_posix (
    vacuum* datum)
{
    (vacuum)datum;
    si (!vexillum_intratum)
    {
        redde VERUM;
    }
    (vacuum)_scribere_posix(NIHIL, (constans i8*)EXEUNDI,
        (i32)(magnitudo(EXEUNDI) - I));
    si (tcsetattr(ZEPHYRUM, TCSAFLUSH, &modus_pristinus) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    vexillum_intratum = 0;
    redde VERUM;
}

interior b32
_resumptum_posix (
    vacuum* datum)
{
    (vacuum)datum;
    si (vexillum_resumptum)
    {
        vexillum_resumptum = 0;
        redde VERUM;
    }
    redde FALSUM;
}

TesseraPons*
tessera_pons_posix_creare (
    TesseraPiscina* piscina)
{
    TesseraPons* pons;

    si (piscina == NIHIL || !isatty(ZEPHYRUM) || !isatty(I))
    {
        redde NIHIL;  /* sine terminali = pons memoriae, expressus */
    }
    pons = (TesseraPons*)tessera_piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(TesseraPons), IV);
    si (pons == NIHIL)
    {
        redde NIHIL;
    }
    pons->datum      = NIHIL;
    pons->legere     = _legere_posix;
    pons->scribere   = _scribere_posix;
    pons->amplitudo  = _amplitudo_posix;
    pons->intrare    = _intrare_posix;
    pons->egredi     = _egredi_posix;
    pons->resumptum  = _resumptum_posix;
    redde pons;
}

/* ================= ex tessera/fontes/tessera_eventum.c ================= */

nomen enumeratio {
    PARS_COMPLETUM = 0,
    PARS_INCOMPLETUM,
    PARS_VACUUM,
    PARS_PRAETERITUM
} ParsFructus;

interior vacuum
_eventum_vacare (
    TesseraEventum* ev)
{
    ev->genus          = TESSERA_EVENTUM_NIHIL;
    ev->runa           = ZEPHYRUM;
    ev->clavis         = TESSERA_CLAVIS_NULLA;
    ev->modificatores  = ZEPHYRUM;
    ev->numerus        = ZEPHYRUM;
    ev->mus_genus      = TESSERA_MUS_PRESSUS;
    ev->mus_x          = ZEPHYRUM;
    ev->mus_y          = ZEPHYRUM;
    ev->mus_pulsus     = ZEPHYRUM;
    ev->latitudo       = ZEPHYRUM;
    ev->altitudo       = ZEPHYRUM;
}

interior vacuum
_consumere (
    TesseraLector* lector,
              i32  numerus)
{
    si (numerus >= lector->mensura)
    {
        lector->mensura = ZEPHYRUM;
        redde;
    }
    memmove(lector->buffer, lector->buffer + numerus,
        (memoriae_index)(lector->mensura - numerus));
    lector->mensura -= numerus;
}

interior vacuum
_clavem_ponere (
    TesseraEventum* ev,
     TesseraClavis  clavis,
               i32  modificatores)
{
    ev->genus          = TESSERA_EVENTUM_CLAVIS;
    ev->clavis         = clavis;
    ev->modificatores  = modificatores;
}

interior vacuum
_runam_ponere (
    TesseraEventum* ev,
               s32  runa,
               i32  modificatores)
{
    ev->genus          = TESSERA_EVENTUM_CLAVIS;
    ev->runa           = runa;
    ev->modificatores  = modificatores;
}

/* Modificatores CSI (parametrum m): m-1 = bits maiuscula/alterum/
 * imperium */
interior i32
_modificatores_csi (
    s32 m)
{
    i32 fructus = ZEPHYRUM;
    s32 bits;

    si (m <= I)
    {
        redde ZEPHYRUM;
    }
    bits = m - I;
    si (bits & I)
    {
        fructus |= TESSERA_MODIFICATOR_MAIUSCULA;
    }
    si (bits & II)
    {
        fructus |= TESSERA_MODIFICATOR_ALTERUM;
    }
    si (bits & IV)
    {
        fructus |= TESSERA_MODIFICATOR_IMPERIUM;
    }
    redde fructus;
}

/* ~-codices CSI */
interior b32
_clavem_tildae (
               s32  codex,
    TesseraEventum* ev,
               i32  modificatores)
{
    commutatio (codex)
    {
        casus II:    _clavem_ponere(ev, TESSERA_CLAVIS_INSERTIO,
                         modificatores); redde VERUM;
        casus III:   _clavem_ponere(ev, TESSERA_CLAVIS_DELETIO,
                         modificatores); redde VERUM;
        casus V:     _clavem_ponere(ev, TESSERA_CLAVIS_PAGINA_SURSUM,
                         modificatores); redde VERUM;
        casus VI:    _clavem_ponere(ev, TESSERA_CLAVIS_PAGINA_DEORSUM,
                         modificatores); redde VERUM;
        casus I:     _clavem_ponere(ev, TESSERA_CLAVIS_DOMUS,
                         modificatores); redde VERUM;
        casus IV:    _clavem_ponere(ev, TESSERA_CLAVIS_FINIS,
                         modificatores); redde VERUM;
        ordinarius:  frange;
    }
    si (codex >= XI && codex <= XV)
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_FUNCTIO, modificatores);
        ev->numerus = (i32)(codex - X);          /* 11-15 = F1-F5 */
        redde VERUM;
    }
    si (codex >= XVII && codex <= XXI)
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_FUNCTIO, modificatores);
        ev->numerus = (i32)(codex - XI);         /* 17-21 = F6-F10 */
        redde VERUM;
    }
    si (codex == XXIII || codex == XXIV)
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_FUNCTIO, modificatores);
        ev->numerus = (i32)(codex - XII);        /* 23/24 = F11/F12 */
        redde VERUM;
    }
    redde FALSUM;
}

interior b32
_clavem_finalem (
         character  finalis,
    TesseraEventum* ev,
               i32  modificatores)
{
    commutatio (finalis)
    {
        casus 'A': _clavem_ponere(ev, TESSERA_CLAVIS_SURSUM,
                       modificatores); redde VERUM;
        casus 'B': _clavem_ponere(ev, TESSERA_CLAVIS_DEORSUM,
                       modificatores); redde VERUM;
        casus 'C': _clavem_ponere(ev, TESSERA_CLAVIS_DEXTRA,
                       modificatores); redde VERUM;
        casus 'D': _clavem_ponere(ev, TESSERA_CLAVIS_SINISTRA,
                       modificatores); redde VERUM;
        casus 'H': _clavem_ponere(ev, TESSERA_CLAVIS_DOMUS,
                       modificatores); redde VERUM;
        casus 'F': _clavem_ponere(ev, TESSERA_CLAVIS_FINIS,
                       modificatores); redde VERUM;
        casus 'Z': _clavem_ponere(ev, TESSERA_CLAVIS_TABULA,
                       modificatores | TESSERA_MODIFICATOR_MAIUSCULA);
                   redde VERUM;
        ordinarius: redde FALSUM;
    }
}

/* CSI: buffer[0]=ESC buffer[1]='['. Parametra numerica leguntur,
 * octetus finalis 0x40-0x7E. Mus SGR: '<' post CSI. */
interior ParsFructus
_csi_parsare (
     TesseraLector* lector,
    TesseraEventum* ev,
               i32* consumendum)
{
          i32 i        = II;
          b32 est_mus  = FALSUM;
          b32 privata  = FALSUM;
          s32 parametra[IV];
          i32 numerus_parametrorum  = ZEPHYRUM;
          s32 valor_currens         = ZEPHYRUM;
          b32 valor_visus           = FALSUM;
    character finalis               = '\0';

    si (i < lector->mensura && lector->buffer[i] == '<')
    {
        est_mus = VERUM;
        i++;
    }
    /* Grammatica CSI: parametra 0x30-0x3F, intermedia 0x20-0x2F,
     * finalis 0x40-0x7E. Praefixa privata (?, >, =) sequentiam
     * TOTAM ignotam faciunt - sed usque ad finalem scanditur
     * (\033[?1049h etc. octetos phantasma non effundit). */
    dum (i < lector->mensura)
    {
        i8 b = lector->buffer[i];

        si (b >= '0' && b <= '9')
        {
            valor_currens  = valor_currens * X + (s32)(b - '0');
            valor_visus    = VERUM;
            i++;
        }
        alioquin si (b == ';')
        {
            si (numerus_parametrorum < IV)
            {
                parametra[numerus_parametrorum++] = valor_currens;
            }
            valor_currens  = ZEPHYRUM;
            valor_visus    = FALSUM;
            i++;
        }
        alioquin si (b >= 0x30 && b <= 0x3F)
        {
            privata = VERUM;  /* ?, >, =, : - privata/ignota */
            i++;
        }
        alioquin si (b >= 0x20 && b <= 0x2F)
        {
            privata = VERUM;  /* intermedia - forma ignota nobis */
            i++;
        }
        alioquin si (b >= 0x40 && b <= 0x7E)
        {
            finalis = (character)b;
            i++;
            frange;
        }
        alioquin
        {
            /* octetus regiminis INTRA seriem: series abrupta -
             * partem visam consumere, octetum relinquere */
            *consumendum = i;
            redde PARS_PRAETERITUM;
        }
    }
    si (finalis == '\0')
    {
        redde PARS_INCOMPLETUM;
    }
    si (privata)
    {
        *consumendum = i;
        redde PARS_PRAETERITUM;  /* tota tacite consumpta */
    }
    si (valor_visus && numerus_parametrorum < IV)
    {
        parametra[numerus_parametrorum++] = valor_currens;
    }
    *consumendum = i;

    si (   est_mus && (finalis == 'M' || finalis == 'm')
        && numerus_parametrorum >= III)
    {
        s32 pulsus = parametra[ZEPHYRUM];

        ev->genus = TESSERA_EVENTUM_MUS;
        ev->mus_x = parametra[I] - I;    /* 1-basata -> 0 */
        ev->mus_y = parametra[II] - I;
        si (pulsus & LXIV)
        {
            ev->mus_genus = (pulsus & I) ? TESSERA_MUS_ROTA_DEORSUM
                                         : TESSERA_MUS_ROTA_SURSUM;
            ev->mus_pulsus = ZEPHYRUM;
        }
        alioquin
        {
            ev->mus_genus = (finalis == 'M') ? TESSERA_MUS_PRESSUS
                                             : TESSERA_MUS_SOLUTUS;
            ev->mus_pulsus = (i32)(pulsus & III);
        }
        redde PARS_COMPLETUM;
    }

    {
        i32 modificatores = (numerus_parametrorum >= II)
            ? _modificatores_csi(parametra[I]) : ZEPHYRUM;

        si (finalis == '~' && numerus_parametrorum >= I)
        {
            si (_clavem_tildae(parametra[ZEPHYRUM], ev,
                    modificatores))
            {
                redde PARS_COMPLETUM;
            }
            redde PARS_PRAETERITUM;
        }
        si (_clavem_finalem(finalis, ev, modificatores))
        {
            redde PARS_COMPLETUM;
        }
    }
    redde PARS_PRAETERITUM;  /* CSI ignota tacite consumpta */
}

/* SS3: ESC O <finalis> - frecce + F1-F4 (modus applicationis) */
interior ParsFructus
_ss3_parsare (
     TesseraLector* lector,
    TesseraEventum* ev,
               i32* consumendum)
{
    character finalis;

    si (lector->mensura < III)
    {
        redde PARS_INCOMPLETUM;
    }
    finalis       = (character)lector->buffer[II];
    *consumendum  = III;
    si (_clavem_finalem(finalis, ev, ZEPHYRUM))
    {
        redde PARS_COMPLETUM;
    }
    si (finalis >= 'P' && finalis <= 'S')
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_FUNCTIO, ZEPHYRUM);
        ev->numerus = (i32)(finalis - 'P') + I;  /* P-S = F1-F4 */
        redde PARS_COMPLETUM;
    }
    redde PARS_PRAETERITUM;
}

/* Octetus regiminis solus (non ESC) */
interior vacuum
_regimen_parsare (
                i8  b,
    TesseraEventum* ev,
               i32  modificatores)
{
    si (b == 0x0D || b == 0x0A)
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_REDITUS, modificatores);
    }
    alioquin si (b == 0x09)
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_TABULA, modificatores);
    }
    alioquin si (b == 0x08 || b == 0x7F)
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_RETRORSUM, modificatores);
    }
    alioquin si (b >= I && b <= XXVI)
    {
        _runam_ponere(ev, (s32)('a' + b - I),
            modificatores | TESSERA_MODIFICATOR_IMPERIUM);
    }
    alioquin si (b == ZEPHYRUM)
    {
        _runam_ponere(ev, (s32)' ',
            modificatores | TESSERA_MODIFICATOR_IMPERIUM);
    }
    alioquin
    {
        /* 0x1C-0x1F: imperium + symbolum */
        _runam_ponere(ev, (s32)(b | 0x40),
            modificatores | TESSERA_MODIFICATOR_IMPERIUM);
    }
}

/* Unum eventum a fronte bufferis parsare temptare */
interior ParsFructus
_parsare (
     TesseraLector* lector,
    TesseraEventum* ev)
{
             i8 primus;
            i32 consumendum = ZEPHYRUM;
    ParsFructus fructus;

    si (lector->mensura == ZEPHYRUM)
    {
        redde PARS_VACUUM;
    }
    primus = lector->buffer[ZEPHYRUM];

    si (primus == 0x1B)
    {
        si (lector->mensura == I)
        {
            redde PARS_INCOMPLETUM;  /* FUGA sola? mora dicet */
        }
        si (lector->buffer[I] == '[')
        {
            fructus = _csi_parsare(lector, ev, &consumendum);
            si (   fructus == PARS_COMPLETUM
                || fructus == PARS_PRAETERITUM)
            {
                _consumere(lector, consumendum);
            }
            redde fructus;
        }
        si (lector->buffer[I] == 'O')
        {
            fructus = _ss3_parsare(lector, ev, &consumendum);
            si (   fructus == PARS_COMPLETUM
                || fructus == PARS_PRAETERITUM)
            {
                _consumere(lector, consumendum);
            }
            redde fructus;
        }
        /* ALTERUM + clavis: octetum post ESC parsare */
        {
            i8 alter = lector->buffer[I];

            si (alter == 0x1B)
            {
                /* ESC ESC: FUGA una, altera manet */
                _clavem_ponere(ev, TESSERA_CLAVIS_FUGA, ZEPHYRUM);
                _consumere(lector, I);
                redde PARS_COMPLETUM;
            }
            si (alter < 0x20 || alter == 0x7F)
            {
                _regimen_parsare(alter, ev,
                    TESSERA_MODIFICATOR_ALTERUM);
                _consumere(lector, II);
                redde PARS_COMPLETUM;
            }
            {
                constans i8* cursor  = lector->buffer + I;
                constans i8* finis   = lector->buffer + lector->mensura;
                        s32  longitudo_runae =
                            tessera_utf8_longitudo_byte(lector->buffer[I]);
                s32 runa;

                si (   longitudo_runae > ZEPHYRUM
                    && (i32)longitudo_runae > lector->mensura - I)
                {
                    redde PARS_INCOMPLETUM;
                }
                runa = tessera_utf8_decodere(&cursor, finis);
                si (runa < ZEPHYRUM)
                {
                    /* invalidum post ESC: FUGA + octetum relinquere */
                    _clavem_ponere(ev, TESSERA_CLAVIS_FUGA, ZEPHYRUM);
                    _consumere(lector, I);
                    redde PARS_COMPLETUM;
                }
                _runam_ponere(ev, runa, TESSERA_MODIFICATOR_ALTERUM);
                _consumere(lector,
                    (i32)(cursor - lector->buffer));
                redde PARS_COMPLETUM;
            }
        }
    }

    si (primus < 0x20 || primus == 0x7F)
    {
        _regimen_parsare(primus, ev, ZEPHYRUM);
        _consumere(lector, I);
        redde PARS_COMPLETUM;
    }

    /* runa UTF-8 */
    {
        constans i8* cursor = lector->buffer;
        constans i8* finis = lector->buffer + lector->mensura;
                s32  longitudo_runae = tessera_utf8_longitudo_byte(primus);
                s32  runa;

        si (   longitudo_runae > ZEPHYRUM
            && (i32)longitudo_runae > lector->mensura)
        {
            redde PARS_INCOMPLETUM;  /* runa dimidia in fine */
        }
        runa = tessera_utf8_decodere(&cursor, finis);
        si (runa < ZEPHYRUM)
        {
            _consumere(lector, I);   /* octetus invalidus abicitur */
            redde PARS_PRAETERITUM;
        }
        _runam_ponere(ev, runa, ZEPHYRUM);
        _consumere(lector, (i32)(cursor - lector->buffer));
        redde PARS_COMPLETUM;
    }
}

TesseraLector*
tessera_lector_creare (
        TesseraPiscina* piscina,
    TesseraPons* pons)
{
    TesseraLector* lector;

    si (piscina == NIHIL || pons == NIHIL)
    {
        redde NIHIL;
    }
    lector = (TesseraLector*)tessera_piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(TesseraLector), IV);
    si (lector == NIHIL)
    {
        redde NIHIL;
    }
    lector->pons     = pons;
    lector->mensura  = ZEPHYRUM;
    si (!pons->amplitudo(pons->datum, &lector->latitudo_nota,
            &lector->altitudo_nota))
    {
        lector->latitudo_nota = ZEPHYRUM;
        lector->altitudo_nota = ZEPHYRUM;
    }
    redde lector;
}

TesseraEventumGenus
tessera_eventum_expectare (
     TesseraLector* lector,
    TesseraEventum* eventum,
               s32  mora_ms)
{
    ParsFructus fructus;

    si (lector == NIHIL || eventum == NIHIL)
    {
        redde TESSERA_EVENTUM_NIHIL;
    }
    _eventum_vacare(eventum);

    /* Resumptio? (roga-et-purga; NIHIL licet) */
    si (   lector->pons->resumptum != NIHIL
        && lector->pons->resumptum(lector->pons->datum))
    {
        eventum->genus = TESSERA_EVENTUM_RESUMPTUM;
        redde eventum->genus;
    }

    /* Amplitudo mutata? (interrogatio - SIGWINCH select solum
     * interrumpit) */
    {
        i32 lat;
        i32 alt;

        si (   lector->pons->amplitudo(lector->pons->datum, &lat, &alt)
            && (lat != lector->latitudo_nota
                || alt != lector->altitudo_nota))
        {
            lector->latitudo_nota  = lat;
            lector->altitudo_nota  = alt;
            eventum->genus         = TESSERA_EVENTUM_AMPLITUDO;
            eventum->latitudo      = lat;
            eventum->altitudo      = alt;
            redde eventum->genus;
        }
    }

    /* Octeti gestati primum */
    dum (VERUM)
    {
        fructus = _parsare(lector, eventum);
        si (fructus == PARS_COMPLETUM)
        {
            redde eventum->genus;
        }
        si (fructus == PARS_PRAETERITUM)
        {
            perge;  /* strepitus consumptus; iterum */
        }
        frange;  /* VACUUM aut INCOMPLETUM: legendum */
    }

    /* Legere (mora vocantis), deinde parsare iterum */
    {
        s32 n = lector->pons->legere(lector->pons->datum,
            lector->buffer + lector->mensura,
            (i32)TESSERA_LECTOR_BUFFER - lector->mensura, mora_ms);

        si (n > ZEPHYRUM)
        {
            lector->mensura += (i32)n;
        }
    }
    dum (VERUM)
    {
        fructus = _parsare(lector, eventum);
        si (fructus == PARS_COMPLETUM)
        {
            redde eventum->genus;
        }
        si (fructus == PARS_PRAETERITUM)
        {
            perge;
        }
        frange;
    }

    /* INCOMPLETUM: mora fugae (~25ms) pro reliquo seriei */
    si (fructus == PARS_INCOMPLETUM)
    {
        s32 n = lector->pons->legere(lector->pons->datum,
            lector->buffer + lector->mensura,
            (i32)TESSERA_LECTOR_BUFFER - lector->mensura,
            TESSERA_MORA_FUGAE_MS);

        si (n > ZEPHYRUM)
        {
            lector->mensura += (i32)n;
            dum (VERUM)
            {
                fructus = _parsare(lector, eventum);
                si (fructus == PARS_COMPLETUM)
                {
                    redde eventum->genus;
                }
                si (fructus == PARS_PRAETERITUM)
                {
                    perge;
                }
                frange;
            }
        }
        si (fructus == PARS_INCOMPLETUM)
        {
            /* mora exacta: ESC solum = FUGA; runa dimidia abicitur */
            si (lector->buffer[ZEPHYRUM] == 0x1B)
            {
                _clavem_ponere(eventum, TESSERA_CLAVIS_FUGA,
                    ZEPHYRUM);
                _consumere(lector, I);
                redde eventum->genus;
            }
            _consumere(lector, I);
        }
    }
    redde TESSERA_EVENTUM_NIHIL;
}

/* ================= ex tessera/fontes/tessera_opus.c ================= */

/* Cellula vacua: signum 0, colores nativi, sine ornamentis.
 * Pictura prima cellulas HUIC aequales praeterit (ED 2J eas iam
 * pinxit). */
hic_manens constans TesseraCellula CELLULA_VACUA = {
    ZEPHYRUM, TESSERA_COLOR_NATIVUS, TESSERA_COLOR_NATIVUS, ZEPHYRUM
};

interior b32
_cellulae_aequales (
    constans TesseraCellula* a,
    constans TesseraCellula* b)
{
    redde (a->signum == b->signum
        && a->color_litterae == b->color_litterae
        && a->color_fundi == b->color_fundi
        && a->ornamenta == b->ornamenta) ? VERUM : FALSUM;
}

interior b32
_in_finibus (
    constans TesseraOpus* opus,
                     s32  x,
                     s32  y)
{
    redde (x >= ZEPHYRUM && y >= ZEPHYRUM
        && x < (s32)opus->latitudo && y < (s32)opus->altitudo)
        ? VERUM : FALSUM;
}

interior i32
_index (
    s32 x,
    s32 y)
{
    redde (i32)y * TESSERA_LATITUDO_MAXIMA + (i32)x;
}

/* CUP: "\033[<y+1>;<x+1>H" (1-basatum) */
interior vacuum
_positum_emittere (
    TesseraChordaAedificator* aed,
                  i32  x,
                  i32  y)
{
    tessera_chorda_aedificator_appendere_literis(aed, "\033[");
    tessera_chorda_aedificator_appendere_i32(aed, y + I);
    tessera_chorda_aedificator_appendere_character(aed, ';');
    tessera_chorda_aedificator_appendere_i32(aed, x + I);
    tessera_chorda_aedificator_appendere_character(aed, 'H');
}

/* SGR: reditio plena + ornamenta + colores (nativus = nihil -
 * reditio nuda defaltas terminalis dat) */
interior vacuum
_stilum_emittere (
         TesseraChordaAedificator* aed,
    constans TesseraStilus* st)
{
    tessera_chorda_aedificator_appendere_literis(aed, "\033[0");
    si (st->ornamenta & TESSERA_ORNAMENTUM_CRASSUM)
    {
        tessera_chorda_aedificator_appendere_literis(aed, ";1");
    }
    si (st->ornamenta & TESSERA_ORNAMENTUM_OBSCURUM)
    {
        tessera_chorda_aedificator_appendere_literis(aed, ";2");
    }
    si (st->ornamenta & TESSERA_ORNAMENTUM_CURSIVUM)
    {
        tessera_chorda_aedificator_appendere_literis(aed, ";3");
    }
    si (st->ornamenta & TESSERA_ORNAMENTUM_SUBLINEATUM)
    {
        tessera_chorda_aedificator_appendere_literis(aed, ";4");
    }
    si (st->ornamenta & TESSERA_ORNAMENTUM_INVERSUM)
    {
        tessera_chorda_aedificator_appendere_literis(aed, ";7");
    }
    si (st->ornamenta & TESSERA_ORNAMENTUM_TRANSFIXUM)
    {
        tessera_chorda_aedificator_appendere_literis(aed, ";9");
    }
    si (st->color_litterae != TESSERA_COLOR_NATIVUS)
    {
        tessera_chorda_aedificator_appendere_literis(aed, ";38;2;");
        tessera_chorda_aedificator_appendere_i32(aed,
            (st->color_litterae >> XVI) & 0xFF);
        tessera_chorda_aedificator_appendere_character(aed, ';');
        tessera_chorda_aedificator_appendere_i32(aed,
            (st->color_litterae >> VIII) & 0xFF);
        tessera_chorda_aedificator_appendere_character(aed, ';');
        tessera_chorda_aedificator_appendere_i32(aed,
            st->color_litterae & 0xFF);
    }
    si (st->color_fundi != TESSERA_COLOR_NATIVUS)
    {
        tessera_chorda_aedificator_appendere_literis(aed, ";48;2;");
        tessera_chorda_aedificator_appendere_i32(aed,
            (st->color_fundi >> XVI) & 0xFF);
        tessera_chorda_aedificator_appendere_character(aed, ';');
        tessera_chorda_aedificator_appendere_i32(aed,
            (st->color_fundi >> VIII) & 0xFF);
        tessera_chorda_aedificator_appendere_character(aed, ';');
        tessera_chorda_aedificator_appendere_i32(aed, st->color_fundi & 0xFF);
    }
    tessera_chorda_aedificator_appendere_character(aed, 'm');
}

TesseraOpus*
tessera_aperire (
        TesseraPiscina* piscina,
    TesseraPons* pons)
{
       TesseraOpus* opus;
    memoriae_index  cellulae = (memoriae_index)TESSERA_LATITUDO_MAXIMA
        * (memoriae_index)TESSERA_ALTITUDO_MAXIMA;
    i32 latitudo;
    i32 altitudo;
    i32 k;

    si (piscina == NIHIL || pons == NIHIL)
    {
        redde NIHIL;  /* defalta posix = Phase B */
    }
    si (!pons->amplitudo(pons->datum, &latitudo, &altitudo))
    {
        redde NIHIL;
    }
    opus = (TesseraOpus*)tessera_piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(TesseraOpus), IV);
    si (opus == NIHIL)
    {
        redde NIHIL;
    }
    opus->piscina  = piscina;
    opus->pons     = pons;
    opus->frons = (TesseraCellula*)tessera_piscina_allocare_ordinatum(piscina,
        cellulae * magnitudo(TesseraCellula), IV);
    opus->tergum = (TesseraCellula*)tessera_piscina_allocare_ordinatum(
        piscina, cellulae * magnitudo(TesseraCellula), IV);
    si (opus->frons == NIHIL || opus->tergum == NIHIL)
    {
        redde NIHIL;
    }
    opus->latitudo = (latitudo > ZEPHYRUM)
        ? ((latitudo <= TESSERA_LATITUDO_MAXIMA)
            ? latitudo : TESSERA_LATITUDO_MAXIMA)
        : I;
    opus->altitudo = (altitudo > ZEPHYRUM)
        ? ((altitudo <= TESSERA_ALTITUDO_MAXIMA)
            ? altitudo : TESSERA_ALTITUDO_MAXIMA)
        : I;

    /* tergum vacuum; frons post picturam primam impletur */
    per (k = ZEPHYRUM; k < (i32)cellulae; k++)
    {
        opus->tergum[k] = CELLULA_VACUA;
    }

    /* praedimensus: cellulae activae * ~20 octeti + effugia fixa -
     * status stabilis nihil allocat (spectare + reset) */
    opus->aed = tessera_chorda_aedificator_creare(piscina,
        (memoriae_index)(opus->latitudo * opus->altitudo) * XX
            + 1024);
    si (opus->aed == NIHIL)
    {
        redde NIHIL;
    }

    opus->cursor_x                        = -I;
    opus->cursor_y                        = -I;
    opus->cursor_x_actus                  = -I;
    opus->cursor_y_actus                  = -I;
    opus->cursor_visibilis_actus          = FALSUM;
    opus->primum                          = VERUM;
    opus->fructus.cellulae_collatae       = ZEPHYRUM;
    opus->fructus.cellulae_mutatae        = ZEPHYRUM;
    opus->fructus.octeti_emissi           = ZEPHYRUM;
    opus->fructus.praesentationes         = ZEPHYRUM;
    opus->fructus.tempus_praesentandi_ms  = 0.0;

    si (!pons->intrare(pons->datum))
    {
        redde NIHIL;
    }
    redde opus;
}

vacuum
tessera_claudere (
    TesseraOpus* opus)
{
    si (opus == NIHIL)
    {
        redde;
    }
    /* SGR reditio + cursor visibilis - scrinium mundum relinquere */
    tessera_chorda_aedificator_reset(opus->aed);
    tessera_chorda_aedificator_appendere_literis(opus->aed, "\033[0m\033[?25h");
    {
        TesseraChorda visus = tessera_chorda_aedificator_spectare(opus->aed);

        opus->pons->scribere(opus->pons->datum, visus.datum,
            (i32)visus.mensura);
    }
    opus->pons->egredi(opus->pons->datum);
}

vacuum
tessera_intermittere (
    TesseraOpus* opus)
{
    si (opus == NIHIL)
    {
        redde;
    }
    tessera_chorda_aedificator_reset(opus->aed);
    tessera_chorda_aedificator_appendere_literis(opus->aed,
        "\033[0m\033[?25h");
    {
        TesseraChorda visus = tessera_chorda_aedificator_spectare(opus->aed);

        opus->pons->scribere(opus->pons->datum, visus.datum,
            (i32)visus.mensura);
    }
    opus->pons->egredi(opus->pons->datum);
}

vacuum
tessera_resumere (
    TesseraOpus* opus)
{
    si (opus == NIHIL)
    {
        redde;
    }
    opus->pons->intrare(opus->pons->datum);
    opus->primum                  = VERUM;   /* pictura plena sequitur */
    opus->cursor_x_actus          = -I;
    opus->cursor_y_actus          = -I;
    opus->cursor_visibilis_actus  = FALSUM;
}

i32
tessera_latitudo (
    constans TesseraOpus* opus)
{
    redde (opus != NIHIL) ? opus->latitudo : ZEPHYRUM;
}

i32
tessera_altitudo (
    constans TesseraOpus* opus)
{
    redde (opus != NIHIL) ? opus->altitudo : ZEPHYRUM;
}

vacuum
tessera_purgare (
      TesseraOpus* opus,
    TesseraStilus  stilus)
{
    s32 x;
    s32 y;

    si (opus == NIHIL)
    {
        redde;
    }
    per (y = ZEPHYRUM; y < (s32)opus->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < (s32)opus->latitudo; x++)
        {
            TesseraCellula* cella = &opus->tergum[_index(x, y)];

            cella->signum          = ZEPHYRUM;
            cella->color_litterae  = stilus.color_litterae;
            cella->color_fundi     = stilus.color_fundi;
            cella->ornamenta       = stilus.ornamenta;
        }
    }
}

vacuum
tessera_cellulam_ponere (
      TesseraOpus* opus,
              s32  x,
              s32  y,
              i32  signum,
    TesseraStilus  stilus)
{
    TesseraCellula* cella;

    si (opus == NIHIL || !_in_finibus(opus, x, y))
    {
        redde;  /* praecisio taciturna */
    }
    cella                  = &opus->tergum[_index(x, y)];
    cella->signum          = signum;
    cella->color_litterae  = stilus.color_litterae;
    cella->color_fundi     = stilus.color_fundi;
    cella->ornamenta       = stilus.ornamenta;
}

TesseraCellula
tessera_cellulam_legere (
    constans TesseraOpus* opus,
                     s32  x,
                     s32  y)
{
    si (opus == NIHIL || !_in_finibus(opus, x, y))
    {
        redde CELLULA_VACUA;
    }
    redde opus->tergum[_index(x, y)];
}

/* Nucleus scriptionis (parametra constantia - scribere_literis
 * qualificatorem numquam abicit) */
interior vacuum
_octetos_scribere (
      TesseraOpus* opus,
              s32  x,
              s32  y,
      constans i8* datum,
              i32  mensura,
    TesseraStilus  stilus)
{
    constans i8* cursor;
    constans i8* finis;
            s32  cx = x;

    si (opus == NIHIL || datum == NIHIL)
    {
        redde;
    }
    cursor  = datum;
    finis   = datum + mensura;
    dum (cursor < finis)
    {
        i8 primus = *cursor;

        si (primus < 0x20 || primus == 0x7F)
        {
            /* octetus regiminis -> '?' */
            tessera_cellulam_ponere(opus, cx, y, '?', stilus);
            cursor++;
        }
        alioquin
        {
            constans i8* post = cursor;
                    s32  runa = tessera_utf8_decodere(&post, finis);

            si (runa < ZEPHYRUM)
            {
                /* series invalida -> '?' (octetus unus) */
                tessera_cellulam_ponere(opus, cx, y, '?', stilus);
                cursor++;
            }
            alioquin
            {
                i32 n = (i32)(post - cursor);

                tessera_cellulam_ponere(opus, cx, y,
                    tessera_signum_ex_octetis(cursor, n), stilus);
                cursor = post;
            }
        }
        cx++;
    }
}

vacuum
tessera_scribere (
      TesseraOpus* opus,
              s32  x,
              s32  y,
           TesseraChorda  textus,
    TesseraStilus  stilus)
{
    _octetos_scribere(opus, x, y, textus.datum, textus.mensura,
        stilus);
}

vacuum
tessera_scribere_literis (
           TesseraOpus* opus,
                   s32  x,
                   s32  y,
    constans character* textus,
         TesseraStilus  stilus)
{
    si (textus == NIHIL)
    {
        redde;
    }
    _octetos_scribere(opus, x, y, (constans i8*)textus,
        (i32)strlen(textus), stilus);
}

/* Signa marginum per genus: h, v, ss, sd, is, id */
interior vacuum
_signa_lineae (
    TesseraLineaGenus  genus,
                  i32* signa)
{
    commutatio (genus)
    {
        casus TESSERA_LINEA_DUPLEX:
            signa[ZEPHYRUM] = TESSERA_SIGNUM_DUPLEX_H;
            signa[I] = TESSERA_SIGNUM_DUPLEX_V;
            signa[II] = TESSERA_SIGNUM_DUPLEX_SS;
            signa[III] = TESSERA_SIGNUM_DUPLEX_SD;
            signa[IV] = TESSERA_SIGNUM_DUPLEX_IS;
            signa[V] = TESSERA_SIGNUM_DUPLEX_ID;
            frange;
        casus TESSERA_LINEA_ROTUNDATA:
            signa[ZEPHYRUM] = TESSERA_SIGNUM_SIMPLEX_H;
            signa[I] = TESSERA_SIGNUM_SIMPLEX_V;
            signa[II] = TESSERA_SIGNUM_ROTUNDATUM_SS;
            signa[III] = TESSERA_SIGNUM_ROTUNDATUM_SD;
            signa[IV] = TESSERA_SIGNUM_ROTUNDATUM_IS;
            signa[V] = TESSERA_SIGNUM_ROTUNDATUM_ID;
            frange;
        ordinarius:
            signa[ZEPHYRUM] = TESSERA_SIGNUM_SIMPLEX_H;
            signa[I] = TESSERA_SIGNUM_SIMPLEX_V;
            signa[II] = TESSERA_SIGNUM_SIMPLEX_SS;
            signa[III] = TESSERA_SIGNUM_SIMPLEX_SD;
            signa[IV] = TESSERA_SIGNUM_SIMPLEX_IS;
            signa[V] = TESSERA_SIGNUM_SIMPLEX_ID;
            frange;
    }
}

vacuum
tessera_quadrum_pingere (
          TesseraOpus* opus,
                  s32  x,
                  s32  y,
                  s32  latitudo,
                  s32  altitudo,
    TesseraLineaGenus  genus,
        TesseraStilus  stilus)
{
    i32 signa[VI];
    s32 k;

    si (opus == NIHIL || latitudo < II || altitudo < II)
    {
        redde;
    }
    _signa_lineae(genus, signa);

    tessera_cellulam_ponere(opus, x, y, signa[II], stilus);
    tessera_cellulam_ponere(opus, x + latitudo - I, y, signa[III],
        stilus);
    tessera_cellulam_ponere(opus, x, y + altitudo - I, signa[IV],
        stilus);
    tessera_cellulam_ponere(opus, x + latitudo - I, y + altitudo - I,
        signa[V], stilus);
    per (k = I; k < latitudo - I; k++)
    {
        tessera_cellulam_ponere(opus, x + k, y, signa[ZEPHYRUM],
            stilus);
        tessera_cellulam_ponere(opus, x + k, y + altitudo - I,
            signa[ZEPHYRUM], stilus);
    }
    per (k = I; k < altitudo - I; k++)
    {
        tessera_cellulam_ponere(opus, x, y + k, signa[I], stilus);
        tessera_cellulam_ponere(opus, x + latitudo - I, y + k,
            signa[I], stilus);
    }
}

vacuum
tessera_lineam_pingere (
          TesseraOpus* opus,
                  s32  x,
                  s32  y,
                  s32  longitudo,
                  b32  verticalis,
    TesseraLineaGenus  genus,
        TesseraStilus  stilus)
{
    i32 signa[VI];
    i32 signum;
    s32 k;

    si (opus == NIHIL || longitudo <= ZEPHYRUM)
    {
        redde;
    }
    _signa_lineae(genus, signa);
    signum = verticalis ? signa[I] : signa[ZEPHYRUM];
    per (k = ZEPHYRUM; k < longitudo; k++)
    {
        tessera_cellulam_ponere(opus, verticalis ? x : (x + k),
            verticalis ? (y + k) : y, signum, stilus);
    }
}

vacuum
tessera_replere (
      TesseraOpus* opus,
              s32  x,
              s32  y,
              s32  latitudo,
              s32  altitudo,
              i32  signum,
    TesseraStilus  stilus)
{
    s32 dx;
    s32 dy;

    si (opus == NIHIL)
    {
        redde;
    }
    per (dy = ZEPHYRUM; dy < altitudo; dy++)
    {
        per (dx = ZEPHYRUM; dx < latitudo; dx++)
        {
            tessera_cellulam_ponere(opus, x + dx, y + dy, signum,
                stilus);
        }
    }
}

vacuum
tessera_cursorem_ponere (
    TesseraOpus* opus,
            s32  x,
            s32  y)
{
    si (opus == NIHIL)
    {
        redde;
    }
    opus->cursor_x = x;
    opus->cursor_y = y;
}

b32
tessera_praesentare (
    TesseraOpus* opus)
{
          clock_t t0;
              s32 pos_x = -I;
              s32 pos_y = -I;
    TesseraStilus stilus_currens;
              b32 stilus_validus  = FALSUM;
              i32 mutatae_quadri  = ZEPHYRUM;
              b32 successus       = VERUM;
              s32 x;
              s32 y;

    si (opus == NIHIL)
    {
        redde FALSUM;
    }
    t0 = clock();
    tessera_chorda_aedificator_reset(opus->aed);
    stilus_currens = tessera_stilus_nativus();

    si (opus->primum)
    {
        tessera_chorda_aedificator_appendere_literis(opus->aed,
            "\033[?25l\033[2J");
        opus->cursor_visibilis_actus  = FALSUM;
        opus->cursor_x_actus          = -I;
        opus->cursor_y_actus          = -I;
    }

    per (y = ZEPHYRUM; y < (s32)opus->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < (s32)opus->latitudo; x++)
        {
                                i32 idx     =
                                    _index(x, y);
            constans TesseraCellula* cella  = &opus->tergum[idx];
                                b32  pingenda;

            opus->fructus.cellulae_collatae++;
            si (opus->primum)
            {
                pingenda = !_cellulae_aequales(cella, &CELLULA_VACUA);
                opus->frons[idx] = *cella;
            }
            alioquin
            {
                pingenda = !_cellulae_aequales(cella,
                    &opus->frons[idx]);
            }
            si (!pingenda)
            {
                perge;
            }

            si (!(pos_x == x && pos_y == y))
            {
                _positum_emittere(opus->aed, (i32)x, (i32)y);
            }
            {
                TesseraStilus stilus_cellae;

                stilus_cellae.color_litterae  = cella->color_litterae;
                stilus_cellae.color_fundi     = cella->color_fundi;
                stilus_cellae.ornamenta       = cella->ornamenta;
                si (   !stilus_validus
                    || !tessera_stilus_aequalis(stilus_currens,
                           stilus_cellae))
                {
                    _stilum_emittere(opus->aed, &stilus_cellae);
                    stilus_currens = stilus_cellae;
                    stilus_validus = VERUM;
                }
            }
            tessera_signum_scribere(opus->aed, cella->signum);
            opus->frons[idx] = *cella;
            opus->fructus.cellulae_mutatae++;
            mutatae_quadri++;

            pos_x = x + I;
            pos_y = y;
            si (pos_x >= (s32)opus->latitudo)
            {
                pos_x = -I;  /* involutio numquam creditur */
            }
        }
    }

    /* Cursor in fine quadri applicatur */
    si (opus->cursor_x >= ZEPHYRUM && opus->cursor_y >= ZEPHYRUM)
    {
        si (   mutatae_quadri > ZEPHYRUM
            || opus->cursor_x != opus->cursor_x_actus
            || opus->cursor_y != opus->cursor_y_actus
            || !opus->cursor_visibilis_actus)
        {
            _positum_emittere(opus->aed, (i32)opus->cursor_x,
                (i32)opus->cursor_y);
            si (!opus->cursor_visibilis_actus)
            {
                tessera_chorda_aedificator_appendere_literis(opus->aed,
                    "\033[?25h");
            }
            opus->cursor_x_actus          = opus->cursor_x;
            opus->cursor_y_actus          = opus->cursor_y;
            opus->cursor_visibilis_actus  = VERUM;
        }
    }
    alioquin si (opus->cursor_visibilis_actus)
    {
        tessera_chorda_aedificator_appendere_literis(opus->aed, "\033[?25l");
        opus->cursor_visibilis_actus = FALSUM;
    }

    {
        TesseraChorda visus = tessera_chorda_aedificator_spectare(opus->aed);

        si (visus.mensura > ZEPHYRUM)
        {
            successus = opus->pons->scribere(opus->pons->datum,
                visus.datum, (i32)visus.mensura);
            opus->fructus.octeti_emissi += visus.mensura;
        }
    }
    opus->primum = FALSUM;
    opus->fructus.praesentationes++;
    opus->fructus.tempus_praesentandi_ms +=
        (f64)(clock() - t0) * 1000.0 / (f64)CLOCKS_PER_SEC;
    redde successus;
}

b32
tessera_magnitudinem_renovare (
    TesseraOpus* opus)
{
    i32 latitudo;
    i32 altitudo;

    si (   opus == NIHIL
        || !opus->pons->amplitudo(opus->pons->datum, &latitudo,
               &altitudo))
    {
        redde FALSUM;
    }
    opus->latitudo = (latitudo > ZEPHYRUM)
        ? ((latitudo <= TESSERA_LATITUDO_MAXIMA)
            ? latitudo : TESSERA_LATITUDO_MAXIMA)
        : I;
    opus->altitudo = (altitudo > ZEPHYRUM)
        ? ((altitudo <= TESSERA_ALTITUDO_MAXIMA)
            ? altitudo : TESSERA_ALTITUDO_MAXIMA)
        : I;
    opus->primum = VERUM;  /* pictura plena sequitur */
    redde VERUM;
}
