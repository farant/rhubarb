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
    d->armatus         = FALSUM;
    d->trahens         = FALSUM;
    d->origo.x         = ZEPHYRUM;
    d->origo.y         = ZEPHYRUM;
    d->botton          = MUS_SINISTER;
}

/* Derivatum post crudum: copia eventus, genere et bottone pressionis */
interior Eventus*
_derivatum_addere (
           Derivator* d,
    constans Eventus* ev,
     eventus_genus_t  genus,
                 Xar* effusio)
{
    Eventus* sedes;

    sedes                    = (Eventus*)xar_addere(effusio);
    *sedes                   = *ev;
    sedes->genus             = genus;
    sedes->datum.mus.botton  = d->botton;
    redde sedes;
}

/* Tractus (A5): limen ULTRA distantiam, axe utrovis */
interior vacuum
_tractum_derivare (
           Derivator* d,
    constans Eventus* ev,
                 Xar* effusio)
{
    Eventus* incipit;

    commutatio (ev->genus)
    {
        casus EVENTUS_MUS_DEPRESSUS:
            d->armatus   = VERUM;
            d->trahens   = FALSUM;
            d->origo.x   = ev->datum.mus.x;
            d->origo.y   = ev->datum.mus.y;
            d->botton    = ev->datum.mus.botton;
            frange;
        casus EVENTUS_MUS_MOTUS:
            si (d->trahens)
            {
                (vacuum)_derivatum_addere(d, ev, EVENTUS_TRACTUS,
                    effusio);
            }
            alioquin si (   d->armatus
                         && (   abs_i32(ev->datum.mus.x - d->origo.x)
                           > d->distantia
                         || abs_i32(ev->datum.mus.y - d->origo.y)
                           > d->distantia))
            {
                incipit = _derivatum_addere(d, ev,
                    EVENTUS_TRACTUS_INCIPIT, effusio);
                incipit->datum.mus.x                   = d->origo.x;
                incipit->datum.mus.y                   = d->origo.y;
                incipit->datum.mus.exempla             = NIHIL;
                incipit->datum.mus.numerus_exemplorum  = ZEPHYRUM;
                d->trahens                             = VERUM;
                /* tractus != ictus primus duplicis */
                d->habet_ultimum                       = FALSUM;
            }
            frange;
        casus EVENTUS_MUS_LIBERATUS:
            si (d->trahens)
            {
                (vacuum)_derivatum_addere(d, ev, EVENTUS_TRACTUS_FINIT,
                    effusio);
            }
            d->armatus  = FALSUM;
            d->trahens  = FALSUM;
            frange;
        ordinarius:
            frange;
    }
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
    {
        _tractum_derivare(d, ev, effusio);
        redde;
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
        d->tempus_ultimi      = ev->tempus;
                d->ultimus.x  = (s32)ev->datum.mus.x;
        d->ultimus.y          = (s32)ev->datum.mus.y;
        d->habet_ultimum      = VERUM;
    }
    _tractum_derivare(d, ev, effusio);
}
