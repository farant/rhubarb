/* temperley.h - Algebra Temperley-Lieb TL_n: moduli standard W_{n,j}
 *
 * STATUS NEXUS. Fila n puncta 0..n-1 in linea; status = paria non
 * transeuntia (arcus) et j DEFECTUS (puncta sine pari), nullus
 * defectus intra arcum inclusus. Dimensio moduli W_{n,j}
 *   d = C(n, (n-j)/2) - C(n, (n-j)/2 - 1),
 * et summa d^2 super j (j eiusdem paritatis ac n) = Catalan(n).
 *
 * ORDO statuum: lexicographicus formae textus ("(())|"), ( < ) < |,
 * puncto 0 primo - ipse ordo quo enumeratio eos gignit (nullus
 * qsort). Indices statuum ergo stabiles et per API visibiles.
 *
 * ACTIO e_i (1 <= i < n, unum-basatum: puncta i-1 et i): status in
 * statum aut nihil -
 *   (a) i-1 et i inter se paria: e_i s = delta s (ansa clausa);
 *   (b) ambo paria aliis: arcus (i-1, i), pares eorum inter se;
 *   (c) unum par, alterum defectus: arcus (i-1, i), par prior fit
 *       defectus;
 *   (d) ambo defectus: e_i s = 0 (in moduli standard).
 * Coefficientes nulli in bibliotheca: genus (NIHIL, UNUM, DELTA) solum,
 * ergo idem modulus in omni anulo servit (modulo p, Z[A], radices
 * unitatis).
 *
 * TEMPERLEY_FILA_MAXIMA (XVI) tenet: clavem status (2n bitorum) in
 * i32, et indices d^2 (maximum ~1.3e7) in i32.
 *
 * USUS:
 *   TemperleyModulus* m = temperley_modulus_creare(IV, ZEPHYRUM, piscina);
 *   i32 imago;
 *   s32 genus = temperley_actio(m, I, ZEPHYRUM, &imago);
 *
 * Vide project-specs/temperley-lieb-plan.md.
 */
/* <aedilis corpus="lib/temperley.c"/> */
#ifndef TEMPERLEY_H
#define TEMPERLEY_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

#define TEMPERLEY_FILA_MAXIMA  XVI

#define TEMPERLEY_DEFECTUS     (-I)       /* par puncti defectus */
#define TEMPERLEY_ERRATUM      (-I)       /* argumenta extra ambitum */
#define TEMPERLEY_NIHIL        ZEPHYRUM   /* e_i s = 0 */
#define TEMPERLEY_UNUM         I          /* e_i s = imago */
#define TEMPERLEY_DELTA        II         /* e_i s = delta imago */

nomen structura TemperleyModulus TemperleyModulus;   /* opacum */


/* ==================================================
 * Modulus
 * ================================================== */

/* W_{fila, defectus}: status enumerati, actio e_i tabulata. NIHIL si
 * fila nulla aut > TEMPERLEY_FILA_MAXIMA, defectus > fila, aut fila -
 * defectus impar. Memoria in piscina. */
TemperleyModulus*
temperley_modulus_creare (
         i32  fila,
         i32  defectus,
     Piscina* piscina);

i32
temperley_modulus_dimensio (
    constans TemperleyModulus* m);

i32
temperley_modulus_fila (
    constans TemperleyModulus* m);

i32
temperley_modulus_defectus (
    constans TemperleyModulus* m);


/* ==================================================
 * Actio et status
 * ================================================== */

/* e_i in statum: genus TEMPERLEY_NIHIL, _UNUM aut _DELTA, et *imago
 * (index status; non scriptum pro NIHIL). TEMPERLEY_ERRATUM si i
 * extra 1..fila-1 aut status >= dimensio (*imago non tactum). */
s32
temperley_actio (
    constans TemperleyModulus* m,
                          i32  i,
                          i32  status,
                          i32* imago);

/* paria[x] (x = 0..fila-1) = par puncti x, TEMPERLEY_DEFECTUS si
 * defectus. FALSUM si status >= dimensio (paria non tacta). */
b32
temperley_status_paria (
    constans TemperleyModulus* m,
                          i32  status,
                          s32* paria);

/* index status cuius paria data sunt (fila elementa); TEMPERLEY_ERRATUM
 * si paria non status huius moduli (transeuntia, defectus inclusus,
 * numerus defectuum alius, par non reciprocum). Ad mappas inter
 * modulos, formam Gram, Jones-Wenzl. */
s32
temperley_status_quaerere (
    constans TemperleyModulus* m,
                 constans s32* paria);

/* forma textus status ("(())|"), fila characteres; chorda vacua si
 * status >= dimensio */
chorda
temperley_status_ad_chordam (
    constans TemperleyModulus* m,
                          i32  status,
                      Piscina* piscina);

#endif /* TEMPERLEY_H */
