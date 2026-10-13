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
 * DEFECTUS: FALSUM si anulus non Z, vectores dependentes (basis;
 * plures lineae quam columnae statim refutantur) aut forma non
 * definita positiva, dimensiones discrepant, delta extra fines; exitus
 * tunc non tanguntur. Q CONSULTO in reticulum_relationes solo (lineae
 * per denominatores multiplicari possunt sine damno); basis aut forma
 * super Q a vocante scalanda est (normae per quadratum scalae).
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
 * independens a via reductionis. piscina reservata (non adhibetur:
 * exitus b32 tantum). */
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
 * Reductio LLL integra (Cohen, Algorithmus 2.6.7)
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
 * Enumeratio (Fincke-Pohst integra)
 *
 * Vectores v != 0 cum |v|^2 <= radius, MODULO SIGNO (par {v, -v} semel,
 * repraesentans cuius coordinata prima non nulla positiva est). Ordo
 * canonicus: norma, deinde lexicographice ascendens. Super formam
 * LLL-reductam (delta 99/100) internam, in integris - sine
 * fractionibus et sine fluitantibus. Effectus COMPLETUS a basi data
 * non pendet (idem reticulum, idem effectus); nodi, et sub limite
 * nodorum numerus, completa et servati, autem pendent.
 * ================================================== */

nomen structura {
    Magnus radius_quadratus;    /* 0 < |v|^2 <= radius_quadratus; >= 0 */
       i32 limes_servandi;      /* vectores servati maximi (0: numerare
                                 * tantum) */
       i64 limes_nodorum;       /* nodi arboris (assignationes partiales
                                 * admissibiles); 0 = sine limite */
} ReticulumQuaestio;

nomen structura {
       i64 numerus;            /* vectores intra radium, modulo signo;
                                 * si completa FALSUM, LIMES INFERIOR */
       b32 completa;           /* FALSUM: limes_nodorum attactus (nodus
                                 * admissibilis non visitatus - numerus
                                 * iam verus esse potest) */
       i64 nodi;               /* nodi visitati: diagnosis, ab
                                 * implementatione pendet (sicut apex) */
       i32 servati;            /* min(limes_servandi, numerus) */
    Matrix vectores;           /* servati x columnae: primi ordine
                                 * canonico. Si completa FALSUM, primi
                                 * INTER INVENTOS tantum. */
    Magnus* normae;             /* |v|^2 vectorum servatorum; NIHIL si
                                 * servati == 0 */
} ReticulumEnumeratio;

/* vectores v != 0 reticuli, coordinatae ambientis (columnae basis) */
b32
reticulum_enumera (
                        Matrix  basis,
    constans ReticulumQuaestio* quaestio,
                       Piscina* piscina,
           ReticulumEnumeratio* exitus);

/* vectores coefficientium x != 0 cum x^T forma x <= radius, in
 * coordinatis formae DATAE (non reductae); normae = x^T forma x */
b32
reticulum_formam_enumera (
                        Matrix  forma,
    constans ReticulumQuaestio* quaestio,
                       Piscina* piscina,
           ReticulumEnumeratio* exitus);

/* vector brevissimus non nullus (primus canonice si plures): vector
 * 1 x columnae, norma |v|^2; uterque NIHIL esse potest. FALSUM si basis
 * vacua. */
b32
reticulum_minimum (
      Matrix  basis,
     Piscina* piscina,
      Matrix* vector,
      Magnus* norma);


/* ==================================================
 * Relationes integrae
 * ================================================== */

/* basis LLL-reducta (delta) reticuli TOTIUS {x in Z^n : a x = 0}:
 * relationes integrae inter COLUMNAS a, lineae exitus (n - gradus(a)
 * lineae; 0 x n si nullae). Omnis relatio integra combinatio integra
 * linearum est (saturata, non solum relationes rationales). a super Z
 * aut Q (Q: linea quaeque per denominatores multiplicatur, nucleus
 * idem). Relationes brevissimae: reticulum_enumera super exitum. */
b32
reticulum_relationes (
              Matrix  a,
    constans Fractio* delta,
             Piscina* piscina,
              Matrix* exitus);


/* ==================================================
 * Diagnosis
 * ================================================== */

/* usus piscinarum temporariarum (octeti) in ultima vocatione publica:
 * SUMMA apicum omnium - limes superior usus simul vivi, numquam minor
 * (compactio LLL: maximus veterum bis). Officinae matrix.h non
 * numerantur (matrix_apex_officinarum). Vocatio refutata: apex suus,
 * non prioris. Computator sumptus deterministicus. */
memoriae_index
reticulum_apex_officinarum (
    vacuum);

#endif /* RETICULUM_H */
