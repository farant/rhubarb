/* harmonia.h - Quid simul sonat: acordum nominatum (analysis, non
 * coniectura)
 *
 * Agens harmoniam in capite portabat dum componebat ('vesperae',
 * 2026-10-23); haec bibliotheca eam NOMINAT: classes altitudinum
 * sonantium (XII bits) cum exemplaribus EXACTE comparantur. Quod sonat
 * refertur, non quod implicatur - cantus duarum vocum fere dyades
 * monstrat ('(E)', 'E5'); harmonia implicita (opinio) alia res est.
 *
 * EXEMPLARIA (intervalla a radice):
 *   ""     {0,4,7}      m      {0,3,7}      dim    {0,3,6}
 *   aug    {0,4,8}      7      {0,4,7,10} et {0,4,10}
 *   maj7   {0,4,7,11}   m7     {0,3,7,10}   m7b5   {0,3,6,10}
 *   dim7   {0,3,6,9}    sus4   {0,5,7}      sus2   {0,2,7}
 *   (5)    {0,7}        (3)    {0,4}        m(3)   {0,3}
 * dyades in parenthesi: 'A5' notam A5 legeretur (grammatica eadem ac
 * partitura)
 * sextae (C6 = Am7) nondum (Franus 10-23): classes eaedem -> m7.
 * Radices plures (aug, dim7, dyades): radix = bassus si congruit,
 * aliter classis minima (ordo exemplarium, deinde classis).
 *
 * SCRIPTIO: "D", "Bm", "A7", "G/B" (bassus non radix), "A(5)", "(E)"
 * (classis una), "[C E F#]" (ignotum, a basso sursum), "r" (nihil
 * sonat); litterae ex clave ad pulsum (partitura_orthographia).
 *
 * LINEA (partitura.h, linea addita): per tempus mensurae (1 /
 * denominator; 6/8 9/8 12/8: tres octavae) acordum ad initium temporis;
 * aequalia sequentia in signum unum cum valore (D:2 G:2).
 *
 * USUS:
 *   forma.additae[0].titulus  = "harmonia";
 *   forma.additae[0].scribere = harmonia_lineam_scribere;
 *   (vacuum)partitura_scribere(carmen, &forma, aedificator);
 */
/* <aedilis corpus="lib/harmonia.c"/> */
#ifndef HARMONIA_H
#define HARMONIA_H

#include "latina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "carmen.h"

nomen enumeratio {
    HARMONIA_MAIOR,
    HARMONIA_MINOR,
    HARMONIA_DIMINUTA,
    HARMONIA_AUCTA,
    HARMONIA_SEPTIMA,
    HARMONIA_SEPTIMA_MAIOR,
    HARMONIA_SEPTIMA_MINOR,
    HARMONIA_DIMIDIA_DIMINUTA,
    HARMONIA_DIMINUTA_SEPTIMA,
    HARMONIA_SUSPENSA_IV,
    HARMONIA_SUSPENSA_II,
    HARMONIA_QUINTA,
    HARMONIA_TERTIA_MAIOR,
    HARMONIA_TERTIA_MINOR,
    HARMONIA_CLASSIS_UNA,
    HARMONIA_IGNOTA,
    HARMONIA_NULLA
} HarmoniaGenus;

nomen structura {
    HarmoniaGenus genus;
              i32 radix;     /* classis 0..11 (C = 0) */
              i32 bassus;    /* classis notae imae */
              i32 classes;   /* XII bits: 1 << classis */
        character litterae[XII];  /* littera scripta per classem ('\0' =
                                   * nulla): orthographia auctoris nomen
                                   * acordi regit (C#m, non Dbm) */
} Harmonia;

/* purum: classes (bits) et bassus (classis notae imae) -> acordum */
Harmonia
harmonia_ex_classibus (
    i32 classes,
    i32 bassus);

/* littera notae in classem eius (prima vincit; '\0' ignoratur) */
vacuum
harmonia_litteram_notare (
               Harmonia* h,
    constans CarmenNota* nota);

/* acordum sine tertia: classes (bits) congruunt exemplari cuius tertia
 * abest - {0} classis una, {0,7} quinta, {0,7,10} septima, {0,7,11}
 * septima maior. Radix in his exemplaribus unica; VERUM -> *radix
 * (classis 0..11). sus2/sus4 NON: tertia consulto substituta (titulus
 * eam nominat) */
b32
harmonia_sine_tertia (
     i32  classes,
     i32* radix);

/* quod sonat ad pulsum: partes omnes non mutae (si sola quaedam,
 * solae), sine percussione */
Harmonia
harmonia_ad_pulsum (
    constans Carmen* carmen,
                s64  pulsus);

/* nomen acordi (litterae ex clave ad pulsum). FALSUM: memoria deest */
b32
harmonia_scribere (
       constans Carmen* carmen,
     constans Harmonia* harmonia,
                   s64  pulsus,
     ChordaAedificator* textus);

/* linea addita partiturae (PartituraAddita.scribere): ctx non
 * adhibetur */
b32
harmonia_lineam_scribere (
               vacuum* ctx,
      constans Carmen* carmen,
                  s64  initium,
                  s64  finis,
    ChordaAedificator* textus);

#endif /* HARMONIA_H */
