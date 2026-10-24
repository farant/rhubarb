/* probatio_sonus.c - Probationes primitivorum soni (include/sonus.h)
 *
 * Mensurae ex signo ipso, non ex implementatione: frequentia per
 * circumvolutiones numeratas in secundo uno, officium per exempla
 * positiva, colum contra responsum analyticum filtri unius poli
 * |H| = a / |1 - (1-a) e^-jw| (valores in sectione colum).
 * Vide lib/sonus.worklog.md.
 */
#include "latina.h"
#include "credo.h"
#include "sonus.h"
#include <math.h>
#include <string.h>

/* circumvolutiones serrae in n exemplis: saltus deorsum > 1 */
interior i32
_saltus_serrae (
    SonusUnda* u,
          i32  n)
{
    i32 k;
    i32 numerus  = ZEPHYRUM;
    f32 prius    = sonus_unda_proximum(u);

    per (k = I; k < n; k++)
    {
        f32 x = sonus_unda_proximum(u);

        si (x < prius - 1.0f)
        {
            numerus++;
        }
        prius = x;
    }
    redde numerus;
}

/* transitus a negativo ad non-negativum in n exemplis */
interior i32
_transitus_ascendentes (
    SonusUnda* u,
          i32  n)
{
    i32 k;
    i32 numerus  = ZEPHYRUM;
    f32 prius    = sonus_unda_proximum(u);

    per (k = I; k < n; k++)
    {
        f32 x = sonus_unda_proximum(u);

        si (prius < 0.0f && x >= 0.0f)
        {
            numerus++;
        }
        prius = x;
    }
    redde numerus;
}

/* amplitudo stabilis sinus 'frequentia' per colum: maximum |y| in
 * secundo dimidio secundi unius (transiens primum dimissum) */
interior f32
_amplitudo_per_colum (
    SonusGenusColi genus,
               f32 limes,
               f32 frequentia)
{
     SonusUnda u;
    SonusColum c;
           i32 k;
           f32 maximum = 0.0f;

    sonus_unda_parare(&u, SONUS_UNDA_SINUS, ZEPHYRUM);
    sonus_unda_frequentiam_ponere(&u, frequentia);
    sonus_colum_parare(&c, genus, limes);
    per (k = ZEPHYRUM; k < SONUS_FREQUENTIA; k++)
    {
        f32 y = sonus_colum_proximum(&c, sonus_unda_proximum(&u));

        si (y < 0.0f)
        {
            y = -y;
        }
        si (k >= SONUS_FREQUENTIA / II && y > maximum)
        {
            maximum = y;
        }
    }
    redde maximum;
}

interior vacuum
_passus (
    SonusInvolucrum* inv,
                i32  n)
{
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        (vacuum)sonus_involucrum_proximum(inv);
    }
}


    /* ==================================================
     * UNDA: frequentia per circumvolutiones in secundo uno
     * ================================================== */

/* amplitudo stabilis sinus 'frequentia' per colum resonantem (ut
 * _amplitudo_per_colum) */
interior f32
_amplitudo_per_resonantem (
    SonusGenusResonantis genus,
                     f32 limes,
                     f32 acumen,
                     f32 frequentia)
{
        SonusUnda u;
    SonusResonans r;
              i32 k;
              f32 maximum = 0.0f;

    sonus_unda_parare(&u, SONUS_UNDA_SINUS, ZEPHYRUM);
    sonus_unda_frequentiam_ponere(&u, frequentia);
    sonus_resonans_parare(&r, genus, limes, acumen);
    per (k = ZEPHYRUM; k < SONUS_FREQUENTIA; k++)
    {
        f32 y = sonus_resonans_proximum(&r, sonus_unda_proximum(&u));

        si (y < 0.0f)
        {
            y = -y;
        }
        si (k >= SONUS_FREQUENTIA / II && y > maximum)
        {
            maximum = y;
        }
    }
    redde maximum;
}

interior vacuum
_sectio_frequentia (
    CredoContextus* c)
{
    SonusUnda u;
          i32 k;

    interior constans f32 frequentiae[IV] = {
        100.0f, 440.0f, 1000.0f, 5000.0f };
    interior constans s32 exspectatae[IV] = { C, CDXL, M, V * M };

    (vacuum)c;
    per (k = ZEPHYRUM; k < IV; k++)
    {
        s32 n;

        sonus_unda_parare(&u, SONUS_UNDA_SERRA, ZEPHYRUM);
        sonus_unda_frequentiam_ponere(&u, frequentiae[k]);
        n = (s32)_saltus_serrae(&u, SONUS_FREQUENTIA);
        CREDO_VERUM (n >= exspectatae[k] - I
            && n <= exspectatae[k] + I);
    }
    {
        s32 n;

        sonus_unda_parare(&u, SONUS_UNDA_PULSUS, ZEPHYRUM);
        sonus_unda_frequentiam_ponere(&u, 440.0f);
        n = (s32)_transitus_ascendentes(&u, SONUS_FREQUENTIA);
        CREDO_VERUM (n >= CDXXXIX && n <= CDXLI);

        sonus_unda_parare(&u, SONUS_UNDA_SINUS, ZEPHYRUM);
        sonus_unda_frequentiam_ponere(&u, 1000.0f);
        n = (s32)_transitus_ascendentes(&u, SONUS_FREQUENTIA);
        CREDO_VERUM (n >= CMXCIX && n <= MI);

        sonus_unda_parare(&u, SONUS_UNDA_TRIANGULUM, ZEPHYRUM);
        sonus_unda_frequentiam_ponere(&u, 250.0f);
        n = (s32)_transitus_ascendentes(&u, SONUS_FREQUENTIA);
        CREDO_VERUM (n >= CCXLIX && n <= CCLI);
    }
}


    /* ==================================================
     * UNDA: officium pulsus (exempla positiva / omnia)
     * ================================================== */

