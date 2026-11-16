/*
 * plastica_membrum.c - membrum duorum ossium (plastica_membrum.h)
 */
#include "plastica_membrum.h"
#include <math.h>
#include <string.h>

#define RADIANS           (3.14159265358979323846 / 180.0)
#define NUMERUS_MAXIMUS   1.0e9
#define DISTANTIA_MINIMA  1.0e-6


PlasticaMembrumSolutio
plastica_membrum_solvere (
    f32 longitudo_prima,
    f32 longitudo_secunda,
    f32 meta_ante,
    f32 meta_infra,
    b32 flexus_ante)
{
    PlasticaMembrumSolutio s;
                       f64 prima    = (f64)longitudo_prima;
                       f64 secunda  = (f64)longitudo_secunda;
                       f64 ante     = (f64)meta_ante;
                       f64 infra    = (f64)meta_infra;
                       f64 distantia;
                       f64 cosinus;
                       f64 medium;
                       f64 apertura;
                       f64 directio;

    memset(&s, ZEPHYRUM, magnitudo(s));
    /* comparatio cum NaN semper falsa. Longitudo una infinita aut
     * meta infinita ambitu infra recusatur; ambae infinitae hic */
    si (   !(prima > 0.0) || !(secunda > 0.0)
        || !(prima + secunda < NUMERUS_MAXIMUS) || ante != ante
        || infra != infra)
    {
        redde s;
    }
    distantia = sqrt(ante * ante + infra * infra);
    si (   distantia < DISTANTIA_MINIMA
        || distantia > prima + secunda + (f64)PLASTICA_MEMBRUM_MARGO
        || distantia < fabs(prima - secunda)
            - (f64)PLASTICA_MEMBRUM_MARGO)
    {
        redde s;
    }
    /* flexus medii: lex cosinuum; 0 = rectum */
    cosinus = (distantia * distantia - prima * prima
        - secunda * secunda) / (2.0 * prima * secunda);
    medium  = acos(cosinus > 1.0 ? 1.0 : cosinus < -1.0 ? -1.0
        : cosinus);
    /* angulus inter os primum et lineam a radice ad metam */
    cosinus = (prima * prima + distantia * distantia
        - secunda * secunda) / (2.0 * prima * distantia);
    apertura = acos(cosinus > 1.0 ? 1.0 : cosinus < -1.0 ? -1.0
        : cosinus);
    /* directio metae a recto deorsum, antrorsum positiva */
    directio = atan2(ante, infra);
    si (flexus_ante)
    {
        s.angulus_radicis  = (f32)(-(directio + apertura) / RADIANS);
        s.angulus_medii    = (f32)(medium / RADIANS);
    }
    alioquin
    {
        s.angulus_radicis  = (f32)(-(directio - apertura) / RADIANS);
        s.angulus_medii    = (f32)(-medium / RADIANS);
    }
    s.successus = VERUM;
    redde s;
}
