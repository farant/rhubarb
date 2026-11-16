/* mundus.c - corpora in solo (vide mundus.h) */

#include "mundus.h"
#include <math.h>

/* gradus in radiano uno (180 / pi) */
#define GRADUS_IN_RADIO     57.29577951308232
#define LONGITUDO_MINIMA    0.000001f

b32
mundus_corpus_ponere (
    constans Mundus* m,
       MundusCorpus* c,
                f32  x,
                f32  z)
{
    f32 y = 0.0f;

    si (   !m || !c || !m->solum
        || !mundus_solum_stare(m->solum, x, z, c->radius)
        || !mundus_solum_altitudo(m->solum, x, z, &y))
    {
        redde FALSUM;
    }
    c->x               = x;
    c->z               = z;
    c->y               = y;
    c->movetur         = FALSUM;
    c->tempus_motus    = 0.0f;
    c->tempus_quietis  = 0.0f;
    redde VERUM;
}

/* angulum in [0, 360) cogere */
interior f32
_angulum_cogere (
    f32 angulus)
{
    angulus = angulus - 360.0f * (f32)floor((f64)(angulus / 360.0f));
    redde angulus >= 360.0f ? 0.0f : angulus;
}

/* faciem ad metam vertere, via breviore, non ultra 'maximum' gradus */
interior f32
_faciem_vertere (
    f32 facies,
    f32 meta,
    f32 maximum)
{
    f32 differentia = _angulum_cogere(meta - facies);

    si (differentia > 180.0f)
    {
        differentia -= 360.0f;
    }
    si (differentia > maximum)
    {
        differentia = maximum;
    }
    si (differentia < -maximum)
    {
        differentia = -maximum;
    }
    redde _angulum_cogere(facies + differentia);
}

/* corpus unum promovere */
interior vacuum
_corpus_gradi (
    constans MundusSolum* solum,
            MundusCorpus* c,
                     f32  dt)
{
    f32 longitudo;
    f32 directio_x;
    f32 directio_z;
    f32 restat;
    f32 passus;
    f32 y;
    b32 motum = FALSUM;

    longitudo = c->intendit
        ? (f32)sqrt((f64)(c->intentum_x * c->intentum_x
              + c->intentum_z * c->intentum_z))
        : 0.0f;
    si (!(longitudo > LONGITUDO_MINIMA) || !(dt > 0.0f))
    {
        c->movetur         = FALSUM;
        c->tempus_motus    = 0.0f;
        c->tempus_quietis  += dt;
        redde;
    }
    directio_x = c->intentum_x / longitudo;
    directio_z = c->intentum_z / longitudo;
    c->facies   = _faciem_vertere(c->facies,
        (f32)(atan2((f64)directio_x, (f64)directio_z)
            * GRADUS_IN_RADIO), MUNDUS_VERSURA * dt);
    restat = MUNDUS_CELERITAS * dt;
    dum (restat > LONGITUDO_MINIMA)
    {
        passus = restat < MUNDUS_PASSUS ? restat : MUNDUS_PASSUS;
        si (mundus_solum_licet(solum, c->x, c->z,
                c->x + directio_x * passus, c->z + directio_z * passus,
                c->radius))
        {
            c->x += directio_x * passus;
            c->z += directio_z * passus;
        }
        alioquin si (   directio_x != 0.0f
                     && mundus_solum_licet(solum, c->x, c->z,
                            c->x + directio_x * passus, c->z,
                            c->radius))
        {
            c->x += directio_x * passus;
        }
        alioquin si (   directio_z != 0.0f
                     && mundus_solum_licet(solum, c->x, c->z, c->x,
                            c->z + directio_z * passus, c->radius))
        {
            c->z += directio_z * passus;
        }
        alioquin
        {
            frange;
        }
        motum   = VERUM;
        restat  -= passus;
    }
    si (mundus_solum_altitudo(solum, c->x, c->z, &y))
    {
        c->y = y;
    }
    c->movetur         = motum;
    c->tempus_motus    = motum ? c->tempus_motus + dt : 0.0f;
    c->tempus_quietis  = motum ? 0.0f : c->tempus_quietis + dt;
}

vacuum
mundus_gradi (
    Mundus* m,
       f32  dt)
{
    i32 k;

    si (!m || !m->solum || !m->corpora)
    {
        redde;
    }
    si (!(dt > 0.0f))
    {
        dt = 0.0f;
    }
    si (dt > MUNDUS_GRADUS_MAXIMUS)
    {
        dt = MUNDUS_GRADUS_MAXIMUS;
    }
    per (k = ZEPHYRUM; k < m->corporum; k++)
    {
        _corpus_gradi(m->solum, &m->corpora[k], dt);
    }
}
