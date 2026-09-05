/* derivare.c - Derivatio eventuum pura */

#include "derivare.h"


interior i32
abs_i32 (
    s32 v)
{
    redde (i32)(v < ZEPHYRUM ? -v : v);
}

vacuum
derivator_initiare (
    Derivator* d,
          s64  intervallum_ms,
          i32  distantia)
{
    d->tempus_ultimi   = ZEPHYRUM;
    d->ultimus.x       = ZEPHYRUM;
    d->ultimus.y       = ZEPHYRUM;
    d->habet_ultimum   = FALSUM;
    d->intervallum_ms  = intervallum_ms;
    d->distantia       = distantia;
}

/* <purus/> */
vacuum
derivare (
           Derivator* d,
    constans Eventus* ev,
                 Xar* effusio)
{
    Eventus* sedes;
        b32  est_geminus;

    sedes   = (Eventus*)xar_addere(effusio);
    *sedes  = *ev;

    si (ev->genus != EVENTUS_MUS_DEPRESSUS)
    { redde;
    }

    est_geminus = d->habet_ultimum
          && (ev->tempus - d->tempus_ultimi) <= d->intervallum_ms
          && abs_i32((s32)ev->datum.mus.x - (s32)d->ultimus.x)
              <= d->distantia
          && abs_i32((s32)ev->datum.mus.y - (s32)d->ultimus.y)
              <= d->distantia;

    si (est_geminus)
    {
        sedes             = (Eventus*)xar_addere(effusio);
        *sedes            = *ev;
        sedes->genus      = EVENTUS_MUS_DUPLEX;
        d->habet_ultimum  = FALSUM;   /* ne triplex fiat est_geminus */
    }
    alioquin
    {
        d->tempus_ultimi  = ev->tempus;
        d->ultimus.x      = ev->datum.mus.x;
        d->ultimus.y      = ev->datum.mus.y;
        d->habet_ultimum  = VERUM;
    }
}
