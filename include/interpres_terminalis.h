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

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

#include "latina.h"
#include "eventus.h"
#include "eventus_cauda.h"
#include "series_terminalis.h"

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

/* Glutinum promotum (eventus B3b): viae absolutae '\n' iunctae ->
 * EVENTUS_DEPOSITIO (promota) ad indicatorem ultimum - ANTE tractum
 * visum, non locum depositionis (terminal per tractum caecus est; park
 * 008). Redde 0 si tabula caudae sine loco (vocans textum reddat). */
i32
interpres_depositio (
    InterpresTerminalis* interpres,
            constans i8* viae,
                    i32  mensura,
                    i32  numerus,
                    s64  tempus,
           EventusCauda* cauda);

#endif /* INTERPRES_TERMINALIS_H */
