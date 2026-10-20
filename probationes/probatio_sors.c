/* probatio_sors.c - Probationes sortis (PCG32 + SplitMix64)
 *
 * Vectores ex implementatione Pythonis INDEPENDENTI (ipsa prius contra
 * pcg32-demo canonicum et SplitMix64 semen 1234567 probata) - latera
 * duo, differentia. Vide lib/sors.worklog.md.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "sors.h"
#include <stdio.h>

interior vacuum
_vectorem_probare (
          i64  semen,
          i64  series,
    constans i32* exspectata)
{
    Sors s;
     i32 k;

    sors_seminare(&s, semen, series);
    per (k = ZEPHYRUM; k < VI; k++)
    {
        CREDO_AEQUALIS_I32 (sors_proximum(&s), exspectata[k]);
    }
}


    /* ==================================================
     * VECTORES: seminare (SplitMix64 -> PCG32) contra Pythonem
     * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        interior constans i32 v_42_54[VI] = {
            0xcd0fae4eU, 0xc3363e40U, 0xc448683dU,
            0x69675842U, 0x2794a4f8U, 0x8945b4caU };
        interior constans i32 v_0_0[VI] = {
            0xc0c7e2c5U, 0x750c892aU, 0x575c7ac6U,
            0xe20b9cecU, 0x61146cafU, 0xa95b436fU };
        interior constans i32 v_max_1[VI] = {
            0x43b49779U, 0xf99c3f44U, 0x5fee74e0U,
            0x2cabf1feU, 0xd0d9d5b6U, 0x9d73c89dU };

        _vectorem_probare(42ULL, 54ULL, v_42_54);
        _vectorem_probare(0ULL, 0ULL, v_0_0);
        _vectorem_probare(0xFFFFFFFFFFFFFFFFULL, 1ULL, v_max_1);
    }
}


    /* ==================================================
     * DETERMINISMUS et SERIES disiunctae
     * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Sors a;
        Sors b;
        Sors c;
         i32 k;
         b32 omnes_pares     = VERUM;
         b32 aliqua_diversa  = FALSUM;

        sors_seminare(&a, 7ULL, 3ULL);
        sors_seminare(&b, 7ULL, 3ULL);
        sors_seminare(&c, 7ULL, 4ULL);
        per (k = ZEPHYRUM; k < M; k++)
        {
            i32 x = sors_proximum(&a);

            si (x != sors_proximum(&b))
            {
                omnes_pares = FALSUM;
            }
            si (x != sors_proximum(&c))
            {
                aliqua_diversa = VERUM;
            }
        }
        CREDO_VERUM (omnes_pares);
        CREDO_VERUM (aliqua_diversa);
    }
}


    /* ==================================================
     * INTRA / INTER: fines
     * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Sors s;
         i32 k;
         b32 bene = VERUM;

        sors_seminare(&s, 11ULL, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (sors_intra(&s, ZEPHYRUM), ZEPHYRUM);
        per (k = ZEPHYRUM; k < C; k++)
        {
            si (sors_intra(&s, I) != ZEPHYRUM)             bene =
                                                               FALSUM;
            si (sors_intra(&s, 0x80000000U) >= 0x80000000U) bene =
                                                                FALSUM;
            si (sors_intra(&s, 0xFFFFFFFFU) >= 0xFFFFFFFFU) bene =
                                                                FALSUM;
            {
                s32 x = sors_inter(&s, -V, V);

                si (x < -V || x > V) bene = FALSUM;
            }
        }
        CREDO_VERUM (bene);
        CREDO_AEQUALIS_I32 ((i32)sors_inter(&s, VII, VII), VII);
        CREDO_AEQUALIS_I32 ((i32)sors_inter(&s, IX, III), IX);
        /* spatium totum s32: sine ruina */
        (vacuum)sors_inter(&s, (s32)0x80000000U, (s32)0x7FFFFFFF);
    }
}


    /* ==================================================
     * UNIFORMITAS grossa (determinata: semen fixum, non fluctuat)
     * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Sors s;
         i32 situlae[X];
         i32 k;
         b32 bene = VERUM;

        per (k = ZEPHYRUM; k < X; k++)
        {
            situlae[k] = ZEPHYRUM;
        }
        sors_seminare(&s, 2026ULL, ZEPHYRUM);
        per (k = ZEPHYRUM; k < C * M; k++)
        {
            situlae[sors_intra(&s, X)]++;
        }
        per (k = ZEPHYRUM; k < X; k++)
        {
            si (situlae[k] < IX * M || situlae[k] > XI * M)
            {
                bene = FALSUM;
            }
        }
        CREDO_VERUM (bene);
    }
}


    /* ==================================================
     * FLUITANTES in [0, 1)
     * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Sors s;
         i32 k;
         b32 bene = VERUM;

        sors_seminare(&s, 5ULL, ZEPHYRUM);
        per (k = ZEPHYRUM; k < M; k++)
        {
            f64 d = sors_f64(&s);
            f32 f = sors_f32(&s);

            si (d < 0.0 || d >= 1.0 || f < 0.0f || f >= 1.0f)
            {
                bene = FALSUM;
            }
        }
        CREDO_VERUM (bene);
    }
}


    /* ==================================================
     * CASU
     * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Sors s;
         i32 k;
         b32 bene = VERUM;

        sors_seminare(&s, 9ULL, ZEPHYRUM);
        per (k = ZEPHYRUM; k < C; k++)
        {
            si (sors_casu(&s, ZEPHYRUM, III)) bene = FALSUM;
            si (!sors_casu(&s, III, III))     bene = FALSUM;
        }
        CREDO_VERUM (bene);
        CREDO_FALSUM (sors_casu(&s, I, ZEPHYRUM));
    }
}


    /* ==================================================
     * MISCERE: permutatio vera, determinata, non identitas
     * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Sors s;
        Sors t;
         s32 a[X];
         s32 b[X];
         i32 k;
         i32 summa = ZEPHYRUM;
         b32 pares = VERUM;
         b32 motum = FALSUM;
         i32 visi[X];

        per (k = ZEPHYRUM; k < X; k++)
        {
            a[k]     = (s32)k;
            b[k]     = (s32)k;
            visi[k]  = ZEPHYRUM;
        }
        sors_seminare(&s, 13ULL, ZEPHYRUM);
        sors_seminare(&t, 13ULL, ZEPHYRUM);
        sors_miscere(&s, a, X, (i32)magnitudo(s32));
        sors_miscere(&t, b, X, (i32)magnitudo(s32));
        per (k = ZEPHYRUM; k < X; k++)
        {
            si (a[k] != b[k])       pares = FALSUM;
            si (a[k] != (s32)k)     motum = VERUM;
            visi[(i32)a[k]]++;
        }
        per (k = ZEPHYRUM; k < X; k++)
        {
            summa += visi[k] == I ? I : ZEPHYRUM;
        }
        CREDO_VERUM (pares);
        CREDO_VERUM (motum);
        CREDO_AEQUALIS_I32 (summa, X);
    }
}


    /* ==================================================
     * DERIVARE: parens intactus, clavis eadem = filius idem
     * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Sors p;
        Sors ante;
        Sors f1;
        Sors f1_iterum;
        Sors f2;
         i32 x1;
         i32 x2;

        sors_seminare(&p, 99ULL, ZEPHYRUM);
        ante       = p;
        f1         = sors_derivare(&p, 1ULL);
        f2         = sors_derivare(&p, 2ULL);
        f1_iterum  = sors_derivare(&p, 1ULL);
        CREDO_AEQUALIS_S64 ((s64)p.status, (s64)ante.status);
        CREDO_AEQUALIS_S64 ((s64)p.incrementum, (s64)ante.incrementum);
        x1 = sors_proximum(&f1);
        x2 = sors_proximum(&f2);
        CREDO_AEQUALIS_I32 (sors_proximum(&f1_iterum), x1);
        CREDO_VERUM (x1 != x2);
        CREDO_VERUM (x1 != sors_proximum(&p));
    }
}


    /* ==================================================
     * LEGATUM: LCG classicus ex statu 12345 (vector Pythonis)
     * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    (vacuum)c;
    {
        i32 status = 12345U;

        CREDO_AEQUALIS_I32 (sors_lcg_classicus(&status), 0xd3dc167eU);
        CREDO_AEQUALIS_I32 (sors_lcg_classicus(&status), 0xa70427dfU);
        CREDO_AEQUALIS_I32 (sors_lcg_classicus(&status), 0xd6651c2cU);
        CREDO_AEQUALIS_I32 (sors_lcg_classicus(&status), 0x0daa96f5U);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "vectores", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "determinismum", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "intra et inter", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "uniformitatem", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fluitantes", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "casum", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "miscere", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "derivare", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lcg classicum", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("sors", SECTIONES);
}
