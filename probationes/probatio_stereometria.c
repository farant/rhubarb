/* probatio_stereometria.c - mathematica spatii (stereometria.h)
 *
 * vectores: crux dextera, productum, longitudo, normalizatio (nullus
 * manet nullus), mixtio. rotationes: y fert +x in -z, x fert +y in
 * +z. trs: scala prima, deinde rotatio, deinde translatio; aequat
 * multiplicationem. multiplicare: b primum. slerp: fines, medium, via
 * brevior, prope parallelos. perspectiva: prope -> -1, procul -> +1.
 * in_trs: compositum resolvitur in partes suas (et scala negativa).
 */
#include "latina.h"
#include "stereometria.h"
#include "credo.h"
#include <math.h>

#define EPSILON 0.00001f

#define CREDO_VECTOR(v, ex, ey, ez) \
    fac { \
        CREDO_F32_PROXIMUS ((v).x, (ex), EPSILON); \
        CREDO_F32_PROXIMUS ((v).y, (ey), EPSILON); \
        CREDO_F32_PROXIMUS ((v).z, (ez), EPSILON); \
    } dum (FALSUM)

/* rotatio anguli circa axem y */
interior StereometriaQuaternio
_circa_y (
    f32 angulus)
{
    StereometriaQuaternio q;

    q.x = 0.0f;
    q.y = (f32)sin((f64)angulus * 0.5);
    q.z = 0.0f;
    q.w = (f32)cos((f64)angulus * 0.5);
    redde q;
}

interior vacuum
_sectio_vectores (
    CredoContextus* c)
{
    StereometriaVector v;

    (vacuum)c;
    v = stereometria_crux(stereometria_vector(1.0f, 0.0f, 0.0f),
        stereometria_vector(0.0f, 1.0f, 0.0f));
    CREDO_VECTOR (v, 0.0f, 0.0f, 1.0f);
    v = stereometria_addere(stereometria_vector(1.0f, 2.0f, 3.0f),
        stereometria_vector(4.0f, 5.0f, 6.0f));
    CREDO_VECTOR (v, 5.0f, 7.0f, 9.0f);
    v = stereometria_subtrahere(stereometria_vector(1.0f, 2.0f, 3.0f),
        stereometria_vector(4.0f, 6.0f, 8.0f));
    CREDO_VECTOR (v, -3.0f, -4.0f, -5.0f);
    v = stereometria_scalare(stereometria_vector(1.0f, -2.0f, 3.0f),
        2.0f);
    CREDO_VECTOR (v, 2.0f, -4.0f, 6.0f);
    CREDO_F32_PROXIMUS (stereometria_productum(
        stereometria_vector(1.0f, 2.0f, 3.0f),
        stereometria_vector(4.0f, 5.0f, 6.0f)), 32.0f, EPSILON);
    CREDO_F32_PROXIMUS (stereometria_longitudo(
        stereometria_vector(3.0f, 4.0f, 12.0f)), 13.0f, EPSILON);
    v = stereometria_normalizare(stereometria_vector(0.0f, 3.0f, 4.0f));
    CREDO_VECTOR (v, 0.0f, 0.6f, 0.8f);
    v = stereometria_normalizare(stereometria_vector(0.0f, 0.0f, 0.0f));
    CREDO_VECTOR (v, 0.0f, 0.0f, 0.0f);
    v = stereometria_miscere(stereometria_vector(0.0f, 0.0f, 0.0f),
        stereometria_vector(2.0f, 4.0f, -8.0f), 0.25f);
    CREDO_VECTOR (v, 0.5f, 1.0f, -2.0f);
}

