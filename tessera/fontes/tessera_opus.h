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
