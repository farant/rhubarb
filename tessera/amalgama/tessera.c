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

/* Vexilla latitudinis (non SGR; tessera sola ea ponit): cellula prima
 * runae duarum cellularum et dimidium secundum (signum 0) */
#define TESSERA_ORNAMENTUM_LATUM        0x40
#define TESSERA_ORNAMENTUM_CONTINUATIO  0x80
#define TESSERA_ORNAMENTA_STILI         0x3F
#define TESSERA_ORNAMENTUM_GRAPHEMA     0x100 /* signum = ID graphematis */

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

/* Graphemata internata (runae U5b): limites tabulae per opus */
#define TESSERA_GRAPHEMATA_MAXIMA      16384
#define TESSERA_GRAPHEMA_OCTETI_MAXIMI 64
#define TESSERA_GRAPHEMATA_OCTETI      262144

/* Politica latitudinis graphematum (runae U5c): ex ambitu eligitur */
typedef enum TesseraPolitica {
    TESSERA_POLITICA_GRAPHEMATUM = 0,   /* regula Ghostty */
    TESSERA_POLITICA_SIMPLEX            /* ut Terminal.app: ZWJ non
                                         * iungit */
} TesseraPolitica;

/* Profunditas colorum emissionis (quadrans Q4): PLENI = 38;2 / 48;2;
 * CCLVI = 38;5 / 48;5 (cubus xterm + grisei). Cellulae RGB servant. */
typedef enum TesseraColores {
    TESSERA_COLORES_PLENI = 0,
    TESSERA_COLORES_CCLVI
} TesseraColores;

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
    unsigned char*            graphemata_octeti;      /* arena */
    unsigned int              graphemata_octeti_usi;
    unsigned int*             graphemata_initia;      /* ID -> offset */
    unsigned char*            graphemata_longitudines;
    unsigned int              graphemata_numerus;
    unsigned int*             graphemata_index;       /* ID+1, 0 vacuum */
    TesseraPolitica           politica;
    TesseraColores            colores;
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
void tessera_politicam_ponere(TesseraOpus* opus,
    TesseraPolitica politica);
TesseraPolitica tessera_politica_ambitus(void);
/* Profunditas: COLORTERM truecolor/24bit -> PLENI; TERM_PROGRAM
 * Apple_Terminal -> CCLVI; aliter PLENI */
void tessera_colores_ponere(TesseraOpus* opus, TesseraColores colores);
TesseraColores tessera_colores_ambitus(void);
unsigned int tessera_cellulae_octeti(const TesseraOpus* opus, int x, int y,
    unsigned char* exitus, unsigned int capacitas);
/* Unitatem pingendam PRIMAM [initium, finis) ad (x, y) ponere:
 * regimen C0/DEL aut octetus invalidus -> '?' (I columna); ceterum
 * graphema sub politica operis; latitudinis 0 nihil pingitur. Reddit
 * indicatorem post unitatem, latitudinem unitatis in *latitudo (x per
 * eam promovetur). initium >= finis: initium, 0; opus NULL: finis. */
const unsigned char* tessera_graphema_ponere(TesseraOpus* opus, int x,
    int y, const unsigned char* initium, const unsigned char* finis,
    TesseraStilus stilus, unsigned int* latitudo);
/* Textum scribere: unitates per tessera_graphema_ponere, x per
 * latitudinem cuiusque promotum */
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
#define TESSERA_GLUTINUM_CAPACITAS 65536  /* collector, in creatione */
#define TESSERA_MORA_GLUTINI_MS 3000      /* silentium finit glutinum */

typedef enum {
    TESSERA_EVENTUM_NIHIL = 0,
    TESSERA_EVENTUM_CLAVIS,
    TESSERA_EVENTUM_MUS,
    TESSERA_EVENTUM_AMPLITUDO,
    TESSERA_EVENTUM_RESUMPTUM,
    TESSERA_EVENTUM_GLUTINUM    /* ?2004: textus insertus, unum */
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
    TESSERA_MUS_ROTA_DEORSUM,
    TESSERA_MUS_ROTA_SINISTRORSUM,  /* 66: rota lateralis (trackpad) */
    TESSERA_MUS_ROTA_DEXTRORSUM,    /* 67 */
    TESSERA_MUS_TRACTUS             /* motus botton tento (?1002):
                                     * mus_pulsus = botton 0/1/2 */
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
    TesseraChorda       glutinum;  /* GLUTINUM: visus usque ad
                                    * exspectationem proximam */
    int                 glutinum_truncatum;
} TesseraEventum;

/* lexemator fluminis terminalis (series_terminalis vendicatus,
 * eventus B1b) - opacus; typus in tessera.c internus */
typedef struct TesseraSeriesLector TesseraSeriesLector;

/* pipeline initus (rivus_terminalis vendicatus, eventus B3a): lector
 * tesserae = proiectio Eventus eius - opacus */
typedef struct TesseraRivusTerminalis TesseraRivusTerminalis;

typedef struct TesseraLector {
    TesseraPons*  pons;
    unsigned char buffer[TESSERA_LECTOR_BUFFER];
    unsigned int  mensura;        /* octeti in rivo nondum consumpti */
    unsigned int  latitudo_nota;
    unsigned int  altitudo_nota;
    TesseraRivusTerminalis* rivus;
} TesseraLector;

TesseraLector* tessera_lector_creare(TesseraPiscina* piscina,
    TesseraPons* pons);
TesseraEventumGenus tessera_eventum_expectare(TesseraLector* lector,
    TesseraEventum* eventum, int mora_ms);

#endif /* TESSERA_H */

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
/* chorda_aedificator.h - chordas accumulare (string builder) */
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
tessera_chorda_aedificator_appendere_chorda (
    TesseraChordaAedificator* aedificator,
               TesseraChorda  s);

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
/* utf8.h - UTF-8 decodere et encodere (unicode, codepoints)
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

/*
 * utf8_codere - Codere unam runam in sequentiam UTF-8
 *   (par decodere; buffer IV bytes minimum capere debet)
 *
 * @runa: Codepoint (0..0x10FFFF, surrogata D800-DFFF exclusa)
 * @buffer: Quo bytes scribuntur (1-4)
 *
 * Redde: Numerus bytes scriptorum; 0 si runa invalida
 */
static s32
tessera_utf8_codere (
    s32  runa,
     i8* buffer);

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

/* ================= ex include/runae.h ================= */
/* runae.h - Nucleus Unicode (acervus textus, stratum primum)
 *
 * Proprietates runarum (codepoints) ex TABULIS GENERATIS e datis
 * Unicode fixis (probationes/fixa/unicode/<versio>/,
 * tools/runae_generare.sh).
 * Lapis primus: LATITUDO in cellulis terminalis. Postea hic (cum
 * trahuntur): rupturae graphematum (UAX #29), normalizatio, casus,
 * rupturae linearum, bidi. Locale (collatio, formae) NUMQUAM hic.
 *
 * Purum: nulla allocatio, nullus status mutabilis, totale.
 * Unitates in nominibus: octeti (bytes), runa (codepoint), graphema
 * (cluster), latitudo (cellulae) - numquam 'n' nudum.
 */

#ifndef RUNAE_H
#define RUNAE_H

/* Versio datorum Unicode tabularum - contractus, non ornamentum */
#define RUNAE_VERSIO "15.1.0"

/* Latitudo runae in cellulis: 0, 1 aut 2. Regula Ghostty (uucode
 * wcwidth_standalone + wcwidth_zero_in_grapheme): 0 regimina (Cc), Cs,
 * Zl, Zp, Default_Ignorable (praeter U+00AD = 1), Mn, Me, iamo Hangul
 * V/T; 2 East_Asian_Width W/F et Regional_Indicator; Mc = 1;
 * modificatores emoji et Prepend latitudinem suam servant; ceterae 1.
 * TOTALIS: runa invalida (< 0 aut > U+10FFFF) = 1 (U+FFFD pingitur). */
static i32
tessera_runae_latitudo (
    s32 runa);


/* ==================================================
 * Graphemata (UAX #29 15.1, graphemata extensa)
 * ================================================== */

/* Status rupturae a vocante possessus (nulla allocatio): paritas
 * indicatorum regionum, series Extended_Pictographic Extend* (ZWJ),
 * series InCB consonans [extend/linker]* (linker visus). Initiandus
 * per runae_rupturam_initiare ante primam vocationem. */
nomen structura {
    i32 status;
} RunaeRuptura;

static vacuum
tessera_runae_rupturam_initiare (
    RunaeRuptura* ruptura);

/* VERUM si limes graphematis inter prior et runa. Vocanda SEQUENTER
 * per omnes paria contigua (status priorem quisque vocatione
 * accipit). UAX #29 pura (GB3-GB13, GB9c); runa invalida utrimque
 * rumpit (ut Control). */
static b32
tessera_runae_rumpitur (
             s32  prior,
             s32  runa,
    RunaeRuptura* ruptura);

/* Politica latitudinis graphematum: GRAPHEMATUM = regula Ghostty
 * (modus 2027, ordinaria); SIMPLEX = ut Terminal.app (mensuratum
 * 2026-09-28, aspectibus duobus): ZWJ pictographa NON iungit (quodque
 * emoji graphema suum, latitudo <= II). Cetera eadem - etiam signum
 * spatians (Mc) amplificat: Terminal.app hi II cellulas dat. */
nomen enumeratio {
    RUNAE_POLITICA_GRAPHEMATUM = 0,
    RUNAE_POLITICA_SIMPLEX
} RunaePolitica;

/* Idem sub politica data (SIMPLEX: vide supra). runae_graphema_proximum
 * = politica GRAPHEMATUM. */
static constans i8*
tessera_runae_graphema_ex_politica (
      constans i8* initium,
      constans i8* finis,
    RunaePolitica  politica,
              i32* latitudo);

#endif /* RUNAE_H */

/* ================= ex include/runae_tabulae.h ================= */
/* runae_tabulae.h - Tabulae GENERATAE runarum (runae)
 *
 * lib/runae_tabulae.c a tools/runae_generare.sh scribitur ex datis
 * Unicode fixis; hoc caput manu scriptum et stabile est. Tabula duorum
 * graduum: GRADUS_PRIMUS[runa >> VIII] = index truncii; truncus
 * CCLVI octetorum in GRADUS_SECUNDUS (trunci identici semel servati).
 * Valor octeti: bits 0-1 latitudo (0-II); bits 2-6 classis rupturae
 * graphematum (RunaeClassis); bit 7 basis variationis emoji
 * (emoji-variation-sequences.txt). Caput hoc et generatori et
 * bibliothecae commune est (una sedes dispositionis).
 */

#ifndef RUNAE_TABULAE_H
#define RUNAE_TABULAE_H

#define RUNAE_TRUNCUS 256
#define RUNAE_TRUNCI_PRIMI 4352          /* 0x110000 / RUNAE_TRUNCUS */
#define RUNAE_LATITUDO_MASCULA 0x03
#define RUNAE_CLASSIS_POSITIO  2
#define RUNAE_CLASSIS_MASCULA  0x7C
#define RUNAE_BASIS_VARIATIONIS 0x80

/* Classis rupturae: Grapheme_Cluster_Break cum InCB, Emoji_Modifier et
 * Extended_Pictographic in unum alphabetum conflatis (Unicode 15.1:
 * Extended_Pictographic et InCB Consonant semper GCB Other; InCB
 * Extend in GCB Extend aut ZWJ; InCB Linker et Emoji_Modifier in GCB
 * Extend - generator haec ASSERIT). Valores numquam interponendi. */
nomen enumeratio {
    RUNAE_CLASSIS_ALIA = 0,         /* Other */
    RUNAE_CLASSIS_CR,
    RUNAE_CLASSIS_LF,
    RUNAE_CLASSIS_REGIMEN,          /* Control */
    RUNAE_CLASSIS_EXTENSIO,         /* Extend, InCB None */
    RUNAE_CLASSIS_EXTENSIO_INCB,    /* Extend, InCB Extend */
    RUNAE_CLASSIS_CONIUNCTOR,       /* Extend, InCB Linker */
    RUNAE_CLASSIS_MODIFICATOR,      /* Extend, Emoji_Modifier */
    RUNAE_CLASSIS_IUNCTOR,          /* ZWJ (InCB Extend) */
    RUNAE_CLASSIS_REGIONIS,         /* Regional_Indicator */
    RUNAE_CLASSIS_PRAEPOSITUM,      /* Prepend */
    RUNAE_CLASSIS_SPATIANS,         /* SpacingMark */
    RUNAE_CLASSIS_SYLLABA_INITIALIS,    /* L */
    RUNAE_CLASSIS_SYLLABA_MEDIA,        /* V */
    RUNAE_CLASSIS_SYLLABA_FINALIS,      /* T */
    RUNAE_CLASSIS_SYLLABA_APERTA,       /* LV */
    RUNAE_CLASSIS_SYLLABA_CLAUSA,       /* LVT */
    RUNAE_CLASSIS_PICTOGRAPHUM,     /* Extended_Pictographic */
    RUNAE_CLASSIS_CONSONANS         /* InCB Consonant */
} RunaeClassis;

externus constans i16 TESSERA_RUNAE_GRADUS_PRIMUS[RUNAE_TRUNCI_PRIMI];
externus constans i8  TESSERA_RUNAE_GRADUS_SECUNDUS[];

#endif /* RUNAE_TABULAE_H */

/* ================= ex include/series_terminalis.h ================= */
/* series_terminalis.h - Lexemator fluminis terminalis (DEC/Williams)
 *
 * Grammatica octetorum terminalis, utraque directione: quod applicatio
 * scribit (emulator consumit) et quod terminalis reddit (claves, mus,
 * glutinum, responsa). Octeti intrant, LEXEMATA exeunt. SENSUM non
 * possidet: quid CSI 'A' significet (sursum? cursor?) strata superiora
 * decernunt. Nullum tempus, nulla UTF-8, nulla allocatio post
 * creationem.
 * (eventus B1a; terminal-planning modules/002; machina vt100.net
 * dec_ansi_parser, ex Ghostty Parser.zig / parse_table.zig, MIT, pin
 * 12752b2.)
 *
 * DIVERGENTIAE A WILLIAMS/GHOSTTY (consultae; omnes initus serviunt):
 * - C1 octeti (0x80..0x9F) NON agnoscuntur: continuationes UTF-8 sunt,
 *   ut cetera imprimuntur (Franus, planum B par. III).
 * - DEL (0x7F) in solo = EXSEQUI (clavis retrorsum), non imprimere.
 * - CSI cum ':' quovis finali SERVATUR (separatores): kitty claves
 *   'CSI 97:65;2u' mittit; Ghostty eas extra 'm' abicit.
 * - ESC N / ESC O + octetus = lexema UNUM (SERIES_SS; parametra
 *   licent: 'ESC O 2 P'). Ghostty ESC O statim mittit.
 * - ESC ante seriem (ESC ESC [ A) non perit: 'praefixum' VERUM.
 * - Series abrupta (ESC, CAN, SUB in medio; ESC + octetus altus) =
 *   SERIES_FUGA cum octetis crudis, NON tacite abiecta; octetus
 *   abrumpens NON consumitur (vocatio proxima eum tractat).
 * - 'ESC \' post chordam (OSC/DCS/APC) = terminator, consumptum; nullum
 *   lexema ESC '\' sequitur.
 * - DCS TOTUM (non hook/put/unhook) et truncatum (Franus).
 * Ut Ghostty: plus quam SERIES_PARAMETRA_MAXIMA parametra -> series
 * TOTA abicitur (csi_ignore) - SGR dimidia peior est quam nulla.
 *
 * VISUS: 'textus' et 'crudum' valent usque ad vocationem proximam
 * (IMPRIMERE: visus in initum vocantis, valet dum initus).
 *
 * TEMPUS: lector solum 'pendet' nuntiat; vocans (lector initus) cum
 * mora
 * sua 'evacuare' vocat ut ESC solum aut seriem dimidiam reddat.
 */

#ifndef SERIES_TERMINALIS_H
#define SERIES_TERMINALIS_H

#define SERIES_PARAMETRA_MAXIMA   XXIV         /* Ghostty MAX_PARAMS */
#define SERIES_INTERMEDIA_MAXIMA  IV           /* Ghostty */
#define SERIES_CHORDA_MAXIMA      (II * MXXIV) /* osc.zig */
#define SERIES_CRUDUM_MAXIMUM     LXIV         /* octeti crudi seriei */
#define SERIES_PARAMETRUM_MAXIMUM 0x7FFFFFFF   /* saturatio */

nomen enumeratio {
    SERIES_NIHIL = ZEPHYRUM,  /* initus consumptus, nullum lexema */
    SERIES_IMPRIMERE,         /* cursus imprimibilis (visus) */
    SERIES_EXSEQUI,           /* C0 aut DEL; octetus in 'finale' */
    SERIES_ESC,               /* ESC + intermedia + finale */
    SERIES_CSI,               /* ESC [ privatum? parametra intermedia
                               * finale */
    SERIES_SS,                /* ESC N|O parametra? finale (SS2/SS3) */
    SERIES_OSC,               /* ESC ] corpus BEL|ST (textus) */
    SERIES_DCS,               /* ESC P parametra intermedia finale
                               * corpus ST (textus) */
    SERIES_APC,               /* ESC _ | ^ | X corpus ST (introductor
                               * dicit APC, PM, SOS) */
    SERIES_FUGA               /* series abrupta aut evacuata: crudum */
} SeriesGenus;

nomen structura {
    SeriesGenus genus;
            s32 parametra[SERIES_PARAMETRA_MAXIMA];
            i32 numerus_parametrorum;
            i32 separatores;    /* bitus i: ':' POST parametrum i */
             i8 intermedia[SERIES_INTERMEDIA_MAXIMA];
            i32 numerus_intermediorum;
             i8 privatum;       /* '?' '>' '<' '=' aut 0 */
             i8 introductor;    /* octetus post ESC ('[' ']' 'P' 'N'
                                 * 'O' '_' '^' 'X') aut 0 */
             i8 finale;         /* finale; EXSEQUI: octetus regiminis */
            b32 praefixum;      /* ESC solum seriem praecessit */
         TesseraChorda textus;         /* IMPRIMERE, OSC, DCS, APC */
         TesseraChorda crudum;         /* octeti seriei (ESC ... finale) */
            b32 truncatum;      /* corpus aut intermedia praecisa */
} SeriesLexema;

/* Lector novus in statu solo; tabula transitionum hic struitur. */
static TesseraSeriesLector*
tessera_series_lectorem_creare (
    TesseraPiscina* piscina);

/* MODUS INITUS (eventus B1b): grammatica directionis INITUS - quod
 * terminalis reddit, non quod applicatio scribit. Octeti idem ambigui
 * sunt (ESC P = DCS aut alt+P); responsa vera terminalium solum formas
 * certas habent, ergo in modo initus:
 * - ESC + (0x20..0x2F) = ESC finale (alt+spatium, alt+!), non
 *   intermedia (designatio characterum numquam ex terminali venit);
 * - ESC + C0 aut DEL = FUGA (ESC solus, octetus NON consumptus) -
 *   vocans ut alterum + regimen legit (alt+reditus, alt+retrorsum);
 * - ESC N, ESC X, ESC ^ = ESC finale (SS2, SOS, PM numquam reddita);
 * - ESC P + octetus finalis (0x40..0x7E) STATIM = ESC finale 'P' +
 *   octetus non consumptus (responsa DCS digito, '>', '!' aut '$'
 *   incipiunt; 'alt+P a' una lectione venit). ESC ] et ESC _ series
 *   manent: alt+] / alt+_ per moram vocantis (FUGA introductoris)
 *   leguntur.
 * Modus scriptionis (ordinarius, FALSUM) DEC purum manet. Purgatio
 * modum servat. */
static vacuum
tessera_series_lectorem_initus_ponere (
    TesseraSeriesLector* lector,
             b32  initus);

/* Ad statum solum redire (seriem pendentem abicere sine lexemate). */
static vacuum
tessera_series_lectorem_purgare (
    TesseraSeriesLector* lector);

/* Lexema proximum ex [*ptr, finis): *ptr promovetur. SERIES_NIHIL si
 * initus consumptus sine lexemate completo - series dimidia in statu
 * manet et vocatione proxima perficitur (scissura quaevis licet). */
static SeriesGenus
tessera_series_lexema_proximum (
     TesseraSeriesLector*  lector,
      constans i8** ptr,
      constans i8*  finis,
     SeriesLexema*  lexema);

/* VERUM si in medio seriei (ESC solum, CSI dimidia, chorda aperta). */
static b32
tessera_series_lector_pendet (
    constans TesseraSeriesLector* lector);

/* Post moram vocantis: series pendens ut SERIES_FUGA redditur (crudum,
 * introductor, praefixum), lector ad solum redit. FALSUM si nihil
 * reddendum (non pendet, aut solum 'ESC' terminatoris chordae
 * exspectabatur). */
static b32
tessera_series_lectorem_evacuare (
    TesseraSeriesLector* lector,
    SeriesLexema* lexema);

#endif /* SERIES_TERMINALIS_H */

/* ================= ex include/eventus.h ================= */
#ifndef EVENTUS_H
#define EVENTUS_H


/* ==================================================
 * Constantae - Genera Eventuum
 * ================================================== */

/* Genera eventuum */
nomen enumeratio {
    EVENTUS_NIHIL = ZEPHYRUM,
    EVENTUS_CLAUDERE,
    EVENTUS_MUTARE_MAGNITUDINEM,
    EVENTUS_FOCUS,
    EVENTUS_DEFOCUS,
    EVENTUS_EXPONERE,
    EVENTUS_CLAVIS_DEPRESSUS,
    EVENTUS_CLAVIS_LIBERATUS,
    EVENTUS_MUS_DEPRESSUS,
    EVENTUS_MUS_LIBERATUS,
    EVENTUS_MUS_MOTUS,
    EVENTUS_MUS_ROTULA,
    EVENTUS_MUS_DUPLEX,         /* Double-click (derivatum) */
    /* Derivata a dispensatore (ludus): numquam a fenestra
     * emissa. Ordo = tabula titulorum eventus_stml.c. */
    EVENTUS_MUS_INTRAVIT,
    EVENTUS_MUS_EXIIT,
    EVENTUS_FOCUS_CAPTUS,
    EVENTUS_FOCUS_AMISSUS,
    EVENTUS_FOCUS_PETITUS,
    /* res menu applicationis pressa (fenestra_menu_addere) */
    EVENTUS_MENU,
    /* Vocabularium sine iactura (eventus A2; spec par. III) - ADDITA
     * ad finem: tituli in plagulis, ordo tabulae titulorum crescit. */
    EVENTUS_TEXTUS,          /* textus commissus aut componens */
    EVENTUS_DEPOSITIO,       /* viae depositae (aut glutinum promotum) */
    EVENTUS_SUSPENSIO,       /* processus suspensus (terminalis) */
    EVENTUS_RESUMPTIO,       /* processus resumptus */
    EVENTUS_FACULTATES,      /* facultates fontis (primus; et mutatae) */
    /* Tractus DERIVATI (eventus A5; spec D4): a ludo (derivare) solum,
     * numquam a fonte. INCIPIT: x/y = ORIGO (ubi pressio fuit), tempus
     * motus qui limen transiit; TRACTUS: positio currens; FINIT:
     * positio liberationis. botton = botton pressionis. */
    EVENTUS_TRACTUS_INCIPIT,
    EVENTUS_TRACTUS,
    EVENTUS_TRACTUS_FINIT
} tessera_eventus_genus_t;


/* ==================================================
 * Constantae - Codices Clavium
 * ================================================== */

/* Codices clavium */
nomen enumeratio {
    CLAVIS_IGNOTA = ZEPHYRUM,

    /* Characteres ASCII imprimibiles (32-126) sunt valores ASCII eorum */
    CLAVIS_SPATIUM = XXXII,

    /* Characteres imperantes */
    CLAVIS_EFFUGIUM = XXVII,
    CLAVIS_REDITUS = XIII,
    CLAVIS_TABULA = IX,
    CLAVIS_RETRORSUM = VIII,
    CLAVIS_DELERE = CXXVII,

    /* Claves navigationis (256+) */
    CLAVIS_SINISTER = CCLVI,
    CLAVIS_DEXTER,
    CLAVIS_SURSUM,
    CLAVIS_DEORSUM,
    CLAVIS_DOMUS,
    CLAVIS_FINIS,
    CLAVIS_PAGINA_SURSUM,
    CLAVIS_PAGINA_DEORSUM,

    /* Claves functionis */
    CLAVIS_F1 = CCXC,
    CLAVIS_F2,
    CLAVIS_F3,
    CLAVIS_F4,
    CLAVIS_F5,
    CLAVIS_F6,
    CLAVIS_F7,
    CLAVIS_F8,
    CLAVIS_F9,
    CLAVIS_F10,
    CLAVIS_F11,
    CLAVIS_F12,

    /* Claves modificantes */
    CLAVIS_SINISTER_SHIFT = CCCXL,
    CLAVIS_DEXTER_SHIFT,
    CLAVIS_SINISTER_IMPERIUM,
    CLAVIS_DEXTER_IMPERIUM,
    CLAVIS_SINISTER_ALT,
    CLAVIS_DEXTER_ALT,
    CLAVIS_SINISTER_SUPER,
    CLAVIS_DEXTER_SUPER,
    CLAVIS_CAPS_LOCK,
    CLAVIS_NUM_LOCK
} clavis_t;


/* ==================================================
 * Constantae - Vexilla Modificantium
 * ================================================== */

/* Vexilla modificantium pro eventibus clavis/muris */
nomen enumeratio {
    MOD_SHIFT     = 0x020000,
    MOD_IMPERIUM  = 0x040000,
    MOD_ALT       = 0x080000,
    MOD_SUPER     = 0x100000,
    MOD_CAPS_LOCK = 0x010000,
    MOD_NUM_LOCK  = 0x200000
} mod_vexilla_t;


/* ==================================================
 * Constantae - Bottones Muris
 * ================================================== */

/* Bottones muris */
nomen enumeratio {
    MUS_SINISTER = I,
    MUS_DEXTER   = II,
    MUS_MEDIUS   = III
} mus_botton_t;


/* ==================================================
 * Modificantes LATERUM (eventus A2)
 * ==================================================
 * Valores = larvae macOS 'device-dependent' (NX_DEVICE*): fenestra
 * modifierFlags crudos iam fert, ergo latera iam in eventibus et
 * plagulis sunt. Conventio vocabularii: fontes alii (terminalis per
 * kitty) in has vertunt. Solum si facultas 'latera'. */
#define MOD_IMPERIUM_SINISTER  0x0001
#define MOD_SHIFT_SINISTER     0x0002
#define MOD_SHIFT_DEXTER       0x0004
#define MOD_SUPER_SINISTER     0x0008
#define MOD_SUPER_DEXTER       0x0010
#define MOD_ALT_SINISTER       0x0020
#define MOD_ALT_DEXTER         0x0040
#define MOD_IMPERIUM_DEXTER    0x2000


/* ==================================================
 * Codex PHYSICUS clavis (spec D3: subsectio W3C 'code')
 * ==================================================
 * Positio clavis, non dispositio (clavis logica in 'clavis' et
 * 'runa'). Tituli plagularum = nomina W3C ("KeyA", "ArrowLeft") per
 * eventus_codex_titulus (eventus_stml.h). Litterae et numeri ut
 * ORDINES: EVENTUS_CODEX_LITTERAE + (c - 'A'), EVENTUS_CODEX_NUMERI
 * + d, EVENTUS_CODEX_FUNCTIONES + (n - 1) pro F1..F12. */

