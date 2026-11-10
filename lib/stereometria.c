/* stereometria.c - mathematica spatii (conventiones: stereometria.h) */

#include "stereometria.h"
#include <math.h>

/* cos anguli supra quem slerp linealiter miscet */
#define SLERP_LIMEN 0.9995f

/* longitudo infra quam vector aut columna nulla habetur */
#define MINIMUM 0.00000001f


/* ==================================================
 * Vectores
 * ================================================== */

StereometriaVector
stereometria_vector (
    f32 x,
    f32 y,
    f32 z)
{
    StereometriaVector v;

    v.x = x;
    v.y = y;
    v.z = z;
    redde v;
}

StereometriaVector
stereometria_addere (
    StereometriaVector a,
    StereometriaVector b)
{
    redde stereometria_vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

StereometriaVector
stereometria_subtrahere (
    StereometriaVector a,
    StereometriaVector b)
{
    redde stereometria_vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

StereometriaVector
stereometria_scalare (
    StereometriaVector a,
                   f32 s)
{
    redde stereometria_vector(a.x * s, a.y * s, a.z * s);
}

f32
stereometria_productum (
    StereometriaVector a,
    StereometriaVector b)
{
    redde a.x * b.x + a.y * b.y + a.z * b.z;
}

StereometriaVector
stereometria_crux (
    StereometriaVector a,
    StereometriaVector b)
{
    redde stereometria_vector(a.y * b.z - a.z * b.y, a.z * b.x
        - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

f32
stereometria_longitudo (
    StereometriaVector a)
{
    redde (f32)sqrt((f64)stereometria_productum(a, a));
}

StereometriaVector
stereometria_normalizare (
    StereometriaVector a)
{
    f32 l = stereometria_longitudo(a);

    si (l < MINIMUM)
    {
        redde stereometria_vector(0.0f, 0.0f, 0.0f);
    }
    redde stereometria_scalare(a, 1.0f / l);
}

StereometriaVector
stereometria_miscere (
    StereometriaVector a,
    StereometriaVector b,
                   f32 t)
{
    redde stereometria_vector(a.x + (b.x - a.x) * t, a.y + (b.y
        - a.y) * t,
        a.z + (b.z - a.z) * t);
}


/* ==================================================
 * Quaterniones
 * ================================================== */

StereometriaQuaternio
stereometria_quaternio_identitas (
    vacuum)
{
    StereometriaQuaternio q;

    q.x = 0.0f;
    q.y = 0.0f;
    q.z = 0.0f;
    q.w = 1.0f;
    redde q;
}

StereometriaQuaternio
stereometria_quaternio_normalizare (
    StereometriaQuaternio q)
{
    f32 l;

    l = (f32)sqrt((f64)(q.x * q.x + q.y * q.y + q.z * q.z
        + q.w * q.w));
    si (l < MINIMUM)
    {
        redde stereometria_quaternio_identitas();
    }
    l    = 1.0f / l;
    q.x  = q.x * l;
    q.y  = q.y * l;
    q.z  = q.z * l;
    q.w  = q.w * l;
    redde q;
}

StereometriaQuaternio
stereometria_quaternio_slerp (
    StereometriaQuaternio a,
    StereometriaQuaternio b,
                      f32 t)
{
    StereometriaQuaternio q;
                      f32 d;
                      f32 angulus;
                      f32 sinus;
                      f32 pa;
                      f32 pb;

    d = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    si (d < 0.0f)
    {
        d    = -d;
        b.x  = -b.x;
        b.y  = -b.y;
        b.z  = -b.z;
        b.w  = -b.w;
    }
    si (d > SLERP_LIMEN)
    {
        pa = 1.0f - t;
        pb = t;
    }
    alioquin
    {
        angulus  = (f32)acos((f64)d);
        sinus    = (f32)sin((f64)angulus);
        pa       = (f32)sin((f64)((1.0f - t) * angulus)) / sinus;
        pb       = (f32)sin((f64)(t * angulus)) / sinus;
    }
    q.x = a.x * pa + b.x * pb;
    q.y = a.y * pa + b.y * pb;
    q.z = a.z * pa + b.z * pb;
    q.w = a.w * pa + b.w * pb;
    redde stereometria_quaternio_normalizare(q);
}


/* ==================================================
 * Matrices
 * ================================================== */

StereometriaMatrix
stereometria_identitas (
    vacuum)
{
    StereometriaMatrix m;
                   i32 k;

    per (k = ZEPHYRUM; k < XVI; k++)
    {
        m.e[k] = (k % V == ZEPHYRUM) ? 1.0f : 0.0f;
    }
    redde m;
}

StereometriaMatrix
stereometria_multiplicare (
    StereometriaMatrix a,
    StereometriaMatrix b)
{
    StereometriaMatrix m;
                   i32 columna;
                   i32 ordo;

    per (columna = ZEPHYRUM; columna < IV; columna++)
    {
        per (ordo = ZEPHYRUM; ordo < IV; ordo++)
        {
            m.e[columna * IV + ordo] =
                  a.e[ordo] * b.e[columna * IV]
                + a.e[IV + ordo] * b.e[columna * IV + I]
                + a.e[VIII + ordo] * b.e[columna * IV + II]
                + a.e[XII + ordo] * b.e[columna * IV + III];
        }
    }
    redde m;
}

StereometriaMatrix
stereometria_translatio (
    StereometriaVector t)
{
    StereometriaMatrix m = stereometria_identitas();

    m.e[XII]   = t.x;
    m.e[XIII]  = t.y;
    m.e[XIV]   = t.z;
    redde m;
}

StereometriaMatrix
stereometria_rotatio_x (
    f32 angulus)
{
    StereometriaMatrix m   = stereometria_identitas();
                   f32 cs  = (f32)cos((f64)angulus);
                   f32 sn  = (f32)sin((f64)angulus);

    m.e[V]   = cs;
    m.e[VI]  = sn;
    m.e[IX]  = -sn;
    m.e[X]   = cs;
    redde m;
}

StereometriaMatrix
stereometria_rotatio_y (
    f32 angulus)
{
    StereometriaMatrix m   = stereometria_identitas();
                   f32 cs  = (f32)cos((f64)angulus);
                   f32 sn  = (f32)sin((f64)angulus);

    m.e[ZEPHYRUM]  = cs;
    m.e[II]        = -sn;
    m.e[VIII]      = sn;
    m.e[X]         = cs;
    redde m;
}

StereometriaMatrix
stereometria_ex_trs (
       StereometriaVector translatio,
    StereometriaQuaternio rotatio,
       StereometriaVector scala)
{
    StereometriaMatrix m    = stereometria_identitas();
                   f32 x_x  = rotatio.x * rotatio.x;
                   f32 y_y  = rotatio.y * rotatio.y;
                   f32 z_z  = rotatio.z * rotatio.z;
                   f32 x_y  = rotatio.x * rotatio.y;
                   f32 x_z  = rotatio.x * rotatio.z;
                   f32 y_z  = rotatio.y * rotatio.z;
                   f32 w_x  = rotatio.w * rotatio.x;
                   f32 w_y  = rotatio.w * rotatio.y;
                   f32 w_z  = rotatio.w * rotatio.z;

    m.e[ZEPHYRUM]  = (1.0f - 2.0f * (y_y + z_z)) * scala.x;
    m.e[I]         = (2.0f * (x_y + w_z)) * scala.x;
    m.e[II]        = (2.0f * (x_z - w_y)) * scala.x;
    m.e[IV]        = (2.0f * (x_y - w_z)) * scala.y;
    m.e[V]         = (1.0f - 2.0f * (x_x + z_z)) * scala.y;
    m.e[VI]        = (2.0f * (y_z + w_x)) * scala.y;
    m.e[VIII]      = (2.0f * (x_z + w_y)) * scala.z;
    m.e[IX]        = (2.0f * (y_z - w_x)) * scala.z;
    m.e[X]         = (1.0f - 2.0f * (x_x + y_y)) * scala.z;
    m.e[XII]       = translatio.x;
    m.e[XIII]      = translatio.y;
    m.e[XIV]       = translatio.z;
    redde m;
}

/* quaternio ex matrice rotationis pura (columnae a, b, d unitariae):
 * ramus maximae diagonalis, ne per parvum dividatur */
interior StereometriaQuaternio
_quaternio_ex_columnis (
    StereometriaVector a,
    StereometriaVector b,
    StereometriaVector d)
{
    StereometriaQuaternio q;
                      f32 vestigium = a.x + b.y + d.z;
                      f32 s;

    si (vestigium > 0.0f)
    {
        s    = (f32)sqrt((f64)(vestigium + 1.0f)) * 2.0f;
        q.w  = 0.25f * s;
        q.x  = (b.z - d.y) / s;
        q.y  = (d.x - a.z) / s;
        q.z  = (a.y - b.x) / s;
    }
    alioquin si (a.x > b.y && a.x > d.z)
    {
        s    = (f32)sqrt((f64)(1.0f + a.x - b.y - d.z)) * 2.0f;
        q.w  = (b.z - d.y) / s;
        q.x  = 0.25f * s;
        q.y  = (b.x + a.y) / s;
        q.z  = (d.x + a.z) / s;
    }
    alioquin si (b.y > d.z)
    {
        s    = (f32)sqrt((f64)(1.0f + b.y - a.x - d.z)) * 2.0f;
        q.w  = (d.x - a.z) / s;
        q.x  = (b.x + a.y) / s;
        q.y  = 0.25f * s;
        q.z  = (d.y + b.z) / s;
    }
    alioquin
    {
        s    = (f32)sqrt((f64)(1.0f + d.z - a.x - b.y)) * 2.0f;
        q.w  = (a.y - b.x) / s;
        q.x  = (d.x + a.z) / s;
        q.y  = (d.y + b.z) / s;
        q.z  = 0.25f * s;
    }
    redde stereometria_quaternio_normalizare(q);
}

vacuum
stereometria_in_trs (
       StereometriaMatrix  m,
       StereometriaVector* translatio,
    StereometriaQuaternio* rotatio,
       StereometriaVector* scala)
{
    StereometriaVector a = stereometria_vector(m.e[ZEPHYRUM], m.e[I],
        m.e[II]);
    StereometriaVector b = stereometria_vector(m.e[IV], m.e[V],
        m.e[VI]);
    StereometriaVector d = stereometria_vector(m.e[VIII], m.e[IX],
        m.e[X]);
    StereometriaVector s;

    s.x = stereometria_longitudo(a);
    s.y = stereometria_longitudo(b);
    s.z = stereometria_longitudo(d);
    /* speculum: determinans negativus -> scala x negatur */
    si (stereometria_productum(stereometria_crux(a, b), d) < 0.0f)
    {
        s.x = -s.x;
    }
    si (translatio)
    {
        *translatio = stereometria_vector(m.e[XII], m.e[XIII],
            m.e[XIV]);
    }
    si (scala)
    {
        *scala = s;
    }
    si (!rotatio)
    {
        redde;
    }
    si (   (s.x < MINIMUM && s.x > -MINIMUM)
        || s.y < MINIMUM
        || s.z < MINIMUM)
    {
        *rotatio = stereometria_quaternio_identitas();
        redde;
    }
    *rotatio = _quaternio_ex_columnis(stereometria_scalare(a, 1.0f
        / s.x),
        stereometria_scalare(b, 1.0f / s.y), stereometria_scalare(d,
        1.0f / s.z));
}

StereometriaMatrix
stereometria_perspectiva (
    f32 campus_y,
    f32 ratio,
    f32 prope,
    f32 procul)
{
    StereometriaMatrix m;
                   f32 f = 1.0f / (f32)tan((f64)campus_y * 0.5);
                   i32 k;

    per (k = ZEPHYRUM; k < XVI; k++)
    {
        m.e[k] = 0.0f;
    }
    m.e[ZEPHYRUM]  = f / ratio;
    m.e[V]         = f;
    m.e[X]         = (procul + prope) / (prope - procul);
    m.e[XI]        = -1.0f;
    m.e[XIV]       = 2.0f * procul * prope / (prope - procul);
    redde m;
}

StereometriaVector
stereometria_punctum_transformare (
    StereometriaMatrix  m,
    StereometriaVector  p,
                   f32* w)
{
    si (w)
    {
        *w = m.e[III] * p.x + m.e[VII] * p.y + m.e[XI] * p.z + m.e[XV];
    }
    redde stereometria_vector(
        m.e[ZEPHYRUM] * p.x + m.e[IV] * p.y + m.e[VIII] * p.z
            + m.e[XII],
        m.e[I] * p.x + m.e[V] * p.y + m.e[IX] * p.z + m.e[XIII],
        m.e[II] * p.x + m.e[VI] * p.y + m.e[X] * p.z + m.e[XIV]);
}

StereometriaVector
stereometria_directionem_transformare (
    StereometriaMatrix m,
    StereometriaVector d)
{
    redde stereometria_vector(
        m.e[ZEPHYRUM] * d.x + m.e[IV] * d.y + m.e[VIII] * d.z,
        m.e[I] * d.x + m.e[V] * d.y + m.e[IX] * d.z,
        m.e[II] * d.x + m.e[VI] * d.y + m.e[X] * d.z);
}