interior vacuum
_sectio_rotationes (
    CredoContextus* c)
{
    StereometriaVector v;
                   f32 w;

    (vacuum)c;
    v = stereometria_punctum_transformare(
        stereometria_rotatio_y(STEREOMETRIA_PI * 0.5f),
        stereometria_vector(1.0f, 0.0f, 0.0f), &w);
    CREDO_VECTOR (v, 0.0f, 0.0f, -1.0f);
    CREDO_F32_PROXIMUS (w, 1.0f, EPSILON);
    v = stereometria_punctum_transformare(
        stereometria_rotatio_x(STEREOMETRIA_PI * 0.5f),
        stereometria_vector(0.0f, 1.0f, 0.0f), NIHIL);
    CREDO_VECTOR (v, 0.0f, 0.0f, 1.0f);
    /* directio translationem neglegit, punctum non */
    v = stereometria_directionem_transformare(
        stereometria_translatio(stereometria_vector(5.0f, 6.0f, 7.0f)),
        stereometria_vector(1.0f, 0.0f, 0.0f));
    CREDO_VECTOR (v, 1.0f, 0.0f, 0.0f);
    v = stereometria_punctum_transformare(
        stereometria_translatio(stereometria_vector(5.0f, 6.0f, 7.0f)),
        stereometria_vector(1.0f, 0.0f, 0.0f), NIHIL);
    CREDO_VECTOR (v, 6.0f, 6.0f, 7.0f);
}

interior vacuum
_sectio_trs (
    CredoContextus* c)
{
    StereometriaMatrix m;
    StereometriaVector v;

    (vacuum)c;
    m = stereometria_ex_trs(stereometria_vector(1.0f, 2.0f, 3.0f),
        stereometria_quaternio_identitas(),
        stereometria_vector(2.0f, 2.0f, 2.0f));
    v = stereometria_punctum_transformare(m,
        stereometria_vector(1.0f, 1.0f, 1.0f), NIHIL);
    CREDO_VECTOR (v, 3.0f, 4.0f, 5.0f);
    /* scala, deinde rotatio (y, 90 gradus), deinde translatio:
     * (1,0,0) -> (2,0,0) -> (0,0,-2) -> (10,0,-2) */
    m = stereometria_ex_trs(stereometria_vector(10.0f, 0.0f, 0.0f),
        _circa_y(STEREOMETRIA_PI * 0.5f),
        stereometria_vector(2.0f, 3.0f, 4.0f));
    v = stereometria_punctum_transformare(m,
        stereometria_vector(1.0f, 0.0f, 0.0f), NIHIL);
    CREDO_VECTOR (v, 10.0f, 0.0f, -2.0f);
    /* (0,0,1) -> (0,0,4) -> (4,0,0) -> (14,0,0) */
    v = stereometria_punctum_transformare(m,
        stereometria_vector(0.0f, 0.0f, 1.0f), NIHIL);
    CREDO_VECTOR (v, 14.0f, 0.0f, 0.0f);
}

interior vacuum
_sectio_multiplicare (
    CredoContextus* c)
{
    StereometriaMatrix m;
    StereometriaMatrix r;
    StereometriaVector v;
                   i32 k;

    (vacuum)c;
    /* translatio * rotatio: rotatio PRIMA */
    m = stereometria_multiplicare(
        stereometria_translatio(stereometria_vector(1.0f, 0.0f, 0.0f)),
        stereometria_rotatio_y(STEREOMETRIA_PI * 0.5f));
    v = stereometria_punctum_transformare(m,
        stereometria_vector(1.0f, 0.0f, 0.0f), NIHIL);
    CREDO_VECTOR (v, 1.0f, 0.0f, -1.0f);
    /* ordo inversus: translatio prima -> (2,0,0) -> (0,0,-2) */
    r = stereometria_rotatio_y(STEREOMETRIA_PI * 0.5f);
    m = stereometria_multiplicare(r,
        stereometria_translatio(stereometria_vector(1.0f, 0.0f, 0.0f)));
    v = stereometria_punctum_transformare(m,
        stereometria_vector(1.0f, 0.0f, 0.0f), NIHIL);
    CREDO_VECTOR (v, 0.0f, 0.0f, -2.0f);
    /* identitas neutra */
    r = stereometria_rotatio_x(0.3f);
    m = stereometria_multiplicare(stereometria_identitas(), r);
    per (k = ZEPHYRUM; k < XVI; k++)
    {
        CREDO_F32_PROXIMUS (m.e[k], r.e[k], EPSILON);
    }
    /* translatio in e[XII..XIV] */
    m = stereometria_translatio(stereometria_vector(7.0f, 8.0f, 9.0f));
    CREDO_F32_PROXIMUS (m.e[XII], 7.0f, EPSILON);
    CREDO_F32_PROXIMUS (m.e[XIII], 8.0f, EPSILON);
    CREDO_F32_PROXIMUS (m.e[XIV], 9.0f, EPSILON);
}

