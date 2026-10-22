/* sonus.c - Primitiva soni (vide include/sonus.h)
 *
 * Arithmetica f32 per exemplum; f64 solum ubi parametra in statum
 * convertuntur (incrementum phasis, coefficiens coli). Vide
 * lib/sonus.worklog.md.
 */
#include "sonus.h"
#include <math.h>

/* 2^32: cyclus unus phasis */
#define SONUS_CYCLUS     4294967296.0
#define SONUS_DUO_PI     6.28318530717958647692
/* strepitus: valor novus quoties tres bita summa phasis mutantur */
#define SONUS_PARS_OCTAVA(p)  ((p) >> XXIX)

interior f32
_coercere (
    f32 v,
    f32 imum,
    f32 summum)
{
    si (v < imum)
    {
        redde imum;
    }
    si (v > summum)
    {
        redde summum;
    }
    redde v;
}

interior f32
_strepitum_trahere (
    Sors* s)
{
    redde sors_f32(s) * 2.0f - 1.0f;
}


    /* ==================================================
     * UNDA
     * ================================================== */

vacuum
sonus_unda_parare (
          SonusUnda* u,
    SonusFormaUndae  forma,
                i64  semen)
{
    u->forma        = forma;
    u->phasis       = ZEPHYRUM;
    u->incrementum  = ZEPHYRUM;
    u->officium     = 0x80000000U;
    sors_seminare(&u->sors, semen, ZEPHYRUM);
    u->retentum    = 0.0f;
    si (forma == SONUS_UNDA_STREPITUS)
    {
        u->retentum = _strepitum_trahere(&u->sors);
    }
}

vacuum
sonus_unda_frequentiam_ponere (
    SonusUnda* u,
          f32  frequentia)
{
    f64 f = (f64)_coercere(frequentia, 0.0f,
                           (f32)SONUS_FREQUENTIA / 2.0f);

    /* rotundatum: CDXL cycli/s -> CDXL circumvolutiones in secundo,
     * non CDXXXIX (truncatio inferius cadit) */
    u->incrementum = (i32)(f / (f64)SONUS_FREQUENTIA * SONUS_CYCLUS
        + 0.5);
}

vacuum
sonus_unda_officium_ponere (
    SonusUnda* u,
          f32  ratio)
{
    u->officium = (i32)((f64)_coercere(ratio, 0.01f,
        0.99f) * SONUS_CYCLUS);
}

f32
sonus_unda_proximum (
    SonusUnda* u)
{
    i32 ante = u->phasis;
    f32 p;
    f32 x;

    si (u->incrementum == ZEPHYRUM)
    {
        redde 0.0f;
    }
    u->phasis  = ante + u->incrementum;   /* circumvolutio insignata */
    p          = (f32)((f64)ante / SONUS_CYCLUS);

    commutatio (u->forma)
    {
        casus SONUS_UNDA_PULSUS:
            x = ante < u->officium ? 1.0f : -1.0f;
            frange;
        casus SONUS_UNDA_SERRA:
            x = 2.0f * p - 1.0f;
            frange;
        casus SONUS_UNDA_TRIANGULUM:
            x = p < 0.5f ? 4.0f * p - 1.0f : 3.0f - 4.0f * p;
            frange;
        casus SONUS_UNDA_SINUS:
            x = (f32)sin(SONUS_DUO_PI * (f64)p);
            frange;
        casus SONUS_UNDA_STREPITUS:
            x = u->retentum;
            si (SONUS_PARS_OCTAVA(ante) != SONUS_PARS_OCTAVA(u->phasis))
            {
                u->retentum = _strepitum_trahere(&u->sors);
            }
            frange;
        ordinarius:
            x = 0.0f;
            frange;
    }
    redde x;
}


    /* ==================================================
     * INVOLUCRUM
     *
     * Impetus et lapsus = CLIVI fixi (impetus 1/n per exemplum), ergo
     * repercussio a 0.25 citius ad 1 pervenit. Remissio = TEMPUS fixum
     * a valore currente (valor/n), ergo remissio semper 'remissio'
     * secundas durat.
     * ================================================== */

/* mutatio per exemplum ut 'amplitudo' in 'secundae' transeat;
 * secundae minores exemplo uno = saltus totus statim */
interior f32
_passus_involucri (
    f32 secundae,
    f32 amplitudo)
{
    f32 n = secundae * (f32)SONUS_FREQUENTIA;

    si (n < 1.0f)
    {
        redde amplitudo;
    }
    redde amplitudo / n;
}

vacuum
sonus_involucrum_parare (
                 SonusInvolucrum* inv,
    constans SonusInvolucriForma* forma)
{
    inv->forma.impetus  = forma->impetus < 0.0f ? 0.0f : forma->impetus;
    inv->forma.lapsus   = forma->lapsus < 0.0f ? 0.0f : forma->lapsus;
    inv->forma.tenor    = _coercere(forma->tenor, 0.0f, 1.0f);
    inv->forma.remissio = forma->remissio
        < 0.0f ? 0.0f : forma->remissio;
    inv->gradus  = SONUS_INVOLUCRUM_QUIESCIT;
    inv->valor   = 0.0f;
    inv->passus  = 0.0f;
}