interior vacuum
_sectio_officium (
    CredoContextus* c)
{
    SonusUnda u;
          i32 k;
          i32 positiva;

    (vacuum)c;
    sonus_unda_parare(&u, SONUS_UNDA_PULSUS, ZEPHYRUM);
    sonus_unda_frequentiam_ponere(&u, 1000.0f);
    sonus_unda_officium_ponere(&u, 0.25f);
    positiva = ZEPHYRUM;
    per (k = ZEPHYRUM; k < SONUS_FREQUENTIA; k++)
    {
        si (sonus_unda_proximum(&u) > 0.0f)
        {
            positiva++;
        }
    }
    CREDO_VERUM (positiva >= XII * M - XLVIII
                 && positiva <= XII * M + XLVIII);

    /* officium 0 ad 0.01 coercetur: pulsus angustus, non silentium.
     * Ad X cycli/s (pulsus XLVIII exempla latus). Pulsus paucorum
     * exemplorum NON numerandus: phasis per cyclum vix errat, ergo
     * cyclus quisque eundem numerum integrum capit - ad M cycli/s
     * (0.48 exempli) M aut 0 positiva, ad C (IV.VIII) D aut CD,
     * secundum phasim initialem - numquam CDLXXX */
    sonus_unda_frequentiam_ponere(&u, 10.0f);
    sonus_unda_officium_ponere(&u, 0.0f);
    positiva = ZEPHYRUM;
    per (k = ZEPHYRUM; k < SONUS_FREQUENTIA; k++)
    {
        si (sonus_unda_proximum(&u) > 0.0f)
        {
            positiva++;
        }
    }
    CREDO_VERUM (positiva >= CDLXX && positiva <= D);
}


    /* ==================================================
     * UNDA: fines, silentium, coercitio, phasis servata
     * ================================================== */

interior vacuum
_sectio_fines (
    CredoContextus* c)
{
    SonusUnda u;
          i32 forma;
          i32 k;

    (vacuum)c;
    per (forma = ZEPHYRUM; forma <= (i32)SONUS_UNDA_STREPITUS; forma++)
    {
        f32 maximum  = -2.0f;
        f32 minimum  = 2.0f;
        b32 intra    = VERUM;
        b32 tacita   = VERUM;

        sonus_unda_parare(&u, (SonusFormaUndae)forma, VII);
        sonus_unda_frequentiam_ponere(&u, 1234.0f);
        per (k = ZEPHYRUM; k < SONUS_FREQUENTIA; k++)
        {
            f32 x = sonus_unda_proximum(&u);

            si (x < -1.0f || x > 1.0f)
            {
                intra = FALSUM;
            }
            si (x > maximum) maximum = x;
            si (x < minimum) minimum = x;
        }
        CREDO_VERUM (intra);
        CREDO_VERUM (maximum > 0.99f);
        CREDO_VERUM (minimum < -0.99f);

        /* frequentia 0 = silentium in omni forma */
        sonus_unda_frequentiam_ponere(&u, 0.0f);
        per (k = ZEPHYRUM; k < M; k++)
        {
            si (sonus_unda_proximum(&u) != 0.0f)
            {
                tacita = FALSUM;
            }
        }
        CREDO_VERUM (tacita);
    }

    /* incrementum ROTUNDATUM: CDXL / XLVIII milia x 2^32 = 39370533.55
     * (truncatio 39370533 = CDXXXIX.99998 cycli in secundo) */
    sonus_unda_parare(&u, SONUS_UNDA_SERRA, ZEPHYRUM);
    sonus_unda_frequentiam_ponere(&u, 440.0f);
    CREDO_AEQUALIS_I32 (u.incrementum, 39370534U);

    /* supra Nyquist: coercetur ad dimidium cycli per exemplum */
    sonus_unda_parare(&u, SONUS_UNDA_SERRA, ZEPHYRUM);
    sonus_unda_frequentiam_ponere(&u, 30000.0f);
    CREDO_AEQUALIS_I32 (u.incrementum, 0x80000000U);
    sonus_unda_frequentiam_ponere(&u, -5.0f);
    CREDO_AEQUALIS_I32 (u.incrementum, ZEPHYRUM);

    /* mutatio frequentiae phasim non reponit */
    {
        i32 phasis;

        sonus_unda_frequentiam_ponere(&u, 440.0f);
        per (k = ZEPHYRUM; k < C; k++)
        {
            (vacuum)sonus_unda_proximum(&u);
        }
        phasis = u.phasis;
        CREDO_VERUM (phasis != ZEPHYRUM);
        sonus_unda_frequentiam_ponere(&u, 880.0f);
        CREDO_AEQUALIS_I32 (u.phasis, phasis);
    }
}


    /* ==================================================
     * UNDA: strepitus seminatus et retentus
     * ================================================== */

