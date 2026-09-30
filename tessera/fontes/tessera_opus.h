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

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"

#define TESSERA_LATITUDO_MAXIMA 512
#define TESSERA_ALTITUDO_MAXIMA 256

/* Graphemata (runae U5b): graphema plurium runarum semel internatur in
 * tabula operis; cellula ID eius fert (TESSERA_ORNAMENTUM_GRAPHEMA).
 * Tabula crescit tantum (ID numquam reusatur - frons et tergum eum
 * tenere possunt); ultra limites cellula ad runam primam redit. */
#define TESSERA_GRAPHEMATA_MAXIMA      16384
#define TESSERA_GRAPHEMA_OCTETI_MAXIMI 64
#define TESSERA_GRAPHEMATA_OCTETI      262144

/* Politica latitudinis graphematum (runae U5c): GRAPHEMATUM = regula
 * Ghostty (ordinaria); SIMPLEX = ut Terminal.app (ZWJ pictographa non
 * iungit). Ex AMBITU eligitur (tessera_politica_ambitus:
 * TERM_PROGRAM), numquam quaeritur. */
nomen enumeratio {
    TESSERA_POLITICA_GRAPHEMATUM = 0,
    TESSERA_POLITICA_SIMPLEX
} TesseraPolitica;

/* Profunditas colorum emissionis (quadrans Q4, spec-v2 par. 6): PLENI =
 * SGR 38;2 / 48;2 (XXIV bitorum, ordinaria); CCLVI = 38;5 / 48;5 in
 * cubo xterm 6x6x6 (indices XVI-CCXXXI) et XXIV griseis (CCXXXII-
 * CCLV), proximus per distantiam RGB quadratam (XVI colores thematis
 * numquam - usor eos mutat). Cellulae RGB verum SERVANT; quantizatio
 * in EMISSIONE sola. Ex AMBITU eligitur, numquam quaeritur. */
nomen enumeratio {
    TESSERA_COLORES_PLENI = 0,
    TESSERA_COLORES_CCLVI
} TesseraColores;

/* Numeratores ut productum (mos silvae: fructus, non depuratio) */
nomen structura {
    i32 cellulae_collatae;
    i32 cellulae_mutatae;
    i32 octeti_emissi;
    i32 praesentationes;
    f64 tempus_praesentandi_ms;   /* cumulativum */
} TesseraFructus;

nomen structura TesseraOpus TesseraOpus;

structura TesseraOpus {
              Piscina* piscina;
          TesseraPons* pons;
       TesseraCellula* frons;    /* quod in scrinio est */
       TesseraCellula* tergum;   /* quod pingitur */
                  i32  latitudo;  /* activae (renovatio mutat) */
                  i32  altitudo;
    ChordaAedificator* aed;      /* effugia quadri (praedimensus) */
                  s32  cursor_x;  /* optatum; -1 = celatus */
                  s32  cursor_y;
                  s32  cursor_x_actus;   /* applicatum novissime */
                  s32  cursor_y_actus;
                  b32  cursor_visibilis_actus;
                  b32  primum;   /* quadrum primum = pictura plena */
       TesseraFructus  fructus;

    /* Tabula graphematum internatorum (ID = index) */
                   i8* graphemata_octeti;       /* arena */
                  i32  graphemata_octeti_usi;
                  i32* graphemata_initia;       /* ID -> offset */
                   i8* graphemata_longitudines; /* ID -> octeti */
                  i32  graphemata_numerus;
                  i32* graphemata_index;        /* ID+1; 0 vacuum */
      TesseraPolitica  politica;      /* latitudinis graphematum */
       TesseraColores  colores;       /* profunditas emissionis */
};

/* Pons REQUISITUS in Phase A (defalta posix = Phase B) */
TesseraOpus*
tessera_aperire (
        Piscina* piscina,
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
           chorda  textus,
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
