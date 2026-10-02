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
 */

#ifndef INTERPRES_TERMINALIS_H
#define INTERPRES_TERMINALIS_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

#include "latina.h"
#include "eventus.h"
#include "eventus_cauda.h"
#include "series_terminalis.h"

nomen structura {
    s32 cellula_latitudo;   /* pixela NOSTRA per cellulam (Modulus) */
    s32 cellula_altitudo;   /* et gradus rotulae (linea una) */
    b32 alterum_pendens;    /* ESC solus abruptus: alterum proximae */
} InterpresTerminalis;

/* Cellula in pixelis nostris: mus ad CENTRUM cellulae ponitur. */
vacuum
interpres_initiare (
    InterpresTerminalis* interpres,
                    s32  cellula_latitudo,
                    s32  cellula_altitudo);

/* Lexema unum -> eventa (0..n) in caudam, tempore dato. post_moram:
 * lexema FUGA ex series_lectorem_evacuare (ESC solus = Effugium, ESC
 * ESC = duo, 'ESC x' = alterum + x, cetera abiciuntur). CSI 200~/201~
 * (glutinum) nihil reddit: fons corpus colligit (_glutinum). Redde
 * numerum eventorum impulsorum. */
i32
interpres_lexema (
         InterpresTerminalis* interpres,
       constans SeriesLexema* lexema,
                         b32  post_moram,
                         s64  tempus,
                EventusCauda* cauda);

/* Mus X10: octeti CRUDI post 'ESC [ M' (cb, cx, cy, cum offsetibus
 * +32/+33 ut in filo). Redde numerum eventorum. */
i32
interpres_x10 (
    InterpresTerminalis* interpres,
                    i32  cb,
                    i32  cx,
                    i32  cy,
                    s64  tempus,
           EventusCauda* cauda);

/* Corpus glutini (?2004) a fonte collectum -> TEXT origo GLUTINATA
 * (copiatum; ultra tabulam caudae truncatum). */
i32
interpres_glutinum (
    InterpresTerminalis* interpres,
            constans i8* octeti,
                    i32  mensura,
                    s64  tempus,
           EventusCauda* cauda);

#endif /* INTERPRES_TERMINALIS_H */