interior vacuum
_sectio_strepitus (
    CredoContextus* c)
{
    SonusUnda a;
    SonusUnda b;
    SonusUnda d;
          i32 k;
          i32 mutationes  = ZEPHYRUM;
          b32 pares       = VERUM;
          b32 diversa     = FALSUM;
          f32 prius;

    (vacuum)c;
    sonus_unda_parare(&a, SONUS_UNDA_STREPITUS, IX);
    sonus_unda_parare(&b, SONUS_UNDA_STREPITUS, IX);
    sonus_unda_parare(&d, SONUS_UNDA_STREPITUS, X);
    sonus_unda_frequentiam_ponere(&a, 1000.0f);
    sonus_unda_frequentiam_ponere(&b, 1000.0f);
    sonus_unda_frequentiam_ponere(&d, 1000.0f);
    prius = sonus_unda_proximum(&a);
    (vacuum)sonus_unda_proximum(&b);
    (vacuum)sonus_unda_proximum(&d);
    per (k = I; k < SONUS_FREQUENTIA; k++)
    {
        f32 x = sonus_unda_proximum(&a);

        si (x != sonus_unda_proximum(&b))
        {
            pares = FALSUM;
        }
        si (x != sonus_unda_proximum(&d))
        {
            diversa = VERUM;
        }
        si (x != prius)
        {
            mutationes++;
        }
        prius = x;
    }
    CREDO_VERUM (pares);
    CREDO_VERUM (diversa);
    /* valor novus octies per cyclum: VIII milia per secundum */
    CREDO_VERUM (mutationes >= VII * M + CM && mutationes <= VIII * M);
}


    /* ==================================================
     * INVOLUCRUM: tempora graduum
     * ================================================== */

interior vacuum
_sectio_involucrum (
    CredoContextus* c)
{
        SonusInvolucrum inv;
    SonusInvolucriForma forma;

    (vacuum)c;
    forma.impetus   = 0.01f;   /* CDLXXX exempla */
    forma.lapsus    = 0.02f;   /* CMLX */
    forma.tenor     = 0.5f;
    forma.remissio  = 0.04f;   /* MCMXX */

    sonus_involucrum_parare(&inv, &forma);
    CREDO_VERUM (sonus_involucrum_quiescit(&inv));
    CREDO_F32_PROXIMUS (sonus_involucrum_proximum(&inv), 0.0f, 0.0f);

    sonus_involucrum_aperire(&inv);
    CREDO_VERUM (!sonus_involucrum_quiescit(&inv));
    _passus(&inv, CDLXIX);
    CREDO_AEQUALIS_I32 ((i32)inv.gradus, (i32)SONUS_INVOLUCRUM_IMPETUS);
    CREDO_F32_PROXIMUS (sonus_involucrum_proximum(&inv),
                        470.0f / 480.0f, 0.001f);
    _passus(&inv, XX);
    CREDO_AEQUALIS_I32 ((i32)inv.gradus, (i32)SONUS_INVOLUCRUM_LAPSUS);
    _passus(&inv, CMLX);
    CREDO_AEQUALIS_I32 ((i32)inv.gradus, (i32)SONUS_INVOLUCRUM_TENOR);
    _passus(&inv, V * M);
    CREDO_F32_PROXIMUS (sonus_involucrum_proximum(&inv), 0.5f,
        0.000001f);

    sonus_involucrum_claudere(&inv);
    _passus(&inv, MCMX);
    CREDO_AEQUALIS_I32 ((i32)inv.gradus,
        (i32)SONUS_INVOLUCRUM_REMISSIO);
    _passus(&inv, XX);
    CREDO_VERUM (sonus_involucrum_quiescit(&inv));
    CREDO_F32_PROXIMUS (sonus_involucrum_proximum(&inv), 0.0f, 0.0f);

    /* tempora nulla: saltus statim */
    forma.impetus   = 0.0f;
    forma.lapsus    = 0.0f;
    forma.tenor     = 1.0f;
    forma.remissio  = 0.0f;
    sonus_involucrum_parare(&inv, &forma);
    sonus_involucrum_aperire(&inv);
    CREDO_F32_PROXIMUS (sonus_involucrum_proximum(&inv), 1.0f, 0.0f);
    sonus_involucrum_claudere(&inv);
    CREDO_F32_PROXIMUS (sonus_involucrum_proximum(&inv), 0.0f, 0.0f);
    CREDO_VERUM (sonus_involucrum_quiescit(&inv));
}


    /* ==================================================
     * INVOLUCRUM: repercussio et remissio a valore currente
     * ================================================== */

