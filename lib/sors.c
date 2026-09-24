/* sors.c - Numeri pseudo-fortuiti seminabiles (PCG32 + SplitMix64)
 *
 * PCG32 (O'Neill): status LXIV bits, effusio XXXII bits per
 * 'xorshift + rotatio'. SplitMix64 (Vigna) semen usoris in statum
 * initialem bene mixtum vertit. Vide lib/sors.worklog.md.
 */
#include "sors.h"

#define SORS_MULTIPLICATOR  6364136223846793005ULL
#define SPLITMIX_GAMMA      0x9E3779B97F4A7C15ULL
#define SPLITMIX_MISCENS_I   0xBF58476D1CE4E5B9ULL
#define SPLITMIX_MISCENS_II  0x94D049BB133111EBULL

/* SplitMix64: statum provehit, valorem mixtum reddit */
interior i64
_splitmix_proximum (
    i64* x)
{
    i64 z;

    *x  = *x + SPLITMIX_GAMMA;
    z   = *x;
    z   = (z ^ (z >> XXX)) * SPLITMIX_MISCENS_I;
    z   = (z ^ (z >> XXVII)) * SPLITMIX_MISCENS_II;
    redde z ^ (z >> XXXI);
}

/* PCG32 initium canonicum (pcg32_srandom_r): status nullus,
 * incrementum impar ex serie, gradus, status += initium, gradus */
interior vacuum
_pcg_initiare (
    Sors* s,
     i64  initium,
     i64  series)
{
    s->status       = 0ULL;
    s->incrementum  = (series << I) | 1ULL;
    (vacuum)sors_proximum(s);
    s->status = s->status + initium;
    (vacuum)sors_proximum(s);
}

vacuum
sors_seminare (
    Sors* s,
     i64  semen,
     i64  series)
{
    i64 x = semen;

    _pcg_initiare(s, _splitmix_proximum(&x), series);
}

Sors
sors_derivare (
    constans Sors* s,
              i64  clavis)
{
    Sors filius;
     i64 y = s->status + clavis * SPLITMIX_GAMMA;

    sors_seminare(&filius, _splitmix_proximum(&y) ^ s->incrementum,
        clavis);
    redde filius;
}

i32
sors_proximum (
    Sors* s)
{
    i64 vetus = s->status;
    i32 mixtum;
    i32 rotatio;

    s->status  = vetus * SORS_MULTIPLICATOR + s->incrementum;
    mixtum     = (i32)(((vetus >> XVIII) ^ vetus) >> XXVII);
    rotatio    = (i32)(vetus >> LIX);
    redde (mixtum >> rotatio) | (mixtum << ((0U - rotatio) & XXXI));
}

i32
sors_intra (
    Sors* s,
     i32  limes)
{
    i64 productum;
    i32 inferior;

    si (limes == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    productum  = (i64)sors_proximum(s) * (i64)limes;
    inferior   = (i32)productum;
    si (inferior < limes)
    {
        i32 limen = (0U - limes) % limes;

        dum (inferior < limen)
        {
            productum  = (i64)sors_proximum(s) * (i64)limes;
            inferior   = (i32)productum;
        }
    }
    redde (i32)(productum >> XXXII);
}

s32
sors_inter (
    Sors* s,
     s32  imum,
     s32  summum)
{
    i64 spatium;

    si (imum >= summum)
    {
        redde imum;
    }
    spatium = (i64)((s64)summum - (s64)imum) + 1ULL;
    si (spatium > 0xFFFFFFFFULL)
    {
        redde (s32)sors_proximum(s);
    }
    redde (s32)((s64)imum + (s64)sors_intra(s, (i32)spatium));
}

f64
sors_f64 (
    Sors* s)
{
    i32 superior = sors_proximum(s) >> V;
    i32 inferior = sors_proximum(s) >> VI;

    redde ((f64)superior * 67108864.0 + (f64)inferior)
        / 9007199254740992.0;
}

f32
sors_f32 (
    Sors* s)
{
    redde (f32)(sors_proximum(s) >> VIII) * (1.0f / 16777216.0f);
}

b32
sors_casu (
    Sors* s,
     i32  numerator,
     i32  denominator)
{
    si (denominator == ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde sors_intra(s, denominator) < numerator;
}

vacuum
sors_miscere (
      Sors* s,
    vacuum* elementa,
       i32  numerus,
       i32  magnitudo_elementi)
{
     i8* octeti = (i8*)elementa;
    i32  i;

    si (elementa == NIHIL || numerus < II)
    {
        redde;
    }
    per (i = numerus - I; i > ZEPHYRUM; i--)
    {
        i32 j = sors_intra(s, i + I);
        i32 k;

        si (j == i)
        {
            perge;
        }
        per (k = ZEPHYRUM; k < magnitudo_elementi; k++)
        {
            i8 transitus = octeti[i * magnitudo_elementi + k];

            octeti[i * magnitudo_elementi + k]  =
                octeti[j * magnitudo_elementi + k];
            octeti[j * magnitudo_elementi + k]  = transitus;
        }
    }
}

i32
sors_lcg_classicus (
    i32* status)
{
    *status = *status * 1103515245U + 12345U;
    redde *status;
}
