/* tonus.h - Tonus (modus ecclesiasticus) lineae unius: finalis,
 * ambitus, tenor, clausulae (purum: carmen et clavis tantum)
 *
 * LINEA: pars monodica (notae non simul; percussio omissa). Clavis
 * modalis (carmen.h MODUS) finalem et collectionem dat - clavis ad
 * notam ultimam lineae; clavis tonalis: tonus non iudicatur.
 *
 * FINALIS: nota ultima lineae - congruit si classis = finalis clavis.
 * AMBITUS: nota ima et summa. PLAGALIS si ima quartam aut plus infra
 *   finalem (<= finalis - V semitonia; finalis = nota ultima classis
 *   finalis in linea - nulla: proxima infra imam), aliter AUTHENTICUS
 *   (subtonium licet). Numerus
 *   toni: dorius I/II, phrygius III/IV, lydius V/VI, mixolydius
 *   VII/VIII, aeolius IX/X, ionius XI/XII (Glareanus).
 * TENOR (repercussio): classis praeter finalem cum longitudine summata
 *   maxima (aequales: exspectata, deinde classis minima); exspectatus
 *   a finali: I +VII, II +III, III +VIII, IV +V, V +VII, VI +IV, VII
 *   +VII, VIII +V, IX +VII, X +III, XI +VII, XII +IV - congruit aut
 *   nominatur.
 * CLAUSULAE: notae phrasium finales - sub fermata (corona initium
 *   tegit), ante pausam, et ultima; ordine temporis.
 *
 * Linea (tonus_scribere; auditio -carmen T -tonus, per partem):
 *   "I (dorius authenticus): finalis D, ambitus C4-D5, tenor A,
 *    clausulae A A F D" - tenor alienus: "tenor G (exspectatus A)"
 * Censura: regula 'finalis' (censura.h; ORD., in clave modali) -
 * linea monodica quae non in finali desinit nominatur.
 */
/* <aedilis corpus="lib/tonus.c"/> */
#ifndef TONUS_H
#define TONUS_H

#include "latina.h"
#include "chorda_aedificator.h"
#include "carmen.h"

/* clausulae servatae maximae (numerus totus numeratur) */
#define TONUS_CLAUSULAE XXXII

nomen structura {
                    s32  numerus;        /* I..XII */
                    b32  plagalis;
                    s32  modus;          /* CarmenClavis.modus 1..6 */
                    s32  finalis;        /* classis finalis clavis */
                    b32  in_finali;      /* nota ultima in finali */
    constans CarmenNota* ima;
    constans CarmenNota* summa;
    constans CarmenNota* ultima;
                    s64  pulsus_ultimae; /* in carmine */
                    s32  frustum_ultimae;
                    s32  tenor;          /* classis inventa; -1 nulla */
                    s32  tenor_exspectatus;
    constans CarmenNota* clausulae[TONUS_CLAUSULAE];
                    s64  pulsus_clausularum[TONUS_CLAUSULAE];
                    i32  numerus_clausularum;  /* totus */
} Tonus;

/* tonus partis 'pars' (index in Carmen.partes). FALSUM: clavis ad notam
 * ultimam tonalis, pars vacua aut non monodica */
b32
tonus_partis (
    constans Carmen* carmen,
                i32  pars,
              Tonus* tonus);

/* "I (dorius authenticus): finalis D, ambitus C4-D5, tenor A,
 * clausulae A A F D" (sine '\n') */
b32
tonus_scribere (
       constans Carmen* carmen,
        constans Tonus* tonus,
     ChordaAedificator* textus);

#endif /* TONUS_H */
