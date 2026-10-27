/* acordum.h - Acordum: classes altitudinum cum nomine (purum, sine
 * carmine)
 *
 * Quid acordum SIT: classes (XII bits), radix, bassus, genus ex tabula
 * exemplarium - et nomen eius in utramque partem: classes -> genus
 * (acordum_ex_classibus; harmonia.h quod in carmine sonat nominat) et
 * nomen -> classes (acordum_legere; comitatus acorda scripta componit).
 * Tabula UNA utrique: vocabularium novum (sextae, nonae) hic solum
 * additur.
 *
 * EXEMPLARIA (intervalla a radice):
 *   ""     {0,4,7}      m      {0,3,7}      dim    {0,3,6}
 *   aug    {0,4,8}      7      {0,4,7,10} et {0,4,10}
 *   maj7   {0,4,7,11}   m7     {0,3,7,10}   m7b5   {0,3,6,10}
 *   dim7   {0,3,6,9}    sus4   {0,5,7}      sus2   {0,2,7}
 *   (5)    {0,7}        (3)    {0,4}        m(3)   {0,3}
 *   6      {0,4,7,9}    m6     {0,3,7,9}     (radix in basso solum -
 *                                             aliter m7, m7b5: C E G A
 *                                             super A = Am7, super E =
 *                                             Am7/E; item 7sus4)
 *   add9   {0,2,4,7}    9      {0,4,7,10,2} et {0,4,10,2}
 *   7b9    {0,4,7,10,1} et {0,4,10,1}
 *   maj9   {0,4,7,11,2} m9     {0,3,7,10,2}
 *   7sus4  {0,5,7,10}   7#5    {0,4,8,10}
 * dyades in parenthesi: 'A5' notam A5 legeretur (grammatica eadem ac
 * partitura)
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
 * LITTERAE ex nomine: radix et bassus ut scripti; ceterae per gradus
 * litterarum exemplaris (tonus quisque suum: IX semitonia sexta +5 in
 * 6, septima diminuta +6 in dim7) - C#m -> E G#; Bdim7 -> D F Ab;
 * Caug -> G#; Bbm6 -> G; C7b9 -> Db; F#7#5 -> C##.
 *
 * NOTA ADDITA (explicatio.h): tonus additus (sexta, nona, nona minor;
 * quarta in 7sus4, quinta aucta in 7#5) acordi pars est solum si nota
 * eius candidata non est (forma melodica
 * alienae caret) - A transiens super C E G manet transitus; A tenta
 * C6 facit.
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
    ACORDUM_SEXTA,
    ACORDUM_SEXTA_MINOR,
    ACORDUM_NONA_ADDITA,
    ACORDUM_NONA,
    ACORDUM_NONA_MINOR,
    ACORDUM_NONA_MAIOR,
    ACORDUM_NONA_MOLLIS,          /* m9 */
    ACORDUM_SEPTIMA_SUSPENSA,
    ACORDUM_SEPTIMA_AUCTA,
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

/* PROPRIETATES generis (tabula una, statica - explicatio, functio,
 * comitatus eas legunt, numquam ordinem enumerationis): intervalla a
 * radice tertiae, quintae, septimae (-1 nulla); gradus nominis
 * (explicatio, nomina aequalia: III plenum - trias, septima; II
 * suspensa; I dyas; 0 classis una, ignota, nulla); notatio numeralis
 * (functio: "o" "+" "M" "h" "sus4" "sus2" "(5)" "(3)", "" ceteris).
 * Numeralis parva = tertia minor. Genus septimae (figura 7 65 43 42)
 * = septima >= 0; figura inversionis solum gradu III */
nomen structura {
    constans character* suffixum;    /* ut acordum_suffixum */
    constans character* numeralis;
                   s32  tertia;      /* III, IV; -1 nulla */
                   s32  quinta;      /* VI, VII, VIII; -1 nulla */
                   s32  septima;     /* IX, X, XI; -1 nulla */
                   i32  gradus;      /* III, II, I, 0 */
                   i32  additae;     /* bits intervallorum ADDITORUM a
                                      * radice (sexta, nona):
                                      * explicatio */
} AcordumProprietates;

/* genus extra tabulam: proprietates NULLA */
constans AcordumProprietates*
acordum_proprietates (
    AcordumGenus genus);

#endif /* ACORDUM_H */
