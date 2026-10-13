/* reticulum.h - Reticula integra EXACTA: Gram-Schmidt, reductio LLL,
 * enumeratio vectorum brevium, relationes integrae
 *
 * Reticulum = combinationes integrae vectorum basis. Duae formae:
 * - BASIS: vectores sunt LINEAE matricis super ANULUS_INTEGRORUM,
 *   productum interius ordinarium;
 * - FORMA: matrix Gram integra, symmetrica, definita positiva (G_ij =
 *   <b_i, b_j>), sine vectoribus ipsis - formae quadraticae (e.g.
 *   Goeritz) eodem nucleo tractantur.
 *
 * EXACTE, sine fluitantibus: Gram-Schmidt integra (de Weger; Cohen,
 * Algorithmus 2.6.7) per determinantes d_i et lambda_ij INTEGROS,
 * divisionibus exactis tantum. Idem effectus in omni machina.
 *
 * DUAE REGULAE (Fran, 2026-10-13; project-specs/reticulum-plan.md):
 * 1. nullus effectus approximatus pro exacto: computatio limitata
 *    dicit num perfecta sit (ReticulumEnumeratio.completa);
 * 2. certificata ubi vilia: reductio u reddit (reducta = u * basis,
 *    det u = +-1), verificabile sine hac bibliotheca; enumeratio
 *    ordine CANONICO (norma, deinde lexicographice; signum: coordinata
 *    prima non nulla positiva).
 *
 * DELTA: parametrum Lovasz exactum, 1/4 < delta <= 1; NIHIL = 99/100.
 *
 * DEFECTUS: FALSUM si anulus non Z, vectores dependentes (basis) aut
 * forma non definita positiva, dimensiones discrepant, delta extra
 * fines; exitus tunc non tanguntur. Functiones nondum factae (T2-T4
 * consilii) FALSUM semper reddunt - vide singulas.
 *
 * MEMORIA: temporaria in piscinis propriis, destructis; effecta SOLA in
 * piscina vocantis (reticulum_apex_officinarum metitur).
 *
 * USUS:
 *   Matrix b, mu;
 *   Fractio* normae;
 *   b32 reductum;
 *   (vacuum)matrix_ex_chorda(&ANULUS_INTEGRORUM,
 *       chorda_ex_literis("[1, 1, 1; -1, 0, 2; 3, 5, 6]", piscina),
 *       piscina, &b);
 *   si (reticulum_orthogonaliza(b, piscina, &mu, &normae)) ...
 *   si (reticulum_est_reductum(b, NIHIL, piscina, &reductum)) ...
 *
 * Vide lib/reticulum.worklog.md.
 */
/* <aedilis corpus="lib/reticulum.c"/> */
#ifndef RETICULUM_H
#define RETICULUM_H

#include "latina.h"
#include "piscina.h"
#include "magnus.h"
#include "fractio.h"
#include "matrix.h"


/* ==================================================
 * Gram-Schmidt et iudicium reductionis
 * ================================================== */

/* Gram-Schmidt exacte. coefficientes: n x n super ANULUS_RATIONALIUM,
 * triangularis inferior, diagonalis 1 (basis = coefficientes * basis*
 * ubi basis* orthogonalis); *normae: n fractiones |b*_i|^2 > 0 in
 * piscina (NIHIL si n == 0). Uterque exitus NIHIL esse potest. */
b32
reticulum_orthogonaliza (
      Matrix   basis,
     Piscina*  piscina,
      Matrix*  coefficientes,
     Fractio** normae);

/* idem ex forma (matrix Gram n x n) */
b32
reticulum_formam_orthogonaliza (
      Matrix   forma,
     Piscina*  piscina,
      Matrix*  coefficientes,
     Fractio** normae);

/* iudicium EXACTUM reductionis LLL: |mu_ij| <= 1/2 (j < i) et
 * |b*_i|^2 >= (delta - mu_i,i-1^2) |b*_i-1|^2. Oraculum probationum,
 * independens a via reductionis. */
b32
reticulum_est_reductum (
              Matrix  basis,
    constans Fractio* delta,
             Piscina* piscina,
                 b32* exitus);

b32
reticulum_formam_est_reducta (
              Matrix  forma,
    constans Fractio* delta,
             Piscina* piscina,
                 b32* exitus);


/* ==================================================
 * Reductio LLL (consilium T2 - nondum: FALSUM)
 * ================================================== */

nomen structura {
    i64 permutationes;          /* condicio Lovasz fracta, b_k <-> b_k-1 */
    i64 reductiones;            /* gradus reductionis magnitudinis */
} ReticulumStatistica;

/* reducta = u * basis, LLL-reducta cum delta; u unimodularis. u,
 * statistica NIHIL si non quaeruntur. */
b32
reticulum_reduce (
                 Matrix  basis,
       constans Fractio* delta,
                Piscina* piscina,
                 Matrix* reducta,
                 Matrix* u,
    ReticulumStatistica* statistica);

/* forma_reducta = u * forma * u^T, LLL-reducta cum delta */
b32
reticulum_formam_reduce (
                 Matrix  forma,
       constans Fractio* delta,
                Piscina* piscina,
                 Matrix* forma_reducta,
                 Matrix* u,
    ReticulumStatistica* statistica);


/* ==================================================
 * Enumeratio (consilium T3 - nondum: FALSUM)
 * ================================================== */

nomen structura {
    Magnus radius_quadratus;    /* 0 < |v|^2 <= radius_quadratus */
       i32 limes_servandi;      /* vectores servati maximi */
       i64 limes_nodorum;       /* sumptus arboris; 0 = sine limite */
} ReticulumQuaestio;

nomen structura {
       i64 numerus;            /* vectores intra radium, modulo signo;
                                 * si completa FALSUM, LIMES INFERIOR */
       b32 completa;           /* FALSUM: limes_nodorum attactus */
       i32 servati;
    Matrix vectores;           /* lineae: primi 'servati' ordine
                                 * canonico */
    Magnus* normae;             /* |v|^2 vectorum servatorum */
} ReticulumEnumeratio;

/* vectores v != 0 reticuli (coordinatae ambientis) */
b32
reticulum_enumera (
                        Matrix  basis,
    constans ReticulumQuaestio* quaestio,
                       Piscina* piscina,
           ReticulumEnumeratio* exitus);

/* vectores coefficientium x != 0 cum x^T forma x <= radius */
b32
reticulum_formam_enumera (
                        Matrix  forma,
    constans ReticulumQuaestio* quaestio,
                       Piscina* piscina,
           ReticulumEnumeratio* exitus);

/* vector brevissimus non nullus (primus canonice si plures) */
b32
reticulum_minimum (
      Matrix  basis,
     Piscina* piscina,
      Matrix* vector,
      Magnus* norma);


/* ==================================================
 * Relationes integrae (consilium T4 - nondum: FALSUM)
 * ================================================== */

/* basis LLL-reducta reticuli {x in Z^n : a x = 0}, relationes inter
 * COLUMNAS a, lineae exitus */
b32
reticulum_relationes (
              Matrix  a,
    constans Fractio* delta,
             Piscina* piscina,
              Matrix* exitus);


/* ==================================================
 * Diagnosis
 * ================================================== */

/* maximus usus (octeti) piscinarum temporariarum in ultima vocatione;
 * computator sumptus deterministicus */
memoriae_index
reticulum_apex_officinarum (
    vacuum);

#endif /* RETICULUM_H */