vacuum
sonus_involucrum_aperire (
    SonusInvolucrum* inv)
{
    inv->gradus = SONUS_INVOLUCRUM_IMPETUS;
    inv->passus = _passus_involucri(inv->forma.impetus, 1.0f);
}

vacuum
sonus_involucrum_claudere (
    SonusInvolucrum* inv)
{
    si (inv->gradus == SONUS_INVOLUCRUM_QUIESCIT)
    {
        redde;
    }
    si (inv->valor <= 0.0f)
    {
        inv->valor   = 0.0f;
        inv->gradus  = SONUS_INVOLUCRUM_QUIESCIT;
        redde;
    }
    inv->gradus = SONUS_INVOLUCRUM_REMISSIO;
    inv->passus = _passus_involucri(inv->forma.remissio, inv->valor);
}

f32
sonus_involucrum_proximum (
    SonusInvolucrum* inv)
{
    commutatio (inv->gradus)
    {
        casus SONUS_INVOLUCRUM_IMPETUS:
            inv->valor += inv->passus;
            si (inv->valor >= 1.0f)
            {
                inv->valor   = 1.0f;
                inv->gradus  = SONUS_INVOLUCRUM_LAPSUS;
                inv->passus = _passus_involucri(inv->forma.lapsus,
                                                1.0f
                                                    - inv->forma.tenor);
            }
            frange;
        casus SONUS_INVOLUCRUM_LAPSUS:
            inv->valor -= inv->passus;
            si (inv->valor <= inv->forma.tenor)
            {
                inv->valor   = inv->forma.tenor;
                inv->gradus  = SONUS_INVOLUCRUM_TENOR;
            }
            frange;
        casus SONUS_INVOLUCRUM_TENOR:
            inv->valor = inv->forma.tenor;
            frange;
        casus SONUS_INVOLUCRUM_REMISSIO:
            inv->valor -= inv->passus;
            si (inv->valor <= 0.0f)
            {
                inv->valor   = 0.0f;
                inv->gradus  = SONUS_INVOLUCRUM_QUIESCIT;
            }
            frange;
        casus SONUS_INVOLUCRUM_QUIESCIT:
        ordinarius:
            inv->valor = 0.0f;
            frange;
    }
    redde inv->valor;
}

b32
sonus_involucrum_quiescit (
    constans SonusInvolucrum* inv)
{
    redde inv->gradus == SONUS_INVOLUCRUM_QUIESCIT;
}


    /* ==================================================
     * COLUM: y += a (x - y), a = 1 - e^(-2 pi fc / fs)
     * ================================================== */

vacuum
sonus_colum_parare (
        SonusColum* c,
    SonusGenusColi  genus,
               f32  frequentia)
{
    c->genus    = genus;
    c->memoria  = 0.0f;
    sonus_colum_frequentiam_ponere(c, frequentia);
}

vacuum
sonus_colum_frequentiam_ponere (
    SonusColum* c,
           f32  frequentia)
{
    f64 f;

    si (frequentia <= 0.0f)
    {
        c->coefficiens = 0.0f;
        redde;
    }
    f = (f64)_coercere(frequentia, 0.0f, (f32)SONUS_FREQUENTIA / 2.0f);
    c->coefficiens = (f32)(1.0 - exp(-SONUS_DUO_PI * f
                                     / (f64)SONUS_FREQUENTIA));
}

f32
sonus_colum_proximum (
    SonusColum* c,
           f32  x)
{
    si (c->coefficiens == 0.0f)
    {
        redde x;
    }
    c->memoria += c->coefficiens * (x - c->memoria);
    si (c->genus == SONUS_COLUM_ACUTA)
    {
        redde x - c->memoria;
    }
    redde c->memoria;
}


    /* ==================================================
     * RETENTIO et COMMINUTIO
     * ================================================== */

vacuum
sonus_retentio_parare (
    SonusRetentio* r,
              i32  spatium)
{
    r->spatium  = spatium;
    r->reliqua  = ZEPHYRUM;
    r->valor    = 0.0f;
}

f32
sonus_retentio_proximum (
    SonusRetentio* r,
              f32  x)
{
    si (r->spatium < II)
    {
        redde x;
    }
    si (r->reliqua == ZEPHYRUM)
    {
        r->valor    = x;
        r->reliqua  = r->spatium;
    }
    r->reliqua--;
    redde r->valor;
}

f32
sonus_comminuere (
    f32 x,
    i32 gradus)
{
    f32 dimidium;

    si (gradus < II)
    {
        redde x;
    }
    dimidium = (f32)gradus / 2.0f;
    redde (f32)floor((f64)(x * dimidium) + 0.5) / dimidium;
}


    /* ==================================================
     * ALTITUDO
     * ================================================== */

f32
sonus_ratio_semitoniorum (
    f32 semitonia)
{
    redde (f32)pow(2.0, (f64)semitonia / 12.0);
}

f32
sonus_notae_frequentia (
    f32 nota)
{
    redde 440.0f * sonus_ratio_semitoniorum(nota - 69.0f);
}