nomen enumeratio {
    EVENTUS_CODEX_IGNOTUS = ZEPHYRUM,
    EVENTUS_CODEX_LITTERAE,                          /* KeyA .. KeyZ */
    EVENTUS_CODEX_NUMERI = EVENTUS_CODEX_LITTERAE + XXVI, /* Digit0..9 */
    EVENTUS_CODEX_FUNCTIONES = EVENTUS_CODEX_NUMERI + X,  /* F1..F12 */
    EVENTUS_CODEX_SPATIUM = EVENTUS_CODEX_FUNCTIONES + XII,
    EVENTUS_CODEX_REDITUS,
    EVENTUS_CODEX_TABULA,
    EVENTUS_CODEX_RETRORSUM,
    EVENTUS_CODEX_EFFUGIUM,
    EVENTUS_CODEX_SAGITTA_SINISTRA,
    EVENTUS_CODEX_SAGITTA_DEXTRA,
    EVENTUS_CODEX_SAGITTA_SURSUM,
    EVENTUS_CODEX_SAGITTA_DEORSUM,
    EVENTUS_CODEX_DOMUS,
    EVENTUS_CODEX_FINIS,
    EVENTUS_CODEX_PAGINA_SURSUM,
    EVENTUS_CODEX_PAGINA_DEORSUM,
    EVENTUS_CODEX_DELERE,
    EVENTUS_CODEX_INSERERE,
    EVENTUS_CODEX_GRAVIS,             /* Backquote */
    EVENTUS_CODEX_MINUS,
    EVENTUS_CODEX_AEQUALE,
    EVENTUS_CODEX_UNCUS_SINISTER,     /* BracketLeft */
    EVENTUS_CODEX_UNCUS_DEXTER,
    EVENTUS_CODEX_VIRGULA_INVERSA,    /* Backslash */
    EVENTUS_CODEX_PUNCTUM_VIRGULA,    /* Semicolon */
    EVENTUS_CODEX_APOSTROPHUS,        /* Quote */
    EVENTUS_CODEX_VIRGULA,            /* Comma */
    EVENTUS_CODEX_PUNCTUM,            /* Period */
    EVENTUS_CODEX_VIRGULA_OBLIQUA,    /* Slash */
    EVENTUS_CODEX_MAIUSCULA_SINISTRA, /* ShiftLeft */
    EVENTUS_CODEX_MAIUSCULA_DEXTRA,
    EVENTUS_CODEX_IMPERIUM_SINISTRUM, /* ControlLeft */
    EVENTUS_CODEX_IMPERIUM_DEXTRUM,
    EVENTUS_CODEX_ALTERUM_SINISTRUM,  /* AltLeft */
    EVENTUS_CODEX_ALTERUM_DEXTRUM,
    EVENTUS_CODEX_SUPER_SINISTRUM,    /* MetaLeft */
    EVENTUS_CODEX_SUPER_DEXTRUM,
    EVENTUS_CODEX_SERA_MAIUSCULARUM,  /* CapsLock */
    EVENTUS_CODICES_NUMERUS
} EventusCodex;

/* Actio clavis: ITERATA = auto-repetitio (genus DEPRESSUS manet);
 * SOLUTA in LIBERATUS. */
nomen enumeratio {
    EVENTUS_ACTIO_PRESSA = ZEPHYRUM,
    EVENTUS_ACTIO_ITERATA,
    EVENTUS_ACTIO_SOLUTA
} EventusActio;

nomen enumeratio {
    EVENTUS_INDICATOR_MUS = ZEPHYRUM,
    EVENTUS_INDICATOR_STILUS,
    EVENTUS_INDICATOR_TACTUS
} EventusIndicatorGenus;

#define EVENTUS_PRESSIO_IGNOTA (-1)   /* pressio 0..M; mus, terminalis */
#define EVENTUS_EXEMPLA_MAXIMA LXIV   /* spec D5 */

/* Exemplum motus coaliti (spec Q13): positio + tempus */
nomen structura {
    s32 x;
    s32 y;
    s64 tempus;
} EventusExemplum;

nomen enumeratio {
    EVENTUS_ROTULA_IGNOTA = ZEPHYRUM,  /* plagulae veteres (f32 sola) */
    EVENTUS_ROTULA_PRAECISA,            /* trackpad: pixela */
    EVENTUS_ROTULA_GRADATA             /* rota, terminalis: gradus */
} EventusRotulaGenus;

nomen enumeratio {
    EVENTUS_TEXTUS_COMMISSUM = ZEPHYRUM,
    EVENTUS_TEXTUS_COMPONENS           /* praeeditio IME */
} EventusTextusGenus;

nomen enumeratio {
    EVENTUS_ORIGO_SCRIPTA = ZEPHYRUM,  /* typed */
    EVENTUS_ORIGO_GLUTINATA,           /* pasted */
    EVENTUS_ORIGO_COMPOSITA            /* IME commit, dead keys */
} EventusOrigo;

/* Facultates trinae */
#define EVENTUS_FACULTAS_NULLA       ZEPHYRUM
#define EVENTUS_FACULTAS_FORTASSE    I    /* scriptura copiae: OSC 52 */
#define EVENTUS_FACULTAS_CERTA       II
#define EVENTUS_DEPOSITIO_NULLA      ZEPHYRUM
#define EVENTUS_DEPOSITIO_HEURISTICA I    /* glutinum viarum promotum */
#define EVENTUS_DEPOSITIO_NATIVA     II

/* Quod fons NARRARE potest (spec Q4, Q17): semel primus, iterum si
 * discit (per observationem, numquam per quaestionem). */
nomen structura {
    b32 liberationes;       /* SOLUTA nuntiatur */
    b32 codex_physicus;     /* 'codex' verus */
    b32 tabula_distincta;   /* Tab != Ctrl+I */
    b32 latera;             /* MOD_*_SINISTER/DEXTER */
    b32 super;              /* hover (motus sine botone) */
    b32 praeeditio;         /* TEXTUS COMPONENS */
    i32 scriptura_copiae;   /* EVENTUS_FACULTAS_* */
    i32 depositio;          /* EVENTUS_DEPOSITIO_* */
    s32 gradus_rotulae;     /* pixela nostra per gradum (GRADATA) */
    b32 pressio;            /* pressio indicatoris vera */
    /* B4: modificantes clavium imprimibilium narrantur (Shift+A non
     * solum 'A'). Legacy FALSUM (Shift ut Caps Lock videtur); kitty
     * cum vexillo OMNES VERUM; fenestra VERUM. Ad finem additum. */
    b32 modificantes_textus;
} EventusFacultates;


/* ==================================================
 * Typi - Eventus
 * ================================================== */

/* Structura eventi fenestrae */
nomen structura {
    tessera_eventus_genus_t genus;

    /* Tempus eventus in MILLISECUNDIS (s64). A fenestra stampatum in
     * productione, a plagula in replay. TEMPUS EST DATUM IN EVENTU -
     * nihil infra fenestram horologium vocat. ZEPHYRUM = nondum
     * stampatum: impellere_eventum id implet. */
    s64 tempus;
    unio {
        structura {
            i32 latitudo;
            i32 altitudo;
        } mutare_magnitudinem;
        structura {
             clavis_t clavis;        /* LOGICA (nominata; ASCII ut olim) */
            character typus;         /* DEPRECATUM (spec D2): textus in
                                      * EVENTUS_TEXTUS */
                  i32 modificantes;  /* MOD_* + latera */
                  s32 runa;          /* runa sine maiuscula (logica) */
         EventusCodex codex;         /* PHYSICUS; IGNOTUS si nescitur */
         EventusActio actio;
        } clavis;
        structura {
                     s32 x;          /* pixela NOSTRA; extra fenestram
                                      * negativa aut >= latitudo (A3c) */
                     s32 y;
            mus_botton_t botton;
                     i32 modificantes;
                     s32 indicator;           /* 0 = mus */
    EventusIndicatorGenus indicator_genus;
                     s32 pressio;             /* 0..M; IGNOTA = -1 */
    constans EventusExemplum* exempla;        /* VISUS (Q16) */
                     i32 numerus_exemplorum;
        } mus;
        structura {
            f32 delta_x;             /* DEPRECATUM (spec D2) */
            f32 delta_y;
            s32 dx;                  /* pixela nostra, integra */
            s32 dy;
            EventusRotulaGenus genus;
            /* B3a: positio indicatoris (pixela nostra, ut mus) et
             * modificantes - shift+rota, ctrl+rota (zoom); rotula ad
             * tabulam sub indicatore destinari potest */
            s32 x;
            s32 y;
            i32 modificantes;
        } rotula;
        structura {
                TesseraChorda contentum;    /* VISUS usque ad lectionem proximam */
    EventusTextusGenus genus;
                   s32 cursor;       /* COMPONENS: octetus */
          EventusOrigo origo;
                   b32 truncatum;
        } textus;
        structura {
               s32 x;
               s32 y;
            TesseraChorda viae;             /* VISUS: viae absolutae, '\n' */
               i32 numerus;
               b32 promota;          /* glutinum terminalis promotum */
        } depositio;
        EventusFacultates facultates;
        structura {
            i32 signum;         /* a fenestra_menu_addere datum */
        } menu;
    } datum;
} Eventus;

/* Eventus NOTATUS (eventus A6; spec D6): eventus crudus + destinatum
 * eius a dispensatore resolutum - notarius (dispensator_notarium_
 * ponere) scribit, non fons. scopus = id componentis (vacuus: nullus,
 * e.g. clavis), scopus_x/y = punctum in spatio eius. Plagula:
 * eventus_notata_scribere_stml. Eventus ipse scopum non fert: scopus
 * datum fontis non est. */
nomen structura {
    Eventus eventus;
     TesseraChorda scopus;
        s32 scopus_x;
        s32 scopus_y;
} EventusNotatum;

#endif /* EVENTUS_H */

/* ================= ex include/eventus_cauda.h ================= */
/* eventus_cauda.h - Cauda eventuum fontis: anulus + onera per lectionem
 * (eventus A3; spec Q10, Q13, Q16, D5)
 *
 * PURA (sine Cocoa, sine tessera): fenestra eam habet, fons
 * terminalis (phasis B) eadem utetur. Tempus NON hic stampatur (horologium
 * platformae est): fons tempus implet ante impulsum.
 *
 * ONERA (textus, exempla motus) in tabulis INTERNIS caudae copiantur; visus
 * eventuum (datum.textus.contentum) in eas monstrant. VITA: usque ad
 * lectionem proximam (eventus_cauda_lectio_incipit) - SED tabulae
 * vacantur SOLUM si cauda vacua est: eventa nondum extracta visus suos
 * servant. Tabula plena: textus truncatur (truncatum VERUM).
 */

#ifndef EVENTUS_CAUDA_H
#define EVENTUS_CAUDA_H

#define EVENTUS_CAUDA_CAPACITAS  CCLVI       /* eventa */
#define EVENTUS_CAUDA_TEXTUS     65536       /* octeti oneris textus */
#define EVENTUS_CAUDA_EXEMPLA    (IV * MXXIV) /* exempla motus coaliti */

nomen structura {
            Eventus eventus[EVENTUS_CAUDA_CAPACITAS];
                i32 caput;
                i32 finis;
                i32 numerus;
                i32 amissa;          /* eventa abiecta: cauda plena */
                 i8 textus[EVENTUS_CAUDA_TEXTUS];
                i32 textus_mensura;
    EventusExemplum exempla[EVENTUS_CAUDA_EXEMPLA];
                i32 exempla_mensura;
} EventusCauda;

static vacuum
tessera_eventus_caudam_initiare (
    EventusCauda* cauda);

/* Initium lectionis fontis (fenestra: perscrutari). Si cauda vacua:
 * tabulae onerum vacantur (visus priores iam consumpti). */
static vacuum
tessera_eventus_cauda_lectio_incipit (
    EventusCauda* cauda);

/* Eventum (valore) addere; FALSUM si plena (amissa++). */
static b32
tessera_eventus_caudae_impellere (
          EventusCauda* cauda,
      constans Eventus* eventus);

/* EVENTUS_TEXTUS COMMISSUM cum octetis COPIATIS addere. Mensura 0 ->
 * nihil, FALSUM. Tabula plena -> pars quae capit, truncatum VERUM. */
static b32
tessera_eventus_caudae_textum_impellere (
       EventusCauda* cauda,
                s64  tempus,
        constans i8* octeti,
                i32  mensura,
       EventusOrigo  origo);

/* EVENTUS_MUS_MOTUS cum COALITIONE (spec Q10, Q13, D5): si eventus
 * ULTIMUS caudae (nondum extractus) MOTUS est cum eisdem bottone,
 * modificantibus, indicatore - positio eius (x, y, tempus) exemplum
 * fit et novus eam supplet; alioquin impellitur ut eventus novus.
 * Exempla eventus unius CONTIGUA sunt (solum ultimus crescit), visus
 * in tabulam caudae. Plura quam EVENTUS_EXEMPLA_MAXIMA (aut tabula
 * plena): antiquissima servantur, positio ultima semper vera. */
static b32
tessera_eventus_caudae_motum_impellere (
          EventusCauda* cauda,
      constans Eventus* eventus);

/* EVENTUS_DEPOSITIO cum viis COPIATIS (datum.depositio.viae visus
 * vocantis -> tabula caudae). Viae truncari nequeunt: tabula sine loco
 * -> FALSUM, nihil impulsum (vocans textum reddere potest). */
static b32
tessera_eventus_caudae_depositionem_impellere (
          EventusCauda* cauda,
      constans Eventus* eventus);

/* Eventum antiquissimum extrahere; FALSUM si vacua. */
static b32
tessera_eventus_caudae_extrahere (
    EventusCauda* cauda,
         Eventus* exitus);

#endif /* EVENTUS_CAUDA_H */

/* ================= ex include/interpres_terminalis.h ================= */
/* interpres_terminalis.h - Lexemata terminalis -> Eventus (eventus B2)
 *
 * Decodificator PURUS: lexema series_terminalis (modo initus) intrat,
 * Eventus in EventusCaudam exeunt (textus copiatus, motus coalitus -
 * cauda phasis A). Status solum lexematicus: praefixum alterum (ESC
 * solus ante clavem proximam), discipulus kitty (B2b).
 *
 * NON possidet (fons possidet, B3; hodie lector tesserae): moram ESC,
 * reliquias post moram, canales crudos (mus X10 octeti, corpus glutini,
 * caudae alienae). Fons decodificatori tradit lexemata, signum
 * 'post_moram' pro serie evacuata, et aditus crudos (_x10, _glutinum).
 *
 * FIDELITAS HONESTA (Franus 2026-10-02): quod protocollum non dicit
 * non fingitur. Legacy: nulla solutio (SOLUTA), codex IGNOTUS ubi
 * series ambigua (Enter = Ctrl+M, Tab = Ctrl+I), nullus bitus
 * maiusculae (Shift an Caps Lock nescitur - textus casum fert);
 * '\n' = Ctrl+J, 0x08 = Ctrl+H (proiectio tesserae eas coniungit, B5).
 * Codex POSITUS ubi series clavem physicam nominat (sagittae,
 * navigatio, F1-F12, Shift+Tab).
 *
 * KITTY (eventus B2b): 'CSI clavis[:maiuscula[:basis]] [;modi[:genus]]
 * [;textus] u' et formae legacy cum subcampo generis (CSI 1;5:3 A):
 * PRESSA/ITERATA/SOLUTA, modificatores kitty omnes (Caps, Num quoque),
 * textus associatus (solum si campus adest), codex ex clavi BASIS
 * (claves_codex_ex_littera) aut ex numero functionali
 * (claves_codex_ex_kitty). Series kitty prima: facultates discuntur
 * (tabula distincta; soluta et codex secundum vexilla impulsa) et
 * eventus FACULTATES impellitur ANTE clavem.
 */

#ifndef INTERPRES_TERMINALIS_H
#define INTERPRES_TERMINALIS_H

/* Vexilla kitty (CSI > f u) quae fons impellit: decodificator scit
 * quid absentia campi significet (e.g. basis absens + ALTERNAE =
 * basis eadem ac clavis). */
#define INTERPRES_KITTY_DISCERNERE  0x01   /* Esc, Ctrl+I != Tab */
#define INTERPRES_KITTY_GENERA      0x02   /* iterata, soluta */
#define INTERPRES_KITTY_ALTERNAE    0x04   /* clavis basis (codex) */
#define INTERPRES_KITTY_OMNES       0x08   /* omnes claves ut CSI u */
#define INTERPRES_KITTY_TEXTUS      0x10   /* textus associatus */

nomen structura {
              s32 cellula_latitudo; /* pixela NOSTRA per cellulam */
              s32 cellula_altitudo; /* et gradus rotulae (linea una) */
              b32 alterum_pendens;  /* ESC solus abruptus: alterum */
              i32 kitty_vexilla;    /* a fonte impulsa (B3) */
              b32 kitty_visus;      /* series kitty iam visa */
              s32 indicator_x;      /* positio muris ultima (pixela;
                                     * 0,0 nondum visa): depositio */
              s32 indicator_y;
EventusFacultates facultates;       /* legacy ab initio; kitty discitur
                                     * (eventus FACULTATES) */
} InterpresTerminalis;

/* Cellula in pixelis nostris: mus ad CENTRUM cellulae ponitur. */
static vacuum
tessera_interpres_initiare (
    InterpresTerminalis* interpres,
                    s32  cellula_latitudo,
                    s32  cellula_altitudo);

/* Lexema unum -> eventa (0..n) in caudam, tempore dato. post_moram:
 * lexema FUGA ex series_lectorem_evacuare (ESC solus = Effugium, ESC
 * ESC = duo, 'ESC x' = alterum + x, cetera abiciuntur). CSI 200~/201~
 * (glutinum) nihil reddit: fons corpus colligit (_glutinum). Redde
 * numerum eventorum impulsorum. */
static i32
tessera_interpres_lexema (
         InterpresTerminalis* interpres,
       constans SeriesLexema* lexema,
                         b32  post_moram,
                         s64  tempus,
                EventusCauda* cauda);

/* Mus X10: octeti CRUDI post 'ESC [ M' (cb, cx, cy, cum offsetibus
 * +32/+33 ut in filo). Redde numerum eventorum. */
static i32
tessera_interpres_x10 (
    InterpresTerminalis* interpres,
                    i32  cb,
                    i32  cx,
                    i32  cy,
                    s64  tempus,
           EventusCauda* cauda);

/* Corpus glutini (?2004) a fonte collectum -> TEXT origo GLUTINATA
 * (copiatum; ultra tabulam caudae truncatum). */
static i32
tessera_interpres_glutinum (
    InterpresTerminalis* interpres,
            constans i8* octeti,
                    i32  mensura,
                    s64  tempus,
           EventusCauda* cauda);

/* Glutinum promotum (eventus B3b): viae absolutae '\n' iunctae ->
 * EVENTUS_DEPOSITIO (promota) ad indicatorem ultimum - ANTE tractum
 * visum, non locum depositionis (terminal per tractum caecus est; park
 * 008). Redde 0 si tabula caudae sine loco (vocans textum reddat). */
static i32
tessera_interpres_depositio (
    InterpresTerminalis* interpres,
            constans i8* viae,
                    i32  mensura,
                    i32  numerus,
                    s64  tempus,
           EventusCauda* cauda);

#endif /* INTERPRES_TERMINALIS_H */

/* ================= ex include/rivus_terminalis.h ================= */
/* rivus_terminalis.h - Fons eventuum terminalis: octeti -> Eventus
 * (eventus B3)
 *
 * Pipeline UNA (ex lectore tesserae B1b translata): octeti ->
 * series_terminalis (modo initus) -> interpres_terminalis -> Eventus in
 * caudam. Possidet: buffer crudum, moram ESC et glutini, reliquias post
 * moram (H7/H8), canales crudos (mus X10, caudae alienae CSI [, corpus
 * glutini ?2004).
 *
 * PURUS: nec legit nec horologium tenet (in lib/, sub tessera - pontem
 * eius non novit). Vocans octetos tradit (rivus_tradere), rogat quamdiu
 * exspectet (rivus_mora_ms), silentium nuntiat (rivus_moram), eventa
 * trahit (rivus_eventum). Decodificatio PIGRA: lexema unum quoad
 * eventum
 * adest - motus NON coalescit (proiectio tesserae eventum quemque
 * videt). Consumptores Eventus rivus_eventum_coalitum legunt (motus
 * per lectionem coalescit, ut in fenestra; spec Q10).
 *
 * FACULTATES primum eventum fluxus sunt (spec Q4), iterum cum modi
 * declarantur aut kitty primum videtur.
 *
 * VISUS (textus eventuum) valent usque ad rivus_eventum proximum.
 */

#ifndef RIVUS_TERMINALIS_H
#define RIVUS_TERMINALIS_H

#define RIVUS_BUFFER              CCLVI     /* octeti crudi gestati */
#define RIVUS_MORA_FUGAE_MS       XXV       /* ESC solus vs series */
#define RIVUS_MORA_GLUTINI_MS     (III * M) /* silentium: finis */
#define RIVUS_GLUTINUM_CAPACITAS  65536     /* = EVENTUS_CAUDA_TEXTUS */

/* Modi quos applicatio DECLARAT (eventus B3b). Rivus octetos scribendos
 * reddit, non scribit (PURUS). Terminal modum ignotum tacite neglegit.
 *   MUS       ?1000 ?1002 ?1006: pressio, tractus, forma SGR
 *   SUPER     ?1003 hover (spec Q24: solum declaratum); MUS includit
 *   GLUTINUM  ?2004: glutinum uncis inclusum
 *   FOCUS     ?1004: focus I / O
 *   KITTY     CSI > 31 u impellitur, CSI < u extrahitur
 *   DEPOSITIO glutinum VIARUM absolutarum -> EVENTUS_DEPOSITIO
 *             (promota); GLUTINUM includit. POSITIO = indicator
 *             ultimus visus ANTE tractum (0,0 si nullus): per tractum
 *             ex alia applicatione terminalis caecus est - destinatio
 *             per focum, non per positionem (park 008)
 * RIVUS_MODI_MAXIMUM = octeti quos buffer modorum capere debet. */
#define RIVUS_MODUS_MUS       0x01
#define RIVUS_MODUS_SUPER     0x02
#define RIVUS_MODUS_GLUTINUM  0x04
#define RIVUS_MODUS_FOCUS     0x08
#define RIVUS_MODUS_KITTY     0x10
#define RIVUS_MODUS_DEPOSITIO 0x20
#define RIVUS_MODI_MAXIMUM    LXIV

/* Cellula in pixelis nostris (interpres: mus ad centrum cellulae). */
static TesseraRivusTerminalis*
tessera_rivus_creare (
    TesseraPiscina* piscina,
        s32  cellula_latitudo,
        s32  cellula_altitudo);

/* Octeti quos rivus nunc capere potest; vocans non plus tradat. */
static i32
tessera_rivus_spatium (
    constans TesseraRivusTerminalis* rivus);

/* Octetos a fonte lectos tradere (ordine). Redde quot accepti. */
static i32
tessera_rivus_tradere (
    TesseraRivusTerminalis* rivus,
        constans i8* octeti,
                i32  mensura);

/* Eventum proximum. FALSUM: octeti desunt - vocans legat (rivus_mora_ms
 * dicit quamdiu) aut silentium nuntiet (rivus_moram). */
static b32
tessera_rivus_eventum (
    TesseraRivusTerminalis* rivus,
                s64  tempus,
            Eventus* eventus);

/* 0 = nihil pendet (vocans moram suam habet); > 0 = series, runa aut
 * glutinum pendet: post tot ms silentii rivus_moram vocetur. */
static s32
tessera_rivus_mora_ms (
    constans TesseraRivusTerminalis* rivus);

/* Silentium moram exhausit: series pendens evacuatur (ESC = Effugium,
 * ESC ESC = duo, 'ESC x' = alterum + x, cetera abiciuntur; mus SGR
 * dimidia et ESC solus reliquiae fiunt continuationi), glutinum
 * truncatum finitur, runa dimidia abicitur. */
static vacuum
tessera_rivus_moram (
    TesseraRivusTerminalis* rivus,
                s64  tempus);

/* Octeti in buffere nondum consumpti (ad finem fluxus probandum). */
static i32
tessera_rivus_pendentes (
    constans TesseraRivusTerminalis* rivus);

#endif /* RIVUS_TERMINALIS_H */

/* ================= ex include/claves_physicae.h ================= */
/* claves_physicae.h - Codices clavium platformarum -> EventusCodex
 * (eventus A3; spec D3)
 *
 * Tabulae PURAE: codex virtualis platformae (macOS kVK_*; terminalis
 * kitty in phasi B) -> codex physicus vocabularii (W3C 'code'). Positio
 * clavis, non dispositio: kVK 0 est "KeyA" in QWERTY, AZERTY, Dvorak
 * idem. Ignotus -> EVENTUS_CODEX_IGNOTUS. Sine Cocoa: fenestra_macos.m
 * eam vocat, probationes sine fenestra.
 */

#ifndef CLAVES_PHYSICAE_H
#define CLAVES_PHYSICAE_H

/* Character dispositionis US basicae (minusculae, numeri, signa,
 * spatium) -> codex physicus: clavis BASIS kitty ('base layout key',
 * vexillum IV) positio est, non dispositio currens (eventus B2b).
 * Maiusculae et non-ASCII -> IGNOTUS (kitty basem minusculam
 * mittit). */
static EventusCodex
tessera_claves_codex_ex_littera (
    s32 runa);

/* kitty: numeri clavium functionalium (Escape 27, Enter 13, Tab 9,
 * Backspace 127, Caps Lock 57358, modificatores laterales 57441..57450)
 * -> codex physicus. Ceteri (runae, F13+, tabula numerica sine codice
 * in vocabulario) -> IGNOTUS. */
static EventusCodex
tessera_claves_codex_ex_kitty (
    s32 numerus);

#endif /* CLAVES_PHYSICAE_H */

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
  * ORNAMENTA = sex tuta (SGR singuli): crassum 1, obscurum 2,
 * cursivum 3, sublineatum 4, inversum 7, transfixum 9. Bits 0x40/0x80
 * NON SGR: vexilla latitudinis (runae U5) - cellula prima runae II
 * cellularum (LATUM) et secunda (CONTINUATIO, signum 0, numquam
 * emissa). Ex stilo vocantis semper auferuntur; in aequalitate
 * cellularum numerantur.
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

/* Vexilla latitudinis (non SGR; ponuntur SOLUM a tessera) */
#define TESSERA_ORNAMENTUM_LATUM        0x40  /* runa latitudinis II */
#define TESSERA_ORNAMENTUM_CONTINUATIO  0x80  /* dimidium secundum */
#define TESSERA_ORNAMENTUM_GRAPHEMA     0x100 /* signum = ID graphematis
                                               * internati (runae U5b) */
