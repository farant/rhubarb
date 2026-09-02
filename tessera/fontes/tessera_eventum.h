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

#include "latina.h"
#include "piscina.h"
#include "tessera_pons.h"

#define TESSERA_LECTOR_BUFFER 64
#define TESSERA_MORA_FUGAE_MS 25

nomen enumeratio {
    TESSERA_EVENTUM_NIHIL = 0,   /* mora exacta */
    TESSERA_EVENTUM_CLAVIS,
    TESSERA_EVENTUM_MUS,
    TESSERA_EVENTUM_AMPLITUDO,   /* magnitudo scrinii mutata */
    TESSERA_EVENTUM_RESUMPTUM    /* post SIGCONT - pictura plena! */
} TesseraEventumGenus;

nomen enumeratio {
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
    TESSERA_CLAVIS_FUGA,        /* ESC */
    TESSERA_CLAVIS_REDITUS,
    TESSERA_CLAVIS_TABULA,
    TESSERA_CLAVIS_RETRORSUM,
    TESSERA_CLAVIS_FUNCTIO      /* F1-12: campus numerus */
} TesseraClavis;

#define TESSERA_MODIFICATOR_IMPERIUM  0x01  /* ctrl */
#define TESSERA_MODIFICATOR_ALTERUM   0x02  /* alt/meta */
#define TESSERA_MODIFICATOR_MAIUSCULA 0x04  /* shift (ubi noscibile) */

nomen enumeratio {
    TESSERA_MUS_PRESSUS = 0,
    TESSERA_MUS_SOLUTUS,
    TESSERA_MUS_ROTA_SURSUM,
    TESSERA_MUS_ROTA_DEORSUM
} TesseraMusGenus;

nomen structura {
    TesseraEventumGenus genus;

    /* CLAVIS */
              s32 runa;          /* codepoint; 0 si clavis specialis */
    TesseraClavis clavis;        /* NULLA si runa */
              i32 modificatores;
              i32 numerus;       /* FUNCTIO: 1-12 */

    /* MUS */
    TesseraMusGenus mus_genus;
                s32 mus_x;         /* 0-basata */
                s32 mus_y;
                i32 mus_pulsus;    /* 0/1/2 */

    /* AMPLITUDO */
    i32 latitudo;
    i32 altitudo;
} TesseraEventum;

nomen structura {
    TesseraPons* pons;
             i8  buffer[TESSERA_LECTOR_BUFFER];
            i32  mensura;        /* octeti validi */
            i32  latitudo_nota;  /* amplitudo novissime visa */
            i32  altitudo_nota;
} TesseraLector;

TesseraLector*
tessera_lector_creare (
        Piscina* piscina,
    TesseraPons* pons);

/* Eventum proximum intra moram (ms); mora < 0 = sine fine.
 * NIHIL genus = mora exacta. */
TesseraEventumGenus
tessera_eventum_expectare (
     TesseraLector* lector,
    TesseraEventum* eventum,
               s32  mora_ms);

#endif /* TESSERA_EVENTUM_H */
