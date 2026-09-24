/* sors.h - Numeri pseudo-fortuiti SEMINABILES et deterministici
 *
 * Idem semen = eadem series in omni machina, compilatore et versione:
 * integri soli (PCG32 pro rivo, SplitMix64 pro semine expandendo), ergo
 * replay (ictus aspergilli), artificia proceduralia et probationes
 * fuzzing reproducibiles sunt. Distincta a moneta (/dev/urandom pro
 * identitatibus - tuta sed NON seminabilis).
 *
 * Sors valor purus est (XVI octeti): sine piscina, sine statu globali;
 * copiatur et in ictu servatur libere.
 *
 * USUS:
 *   Sors s;
 *   sors_seminare(&s, semen, ZEPHYRUM);
 *   x = sors_intra(&s, C);            (* [0, C) sine inclinatione *)
 *   Sors ictus = sors_derivare(&s, numerus_ictus);   (* rivus filius *)
 */
/* <aedilis corpus="lib/sors.c"/> */
#ifndef SORS_H
#define SORS_H

#include "latina.h"

nomen structura {
    i64 status;
    i64 incrementum;   /* impar semper - rivum (seriem) eligit */
} Sors;

/* semen: quilibet valor (SplitMix64 eum miscet); series: numerus rivi
 * independentis (0, 1, 2...) - eadem semen, series diversae = rivi
 * disiuncti */
vacuum
sors_seminare (
    Sors* s,
     i64  semen,
     i64  series);

/* rivus filius ex parente et clave - PURUS in parente (parens non
 * mutatur): filius clavis k idem est quotcumque alii ante derivati
 * sunt. Pro seminibus per ictum, per artificium, per casum
 * probationis */
Sors
sors_derivare (
    constans Sors* s,
              i64  clavis);

/* XXXII bits */
i32
sors_proximum (
    Sors* s);

/* [0, limes) sine inclinatione (Lemire); limes 0 -> 0 */
i32
sors_intra (
    Sors* s,
     i32  limes);

/* [imum, summum] inclusive; imum > summum -> imum */
s32
sors_inter (
    Sors* s,
     s32  imum,
     s32  summum);

/* [0, 1) ex LIII bits */
f64
sors_f64 (
    Sors* s);

/* [0, 1) ex XXIV bits */
f32
sors_f32 (
    Sors* s);

/* VERUM cum probabilitate numerator/denominator; denominator 0 ->
 * FALSUM */
b32
sors_casu (
    Sors* s,
     i32  numerator,
     i32  denominator);

/* Fisher-Yates: numerus elementa magnitudinis 'magnitudo_elementi'
 * permutat (octeti permutantur - nulla allocatio) */
vacuum
sors_miscere (
      Sors* s,
    vacuum* elementa,
       i32  numerus,
       i32  magnitudo_elementi);

/* LEGATUM: LCG classicus ANSI (status * 1103515245 + 12345) - statum
 * provehit et reddit. Ut probationes totalitatis bibliotheca utantur
 * sine casibus generatis mutatis */
i32
sors_lcg_classicus (
    i32* status);

#endif /* SORS_H */