#define TESSERA_ORNAMENTA_STILI         0x3F  /* bits SGR soli */

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
 * ALTERUM, mus SGR (ESC [ < btn;x;y M/m; coordinatae 1-basatae ->
 * 0-basatae) et X10 (ESC [ M + tres octeti crudi; solutio = pulsus
 * III, botton ignotus): rota = btn&64 (sursum/deorsum/sinistrorsum/
 * dextrorsum), modificatores ex bits 4/8/16, motus (bit 32) cum
 * bottone 0-2 = TRACTUS (?1002; finalis M/m neglecta); motus sine
 * bottone (35, ?1003 non petitus), motus + rota (96/97) et rota soluta
 * TACITE consumuntur. CSI ignota TACITE consumuntur (strepitus
 * regiminis clavem phantasma fieri non debet).
 *
 * RUNA = codepoint DECODITUS (non compactus!): initus comparationes
 * et mathematicam casus vult; cellula.signum compactus manet -
 * nomina diversa confusionem vetant.
 *
 * LEXEMATOR (eventus B1b): grammatica octetorum = series_terminalis
 * (lib/, modo initus): OSC/DCS/APC responsa tacite consumuntur, ESC ESC
 * seriem = praefixum (ALTERUM), series abrupta abicitur. Lector
 * SEMANTICAM solum possidet, et casus crudos: mus X10 (tres octeti
 * post CSI M), formae alienae (CSI [ + cauda: Linux console, putty),
 * glutinum. MORA (moram vocantis lexemator non videt): series pendens
 * evacuatur - ESC solus = FUGA, 'ESC x' = alterum + x, cetera
 * abiciuntur; mus SGR dimidia (CSI <) et ESC solus RELIQUIAE fiunt,
 * quae continuationi sequenti (';5M', '[<..', '[M..') redduntur (H8).
 * LECTOR: buffer gestationis 64 octetorum (series trans lectiones
 * QUOTLIBET scissae accumulantur - legitur dum octeti intra moram
 * ~25ms adveniunt; sola lectio vacua moram exactam facit); ESC solum
 * per eam moram disambiguatur; amplitudo pontis quaque exspectatione rogatur
 * (AMPLITUDO eventum - SIGWINCH select solum interrumpit);
 * resumptum pontis rogatur (RESUMPTUM eventum) - tractator
 * utriusque = tessera_magnitudinem_renovare + pictura.
 *
 * GLUTINUM (?2004, textus insertus): corpus inter CSI 200~ et CSI 201~
 * UNUM eventum fit, octeti verbatim (nullae claves, nullus mus inde
 * parsatus - textus insertus mandata numquam exsequitur). glutinum =
 * VISUS in collectorem lectoris: validus usque ad
 * tessera_eventum_expectare proximum - tracta statim aut copia,
 * numquam serva. Ultra TESSERA_GLUTINUM_CAPACITAS octeti abiciuntur
 * (usque ad terminum hauriuntur); silentium TESSERA_MORA_GLUTINI_MS
 * sine termino glutinum finit. Utroque casu glutinum_truncatum.
 */

#ifndef TESSERA_EVENTUM_H
#define TESSERA_EVENTUM_H

#define TESSERA_LECTOR_BUFFER 64
#define TESSERA_MORA_FUGAE_MS 25
#define TESSERA_GLUTINUM_CAPACITAS 65536  /* collector, in creatione */
#define TESSERA_MORA_GLUTINI_MS 3000      /* silentium finit glutinum */

#define TESSERA_MODIFICATOR_IMPERIUM  0x01  /* ctrl */
#define TESSERA_MODIFICATOR_ALTERUM   0x02  /* alt/meta */
#define TESSERA_MODIFICATOR_MAIUSCULA 0x04  /* shift (ubi noscibile) */

TesseraLector*
tessera_lector_creare (
        TesseraPiscina* piscina,
    TesseraPons* pons);

/* Eventum proximum intra moram (ms); mora < 0 = sine fine.
 * NIHIL genus = mora exacta. Glutino incepto mora vocantis cedit:
 * glutinum ad terminum (aut silentium TESSERA_MORA_GLUTINI_MS) legitur
 * et UNUM eventum redditur. */
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

/* Graphemata (runae U5b): graphema plurium runarum semel internatur in
 * tabula operis; cellula ID eius fert (TESSERA_ORNAMENTUM_GRAPHEMA).
 * Tabula crescit tantum (ID numquam reusatur - frons et tergum eum
 * tenere possunt); ultra limites cellula ad runam primam redit. */
#define TESSERA_GRAPHEMATA_MAXIMA      16384
#define TESSERA_GRAPHEMA_OCTETI_MAXIMI 64
#define TESSERA_GRAPHEMATA_OCTETI      262144

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

/* Politica latitudinis ponere - statim post tessera_aperire (cellulae
 * latitudines quibus pictae sunt servant). Ordinaria: GRAPHEMATUM. */
vacuum
tessera_politicam_ponere (
        TesseraOpus* opus,
    TesseraPolitica  politica);

/* Politica ex ambitu (non quaestio terminalis): TERM_PROGRAM
 * "Apple_Terminal" -> SIMPLEX, aliter GRAPHEMATUM. */
TesseraPolitica
tessera_politica_ambitus (vacuum);

/* Profunditatem colorum ponere (ordinaria: PLENI); cellulae non
 * mutantur - frons tota iterum emittitur */
vacuum
tessera_colores_ponere (
       TesseraOpus* opus,
    TesseraColores  colores);

/* Profunditas ex ambitu (non quaestio terminalis): COLORTERM
 * "truecolor"/"24bit" -> PLENI; aliter TERM_PROGRAM "Apple_Terminal"
 * (48;2 male legit, runae U5) -> CCLVI; aliter PLENI. */
TesseraColores
tessera_colores_ambitus (vacuum);

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

/* Octeti UTF-8 cellulae (signum compactum aut graphema internatum) in
 * exitus[capacitas]; reddit numerum octetorum (0 = vacua aut
 * continuatio aut capacitas nimis parva). */
i32
tessera_cellulae_octeti (
    constans TesseraOpus* opus,
                     s32  x,
                     s32  y,
                      i8* exitus,
                     i32  capacitas);

/* Unitatem pingendam PRIMAM [initium, finis) ad (x, y) ponere (runae
 * U6c; regula runae_latitudo_textus): octetus C0/DEL aut invalidus ->
 * '?' (octetus unus, I columna); ceterum graphema sub politica operis
 * (latum = cellulae II, plurium runarum = internatum); latitudinis 0
 * nihil pingitur. Reddit indicatorem post unitatem, latitudinem
 * UNITATIS in *latitudo (vocans x per eam promovet) - etiam cum cellula
 * praeciditur (columna ultima: spatium; extra fines: nihil).
 * initium >= finis: initium, latitudo 0; opus NIHIL: finis. */
constans i8*
tessera_graphema_ponere (
      TesseraOpus* opus,
              s32  x,
              s32  y,
      constans i8* initium,
      constans i8* finis,
    TesseraStilus  stilus,
              i32* latitudo);

/* Textum scribere: unitates per tessera_graphema_ponere, x per
 * latitudinem cuiusque promotum (graphemata lata II cellulas tenent) */
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

/* ================= ex tessera/fontes/tessera_modi.h ================= */
/* tessera_modi.h - Effugia modorum terminalis: intrare et exire
 *
 * INTERNUM (non caput publicum): pons posix eas ut chordas STATICAS
 * adhibet - tractatores signorum (fatalis, TSTP, CONT) eas per
 * write(2) scribunt, async-signal-tute; ergo manent literae
 * praecompositae, non tempore cursus aedificatae.
 *
 * LEX PARIUM (probatio_tessera_modi.c): omnis modus privatus "?Nh" in
 * INTRANDI suum "?Nl" in EXEUNDI habet, et nullus "?Nl" sine pari -
 * modus intratus sed in ruina non relictus terminalem vexat post
 * exitum (glutinum 200~ in concha, quadra suspensa, mus mortuus).
 *
 * Tessera modos PONIT, numquam QUAERIT: terminal modum ignotum
 * tacite neglegit (thesis xterm-solum).
 */

#ifndef TESSERA_MODI_H
#define TESSERA_MODI_H

/* Scrinium alternum + mus (pressus/solutus) + tractus (motus botton
 * tento solum, ?1002) + mus SGR + glutinum uncis inclusum (?2004:
 * textus insertus ut eventum unum, numquam claves) */
#define INTRANDI "\033[?1049h\033[?1000h\033[?1002h\033[?1006h" \
                 "\033[?2004h"

/* Modus PER QUADRUM (non in INTRANDI): tessera_praesentare quadrum
 * non vacuum his includit - terminal quadrum integrum ostendit (nulla
 * laceratio). Terminal ignarus modum tacite neglegit. */
#define QUADRUM_INITIUM "\033[?2026h"
#define QUADRUM_FINIS   "\033[?2026l"

/* Quadrum apertum PRIMUM clauditur (ruina inter initium et finem
 * terminalem sustinentem non congelet), deinde modi ordine inverso;
 * deinde stilus nativus + cursor visibilis */
#define EXEUNDI  QUADRUM_FINIS \
                 "\033[?2004l" \
                 "\033[?1006l\033[?1002l\033[?1000l\033[?1049l" \
                 "\033[0m\033[?25h"

#endif /* TESSERA_MODI_H */

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
    redde _allocare_interna(piscina, mensura,
        PISCINA_ORDINATIO_ORDINARIA, VERUM);
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

    buffer_novum = (i8*)tessera_piscina_allocare(aedificator->piscina,
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
    TesseraChordaAedificator* aedificator,
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
tessera_chorda_aedificator_appendere_chorda (
    TesseraChordaAedificator* aedificator,
               TesseraChorda  s)
{
    si (!aedificator || !s.datum) redde FALSUM;

    redde _appendere_interna(aedificator, s.datum, s.mensura);
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
            si (   codepoint >= SURROGATUM_INITIUM
                && codepoint <= SURROGATUM_FINIS)
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

static s32
tessera_utf8_codere (
    s32  runa,
     i8* buffer)
{
    si (   buffer == NIHIL || runa < 0
        || (runa >= 0xD800 && runa <= 0xDFFF)
        || runa > 0x10FFFF)
    {
        redde 0;
    }
    si (runa < 0x80)
    {
        buffer[0] = (i8)runa;
        redde 1;
    }
    si (runa < 0x800)
    {
        buffer[0] = (i8)(0xC0 | (runa >> 6));
        buffer[1] = (i8)(0x80 | (runa & 0x3F));
        redde 2;
    }
    si (runa < 0x10000)
    {
        buffer[0] = (i8)(0xE0 | (runa >> 12));
        buffer[1] = (i8)(0x80 | ((runa >> 6) & 0x3F));
        buffer[2] = (i8)(0x80 | (runa & 0x3F));
        redde 3;
    }
    buffer[0] = (i8)(0xF0 | (runa >> 18));
    buffer[1] = (i8)(0x80 | ((runa >> 12) & 0x3F));
    buffer[2] = (i8)(0x80 | ((runa >> 6) & 0x3F));
    buffer[3] = (i8)(0x80 | (runa & 0x3F));
    redde 4;
}

/* ================= ex lib/runae.c ================= */

#define RUNA_MAXIMA 0x10FFFF

/* Status rupturae (RunaeRuptura.status): series quae in PRIORE finitur */
#define STATUS_RI_IMPAR       0x01   /* indicatores regionum impares */
#define STATUS_EMOJI          0x02   /* Extended_Pictographic Extend* */
#define STATUS_EMOJI_IUNCTOR  0x04   /* ... Extend* ZWJ (prior = ZWJ) */
#define STATUS_CONSONANS      0x08   /* InCB Consonant [Extend Linker]* */
#define STATUS_CONIUNCTOR     0x10   /* ... cum Linker viso */

interior b32
_valida (
    s32 runa)
{
    redde (b32)(runa >= ZEPHYRUM && runa <= RUNA_MAXIMA);
}

/* Octetus tabulae runae VALIDAE */
interior i32
_valor (
    s32 runa)
{
    i32 truncus = (i32)TESSERA_RUNAE_GRADUS_PRIMUS[(i32)runa >> VIII];

    redde (i32)TESSERA_RUNAE_GRADUS_SECUNDUS[truncus * RUNAE_TRUNCUS
        + ((i32)runa & 0xFF)];
}

/* Classis rupturae; runa invalida = REGIMEN (utrimque rumpit) */
interior i32
_classis (
    s32 runa)
{
    si (!_valida(runa))
    {
        redde RUNAE_CLASSIS_REGIMEN;
    }
    redde (_valor(runa) & RUNAE_CLASSIS_MASCULA)
        >> RUNAE_CLASSIS_POSITIO;
}

/* GCB Extend (quattuor species) */
interior b32
_extensio (
    i32 classis)
{
    redde (b32)(   classis == RUNAE_CLASSIS_EXTENSIO
                || classis == RUNAE_CLASSIS_EXTENSIO_INCB
                || classis == RUNAE_CLASSIS_CONIUNCTOR
                || classis == RUNAE_CLASSIS_MODIFICATOR);
}

/* uucode wcwidth_zero_in_grapheme, derivatum: latitudo 0, aut Prepend,
 * aut modificator emoji (lib/runae.phase-log.md U4) */
interior b32
_nulla_in_graphemate (
    s32 runa)
{
    i32 classis = _classis(runa);

    redde (b32)(   tessera_runae_latitudo(runa) == ZEPHYRUM
                || classis == RUNAE_CLASSIS_PRAEPOSITUM
                || classis == RUNAE_CLASSIS_MODIFICATOR);
}

static i32
tessera_runae_latitudo (
    s32 runa)
{
    si (!_valida(runa))
    {
        redde I;   /* invalida: U+FFFD pingitur */
    }
    redde (i32)(_valor(runa) & RUNAE_LATITUDO_MASCULA);
}

static vacuum
tessera_runae_rupturam_initiare (
    RunaeRuptura* ruptura)
{
    ruptura->status = ZEPHYRUM;
}

static b32
tessera_runae_rumpitur (
             s32  prior,
             s32  runa,
    RunaeRuptura* ruptura)
{
    i32 p = _classis(prior);
    i32 c = _classis(runa);
    i32 s = ruptura->status;

    /* status: series quae in priore finitur (prior semel accipitur) */
    si (p == RUNAE_CLASSIS_REGIONIS)
    {
        s ^= STATUS_RI_IMPAR;
    }
    alioquin
    {
        s &= ~(i32)STATUS_RI_IMPAR;
    }
    si (p == RUNAE_CLASSIS_PICTOGRAPHUM)
    {
        s = (s | STATUS_EMOJI) & ~(i32)STATUS_EMOJI_IUNCTOR;
    }
    alioquin si ((s & STATUS_EMOJI) && _extensio(p))
    {
        s &= ~(i32)STATUS_EMOJI_IUNCTOR;
    }
    alioquin si ((s & STATUS_EMOJI) && p == RUNAE_CLASSIS_IUNCTOR)
    {
        s = (s & ~(i32)STATUS_EMOJI) | STATUS_EMOJI_IUNCTOR;
    }
    alioquin
    {
        s &= ~(i32)(STATUS_EMOJI | STATUS_EMOJI_IUNCTOR);
    }
    si (p == RUNAE_CLASSIS_CONSONANS)
    {
        s = (s | STATUS_CONSONANS) & ~(i32)STATUS_CONIUNCTOR;
    }
    alioquin si (   (s & STATUS_CONSONANS)
                 && (p == RUNAE_CLASSIS_EXTENSIO_INCB
                     || p == RUNAE_CLASSIS_IUNCTOR))
    {
        /* InCB Extend: series manet */
    }
    alioquin si (   (s & STATUS_CONSONANS)
                 && p == RUNAE_CLASSIS_CONIUNCTOR)
    {
        s |= STATUS_CONIUNCTOR;
    }
    alioquin
    {
        s &= ~(i32)(STATUS_CONSONANS | STATUS_CONIUNCTOR);
    }
    ruptura->status = s;

    /* GB3: CR x LF */
    si (p == RUNAE_CLASSIS_CR && c == RUNAE_CLASSIS_LF)
    {
        redde FALSUM;
    }
    /* GB4, GB5: regimina utrimque rumpunt */
    si (   p == RUNAE_CLASSIS_CR || p == RUNAE_CLASSIS_LF
        || p == RUNAE_CLASSIS_REGIMEN || c == RUNAE_CLASSIS_CR
        || c == RUNAE_CLASSIS_LF || c == RUNAE_CLASSIS_REGIMEN)
    {
        redde VERUM;
    }
    /* GB6-GB8: syllabae Hangul */
    si (   p == RUNAE_CLASSIS_SYLLABA_INITIALIS
        && (   c == RUNAE_CLASSIS_SYLLABA_INITIALIS
            || c == RUNAE_CLASSIS_SYLLABA_MEDIA
            || c == RUNAE_CLASSIS_SYLLABA_APERTA
            || c == RUNAE_CLASSIS_SYLLABA_CLAUSA))
    {
        redde FALSUM;
    }
    si (   (p == RUNAE_CLASSIS_SYLLABA_APERTA
            || p == RUNAE_CLASSIS_SYLLABA_MEDIA)
        && (c == RUNAE_CLASSIS_SYLLABA_MEDIA
            || c == RUNAE_CLASSIS_SYLLABA_FINALIS))
    {
        redde FALSUM;
    }
    si (   (p == RUNAE_CLASSIS_SYLLABA_CLAUSA
            || p == RUNAE_CLASSIS_SYLLABA_FINALIS)
        && c == RUNAE_CLASSIS_SYLLABA_FINALIS)
    {
        redde FALSUM;
    }
    /* GB9, GB9a, GB9b */
    si (   _extensio(c) || c == RUNAE_CLASSIS_IUNCTOR
        || c == RUNAE_CLASSIS_SPATIANS
        || p == RUNAE_CLASSIS_PRAEPOSITUM)
    {
        redde FALSUM;
    }
    /* GB9c: consonans [extend linker]* linker [extend linker]* x
     * consonans */
    si (   c == RUNAE_CLASSIS_CONSONANS && (s & STATUS_CONSONANS)
        && (s & STATUS_CONIUNCTOR))
    {
        redde FALSUM;
    }
    /* GB11: pictographum Extend* ZWJ x pictographum */
    si (c == RUNAE_CLASSIS_PICTOGRAPHUM && (s & STATUS_EMOJI_IUNCTOR))
    {
        redde FALSUM;
    }
    /* GB12, GB13: indicatores regionum per paria */
    si (   p == RUNAE_CLASSIS_REGIONIS && c == RUNAE_CLASSIS_REGIONIS
        && (s & STATUS_RI_IMPAR))
    {
        redde FALSUM;
    }
    redde VERUM;   /* GB999 */
}

static constans i8*
tessera_runae_graphema_ex_politica (
      constans i8* initium,
      constans i8* finis,
    RunaePolitica  politica,
              i32* latitudo)
{
        constans i8* cursor = initium;

             s32 prior;
             s32 ultima;   /* Ghostty 'prev': runa ultima cum effectu */
             i32 lat;
    RunaeRuptura ruptura;


    *latitudo = ZEPHYRUM;
    si (initium >= finis)
    {
        redde initium;
    }
    prior = tessera_utf8_decodere(&cursor, finis);
    si (prior < ZEPHYRUM)
    {
        *latitudo = I;   /* series invalida: graphema suum (U+FFFD) */
        redde cursor;
    }
    lat     = tessera_runae_latitudo(prior);
    ultima  = prior;
    tessera_runae_rupturam_initiare(&ruptura);
    dum (cursor < finis)
    {
         constans i8* post = cursor;
                 s32  runa = tessera_utf8_decodere(&post, finis);

        si (runa < ZEPHYRUM || tessera_runae_rumpitur(prior, runa, &ruptura))
        {
            frange;   /* invalida aut limes: graphema finitur */
        }
        si (   politica        == RUNAE_POLITICA_SIMPLEX
            && _classis(prior) == RUNAE_CLASSIS_IUNCTOR
            && _classis(runa)  == RUNAE_CLASSIS_PICTOGRAPHUM)
        {
            frange;   /* SIMPLEX: ZWJ pictographa non iungit (GB11 non) */
        }
        si (runa == 0xFE0F || runa == 0xFE0E)
        {
            /* VS16/VS15 solum post basim variationis; aliter nullus
             * effectus, ultima manet (Ghostty .ignore) */
            si (_valor(ultima) & RUNAE_BASIS_VARIATIONIS)
            {
                lat     = (runa == 0xFE0F) ? II : I;
                ultima  = runa;
            }
        }
                alioquin si (!_nulla_in_graphemate(runa))
        {
            lat     = II;   /* runa latitudinem conferens */
            ultima  = runa;
        }
        alioquin
        {
            ultima = runa;
        }
        prior   = runa;
        cursor  = post;
    }
    *latitudo = lat;
    redde cursor;
}

/* ================= ex lib/runae_tabulae.c ================= */

constans i16 TESSERA_RUNAE_GRADUS_PRIMUS[RUNAE_TRUNCI_PRIMI] = {
    0, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 1, 17, 1, 1, 1, 18, 19, 20, 21, 22, 23, 24, 1, 1,
    25, 26, 1, 27, 28, 29, 30, 31, 1, 32, 1, 33, 34, 35, 36, 37,
    38, 39, 40, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 42, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 43, 1, 44, 1, 45, 46, 47, 48, 49, 50, 51, 52,
    53, 54, 55, 49, 50, 51, 52, 53, 54, 55, 49, 50, 51, 52, 53, 54,
    55, 49, 50, 51, 52, 53, 54, 55, 49, 50, 51, 52, 53, 54, 55, 49,
    50, 51, 52, 53, 54, 55, 49, 56, 57, 57, 57, 57, 57, 57, 57, 57,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 41, 41, 58, 1, 1, 59, 60,
    1, 61, 62, 63, 1, 1, 1, 1, 1, 1, 64, 1, 1, 65, 66, 67,
    68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 1, 79, 80, 81, 82,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 83, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 84, 85, 1, 1, 1, 86,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 87, 41, 41, 41, 41, 88, 89, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 90,
    41, 91, 92, 1, 1, 1, 1, 1, 1, 1, 1, 1, 93, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 94,
    1, 95, 96, 1, 1, 1, 1, 1, 1, 1, 97, 1, 1, 1, 1, 1,
    98, 85, 99, 1, 100, 1, 1, 1, 101, 102, 1, 1, 1, 1, 1, 1,
    103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 1, 114, 114, 114, 115,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 116,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
    41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 116,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    117, 118, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

constans i8 TESSERA_RUNAE_GRADUS_SECUNDUS[30720] = {
    /* truncus 0 (ex U+0000) */
    12,12,12,12,12,12,12,12,12,12,8,12,12,4,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    1,1,1,129,1,1,1,1,1,1,129,1,1,1,1,1,
    129,129,129,129,129,129,129,129,129,129,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    1,1,1,1,1,1,1,1,1,197,1,1,1,13,197,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 1 (ex U+0100) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 2 (ex U+0300) */
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,16,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 3 (ex U+0400) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,20,20,20,20,20,16,16,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 4 (ex U+0500) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,1,20,
    1,20,20,1,20,20,1,20,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 5 (ex U+0600) */
    41,41,41,41,41,41,1,1,1,1,1,1,1,1,1,1,
    20,20,20,20,20,20,20,20,20,20,20,1,12,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,20,20,20,20,20,20,20,41,1,20,
    20,20,20,20,20,1,1,20,20,1,20,20,20,20,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 6 (ex U+0700) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,41,
    1,20,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,16,16,16,16,16,16,16,16,16,16,
    16,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,20,20,20,20,20,
    20,20,20,20,1,1,1,1,1,1,1,1,1,20,1,1,
    /* truncus 7 (ex U+0800) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,20,20,20,20,1,20,20,20,20,20,
    20,20,20,20,1,20,20,20,1,20,20,20,20,20,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,20,20,20,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    41,41,1,1,1,1,1,1,20,20,20,20,20,20,20,20,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,41,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    /* truncus 8 (ex U+0900) */
    16,16,16,45,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,73,73,73,73,73,73,73,73,73,73,73,
    73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,
    73,73,73,73,73,73,73,73,73,73,16,45,20,1,45,45,
    45,16,16,16,16,16,16,16,16,45,45,45,45,24,45,45,
    1,20,20,20,20,16,16,16,73,73,73,73,73,73,73,73,
    1,1,16,16,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,73,73,73,73,73,73,73,73,
    1,16,45,45,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,73,73,73,73,73,73,73,73,73,73,73,
    73,73,73,73,73,73,73,73,73,1,73,73,73,73,73,73,
    73,1,73,1,1,1,73,73,73,73,1,1,20,1,17,45,
    45,16,16,16,16,1,1,45,45,1,1,45,45,24,1,1,
    1,1,1,1,1,1,1,17,1,1,1,1,73,73,1,73,
    1,1,16,16,1,1,1,1,1,1,1,1,1,1,1,1,
    73,73,1,1,1,1,1,1,1,1,1,1,1,1,20,1,
    /* truncus 9 (ex U+0A00) */
    1,16,16,45,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,20,1,45,45,
    45,16,16,1,1,1,1,16,16,1,1,16,16,16,1,1,
    1,16,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    16,16,1,1,1,16,1,1,1,1,1,1,1,1,1,1,
    1,16,16,45,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,73,73,73,73,73,73,73,73,73,73,73,
    73,73,73,73,73,73,73,73,73,1,73,73,73,73,73,73,
    73,1,73,73,1,73,73,73,73,73,1,1,20,1,45,45,
    45,16,16,16,16,16,1,16,16,45,1,45,45,24,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,16,16,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,73,16,16,16,16,16,16,
    /* truncus 10 (ex U+0B00) */
    1,16,45,45,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,73,73,73,73,73,73,73,73,73,73,73,
    73,73,73,73,73,73,73,73,73,1,73,73,73,73,73,73,
    73,1,73,73,1,73,73,73,73,73,1,1,20,1,17,16,
    45,16,16,16,16,1,1,45,45,1,1,45,45,24,1,1,
    1,1,1,1,1,16,16,17,1,1,1,1,73,73,1,73,
    1,1,16,16,1,1,1,1,1,1,1,1,1,1,1,1,
    1,73,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,16,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,17,45,
    16,45,45,1,1,1,45,45,45,1,45,45,45,16,1,1,
    1,1,1,1,1,1,1,17,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 11 (ex U+0C00) */
    16,45,45,45,16,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,73,73,73,73,73,73,73,73,73,73,73,
    73,73,73,73,73,73,73,73,73,1,73,73,73,73,73,73,
    73,73,73,73,73,73,73,73,73,73,1,1,20,1,16,16,
    16,45,45,45,45,1,16,16,16,1,16,16,16,24,1,1,
    1,1,1,1,1,20,20,1,73,73,73,1,1,1,1,1,
    1,1,16,16,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,16,45,45,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,20,1,45,16,
    45,45,17,45,45,1,16,45,45,1,45,45,16,16,1,1,
    1,1,1,1,1,17,17,1,1,1,1,1,1,1,1,1,
    1,1,16,16,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,45,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 12 (ex U+0D00) */
    16,16,45,45,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,73,73,73,73,73,73,73,73,73,73,73,
    73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,
    73,73,73,73,73,73,73,73,73,73,73,20,20,1,17,45,
    45,16,16,16,16,1,45,45,45,1,45,45,45,24,41,1,
    1,1,1,1,1,1,1,17,1,1,1,1,1,1,1,1,
    1,1,16,16,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,16,45,45,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,16,1,1,1,1,17,
    45,45,16,16,16,1,16,1,45,45,45,45,45,45,45,17,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,45,45,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 13 (ex U+0E00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,16,1,45,16,16,16,16,20,20,20,1,1,1,1,1,
    1,1,1,1,1,1,1,16,20,20,20,20,16,16,16,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,16,1,45,16,16,16,16,20,20,20,16,16,1,1,1,
    1,1,1,1,1,1,1,1,20,20,20,20,16,16,16,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 14 (ex U+0F00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,20,20,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,20,1,20,1,20,1,1,1,1,45,45,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,20,20,16,20,16,16,16,16,16,20,20,20,20,16,45,
    20,16,20,20,20,1,20,20,1,1,1,1,1,16,16,16,
    16,16,16,16,16,16,16,16,1,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,1,1,1,
    1,1,1,1,1,1,20,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 15 (ex U+1000) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,16,16,16,
    16,45,16,16,16,16,16,20,1,20,20,45,45,16,16,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,45,45,16,16,1,1,1,1,16,16,
    16,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,16,16,16,16,1,1,1,1,1,1,1,1,1,1,1,
    1,1,16,1,45,16,16,1,1,1,1,1,1,20,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,16,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 16 (ex U+1100) */
    50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,
    50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,
    50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,
    50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,
    50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,
    50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,48,
    52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,
    52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,
    52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,
    52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,
    52,52,52,52,52,52,52,52,56,56,56,56,56,56,56,56,
    56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,
    56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,
    56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,
    56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,
    56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,
    /* truncus 17 (ex U+1300) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,20,20,20,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 18 (ex U+1700) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,16,16,20,45,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,16,16,45,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,16,16,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,16,16,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,16,16,45,16,16,16,16,16,16,16,45,45,
    45,45,45,45,45,45,16,45,45,16,16,16,16,16,16,16,
    16,16,20,16,1,1,1,1,1,1,1,1,1,20,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 19 (ex U+1800) */
    1,1,1,1,1,1,1,1,1,1,1,16,16,16,12,16,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,16,16,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,20,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 20 (ex U+1900) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    16,16,16,45,45,45,45,16,16,45,45,45,1,1,1,1,
    45,45,16,45,45,45,45,45,45,20,20,20,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 21 (ex U+1A00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,20,20,45,45,16,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,45,16,45,16,16,16,16,16,16,16,1,
    20,1,16,1,1,16,16,16,16,16,16,16,16,45,45,45,
    45,45,45,16,16,20,20,20,20,20,20,20,20,1,1,20,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,16,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 22 (ex U+1B00) */
    16,16,16,16,45,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,20,17,16,16,16,16,16,45,16,45,45,45,
    45,45,16,45,45,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,20,20,20,20,20,
    20,20,20,20,1,1,1,1,1,1,1,1,1,1,1,1,
    16,16,45,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,45,16,16,16,16,45,45,16,16,45,20,16,16,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,20,45,16,16,45,45,45,16,45,16,
    16,16,45,45,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 23 (ex U+1C00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,45,45,45,45,45,45,45,45,16,16,16,16,
    16,16,16,16,45,45,16,20,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,20,20,1,20,20,20,20,20,20,20,20,20,20,20,20,
    20,45,20,20,20,20,20,20,20,1,1,1,1,20,1,1,
    1,1,1,1,20,1,1,45,20,20,1,1,1,1,1,1,
    /* truncus 24 (ex U+1D00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    /* truncus 25 (ex U+2000) */
    1,1,1,1,1,1,1,1,1,1,1,12,16,32,12,12,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,12,12,12,12,12,12,12,1,
    1,1,1,1,1,1,1,1,1,1,1,1,197,1,1,1,
    1,1,1,1,1,1,1,1,1,197,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,20,20,20,20,20,20,20,20,20,20,20,20,16,16,16,
    16,20,16,16,16,20,20,20,20,20,20,20,20,20,20,20,
    20,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 26 (ex U+2100) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,197,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,197,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,197,197,197,197,197,197,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,197,197,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 27 (ex U+2300) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,198,198,1,1,1,1,
    1,1,1,1,1,1,1,1,197,2,2,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,69,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,197,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,198,198,198,198,197,197,197,
    198,197,197,198,1,1,1,1,197,197,197,1,1,1,1,1,
    /* truncus 28 (ex U+2400) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,197,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 29 (ex U+2500) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,197,197,1,1,1,1,
    1,1,1,1,1,1,197,1,1,1,1,1,1,1,1,1,
    197,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,197,197,198,198,1,
    /* truncus 30 (ex U+2600) */
    197,197,197,197,197,69,1,69,69,69,69,69,69,69,197,69,
    69,197,69,1,198,198,69,69,197,69,69,69,69,197,69,69,
    197,69,197,197,69,69,197,69,69,69,197,69,69,69,197,197,
    69,69,69,69,69,69,69,69,197,197,197,69,69,69,69,69,
    197,69,197,69,69,69,69,69,198,198,198,198,198,198,198,198,
    198,198,198,198,69,69,69,69,69,69,69,69,69,69,69,197,
    197,69,69,197,69,197,197,69,197,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,197,69,69,197,198,
    69,69,69,69,69,69,1,1,1,1,1,1,1,1,1,1,
    69,69,197,198,197,197,197,197,69,197,69,197,197,69,69,69,
    197,198,69,69,69,69,69,197,69,69,198,198,69,69,69,69,
    197,197,69,69,69,69,69,69,69,69,69,69,69,198,198,69,
    69,69,69,69,198,198,69,69,197,69,69,69,69,69,198,197,
    69,197,69,197,198,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,197,198,69,69,69,69,69,
    197,197,198,198,197,198,69,197,197,197,198,69,69,198,69,69,
    /* truncus 31 (ex U+2700) */
    69,69,197,69,69,198,1,1,197,197,198,198,197,197,69,197,
    69,69,197,1,197,1,197,1,1,1,1,1,1,197,1,1,
    1,197,1,1,1,1,1,1,198,1,1,1,1,1,1,1,
    1,1,1,197,197,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,197,1,1,197,1,1,1,1,198,1,198,1,
    1,1,1,198,198,198,1,198,1,1,1,1,1,1,1,1,
    1,1,1,197,197,69,69,69,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,198,198,198,1,1,1,1,1,1,1,1,
    1,197,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    198,1,1,1,1,1,1,1,1,1,1,1,1,1,1,198,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 32 (ex U+2900) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,197,197,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 33 (ex U+2B00) */
    1,1,1,1,1,197,197,197,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,198,198,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    198,1,1,1,1,198,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 34 (ex U+2C00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,20,
    20,20,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 35 (ex U+2D00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,20,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    /* truncus 36 (ex U+2E00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 37 (ex U+2F00) */
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    /* truncus 38 (ex U+3000) */
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,20,20,20,20,22,22,
    198,2,2,2,2,2,2,2,2,2,2,2,2,198,2,1,
    1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,1,1,20,20,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    /* truncus 39 (ex U+3100) */
    1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    /* truncus 40 (ex U+3200) */
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,198,2,198,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    /* truncus 41 (ex U+3300) */
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    /* truncus 42 (ex U+4D00) */
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 43 (ex U+A400) */
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 44 (ex U+A600) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,20,
    16,16,16,1,20,20,20,20,20,20,20,20,20,20,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,20,20,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,20,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 45 (ex U+A800) */
    1,1,16,1,1,1,16,1,1,1,1,16,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,45,45,16,16,45,1,1,1,1,20,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    45,45,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,45,45,45,45,45,45,45,45,45,45,45,45,
    45,45,45,45,16,16,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,1,1,1,1,1,1,1,1,1,1,1,1,1,16,
    /* truncus 46 (ex U+A900) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,16,16,16,16,16,20,20,20,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,16,16,16,16,16,16,16,16,16,
    16,16,45,45,1,1,1,1,1,1,1,1,1,1,1,1,
    50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,
    50,50,50,50,50,50,50,50,50,50,50,50,50,1,1,1,
    16,16,16,45,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,20,45,45,16,16,16,16,45,45,16,16,45,45,
    45,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,16,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 47 (ex U+AA00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,16,16,16,16,16,16,45,
    45,16,16,45,45,16,16,1,1,1,1,1,1,1,1,1,
    1,1,1,16,1,1,1,1,1,1,1,1,16,45,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,16,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,1,20,20,20,1,1,20,20,1,1,1,1,1,20,20,
    1,20,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,45,16,16,45,45,
    1,1,1,1,1,45,20,1,1,1,1,1,1,1,1,1,
    /* truncus 48 (ex U+AB00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,45,45,16,45,45,16,45,45,1,45,20,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 49 (ex U+AC00) */
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    /* truncus 50 (ex U+AD00) */
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    /* truncus 51 (ex U+AE00) */
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    /* truncus 52 (ex U+AF00) */
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    /* truncus 53 (ex U+B000) */
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    /* truncus 54 (ex U+B100) */
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    /* truncus 55 (ex U+B200) */
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    /* truncus 56 (ex U+D700) */
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,62,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    62,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,62,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,1,1,1,1,1,1,1,1,1,1,1,1,
    52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,
    52,52,52,52,52,52,52,1,1,1,1,56,56,56,56,56,
    56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,
    56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,
    56,56,56,56,56,56,56,56,56,56,56,56,1,1,1,1,
    /* truncus 57 (ex U+D800) */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* truncus 58 (ex U+FB00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,20,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 59 (ex U+FE00) */
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,
    20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,1,2,2,2,2,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,12,
    /* truncus 60 (ex U+FF00) */
    1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,17,17,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,
    12,12,12,12,12,12,12,12,12,13,13,13,1,1,1,1,
    /* truncus 61 (ex U+10100) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,20,1,1,
    /* truncus 62 (ex U+10200) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 63 (ex U+10300) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,20,20,20,20,20,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 64 (ex U+10A00) */
    1,16,16,16,1,16,16,1,1,1,1,1,16,20,16,20,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,20,20,20,1,1,1,1,20,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,20,20,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 65 (ex U+10D00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,20,20,20,20,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 66 (ex U+10E00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,20,20,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,20,20,20,
    /* truncus 67 (ex U+10F00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,20,20,20,20,20,20,20,20,20,20,
    20,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,20,20,20,20,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 68 (ex U+11000) */
    45,16,45,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,1,1,16,16,1,1,1,1,1,1,1,1,1,1,20,
    16,16,45,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    45,45,45,16,16,16,16,45,45,16,20,1,1,41,1,1,
    1,1,16,1,1,1,1,1,1,1,1,1,1,41,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 69 (ex U+11100) */
    20,20,20,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,16,16,16,16,16,45,16,16,16,
    16,16,16,20,20,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,45,45,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,20,1,1,1,1,1,1,1,1,1,1,1,1,
    16,16,45,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,45,45,45,16,16,16,16,16,16,16,16,16,45,
    45,1,41,41,1,1,1,1,1,16,20,16,16,1,45,16,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 70 (ex U+11200) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,45,45,45,16,
    16,16,45,45,16,45,20,16,1,1,1,1,1,1,16,1,
    1,16,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,16,
    45,45,45,16,16,16,16,16,16,20,20,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 71 (ex U+11300) */
    16,16,45,45,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,20,20,1,17,45,
    16,45,45,45,45,1,1,45,45,1,1,45,45,45,1,1,
    1,1,1,1,1,1,1,17,1,1,1,1,1,1,1,1,
    1,1,45,45,1,1,20,20,20,20,20,20,20,1,1,1,
    20,20,20,20,20,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 72 (ex U+11400) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,45,45,45,16,16,16,16,16,16,16,16,
    45,45,16,16,16,45,20,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,20,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    17,45,45,16,16,16,16,16,16,45,16,45,45,17,45,16,
    16,45,16,20,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 73 (ex U+11500) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,17,
    45,45,16,16,16,16,1,1,45,45,45,45,16,16,45,16,
    20,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,16,16,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 74 (ex U+11600) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    45,45,45,16,16,16,16,16,16,16,16,45,45,16,45,16,
    16,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,16,45,16,45,45,
    16,16,16,16,16,16,45,20,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 75 (ex U+11700) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,16,16,16,
    1,1,16,16,16,16,45,16,16,16,16,20,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 76 (ex U+11800) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,45,45,45,16,
    16,16,16,16,16,16,16,16,45,16,20,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 77 (ex U+11900) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    17,45,45,45,45,45,1,45,45,1,1,16,16,45,20,41,
    45,41,45,20,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,45,45,45,16,16,16,16,1,1,16,16,45,45,45,45,
    16,1,1,1,45,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 78 (ex U+11A00) */
    1,16,16,16,16,16,16,16,16,16,16,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,16,20,16,16,16,16,45,41,16,16,16,16,1,
    1,1,1,1,1,1,1,20,1,1,1,1,1,1,1,1,
    1,16,16,16,16,16,16,45,45,16,16,16,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,41,41,41,41,41,41,16,16,16,16,16,16,
    16,16,16,16,16,16,16,45,16,20,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 79 (ex U+11C00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,45,
    16,16,16,16,16,16,16,1,16,16,16,16,16,16,45,16,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,1,45,16,16,16,16,16,16,
    16,45,16,16,45,16,16,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 80 (ex U+11D00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,16,16,16,16,16,16,1,1,1,16,1,16,16,1,16,
    16,16,20,16,20,20,41,16,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,45,45,45,45,45,1,
    16,16,1,45,45,16,45,20,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 81 (ex U+11E00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,16,16,45,45,1,1,1,1,1,1,1,1,1,
    /* truncus 82 (ex U+11F00) */
    16,16,41,45,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,45,45,16,16,16,16,16,1,1,1,45,45,
    16,45,20,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 83 (ex U+13400) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    16,1,1,1,1,1,1,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 84 (ex U+16A00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,20,20,20,20,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 85 (ex U+16B00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,20,20,20,20,20,20,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 86 (ex U+16F00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,16,
    1,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,
    45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,
    45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,
    45,45,45,45,45,45,45,45,1,1,1,1,1,1,1,16,
    16,16,16,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,16,1,1,1,1,1,1,1,1,1,1,1,
    46,46,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 87 (ex U+18700) */
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,
    /* truncus 88 (ex U+18C00) */
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 89 (ex U+18D00) */
    2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 90 (ex U+1AF00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,1,2,2,2,2,2,2,2,1,2,2,1,
    /* truncus 91 (ex U+1B100) */
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,1,1,2,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,2,2,2,2,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    /* truncus 92 (ex U+1B200) */
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,
    /* truncus 93 (ex U+1BC00) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,16,20,1,
    12,12,12,12,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 94 (ex U+1CF00) */
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,1,1,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 95 (ex U+1D100) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,21,45,20,20,20,1,1,1,45,21,21,
    21,21,21,12,12,12,12,12,12,12,12,20,20,20,20,20,
    20,20,20,1,1,20,20,20,20,20,20,20,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,20,20,20,20,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 96 (ex U+1D200) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,20,20,20,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 97 (ex U+1DA00) */
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,1,1,1,1,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,1,1,1,
    1,1,1,1,1,16,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,16,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,16,16,16,16,16,
    1,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 98 (ex U+1E000) */
    20,20,20,20,20,20,20,1,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,1,1,20,20,20,20,20,
    20,20,1,20,20,1,20,20,20,20,20,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,20,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 99 (ex U+1E200) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,20,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,20,20,20,20,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 100 (ex U+1E400) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,20,20,20,20,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 101 (ex U+1E800) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    20,20,20,20,20,20,20,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 102 (ex U+1E900) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,20,20,20,20,20,20,20,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    /* truncus 103 (ex U+1F000) */
    69,69,69,69,198,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,70,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    /* truncus 104 (ex U+1F100) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,69,69,69,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,69,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,69,69,69,69,
    197,197,1,1,1,1,1,1,1,1,1,1,1,1,197,197,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,70,1,
    1,70,70,70,70,70,70,70,70,70,70,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,38,38,38,38,38,38,38,38,38,38,
    38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,
    /* truncus 105 (ex U+1F200) */
    2,70,198,69,69,69,69,69,69,69,69,69,69,69,69,69,
    2,2,2,2,2,2,2,2,2,2,198,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,198,
    2,2,70,70,70,70,70,198,70,70,70,2,69,69,69,69,
    2,2,2,2,2,2,2,2,2,69,69,69,69,69,69,69,
    70,70,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    70,70,70,70,70,70,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    /* truncus 106 (ex U+1F300) */
    70,70,70,70,70,70,70,70,70,70,70,70,70,198,198,198,
    70,70,70,70,70,198,70,70,70,70,70,70,198,70,70,70,
    70,197,69,69,197,197,197,197,197,197,197,197,197,70,70,70,
    70,70,70,70,70,70,197,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,198,70,70,70,70,197,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,198,69,69,197,197,69,197,197,197,69,69,197,197,
    70,70,70,70,70,70,70,198,70,70,70,70,198,198,198,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,198,70,198,70,198,70,70,70,198,197,197,197,197,70,
    70,70,70,70,197,197,197,197,197,197,197,197,197,197,197,197,
    198,70,70,70,70,70,70,70,70,70,70,70,70,198,70,70,
    70,69,69,197,70,197,69,197,70,70,70,30,30,30,30,30,
    /* truncus 107 (ex U+1F400) */
    70,70,70,70,70,70,70,70,198,70,70,70,70,70,70,70,
    70,70,70,70,70,198,70,70,70,70,70,70,70,70,70,198,
    70,70,70,70,70,70,198,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,197,
    70,197,198,70,70,70,198,198,198,198,70,70,70,198,198,70,
    70,70,70,198,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,198,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,198,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,198,70,70,70,70,70,70,70,70,70,70,70,70,
    198,70,70,198,70,70,70,70,70,70,70,198,70,70,70,198,
    70,70,70,70,70,70,70,70,70,70,70,198,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,198,70,70,70,70,198,
    70,70,70,70,198,198,198,70,70,70,198,198,198,198,70,70,
    70,70,70,70,70,70,70,198,70,198,198,198,70,197,69,70,
    /* truncus 108 (ex U+1F500) */
    70,70,70,70,70,70,70,70,198,70,70,70,70,198,70,70,
    70,70,198,198,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,1,1,
    1,1,1,1,1,1,69,69,69,197,197,70,70,70,70,69,
    198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,
    198,198,198,198,198,198,198,198,69,69,69,69,69,69,69,197,
    197,69,69,197,197,197,197,197,197,197,70,69,69,69,69,69,
    69,69,69,69,69,69,69,197,69,69,197,197,197,197,69,69,
    197,69,69,69,69,70,70,69,69,69,69,69,69,69,69,69,
    69,69,69,69,70,197,69,69,197,69,69,69,69,69,69,69,
    69,197,197,69,69,69,69,69,69,69,69,69,197,69,69,69,
    69,69,197,197,197,69,69,69,69,69,69,69,69,69,69,69,
    69,197,197,197,69,69,69,69,69,69,69,69,197,197,197,69,
    69,197,69,197,69,69,69,69,197,69,69,69,69,69,69,197,
    69,69,69,197,69,69,69,69,69,69,197,70,70,70,70,70,
    /* truncus 109 (ex U+1F600) */
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    198,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    70,70,70,70,70,70,70,198,70,70,70,70,70,198,70,70,
    70,198,70,70,198,70,70,70,198,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,198,70,70,
    70,70,198,70,70,70,70,70,70,198,198,70,198,70,70,70,
    70,70,70,70,70,70,69,69,69,69,69,197,70,197,197,197,
    70,70,70,69,69,70,70,70,69,69,69,69,70,70,70,70,
    197,197,197,197,197,197,69,69,69,197,69,70,70,69,69,69,
    197,69,69,197,70,70,70,70,70,70,70,70,70,69,69,69,
    /* truncus 110 (ex U+1F700) */
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,69,69,69,69,69,69,69,69,69,69,69,69,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,69,69,69,69,69,69,69,69,69,69,69,
    70,70,70,70,70,70,70,70,70,70,70,70,69,69,69,69,
    70,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    /* truncus 111 (ex U+1F800) */
    1,1,1,1,1,1,1,1,1,1,1,1,69,69,69,69,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,69,69,69,69,69,69,69,69,
    1,1,1,1,1,1,1,1,1,1,69,69,69,69,69,69,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,69,69,69,69,69,69,69,69,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    /* truncus 112 (ex U+1F900) */
    1,1,1,1,1,1,1,1,1,1,1,1,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,1,70,70,70,70,
    70,70,70,70,70,70,1,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    /* truncus 113 (ex U+1FA00) */
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    70,70,70,70,70,70,70,70,70,70,70,70,70,69,69,69,
    70,70,70,70,70,70,70,70,70,69,69,69,69,69,69,69,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,70,70,69,70,
    70,70,70,70,70,70,69,69,69,69,69,69,69,69,70,70,
    70,70,70,70,70,70,70,70,70,70,70,70,69,69,69,69,
    70,70,70,70,70,70,70,70,70,69,69,69,69,69,69,69,
    70,70,70,70,70,70,70,70,70,69,69,69,69,69,69,69,
    /* truncus 114 (ex U+1FC00) */
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    /* truncus 115 (ex U+1FF00) */
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,
    69,69,69,69,69,69,69,69,69,69,69,69,69,69,1,1,
    /* truncus 116 (ex U+2FF00) */
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,
    /* truncus 117 (ex U+E0000) */
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    /* truncus 118 (ex U+E0100) */
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    /* truncus 119 (ex U+E0200) */
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12
};

/* ================= ex lib/series_terminalis.c ================= */

nomen enumeratio {
    STATUS_SOLUM = ZEPHYRUM,
    STATUS_FUGAE,               /* post ESC */
    STATUS_FUGAE_INTERMEDIA,
    STATUS_CSI_INITIUM,
    STATUS_CSI_PARAMETRUM,
    STATUS_CSI_INTERMEDIA,
    STATUS_CSI_IGNORARE,
    STATUS_SS,                  /* post ESC N|O (divergentia) */
    STATUS_DCS_INITIUM,
    STATUS_DCS_PARAMETRUM,
    STATUS_DCS_INTERMEDIA,
    STATUS_DCS_TRANSITUS,       /* corpus DCS */
    STATUS_DCS_IGNORARE,
    STATUS_OSC,
    STATUS_APC,                 /* SOS / PM / APC */
    STATUS_NUMERUS
} SeriesStatus;

nomen enumeratio {
    ACTIO_NULLA = ZEPHYRUM,
    ACTIO_IGNORARE,
    ACTIO_IMPRIMERE,
    ACTIO_EXSEQUI,
    ACTIO_COLLIGERE,
    ACTIO_PRIVATUM,
    ACTIO_PARAMETRUM,
    ACTIO_ESC,
    ACTIO_CSI,
    ACTIO_SS,
    ACTIO_DCS_INCIPERE,
    ACTIO_PONERE
} SeriesActio;

structura TesseraSeriesLector {
    /* tabula transitionum: [octetus][status] */
     i8 tabula_status[CCLVI][STATUS_NUMERUS];
     i8 tabula_actio[CCLVI][STATUS_NUMERUS];

    i32 status;
    b32 initus;           /* modus initus (B1b): vide caput */
    b32 post_chordam;     /* ESC chordam clausit: '\' terminator */
    b32 crudum_esc;       /* proxima vocatio crudum = "ESC" incipit */

    /* series in constructione */
     s32 parametra[SERIES_PARAMETRA_MAXIMA];
     i32 numerus_parametrorum;
     i32 separatores;
     s32 accumulator;
     i32 digiti;            /* digiti in accumulatore */
      i8 intermedia[SERIES_INTERMEDIA_MAXIMA];
     i32 numerus_intermediorum;
      i8 privatum;
      i8 introductor;
      i8 finale_dcs;
     b32 praefixum;
     b32 truncatum;

      i8 TesseraChorda[SERIES_CHORDA_MAXIMA];
     i32 tessera_chorda_mensura;
      i8 crudum[SERIES_CRUDUM_MAXIMUM];
     i32 crudum_mensura;
};


/* ==================================================
 * Tabula
 * ================================================== */

interior vacuum
_regula (
    TesseraSeriesLector* lx,
             i32  ab,
             i32  ad,
             i32  status,
             i32  status_novus,
             i32  actio)
{
    i32 c;

    per (c = ab; c <= ad; c++)
    {
        lx->tabula_status[c][status]  = (i8)status_novus;
        lx->tabula_actio[c][status]   = (i8)actio;
    }
}

/* C0 sine CAN (0x18), SUB (0x1A), ESC (0x1B): illi ante tabulam */
interior vacuum
_regula_c0 (
    TesseraSeriesLector* lx,
             i32  status,
             i32  actio)
{
    _regula(lx, 0x00, 0x17, status, status, actio);
    _regula(lx, 0x19, 0x19, status, status, actio);
    _regula(lx, 0x1C, 0x1F, status, status, actio);
}

interior vacuum
_tabulam_struere (
    TesseraSeriesLector* lx)
{
    i32 s;

    /* ordinarium: status manet, nihil agitur (Ghostty .none) */
    per (s = ZEPHYRUM; s < STATUS_NUMERUS; s++)
    {
        _regula(lx, 0x00, 0xFF, s, s, ACTIO_NULLA);
    }

    /* solum: C0 et DEL exsequi (div: DEL); 0x80+ imprimere (div: C1
     * non agnita) */
    _regula_c0(lx, STATUS_SOLUM, ACTIO_EXSEQUI);
    /* CAN, SUB in solo: regimina ut cetera (in serie abrumpunt) */
    _regula(lx, 0x18, 0x18, STATUS_SOLUM, STATUS_SOLUM, ACTIO_EXSEQUI);
    _regula(lx, 0x1A, 0x1A, STATUS_SOLUM, STATUS_SOLUM, ACTIO_EXSEQUI);
    _regula(lx, 0x20, 0x7E, STATUS_SOLUM, STATUS_SOLUM,
        ACTIO_IMPRIMERE);
    _regula(lx, 0x7F, 0x7F, STATUS_SOLUM, STATUS_SOLUM, ACTIO_EXSEQUI);
    _regula(lx, 0x80, 0xFF, STATUS_SOLUM, STATUS_SOLUM,
        ACTIO_IMPRIMERE);

    /* fugae (post ESC) */
    _regula_c0(lx, STATUS_FUGAE, ACTIO_EXSEQUI);
    _regula(lx, 0x7F, 0x7F, STATUS_FUGAE, STATUS_FUGAE, ACTIO_IGNORARE);
    _regula(lx, 0x20, 0x2F, STATUS_FUGAE, STATUS_FUGAE_INTERMEDIA,
        ACTIO_COLLIGERE);
    _regula(lx, 0x30, 0x4D, STATUS_FUGAE, STATUS_SOLUM, ACTIO_ESC);
    _regula(lx, 0x4E, 0x4F, STATUS_FUGAE, STATUS_SS, ACTIO_NULLA);
    _regula(lx, 0x50, 0x50, STATUS_FUGAE, STATUS_DCS_INITIUM,
        ACTIO_NULLA);
    _regula(lx, 0x51, 0x57, STATUS_FUGAE, STATUS_SOLUM, ACTIO_ESC);
    _regula(lx, 0x58, 0x58, STATUS_FUGAE, STATUS_APC, ACTIO_NULLA);
    _regula(lx, 0x59, 0x5A, STATUS_FUGAE, STATUS_SOLUM, ACTIO_ESC);
    _regula(lx, 0x5B, 0x5B, STATUS_FUGAE, STATUS_CSI_INITIUM,
        ACTIO_NULLA);
    _regula(lx, 0x5C, 0x5C, STATUS_FUGAE, STATUS_SOLUM, ACTIO_ESC);
    _regula(lx, 0x5D, 0x5D, STATUS_FUGAE, STATUS_OSC, ACTIO_NULLA);
    _regula(lx, 0x5E, 0x5F, STATUS_FUGAE, STATUS_APC, ACTIO_NULLA);
    _regula(lx, 0x60, 0x7E, STATUS_FUGAE, STATUS_SOLUM, ACTIO_ESC);

    /* fugae intermedia */
    _regula_c0(lx, STATUS_FUGAE_INTERMEDIA, ACTIO_EXSEQUI);
    _regula(lx, 0x20, 0x2F, STATUS_FUGAE_INTERMEDIA,
        STATUS_FUGAE_INTERMEDIA, ACTIO_COLLIGERE);
    _regula(lx, 0x7F, 0x7F, STATUS_FUGAE_INTERMEDIA,
        STATUS_FUGAE_INTERMEDIA, ACTIO_IGNORARE);
    _regula(lx, 0x30, 0x7E, STATUS_FUGAE_INTERMEDIA, STATUS_SOLUM,
        ACTIO_ESC);

    /* csi initium */
    _regula_c0(lx, STATUS_CSI_INITIUM, ACTIO_EXSEQUI);
    _regula(lx, 0x7F, 0x7F, STATUS_CSI_INITIUM, STATUS_CSI_INITIUM,
        ACTIO_IGNORARE);
    _regula(lx, 0x40, 0x7E, STATUS_CSI_INITIUM, STATUS_SOLUM,
        ACTIO_CSI);
    _regula(lx, 0x3A, 0x3A, STATUS_CSI_INITIUM, STATUS_CSI_IGNORARE,
        ACTIO_NULLA);
    _regula(lx, 0x20, 0x2F, STATUS_CSI_INITIUM, STATUS_CSI_INTERMEDIA,
        ACTIO_COLLIGERE);
    _regula(lx, 0x30, 0x39, STATUS_CSI_INITIUM, STATUS_CSI_PARAMETRUM,
        ACTIO_PARAMETRUM);
    _regula(lx, 0x3B, 0x3B, STATUS_CSI_INITIUM, STATUS_CSI_PARAMETRUM,
        ACTIO_PARAMETRUM);
    _regula(lx, 0x3C, 0x3F, STATUS_CSI_INITIUM, STATUS_CSI_PARAMETRUM,
        ACTIO_PRIVATUM);

    /* csi parametrum */
    _regula_c0(lx, STATUS_CSI_PARAMETRUM, ACTIO_EXSEQUI);
    _regula(lx, 0x30, 0x3B, STATUS_CSI_PARAMETRUM,
        STATUS_CSI_PARAMETRUM,
        ACTIO_PARAMETRUM);
    _regula(lx, 0x7F, 0x7F, STATUS_CSI_PARAMETRUM,
        STATUS_CSI_PARAMETRUM,
        ACTIO_IGNORARE);
    _regula(lx, 0x40, 0x7E, STATUS_CSI_PARAMETRUM, STATUS_SOLUM,
        ACTIO_CSI);
    _regula(lx, 0x3C, 0x3F, STATUS_CSI_PARAMETRUM, STATUS_CSI_IGNORARE,
        ACTIO_NULLA);
    _regula(lx, 0x20, 0x2F, STATUS_CSI_PARAMETRUM,
        STATUS_CSI_INTERMEDIA,
        ACTIO_COLLIGERE);

    /* csi intermedia */
    _regula_c0(lx, STATUS_CSI_INTERMEDIA, ACTIO_EXSEQUI);
    _regula(lx, 0x20, 0x2F, STATUS_CSI_INTERMEDIA,
        STATUS_CSI_INTERMEDIA,
        ACTIO_COLLIGERE);
    _regula(lx, 0x7F, 0x7F, STATUS_CSI_INTERMEDIA,
        STATUS_CSI_INTERMEDIA,
        ACTIO_IGNORARE);
    _regula(lx, 0x40, 0x7E, STATUS_CSI_INTERMEDIA, STATUS_SOLUM,
        ACTIO_CSI);
    _regula(lx, 0x30, 0x3F, STATUS_CSI_INTERMEDIA, STATUS_CSI_IGNORARE,
        ACTIO_NULLA);

    /* csi ignorare: series mala tota consumitur, tacite (Ghostty) */
    _regula_c0(lx, STATUS_CSI_IGNORARE, ACTIO_EXSEQUI);
    _regula(lx, 0x20, 0x3F, STATUS_CSI_IGNORARE, STATUS_CSI_IGNORARE,
        ACTIO_IGNORARE);
    _regula(lx, 0x7F, 0x7F, STATUS_CSI_IGNORARE, STATUS_CSI_IGNORARE,
        ACTIO_IGNORARE);
    _regula(lx, 0x40, 0x7E, STATUS_CSI_IGNORARE, STATUS_SOLUM,
        ACTIO_IGNORARE);

    /* ss (div): ESC N|O parametra? finale - 'ESC O 2 P' */
    _regula_c0(lx, STATUS_SS, ACTIO_EXSEQUI);
    _regula(lx, 0x30, 0x39, STATUS_SS, STATUS_SS, ACTIO_PARAMETRUM);
    _regula(lx, 0x3B, 0x3B, STATUS_SS, STATUS_SS, ACTIO_PARAMETRUM);
    _regula(lx, 0x7F, 0x7F, STATUS_SS, STATUS_SS, ACTIO_IGNORARE);
    _regula(lx, 0x20, 0x2F, STATUS_SS, STATUS_SOLUM, ACTIO_SS);
    _regula(lx, 0x3A, 0x3A, STATUS_SS, STATUS_SOLUM, ACTIO_SS);
    _regula(lx, 0x3C, 0x7E, STATUS_SS, STATUS_SOLUM, ACTIO_SS);

    /* dcs initium */
    _regula_c0(lx, STATUS_DCS_INITIUM, ACTIO_IGNORARE);
    _regula(lx, 0x7F, 0x7F, STATUS_DCS_INITIUM, STATUS_DCS_INITIUM,
        ACTIO_IGNORARE);
    _regula(lx, 0x20, 0x2F, STATUS_DCS_INITIUM, STATUS_DCS_INTERMEDIA,
        ACTIO_COLLIGERE);
    _regula(lx, 0x3A, 0x3A, STATUS_DCS_INITIUM, STATUS_DCS_IGNORARE,
        ACTIO_NULLA);
    _regula(lx, 0x30, 0x39, STATUS_DCS_INITIUM, STATUS_DCS_PARAMETRUM,
        ACTIO_PARAMETRUM);
    _regula(lx, 0x3B, 0x3B, STATUS_DCS_INITIUM, STATUS_DCS_PARAMETRUM,
        ACTIO_PARAMETRUM);
    _regula(lx, 0x3C, 0x3F, STATUS_DCS_INITIUM, STATUS_DCS_PARAMETRUM,
        ACTIO_PRIVATUM);
    _regula(lx, 0x40, 0x7E, STATUS_DCS_INITIUM, STATUS_DCS_TRANSITUS,
        ACTIO_DCS_INCIPERE);

    /* dcs parametrum */
    _regula_c0(lx, STATUS_DCS_PARAMETRUM, ACTIO_IGNORARE);
    _regula(lx, 0x30, 0x39, STATUS_DCS_PARAMETRUM,
        STATUS_DCS_PARAMETRUM,
        ACTIO_PARAMETRUM);
    _regula(lx, 0x3B, 0x3B, STATUS_DCS_PARAMETRUM,
        STATUS_DCS_PARAMETRUM,
        ACTIO_PARAMETRUM);
    _regula(lx, 0x7F, 0x7F, STATUS_DCS_PARAMETRUM,
        STATUS_DCS_PARAMETRUM,
        ACTIO_IGNORARE);
    _regula(lx, 0x3A, 0x3A, STATUS_DCS_PARAMETRUM, STATUS_DCS_IGNORARE,
        ACTIO_NULLA);
    _regula(lx, 0x3C, 0x3F, STATUS_DCS_PARAMETRUM, STATUS_DCS_IGNORARE,
        ACTIO_NULLA);
    _regula(lx, 0x20, 0x2F, STATUS_DCS_PARAMETRUM,
        STATUS_DCS_INTERMEDIA,
        ACTIO_COLLIGERE);
    _regula(lx, 0x40, 0x7E, STATUS_DCS_PARAMETRUM, STATUS_DCS_TRANSITUS,
        ACTIO_DCS_INCIPERE);

    /* dcs intermedia */
    _regula_c0(lx, STATUS_DCS_INTERMEDIA, ACTIO_IGNORARE);
    _regula(lx, 0x20, 0x2F, STATUS_DCS_INTERMEDIA,
        STATUS_DCS_INTERMEDIA,
        ACTIO_COLLIGERE);
    _regula(lx, 0x7F, 0x7F, STATUS_DCS_INTERMEDIA,
        STATUS_DCS_INTERMEDIA,
        ACTIO_IGNORARE);
    _regula(lx, 0x30, 0x3F, STATUS_DCS_INTERMEDIA, STATUS_DCS_IGNORARE,
        ACTIO_NULLA);
    _regula(lx, 0x40, 0x7E, STATUS_DCS_INTERMEDIA, STATUS_DCS_TRANSITUS,
        ACTIO_DCS_INCIPERE);

    /* dcs transitus (corpus) et ignorare */
    _regula_c0(lx, STATUS_DCS_TRANSITUS, ACTIO_PONERE);
    _regula(lx, 0x20, 0x7E, STATUS_DCS_TRANSITUS, STATUS_DCS_TRANSITUS,
        ACTIO_PONERE);
    _regula(lx, 0x7F, 0x7F, STATUS_DCS_TRANSITUS, STATUS_DCS_TRANSITUS,
        ACTIO_IGNORARE);
    _regula(lx, 0x80, 0xFF, STATUS_DCS_TRANSITUS, STATUS_DCS_TRANSITUS,
        ACTIO_PONERE);
    _regula(lx, 0x00, 0xFF, STATUS_DCS_IGNORARE, STATUS_DCS_IGNORARE,
        ACTIO_IGNORARE);

    /* osc: C0 ignorantur (BEL ante tabulam terminat); 0x20+ corpus */
    _regula_c0(lx, STATUS_OSC, ACTIO_IGNORARE);
    _regula(lx, 0x20, 0xFF, STATUS_OSC, STATUS_OSC, ACTIO_PONERE);

    /* apc / pm / sos: omnia corpus (BEL quoque) */
    _regula_c0(lx, STATUS_APC, ACTIO_PONERE);
    _regula(lx, 0x20, 0xFF, STATUS_APC, STATUS_APC, ACTIO_PONERE);
}


/* ==================================================
 * Auxilia
 * ================================================== */

/* Visus sine copia: chorda.datum non-constans est, initus et
 * tabulae lectoris constantia (unio contra -Wcast-qual, mos domus) */
interior TesseraChorda
_visus (
    constans i8* octeti,
            i32  mensura)
{
    TesseraChorda c;
    unio { constans i8* l; i8* m; } u;

    u.l        = octeti;
    c.datum    = u.m;
    c.mensura  = mensura;
    redde c;
}

interior vacuum
_seriem_vacare (
    TesseraSeriesLector* lx)
{
    lx->numerus_parametrorum   = ZEPHYRUM;
    lx->separatores            = ZEPHYRUM;
    lx->accumulator            = ZEPHYRUM;
    lx->digiti                 = ZEPHYRUM;
    lx->numerus_intermediorum  = ZEPHYRUM;
    lx->privatum               = ZEPHYRUM;
    lx->introductor            = ZEPHYRUM;
    lx->finale_dcs             = ZEPHYRUM;
    lx->praefixum              = FALSUM;
    lx->truncatum              = FALSUM;
    lx->tessera_chorda_mensura         = ZEPHYRUM;
}

interior vacuum
_crudum_addere (
    TesseraSeriesLector* lx,
              i8  c)
{
    si (lx->crudum_mensura < SERIES_CRUDUM_MAXIMUM)
    {
        lx->crudum[lx->crudum_mensura] = c;
        lx->crudum_mensura++;
    }
}

interior vacuum
_chordae_addere (
    TesseraSeriesLector* lx,
             i8  c)
{
    si (lx->tessera_chorda_mensura < SERIES_CHORDA_MAXIMA)
    {
        lx->TesseraChorda[lx->tessera_chorda_mensura] = c;
        lx->tessera_chorda_mensura++;
    }
    alioquin
    {
        lx->truncatum = VERUM;
    }
}

/* Accumulatorem in parametrum vertere (si digiti adsunt) */
interior vacuum
_parametrum_figere (
    TesseraSeriesLector* lx)
{
    si (   lx->digiti > ZEPHYRUM
        && lx->numerus_parametrorum < SERIES_PARAMETRA_MAXIMA)
    {
        lx->parametra[lx->numerus_parametrorum] = lx->accumulator;
        lx->numerus_parametrorum++;
    }
}

interior vacuum
_parametrum (
    TesseraSeriesLector* lx,
             i32  c)
{
    s32 d;

    si (c == ';' || c == ':')
    {
        /* nimia: separator neglectus; series in fine abicitur */
        si (lx->numerus_parametrorum >= SERIES_PARAMETRA_MAXIMA)
        {
            redde;
        }
        lx->parametra[lx->numerus_parametrorum] = lx->accumulator;
        si (c == ':')
        {
            lx->separatores = lx->separatores
                | ((i32)I << lx->numerus_parametrorum);
        }
        lx->numerus_parametrorum++;
        lx->accumulator  = ZEPHYRUM;
        lx->digiti       = ZEPHYRUM;
        redde;
    }
    d = (s32)(c - '0');
    si (lx->accumulator > (SERIES_PARAMETRUM_MAXIMUM - d) / X)
    {
        lx->accumulator = SERIES_PARAMETRUM_MAXIMUM;
    }
    alioquin
    {
        lx->accumulator = lx->accumulator * X + d;
    }
    lx->digiti++;
}

/* Lexema ex statu seriei (parametra, intermedia, crudum ...) */
interior vacuum
_lexema_implere (
    constans TesseraSeriesLector* lx,
             SeriesLexema* l,
              SeriesGenus  genus,
                      i32  finale)
{
    memset(l, ZEPHYRUM, magnitudo(SeriesLexema));
    l->genus                 = genus;
    l->numerus_parametrorum  = lx->numerus_parametrorum;
    memcpy(l->parametra, lx->parametra,
        (memoriae_index)lx->numerus_parametrorum * magnitudo(s32));
    l->separatores            = lx->separatores;
    l->numerus_intermediorum  = lx->numerus_intermediorum;
    memcpy(l->intermedia, lx->intermedia,
        (memoriae_index)lx->numerus_intermediorum);
    l->privatum     = lx->privatum;
    l->introductor  = lx->introductor;
    l->finale       = (i8)finale;
    l->praefixum    = lx->praefixum;
    l->truncatum    = lx->truncatum;
    l->crudum       = _visus(lx->crudum, lx->crudum_mensura);
}

interior vacuum
_fugam_implere (
    constans TesseraSeriesLector* lx,
             SeriesLexema* l)
{
    _lexema_implere(lx, l, SERIES_FUGA, ZEPHYRUM);
    l->numerus_parametrorum   = ZEPHYRUM;
    l->separatores            = ZEPHYRUM;
    l->numerus_intermediorum  = ZEPHYRUM;
    l->privatum               = ZEPHYRUM;
    l->truncatum              = FALSUM;
}

/* Modus initus: series incepta (ESC P) re vera
 * 'alterum + introductor' erat - ESC finale introductor, octetus
 * currens NON consumptus. */
interior vacuum
_alterum_reddere (
    TesseraSeriesLector* lx,
    SeriesLexema* l,
              i8  finale)
{
    lx->introductor = ZEPHYRUM;
    _lexema_implere(lx, l, SERIES_ESC, (i32)finale);
    l->numerus_parametrorum  = ZEPHYRUM;
    l->separatores           = ZEPHYRUM;
    lx->status               = STATUS_SOLUM;
}

interior b32
_est_chorda (
    i32 status)
{
    redde status == STATUS_OSC || status == STATUS_APC
        || status == STATUS_DCS_TRANSITUS
        || status == STATUS_DCS_IGNORARE;
}

/* Chordam clausam reddere (OSC, APC, DCS); DCS_IGNORARE: nihil */
interior b32
_chordam_reddere (
    constans TesseraSeriesLector* lx,
             SeriesLexema* l)
{
    SeriesGenus genus;
            i32 finale = ZEPHYRUM;

    si (lx->status == STATUS_DCS_IGNORARE)
    {
        redde FALSUM;
    }
    genus = (lx->status == STATUS_OSC) ? SERIES_OSC
          : (lx->status == STATUS_APC) ? SERIES_APC : SERIES_DCS;
    si (genus == SERIES_DCS)
    {
        finale = (i32)lx->finale_dcs;
    }
    _lexema_implere(lx, l, genus, finale);
    l->textus = _visus(lx->TesseraChorda, lx->tessera_chorda_mensura);
    redde VERUM;
}


/* ==================================================
 * Publica
 * ================================================== */

static TesseraSeriesLector*
tessera_series_lectorem_creare (
    TesseraPiscina* piscina)
{
    TesseraSeriesLector* lx;

    lx = (TesseraSeriesLector*)tessera_piscina_allocare_ordinatum(piscina,
        magnitudo(TesseraSeriesLector), VIII);
    si (lx == NIHIL)
    {
        redde NIHIL;
    }
    _tabulam_struere(lx);
    lx->initus = FALSUM;
    tessera_series_lectorem_purgare(lx);
    redde lx;
}

static vacuum
tessera_series_lectorem_initus_ponere (
    TesseraSeriesLector* lx,
             b32  initus)
{
    lx->initus = initus;
}

static vacuum
tessera_series_lectorem_purgare (
    TesseraSeriesLector* lx)
{
    lx->status          = STATUS_SOLUM;
    lx->post_chordam    = FALSUM;
    lx->crudum_esc      = FALSUM;
    lx->crudum_mensura  = ZEPHYRUM;
    _seriem_vacare(lx);
}

static SeriesGenus
tessera_series_lexema_proximum (
     TesseraSeriesLector*  lx,
      constans i8** ptr,
      constans i8*  finis,
     SeriesLexema*  l)
{
    /* ESC qui chordam clausit seriem proximam incipit (crudum) */
    si (lx->crudum_esc)
    {
        lx->crudum_esc      = FALSUM;
        lx->crudum[0]       = (i8)0x1B;
        lx->crudum_mensura  = I;
    }

    dum (*ptr < finis)
    {
        i32 c = ((i32)**ptr) & 0xFF;
        i32 status_novus;
        i32 actio;

        /* ---- ESC: initium, praefixum, terminator, abruptio ---- */
        si (c == 0x1B)
        {
            si (_est_chorda(lx->status))
            {
                b32 habet;

                (*ptr)++;
                habet             = _chordam_reddere(lx, l);
                lx->status        = STATUS_FUGAE;
                lx->post_chordam  = VERUM;
                lx->crudum_esc    = VERUM;
                _seriem_vacare(lx);
                si (habet)
                {
                    redde l->genus;
                }
                perge;
            }
            si (   lx->status == STATUS_SOLUM
                || (lx->status == STATUS_FUGAE && lx->post_chordam))
            {
                (*ptr)++;
                lx->status        = STATUS_FUGAE;
                lx->post_chordam  = FALSUM;
                _seriem_vacare(lx);
                lx->crudum[0]       = (i8)0x1B;
                lx->crudum_mensura  = I;
                perge;
            }
            si (lx->status == STATUS_FUGAE)
            {
                /* ESC ESC: praefixum (alterum + series) */
                (*ptr)++;
                lx->praefixum = VERUM;
                _crudum_addere(lx, (i8)c);
                perge;
            }
            /* series dimidia abrupta: FUGA, ESC non consumptus */
            _fugam_implere(lx, l);
            lx->status = STATUS_SOLUM;
            redde SERIES_FUGA;
        }

        /* ---- CAN, SUB: abruptio ---- */
        si ((c == 0x18 || c == 0x1A) && lx->status != STATUS_SOLUM)
        {
            si (lx->status == STATUS_FUGAE && lx->post_chordam)
            {
                lx->status        = STATUS_SOLUM;
                lx->post_chordam  = FALSUM;
                perge;
            }
            _fugam_implere(lx, l);
            lx->status = STATUS_SOLUM;
            redde SERIES_FUGA;
        }

        /* ---- post chordam: '\' terminator, alioquin series nova */
        si (lx->status == STATUS_FUGAE && lx->post_chordam)
        {
            si (c == '\\')
            {
                (*ptr)++;
                lx->status        = STATUS_SOLUM;
                lx->post_chordam  = FALSUM;
                perge;
            }
            lx->post_chordam = FALSUM;
        }

        /* ---- MODUS INITUS: alterum + clavis, non series ---- */
        si (lx->initus && lx->status == STATUS_FUGAE)
        {
            si (c < 0x20 || c == 0x7F)
            {
                /* alterum + regimen: FUGA, octetus non consumptus */
                _fugam_implere(lx, l);
                lx->status = STATUS_SOLUM;
                redde SERIES_FUGA;
            }
            si (   (c >= 0x20 && c <= 0x2F)
                || c == 'N' || c == 'X' || c == '^')
            {
                (*ptr)++;
                _crudum_addere(lx, (i8)c);
                lx->status = STATUS_SOLUM;
                _lexema_implere(lx, l, SERIES_ESC, c);
                redde SERIES_ESC;
            }
        }
        si (lx->initus)
        {
            si (   lx->status == STATUS_DCS_INITIUM
                && c >= 0x40 && c <= 0x7E
                && lx->numerus_parametrorum == ZEPHYRUM
                && lx->digiti == ZEPHYRUM && lx->privatum == ZEPHYRUM
                && lx->numerus_intermediorum == ZEPHYRUM)
            {
                _alterum_reddere(lx, l, (i8)'P');
                redde SERIES_ESC;
            }
        }

        /* ---- ESC + octetus altus: ESC solus (alterum + UTF-8) ---- */
        si (lx->status == STATUS_FUGAE && c >= 0x80)
        {
            _fugam_implere(lx, l);
            lx->status = STATUS_SOLUM;
            redde SERIES_FUGA;
        }

        /* ---- OSC: BEL terminat ---- */
        si (lx->status == STATUS_OSC && c == 0x07)
        {
            (*ptr)++;
            (vacuum)_chordam_reddere(lx, l);
            lx->status = STATUS_SOLUM;
            redde SERIES_OSC;
        }

        status_novus  = (i32)lx->tabula_status[c][lx->status];
        actio         = (i32)lx->tabula_actio[c][lx->status];

        /* ---- solum: cursus imprimibilis (visus in initum) ---- */
        si (actio == ACTIO_IMPRIMERE)
        {
            constans i8* initium = *ptr;

            dum (*ptr < finis)
            {
                i32 d = ((i32)**ptr) & 0xFF;

                si (lx->tabula_actio[d][STATUS_SOLUM]
                    != ACTIO_IMPRIMERE)
                {
                    frange;
                }
                (*ptr)++;
            }
            memset(l, ZEPHYRUM, magnitudo(SeriesLexema));
            l->genus   = SERIES_IMPRIMERE;
            l->textus  = _visus(initium, (i32)(*ptr - initium));
            redde SERIES_IMPRIMERE;
        }

        (*ptr)++;
        si (actio == ACTIO_EXSEQUI)
        {
            /* regimen: lexema proprium; series (si qua) manet */
            memset(l, ZEPHYRUM, magnitudo(SeriesLexema));
            l->genus   = SERIES_EXSEQUI;
            l->finale  = (i8)c;
            redde SERIES_EXSEQUI;
        }
        _crudum_addere(lx, (i8)c);

        /* introductor: octetus qui fugam in seriem vertit */
        si (   lx->status   == STATUS_FUGAE
            && status_novus != STATUS_FUGAE
            && status_novus != STATUS_FUGAE_INTERMEDIA
            && status_novus != STATUS_SOLUM)
        {
            lx->introductor     = (i8)c;
            lx->tessera_chorda_mensura  = ZEPHYRUM;
        }

        commutatio (actio)
        {
            casus ACTIO_COLLIGERE:
                si (lx->numerus_intermediorum
                    < SERIES_INTERMEDIA_MAXIMA)
                {
                    lx->intermedia[lx->numerus_intermediorum] = (i8)c;
                    lx->numerus_intermediorum++;
                }
                alioquin
                {
                    lx->truncatum = VERUM;
                }
                frange;

            casus ACTIO_PRIVATUM:
                lx->privatum = (i8)c;
                frange;

            casus ACTIO_PARAMETRUM:
                _parametrum(lx, c);
                frange;

            casus ACTIO_PONERE:
                _chordae_addere(lx, (i8)c);
                frange;

            casus ACTIO_DCS_INCIPERE:
                si (lx->numerus_parametrorum >= SERIES_PARAMETRA_MAXIMA)
                {
                    status_novus = STATUS_DCS_IGNORARE;
                }
                alioquin
                {
                    _parametrum_figere(lx);
                    lx->finale_dcs      = (i8)c;
                    lx->tessera_chorda_mensura  = ZEPHYRUM;
                }
                frange;

            casus ACTIO_ESC:
                lx->status = status_novus;
                _lexema_implere(lx, l, SERIES_ESC, c);
                redde SERIES_ESC;

            casus ACTIO_CSI:
            casus ACTIO_SS:
                lx->status = status_novus;
                si (lx->numerus_parametrorum >= SERIES_PARAMETRA_MAXIMA)
                {
                    perge;      /* nimia: tota abicitur (Ghostty) */
                }
                _parametrum_figere(lx);
                _lexema_implere(lx, l,
                    (actio == ACTIO_CSI) ? SERIES_CSI : SERIES_SS, c);
                redde l->genus;

            ordinarius:
                frange;
        }
        lx->status = status_novus;
    }
    redde SERIES_NIHIL;
}

static b32
tessera_series_lector_pendet (
    constans TesseraSeriesLector* lx)
{
    redde (b32)(lx->status != STATUS_SOLUM);
}

static b32
tessera_series_lectorem_evacuare (
    TesseraSeriesLector* lx,
    SeriesLexema* l)
{
    si (lx->status == STATUS_SOLUM)
    {
        redde FALSUM;
    }
    si (lx->status == STATUS_FUGAE && lx->post_chordam)
    {
        lx->status        = STATUS_SOLUM;
        lx->post_chordam  = FALSUM;
        redde FALSUM;
    }
    _fugam_implere(lx, l);
    lx->status = STATUS_SOLUM;
    redde VERUM;
}

/* ================= ex lib/claves_physicae.c ================= */

static EventusCodex
tessera_claves_codex_ex_littera (
    s32 runa)
{
    si (runa >= 'a' && runa <= 'z')
    {
        redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + (runa - 'a'));
    }
    si (runa >= '0' && runa <= '9')
    {
        redde (EventusCodex)(EVENTUS_CODEX_NUMERI + (runa - '0'));
    }
    commutatio (runa)
    {
        casus '`':  redde EVENTUS_CODEX_GRAVIS;
        casus '-':  redde EVENTUS_CODEX_MINUS;
        casus '=':  redde EVENTUS_CODEX_AEQUALE;
        casus '[':  redde EVENTUS_CODEX_UNCUS_SINISTER;
        casus ']':  redde EVENTUS_CODEX_UNCUS_DEXTER;
        casus '\\': redde EVENTUS_CODEX_VIRGULA_INVERSA;
        casus ';':  redde EVENTUS_CODEX_PUNCTUM_VIRGULA;
        casus '\'': redde EVENTUS_CODEX_APOSTROPHUS;
        casus ',':  redde EVENTUS_CODEX_VIRGULA;
        casus '.':  redde EVENTUS_CODEX_PUNCTUM;
        casus '/':  redde EVENTUS_CODEX_VIRGULA_OBLIQUA;
        casus ' ':  redde EVENTUS_CODEX_SPATIUM;
        ordinarius: redde EVENTUS_CODEX_IGNOTUS;
    }
}

/* kitty 'functional key definitions' (kitty doc keyboard-protocol;
 * Ghostty src/input/kitty.zig, MIT, pin 12752b2) */
static EventusCodex
tessera_claves_codex_ex_kitty (
    s32 numerus)
{
    commutatio (numerus)
    {
        casus XXVII:  redde EVENTUS_CODEX_EFFUGIUM;
        casus XIII:   redde EVENTUS_CODEX_REDITUS;
        casus IX:     redde EVENTUS_CODEX_TABULA;
        casus CXXVII: redde EVENTUS_CODEX_RETRORSUM;
        casus 57358:  redde EVENTUS_CODEX_SERA_MAIUSCULARUM;
        casus 57441:  redde EVENTUS_CODEX_MAIUSCULA_SINISTRA;
        casus 57447:  redde EVENTUS_CODEX_MAIUSCULA_DEXTRA;
        casus 57442:  redde EVENTUS_CODEX_IMPERIUM_SINISTRUM;
        casus 57448:  redde EVENTUS_CODEX_IMPERIUM_DEXTRUM;
        casus 57443:  redde EVENTUS_CODEX_ALTERUM_SINISTRUM;
        casus 57449:  redde EVENTUS_CODEX_ALTERUM_DEXTRUM;
        casus 57444:  redde EVENTUS_CODEX_SUPER_SINISTRUM;
        casus 57450:  redde EVENTUS_CODEX_SUPER_DEXTRUM;
        ordinarius:   redde EVENTUS_CODEX_IGNOTUS;
    }
}

/* ================= ex lib/eventus_cauda.c ================= */

static vacuum
tessera_eventus_caudam_initiare (
    EventusCauda* cauda)
{
    cauda->caput            = ZEPHYRUM;
    cauda->finis            = ZEPHYRUM;
    cauda->numerus          = ZEPHYRUM;
    cauda->amissa           = ZEPHYRUM;
    cauda->textus_mensura   = ZEPHYRUM;
    cauda->exempla_mensura  = ZEPHYRUM;
}

static vacuum
tessera_eventus_cauda_lectio_incipit (
    EventusCauda* cauda)
{
    /* visus eventuum nondum extractorum in tabulas monstrant: vacare
     * solum si nullum restat */
    si (cauda->numerus == ZEPHYRUM)
    {
        cauda->textus_mensura   = ZEPHYRUM;
        cauda->exempla_mensura  = ZEPHYRUM;
    }
}

static b32
tessera_eventus_caudae_impellere (
          EventusCauda* cauda,
      constans Eventus* eventus)
{
    si (cauda->numerus >= EVENTUS_CAUDA_CAPACITAS)
    {
        cauda->amissa++;
        redde FALSUM;
    }
    cauda->eventus[cauda->finis] = *eventus;
    cauda->finis = (cauda->finis + I) % EVENTUS_CAUDA_CAPACITAS;
    cauda->numerus++;
    redde VERUM;
}

static b32
tessera_eventus_caudae_textum_impellere (
       EventusCauda* cauda,
                s64  tempus,
        constans i8* octeti,
                i32  mensura,
       EventusOrigo  origo)
{
    Eventus e;
        i32 locus;
        i32 capit;

    si (mensura == ZEPHYRUM || octeti == NIHIL)
    {
        redde FALSUM;
    }
    locus = EVENTUS_CAUDA_TEXTUS - cauda->textus_mensura;
    capit = (mensura < locus) ? mensura : locus;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                   = EVENTUS_TEXTUS;
    e.tempus                  = tempus;
    e.datum.textus.genus      = EVENTUS_TEXTUS_COMMISSUM;
    e.datum.textus.origo      = origo;
    e.datum.textus.truncatum  = (b32)(capit < mensura);
    e.datum.textus.contentum.datum
        = cauda->textus + cauda->textus_mensura;
    e.datum.textus.contentum.mensura = capit;
    memcpy(cauda->textus + cauda->textus_mensura, octeti,
        (memoriae_index)capit);
    cauda->textus_mensura += capit;
    redde tessera_eventus_caudae_impellere(cauda, &e);
}

static b32
tessera_eventus_caudae_depositionem_impellere (
          EventusCauda* cauda,
      constans Eventus* eventus)
{
    Eventus e;
        i32 mensura = eventus->datum.depositio.viae.mensura;

    si (mensura > EVENTUS_CAUDA_TEXTUS - cauda->textus_mensura)
    {
        redde FALSUM;
    }
    e = *eventus;
    e.datum.depositio.viae.datum = cauda->textus
        + cauda->textus_mensura;
    si (mensura > ZEPHYRUM)
    {
        memcpy(cauda->textus + cauda->textus_mensura,
            eventus->datum.depositio.viae.datum,
            (memoriae_index)mensura);
    }
    cauda->textus_mensura += mensura;
    redde tessera_eventus_caudae_impellere(cauda, &e);
}

static b32
tessera_eventus_caudae_motum_impellere (
          EventusCauda* cauda,
      constans Eventus* eventus)
{
    Eventus* ultimus;

    si (cauda->numerus == ZEPHYRUM)
    {
        redde tessera_eventus_caudae_impellere(cauda, eventus);
    }
    ultimus = &cauda->eventus[(cauda->finis + EVENTUS_CAUDA_CAPACITAS
        - I)
        % EVENTUS_CAUDA_CAPACITAS];
    si (   ultimus->genus               != EVENTUS_MUS_MOTUS
        || ultimus->datum.mus.botton    != eventus->datum.mus.botton
        || ultimus->datum.mus.modificantes
               != eventus->datum.mus.modificantes
        || ultimus->datum.mus.indicator != eventus->datum.mus.indicator
        || ultimus->datum.mus.indicator_genus
               != eventus->datum.mus.indicator_genus)
    {
        redde tessera_eventus_caudae_impellere(cauda, eventus);
    }

    /* positio ultimi exemplum fit - si capit (D5: antiquissima
     * servantur). Exempla ultimi in fine tabulae iacent: nullus
     * eventus post eum exempla addidit. */
    si (   ultimus->datum.mus.numerus_exemplorum
        < EVENTUS_EXEMPLA_MAXIMA
        && cauda->exempla_mensura < EVENTUS_CAUDA_EXEMPLA)
    {
        EventusExemplum* ex = &cauda->exempla[cauda->exempla_mensura];

        si (ultimus->datum.mus.numerus_exemplorum == ZEPHYRUM)
        {
            ultimus->datum.mus.exempla = ex;
        }
        ex->x       = ultimus->datum.mus.x;
        ex->y       = ultimus->datum.mus.y;
        ex->tempus  = ultimus->tempus;
        cauda->exempla_mensura++;
        ultimus->datum.mus.numerus_exemplorum++;
    }
    ultimus->datum.mus.x        = eventus->datum.mus.x;
    ultimus->datum.mus.y        = eventus->datum.mus.y;
    ultimus->datum.mus.pressio  = eventus->datum.mus.pressio;
    ultimus->tempus             = eventus->tempus;
    redde VERUM;
}

static b32
tessera_eventus_caudae_extrahere (
    EventusCauda* cauda,
         Eventus* exitus)
{
    si (cauda->numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    *exitus       = cauda->eventus[cauda->caput];
    cauda->caput  = (cauda->caput + I) % EVENTUS_CAUDA_CAPACITAS;
    cauda->numerus--;
    redde VERUM;
}

/* ================= ex lib/interpres_terminalis.c ================= */

#define MODIFICATOR_MAXIMUS CCLVI   /* ultra: invalidum (ingens) */


/* ==================================================
 * Auxilia
 * ================================================== */

interior i32
_modificantes_csi (
    s32 m)
{
    i32 fructus = ZEPHYRUM;
    s32 bits;

    si (m <= I || m > MODIFICATOR_MAXIMUS)
    {
        redde ZEPHYRUM;
    }
    bits = m - I;
    si (bits & I)
    { fructus |= MOD_SHIFT;
    }
    si (bits & II)
    { fructus |= MOD_ALT;
    }
    si (bits & IV)
    { fructus |= MOD_IMPERIUM;
    }
    si (bits & VIII)
    { fructus |= MOD_SUPER;
    }
    /* kitty: XVI hyper et XXXII meta sine pari in vocabulario */
    si (bits & LXIV)
    { fructus |= MOD_CAPS_LOCK;
    }
    si (bits & CXXVIII)
    { fructus |= MOD_NUM_LOCK;
    }
    redde fructus;
}

/* Clavis cum actione (B2b): SOLUTA -> genus LIBERATUS */
interior i32
_clavem_typo (
    EventusCauda* cauda,
             s64  tempus,
        clavis_t  clavis,
             s32  runa,
             i32  modificantes,
    EventusCodex  codex,
    EventusActio  actio,
       character  typus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = (actio == EVENTUS_ACTIO_SOLUTA)
        ? EVENTUS_CLAVIS_LIBERATUS : EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus                     = tempus;
    e.datum.clavis.clavis        = clavis;
    e.datum.clavis.typus         = typus;
    e.datum.clavis.modificantes  = modificantes;
    e.datum.clavis.runa          = runa;
    e.datum.clavis.codex         = codex;
    e.datum.clavis.actio         = actio;
    redde tessera_eventus_caudae_impellere(cauda, &e) ? I : ZEPHYRUM;
}

/* typus ex clave (nominatae: regimen eius, e.g. '\r', '\t') */
interior i32
_clavem_actio (
    EventusCauda* cauda,
             s64  tempus,
        clavis_t  clavis,
             s32  runa,
             i32  modificantes,
    EventusCodex  codex,
    EventusActio  actio)
{
    redde _clavem_typo(cauda, tempus, clavis, runa, modificantes, codex,
        actio, ((s32)clavis > ZEPHYRUM && (s32)clavis < CXXVIII)
                   ? (character)clavis : '\0');
}

interior i32
_clavem (
    EventusCauda* cauda,
             s64  tempus,
        clavis_t  clavis,
             s32  runa,
             i32  modificantes,
    EventusCodex  codex)
{
    redde _clavem_actio(cauda, tempus, clavis, runa, modificantes,
        codex,
        EVENTUS_ACTIO_PRESSA);
}

/* Runa imprimibilis -> clavis logica (litterae MAIUSCULAE ut fenestra;
 * runa litterarum minuscula - Shift nescitur, textus casum fert) */
interior i32
_runae_clavem (
    EventusCauda* cauda,
             s64  tempus,
             s32  r,
             i32  modificantes)
{
    clavis_t clavis  = CLAVIS_IGNOTA;
         s32 runa    = r;

    si (r >= 'a' && r <= 'z')
    {
        clavis  = (clavis_t)(r - 'a' + 'A');
    }
    alioquin si (r >= 'A' && r <= 'Z')
    {
        clavis  = (clavis_t)r;
        runa    = r - 'A' + 'a';
    }
    alioquin si (r >= 0x20 && r < 0x7F)
    {
        clavis = (clavis_t)r;
    }
    /* typus = character VERUS (ut fenestra characters[0]): 'A' ab
     * 'a' discernit etiam ubi textus deest (alterum) - B3a */
    redde _clavem_typo(cauda, tempus, clavis, runa, modificantes,
        EVENTUS_CODEX_IGNOTUS, EVENTUS_ACTIO_PRESSA,
        (r > ZEPHYRUM && r < CXXVIII) ? (character)r : '\0');
}

/* Octetus regiminis (C0, DEL) -> clavis. HONESTA: '\n' = Ctrl+J,
 * 0x08 = Ctrl+H (proiectio tesserae eas coniungit). */
interior i32
_regimen (
    EventusCauda* cauda,
             s64  tempus,
             i32  b,
             i32  modificantes)
{
    si (b == 0x0D)
    {
        redde _clavem(cauda, tempus, CLAVIS_REDITUS, ZEPHYRUM,
            modificantes, EVENTUS_CODEX_IGNOTUS);
    }
    si (b == 0x09)
    {
        redde _clavem(cauda, tempus, CLAVIS_TABULA, ZEPHYRUM,
            modificantes, EVENTUS_CODEX_IGNOTUS);
    }
    si (b == 0x7F)
    {
        redde _clavem(cauda, tempus, CLAVIS_RETRORSUM, ZEPHYRUM,
            modificantes, EVENTUS_CODEX_IGNOTUS);
    }
    si (b == ZEPHYRUM)
    {
        redde _clavem(cauda, tempus, CLAVIS_SPATIUM, (s32)' ',
            modificantes | MOD_IMPERIUM, EVENTUS_CODEX_IGNOTUS);
    }
    si (b >= I && b <= XXVI)
    {
        redde _clavem(cauda, tempus, (clavis_t)('A' + b - I),
            (s32)('a' + b - I), modificantes | MOD_IMPERIUM,
            EVENTUS_CODEX_IGNOTUS);
    }
    si (b >= 0x1C && b <= 0x1F)
    {
        redde _clavem(cauda, tempus, (clavis_t)(b | 0x40),
            (s32)(b | 0x40), modificantes | MOD_IMPERIUM,
            EVENTUS_CODEX_IGNOTUS);
    }
    redde ZEPHYRUM;
}

/* Clavis nominata ex finali (CSI aut SS3) */
interior i32
_finalem (
    EventusCauda* cauda,
             s64  tempus,
             i32  finale,
             i32  modificantes,
    EventusActio  actio)
{
    commutatio (finale)
    {
        casus 'A': redde _clavem_actio(cauda, tempus, CLAVIS_SURSUM,
                       ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_SAGITTA_SURSUM,
                       actio);
        casus 'B': redde _clavem_actio(cauda, tempus, CLAVIS_DEORSUM,
                       ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_SAGITTA_DEORSUM,
                       actio);
        casus 'C': redde _clavem_actio(cauda, tempus, CLAVIS_DEXTER,
                       ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_SAGITTA_DEXTRA,
                       actio);
        casus 'D': redde _clavem_actio(cauda, tempus, CLAVIS_SINISTER,
                       ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_SAGITTA_SINISTRA,
                       actio);
        casus 'H': redde _clavem_actio(cauda, tempus, CLAVIS_DOMUS,
                       ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_DOMUS, actio);
        casus 'F': redde _clavem_actio(cauda, tempus, CLAVIS_FINIS,
                       ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_FINIS, actio);
        casus 'Z': redde _clavem_actio(cauda, tempus, CLAVIS_TABULA,
                       ZEPHYRUM,
                       modificantes | MOD_SHIFT, EVENTUS_CODEX_TABULA,
                       actio);
        ordinarius:
            frange;
    }
    redde ZEPHYRUM;
}

/* F n (1..12) */
interior i32
_functionem (
    EventusCauda* cauda,
             s64  tempus,
             s32  n,
             i32  modificantes,
    EventusActio  actio)
{
    redde _clavem_actio(cauda, tempus, (clavis_t)((s32)CLAVIS_F1 + n
        - I),
        ZEPHYRUM, modificantes,
        (EventusCodex)((s32)EVENTUS_CODEX_FUNCTIONES + n - I), actio);
}

/* '~'-codices (xterm/vt220) */
interior i32
_clavem_tildae (
    EventusCauda* cauda,
             s64  tempus,
             s32  codex,
             i32  modificantes,
    EventusActio  actio)
{
    commutatio (codex)
    {
        casus I:
        casus VII:
            redde _clavem_actio(cauda, tempus, CLAVIS_DOMUS, ZEPHYRUM,
                modificantes, EVENTUS_CODEX_DOMUS, actio);
        casus IV:
        casus VIII:
            redde _clavem_actio(cauda, tempus, CLAVIS_FINIS, ZEPHYRUM,
                modificantes, EVENTUS_CODEX_FINIS, actio);
        casus II:
            redde _clavem_actio(cauda, tempus, CLAVIS_IGNOTA, ZEPHYRUM,
                modificantes, EVENTUS_CODEX_INSERERE, actio);
        casus III:
            redde _clavem_actio(cauda, tempus, CLAVIS_DELERE, ZEPHYRUM,
                modificantes, EVENTUS_CODEX_DELERE, actio);
        casus V:
            redde _clavem_actio(cauda, tempus, CLAVIS_PAGINA_SURSUM,
                ZEPHYRUM,
                modificantes, EVENTUS_CODEX_PAGINA_SURSUM, actio);
        casus VI:
            redde _clavem_actio(cauda, tempus, CLAVIS_PAGINA_DEORSUM,
                ZEPHYRUM,
                modificantes, EVENTUS_CODEX_PAGINA_DEORSUM, actio);
        ordinarius:
            frange;
    }
    si (codex >= XI && codex <= XV)
    {
        redde _functionem(cauda, tempus, codex - X, modificantes,
            actio);
    }
    si (codex >= XVII && codex <= XXI)
    {
        redde _functionem(cauda, tempus, codex - XI, modificantes,
            actio);
    }
    si (codex == XXIII || codex == XXIV)
    {
        redde _functionem(cauda, tempus, codex - XII, modificantes,
            actio);
    }
    redde ZEPHYRUM;   /* ignota (200/201 glutinum: fons) */
}

/* Mus (SGR aut X10): b = codex bottonis (bits 0-1 botton, 4 maiuscula,
 * 8 alterum, 16 imperium, 32 motus, 64 rota); x, y cellulae 1-basatae
 * -> CENTRUM cellulae in pixelis nostris. Rota: gradus = cellula
 * altitudo; 64 sursum = dy +, 65 = dy -, 66 = dx +, 67 = dx -. */
interior i32
_murem (
    InterpresTerminalis* in,
           EventusCauda* cauda,
                    s64  tempus,
                    s32  b,
                    s32  x,
                    s32  y,
                    b32  solutio)
{
              Eventus e;
                  i32 modi   = ZEPHYRUM;
                  s32 basis  = b & III;
         mus_botton_t botton;

    si (b & IV)
    { modi |= MOD_SHIFT;
    }
    si (b & VIII)
    { modi |= MOD_ALT;
    }
    si (b & XVI)
    { modi |= MOD_IMPERIUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.tempus = tempus;
    /* centrum cellulae; memoratur pro depositione (B3b) */
    in->indicator_x = (x - I) * in->cellula_latitudo
                      + in->cellula_latitudo / II;
    in->indicator_y = (y - I) * in->cellula_altitudo
                      + in->cellula_altitudo / II;
    si (b & LXIV)
    {
        s32 g = in->cellula_altitudo;

        si (solutio || (b & XXXII))
        {
            /* rota solutionem non habet; motus + rota (96/97) tacite,
             * ut tessera (B3a) */
            redde ZEPHYRUM;
        }
        /* B3a: positio (centrum cellulae) et modificantes */
        e.datum.rotula.x             = in->indicator_x;
        e.datum.rotula.y             = in->indicator_y;
        e.datum.rotula.modificantes  = modi;
        e.genus                      = EVENTUS_MUS_ROTULA;
        e.datum.rotula.genus         = EVENTUS_ROTULA_GRADATA;
        e.datum.rotula.dy      = (basis == ZEPHYRUM) ? g
                               : (basis == I) ? -g : ZEPHYRUM;
        e.datum.rotula.dx      = (basis == II) ? g
                               : (basis == III) ? -g : ZEPHYRUM;
        e.datum.rotula.delta_x = (f32)e.datum.rotula.dx;
        e.datum.rotula.delta_y = (f32)e.datum.rotula.dy;
        redde tessera_eventus_caudae_impellere(cauda, &e) ? I : ZEPHYRUM;
    }
    botton = (basis == ZEPHYRUM) ? MUS_SINISTER
           : (basis == I) ? MUS_MEDIUS
           : (basis == II) ? MUS_DEXTER : (mus_botton_t)ZEPHYRUM;
    e.datum.mus.x                = in->indicator_x;
    e.datum.mus.y                = in->indicator_y;
    e.datum.mus.botton           = botton;
    e.datum.mus.modificantes     = modi;
    e.datum.mus.indicator_genus  = EVENTUS_INDICATOR_MUS;
    e.datum.mus.pressio          = EVENTUS_PRESSIO_IGNOTA;
    si (b & XXXII)
    {
        e.genus = EVENTUS_MUS_MOTUS;
        redde tessera_eventus_caudae_motum_impellere(cauda, &e) ? I : ZEPHYRUM;
    }
    e.genus = (solutio || basis == III) ? EVENTUS_MUS_LIBERATUS
                                        : EVENTUS_MUS_DEPRESSUS;
    redde tessera_eventus_caudae_impellere(cauda, &e) ? I : ZEPHYRUM;
}

/* Campi CSI (kitty): parametra per ';' in campos, ':' subcampos
 * dividit (separatores bitus i = ':' post parametrum i). */
#define CAMPI_MAXIMI III

nomen structura {
    s32 valor[CAMPI_MAXIMI][SERIES_PARAMETRA_MAXIMA];
    i32 numerus[CAMPI_MAXIMI];
} CampiCsi;

interior vacuum
_campos_legere (
    constans SeriesLexema* l,
                 CampiCsi* c)
{
    i32 k;
    i32 f = ZEPHYRUM;

    memset(c, ZEPHYRUM, magnitudo(CampiCsi));
    per (k = ZEPHYRUM; k < l->numerus_parametrorum; k++)
    {
        si (f < CAMPI_MAXIMI)
        {
            c->valor[f][c->numerus[f]] = l->parametra[k];
            c->numerus[f]++;
        }
        si (!(l->separatores & ((i32)I << k)))
        {
            f++;      /* ';' post parametrum k: campus novus */
        }
    }
}

interior s32
_campus (
    constans CampiCsi* c,
                  i32  campus,
                  i32  pars,
                  s32  si_abest)
{
    si (campus >= CAMPI_MAXIMI || pars >= c->numerus[campus])
    {
        redde si_abest;
    }
    /* campus vacuus (';;') = 0 = absens */
    redde (c->valor[campus][pars] == ZEPHYRUM)
        ? si_abest : c->valor[campus][pars];
}

/* genus kitty: 1 pressa, 2 iterata, 3 soluta */
interior EventusActio
_actio_kitty (
    s32 genus)
{
    si (genus == II)
    {
        redde EVENTUS_ACTIO_ITERATA;
    }
    si (genus == III)
    {
        redde EVENTUS_ACTIO_SOLUTA;
    }
    redde EVENTUS_ACTIO_PRESSA;
}

/* Series kitty prima: facultates discuntur (per observationem - fons
 * vexilla impulit, terminalis respondendo ea accepit) et eventus
 * FACULTATES ANTE clavem impellitur. */
interior i32
_kitty_discere (
    InterpresTerminalis* in,
                    s64  tempus,
           EventusCauda* cauda)
{
    Eventus e;

    si (in->kitty_visus)
    {
        redde ZEPHYRUM;
    }
    in->kitty_visus                  = VERUM;
    in->facultates.tabula_distincta  = VERUM;
    in->facultates.liberationes           =
        (b32)((in->kitty_vexilla & INTERPRES_KITTY_GENERA) != ZEPHYRUM);
    in->facultates.codex_physicus         =
        (b32)((in->kitty_vexilla & INTERPRES_KITTY_ALTERNAE)
            != ZEPHYRUM);
    /* B4: OMNES = claves imprimibiles ut CSI u, cum modificantibus */
    in->facultates.modificantes_textus    =
        (b32)((in->kitty_vexilla & INTERPRES_KITTY_OMNES) != ZEPHYRUM);
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_FACULTATES;
    e.tempus            = tempus;
    e.datum.facultates  = in->facultates;
    redde tessera_eventus_caudae_impellere(cauda, &e) ? I : ZEPHYRUM;
}

/* Clavis 'CSI clavis[:maiuscula[:basis]] ;modi[:genus] ;textus u' */
interior i32
_kitty_clavem (
    InterpresTerminalis* in,
      constans CampiCsi* c,
                    i32  modi,
           EventusActio  actio,
                    s64  tempus,
           EventusCauda* cauda)
{
             s32 k       = _campus(c, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
             s32 basis   = _campus(c, ZEPHYRUM, II, ZEPHYRUM);
    EventusCodex codex   = tessera_claves_codex_ex_kitty(k);
        clavis_t clavis  = CLAVIS_IGNOTA;
             s32 runa    = ZEPHYRUM;
             i32 n;
             i32 j;

    commutatio (k)
    {
        casus XXVII:  clavis = CLAVIS_EFFUGIUM;   frange;
        casus XIII:   clavis = CLAVIS_REDITUS;    frange;
        casus IX:     clavis = CLAVIS_TABULA;     frange;
        casus CXXVII: clavis = CLAVIS_RETRORSUM;  frange;
        casus 57358:  clavis = CLAVIS_CAPS_LOCK;  frange;
        casus 57360:  clavis = CLAVIS_NUM_LOCK;   frange;
        casus 57441:  clavis = CLAVIS_SINISTER_SHIFT;    frange;
        casus 57447:  clavis = CLAVIS_DEXTER_SHIFT;      frange;
        casus 57442:  clavis = CLAVIS_SINISTER_IMPERIUM; frange;
        casus 57448:  clavis = CLAVIS_DEXTER_IMPERIUM;   frange;
        casus 57443:  clavis = CLAVIS_SINISTER_ALT;      frange;
        casus 57449:  clavis = CLAVIS_DEXTER_ALT;        frange;
        casus 57444:  clavis = CLAVIS_SINISTER_SUPER;    frange;
        casus 57450:  clavis = CLAVIS_DEXTER_SUPER;      frange;
        casus 57409:  clavis = (clavis_t)'.'; runa = '.'; frange;
        casus 57410:  clavis = (clavis_t)'/'; runa = '/'; frange;
        casus 57411:  clavis = (clavis_t)'*'; runa = '*'; frange;
        casus 57412:  clavis = (clavis_t)'-'; runa = '-'; frange;
        casus 57413:  clavis = (clavis_t)'+'; runa = '+'; frange;
        casus 57414:  clavis = CLAVIS_REDITUS;              frange;
        casus 57415:  clavis = (clavis_t)'='; runa = '='; frange;
        casus 57416:  clavis = (clavis_t)','; runa = ','; frange;
        casus 57417:  clavis = CLAVIS_SINISTER;       frange;
        casus 57418:  clavis = CLAVIS_DEXTER;         frange;
        casus 57419:  clavis = CLAVIS_SURSUM;         frange;
        casus 57420:  clavis = CLAVIS_DEORSUM;        frange;
        casus 57421:  clavis = CLAVIS_PAGINA_SURSUM;  frange;
        casus 57422:  clavis = CLAVIS_PAGINA_DEORSUM; frange;
        casus 57423:  clavis = CLAVIS_DOMUS;          frange;
        casus 57424:  clavis = CLAVIS_FINIS;          frange;
        casus 57426:  clavis = CLAVIS_DELERE;         frange;
        ordinarius:
            si (k >= 57399 && k <= 57408)
            {
                /* tabula numerica 0-9 (codex nullus in vocabulario) */
                clavis  = (clavis_t)('0' + (k - 57399));
                runa    = (s32)('0' + (k - 57399));
            }
            alioquin si (k < 57344 || k > 63743)
            {
                /* runa (dispositionis currentis, sine maiuscula) */
                runa    = k;
                clavis  = (k >= 'a'
                    && k <= 'z') ? (clavis_t)(k - 'a' + 'A')
                        : (k >= 0x20 && k < 0x7F) ? (clavis_t)k
                        : CLAVIS_IGNOTA;
                codex   = (basis
                    != ZEPHYRUM) ? tessera_claves_codex_ex_littera(basis)
                        : (in->kitty_vexilla & INTERPRES_KITTY_ALTERNAE)
                            ? tessera_claves_codex_ex_littera(k)
                            : EVENTUS_CODEX_IGNOTUS;
            }
            frange;
    }
    /* typus = character verus: clavis maiuscula (campus 0 pars 1) sub
     * Shift, alioquin clavis ipsa (minuscula) - non clavis_t */
    {
        s32 verus = ((modi & MOD_SHIFT)
            && _campus(c, ZEPHYRUM, I, ZEPHYRUM))
            ? _campus(c, ZEPHYRUM, I, ZEPHYRUM) : runa;

        n = _clavem_typo(cauda, tempus, clavis, runa, modi, codex,
            actio,
            (verus >= 0x20 && verus < 0x7F) ? (character)verus : '\0');
    }
    /* textus associatus: solum si campus adest (vexillum TEXTUS) */
    si (actio != EVENTUS_ACTIO_SOLUTA && c->numerus[II] > ZEPHYRUM)
    {
         i8 octeti[SERIES_PARAMETRA_MAXIMA * IV];
        i32 m = ZEPHYRUM;

        per (j = ZEPHYRUM; j < c->numerus[II]; j++)
        {
            m += (i32)tessera_utf8_codere(c->valor[II][j], octeti + m);
        }
        si (m > ZEPHYRUM)
        {
            n += tessera_eventus_caudae_textum_impellere(cauda, tempus, octeti,
                m,
                EVENTUS_ORIGO_SCRIPTA) ? I : ZEPHYRUM;
        }
    }
    redde n;
}

interior b32
_sola_fuga (
    constans SeriesLexema* l)
{
    i32 k;

    si (l->crudum.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < l->crudum.mensura; k++)
    {
        si (l->crudum.datum[k] != (i8)0x1B)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Alterum pendens consumitur ab eventu proximo (clavis aut textus) */
interior i32
_alterum (
    InterpresTerminalis* in)
{
    i32 m = in->alterum_pendens ? MOD_ALT : ZEPHYRUM;

    in->alterum_pendens = FALSUM;
    redde m;
}


/* ==================================================
 * Publica
 * ================================================== */

static vacuum
tessera_interpres_initiare (
    InterpresTerminalis* interpres,
                    s32  cellula_latitudo,
                    s32  cellula_altitudo)
{
    interpres->cellula_latitudo  = cellula_latitudo;
    interpres->cellula_altitudo  = cellula_altitudo;
    interpres->alterum_pendens   = FALSUM;
    interpres->kitty_vexilla     = ZEPHYRUM;
    interpres->kitty_visus       = FALSUM;
    interpres->indicator_x       = ZEPHYRUM;
    interpres->indicator_y       = ZEPHYRUM;
    /* legacy: quod terminalis sine kitty narrare potest */
    memset(&interpres->facultates, ZEPHYRUM,
        magnitudo(EventusFacultates));
    /* super FALSUM donec ?1003 declaratur (rivus_modos_intrare) */
    interpres->facultates.scriptura_copiae  = EVENTUS_FACULTAS_FORTASSE;
    /* depositio NULLA donec promotio declaratur (rivus, B3b) */
    interpres->facultates.depositio       = EVENTUS_DEPOSITIO_NULLA;
    interpres->facultates.gradus_rotulae  = cellula_altitudo;
}

static i32
tessera_interpres_lexema (
         InterpresTerminalis* in,
       constans SeriesLexema* l,
                         b32  post_moram,
                         s64  tempus,
                EventusCauda* cauda)
{
    i32 n = ZEPHYRUM;
    i32 modi;

    commutatio (l->genus)
    {
        casus SERIES_IMPRIMERE:
        {
            constans i8* p      = l->textus.datum;
            constans i8* finis  = p + l->textus.mensura;

            dum (p < finis)
            {
                constans i8* initium  = p;
                        s32  r        = tessera_utf8_decodere(&p, finis);

                si (r < ZEPHYRUM)
                {
                    si (p == initium)
                    {
                        p++;    /* octetus invalidus abicitur */
                    }
                    perge;
                }
                modi  = _alterum(in);
                n     += _runae_clavem(cauda, tempus, r, modi);
                si (modi == ZEPHYRUM)
                {
                    n += tessera_eventus_caudae_textum_impellere(cauda, tempus,
                        initium, (i32)(p - initium),
                        EVENTUS_ORIGO_SCRIPTA) ? I : ZEPHYRUM;
                }
            }
            redde n;
        }

        casus SERIES_EXSEQUI:
            redde _regimen(cauda, tempus, ((i32)l->finale) & 0xFF,
                _alterum(in));

        casus SERIES_FUGA:
            si (!post_moram)
            {
                /* abrupta: ESC solus = alterum clavis proximae */
                si (_sola_fuga(l))
                {
                    in->alterum_pendens = VERUM;
                }
                redde ZEPHYRUM;
            }
            in->alterum_pendens = FALSUM;
            si (_sola_fuga(l))
            {
                i32 k;

                /* ESC (ESC) post moram: Effugium pro quoque */
                per (k = ZEPHYRUM; k < l->crudum.mensura; k++)
                {
                    n += _clavem(cauda, tempus, CLAVIS_EFFUGIUM,
                        ZEPHYRUM,
                        ZEPHYRUM, EVENTUS_CODEX_IGNOTUS);
                }
                redde n;
            }
            si (l->crudum.mensura == II)
            {
                /* 'ESC [' / 'ESC O' / 'ESC P' solum = alterum + x */
                redde _runae_clavem(cauda, tempus,
                    ((s32)l->crudum.datum[I]) & 0xFF, MOD_ALT);
            }
            redde ZEPHYRUM;   /* series dimidia abicitur */

        casus SERIES_ESC:
            in->alterum_pendens = FALSUM;
            si (   l->numerus_intermediorum > ZEPHYRUM
                || l->finale < 0x20 || l->finale > 0x7E)
            {
                redde ZEPHYRUM;
            }
            redde _runae_clavem(cauda, tempus, (s32)l->finale, MOD_ALT);

        casus SERIES_CSI:
        {
                CampiCsi c;
                     s32 p0;
            EventusActio actio;

            in->alterum_pendens = FALSUM;
            /* intermedia, privata praeter '<' (e.g. '?31u' responsum
             * vexillorum kitty, DA): ignota */
            si (   l->numerus_intermediorum > ZEPHYRUM
                || (l->privatum != ZEPHYRUM && l->privatum != '<'))
            {
                redde ZEPHYRUM;
            }
            p0 = (l->numerus_parametrorum >= I) ? l->parametra[0]
                                                : ZEPHYRUM;
            si (l->privatum == '<')
            {
                si (   (l->finale == 'M' || l->finale == 'm')
                    && l->numerus_parametrorum >= III)
                {
                    redde _murem(in, cauda, tempus, p0, l->parametra[I],
                        l->parametra[II], (b32)(l->finale == 'm'));
                }
                redde ZEPHYRUM;
            }
            si (   l->numerus_parametrorum == ZEPHYRUM
                && (l->finale == 'I' || l->finale == 'O'))
            {
                Eventus e;

                memset(&e, ZEPHYRUM, magnitudo(Eventus));
                e.genus   = (l->finale == 'I') ? EVENTUS_FOCUS
                                               : EVENTUS_DEFOCUS;
                e.tempus  = tempus;
                redde tessera_eventus_caudae_impellere(cauda,
                    &e) ? I : ZEPHYRUM;
            }
            _campos_legere(l, &c);
            modi   = _modificantes_csi(_campus(&c, I, ZEPHYRUM, I));
            actio  = _actio_kitty(_campus(&c, I, I, I));
            si (l->praefixum)
            {
                modi |= MOD_ALT;   /* ESC ESC [ A = alterum + sursum */
            }
            /* kitty: 'u', aut pars generis in forma legacy */
            si (l->finale == 'u' || c.numerus[I] >= II)
            {
                n = _kitty_discere(in, tempus, cauda);
            }
            si (l->finale == 'u')
            {
                redde n + _kitty_clavem(in, &c, modi, actio, tempus,
                    cauda);
            }
            si (   l->finale     == '~' && p0 == XXVII
                && c.numerus[II] >= I)
            {
                /* xterm modifyOtherKeys: CSI 27 ; m ; c ~ (codificator
                 * legacy Enter/Tab/Escape modificatos sic mittit,
                 * B6a) */
                s32 k = _campus(&c, II, ZEPHYRUM, ZEPHYRUM);

                commutatio (k)
                {
                    casus XIII:
                        redde n + _clavem(cauda, tempus, CLAVIS_REDITUS,
                            ZEPHYRUM, modi, EVENTUS_CODEX_IGNOTUS);
                    casus IX:
                        redde n + _clavem(cauda, tempus, CLAVIS_TABULA,
                            ZEPHYRUM, modi, EVENTUS_CODEX_IGNOTUS);
                    casus XXVII:
                        redde n + _clavem(cauda, tempus,
                            CLAVIS_EFFUGIUM,
                            ZEPHYRUM, modi, EVENTUS_CODEX_IGNOTUS);
                    casus CXXVII:
                        redde n + _clavem(cauda, tempus,
                            CLAVIS_RETRORSUM,
                            ZEPHYRUM, modi, EVENTUS_CODEX_IGNOTUS);
                    ordinarius:
                        redde n + ((k > ZEPHYRUM)
                            ? _runae_clavem(cauda, tempus, k, modi)
                            : ZEPHYRUM);
                }
            }
            si (l->finale == '~')
            {
                redde n + ((c.numerus[ZEPHYRUM] >= I)
                    ? _clavem_tildae(cauda, tempus, p0, modi, actio)
                    : ZEPHYRUM);
            }
            /* kitty F1 F2 F4: CSI P Q S (F3 = CSI 13~: CSI R = CPR) */
            si (   l->finale == 'P' || l->finale == 'Q'
                || l->finale == 'S')
            {
                redde n + _functionem(cauda, tempus,
                    (l->finale == 'S') ? IV : (s32)(l->finale - 'P')
                        + I,
                    modi, actio);
            }
            redde n + _finalem(cauda, tempus, (i32)l->finale, modi,
                actio);
        }

        casus SERIES_SS:
            in->alterum_pendens = FALSUM;
            si (l->introductor != 'O')
            {
                redde ZEPHYRUM;
            }
            modi = (l->numerus_parametrorum >= I)
                ? _modificantes_csi(l->parametra[0]) : ZEPHYRUM;
            si (l->praefixum)
            {
                modi |= MOD_ALT;
            }
            si (l->finale >= 'P' && l->finale <= 'S')
            {
                redde _functionem(cauda, tempus,
                    (s32)(l->finale - 'P') + I, modi,
                    EVENTUS_ACTIO_PRESSA);
            }
            redde _finalem(cauda, tempus, (i32)l->finale, modi,
                EVENTUS_ACTIO_PRESSA);

        ordinarius:
            /* OSC, DCS, APC (responsa), NIHIL */
            redde ZEPHYRUM;
    }
}

static i32
tessera_interpres_x10 (
    InterpresTerminalis* interpres,
                    i32  cb,
                    i32  cx,
                    i32  cy,
                    s64  tempus,
           EventusCauda* cauda)
{
    s32 b = (s32)cb - XXXII;

    si (b < ZEPHYRUM || cx < XXXIII || cy < XXXIII)
    {
        redde ZEPHYRUM;   /* onus malum */
    }
    redde _murem(interpres, cauda, tempus, b, (s32)cx - XXXII,
        (s32)cy - XXXII, FALSUM);
}

static i32
tessera_interpres_glutinum (
    InterpresTerminalis* interpres,
            constans i8* octeti,
                    i32  mensura,
                    s64  tempus,
           EventusCauda* cauda)
{
    (vacuum)interpres;
    redde tessera_eventus_caudae_textum_impellere(cauda, tempus, octeti,
        mensura,
        EVENTUS_ORIGO_GLUTINATA) ? I : ZEPHYRUM;
}

static i32
tessera_interpres_depositio (
    InterpresTerminalis* interpres,
            constans i8* viae,
                    i32  mensura,
                    i32  numerus,
                    s64  tempus,
           EventusCauda* cauda)
{
    Eventus e;
    /* visus vocantis (constans): cauda copiat, non scribit */
    unio { constans i8* l; i8* m; } u;

    u.l = viae;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                         = EVENTUS_DEPOSITIO;
    e.tempus                        = tempus;
    e.datum.depositio.x             = interpres->indicator_x;
    e.datum.depositio.y             = interpres->indicator_y;
    e.datum.depositio.viae.datum    = u.m;
    e.datum.depositio.viae.mensura  = mensura;
    e.datum.depositio.numerus       = numerus;
    e.datum.depositio.promota       = VERUM;
    redde tessera_eventus_caudae_depositionem_impellere(cauda, &e) ? I
                                                           : ZEPHYRUM;
}

/* ================= ex lib/rivus_terminalis.c ================= */

#define RELIQUIAE_NULLAE ZEPHYRUM
#define RELIQUIAE_FUGA   I      /* ESC solus, iam Effugium redditus */
#define RELIQUIAE_SGR    II     /* CSI < dimidia */

/* Cursus imprimibilis per passum: runa quaeque CLAVIS + TEXTUS, ergo
 * passus unus <= II * LXIV eventa - cauda (CCLVI) numquam superfluit */
#define CURSUS_MAXIMUS LXIV

#define CODEX_INITII_GLUTINI CC                 /* CSI 200 ~ */
#define TERMINUS_GLUTINI     "\033[201~"
#define TERMINI_LONGITUDO    ((i32)(magnitudo(TERMINUS_GLUTINI) - I))

structura TesseraRivusTerminalis {
                   i8  buffer[RIVUS_BUFFER];
                  i32  mensura;
         TesseraSeriesLector* series;
  InterpresTerminalis  interpres;
         EventusCauda* cauda;

    /* canales crudi */
             b32 x10_pendens;       /* post CSI M: tres octeti */
             b32 alienum_pendens;   /* post CSI [: usque ad finalem */

    /* reliquiae post moram (H8) */
              i8 reliquiae[SERIES_CRUDUM_MAXIMUM];
             i32 reliquiae_mensura;
             i32 reliquiae_genus;

    /* glutinum (?2004): corpus inter CSI 200~ et CSI 201~ */
             b32  glutinum_pendens;
              i8* glutinum;          /* RIVUS_GLUTINUM_CAPACITAS */
             i32  glutinum_mensura;
             i32  congruentes;       /* octeti termini congruentes */
             b32  glutinum_truncatum;
              i8* viae;              /* glutinum promotum (B3b) */

    /* modi declarati (RIVUS_MODUS_*), ZEPHYRUM = nulli */
             i32 modi_intrati;
};

#define MODI_OMNES (RIVUS_MODUS_MUS | RIVUS_MODUS_SUPER \
    | RIVUS_MODUS_GLUTINUM | RIVUS_MODUS_FOCUS | RIVUS_MODUS_KITTY \
    | RIVUS_MODUS_DEPOSITIO)

/* vexilla impulsa (31): discernere, genera, alternae, omnes, textus */
#define KITTY_IMPULSA (INTERPRES_KITTY_DISCERNERE \
    | INTERPRES_KITTY_GENERA | INTERPRES_KITTY_ALTERNAE \
    | INTERPRES_KITTY_OMNES | INTERPRES_KITTY_TEXTUS)


/* ==================================================
 * Auxilia
 * ================================================== */

interior vacuum
_consumere (
    TesseraRivusTerminalis* r,
                i32  numerus)
{
    si (numerus >= r->mensura)
    {
        r->mensura = ZEPHYRUM;
        redde;
    }
    memmove(r->buffer, r->buffer + numerus,
        (memoriae_index)(r->mensura - numerus));
    r->mensura -= numerus;
}

/* FACULTATES interpretis in caudam (primum fluxus; modi mutati) */
interior vacuum
_facultates_impellere (
    TesseraRivusTerminalis* r)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_FACULTATES;
    e.datum.facultates  = r->interpres.facultates;
    (vacuum)tessera_eventus_caudae_impellere(r->cauda, &e);
}

interior b32
_rivi_fuga_sola (
    constans SeriesLexema* l)
{
    i32 k;

    si (l->crudum.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < l->crudum.mensura; k++)
    {
        si (l->crudum.datum[k] != (i8)0x1B)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Octetos lexematori tradere (status eius restituitur; reliquiae
 * praefixum seriei pendentis sunt - nullum lexema completur) */
interior vacuum
_octetos_tradere (
    TesseraRivusTerminalis* r,
        constans i8* octeti,
                i32  mensura)
{
     constans i8* p = octeti;
    SeriesLexema  l;

    dum (   p < octeti + mensura
         && tessera_series_lexema_proximum(r->series, &p, octeti + mensura, &l)
                != SERIES_NIHIL)
    {
    }
}

/* Reliquiae continuationi redduntur si initus novus eam continuare
 * videtur; alioquin abiciuntur. FALSUM = nondum decernitur ('[' solum
 * post ESC: octetus proximus dicet). */
interior b32
_reliquias_reddere (
    TesseraRivusTerminalis* r)
{
    b32 congruit  = FALSUM;
     i8 b         = r->buffer[ZEPHYRUM];

    si (r->reliquiae_genus == RELIQUIAE_FUGA)
    {
        si (r->mensura == I && b == '[')
        {
            redde FALSUM;
        }
        congruit = r->mensura >= II && b == '['
            && (r->buffer[I] == '<' || r->buffer[I] == 'M');
    }
    alioquin si (r->reliquiae_genus == RELIQUIAE_SGR)
    {
        congruit = (b >= '0' && b <= '9') || b == ';' || b == 'M'
            || b == 'm';
    }
    si (congruit)
    {
        _octetos_tradere(r, r->reliquiae, r->reliquiae_mensura);
    }
    r->reliquiae_genus    = RELIQUIAE_NULLAE;
    r->reliquiae_mensura  = ZEPHYRUM;
    redde VERUM;
}

interior vacuum
_glutino_addere (
    TesseraRivusTerminalis* r,
                 i8  octetus)
{
    si (r->glutinum_mensura < RIVUS_GLUTINUM_CAPACITAS)
    {
        r->glutinum[r->glutinum_mensura] = octetus;
        r->glutinum_mensura++;
    }
    alioquin
    {
        r->glutinum_truncatum = VERUM;
    }
}

/* Spatium album inter vias (nec citatum nec effugitum) */
interior b32
_album (
    i8 c)
{
    redde (b32)(c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

interior s32
_hexadecimalis (
    i8 c)
{
    si (c >= '0' && c <= '9')
    {
        redde (s32)(c - '0');
    }
    si (c >= 'a' && c <= 'f')
    {
        redde (s32)(c - 'a') + X;
    }
    si (c >= 'A' && c <= 'F')
    {
        redde (s32)(c - 'A') + X;
    }
    redde -I;
}

/* Via una in exitus[initium..*finis): 'file://' (et 'localhost')
 * demitur, %XX decodificatur; absoluta esse debet, sine '\n' et NUL.
 * FALSUM = non via. */
interior b32
_viam_probare (
     i8* exitus,
    i32  initium,
    i32* finis)
{
     i8* v = exitus + initium;
    i32  n = *finis - initium;
    i32  k;

    si (n >= VII && memcmp(v, "file://", VII) == ZEPHYRUM)
    {
        i32 demenda  = VII;
        i32 j        = ZEPHYRUM;

        si (   n - VII                          >= IX
            && memcmp(v + VII, "localhost", IX) == ZEPHYRUM)
        {
            demenda += IX;
        }
        memmove(v, v + demenda, (memoriae_index)(n - demenda));
        n -= demenda;
        per (k = ZEPHYRUM; k < n; k++)
        {
            s32 alta = (k + II < n) ? _hexadecimalis(v[k + I]) : -I;
            s32 humilis = (k + II < n) ? _hexadecimalis(v[k + II]) : -I;

            si (v[k] == '%' && alta >= ZEPHYRUM && humilis >= ZEPHYRUM)
            {
                v[j]  = (i8)(alta * XVI + humilis);
                k     += II;
            }
            alioquin
            {
                v[j] = v[k];
            }
            j++;
        }
        n = j;
    }
    si (n == ZEPHYRUM || v[ZEPHYRUM] != '/')
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (v[k] == '\n' || v[k] == '\0')
        {
            redde FALSUM;
        }
    }
    *finis = initium + n;
    redde VERUM;
}

/* Glutinum -> viae absolutae '\n' iunctae (more conchae: '\x',
 * '...', "..." cum \" \\ \$ \`; file:// URI). Exitus <= textus
 * (effugia et separatores numquam crescunt). Redde numerum viarum; 0 =
 * non viae (textus manet). */
interior i32
_vias_legere (
    constans i8* t,
            i32  n,
             i8* exitus,
            i32* mensura)
{
    i32 i        = ZEPHYRUM;
    i32 o        = ZEPHYRUM;
    i32 numerus  = ZEPHYRUM;

    per (;;)
    {
        i32 initium;
         i8 citatio = ZEPHYRUM;

        dum (i < n && _album(t[i]))
        {
            i++;
        }
        si (i >= n)
        {
            frange;
        }
        si (numerus > ZEPHYRUM)
        {
            exitus[o] = '\n';
            o++;
        }
        initium = o;
        dum (i < n)
        {
            i8 c = t[i];

            si (citatio == '\'')
            {
                si (c == '\'')
                {
                    citatio = ZEPHYRUM;
                }
                alioquin
                {
                    exitus[o] = c;
                    o++;
                }
                i++;
                perge;
            }
            si (citatio == '"')
            {
                si (c == '"')
                {
                    citatio = ZEPHYRUM;
                    i++;
                    perge;
                }
                si (   c == '\\' && i + I < n
                    && (   t[i + I] == '"' || t[i + I] == '\\'
                        || t[i + I] == '$' || t[i + I] == '`'))
                {
                    i++;
                }
                exitus[o] = t[i];
                o++;
                i++;
                perge;
            }
            si (_album(c))
            {
                frange;
            }
            si (c == '\\')
            {
                si (i + I >= n)
                {
                    redde ZEPHYRUM;
                }
                exitus[o] = t[i + I];
                o++;
                i += II;
                perge;
            }
            si (c == '\'' || c == '"')
            {
                citatio = c;
                i++;
                perge;
            }
            exitus[o] = c;
            o++;
            i++;
        }
        si (citatio != ZEPHYRUM || !_viam_probare(exitus, initium, &o))
        {
            redde ZEPHYRUM;
        }
        numerus++;
    }
    *mensura = o;
    redde numerus;
}

/* Glutinum finitum: TEXT GLUTINATA (copiatum in caudam), truncatum
 * notatur in eventu ipso */
interior vacuum
_glutinum_finire (
    TesseraRivusTerminalis* r,
                s64  tempus)
{
    i32 viae_mensura  = ZEPHYRUM;
    i32 numerus       = ZEPHYRUM;

    /* promotio declarata: glutinum integrum viarum -> DEPOSITIO; si
     * non viae aut cauda sine loco, textus manet */
    si (   (r->modi_intrati & RIVUS_MODUS_DEPOSITIO)
        && !r->glutinum_truncatum && r->glutinum_mensura > ZEPHYRUM)
    {
        numerus = _vias_legere(r->glutinum, r->glutinum_mensura,
            r->viae,
            &viae_mensura);
    }
    si (   numerus > ZEPHYRUM
        && tessera_interpres_depositio(&r->interpres, r->viae, viae_mensura,
               numerus, tempus, r->cauda) > ZEPHYRUM)
    {
        /* promotum */
    }
    alioquin si (r->glutinum_mensura == ZEPHYRUM)
    {
        /* glutinum VACUUM est eventus (cauda textum vacuum recusat) */
        Eventus e;

        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus                   = EVENTUS_TEXTUS;
        e.tempus                  = tempus;
        e.datum.textus.origo      = EVENTUS_ORIGO_GLUTINATA;
        e.datum.textus.truncatum  = r->glutinum_truncatum;
        (vacuum)tessera_eventus_caudae_impellere(r->cauda, &e);
    }
    alioquin si (   tessera_interpres_glutinum(&r->interpres, r->glutinum,
                 r->glutinum_mensura, tempus, r->cauda) > ZEPHYRUM
                 && r->glutinum_truncatum)
    {
        r->cauda->eventus[(r->cauda->finis + EVENTUS_CAUDA_CAPACITAS
            - I)
            % EVENTUS_CAUDA_CAPACITAS].datum.textus.truncatum = VERUM;
    }
    r->glutinum_pendens    = FALSUM;
    r->glutinum_mensura    = ZEPHYRUM;
    r->congruentes         = ZEPHYRUM;
    r->glutinum_truncatum  = FALSUM;
}

/* Corpus glutini ex buffere: VERUM si terminus inventus. Discordia:
 * praefixum congruens corpus est, octetus ut ESC novum iterum temptatur
 * (terminus ESC solum in capite habet). */
interior b32
_glutinum_colligere (
    TesseraRivusTerminalis* r,
               s64  tempus)
{
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < r->mensura; k++)
    {
        i8 b = r->buffer[k];

        si (b == (i8)TERMINUS_GLUTINI[r->congruentes])
        {
            r->congruentes++;
            si (r->congruentes == TERMINI_LONGITUDO)
            {
                _consumere(r, k + I);
                _glutinum_finire(r, tempus);
                redde VERUM;
            }
            perge;
        }
        per (j = ZEPHYRUM; j < r->congruentes; j++)
        {
            _glutino_addere(r, (i8)TERMINUS_GLUTINI[j]);
        }
        si (b == (i8)0x1B)
        {
            r->congruentes = I;
        }
        alioquin
        {
            r->congruentes = ZEPHYRUM;
            _glutino_addere(r, b);
        }
    }
    r->mensura = ZEPHYRUM;
    redde FALSUM;
}

/* Cursus imprimibilis: runae INTEGRAE interpreti traduntur; runa
 * dimidia
 * in fine bufferis manet (lexemator in solo - reditus innocuus). Redde
 * octetos consumendos. */
interior i32
_cursum_tradere (
          TesseraRivusTerminalis* r,
    constans SeriesLexema* l,
                      s64  tempus)
{
      constans i8* initium  = l->textus.datum;
      constans i8* finis    = initium + l->textus.mensura;
      constans i8* p        = initium;
     SeriesLexema  integrum;

    /* runa dimidia SOLUM si cursus ad finem bufferis pertinet */
    si (finis == r->buffer + r->mensura)
    {
         constans i8* q = finis;
                 i32  k;

        per (k = ZEPHYRUM; k < III && q > initium; k++)
        {
            q--;
            si (!tessera_utf8_est_continuatio(*q))
            {
                s32 longitudo = tessera_utf8_longitudo_byte(*q);

                si (   longitudo > ZEPHYRUM
                    && (s32)(finis - q) < longitudo)
                {
                    finis = q;   /* runa dimidia: exspectatur */
                }
                frange;
            }
        }
    }
    /* passus finitus (cauda): sectio ad initium runae */
    si (finis - initium > CURSUS_MAXIMUS)
    {
        finis = initium + CURSUS_MAXIMUS;
        dum (finis > initium && tessera_utf8_est_continuatio(*finis))
        {
            finis--;
        }
    }
    p = finis;
    si (p > initium)
    {
        integrum                 = *l;
        integrum.textus.mensura  = (i32)(p - initium);
        (vacuum)tessera_interpres_lexema(&r->interpres, &integrum, FALSUM,
            tempus,
            r->cauda);
    }
    redde (i32)(p - r->buffer);
}

/* Passus unus: lexema aut canalis crudus. VERUM si progressus. */
interior b32
_passus (
    TesseraRivusTerminalis* r,
                s64  tempus)
{
      constans i8* ptr;
     SeriesLexema  l;
      SeriesGenus  g;
              i32  consumpti;

    si (   r->mensura > ZEPHYRUM
        && r->reliquiae_genus != RELIQUIAE_NULLAE
        && !_reliquias_reddere(r))
    {
        redde FALSUM;   /* '[' post ESC: exspecta */
    }
    si (r->glutinum_pendens)
    {
        redde _glutinum_colligere(r, tempus);
    }
    si (r->x10_pendens)
    {
        si (r->mensura < III)
        {
            redde FALSUM;
        }
        r->x10_pendens = FALSUM;
        (vacuum)tessera_interpres_x10(&r->interpres,
            (i32)r->buffer[ZEPHYRUM], (i32)r->buffer[I],
            (i32)r->buffer[II], tempus, r->cauda);
        _consumere(r, III);
        redde VERUM;
    }
    si (r->alienum_pendens)
    {
        i32 k;

        per (k = ZEPHYRUM; k < r->mensura; k++)
        {
            si (r->buffer[k] >= 0x40 && r->buffer[k] <= 0x7E)
            {
                _consumere(r, k + I);
                r->alienum_pendens = FALSUM;
                redde VERUM;
            }
        }
        r->mensura = ZEPHYRUM;
        redde FALSUM;
    }
    si (r->mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    ptr        = r->buffer;
    g          = tessera_series_lexema_proximum(r->series, &ptr,
        r->buffer + r->mensura, &l);
    consumpti  = (i32)(ptr - r->buffer);

    si (g == SERIES_NIHIL)
    {
        _consumere(r, consumpti);
        redde (b32)(consumpti > ZEPHYRUM);
    }
    si (g == SERIES_IMPRIMERE)
    {
        i32 n = _cursum_tradere(r, &l, tempus);

        _consumere(r, n);
        redde (b32)(n > ZEPHYRUM);
    }
    _consumere(r, consumpti);
    si (   g == SERIES_CSI && l.privatum == ZEPHYRUM
        && l.numerus_intermediorum == ZEPHYRUM
        && l.separatores == ZEPHYRUM)
    {
        /* canales crudi: X10 (CSI M), forma aliena (CSI [), glutinum */
        si (l.numerus_parametrorum == ZEPHYRUM && l.finale == 'M')
        {
            r->x10_pendens = VERUM;
            redde VERUM;
        }
        si (l.numerus_parametrorum == ZEPHYRUM && l.finale == '[')
        {
            r->alienum_pendens = VERUM;
            redde VERUM;
        }
        si (   l.numerus_parametrorum == I && l.finale == '~'
            && l.parametra[ZEPHYRUM]  == CODEX_INITII_GLUTINI)
        {
            r->glutinum_pendens    = VERUM;
            r->glutinum_mensura    = ZEPHYRUM;
            r->congruentes         = ZEPHYRUM;
            r->glutinum_truncatum  = FALSUM;
            redde VERUM;
        }
    }
    (vacuum)tessera_interpres_lexema(&r->interpres, &l, FALSUM, tempus,
        r->cauda);
    redde VERUM;
}


/* ==================================================
 * Publica
 * ================================================== */

static TesseraRivusTerminalis*
tessera_rivus_creare (
    TesseraPiscina* piscina,
        s32  cellula_latitudo,
        s32  cellula_altitudo)
{
    TesseraRivusTerminalis* r;

    r = (TesseraRivusTerminalis*)tessera_piscina_allocare_ordinatum(piscina,
        magnitudo(TesseraRivusTerminalis), VIII);
    si (r == NIHIL)
    {
        redde NIHIL;
    }
    memset(r, ZEPHYRUM, magnitudo(TesseraRivusTerminalis));
    r->series   = tessera_series_lectorem_creare(piscina);
    r->cauda    = (EventusCauda*)tessera_piscina_allocare_ordinatum(piscina,
        magnitudo(EventusCauda), VIII);
    r->glutinum = (i8*)tessera_piscina_allocare(piscina,
        (memoriae_index)RIVUS_GLUTINUM_CAPACITAS);
    r->viae     = (i8*)tessera_piscina_allocare(piscina,
        (memoriae_index)RIVUS_GLUTINUM_CAPACITAS);
    si (   r->series   == NIHIL || r->cauda == NIHIL
        || r->glutinum == NIHIL
        || r->viae     == NIHIL)
    {
        redde NIHIL;
    }
    tessera_series_lectorem_initus_ponere(r->series, VERUM);
    tessera_interpres_initiare(&r->interpres, cellula_latitudo,
        cellula_altitudo);
    tessera_eventus_caudam_initiare(r->cauda);
    _facultates_impellere(r);     /* primum fluxus (spec Q4) */
    redde r;
}

static i32
tessera_rivus_spatium (
    constans TesseraRivusTerminalis* r)
{
    redde RIVUS_BUFFER - r->mensura;
}

static i32
tessera_rivus_tradere (
    TesseraRivusTerminalis* r,
        constans i8* octeti,
                i32  mensura)
{
    i32 n = (mensura < RIVUS_BUFFER - r->mensura)
        ? mensura : RIVUS_BUFFER - r->mensura;

    memcpy(r->buffer + r->mensura, octeti, (memoriae_index)n);
    r->mensura += n;
    redde n;
}

static b32
tessera_rivus_eventum (
    TesseraRivusTerminalis* r,
                s64  tempus,
            Eventus* eventus)
{
    si (tessera_eventus_caudae_extrahere(r->cauda, eventus))
    {
        redde VERUM;
    }
    /* cauda vacua: onera (textus, exempla) vacantur, deinde pigre
     * decoditur - passus unus quoad eventum adest */
    tessera_eventus_cauda_lectio_incipit(r->cauda);
    dum (_passus(r, tempus))
    {
        si (tessera_eventus_caudae_extrahere(r->cauda, eventus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

static s32
tessera_rivus_mora_ms (
    constans TesseraRivusTerminalis* r)
{
    si (r->glutinum_pendens)
    {
        redde RIVUS_MORA_GLUTINI_MS;
    }
    si (   tessera_series_lector_pendet(r->series) || r->x10_pendens
        || r->alienum_pendens || r->interpres.alterum_pendens
        || r->mensura > ZEPHYRUM)
    {
        redde RIVUS_MORA_FUGAE_MS;
    }
    redde ZEPHYRUM;
}

static vacuum
tessera_rivus_moram (
    TesseraRivusTerminalis* r,
                s64  tempus)
{
    SeriesLexema l;

    si (r->glutinum_pendens)
    {
        /* silentium sine termino: praefixum pendens corpus fit */
        i32 j;

        per (j = ZEPHYRUM; j < r->congruentes; j++)
        {
            _glutino_addere(r, (i8)TERMINUS_GLUTINI[j]);
        }
        r->glutinum_truncatum = VERUM;
        _glutinum_finire(r, tempus);
        redde;
    }
    si (r->x10_pendens)
    {
        r->x10_pendens  = FALSUM;   /* X10 dimidium abicitur (H7) */
        r->mensura      = ZEPHYRUM;
        redde;
    }
    si (r->alienum_pendens)
    {
        r->alienum_pendens = FALSUM;
        redde;
    }
    si (tessera_series_lector_pendet(r->series))
    {
        si (!tessera_series_lectorem_evacuare(r->series, &l))
        {
            redde;   /* solum terminator chordae */
        }
        si (   l.crudum.mensura   >= III && l.crudum.datum[I] == '['
            && l.crudum.datum[II] == '<')
        {
            /* mus SGR dimidia: servatur pro continuatione (H8) */
            memcpy(r->reliquiae, l.crudum.datum,
                (memoriae_index)l.crudum.mensura);
            r->reliquiae_mensura  = l.crudum.mensura;
            r->reliquiae_genus    = RELIQUIAE_SGR;
            redde;
        }
        (vacuum)tessera_interpres_lexema(&r->interpres, &l, VERUM, tempus,
            r->cauda);
        si (_rivi_fuga_sola(&l) && l.crudum.mensura == I)
        {
            /* mus forte sequetur ('[<..', '[M..'): H8 */
            r->reliquiae[ZEPHYRUM]  = (i8)0x1B;
            r->reliquiae_mensura    = I;
            r->reliquiae_genus      = RELIQUIAE_FUGA;
        }
        redde;
    }
    si (r->reliquiae_genus != RELIQUIAE_NULLAE && r->mensura > ZEPHYRUM)
    {
        /* '[' post ESC sine continuatione: reliquiae abiciuntur, '['
         * ut runa legitur */
        r->reliquiae_genus    = RELIQUIAE_NULLAE;
        r->reliquiae_mensura  = ZEPHYRUM;
        redde;
    }
    si (r->interpres.alterum_pendens)
    {
        /* 'ESC' + runa dimidia: Effugium (FUGA post moram ficta - eadem
         * via ac ESC solus); runa dimidia mox abicitur */
        i8 fuga[I];

        fuga[ZEPHYRUM] = (i8)0x1B;
        memset(&l, ZEPHYRUM, magnitudo(SeriesLexema));
        l.genus           = SERIES_FUGA;
        l.crudum.datum    = fuga;
        l.crudum.mensura  = I;
        (vacuum)tessera_interpres_lexema(&r->interpres, &l, VERUM, tempus,
            r->cauda);
        redde;
    }
    si (r->mensura > ZEPHYRUM)
    {
        _consumere(r, I);   /* runa dimidia (aut invalida) abicitur */
    }
}

static i32
tessera_rivus_pendentes (
    constans TesseraRivusTerminalis* r)
{
    redde r->mensura;
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

/* Effugia intrandi/exeundi (INTRANDI, EXEUNDI): tessera_modi.h -
 * sedes una, lex parium probata (probatio_tessera_modi.c) */


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

interior vacuum
_eventum_vacare (
    TesseraEventum* ev)
{
    ev->genus               = TESSERA_EVENTUM_NIHIL;
    ev->runa                = ZEPHYRUM;
    ev->clavis              = TESSERA_CLAVIS_NULLA;
    ev->modificatores       = ZEPHYRUM;
    ev->numerus             = ZEPHYRUM;
    ev->mus_genus           = TESSERA_MUS_PRESSUS;
    ev->mus_x               = ZEPHYRUM;
    ev->mus_y               = ZEPHYRUM;
    ev->mus_pulsus          = ZEPHYRUM;
    ev->latitudo            = ZEPHYRUM;
    ev->altitudo            = ZEPHYRUM;
    ev->glutinum.mensura    = ZEPHYRUM;
    ev->glutinum.datum      = NIHIL;
    ev->glutinum_truncatum  = FALSUM;
}

interior i32
_modificatores (
    i32 m)
{
    i32 fructus = ZEPHYRUM;

    si (m & MOD_SHIFT)
    {
        fructus |= TESSERA_MODIFICATOR_MAIUSCULA;
    }
    si (m & MOD_ALT)
    {
        fructus |= TESSERA_MODIFICATOR_ALTERUM;
    }
    si (m & MOD_IMPERIUM)
    {
        fructus |= TESSERA_MODIFICATOR_IMPERIUM;
    }
    redde fructus;
}

interior b32
_clavem_ponere (
    TesseraEventum* ev,
     TesseraClavis  clavis,
               i32  modificatores)
{
    ev->genus          = TESSERA_EVENTUM_CLAVIS;
    ev->clavis         = clavis;
    ev->modificatores  = modificatores;
    redde VERUM;
}

/* Clavis: nominatae ad TesseraClavis, ceterae runa (character verus) */
interior b32
_clavem_proicere (
    constans Eventus* e,
      TesseraEventum* ev)
{
    i32 modi  = _modificatores(e->datum.clavis.modificantes);
    s32 c     = (s32)e->datum.clavis.clavis;
    s32 typus;

    commutatio (e->datum.clavis.clavis)
    {
        casus CLAVIS_REDITUS:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_REDITUS, modi);
        casus CLAVIS_TABULA:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_TABULA, modi);
        casus CLAVIS_RETRORSUM:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_RETRORSUM, modi);
        casus CLAVIS_EFFUGIUM:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_FUGA, modi);
        casus CLAVIS_SURSUM:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_SURSUM, modi);
        casus CLAVIS_DEORSUM:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_DEORSUM, modi);
        casus CLAVIS_DEXTER:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_DEXTRA, modi);
        casus CLAVIS_SINISTER:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_SINISTRA, modi);
        casus CLAVIS_DOMUS:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_DOMUS, modi);
        casus CLAVIS_FINIS:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_FINIS, modi);
        casus CLAVIS_PAGINA_SURSUM:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_PAGINA_SURSUM,
                modi);
        casus CLAVIS_PAGINA_DEORSUM:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_PAGINA_DEORSUM,
                modi);
        casus CLAVIS_DELERE:
            redde _clavem_ponere(ev, TESSERA_CLAVIS_DELETIO, modi);
        ordinarius:
            frange;
    }
    si (c >= (s32)CLAVIS_F1 && c <= (s32)CLAVIS_F12)
    {
        _clavem_ponere(ev, TESSERA_CLAVIS_FUNCTIO, modi);
        ev->numerus = (i32)(c - (s32)CLAVIS_F1) + I;
        redde VERUM;
    }
    si (e->datum.clavis.codex == EVENTUS_CODEX_INSERERE)
    {
        redde _clavem_ponere(ev, TESSERA_CLAVIS_INSERTIO, modi);
    }
    /* coniunctiones deperditae: Ctrl+J = '\n' -> reditus, Ctrl+H =
     * 0x08 -> retrorsum */
    si (modi & TESSERA_MODIFICATOR_IMPERIUM)
    {
        si (c == 'J')
        {
            redde _clavem_ponere(ev, TESSERA_CLAVIS_REDITUS,
                modi & ~(i32)TESSERA_MODIFICATOR_IMPERIUM);
        }
        si (c == 'H')
        {
            redde _clavem_ponere(ev, TESSERA_CLAVIS_RETRORSUM,
                modi & ~(i32)TESSERA_MODIFICATOR_IMPERIUM);
        }
    }
    si (e->datum.clavis.runa == ZEPHYRUM)
    {
        redde FALSUM;   /* clavis sine nomine tesserae (F13, ...) */
    }
    /* runa: character verus (typus) - sub imperio runa (minuscula,
     * 'ctrl+a' ut olim) */
    typus              = ((s32)e->datum.clavis.typus) & 0xFF;
    ev->genus          = TESSERA_EVENTUM_CLAVIS;
    ev->modificatores  = modi;
    ev->runa = (   !(modi & TESSERA_MODIFICATOR_IMPERIUM)
                && typus >= 0x20 && typus < 0x7F)
        ? typus : e->datum.clavis.runa;
    redde VERUM;
}

interior i32
_pulsus (
    mus_botton_t b)
{
    commutatio (b)
    {
        casus MUS_SINISTER: redde ZEPHYRUM;
        casus MUS_MEDIUS:   redde I;
        casus MUS_DEXTER:   redde II;
        ordinarius:         redde III;   /* ignotus (X10 solutio) */
    }
}

interior b32
_proicere (
    constans Eventus* e,
      TesseraEventum* ev)
{
    commutatio (e->genus)
    {
        casus EVENTUS_CLAVIS_DEPRESSUS:
            redde _clavem_proicere(e, ev);

        casus EVENTUS_TEXTUS:
            si (e->datum.textus.origo != EVENTUS_ORIGO_GLUTINATA)
            {
                redde FALSUM;   /* textus scriptus: clavis eum fert */
            }
            ev->genus               = TESSERA_EVENTUM_GLUTINUM;
            ev->glutinum            = e->datum.textus.contentum;
            ev->glutinum_truncatum  = e->datum.textus.truncatum;
            redde VERUM;

        casus EVENTUS_MUS_DEPRESSUS:
        casus EVENTUS_MUS_LIBERATUS:
        casus EVENTUS_MUS_MOTUS:
            si (   e->genus            == EVENTUS_MUS_MOTUS
                && e->datum.mus.botton == (mus_botton_t)ZEPHYRUM)
            {
                redde FALSUM;   /* motus sine bottone (?1003 non petitus) */
            }
            ev->genus          = TESSERA_EVENTUM_MUS;
            ev->mus_genus      = (e->genus == EVENTUS_MUS_DEPRESSUS)
                                     ? TESSERA_MUS_PRESSUS
                               : (e->genus == EVENTUS_MUS_LIBERATUS)
                                     ? TESSERA_MUS_SOLUTUS
                                     : TESSERA_MUS_TRACTUS;
            ev->mus_x          = e->datum.mus.x;
            ev->mus_y          = e->datum.mus.y;
            ev->mus_pulsus     = _pulsus(e->datum.mus.botton);
            ev->modificatores  =
                _modificatores(e->datum.mus.modificantes);
            redde VERUM;

        casus EVENTUS_MUS_ROTULA:
            ev->genus          = TESSERA_EVENTUM_MUS;
            ev->mus_genus      = (e->datum.rotula.dy > ZEPHYRUM)
                                     ? TESSERA_MUS_ROTA_SURSUM
                               : (e->datum.rotula.dy < ZEPHYRUM)
                                     ? TESSERA_MUS_ROTA_DEORSUM
                               : (e->datum.rotula.dx > ZEPHYRUM)
                                     ? TESSERA_MUS_ROTA_SINISTRORSUM
                                     : TESSERA_MUS_ROTA_DEXTRORSUM;
            ev->mus_x          = e->datum.rotula.x;
            ev->mus_y          = e->datum.rotula.y;
            ev->mus_pulsus     = ZEPHYRUM;
            ev->modificatores  = _modificatores(
                e->datum.rotula.modificantes);
            redde VERUM;

        ordinarius:
            redde FALSUM;   /* solutiones, focus, facultates */
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
        (memoriae_index)magnitudo(TesseraLector), VIII);
    si (lector == NIHIL)
    {
        redde NIHIL;
    }
    /* cellula I x I: pixela rivi = cellulae tesserae */
    lector->rivus = tessera_rivus_creare(piscina, I, I);
    si (lector->rivus == NIHIL)
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

    /* Eventa rivi proiciuntur; deficientibus legitur - mora RIVI si
     * aliquid pendet (series dimidia, runa, glutinum: H6, ESC pendens
     * moram fugae SOLAM habet), alioquin mora vocantis. Silentium post
     * moram rivi -> rivus_moram (ESC = fuga, reliquiae ...). */
    per (;;)
    {
        Eventus e;
            s32 mora;
            s32 n;
            i32 capax;

        dum (tessera_rivus_eventum(lector->rivus, ZEPHYRUM, &e))
        {
            si (_proicere(&e, eventum))
            {
                lector->mensura = tessera_rivus_pendentes(lector->rivus);
                redde eventum->genus;
            }
        }
        mora   = tessera_rivus_mora_ms(lector->rivus);
        capax  = tessera_rivus_spatium(lector->rivus);
        si (capax > (i32)TESSERA_LECTOR_BUFFER)
        {
            capax = (i32)TESSERA_LECTOR_BUFFER;
        }
        n = (capax > ZEPHYRUM)
            ? lector->pons->legere(lector->pons->datum, lector->buffer,
                  capax, (mora > ZEPHYRUM) ? mora : mora_ms)
            : ZEPHYRUM;
        si (n > ZEPHYRUM)
        {
            (vacuum)tessera_rivus_tradere(lector->rivus, lector->buffer,
                (i32)n);
            perge;
        }
        si (mora > ZEPHYRUM || capax == ZEPHYRUM)
        {
            tessera_rivus_moram(lector->rivus, ZEPHYRUM);
            perge;
        }
        lector->mensura = tessera_rivus_pendentes(lector->rivus);
        redde TESSERA_EVENTUM_NIHIL;
    }
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

/* Index xterm CCLVI proximus (XVI-CCLV) per distantiam RGB quadratam:
 * cubus (gradus 0/95/135/175/215/255 per canalem; in aequalitate
 * gradus inferior) aut griseus (8 + 10k, k 0-23; proximus mediae RGB,
 * quia distantia ad griseum in gradu convexa est). Aequalitas:
 * cubus. */
interior i32
_cclvi (
    i32 color)
{
    hic_manens constans i32 GRADUS[VI] = { 0, 95, 135, 175, 215, 255 };
                        i32 index[III];
                        i32 d_cubi    = ZEPHYRUM;
                        i32 d_grisei  = ZEPHYRUM;
                        i32 summa     = ZEPHYRUM;
                        i32 griseus;
                        i32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        s32 c = (s32)((color >> (XVI - VIII * k)) & 0xFF);
        s32 d;
        i32 j;

        index[k] = ZEPHYRUM;
        per (j = I; j < VI; j++)
        {
            s32 prior  = c - (s32)GRADUS[index[k]];
            s32 hic    = c - (s32)GRADUS[j];

            si (hic * hic < prior * prior)
            {
                index[k] = j;
            }
        }
        d       = c - (s32)GRADUS[index[k]];
        d_cubi  += (i32)(d * d);
        summa   += (i32)c;
    }
    griseus = (summa / III < VIII) ? ZEPHYRUM
                                   : (summa / III - VIII + V) / X;
    si (griseus > XXIII)
    {
        griseus = XXIII;
    }
    per (k = ZEPHYRUM; k < III; k++)
    {
        s32 d = (s32)((color >> (XVI - VIII * k)) & 0xFF)
            - (s32)(VIII + X * griseus);

        d_grisei += (i32)(d * d);
    }
    si (d_grisei < d_cubi)
    {
        redde CCXXXII + griseus;
    }
    redde XVI + XXXVI * index[0] + VI * index[1] + index[2];
}

/* Colorem SGR emittere: praefixum ";38;" aut ";48;", deinde "2;R;G;B"
 * (PLENI) aut "5;n" (CCLVI) */
interior vacuum
_colorem_emittere (
      TesseraChordaAedificator* aed,
     constans character* praefixum,
                    i32  color,
         TesseraColores  colores)
{
    tessera_chorda_aedificator_appendere_literis(aed, praefixum);
    si (colores == TESSERA_COLORES_CCLVI)
    {
        tessera_chorda_aedificator_appendere_literis(aed, "5;");
        tessera_chorda_aedificator_appendere_i32(aed, _cclvi(color));
        redde;
    }
    tessera_chorda_aedificator_appendere_literis(aed, "2;");
    tessera_chorda_aedificator_appendere_i32(aed, (color >> XVI) & 0xFF);
    tessera_chorda_aedificator_appendere_character(aed, ';');
    tessera_chorda_aedificator_appendere_i32(aed, (color >> VIII) & 0xFF);
    tessera_chorda_aedificator_appendere_character(aed, ';');
    tessera_chorda_aedificator_appendere_i32(aed, color & 0xFF);
}

/* SGR: reditio plena + ornamenta + colores (nativus = nihil -
 * reditio nuda defaltas terminalis dat) */
interior vacuum
_stilum_emittere (
          TesseraChordaAedificator* aed,
     constans TesseraStilus* st,
             TesseraColores  colores)
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
        _colorem_emittere(aed, ";38;", st->color_litterae, colores);
    }
    si (st->color_fundi != TESSERA_COLOR_NATIVUS)
    {
        _colorem_emittere(aed, ";48;", st->color_fundi, colores);
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

    /* tabula graphematum (runae U5b): praeparata, crescit tantum */
    opus->graphemata_octeti = (i8*)tessera_piscina_allocare(piscina,
        (memoriae_index)TESSERA_GRAPHEMATA_OCTETI);
    opus->graphemata_initia = (i32*)tessera_piscina_allocare_ordinatum(piscina,
        (memoriae_index)TESSERA_GRAPHEMATA_MAXIMA * magnitudo(i32), IV);
    opus->graphemata_longitudines = (i8*)tessera_piscina_allocare(piscina,
        (memoriae_index)TESSERA_GRAPHEMATA_MAXIMA);
    opus->graphemata_index = (i32*)tessera_piscina_allocare_ordinatum(piscina,
        (memoriae_index)(II * TESSERA_GRAPHEMATA_MAXIMA) * magnitudo(i32),
        IV);
    si (   opus->graphemata_octeti       == NIHIL
        || opus->graphemata_initia       == NIHIL
        || opus->graphemata_longitudines == NIHIL
        || opus->graphemata_index        == NIHIL)
    {
        redde NIHIL;
    }
    memset(opus->graphemata_index, ZEPHYRUM,
        (memoriae_index)(II * TESSERA_GRAPHEMATA_MAXIMA) * magnitudo(i32));
    opus->graphemata_octeti_usi  = ZEPHYRUM;
    opus->graphemata_numerus     = ZEPHYRUM;
    opus->politica               = TESSERA_POLITICA_GRAPHEMATUM;
    opus->colores                = TESSERA_COLORES_PLENI;

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
    /* quadrum synchronum (si apertum) claudere ante $EDITOR - cautela */
    tessera_chorda_aedificator_appendere_literis(opus->aed,
        QUADRUM_FINIS "\033[0m\033[?25h");
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
tessera_politicam_ponere (
        TesseraOpus* opus,
    TesseraPolitica  politica)
{
    si (opus != NIHIL)
    {
        opus->politica = politica;
    }
}

TesseraPolitica
tessera_politica_ambitus (vacuum)
{
    constans character* programma = getenv("TERM_PROGRAM");

    si (   programma                           != NIHIL
        && strcmp(programma, "Apple_Terminal") == ZEPHYRUM)
    {
        redde TESSERA_POLITICA_SIMPLEX;
    }
    redde TESSERA_POLITICA_GRAPHEMATUM;
}

vacuum
tessera_colores_ponere (
       TesseraOpus* opus,
    TesseraColores  colores)
{
    si (opus != NIHIL)
    {
        opus->colores  = colores;
        opus->primum   = VERUM;   /* frons tota iterum emittitur */
    }
}

TesseraColores
tessera_colores_ambitus (vacuum)
{
    constans character* profunditas  = getenv("COLORTERM");
    constans character* programma    = getenv("TERM_PROGRAM");

    si (   profunditas != NIHIL
        && (   strcmp(profunditas, "truecolor") == ZEPHYRUM
            || strcmp(profunditas, "24bit") == ZEPHYRUM))
    {
        redde TESSERA_COLORES_PLENI;
    }
    si (   programma                           != NIHIL
        && strcmp(programma, "Apple_Terminal") == ZEPHYRUM)
    {
        redde TESSERA_COLORES_CCLVI;
    }
    redde TESSERA_COLORES_PLENI;
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
            cella->ornamenta       = stilus.ornamenta
                & TESSERA_ORNAMENTA_STILI;
        }
    }
}

/* Latitudo signi compacti (runae U5): ASCII = I sine decodificatione;
 * ceterum runae_latitudo. Latitudo 0 hic = I (vocans cellulam
 * explicite posuit; scriptio signa componentia ante omittit). */
interior i32
_latitudo_signi (
    i32 signum)
{
             i8  octeti[IV];
            i32  k;
    constans i8* cursor = octeti;
            s32  runa;

    si (signum < 0x80)
    {
        redde I;
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        octeti[k] = (i8)((signum >> (VIII * k)) & 0xFF);
    }
    runa = tessera_utf8_decodere(&cursor, octeti
        + tessera_signum_mensura(signum));
    redde (tessera_runae_latitudo(runa) == II) ? II : I;
}

/* Runam latam quae per (x, y) scinderetur solvere: si (x, y) est
 * continuatio, initium eius vacuatur; si initium, continuatio eius.
 * Cellula ipsa a vocante mox scribitur. */
interior vacuum
_dimidium_solvere (
    TesseraOpus* opus,
            s32  x,
            s32  y)
{
    TesseraCellula* cella = &opus->tergum[_index(x, y)];

    si (   (cella->ornamenta & TESSERA_ORNAMENTUM_CONTINUATIO)
        && _in_finibus(opus, x - I, y))
    {
        TesseraCellula* initium = &opus->tergum[_index(x - I, y)];

        initium->signum     = ZEPHYRUM;
        initium->ornamenta &= ~(i32)(TESSERA_ORNAMENTUM_LATUM
                                     | TESSERA_ORNAMENTUM_GRAPHEMA);
    }
    si (   (cella->ornamenta & TESSERA_ORNAMENTUM_LATUM)
        && _in_finibus(opus, x + I, y))
    {
        TesseraCellula* continuatio = &opus->tergum[_index(x + I, y)];

        continuatio->signum     = ZEPHYRUM;
        continuatio->ornamenta  &= ~(i32)TESSERA_ORNAMENTUM_CONTINUATIO;
    }
}

/* Graphema plurium runarum internare: ID (idem pro octetis aequalibus)
 * aut -1 si limes (tabula plena, arena plena, graphema longius).
 * Dispersio FNV-1a, tentatio linearis; loculi ID+1 (0 = vacuus). */
interior s32
_graphema_internare (
    TesseraOpus* opus,
    constans i8* octeti,
            i32  mensura)
{
    i32 friatio = 2166136261u;
    i32 mascula = (i32)(II * TESSERA_GRAPHEMATA_MAXIMA) - I;
    i32 loculus;
    i32 k;
    i32 id;

    si (mensura <= ZEPHYRUM || mensura > TESSERA_GRAPHEMA_OCTETI_MAXIMI)
    {
        redde -I;
    }
    per (k = ZEPHYRUM; k < mensura; k++)
    {
        friatio ^= (i32)octeti[k];
        friatio *= 16777619;
    }
    loculus = friatio & mascula;
    dum (opus->graphemata_index[loculus] != ZEPHYRUM)
    {
        id = opus->graphemata_index[loculus] - I;
        si (   (i32)opus->graphemata_longitudines[id] == mensura
            && memcmp(opus->graphemata_octeti
                + opus->graphemata_initia[id],
                   octeti, (memoriae_index)mensura) == ZEPHYRUM)
        {
            redde (s32)id;
        }
        loculus = (loculus + I) & mascula;
    }
    si (   opus->graphemata_numerus >= (i32)TESSERA_GRAPHEMATA_MAXIMA
        || opus->graphemata_octeti_usi + mensura
               > (i32)TESSERA_GRAPHEMATA_OCTETI)
    {
        redde -I;   /* limes: vocans ad runam primam redit */
    }
    id = opus->graphemata_numerus++;
    memcpy(opus->graphemata_octeti + opus->graphemata_octeti_usi,
        octeti,
        (memoriae_index)mensura);
    opus->graphemata_initia[id]        = opus->graphemata_octeti_usi;
    opus->graphemata_longitudines[id]  = (i8)mensura;
    opus->graphemata_octeti_usi        += mensura;
    opus->graphemata_index[loculus]    = id + I;
    redde (s32)id;
}

/* Nucleus collocationis: latitudo data (I aut II), graphema = signum
 * est ID internatum. Regulae U5: dimidia scissa vacuantur, columna
 * ultima spatium fit. */
interior vacuum
_cellulam_collocare (
      TesseraOpus* opus,
              s32  x,
              s32  y,
              i32  signum,
    TesseraStilus  stilus,
              i32  latitudo,
              b32  graphema)
{
    TesseraCellula* cella;
               i32  ornamenta =
                   stilus.ornamenta & TESSERA_ORNAMENTA_STILI;

    si (opus == NIHIL || !_in_finibus(opus, x, y))
    {
        redde;  /* praecisio taciturna */
    }
    _dimidium_solvere(opus, x, y);
    si (latitudo == II)
    {
        si (_in_finibus(opus, x + I, y))
        {
            _dimidium_solvere(opus, x + I, y);
        }
        alioquin
        {
            signum    = ZEPHYRUM;   /* columna ultima: spatium, numquam
                                     * scissa neque involuta */
            latitudo = I;
            graphema = FALSUM;
        }
    }
    cella                  = &opus->tergum[_index(x, y)];
    cella->signum          = signum;
    cella->color_litterae  = stilus.color_litterae;
    cella->color_fundi     = stilus.color_fundi;
    cella->ornamenta       = ornamenta
        | ((latitudo == II) ? TESSERA_ORNAMENTUM_LATUM : ZEPHYRUM)
        | (graphema ? TESSERA_ORNAMENTUM_GRAPHEMA : ZEPHYRUM);
    si (latitudo == II)
    {
        TesseraCellula* continuatio = &opus->tergum[_index(x + I, y)];

        continuatio->signum          = ZEPHYRUM;
        continuatio->color_litterae  = stilus.color_litterae;
        continuatio->color_fundi     = stilus.color_fundi;
        continuatio->ornamenta       = ornamenta
            | TESSERA_ORNAMENTUM_CONTINUATIO;
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
    _cellulam_collocare(opus, x, y, signum, stilus,
        _latitudo_signi(signum), FALSUM);
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

i32
tessera_cellulae_octeti (
    constans TesseraOpus* opus,
                     s32  x,
                     s32  y,
                      i8* exitus,
                     i32  capacitas)
{
    TesseraCellula cella;
               i32 n;
               i32 k;

    si (opus == NIHIL || exitus == NIHIL || !_in_finibus(opus, x, y))
    {
        redde ZEPHYRUM;
    }
    cella = opus->tergum[_index(x, y)];
    si (cella.ornamenta & TESSERA_ORNAMENTUM_GRAPHEMA)
    {
        n = (i32)opus->graphemata_longitudines[cella.signum];
        si (n > capacitas)
        {
            redde ZEPHYRUM;
        }
        memcpy(exitus, opus->graphemata_octeti
            + opus->graphemata_initia[cella.signum], (memoriae_index)n);
        redde n;
    }
    n = tessera_signum_mensura(cella.signum);
    si (n > capacitas)
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        exitus[k] = (i8)((cella.signum >> (VIII * k)) & 0xFF);
    }
    redde n;
}

constans i8*
tessera_graphema_ponere (
      TesseraOpus* opus,
              s32  x,
              s32  y,
      constans i8* initium,
      constans i8* finis,
    TesseraStilus  stilus,
              i32* latitudo)
{
    constans i8* post_runae = initium;
    constans i8* post;
            s32  runa;
            s32  id;
            i32  prima;

    *latitudo = ZEPHYRUM;
    si (opus == NIHIL)
    {
        redde finis;
    }
    si (initium == NIHIL || initium >= finis)
    {
        redde initium;
    }
    si (*initium < 0x20 || *initium == 0x7F)
    {
        /* octetus regiminis -> '?' */
        _cellulam_collocare(opus, x, y, '?', stilus, I, FALSUM);
        *latitudo = I;
        redde initium + I;
    }
    runa = tessera_utf8_decodere(&post_runae, finis);
    si (runa < ZEPHYRUM)
    {
        /* series invalida -> '?' per OCTETUM (non per seriem) */
        _cellulam_collocare(opus, x, y, '?', stilus, I, FALSUM);
        *latitudo = I;
        redde initium + I;
    }
    /* graphema (UAX #29) et latitudo eius (Ghostty) */
    post = tessera_runae_graphema_ex_politica(initium, finis,
        (opus->politica == TESSERA_POLITICA_SIMPLEX)
            ? RUNAE_POLITICA_SIMPLEX : RUNAE_POLITICA_GRAPHEMATUM,
        latitudo);
    si (*latitudo == ZEPHYRUM)
    {
        redde post;   /* signum sine basi: nihil pingitur */
    }
    si (post == post_runae)
    {
        _cellulam_collocare(opus, x, y,
            tessera_signum_ex_octetis(initium, (i32)(post - initium)),
            stilus, *latitudo, FALSUM);
        redde post;
    }
    id = _graphema_internare(opus, initium, (i32)(post - initium));
    si (id >= ZEPHYRUM)
    {
        _cellulam_collocare(opus, x, y, (i32)id, stilus, *latitudo,
            VERUM);
        redde post;
    }
    /* limes tabulae: runa prima sola, columnae reliquae unitatis
     * spatia (mensura runae servatur) */
    prima = tessera_runae_latitudo(runa);
    si (prima > ZEPHYRUM)
    {
        _cellulam_collocare(opus, x, y,
            tessera_signum_ex_octetis(initium,
                (i32)(post_runae - initium)),
            stilus, prima, FALSUM);
    }
    per (; prima < *latitudo; prima++)
    {
        _cellulam_collocare(opus, x + (s32)prima, y, (i32)' ', stilus,
            I, FALSUM);
    }
    redde post;
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
            i32  latitudo;

    si (opus == NIHIL || datum == NIHIL)
    {
        redde;
    }
    cursor  = datum;
    finis   = datum + mensura;
    dum (cursor < finis)
    {
        cursor  = tessera_graphema_ponere(opus, cx, y, cursor, finis,
            stilus, &latitudo);
        cx     += (s32)latitudo;
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
    /* Quadrum synchronum (?2026): initium semper praemittitur; si
     * nihil sequitur, quadrum vacuum manet (nulli octeti, ut prius) */
    tessera_chorda_aedificator_appendere_literis(opus->aed, QUADRUM_INITIUM);
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
            si (cella->ornamenta & TESSERA_ORNAMENTUM_CONTINUATIO)
            {
                /* dimidium secundum numquam emittitur: initium eius
                 * (cellula praecedens) pro utroque pingitur */
                opus->frons[idx] = *cella;
                perge;
            }
            si (opus->primum)
            {
                pingenda = !_cellulae_aequales(cella, &CELLULA_VACUA);
                opus->frons[idx] = *cella;
            }
            alioquin
            {
                pingenda = !_cellulae_aequales(cella,
                    &opus->frons[idx]);
                si (   !pingenda
                    && (cella->ornamenta & TESSERA_ORNAMENTUM_LATUM)
                    && x + I < (s32)opus->latitudo)
                {
                    /* continuatio mutata, initium non: repingere */
                    pingenda = !_cellulae_aequales(&opus->tergum[idx
                        + I],
                        &opus->frons[idx + I]);
                }
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
                stilus_cellae.ornamenta       = cella->ornamenta
                    & TESSERA_ORNAMENTA_STILI;
                si (   !stilus_validus
                    || !tessera_stilus_aequalis(stilus_currens,
                           stilus_cellae))
                {
                    _stilum_emittere(opus->aed, &stilus_cellae,
                        opus->colores);
                    stilus_currens = stilus_cellae;
                    stilus_validus = VERUM;
                }
            }
            si (cella->ornamenta & TESSERA_ORNAMENTUM_GRAPHEMA)
            {
                TesseraChorda graphema;

                graphema.datum    = opus->graphemata_octeti
                    + opus->graphemata_initia[cella->signum];
                graphema.mensura  = (i32)
                    opus->graphemata_longitudines[cella->signum];
                tessera_chorda_aedificator_appendere_chorda(opus->aed,
                    graphema);
            }
            alioquin
            {
                tessera_signum_scribere(opus->aed, cella->signum);
            }
            opus->frons[idx] = *cella;
            opus->fructus.cellulae_mutatae++;
            mutatae_quadri++;

            /* terminal consentiens cursorem per latitudinem movet */
            pos_x = x + ((cella->ornamenta & TESSERA_ORNAMENTUM_LATUM)
                             ? II : I);
            pos_y = y;
                        si (cella->ornamenta & (TESSERA_ORNAMENTUM_LATUM
                                    | TESSERA_ORNAMENTUM_GRAPHEMA))
                        {
                /* CONTINENTIA (etiam post graphema plurium runarum:
                 * ibi terminalia maxime dissentiunt): terminal dissentiens de latitudine
                 * cellulam unam laedit, non ordinem - CUP ante
                 * proximam (features/004) */
                pos_x = -I;
                        }
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

        /* plus quam initium synchroniae = quadrum non vacuum: claudere
         * et scribere; aliter NIHIL scribitur (quadrum vacuum) */
        si (visus.mensura > (i32)(magnitudo(QUADRUM_INITIUM) - I))
        {
            tessera_chorda_aedificator_appendere_literis(opus->aed,
                QUADRUM_FINIS);
            visus = tessera_chorda_aedificator_spectare(opus->aed);
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
