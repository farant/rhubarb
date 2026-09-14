#ifndef PARTITIO_H
#define PARTITIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"


/* ==================================================
 * Partitio - greges ordinum (T35 a, oratio-spec par. 7)
 *
 * Partitio ordines 0..numerus-1 in greges dividit. Greges numerantur
 * ORDINE PRIMAE APPARITIONIS (numeratio canonica): ordo 0 in grege 0,
 * ordo qui gregem novum aperit numerum proximum accipit. Inde:
 * - partitiones aequales tabulas grex[] identicas habent;
 * - grex quisque ordinem REPRAESENTANTEM habet (primus[g]): partitio
 *   titulos non fert, lector gregem quemlibet (etiam infimi) ex
 *   columnis ordinis primi nominat.
 *
 * Ex scrutinio knotapel (knotapel/raqiya/scrutinium.h) translata, non
 * vendita: subtilior O(n) per repraesentantem (ibi greges x n),
 * latitudo per Dilworth (ibi vis bruta k <= XX), tegmina addita;
 * vtabula anuli, histogrammata, acies, analysator graphi omissa.
 *
 * Memoria ex piscina sola. Numeri ordinum inaequales RECUSANTUR
 * (NIHIL aut FALSUM), numquam truncantur. Numerus 0 = partitio vacua.
 * ================================================== */

nomen structura {
    i32  numerus;          /* ordines */
    i32  numerus_gregum;
    i32* grex;             /* grex[i]: ordine primae apparitionis */
    i32* magnitudines;     /* ordines per gregem */
    i32* primus;           /* primus[g]: ordo primus gregis g */
} Partitio;


/* ==================================================
 * Constructores
 * ================================================== */

/* notae arbitrariae (valores quilibet) -> numeratio canonica;
 * NIHIL si notae NIHIL (numerus > 0) aut memoria deficit */
Partitio*
partitio_ex_notis (
          Piscina* piscina,
              i32  numerus,
     constans i32* notae);

/* valores cellarum per octetos aequales -> greges */
Partitio*
partitio_ex_chordis (
             Piscina* piscina,
                 i32  numerus,
     constans chorda* valores);

/* grex unus (supremum reticuli) */
Partitio*
partitio_una (
     Piscina* piscina,
         i32  numerus);

/* ordo quisque solus (infimum reticuli) */
Partitio*
partitio_discreta (
     Piscina* piscina,
         i32  numerus);


/* ==================================================
 * Ordo et operationes
 * ================================================== */

/* a subtilior b (a refinat b): grex quisque a intra gregem unum b.
 * Reflexiva. FALSUM si numeri ordinum inaequales. */
b32
partitio_subtilior (
    constans Partitio* a,
    constans Partitio* b);

/* eadem divisio ordinum (numeratione canonica: tabulae identicae) */
b32
partitio_aequalis (
    constans Partitio* a,
    constans Partitio* b);

/* infimum (meet): ordines idem grex iff idem in a ET in b */
Partitio*
partitio_infimum (
              Piscina* piscina,
    constans Partitio* a,
    constans Partitio* b);

/* supremum (join): idem grex iff catena gregum a aut b iungit */
Partitio*
partitio_supremum (
              Piscina* piscina,
    constans Partitio* a,
    constans Partitio* b);


/* ==================================================
 * Reticulum super k partitiones
 *
 * ordo: relatio subtilior (diagonalis FALSUM). geminus: index primae
 * aequalis (sibi ipsi si unica). Tegmina, altitudo et latitudo inter
 * UNICAS solas et stricte. Latitudo per Dilworth: unicae minus
 * congruentia maxima super ordinem strictum (subtilior transitiva est,
 * ergo tegumentum catenarum minimum = unicae - congruentia).
 * In censu: latitudo = dimensiones testimonii independentes quas
 * columnae ferunt; altitudo = profunditas regressus quam nidificatio
 * sola permittit.
 * ================================================== */

nomen structura {
    i32  numerus;          /* k */
    b32* ordo;             /* ordo[i*k+j]: i subtilior j */
    b32* tegit;            /* tegit[i*k+j]: j tegit i immediate */
    i32* geminus;           /* index aequalis primae */
    i32  altitudo;         /* catena stricta longissima (arcus) */
    i32  latitudo;         /* antichain maxima */
} PartitioReticulum;

/* NIHIL si pars NIHIL, ordines inaequales, aut memoria deficit */
PartitioReticulum*
partitio_reticulum_struere (
                      Piscina* piscina,
    constans Partitio* constans* partes,
                           i32 k);

#endif /* PARTITIO_H */
