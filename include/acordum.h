/* acordum.h - Acordum: classes altitudinum cum nomine (purum, sine
 * carmine)
 *
 * Quid acordum SIT: classes (XII bits), radix, bassus, genus ex tabula
 * exemplarium - et nomen eius in utramque partem: classes -> genus
 * (acordum_ex_classibus; harmonia.h quod in carmine sonat nominat) et
 * nomen -> classes (acordum_legere; comitatus acorda scripta realizat).
 * Tabula UNA utrique: vocabularium novum (sextae, nonae) hic solum
 * additur.
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
 * NOMEN (acordum_legere) - grammatica harmonia_scribere:
 *   nomen := radix suffixum [ '/' radix ]
 *   radix := A..G (magna) [ '#' | '##' | 'b' | 'bb' ]
 *   suffixum ex tabula, LONGISSIMUM congruens ("m7b5" ante "m7" ante
 *   "m"); exemplar PRIMUM generis legitur ("7" = {0,4,7,10})
 *   "C" "Bbm7b5" "F#dim7" "G/B" "C/D" "A(5)" "Em(3)"
 * Bassus post '/' extra classes stare potest ("C/D": D transiens) -
 * classes = exemplar solum. Non hic: "(E)", "[C E F#]", "r"
 * (scriptiones analysis, non acorda petita).
 * LITTERAE ex nomine: radix et bassus ut scripti; ceterae per
 * intervallum, gradibus litterarum a radice - 2: +1; 3, 4: +2; 5: +3;
 * 6, 7, 8: +4; 9, 10, 11: +6 (C#m -> E G#; Bdim7 -> D F Ab;
 * Caug -> G#).
 */
/* <aedilis corpus="lib/acordum.c"/> */
#ifndef ACORDUM_H
#define ACORDUM_H

#include "latina.h"
#include "chorda.h"

nomen enumeratio {
    ACORDUM_MAIOR,
    ACORDUM_MINOR,
    ACORDUM_DIMINUTA,
    ACORDUM_AUCTA,
    ACORDUM_SEPTIMA,
    ACORDUM_SEPTIMA_MAIOR,
    ACORDUM_SEPTIMA_MINOR,
    ACORDUM_DIMIDIA_DIMINUTA,
    ACORDUM_DIMINUTA_SEPTIMA,
    ACORDUM_SUSPENSA_IV,
    ACORDUM_SUSPENSA_II,
    ACORDUM_QUINTA,
    ACORDUM_TERTIA_MAIOR,
    ACORDUM_TERTIA_MINOR,
    ACORDUM_CLASSIS_UNA,
    ACORDUM_IGNOTA,
    ACORDUM_NULLA
} AcordumGenus;

nomen structura {
    AcordumGenus genus;
             i32 radix;     /* classis 0..11 (C = 0) */
             i32 bassus;    /* classis notae imae */
             i32 classes;   /* XII bits: 1 << classis */
       character litterae[XII];  /* littera scripta per classem ('\0' =
                                  * nulla): orthographia auctoris nomen
                                  * acordi regit (C#m, non Dbm) */
} Acordum;

/* purum: classes (bits) et bassus (classis notae imae) -> acordum */
Acordum
acordum_ex_classibus (
    i32 classes,
    i32 bassus);

/* acordum sine tertia: classes (bits) congruunt exemplari cuius tertia
 * abest - {0} classis una, {0,7} quinta, {0,7,10} septima, {0,7,11}
 * septima maior. Radix in his exemplaribus unica; VERUM -> *radix
 * (classis 0..11). sus2/sus4 NON: tertia consulto substituta (titulus
 * eam nominat) */
b32
acordum_sine_tertia (
     i32  classes,
     i32* radix);

/* nomen TOTUM -> acordum cum litteris. FALSUM: nomen ignotum (acordum
 * non mutatur) */
b32
acordum_legere (
      chorda  titulus,
     Acordum* acordum);

/* suffixum generis: "m7", "" (maior) ...; "" generibus sine nomine
 * (classis una, ignota, nulla). Statica */
constans character*
acordum_suffixum (
    AcordumGenus genus);

#endif /* ACORDUM_H */