interior vacuum
_sectio_repercussio (
    CredoContextus* c)
{
        SonusInvolucrum inv;
    SonusInvolucriForma forma;
                    f32 ante;
                    f32 post;

    (vacuum)c;
    forma.impetus   = 0.01f;
    forma.lapsus    = 0.0f;
    forma.tenor     = 1.0f;
    forma.remissio  = 0.04f;

    /* repercussio in remissione: impetus a 0.25, non a 0 */
    sonus_involucrum_parare(&inv, &forma);
    sonus_involucrum_aperire(&inv);
    _passus(&inv, M);
    sonus_involucrum_claudere(&inv);
    _passus(&inv, MCDXL);
    ante = inv.valor;
    CREDO_F32_PROXIMUS (ante, 0.25f, 0.002f);
    sonus_involucrum_aperire(&inv);
    post = sonus_involucrum_proximum(&inv);
    CREDO_VERUM (post > ante);
    CREDO_F32_PROXIMUS (post, ante + 1.0f / 480.0f, 0.0001f);

    /* remissio in medio impetu: descendit a valore currente */
    sonus_involucrum_parare(&inv, &forma);
    sonus_involucrum_aperire(&inv);
    _passus(&inv, CCXL);
    ante = inv.valor;
    CREDO_F32_PROXIMUS (ante, 0.5f, 0.002f);
    sonus_involucrum_claudere(&inv);
    post = sonus_involucrum_proximum(&inv);
    CREDO_VERUM (post < ante);
    CREDO_F32_PROXIMUS (post, ante - ante / 1920.0f, 0.00001f);
}


    /* ==================================================
     * COLUM: responsum contra analysin (a = 1 - e^(-2 pi fc / fs))
     *   gravia, fc M: C -> 0.995, M -> 0.7075, X M -> 0.107
     *   acuta,  fc M: C -> 0.093, M -> 0.6617, X M -> 0.931
     * ================================================== */

interior vacuum
_sectio_colum (
    CredoContextus* c)
{
    SonusColum colum;
           f32 memoria;
           i32 k;

    (vacuum)c;
    CREDO_VERUM (_amplitudo_per_colum(SONUS_COLUM_GRAVIA, 1000.0f,
                                      100.0f) > 0.99f);
    CREDO_F32_PROXIMUS (_amplitudo_per_colum(SONUS_COLUM_GRAVIA,
        1000.0f,
                                             1000.0f), 0.7075f, 0.01f);
    CREDO_VERUM (_amplitudo_per_colum(SONUS_COLUM_GRAVIA, 1000.0f,
                                      10000.0f) < 0.12f);
    CREDO_VERUM (_amplitudo_per_colum(SONUS_COLUM_ACUTA, 1000.0f,
                                      100.0f) < 0.1f);
    CREDO_F32_PROXIMUS (_amplitudo_per_colum(SONUS_COLUM_ACUTA, 1000.0f,
                                             1000.0f), 0.6617f, 0.01f);
    CREDO_VERUM (_amplitudo_per_colum(SONUS_COLUM_ACUTA, 1000.0f,
                                      10000.0f) > 0.9f);

    /* frequentia 0 = transit, octetim */
    sonus_colum_parare(&colum, SONUS_COLUM_GRAVIA, 0.0f);
    CREDO_F32_PROXIMUS (sonus_colum_proximum(&colum, 0.3f), 0.3f, 0.0f);
    CREDO_F32_PROXIMUS (sonus_colum_proximum(&colum, -0.7f), -0.7f,
        0.0f);

    /* limes mutatus memoriam servat */
    sonus_colum_parare(&colum, SONUS_COLUM_GRAVIA, 1000.0f);
    per (k = ZEPHYRUM; k < C; k++)
    {
        (vacuum)sonus_colum_proximum(&colum, 1.0f);
    }
    memoria = colum.memoria;
    CREDO_VERUM (memoria > 0.5f);
    sonus_colum_frequentiam_ponere(&colum, 2000.0f);
    CREDO_F32_PROXIMUS (colum.memoria, memoria, 0.0f);
}


    /* ==================================================
     * RESONANS - RBJ: |H(f0)| = Q (gravia, acuta), 1 (media);
     * media |H| = 1 / sqrt(1 + Q^2 (f/f0 - f0/f)^2) (Q 5, f0/4 ->
     * ~0.053)
     * ================================================== */

