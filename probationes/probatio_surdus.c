/* probatio_surdus.c - Probationes surdorum
 *
 * ORACULUM: extensio - basis sqrt(P_S) in Q(cos 2 pi/120) per
 * algebraicus_radix_quadrata (summae Gaussianae), signum per
 * algebraicus_signum (certificatum bitorum): algorithmus alienus.
 * Omne signum quod surdus reddit (VERUM) oraculo aequale esse debet,
 * cum filtro et sine eo (gradus 3 directe).
 *
 * PROPE NULLUM: unitates Pell (x^2 - d y^2 = 1, ergo x - y sqrt d =
 * 1/(x + y sqrt d) minimum, coefficientibus magnis) earumque producta et
 * differentiae per sqrt2, sqrt3, sqrt5 - filtrum cedere cogitur, gradus 3
 * decernit aut (quadratis s64 excedentibus) RECUSAT. Probatur gradum 3
 * et recusationem utramque re vera accidisse. Vide lib/surdus.worklog.md.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "sors.h"
#include "fractio.h"
#include "extensio.h"
#include "surdus.h"
#include "surdus_interna.h"
#include <stdio.h>

interior Piscina*     piscina;
interior Extensio*    corpus;
interior Algebraicus  basis_oraculi[VIII];
interior SurdiSpatium spatium;

interior s32 casus_oraculi   = ZEPHYRUM;
interior s32 casus_gradus_iii = ZEPHYRUM;
interior s32 casus_recusati   = ZEPHYRUM;

/* signum oraculi (extensio) */
interior s32
_signum_oraculi (
    Surdus x)
{
    Algebraicus summa = algebraicus_ex_fractione(corpus,
        fractio_ex_s64(0),
        piscina);
    s32 s;
    s32 signum = ZEPHYRUM;

    per (s = ZEPHYRUM; s < VIII; s++)
    {
        si (x.c[s] != 0)
        {
            summa = algebraicus_adde(summa, algebraicus_multiplica(
                algebraicus_ex_fractione(corpus, fractio_ex_s64(x.c[s]),
                piscina), basis_oraculi[s], piscina), piscina);
        }
    }
    si (!algebraicus_signum(summa, piscina, &signum))
    {
        imprimere("  ORACULUM RECUSAVIT\n");
        redde XCIX;
    }
    redde signum;
}

/* surdus (cum filtro et sine) contra oraculum; VERUM si concordant aut
 * surdus recte recusat */
interior b32
_conferre (
    constans character* titulus,
                Surdus  x)
{
    PiscinaNotatio nota      = piscina_notare(piscina);
               s32 oraculum  = _signum_oraculi(x);
               s32 s1        = XCIX;
               s32 s2        = XCIX;
               s32 g1        = ZEPHYRUM;
               b32 v1 = surdi_signum_gradu(&spatium, x,
                   VERUM, &s1,
                   &g1);
               b32 v2 = surdi_signum_gradu(&spatium, x,
                   FALSUM, &s2,
                   NIHIL);
               b32 v3;
               s32 s3 = XCIX;

    piscina_reficere(piscina, nota);
    casus_oraculi++;
    v3 = surdus_signum(&spatium, x, &s3);
    si (v1 && g1 == III)
    {
        casus_gradus_iii++;
    }
    si (!v1)
    {
        casus_recusati++;
    }
    si (   (v1 && s1 != oraculum) || (v2 && s2 != oraculum) || v3 != v1
        || (v3 && s3 != s1) || oraculum == XCIX)
    {
        imprimere("  FRACTA %s: oraculum %d, filtro %d/%d (gradus %d), "
            "sine %d/%d, publicum %d/%d\n", titulus, oraculum, v1, s1,
            g1,
            v2, s2, v3, s3);
        redde FALSUM;
    }
    redde VERUM;
}

interior Surdus
_surdus (
    s64 c0,
    s64 c1,
    s64 c2,
    s64 c3,
    s64 c4,
    s64 c5,
    s64 c6,
    s64 c7)
{
    Surdus x;

    x.c[ZEPHYRUM]  = c0;
    x.c[I]         = c1;
    x.c[II]        = c2;
    x.c[III]       = c3;
    x.c[IV]        = c4;
    x.c[V]         = c5;
    x.c[VI]        = c6;
    x.c[VII]       = c7;
    redde x;
}

