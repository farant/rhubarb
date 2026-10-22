/* circulus.h - anulus unius scriptoris et unius lectoris (SPSC)
 *
 * Bibliotheca generalis (non soni propria): loci mensurae fixae
 * in memoria vocantis, scriptor UNUS et lector UNUS, fortasse in filis
 * diversis. Numquam obstruit (plenus / vacuus = FALSUM statim), numquam
 * allocat post circulus_parare, numquam serat (lock) - ergo in filo
 * soni reali tutus (spec sonus §IV.4).
 *
 * Indices = numeratores crescentes (circumvolutio insignata in C
 * definita); capacitas potentia II, ergo locus = index & (capacitas -
 * I) et numerus = caput - cauda etiam post circumvolutionem. caput SOLUS
 * scriptor scribit, caudam SOLUS lector; quisque alterius indicem per
 * atomus_legere legit et suum per atomus_publicare publicat - locus
 * ante indicem scriptum, ergo lector locum semiscriptum numquam videt.
 *
 * Structura valor in memoria vocantis (sicut Sors); campi interni sunt -
 * per functiones solas tange.
 *
 * USUS:
 *   Circulus c;
 *   i8* memoria = piscina_allocare(piscina, CCLVI * magnitudo(Iussum));
 *   circulus_parare(&c, memoria, CCLVI, magnitudo(Iussum));
 *   scriptor:  si (!circulus_scribere(&c, &iussum)) amissum++;
 *   lector:    dum (circulus_legere(&c, &iussum)) agere(&iussum);
 */
/* <aedilis corpus="lib/circulus.c"/> */
#ifndef CIRCULUS_H
#define CIRCULUS_H

#include "latina.h"

nomen structura {
               i8* loci;        /* capacitas x mensura octetorum */
              i32  capacitas;   /* loci; potentia II */
              i32  mensura;     /* octeti per locum */
    volatilis i32  caput;       /* loci scripti (scriptor solus) */
    volatilis i32  cauda;       /* loci lecti (lector solus) */
} Circulus;

/* FALSUM si capacitas non potentia II (aut 0), mensura 0, memoria
 * NIHIL; memoria >= capacitas x mensura octeti */
b32
circulus_parare (
    Circulus* c,
      vacuum* memoria,
         i32  capacitas,
         i32  mensura);

/* SCRIPTOR solus: elementum (mensura octeti) copiat; FALSUM si plenus
 * (nihil scriptum) */
b32
circulus_scribere (
             Circulus* c,
      constans vacuum* elementum);

/* LECTOR solus: elementum proximum in 'elementum' copiat; FALSUM si
 * vacuus */
b32
circulus_legere (
    Circulus* c,
      vacuum* elementum);

/* elementa scripta nondum lecta - utrique filo aestimatio (alter
 * interim movet), exacta si alter quiescit */
i32
circulus_numerus (
    constans Circulus* c);

#endif /* CIRCULUS_H */