interior vacuum
_sectio_resonans (
    CredoContextus* c)
{
    SonusResonans r;
              f32 memoria[IV];
              f32 y;
              i32 k;
              b32 finita = VERUM;

    (vacuum)c;
    CREDO_GRADUS ("gravia: planum, -3 dB, resonantia, declivitas");
    CREDO_VERUM (_amplitudo_per_resonantem(SONUS_RESONANS_GRAVIA,
        1000.0f, 0.7071f, 100.0f) > 0.99f);
    CREDO_F32_PROXIMUS (_amplitudo_per_resonantem(SONUS_RESONANS_GRAVIA,
        1000.0f, 0.7071f, 1000.0f), 0.7071f, 0.01f);
    CREDO_F32_PROXIMUS (_amplitudo_per_resonantem(SONUS_RESONANS_GRAVIA,
        1000.0f, 10.0f, 1000.0f), 10.0f, 0.15f);
    CREDO_VERUM (_amplitudo_per_resonantem(SONUS_RESONANS_GRAVIA,
        1000.0f, 0.7071f, 10000.0f) < 0.02f);
    /* acumen 0 = 0.707 */
    CREDO_F32_PROXIMUS (_amplitudo_per_resonantem(SONUS_RESONANS_GRAVIA,
        1000.0f, 0.0f, 1000.0f), 0.7071f, 0.01f);

    CREDO_GRADUS ("acuta: speculum gravium");
    CREDO_VERUM (_amplitudo_per_resonantem(SONUS_RESONANS_ACUTA,
        1000.0f, 0.7071f, 100.0f) < 0.02f);
    CREDO_F32_PROXIMUS (_amplitudo_per_resonantem(SONUS_RESONANS_ACUTA,
        1000.0f, 0.7071f, 1000.0f), 0.7071f, 0.01f);
    CREDO_VERUM (_amplitudo_per_resonantem(SONUS_RESONANS_ACUTA,
        1000.0f, 0.7071f, 10000.0f) > 0.98f);

    CREDO_GRADUS ("media: 0 dB ad medium, procul attenuata");
    CREDO_F32_PROXIMUS (_amplitudo_per_resonantem(SONUS_RESONANS_MEDIA,
        1000.0f, 5.0f, 1000.0f), 1.0f, 0.01f);
    CREDO_VERUM (_amplitudo_per_resonantem(SONUS_RESONANS_MEDIA,
        1000.0f, 5.0f, 250.0f) < 0.07f);
    CREDO_VERUM (_amplitudo_per_resonantem(SONUS_RESONANS_MEDIA,
        1000.0f, 5.0f, 4000.0f) < 0.07f);

    CREDO_GRADUS ("frequentia 0 = transit, octetim");
    sonus_resonans_parare(&r, SONUS_RESONANS_GRAVIA, 0.0f, 4.0f);
    CREDO_F32_PROXIMUS (sonus_resonans_proximum(&r, 0.3f), 0.3f, 0.0f);
    CREDO_F32_PROXIMUS (sonus_resonans_proximum(&r, -0.7f), -0.7f,
        0.0f);

    CREDO_GRADUS ("prope Nyquist et supra: stabile, impulsus decidit");
    sonus_resonans_parare(&r, SONUS_RESONANS_GRAVIA, 23000.0f, 10.0f);
    y = sonus_resonans_proximum(&r, 1.0f);
    per (k = ZEPHYRUM; k < SONUS_FREQUENTIA; k++)
    {
        y       = sonus_resonans_proximum(&r, 0.0f);
        finita  = finita && y == y && y < 100.0f && y > -100.0f;
    }
    CREDO_VERUM (finita);
    CREDO_VERUM (y < 1.0e-6f && y > -1.0e-6f);
    sonus_resonans_parare(&r, SONUS_RESONANS_MEDIA, 30000.0f, 10.0f);
    per (k = ZEPHYRUM; k < M; k++)
    {
        y       = sonus_resonans_proximum(&r, k == ZEPHYRUM ? 1.0f
                                                         : 0.0f);
        finita  = finita && y == y && y < 100.0f && y > -100.0f;
    }
    CREDO_VERUM (finita);

    CREDO_GRADUS ("ponere memoriam servat");
    sonus_resonans_parare(&r, SONUS_RESONANS_GRAVIA, 1000.0f, 2.0f);
    per (k = ZEPHYRUM; k < C; k++)
    {
        (vacuum)sonus_resonans_proximum(&r, k % II ? 1.0f : 0.5f);
    }
    memoria[0] = r.x1;
    memoria[1] = r.x2;
    memoria[2] = r.y1;
    memoria[3] = r.y2;
    CREDO_VERUM (memoria[2] > 0.5f);
    sonus_resonans_ponere(&r, 2000.0f, 4.0f);
    CREDO_VERUM (   r.x1 == memoria[0] && r.x2 == memoria[1]
                 && r.y1 == memoria[2] && r.y2 == memoria[3]);
}


    /* ==================================================
     * REVERBERATIO - responsum impulsus: silentium manet silentium,
     * aula caudam longiorem, mollities obscuriorem, latitudo 0 = mono;
     * stabilis ad aulam 1; deterministica; ponere memoriam servat
     * ================================================== */

#define CAUDA_QUADRA (II * SONUS_FREQUENTIA)

hic_manens SonusReverberatio aula_probanda;
hic_manens f32 cauda_sinistra[CAUDA_QUADRA];
hic_manens f32 cauda_dextra[CAUDA_QUADRA];

/* status sectionis nullatus */
interior vacuum
_parare_caudam (
    CredoContextus* c)
{
    (vacuum)c;
    memset(&aula_probanda, ZEPHYRUM, magnitudo(aula_probanda));
    memset(cauda_sinistra, ZEPHYRUM, magnitudo(cauda_sinistra));
    memset(cauda_dextra, ZEPHYRUM, magnitudo(cauda_dextra));
}

/* impulsus (aut strepitus 'strepitus' quadris) deinde silentium */
interior vacuum
_caudam_reddere (
    constans SonusReverberatioForma* forma,
                                i32  strepitus)
{
    i32 k;
    i32 semen = 12345U;

    sonus_reverberationem_parare(&aula_probanda, forma);
    per (k = ZEPHYRUM; k < CAUDA_QUADRA; k++)
    {
        f32 x = k == ZEPHYRUM ? 1.0f : 0.0f;

        si (k < strepitus)
        {
            semen  = semen * 1103515245U + 12345U;
            x      = (f32)(semen >> VIII) / 8388608.0f - 1.0f;
        }
        sonus_reverberationis_proximum(&aula_probanda, x,
            &cauda_sinistra[k], &cauda_dextra[k]);
    }
}

