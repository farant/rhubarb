/* quaternio.h - Quaterniones super quemlibet anulum (commutativum)
 *
 * q = a + b i + c j + d k, i^2 = j^2 = k^2 = ijk = -1 (Hamilton).
 * Partes elementa anuli dati (Anulus): Z (quaterniones Lipschitz), Q,
 * Q(sqrt 2), Q(sqrt 5) (icosiani), ... Valores immutabiles, effectus in
 * piscina data; anuli mixti refutantur (FALSUM), sicut matrix.
 *
 * ROTATIONES INTRA ANULUM: q v conj(q) = N(q) R(v), ergo quaternio
 * integer rotationem cum matrice integra dat sine divisione;
 * quaternio_matrix N(q) R reddit. Divisio (inversum) solum super
 * corpus.
 *
 * GEOMETRIA EXACTA sine radicibus quadratis: eadem rotatio (p = s q),
 * eadem axis (partes vectoriae parallelae) per anulum solum; angulus
 * rotationis (cos^2(theta/2) = a^2 / N) et directio proxima (cellula
 * Voronoi) per quadrata comparantur et ORDINEM anuli postulant
 * (anulus->signum; NIHIL -> FALSUM).
 *
 * USUS:
 *   constans Anulus* q = &ANULUS_RATIONALIUM;
 *   Quaternio h;
 *   (vacuum)quaternio_ex_chorda(q, chorda_ex_literis(
 *       "[1/2, 1/2, 1/2, 1/2]", piscina), piscina, &h);
 *       (* unitas Hurwitz: rotatio 120 gradus *)
 *
 * Vide lib/quaternio.worklog.md.
 */
/* <aedilis corpus="lib/quaternio.c"/> */
#ifndef QUATERNIO_H
#define QUATERNIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "anulus.h"
#include "matrix.h"

/* PRIVATUM - per functiones legendum. partes: IV elementa anuli
 * (mensura anulus->mensura), ordine a, b, c, d. */
nomen structura {
    constans Anulus* anulus;
                 i8* partes;
} Quaternio;


/* ==================================================
 * Creatio et lectio
 * ================================================== */

/* a + b i + c j + d k (elementa copiantur); FALSUM si anulus NIHIL */
b32
quaternio_ex_partibus (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
    constans vacuum* c,
    constans vacuum* d,
            Piscina* piscina,
          Quaternio* exitus);

b32
quaternio_nullum (
    constans Anulus* anulus,
            Piscina* piscina,
          Quaternio* exitus);

b32
quaternio_unum (
    constans Anulus* anulus,
            Piscina* piscina,
          Quaternio* exitus);

/* pars index: 0 = a, 1 = i, 2 = j, 3 = k; NIHIL si index > 3 */
constans vacuum*
quaternio_pars (
    Quaternio q,
          i32 index);

constans Anulus*
quaternio_anulus (
    Quaternio q);


/* ==================================================
 * Arithmetica (FALSUM si anuli mixti aut anulus refutat)
 * ================================================== */

b32
quaternio_adde (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina,
    Quaternio* exitus);

b32
quaternio_subtrahe (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina,
    Quaternio* exitus);

/* p q (Hamilton; non commutativum) */
b32
quaternio_multiplica (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina,
    Quaternio* exitus);

/* s q, s elementum anuli */
b32
quaternio_scalari (
           Quaternio  q,
     constans vacuum* s,
             Piscina* piscina,
           Quaternio* exitus);

/* a - b i - c j - d k */
b32
quaternio_conjugatum (
    Quaternio  q,
      Piscina* piscina,
    Quaternio* exitus);

/* N(q) = a^2 + b^2 + c^2 + d^2 in anulo (exitus: elementum anuli) */
b32
quaternio_norma (
    Quaternio  q,
      Piscina* piscina,
       vacuum* exitus);

/* conj(q) / N(q): solum corpus; FALSUM si N(q) = 0 aut non corpus */
b32
quaternio_inversum (
    Quaternio  q,
      Piscina* piscina,
    Quaternio* exitus);

/* FALSUM si anuli diversi */
b32
quaternio_aequalis (
    Quaternio p,
    Quaternio q);

b32
quaternio_est_nullum (
    Quaternio q);


/* ==================================================
 * Rotationes (intra anulum)
 * ================================================== */

/* q v conj(q) = N(q) R(v) */
b32
quaternio_rotare (
    Quaternio  q,
    Quaternio  v,
      Piscina* piscina,
    Quaternio* exitus);

/* matrix 3x3 N(q) R super eundem anulum (columnae: imagines i, j, k) */
b32
quaternio_matrix (
    Quaternio  q,
      Piscina* piscina,
       Matrix* exitus);


/* ==================================================
 * Geometria exacta
 * ================================================== */

/* p = s q pro scalari s (p, q non nulli): eadem rotatio (etiam -q) */
b32
quaternio_eadem_rotatio (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina);

/* partes vectoriae (b, c, d) non nullae et parallelae (utraque
 * directio): eadem axis */
b32
quaternio_eadem_axis (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina);

/* signum(angulus(p) - angulus(q)), angulus rotationis in [0, pi]:
 * cos^2(theta/2) = a^2 / N. ORDINEM postulat; FALSUM si signum anuli
 * NIHIL, quaternio nullus, aut anuli mixti. */
b32
quaternio_compara_angulum (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina,
          s32* exitus);

/* directio proxima partis vectoriae v inter directiones (partes
 * vectoriae): maximum cos anguli; antipodes_idem: |cos| (axis sine
 * directione). Index primus in paritate. ORDINEM postulat; FALSUM si
 * signum NIHIL, numerus 0, v aut directio vectore nullo, anuli
 * mixti. */
b32
quaternio_proximus (
             Quaternio  v,
    constans Quaternio* directiones,
                   i32  numerus,
                   b32  antipodes_idem,
               Piscina* piscina,
                   i32* index);


/* ==================================================
 * Textus
 * ================================================== */

/* "[a, b, c, d]" partes per anulum (anulus->ad_chordam) */
chorda
quaternio_ad_chordam (
    Quaternio  q,
      Piscina* piscina);

/* "[a, b, c, d]" (spatia libera); FALSUM si malformatum aut pars
 * refutata ab anulo */
b32
quaternio_ex_chorda (
     constans Anulus* anulus,
              chorda  textus,
             Piscina* piscina,
           Quaternio* exitus);

#endif /* QUATERNIO_H */
