/*
 * stereometria.h - mathematica spatii: vectores, quaterniones, matrices
 *
 * Fundamentum simulacrorum (project-specs/simulacra-spec.md IV.1):
 * vector trium f32, quaternio, matrix IV x IV. Functiones purae,
 * valores per copiam; nihil de simulacris, plagulis aut pixelis scit.
 *
 * CONVENTIONES (semel hic, ubique servatae):
 *   - manus dextera; +y sursum; oculus per -z spectat
 *   - matrix columnis primum: e[columna * IV + ordo]; translatio in
 *     e[XII], e[XIII], e[XIV]
 *   - vector columna est: p' = M * p. In a * b, b PRIMUM applicatur
 *   - anguli in radianis
 *   - quaternio (x, y, z, w), w pars scalaris (ordo glTF)
 *
 * matrix.h et quaternio.h domus algebra exacta sunt (Z, Q), non haec.
 */

#ifndef STEREOMETRIA_H
#define STEREOMETRIA_H

/* <aedilis corpus="lib/stereometria.c"/> */

#include "latina.h"

#define STEREOMETRIA_PI 3.14159265358979323846f

nomen structura {
    f32 x;
    f32 y;
    f32 z;
} StereometriaVector;

nomen structura {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} StereometriaQuaternio;

nomen structura {
    f32 e[XVI];
} StereometriaMatrix;


/* ==================================================
 * Vectores
 * ================================================== */

StereometriaVector
stereometria_vector (
    f32 x,
    f32 y,
    f32 z);

StereometriaVector
stereometria_addere (
    StereometriaVector a,
    StereometriaVector b);

/* a - b */
StereometriaVector
stereometria_subtrahere (
    StereometriaVector a,
    StereometriaVector b);

StereometriaVector
stereometria_scalare (
    StereometriaVector a,
                   f32 s);

/* productum scalare (a . b) */
f32
stereometria_productum (
    StereometriaVector a,
    StereometriaVector b);

/* productum vectoriale (a x b): x crux y = z */
StereometriaVector
stereometria_crux (
    StereometriaVector a,
    StereometriaVector b);

f32
stereometria_longitudo (
    StereometriaVector a);

/* longitudo I; vector nullus manet nullus (numquam per 0 dividit) */
StereometriaVector
stereometria_normalizare (
    StereometriaVector a);

/* a + (b - a) * t; t extra [0, 1] extrapolat */
StereometriaVector
stereometria_miscere (
    StereometriaVector a,
    StereometriaVector b,
                   f32 t);


/* ==================================================
 * Quaterniones
 * ================================================== */

/* (0, 0, 0, 1) */
StereometriaQuaternio
stereometria_quaternio_identitas (
    vacuum);

/* longitudo I; quaternio nullus fit identitas */
StereometriaQuaternio
stereometria_quaternio_normalizare (
    StereometriaQuaternio q);

/* interpolatio sphaerica, via brevior (b invertitur si a . b < 0);
 * prope parallelos (a . b > 0.9995) linealiter miscet et normalizat.
 * t = 0 -> a, t = 1 -> b (aut -b, rotatio eadem) */
StereometriaQuaternio
stereometria_quaternio_slerp (
    StereometriaQuaternio a,
    StereometriaQuaternio b,
                      f32 t);


/* ==================================================
 * Matrices
 * ================================================== */

StereometriaMatrix
stereometria_identitas (
    vacuum);

/* a * b: b primum applicatur */
StereometriaMatrix
stereometria_multiplicare (
    StereometriaMatrix a,
    StereometriaMatrix b);

StereometriaMatrix
stereometria_translatio (
    StereometriaVector t);

/* rotatio circa axem x: +y versus +z it */
StereometriaMatrix
stereometria_rotatio_x (
    f32 angulus);

/* rotatio circa axem y: +x versus -z it */
StereometriaMatrix
stereometria_rotatio_y (
    f32 angulus);

/* translatio * rotatio * scala (scala prima applicatur); rotatio
 * normalizata exspectatur */
StereometriaMatrix
stereometria_ex_trs (
       StereometriaVector translatio,
    StereometriaQuaternio rotatio,
       StereometriaVector scala);

/* matrix generalis in translationem, rotationem, scalam resoluta
 * (nodus glTF cum 'matrix'). Scala ex longitudinibus columnarum;
 * determinans negativus scalam x negat. Columna nulla: scala 0,
 * rotatio identitas. Obliquitas (shear) amittitur. */
vacuum
stereometria_in_trs (
       StereometriaMatrix  m,
       StereometriaVector* translatio,
    StereometriaQuaternio* rotatio,
       StereometriaVector* scala);

/* perspectiva: campus_y angulus verticalis totus, ratio = latitudo /
 * altitudo, 0 < prope < procul. Post divisionem per w: z = -prope
 * fit -1, z = -procul fit +1; x et y visibilia in [-1, 1] */
StereometriaMatrix
stereometria_perspectiva (
    f32 campus_y,
    f32 ratio,
    f32 prope,
    f32 procul);

/* punctum (w = 1) transformatum; *w = w exitus, NON divisum (vocator
 * dividit - perspectiva). w NIHIL licet */
StereometriaVector
stereometria_punctum_transformare (
    StereometriaMatrix  m,
    StereometriaVector  p,
                   f32* w);

/* directio (w = 0): sine translatione */
StereometriaVector
stereometria_directionem_transformare (
    StereometriaMatrix m,
    StereometriaVector d);

#endif /* STEREOMETRIA_H */