interior f64
_efficax_caudae (
    constans f32* x,
             i32  initium,
             i32  numerus)
{
    f64 s = 0.0;
    i32 k;

    per (k = initium; k < initium + numerus; k++)
    {
        s += (f64)x[k] * (f64)x[k];
    }
    redde sqrt(s / (f64)numerus);
}

/* energia acuta (supra 4 kHz) / tota in fenestra */
interior f64
_fractio_acuta_caudae (
    constans f32* x,
             i32  initium,
             i32  numerus)
{
    SonusResonans r;
              f64 acuta  = 0.0;
              f64 tota   = 0.0;
              i32 k;

    sonus_resonans_parare(&r, SONUS_RESONANS_ACUTA, 4000.0f, 0.7071f);
    per (k = ZEPHYRUM; k < initium + numerus; k++)
    {
        f32 y = sonus_resonans_proximum(&r, x[k]);

        si (k >= initium)
        {
            acuta  += (f64)y * (f64)y;
            tota   += (f64)x[k] * (f64)x[k];
        }
    }
    redde tota > 0.0 ? acuta / tota : 0.0;
}

interior vacuum
_sectio_reverberatio (
    CredoContextus* c)
{
    SonusReverberatioForma forma;
                       f64 parva;
                       f64 magna;
                       f64 clara;
                       f64 mollis;
                       f32 l;
                       f32 r;
                       i32 k;
                       b32 bene = VERUM;

    (vacuum)c;
    forma.aula       = 0.5f;
    forma.mollities  = 0.5f;
    forma.latitudo   = 1.0f;

    CREDO_GRADUS ("silentium manet silentium");
    sonus_reverberationem_parare(&aula_probanda, &forma);
    per (k = ZEPHYRUM; k < M; k++)
    {
        sonus_reverberationis_proximum(&aula_probanda, 0.0f, &l, &r);
        bene = bene && l == 0.0f && r == 0.0f;
    }
    CREDO_VERUM (bene);

    CREDO_GRADUS ("impulsus: cauda adest, aula longiorem facit");
    forma.aula = 0.2f;
    _caudam_reddere(&forma, ZEPHYRUM);
    CREDO_VERUM (_efficax_caudae(cauda_sinistra, SONUS_FREQUENTIA / XX,
                                 SONUS_FREQUENTIA / X) > 1.0e-4);
    parva = _efficax_caudae(cauda_sinistra, SONUS_FREQUENTIA / II,
                            SONUS_FREQUENTIA / X)
          / _efficax_caudae(cauda_sinistra, SONUS_FREQUENTIA / XX,
                            SONUS_FREQUENTIA / X);
    forma.aula = 0.9f;
    _caudam_reddere(&forma, ZEPHYRUM);
    magna = _efficax_caudae(cauda_sinistra, SONUS_FREQUENTIA / II,
                            SONUS_FREQUENTIA / X)
          / _efficax_caudae(cauda_sinistra, SONUS_FREQUENTIA / XX,
                            SONUS_FREQUENTIA / X);
    CREDO_VERUM (magna > 3.0 * parva);

    CREDO_GRADUS ("mollities: cauda obscurior");
    forma.aula       = 0.8f;
    forma.mollities  = 0.0f;
    _caudam_reddere(&forma, ZEPHYRUM);
    clara  = _fractio_acuta_caudae(cauda_sinistra, SONUS_FREQUENTIA / V,
                                   SONUS_FREQUENTIA / V);
    forma.mollities = 1.0f;
    _caudam_reddere(&forma, ZEPHYRUM);
    mollis = _fractio_acuta_caudae(cauda_sinistra, SONUS_FREQUENTIA / V,
                                   SONUS_FREQUENTIA / V);
    CREDO_VERUM (clara > 2.0 * mollis);

    CREDO_GRADUS ("latitudo: 0 = mono, 1 = canales diversi");
    forma.mollities  = 0.5f;
    forma.latitudo   = 0.0f;
    _caudam_reddere(&forma, ZEPHYRUM);
    bene = VERUM;
    per (k = ZEPHYRUM; k < CAUDA_QUADRA; k++)
    {
        bene = bene && cauda_sinistra[k] == cauda_dextra[k];
    }
    CREDO_VERUM (bene);
    forma.latitudo = 1.0f;
    _caudam_reddere(&forma, ZEPHYRUM);
    bene = VERUM;
    per (k = ZEPHYRUM; k < CAUDA_QUADRA; k++)
    {
        bene = bene && cauda_sinistra[k] == cauda_dextra[k];
    }
    CREDO_FALSUM (bene);

    CREDO_GRADUS ("morae scalatae: primum eccho ad pectinem "
                  "brevissimum "
                  "(1116 x 48/44.1 = 1215; dexter +23 -> 1240)");
    {
        s32 primus_l  = -I;
        s32 primus_r  = -I;
        f64 lr        = 0.0;
        f64 ll        = 0.0;
        f64 rr        = 0.0;

        per (k = ZEPHYRUM; k < CAUDA_QUADRA; k++)
        {
            si (primus_l == -I && cauda_sinistra[k] != 0.0f)
            {
                primus_l = (s32)k;
            }
            si (primus_r == -I && cauda_dextra[k] != 0.0f)
            {
                primus_r = (s32)k;
            }
        }
        CREDO_AEQUALIS_S32 (primus_l, (s32)MCCXV);
        CREDO_AEQUALIS_S32 (primus_r, (s32)MCCXL);
        /* canales fere incorrelati (latitudo 1) */
        per (k = SONUS_FREQUENTIA / X; k < SONUS_FREQUENTIA; k++)
        {
            lr += (f64)cauda_sinistra[k] * (f64)cauda_dextra[k];
            ll += (f64)cauda_sinistra[k] * (f64)cauda_sinistra[k];
            rr += (f64)cauda_dextra[k] * (f64)cauda_dextra[k];
        }
        CREDO_VERUM (fabs(lr / sqrt(ll * rr)) < 0.5);
    }

    CREDO_GRADUS ("aula 1, sine mollitie: stabilis, decidit");
    forma.aula       = 1.0f;
    forma.mollities  = 0.0f;
    _caudam_reddere(&forma, SONUS_FREQUENTIA / IV);
    bene = VERUM;
    per (k = ZEPHYRUM; k < CAUDA_QUADRA; k++)
    {
        bene = bene && cauda_sinistra[k] == cauda_sinistra[k]
               && cauda_sinistra[k] < 10.0f
               && cauda_sinistra[k] > -10.0f;
    }
    CREDO_VERUM (bene);
    CREDO_VERUM (_efficax_caudae(cauda_sinistra, CAUDA_QUADRA
                                 - SONUS_FREQUENTIA / X,
                                 SONUS_FREQUENTIA / X)
                 < _efficax_caudae(cauda_sinistra, SONUS_FREQUENTIA
                     / IV,
                                   SONUS_FREQUENTIA / X));

    CREDO_GRADUS ("deterministica");
    forma.aula = 0.6f;
    _caudam_reddere(&forma, SONUS_FREQUENTIA / X);
    l = cauda_sinistra[SONUS_FREQUENTIA / II];
    r = cauda_dextra[SONUS_FREQUENTIA];
    _caudam_reddere(&forma, SONUS_FREQUENTIA / X);
    CREDO_VERUM (   l == cauda_sinistra[SONUS_FREQUENTIA / II]
                 && r == cauda_dextra[SONUS_FREQUENTIA] && l != 0.0f);

    CREDO_GRADUS ("ponere memoriam servat, parare delet");
    sonus_reverberationem_parare(&aula_probanda, &forma);
    sonus_reverberationis_proximum(&aula_probanda, 1.0f, &l, &r);
    per (k = ZEPHYRUM; k < M; k++)
    {
        sonus_reverberationis_proximum(&aula_probanda, 0.0f, &l, &r);
    }
    forma.aula = 0.3f;
    sonus_reverberationem_ponere(&aula_probanda, &forma);
    bene = FALSUM;
    per (k = ZEPHYRUM; k < M; k++)
    {
        sonus_reverberationis_proximum(&aula_probanda, 0.0f, &l, &r);
        bene = bene || l != 0.0f;
    }
    CREDO_VERUM (bene);
    sonus_reverberationem_parare(&aula_probanda, &forma);
    sonus_reverberationis_proximum(&aula_probanda, 0.0f, &l, &r);
    CREDO_VERUM (l == 0.0f && r == 0.0f);
}


    /* ==================================================
     * RETENTIO et COMMINUTIO
     * ================================================== */

