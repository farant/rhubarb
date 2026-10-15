/* plexus.h - Greges braidarum (Artin) B_n, EXACTE: verba, actio in
 * gregem liberum F_n, aequalitas
 *
 * VERBUM: litterae s32, +i = sigma_i, -i = sigma_i^-1, 1 <= i < fila;
 * verbum vacuum = identitas. Plexus valor est (fila, longitudo,
 * litterae); litterae in piscina vocantis, numquam mutantur.
 *
 * FILA: 2 <= fila <= PLEXUS_FILA_MAXIMA (2^20); maius refutatur
 * (FALSUM), numquam truncatur.
 *
 * PERMUTATIO: sigma_i positiones i et i+1 (ab 1 numeratas) commutat,
 * litteris a sinistra ad dextram actis. plexus_permutatio ponit
 * imago[q] = filum (positione initiali, ab 0) quod in positione q (ab
 * 0) finit. Ergo imago_{pq}[q] = imago_p[imago_q[q]].
 *
 * ACTIO IN F_n (Artin): x_1 ... x_n generatores liberi;
 *   sigma_i:    x_i -> x_i x_{i+1} x_i^-1,  x_{i+1} -> x_i
 *   sigma_i^-1: x_i -> x_{i+1},             x_{i+1} -> x_{i+1}^-1 x_i
 *                                                      x_{i+1}
 * ceteri fixi. Plexui a_1 ... a_m automorphismus phi = phi_{a_1} o ...
 * o phi_{a_m} (compositio substitutionum), ergo phi_{pq} = phi_p o
 * phi_q. Actio FIDELIS est (Artin 1925): p = q in B_n si et solum si
 * phi_p = phi_q, ergo plexus_aequales problema verborum EXACTE solvit.
 *
 * LIMES: imagines verborum exponentialiter crescere possunt (|phi(x)|
 * usque ad 3^m). Omnis functio actionis limitem longitudinis summae
 * accipit; limite superato FALSUM redditur, numquam responsum
 * truncatum.
 *
 * Effecta in piscina vocantis; temporaria in piscinis propriis,
 * destructis ante reditum. FALSUM -> exitus non tangitur.
 *
 * USUS:
 *   Plexus p;
 *   b32    aequales;
 *   (vacuum)plexus_ex_chorda(III, chorda_ex_literis("1 2 1", piscina),
 *       piscina, &p);
 *   si (plexus_aequales(p, q, M, piscina, &aequales)) ...
 *
 * Consilium: project-specs/plexus-plan.md. Vide lib/plexus.worklog.md.
 */
/* <aedilis corpus="lib/plexus.c"/> */
#ifndef PLEXUS_H
#define PLEXUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

#define PLEXUS_FILA_MAXIMA              1048576
/* Delta longitudinem n(n-1)/2 habet: n <= 65536 ut in 2^31 capiat */
#define PLEXUS_FILA_DIMIDIAE_TORSIONIS  65536

nomen structura {
     i32  fila;         /* 2 <= n <= PLEXUS_FILA_MAXIMA */
     i32  longitudo;
     s32* litterae;     /* +i sigma_i, -i sigma_i^-1, 1 <= i < fila */
} Plexus;

/* verbum gregis liberi F_n: +j = x_j, -j = x_j^-1, j >= 1; semper
 * libere reductum (nulla littera iuxta inversam suam) */
nomen structura {
     i32  longitudo;
     s32* litterae;
} VerbumLiberum;


/* ==================================================
 * Verba
 * ================================================== */

/* litterae copiantur; FALSUM si fila extra 2..PLEXUS_FILA_MAXIMA aut
 * littera 0 vel |littera| >= fila */
b32
plexus_creare (
                i32  fila,
       constans s32* litterae,
                i32  numerus,
            Piscina* piscina,
             Plexus* exitus);

/* textus: integri signati spatiis separati ("1 -2 1"); vacuus =
 * identitas. FALSUM si malformatus aut littera extra 1..fila-1 */
b32
plexus_ex_chorda (
         i32  fila,
      chorda  textus,
     Piscina* piscina,
      Plexus* exitus);

/* "1 -2 1"; identitas "" */
chorda
plexus_ad_chordam (
     Plexus  p,
    Piscina* piscina);

/* a_m^-1 ... a_1^-1 */
Plexus
plexus_inversum (
     Plexus  p,
    Piscina* piscina);

/* p deinde q; FALSUM si fila diversa */
b32
plexus_iungere (
     Plexus  p,
     Plexus  q,
    Piscina* piscina,
     Plexus* exitus);

/* reductio libera: sigma_i sigma_i^-1 et sigma_i^-1 sigma_i delentur
 * (solum - relationes braidarum non adhibentur) */
Plexus
plexus_reducere (
     Plexus  p,
    Piscina* piscina);

/* summa signorum litterarum (homomorphismus B_n -> Z) */
s64
plexus_summa_exponentium (
    Plexus p);

/* imago[0 .. fila-1]: vide PERMUTATIO supra */
vacuum
plexus_permutatio (
    Plexus  p,
       i32* imago);

/* Delta = (sigma_1 ... sigma_{n-1}) (sigma_1 ... sigma_{n-2}) ...
 * sigma_1, dimidia torsio Garside; Delta^2 centrum B_n generat.
 * FALSUM si fila extra 2..PLEXUS_FILA_DIMIDIAE_TORSIONIS */
b32
plexus_dimidia_torsio (
         i32  fila,
     Piscina* piscina,
      Plexus* exitus);


/* ==================================================
 * Actio in gregem liberum (Artin) et aequalitas
 * ================================================== */

/* phi_p(x_j), 1 <= j <= fila. limes: longitudo maxima, post quamque
 * litteram, summae imaginum omnium generatorum, et cuiusque producti
 * ante reductionem (supra 2^31 - 1 non repraesentabilis). FALSUM si j
 * extra aut limes superatus. Temporaria in piscinis propriis. */
b32
plexus_imago_libera (
           Plexus  p,
              i32  j,
              i64  limes,
          Piscina* piscina,
    VerbumLiberum* exitus);

/* *aequales = (p == q in B_n): r = reductio(p q^-1); summa exponentium
 * et permutatio r prius (necessaria), deinde phi_r = identitas. FALSUM
 * si fila diversa aut limes superatus (tunc *aequales non tangitur).
 * piscina ut alveus temporarius: ad statum initii reficitur. */
b32
plexus_aequales (
     Plexus  p,
     Plexus  q,
        i64  limes,
    Piscina* piscina,
        b32* aequales);

/* "1 2 -1"; identitas "" */
chorda
verbum_liberum_ad_chordam (
    VerbumLiberum  w,
          Piscina* piscina);

#endif /* PLEXUS_H */
