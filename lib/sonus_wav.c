/* sonus_wav.c - Exempla soni in plagulam WAV (vide include/sonus_wav.h)
 *
 * Octeti per manum ordinati (little-endian explicitus), non per
 * structuram in memoria scriptam - ordo octetorum machinae et
 * impletio structurae nihil mutant. Vide lib/sonus_wav.worklog.md.
 */
#include "sonus_wav.h"
#include "filum.h"
#include <math.h>

#define SONUS_WAV_SUMMUM 32767.0

interior vacuum
_xvi_scribere (
     i8* p,
    i32  valor)
{
    p[ZEPHYRUM]  = (i8)(valor & 0xFFU);
    p[I]         = (i8)((valor >> VIII) & 0xFFU);
}

interior vacuum
_xxxii_scribere (
     i8* p,
    i32  valor)
{
    p[ZEPHYRUM]  = (i8)(valor & 0xFFU);
    p[I]         = (i8)((valor >> VIII) & 0xFFU);
    p[II]        = (i8)((valor >> XVI) & 0xFFU);
    p[III]       = (i8)((valor >> XXIV) & 0xFFU);
}

interior vacuum
_literas_scribere (
                    i8* p,
    constans character* literae)
{
    i32 k;

    per (k = ZEPHYRUM; k < IV; k++)
    {
        p[k] = (i8)literae[k];
    }
}

/* f32 -> s16: coercitio ad [-1, 1], deinde rotundatio a zephyro
 * (symmetrica: -0.5 -> -16384 sicut 0.5 -> 16384) */
interior s32
_exemplum_xvi (
    f32 x)
{
    f64 v = (f64)x;

    si (v > 1.0)
    {
        v = 1.0;
    }
    si (v < -1.0)
    {
        v = -1.0;
    }
    v *= SONUS_WAV_SUMMUM;
    si (v >= 0.0)
    {
        redde (s32)floor(v + 0.5);
    }
    redde -(s32)floor(-v + 0.5);
}

chorda
sonus_wav_codificare (
    constans f32* exempla,
             i32  quadra,
             i32  canales,
             i32  frequentia,
         Piscina* piscina)
{
    chorda  fructus;
       i32  data;
       i32  k;
        i8* p;

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;
    si ((canales != I && canales != II) || frequentia == ZEPHYRUM)
    {
        redde fructus;
    }
    data = quadra * canales * II;
    p = (i8*)piscina_allocare(piscina,
                              (memoriae_index)(SONUS_WAV_CAPUT + data));
    si (p == NIHIL)
    {
        redde fructus;
    }

    _literas_scribere(p, "RIFF");
    _xxxii_scribere(p + IV, XXXVI + data);
    _literas_scribere(p + VIII, "WAVE");
    _literas_scribere(p + XII, "fmt ");
    _xxxii_scribere(p + XVI, XVI);
    _xvi_scribere(p + XX, I);                        /* PCM */
    _xvi_scribere(p + XXII, canales);
    _xxxii_scribere(p + XXIV, frequentia);
    _xxxii_scribere(p + XXVIII, frequentia * canales * II);
    _xvi_scribere(p + XXXII, canales * II);
    _xvi_scribere(p + XXXIV, XVI);
    _literas_scribere(p + XXXVI, "data");
    _xxxii_scribere(p + XL, data);

    per (k = ZEPHYRUM; k < quadra * canales; k++)
    {
        _xvi_scribere(p + SONUS_WAV_CAPUT + k * II,
                      (i32)_exemplum_xvi(exempla[k]));
    }
    fructus.datum    = p;
    fructus.mensura  = SONUS_WAV_CAPUT + data;
    redde fructus;
}

b32
sonus_wav_scribere (
    constans character* via,
          constans f32* exempla,
                   i32  quadra,
                   i32  canales,
                   i32  frequentia,
               Piscina* piscina)
{
    chorda octeti = sonus_wav_codificare(exempla, quadra, canales,
                                         frequentia, piscina);

    si (octeti.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde filum_scribere(via, octeti);
}