interior vacuum
_sectio_slerp (
    CredoContextus* c)
{
    StereometriaQuaternio a;
    StereometriaQuaternio b;
    StereometriaQuaternio q;
    StereometriaQuaternio medius;

    (vacuum)c;
    a       = stereometria_quaternio_identitas();
    b       = _circa_y(STEREOMETRIA_PI * 0.5f);
    medius  = _circa_y(STEREOMETRIA_PI * 0.25f);
    q       = stereometria_quaternio_slerp(a, b, 0.0f);
    CREDO_F32_PROXIMUS (q.y, a.y, EPSILON);
    CREDO_F32_PROXIMUS (q.w, a.w, EPSILON);
    q = stereometria_quaternio_slerp(a, b, 1.0f);
    CREDO_F32_PROXIMUS (q.y, b.y, EPSILON);
    CREDO_F32_PROXIMUS (q.w, b.w, EPSILON);
    q = stereometria_quaternio_slerp(a, b, 0.5f);
    CREDO_F32_PROXIMUS (q.x, 0.0f, EPSILON);
    CREDO_F32_PROXIMUS (q.y, medius.y, EPSILON);
    CREDO_F32_PROXIMUS (q.z, 0.0f, EPSILON);
    CREDO_F32_PROXIMUS (q.w, medius.w, EPSILON);
    /* via brevior: ad -b eadem rotatio media */
    b.y  = -b.y;
    b.w  = -b.w;
    q    = stereometria_quaternio_slerp(a, b, 0.5f);
    CREDO_F32_PROXIMUS (q.y, medius.y, EPSILON);
    CREDO_F32_PROXIMUS (q.w, medius.w, EPSILON);
    /* q ad -q: manet q */
    b    = _circa_y(1.0f);
    a.x  = -b.x;
    a.y  = -b.y;
    a.z  = -b.z;
    a.w  = -b.w;
    q    = stereometria_quaternio_slerp(b, a, 0.5f);
    CREDO_F32_PROXIMUS (q.y, b.y, EPSILON);
    CREDO_F32_PROXIMUS (q.w, b.w, EPSILON);
    /* prope parallelos: longitudo I manet */
    a = _circa_y(0.5f);
    b = _circa_y(0.5001f);
    q = stereometria_quaternio_slerp(a, b, 0.5f);
    CREDO_F32_PROXIMUS (q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w,
        1.0f, EPSILON);
    CREDO_F32_PROXIMUS (q.y, _circa_y(0.50005f).y, EPSILON);
    /* normalizare: nullus fit identitas */
    a.x  = 0.0f;
    a.y  = 0.0f;
    a.z  = 0.0f;
    a.w  = 0.0f;
    q    = stereometria_quaternio_normalizare(a);
    CREDO_F32_PROXIMUS (q.w, 1.0f, EPSILON);
    a.w  = 2.0f;
    a.y  = 2.0f;
    q    = stereometria_quaternio_normalizare(a);
    CREDO_F32_PROXIMUS (q.y, 0.70710678f, EPSILON);
    CREDO_F32_PROXIMUS (q.w, 0.70710678f, EPSILON);
}

interior vacuum
_sectio_perspectiva (
    CredoContextus* c)
{
    StereometriaMatrix m;
    StereometriaVector v;
                   f32 w;

    (vacuum)c;
    /* campus 90 gradus, ratio II: tan(45) = 1 */
    m = stereometria_perspectiva(STEREOMETRIA_PI * 0.5f, 2.0f, 1.0f,
        10.0f);
    v = stereometria_punctum_transformare(m,
        stereometria_vector(0.0f, 0.0f, -1.0f), &w);
    CREDO_F32_PROXIMUS (w, 1.0f, EPSILON);
    CREDO_F32_PROXIMUS (v.z / w, -1.0f, EPSILON);
    v = stereometria_punctum_transformare(m,
        stereometria_vector(0.0f, 0.0f, -10.0f), &w);
    CREDO_F32_PROXIMUS (w, 10.0f, EPSILON);
    CREDO_F32_PROXIMUS (v.z / w, 1.0f, 0.0001f);
    /* margo superior et dexter ad distantiam V: y = 5, x = 10 */
    v = stereometria_punctum_transformare(m,
        stereometria_vector(10.0f, 5.0f, -5.0f), &w);
    CREDO_F32_PROXIMUS (v.x / w, 1.0f, EPSILON);
    CREDO_F32_PROXIMUS (v.y / w, 1.0f, EPSILON);
}

