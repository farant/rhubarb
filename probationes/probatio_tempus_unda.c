/* probatio_tempus_unda.c - unda_tumultus super sors (migratio ad sors)
 *
 * unda_tumultus(t) = tumultus 'albus' per situlas temporis (~81 us):
 * intra situlam constans, trans situlas independens. Olim LCG signatus
 * (redundantia s32 = mores indefiniti) et conversio f32 -> s32 sine
 * tecto (t > ~174000 s indefinita). Nunc situla integra -> sors.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "sors.h"
#include "tempus.h"
#include <math.h>
#include <stdio.h>

/* valor exspectatus per formulam documentatam (tempus.c): situla =
 * floor(t * 12345.6789), sors_seminare(situla, 0), f32 * 2 - 1 */
interior f32
_exspectatus (
    f32 t)
{
    Sors s;
     f64 locus = floor((f64)t * 12345.6789);

    sors_seminare(&s, (i64)(s64)locus, (i64)ZEPHYRUM);
    redde sors_f32(&s) * 2.0f - 1.0f;
}

s32 principale (vacuum)
{
           Piscina* piscina;
     volatilis f32  nihil_f = 0.0f;
               f32  extrema[VI];
               f64  summa       = 0.0;
               i32  mutationes  = ZEPHYRUM;
               i32  extra       = ZEPHYRUM;
               f32  prior;
               i32  k;

    piscina = piscina_generare_dynamicum("probatio_tempus_unda", 4096);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans unda_tumultus super sors ---\n");
    /* filum: valor ex sors per formulam documentatam */
    CREDO_VERUM (unda_tumultus(0.5f) == _exspectatus(0.5f));
    CREDO_VERUM (unda_tumultus(3.25f) == _exspectatus(3.25f));
    /* determinatum */
    CREDO_VERUM (unda_tumultus(1.0f) == unda_tumultus(1.0f));
    /* intra situlam constans, trans situlas mutatur */
    CREDO_VERUM (unda_tumultus(0.5f) == unda_tumultus(0.5f + 1.0e-6f));
    prior = unda_tumultus(ZEPHYRUM);
    per (k = I; k < XX * M; k++)
    {
        f32 v = unda_tumultus((f32)((f64)k / 12345.6789 + 1.0e-5));

        si (v != prior)
        {
            mutationes++;
        }
        si (v < -1.0f || v >= 1.0f)
        {
            extra++;
        }
        summa += (f64)v;
        prior = v;
    }
    CREDO_AEQUALIS_I32 (extra, ZEPHYRUM);
    CREDO_VERUM (mutationes > (i32)19900);
    /* medium prope 0 (album, symmetricum) */
    CREDO_VERUM (fabs(summa / (f64)(XX * M)) < 0.03);

    /* inputa olim INDEFINITA: magna, negativa, infinita, NaN - in
     * [-1, 1) et determinata */
    extrema[0]    = 1.0e9f;
    extrema[I]    = -1.0e9f;
    extrema[II]   = 3.0e38f;
    extrema[III]  = -7.5f;
    extrema[IV]   = 1.0f / nihil_f;
    extrema[V]    = nihil_f / nihil_f;
    per (k = ZEPHYRUM; k < VI; k++)
    {
        f32 v = unda_tumultus(extrema[k]);

        CREDO_VERUM (v >= -1.0f && v < 1.0f);
        CREDO_VERUM (v == unda_tumultus(extrema[k]));
    }

    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