/* x - y sqrt(P_S) */
interior Surdus
_unitas (
    s64 x,
    s64 y,
    i32 selectio)
{
    Surdus u = surdus_ex_s64(x);

    u.c[selectio] = -y;
    redde u;
}

interior b32
_aequales (
    Surdus a,
    Surdus b)
{
    s32 s;

    per (s = ZEPHYRUM; s < VIII; s++)
    {
        si (a.c[s] != b.c[s])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

s32 principale (vacuum)
{
    piscina = piscina_generare_dynamicum("probatio_surdus", 1 << 20);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * SPATIUM
     * ================================================== */

    {
                 s32 primi[IV];
        SurdiSpatium sp;

        imprimere("\n--- Probans spatium ---\n");
        primi[ZEPHYRUM]  = II;
        primi[I]         = III;
        primi[II]        = V;
        primi[III]       = VII;
        CREDO_VERUM (surdi_spatium(primi, III, &spatium));
        CREDO_AEQUALIS_S64 (spatium.producta[VII], XXX);
        CREDO_AEQUALIS_S64 (spatium.producta[V], X);
        CREDO_VERUM (surdi_spatium(primi, ZEPHYRUM, &sp));
        CREDO_VERUM (surdi_spatium(primi + III, I, &sp));
        sp.numerus = XCIX;
        CREDO_FALSUM (surdi_spatium(primi, IV, &sp));
        CREDO_AEQUALIS_S32 (sp.numerus, XCIX);
        CREDO_FALSUM (surdi_spatium(primi, -(s32)I, &sp));
        primi[ZEPHYRUM] = IV;
        CREDO_FALSUM (surdi_spatium(primi, I, &sp));
        primi[ZEPHYRUM] = I;
        CREDO_FALSUM (surdi_spatium(primi, I, &sp));
        primi[ZEPHYRUM]  = III;
        primi[I]         = II;
        CREDO_FALSUM (surdi_spatium(primi, II, &sp));
        primi[I]        = III;
        CREDO_FALSUM (surdi_spatium(primi, II, &sp));
        primi[ZEPHYRUM] = 32749;
        CREDO_VERUM (surdi_spatium(primi, I, &sp));
        primi[ZEPHYRUM] = 65537;
        CREDO_FALSUM (surdi_spatium(primi, I, &sp));
        /* 2^31 - 1: limes ANTE primalitatem (d*d s32 excederet -
         * recensio S1) */
        primi[ZEPHYRUM] = 2147483647;
        CREDO_FALSUM (surdi_spatium(primi, I, &sp));
    }

    /* oraculum: Q(cos 2 pi/120) continet sqrt 2, 3, 5 */
    corpus = extensio_cosinus(CXX, piscina);
    CREDO_NON_NIHIL (corpus);
    {
        s32 s;

        basis_oraculi[ZEPHYRUM] = algebraicus_ex_fractione(corpus,
            fractio_ex_s64(I), piscina);
        per (s = I; s < VIII; s++)
        {
            CREDO_VERUM (algebraicus_radix_quadrata(corpus,
                spatium.producta[s], piscina, &basis_oraculi[s]));
        }
    }


    /* ==================================================
     * ARITHMETICA
     * ================================================== */

    {
        Surdus r;
        Surdus x;
        Surdus y;

        imprimere("\n--- Probans arithmeticam ---\n");
        /* sqrt2 sqrt2 = 2; sqrt6 sqrt3 = 3 sqrt2; (sqrt2 + sqrt3)^2 =
         * 5 + 2 sqrt6; sqrt10 sqrt15 = 5 sqrt6 */
        CREDO_VERUM (surdus_basis(&spatium, I, I, &x));
        CREDO_VERUM (surdus_multiplica(&spatium, x, x, &r));
        CREDO_VERUM (_aequales(r, surdus_ex_s64(II)));
        CREDO_VERUM (surdus_basis(&spatium, III, I, &x));
        CREDO_VERUM (surdus_basis(&spatium, II, I, &y));
        CREDO_VERUM (surdus_multiplica(&spatium, x, y, &r));
        CREDO_VERUM (_aequales(r, _surdus(0, 3, 0, 0, 0, 0, 0, 0)));
        x = _surdus(0, 1, 1, 0, 0, 0, 0, 0);
        CREDO_VERUM (surdus_multiplica(&spatium, x, x, &r));
        CREDO_VERUM (_aequales(r, _surdus(5, 0, 0, 2, 0, 0, 0, 0)));
        CREDO_VERUM (surdus_basis(&spatium, V, I, &x));
        CREDO_VERUM (surdus_basis(&spatium, VI, I, &y));
        CREDO_VERUM (surdus_multiplica(&spatium, x, y, &r));
        CREDO_VERUM (_aequales(r, _surdus(0, 0, 0, 5, 0, 0, 0, 0)));
        CREDO_FALSUM (surdus_basis(&spatium, VIII, I, &x));

        /* excessus recusatur, exitus intactus */
        r = surdus_ex_s64(VII);
        CREDO_FALSUM (surdus_adde(surdus_ex_s64(0x7FFFFFFFFFFFFFFFLL),
            surdus_ex_s64(I), &r));
        CREDO_FALSUM (surdus_subtrahe(surdus_ex_s64(-II),
            surdus_ex_s64(0x7FFFFFFFFFFFFFFFLL), &r));
        CREDO_FALSUM (surdus_subtrahe(surdus_ex_s64(ZEPHYRUM),
            surdus_ex_s64(-0x7FFFFFFFFFFFFFFFLL - 1LL), &r));
        CREDO_FALSUM (surdus_scala(surdus_ex_s64(0x100000000LL),
            0x80000000LL, &r));
        CREDO_FALSUM (surdus_multiplica(&spatium,
            _surdus(0, 0x80000000LL, 0, 0, 0, 0, 0, 0),
            _surdus(0, 0x80000000LL, 0, 0, 0, 0, 0, 0), &r));
        CREDO_VERUM (_aequales(r, surdus_ex_s64(VII)));
        /* via celeris: k in limite necessarius (recensio S2) - omnes
         * VIII coefficientes 2^29 - 1 super {2, 3, 5}: bA + bB + bP = 63
         * sine k, verum productum s64 excedit -> RECUSARI debet */
        {
            Surdus magnus;
               s32 j;

            per (j = ZEPHYRUM; j < VIII; j++)
            {
                magnus.c[j] = 0x1FFFFFFFLL;
            }
            r = surdus_ex_s64(VII);
            CREDO_FALSUM (surdus_multiplica(&spatium, magnus, magnus,
                &r));
            CREDO_VERUM (_aequales(r, surdus_ex_s64(VII)));
        }
        /* coefficiens extra basin (S >= 2^k) recusatur (recensio S3) */
        {
            SurdiSpatium duo;
                     s32 primi_duo[II];
                     s32 signum = XCIX;

            primi_duo[ZEPHYRUM]  = II;
            primi_duo[I]         = III;
            CREDO_VERUM (surdi_spatium(primi_duo, II, &duo));
            x = _surdus(1, 0, 0, 0, 5, 0, 0, 0);
            CREDO_FALSUM (surdus_signum(&duo, x, &signum));
            CREDO_AEQUALIS_S32 (signum, XCIX);
            CREDO_FALSUM (surdus_multiplica(&duo, x, surdus_ex_s64(I),
                &r));
            CREDO_FALSUM (surdus_compara(&duo, x, surdus_ex_s64(0),
                &signum));
        }
        /* in ipso limite ACCIPITUR (custodia non nimis stricta) */
        CREDO_VERUM (surdus_subtrahe(surdus_ex_s64(-1LL),
            surdus_ex_s64(0x7FFFFFFFFFFFFFFFLL), &r));
        CREDO_VERUM (r.c[ZEPHYRUM] == -0x7FFFFFFFFFFFFFFFLL - 1LL);
        CREDO_VERUM (surdus_subtrahe(surdus_ex_s64(0x7FFFFFFFFFFFFFFELL),
            surdus_ex_s64(-1LL), &r));
        CREDO_AEQUALIS_S64 (r.c[ZEPHYRUM], 0x7FFFFFFFFFFFFFFFLL);
        CREDO_VERUM (surdus_adde(surdus_ex_s64(0x7FFFFFFFFFFFFFFELL),
            surdus_ex_s64(1LL), &r));
        CREDO_AEQUALIS_S64 (r.c[ZEPHYRUM], 0x7FFFFFFFFFFFFFFFLL);
        CREDO_VERUM (surdus_adde(surdus_ex_s64(-0x7FFFFFFFFFFFFFFFLL),
            surdus_ex_s64(-1LL), &r));
        CREDO_VERUM (r.c[ZEPHYRUM] == -0x7FFFFFFFFFFFFFFFLL - 1LL);
        /* limes: (2^31 sqrt2)^2 = 2^63 recusatur; (2^30 sqrt2)^2 = 2^61 */
        CREDO_VERUM (surdus_multiplica(&spatium,
            _surdus(0, 0x40000000LL, 0, 0, 0, 0, 0, 0),
            _surdus(0, 0x40000000LL, 0, 0, 0, 0, 0, 0), &r));
        CREDO_AEQUALIS_S64 (r.c[ZEPHYRUM], 0x2000000000000000LL);
    }

    /* multiplicatio: via celeris et custodita (bita > 63) contra oraculum:
     * imago producti == productum imaginum */
    {
        Sors s;
         s32 k;
         b32 bene = VERUM;

        imprimere("\n--- Probans multiplicationem contra oraculum ---\n");
        sors_seminare(&s, 2026ULL, VII);
        per (k = ZEPHYRUM; k < CC; k++)
        {
            PiscinaNotatio nota = piscina_notare(piscina);
                    Surdus a;
                    Surdus b;
                    Surdus r;
                       s32 j;
                       s32 amplitudo  = k < C ? X : XXVIII;   /* bita */
                       s32 signum     = ZEPHYRUM;

            per (j = ZEPHYRUM; j < VIII; j++)
            {
                a.c[j] = (s64)sors_inter(&s, -(s32)(I << (amplitudo
                    - I)),
                    (s32)(I << (amplitudo - I)));
                b.c[j] = (s64)sors_inter(&s, -(s32)(I << (amplitudo
                    - I)),
                    (s32)(I << (amplitudo - I)));
            }
            si (surdus_multiplica(&spatium, a, b, &r))
            {
                Algebraicus imago_a = basis_oraculi[ZEPHYRUM];
                Algebraicus imago_b;
                Algebraicus imago_producti;
                        s32 t;

                imago_a = algebraicus_ex_fractione(corpus,
                    fractio_ex_s64(0),
                    piscina);
                imago_b         = imago_a;
                imago_producti  = imago_a;
                per (t = ZEPHYRUM; t < VIII; t++)
                {
                    imago_a = algebraicus_adde(imago_a,
                        algebraicus_multiplica(
                        algebraicus_ex_fractione(corpus,
                        fractio_ex_s64(a.c[t]), piscina),
                        basis_oraculi[t],
                        piscina), piscina);
                    imago_b = algebraicus_adde(imago_b,
                        algebraicus_multiplica(
                        algebraicus_ex_fractione(corpus,
                        fractio_ex_s64(b.c[t]), piscina),
                        basis_oraculi[t],
                        piscina), piscina);
                    imago_producti = algebraicus_adde(imago_producti,
                        algebraicus_multiplica(
                        algebraicus_ex_fractione(corpus,
                        fractio_ex_s64(r.c[t]), piscina),
                        basis_oraculi[t],
                        piscina), piscina);
                }
                si (   !algebraicus_signum(algebraicus_subtrahe(
                        algebraicus_multiplica(imago_a, imago_b,
                        piscina), imago_producti, piscina),
                        piscina, &signum) || signum != 0)
                {
                    imprimere("  FRACTA multiplicatio k=%d\n", k);
                    bene = FALSUM;
                }
            }
            alioquin si (amplitudo == X)
            {
                imprimere("  FRACTA multiplicatio parva recusata k=%d\n",
                    k);
                bene = FALSUM;
            }
            piscina_reficere(piscina, nota);
        }
        CREDO_VERUM (bene);
    }


    /* ==================================================
     * SIGNUM CONTRA ORACULUM: casu
     * ================================================== */

    {
        Sors s;
         s32 k;
         b32 bene = VERUM;

        imprimere("\n--- Probans signum casu ---\n");
        sors_seminare(&s, 2026ULL, XI);
        per (k = ZEPHYRUM; k < M; k++)
        {
            Surdus x;
               s32 j;
               s32 amplitudo = k < D ? III : L;

            per (j = ZEPHYRUM; j < VIII; j++)
            {
                /* coefficientes multi nulli: subspatia quoque */
                x.c[j] = sors_intra(&s, III) == 0 ? 0
                    : (s64)sors_inter(&s, -amplitudo, amplitudo);
            }
            bene = bene && _conferre("casu", x);
        }
        CREDO_VERUM (bene);
    }


    /* ==================================================
     * PROPE NULLUM: unitates Pell, producta, differentiae
     * ================================================== */

    {
        /* unitates fundamentales x^2 - d y^2 = 1 et subsets */
        constans s64 x1[III]   = { 3, 2, 9 };
        constans s64 y1[III]   = { 2, 1, 4 };
        constans s64 d[III]    = { 2, 3, 5 };
        constans i32 sub[III]  = { 1, 2, 4 };
              Surdus unitates[III][XXIV];
                 s32 numeri[III];
                 s32 decisi_parvi = ZEPHYRUM;
                 s32 q;
                 b32 bene = VERUM;

        imprimere("\n--- Probans prope nullum (Pell) ---\n");
        per (q = ZEPHYRUM; q < III; q++)
        {
            s64 x = x1[q];
            s64 y = y1[q];
            s32 n = ZEPHYRUM;

            /* usque ad x ~ 2^62 (quadrata certe excedunt: recusatio) */
            dum (n < XXIV && x < 0x1000000000000000LL / x1[q])
            {
                s64 x_proximus = x1[q] * x + d[q] * y1[q] * y;
                s64 y_proximus = x1[q] * y + y1[q] * x;

                unitates[q][n] = _unitas(x, y, sub[q]);
                bene = bene && _conferre("Pell", unitates[q][n]);
                /* x < 2^31: quadrata < 2^62 - DECERNI debet, non
                 * recusari (recusatio nimia = tier 3 inutilis) */
                si (x < 0x80000000LL)
                {
                    s32 signum = ZEPHYRUM;

                    si (   !surdus_signum(&spatium, unitates[q][n],
                        &signum)
                        || signum != I)
                    {
                        imprimere("  FRACTA Pell x=%lld recusata aut falsa\n",
                            (long long)x);
                        bene = FALSUM;
                    }
                    decisi_parvi++;
                }
                {
                    Surdus negata;

                    (void)surdus_scala(unitates[q][n], -1LL, &negata);
                    bene = bene && _conferre("Pell negata", negata);
                }
                /* x - y sqrt d - (x' - y' sqrt d) cum x' unitate
                 * proxima: differentia duorum minimorum */
                x = x_proximus;
                y = y_proximus;
                n++;
            }
            numeri[q] = n;
        }
        /* producta et differentiae trans primos */
        {
            s32 i;
            s32 j;

            per (i = ZEPHYRUM; i < numeri[ZEPHYRUM]; i++)
            {
                per (j = ZEPHYRUM; j < numeri[I]; j++)
                {
                    Surdus p;
                    Surdus df;

                    si (surdus_multiplica(&spatium,
                        unitates[ZEPHYRUM][i],
                            unitates[I][j], &p))
                    {
                        bene = bene && _conferre("productum 2x3", p);
                        si (j < numeri[II])
                        {
                            Surdus p3;

                            si (surdus_multiplica(&spatium, p,
                                    unitates[II][j], &p3))
                            {
                                bene = bene
                                    && _conferre("productum 2x3x5",
                                    p3);
                            }
                        }
                    }
                    si (surdus_subtrahe(unitates[ZEPHYRUM][i],
                            unitates[I][j], &df))
                    {
                        bene = bene && _conferre("differentia 2-3", df);
                    }
                }
            }
        }
        CREDO_VERUM (bene);
        imprimere("  casus oraculi %d, gradus III %d, recusati %d, Pell "
            "parvi decisi %d\n", casus_oraculi, casus_gradus_iii,
            casus_recusati, decisi_parvi);
        CREDO_MAIOR_S32 (decisi_parvi, XX);
        /* utrumque re vera accidit */
        CREDO_MAIOR_S32 (casus_gradus_iii, L);
        CREDO_MAIOR_S32 (casus_recusati, X);
    }

    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