interior vacuum
_sectio_in_trs (
    CredoContextus* c)
{
       StereometriaMatrix m;
       StereometriaVector t;
       StereometriaVector s;
       StereometriaVector v;
    StereometriaQuaternio r;
    StereometriaQuaternio q;

    (vacuum)c;
    q = _circa_y(0.7f);
    m = stereometria_ex_trs(stereometria_vector(1.0f, -2.0f, 3.0f), q,
        stereometria_vector(2.0f, 3.0f, 0.5f));
    stereometria_in_trs(m, &t, &r, &s);
    CREDO_VECTOR (t, 1.0f, -2.0f, 3.0f);
    CREDO_VECTOR (s, 2.0f, 3.0f, 0.5f);
    CREDO_F32_PROXIMUS (r.x, q.x, EPSILON);
    CREDO_F32_PROXIMUS (r.y, q.y, EPSILON);
    CREDO_F32_PROXIMUS (r.z, q.z, EPSILON);
    CREDO_F32_PROXIMUS (r.w, q.w, EPSILON);
    /* rotatio ultra 180 gradus (vestigium negativum): recomposita
     * eadem puncta fert */
    q.x = 0.8f;
    q.y = 0.0f;
    q.z = 0.6f;
    q.w = 0.0f;
    m    = stereometria_ex_trs(stereometria_vector(0.0f, 0.0f, 0.0f), q,
        stereometria_vector(1.0f, 1.0f, 1.0f));
    stereometria_in_trs(m, &t, &r, &s);
    v = stereometria_punctum_transformare(stereometria_ex_trs(t, r, s),
        stereometria_vector(1.0f, 2.0f, 3.0f), NIHIL);
    CREDO_VECTOR (v,
        stereometria_punctum_transformare(m,
            stereometria_vector(1.0f, 2.0f, 3.0f), NIHIL).x,
        stereometria_punctum_transformare(m,
            stereometria_vector(1.0f, 2.0f, 3.0f), NIHIL).y,
        stereometria_punctum_transformare(m,
            stereometria_vector(1.0f, 2.0f, 3.0f), NIHIL).z);
    /* speculum (determinans negativus): scala x negatur, recomposita
     * aequat */
    m = stereometria_ex_trs(stereometria_vector(0.0f, 0.0f, 0.0f),
        _circa_y(0.4f), stereometria_vector(-1.0f, 2.0f, 3.0f));
    stereometria_in_trs(m, &t, &r, &s);
    CREDO_VECTOR (s, -1.0f, 2.0f, 3.0f);
    v = stereometria_punctum_transformare(stereometria_ex_trs(t, r, s),
        stereometria_vector(1.0f, 1.0f, 1.0f), NIHIL);
    CREDO_VECTOR (v,
        stereometria_punctum_transformare(m,
            stereometria_vector(1.0f, 1.0f, 1.0f), NIHIL).x,
        stereometria_punctum_transformare(m,
            stereometria_vector(1.0f, 1.0f, 1.0f), NIHIL).y,
        stereometria_punctum_transformare(m,
            stereometria_vector(1.0f, 1.0f, 1.0f), NIHIL).z);
    /* columna nulla: scala 0, rotatio identitas */
    m = stereometria_ex_trs(stereometria_vector(4.0f, 5.0f, 6.0f),
        _circa_y(0.4f), stereometria_vector(0.0f, 2.0f, 3.0f));
    stereometria_in_trs(m, &t, &r, &s);
    CREDO_VECTOR (t, 4.0f, 5.0f, 6.0f);
    CREDO_F32_PROXIMUS (s.x, 0.0f, EPSILON);
    CREDO_F32_PROXIMUS (r.w, 1.0f, EPSILON);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "vectores", _sectio_vectores, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rotationes", _sectio_rotationes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "trs", _sectio_trs, NIHIL, NIHIL, NIHIL, NIHIL },
    { "multiplicare", _sectio_multiplicare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "slerp", _sectio_slerp, NIHIL, NIHIL, NIHIL, NIHIL },
    { "perspectiva", _sectio_perspectiva, NIHIL, NIHIL, NIHIL, NIHIL },
    { "in_trs", _sectio_in_trs, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("stereometria", SECTIONES);
}