interior vacuum
_sectio_asperitas (
    CredoContextus* c)
{
    SonusRetentio r;
              i32 k;
              b32 bene = VERUM;

    interior constans f32 exspectata[IX] = {
        0.0f, 0.0f, 0.0f, 0.0f, 4.0f, 4.0f, 4.0f, 4.0f, 8.0f };

    (vacuum)c;
    sonus_retentio_parare(&r, IV);
    per (k = ZEPHYRUM; k < IX; k++)
    {
        si (sonus_retentio_proximum(&r, (f32)k) != exspectata[k])
        {
            bene = FALSUM;
        }
    }
    CREDO_VERUM (bene);

    sonus_retentio_parare(&r, I);
    CREDO_F32_PROXIMUS (sonus_retentio_proximum(&r, 0.3f), 0.3f, 0.0f);
    CREDO_F32_PROXIMUS (sonus_retentio_proximum(&r, 0.4f), 0.4f, 0.0f);
    sonus_retentio_parare(&r, ZEPHYRUM);
    CREDO_F32_PROXIMUS (sonus_retentio_proximum(&r, 0.5f), 0.5f, 0.0f);

    /* passus 2/XVI = 0.125 */
    CREDO_F32_PROXIMUS (sonus_comminuere(0.3f, XVI), 0.25f, 0.0f);
    CREDO_F32_PROXIMUS (sonus_comminuere(0.32f, XVI), 0.375f, 0.0f);
    CREDO_F32_PROXIMUS (sonus_comminuere(-0.3f, XVI), -0.25f, 0.0f);
    CREDO_F32_PROXIMUS (sonus_comminuere(1.0f, XVI), 1.0f, 0.0f);
    CREDO_F32_PROXIMUS (sonus_comminuere(0.5f, II), 1.0f, 0.0f);
    CREDO_F32_PROXIMUS (sonus_comminuere(0.3f, I), 0.3f, 0.0f);
    CREDO_F32_PROXIMUS (sonus_comminuere(0.3f, ZEPHYRUM), 0.3f, 0.0f);
}


    /* ==================================================
     * ALTITUDO: semitonia et notae MIDI
     * ================================================== */

