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