interior vacuum
_sectio_altitudo (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_F32_PROXIMUS (sonus_ratio_semitoniorum(12.0f), 2.0f,
        0.000001f);
    CREDO_F32_PROXIMUS (sonus_ratio_semitoniorum(-12.0f), 0.5f,
        0.000001f);
    CREDO_F32_PROXIMUS (sonus_ratio_semitoniorum(0.0f), 1.0f, 0.0f);
    CREDO_F32_PROXIMUS (sonus_ratio_semitoniorum(7.0f), 1.4983071f,
                        0.000001f);
    CREDO_F32_PROXIMUS (sonus_notae_frequentia(69.0f), 440.0f, 0.001f);
    CREDO_F32_PROXIMUS (sonus_notae_frequentia(81.0f), 880.0f, 0.001f);
    CREDO_F32_PROXIMUS (sonus_notae_frequentia(60.0f), 261.62556f,
        0.001f);
    CREDO_F32_PROXIMUS (sonus_notae_frequentia(69.5f), 452.89300f,
        0.01f);
}


    /* ==================================================
     * DETERMINISMUS: catena tota bis, exempla octetim eadem
     * ================================================== */

interior f32
_catena (
          SonusUnda* pulsus,
          SonusUnda* strepitus,
    SonusInvolucrum* inv,
         SonusColum* colum,
      SonusRetentio* r)
{
    f32 x = 0.5f * sonus_unda_proximum(pulsus)
          + 0.5f * sonus_unda_proximum(strepitus);

    x = x * sonus_involucrum_proximum(inv);
    x = sonus_colum_proximum(colum, x);
    redde sonus_comminuere(sonus_retentio_proximum(r, x), XVI);
}

interior vacuum
_catenam_parare (
          SonusUnda* pulsus,
          SonusUnda* strepitus,
    SonusInvolucrum* inv,
         SonusColum* colum,
      SonusRetentio* r)
{
    SonusInvolucriForma forma;

    forma.impetus   = 0.005f;
    forma.lapsus    = 0.1f;
    forma.tenor     = 0.3f;
    forma.remissio  = 0.2f;
    sonus_unda_parare(pulsus, SONUS_UNDA_PULSUS, ZEPHYRUM);
    sonus_unda_frequentiam_ponere(pulsus, 330.0f);
    sonus_unda_officium_ponere(pulsus, 0.125f);
    sonus_unda_parare(strepitus, SONUS_UNDA_STREPITUS, XLII);
    sonus_unda_frequentiam_ponere(strepitus, 2000.0f);
    sonus_involucrum_parare(inv, &forma);
    sonus_involucrum_aperire(inv);
    sonus_colum_parare(colum, SONUS_COLUM_GRAVIA, 3000.0f);
    sonus_retentio_parare(r, III);
}

interior vacuum
_sectio_determinismus (
    CredoContextus* c)
{
    SonusUnda       p1, p2, s1, s2;
    SonusInvolucrum i1, i2;
    SonusColum      c1, c2;
    SonusRetentio   r1, r2;
                i32 k;
                b32 pares     = VERUM;
                b32 sonantia  = FALSUM;

    (vacuum)c;
    _catenam_parare(&p1, &s1, &i1, &c1, &r1);
    _catenam_parare(&p2, &s2, &i2, &c2, &r2);
    per (k = ZEPHYRUM; k < SONUS_FREQUENTIA; k++)
    {
        f32 a;
        f32 b;

        si (k == SONUS_FREQUENTIA / II)
        {
            sonus_involucrum_claudere(&i1);
            sonus_involucrum_claudere(&i2);
        }
        a = _catena(&p1, &s1, &i1, &c1, &r1);
        b = _catena(&p2, &s2, &i2, &c2, &r2);
        si (a != b)
        {
            pares = FALSUM;
        }
        si (a != 0.0f)
        {
            sonantia = VERUM;
        }
    }
    CREDO_VERUM (pares);
    CREDO_VERUM (sonantia);
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "frequentia", _sectio_frequentia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "officium", _sectio_officium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fines", _sectio_fines, NIHIL, NIHIL, NIHIL, NIHIL },
    { "strepitus", _sectio_strepitus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "involucrum", _sectio_involucrum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "repercussio", _sectio_repercussio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "colum", _sectio_colum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "resonans", _sectio_resonans, NIHIL, NIHIL, NIHIL, NIHIL },
    { "reverberatio", _sectio_reverberatio, _parare_caudam, NIHIL,
        NIHIL, NIHIL },
    { "asperitas", _sectio_asperitas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "altitudo", _sectio_altitudo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "determinismus", _sectio_determinismus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("sonus", SECTIONES);
}
