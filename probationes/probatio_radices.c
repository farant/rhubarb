/* probatio_radices.c - Probationes radicum realium
 *
 * ORACULUM (scratchpad radices/oraculum.py, Python Decimal 120 digitorum,
 * independens): radices 2 cos(2 pi k/n) polynomiorum Chebyshev 2 T_n(x/2)
 * - 2 (n = 5, 7, 12, 17, 24; radices multiplices et rationales -2, -1, 0,
 * 1, 2 inclusae) et t^7 - 2(50t - 1)^2 (Mignotte: geminae 3.2e-8 inter
 * se prope 1/50), XXV digiti versus nullum truncati. Praeterea: radices
 * rationales exactae, multiplicitas, refutationes contractus, comparatio
 * trans polynomia (aequalitas per divisorem communem), signum h(a),
 * angustatio, et numerus radicum contra extensio_radices_reales (Sturm,
 * algorithmus alienus). Vide lib/radices.worklog.md.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "chorda.h"
#include "magnus.h"
#include "fractio.h"
#include "polynomium.h"
#include "extensio.h"
#include "radices.h"
#include <stdio.h>
#include <string.h>

interior Piscina* piscina;

nomen structura {
    constans character* polynomium;
                   i32  numerus;
    constans character* radices[XIII];
} Casus;

interior constans Casus exempla[] = {
    { "t^5 - 5t^3 + 5t - 2", 3,
      { "-1.6180339887498948482045868",
        "0.6180339887498948482045868",
        "2.0000000000000000000000000" } },
    { "t^7 - 7t^5 + 14t^3 - 7t - 2", 4,
      { "-1.8019377358048382524722046",
        "-0.4450418679126288085778051",
        "1.2469796037174670610500097",
        "2.0000000000000000000000000" } },
    { "t^12 - 12t^10 + 54t^8 - 112t^6 + 105t^4 - 36t^2", 7,
      { "-2.0000000000000000000000000",
        "-1.7320508075688772935274463",
        "-1.0000000000000000000000000",
        "0.0000000000000000000000000",
        "1.0000000000000000000000000",
        "1.7320508075688772935274463",
        "2.0000000000000000000000000" } },
    { "t^17 - 17t^15 + 119t^13 - 442t^11 + 935t^9 - 1122t^7 + 714t^5 - 204t^3 + 17t - 2",
        9,
      { "-1.9659461993678035565638976",
        "-1.7004342714592283042682878",
        "-1.2052692727585127783571763",
        "-0.5473259801441657270781558",
        "0.1845367189266039904793022",
        "0.8914767115530765347929150",
        "1.4780178344413182318490686",
        "1.8649444588087116091462317",
        "2.0000000000000000000000000" } },
    { "t^24 - 24t^22 + 252t^20 - 1520t^18 + 5814t^16 - 14688t^14 + 24752t^12 - 27456t^10 + 19305t^8 - 8008t^6 + 1716t^4 - 144t^2",
        13,
      { "-2.0000000000000000000000000",
        "-1.9318516525781365734994863",
        "-1.7320508075688772935274463",
        "-1.4142135623730950488016887",
        "-1.0000000000000000000000000",
        "-0.5176380902050415246977976",
        "0.0000000000000000000000000",
        "0.5176380902050415246977976",
        "1.0000000000000000000000000",
        "1.4142135623730950488016887",
        "1.7320508075688772935274463",
        "1.9318516525781365734994863",
        "2.0000000000000000000000000" } },
    { "t^7 - 5000t^2 + 200t - 2", 3,
      { "0.0199999840000447998297607",
        "0.0200000160000448001702407",
        "5.4847822366403647432879401" } },
};

interior Polynomium
_p (
    constans character* textus)
{
    Polynomium p = polynomium_nullum();

    si (!polynomium_ex_chorda(chorda_ex_literis(textus, piscina), 't',
        piscina, &p))
    {
        imprimere("  lectio fracta: \"%s\"\n", textus);
    }
    redde p;
}

interior b32
_textus_aequalis (
                chorda  c,
    constans character* expectatum)
{
    redde c.mensura == (i32)strlen(expectatum)
        && memcmp(c.datum, expectatum, (size_t)c.mensura) == 0;
}

interior b32
_radices (
    constans character*  textus,
           RadixRealis** r,
                   i32*  n)
{
    redde radices_reales(_p(textus), piscina, r, n);
}

s32 principale (vacuum)
{
    piscina = piscina_generare_dynamicum("probatio_radices", 1 << 22);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * CONTRA ORACULUM
     * ================================================== */

    {
        i32 k;
        b32 bene = VERUM;

        imprimere("\n--- Probans radices contra oraculum ---\n");
        per (k = ZEPHYRUM; k < magnitudo(exempla)
            / magnitudo(exempla[0]); k++)
        {
            RadixRealis* r;
                    i32  n = ZEPHYRUM;
                    i32  j;
                    i32  n_sturm = ZEPHYRUM;

            si (   !_radices(exempla[k].polynomium, &r, &n)
                || n != exempla[k].numerus)
            {
                imprimere("  FRACTA %s: numerus %u (exspectatus %u)\n",
                    exempla[k].polynomium, n, exempla[k].numerus);
                bene = FALSUM;
                perge;
            }
            per (j = ZEPHYRUM; j < n; j++)
            {
                chorda c;

                si (   !radix_ad_chordam(r[j], XXV, piscina, &c)
                    || !_textus_aequalis(c, exempla[k].radices[j]))
                {
                    imprimere("  FRACTA %s radix %u: %.*s (exspectata %s)\n",
                        exempla[k].polynomium, j, (int)c.mensura,
                        (constans character*)c.datum,
                        exempla[k].radices[j]);
                    bene = FALSUM;
                }
                /* ordo strictus crescens */
                si (j > ZEPHYRUM)
                {
                    s32 s = ZEPHYRUM;

                    bene = bene
                        && radix_compara(r[j - I], r[j], piscina, &s)
                        && s < ZEPHYRUM;
                }
            }
            /* numerus contra Sturm (extensio) */
            bene = bene
                && extensio_radices_reales(_p(exempla[k].polynomium),
                piscina, &n_sturm) && n_sturm == n;
        }
        CREDO_VERUM (bene);
    }


    /* ==================================================
     * RATIONALES, MULTIPLICITAS, REFUTATIONES
     * ================================================== */

    {
        RadixRealis* r;
                i32  n = ZEPHYRUM;
             chorda  c;
            Fractio  q = fractio_ex_s64(ZEPHYRUM);
                s32  s = XCIX;

        imprimere("\n--- Probans rationales et contractum ---\n");
        /* (2t - 1)(3t + 5) t: -5/3 (non dyadica: intervallum), 0, 1/2 */
        CREDO_VERUM (_radices("6t^3 + 7t^2 - 5t", &r, &n));
        CREDO_AEQUALIS_I32 (n, III);
        CREDO_VERUM (fractio_ex_s64_s64(-V, III, piscina, &q));
        CREDO_VERUM (radix_compara(r[ZEPHYRUM], radix_ex_fractione(q,
            piscina),
            piscina, &s));
        CREDO_AEQUALIS_S32 (s, ZEPHYRUM);
        CREDO_VERUM (radix_ad_chordam(r[ZEPHYRUM], VI, piscina, &c)
            && _textus_aequalis(c, "-1.666666"));
        CREDO_VERUM (fractio_compara(r[I].infra, r[I].supra, piscina)
            == 0
            && fractio_signum(r[I].infra) == 0);
        CREDO_VERUM (radix_ad_chordam(r[II], III, piscina, &c)
            && _textus_aequalis(c, "0.500"));
        /* multiplicitas abicitur: (t - 1)^3 (t + 2)^2 */
        CREDO_VERUM (_radices("t^5 + t^4 - 5t^3 - t^2 + 8t - 4", &r,
            &n));
        CREDO_AEQUALIS_I32 (n, II);
        /* nullae, constans */
        CREDO_VERUM (_radices("t^2 + 1", &r, &n));
        CREDO_AEQUALIS_I32 (n, ZEPHYRUM);
        CREDO_VERUM (_radices("-7", &r, &n));
        CREDO_AEQUALIS_I32 (n, ZEPHYRUM);
        /* coefficientes nulli in mediis: nullae radices reales */
        {
            constans character* sine[V] = { "t^4 + 1", "t^6 + 1",
                "t^4 + 5t^2 + 4", "t^8 + t^4 + 1", "t^10 + 3t^6 + 2" };
            i32 j;

            per (j = ZEPHYRUM; j < V; j++)
            {
                CREDO_VERUM (_radices(sine[j], &r, &n));
                CREDO_AEQUALIS_I32 (n, ZEPHYRUM);
            }
            /* nulla in transformatis (x+1)^n Q(1/(x+1)) inter signa
             * aequalia: variatio falsa radicem inveniret (planta R1:
             * 184 / 4000 casu). Numeri per discriminantem: 2t^4 - 2t^2 + 1
             * (u^2: 4 - 8 < 0) nullae; -3t^2 + 2t - 1 nullae; t(2t^2 + t +
             * 2) una */
            CREDO_VERUM (_radices("2t^4 - 2t^2 + 1", &r, &n));
            CREDO_AEQUALIS_I32 (n, ZEPHYRUM);
            CREDO_VERUM (_radices("-3t^2 + 2t - 1", &r, &n));
            CREDO_AEQUALIS_I32 (n, ZEPHYRUM);
            CREDO_VERUM (_radices("2t^3 + t^2 + 2t", &r, &n));
            CREDO_AEQUALIS_I32 (n, I);
            CREDO_VERUM (_radices("t^5 - t", &r, &n));
            CREDO_AEQUALIS_I32 (n, III);
            CREDO_VERUM (_radices("t^9 - 2t^5 + t", &r, &n));  /* t(t^4-1)^2 */
            CREDO_AEQUALIS_I32 (n, III);
        }
        /* refutationes */
        CREDO_FALSUM (_radices("0", &r, &n));
        CREDO_FALSUM (_radices("t^-1 + 1", &r, &n));
    }


    /* ==================================================
     * COMPARATIO TRANS POLYNOMIA, SIGNUM, ANGUSTATIO
     * ================================================== */

    {
        RadixRealis* a;
        RadixRealis* b;
        RadixRealis* c3;
                i32  na        = ZEPHYRUM;
                i32  nb        = ZEPHYRUM;
                i32  nc        = ZEPHYRUM;
                s32  s         = XCIX;
            Fractio  q         = fractio_ex_s64(ZEPHYRUM);
            Fractio  latitudo  = fractio_ex_s64(ZEPHYRUM);
        RadixRealis  angusta;

        imprimere("\n--- Probans comparationem, signum, angustationem ---\n");
        CREDO_VERUM (_radices("t^2 - 2", &a, &na));            /* -sqrt2, sqrt2 */
        CREDO_VERUM (_radices("t^3 - 5t^2 - 2t + 10", &b, &nb)); /* (t^2-2)(t-5) */
        CREDO_VERUM (_radices("t^2 - 3", &c3, &nc));
        CREDO_AEQUALIS_I32 (nb, III);
        /* sqrt2 ex polynomiis diversis: aequales (per divisorem) */
        CREDO_VERUM (radix_compara(a[I], b[I], piscina, &s));
        CREDO_AEQUALIS_S32 (s, ZEPHYRUM);
        CREDO_VERUM (radix_compara(a[ZEPHYRUM], b[ZEPHYRUM], piscina,
            &s));
        CREDO_AEQUALIS_S32 (s, ZEPHYRUM);
        CREDO_VERUM (radix_compara(a[I], b[II], piscina, &s));   /* sqrt2 < 5 */
        CREDO_AEQUALIS_S32 (s, -(s32)I);
        CREDO_VERUM (radix_compara(a[I], c3[I], piscina, &s));   /* < sqrt3 */
        CREDO_AEQUALIS_S32 (s, -(s32)I);
        /* sqrt2 contra 141421356/100000000 et 141421357/100000000 */
        CREDO_VERUM (fractio_ex_s64_s64(141421356LL, 100000000LL,
            piscina, &q));
        CREDO_VERUM (radix_compara(a[I], radix_ex_fractione(q, piscina),
            piscina, &s));
        CREDO_AEQUALIS_S32 (s, I);
        CREDO_VERUM (fractio_ex_s64_s64(141421357LL, 100000000LL,
            piscina, &q));
        CREDO_VERUM (radix_compara(radix_ex_fractione(q, piscina), a[I],
            piscina, &s));
        CREDO_AEQUALIS_S32 (s, I);
        /* signum h(a) */
        CREDO_VERUM (radix_signum_polynomii(_p("3t^2 - 6"), a[I],
            piscina, &s));
        CREDO_AEQUALIS_S32 (s, ZEPHYRUM);
        CREDO_VERUM (radix_signum_polynomii(_p("t - 1"), a[I], piscina,
            &s));
        CREDO_AEQUALIS_S32 (s, I);
        CREDO_VERUM (radix_signum_polynomii(_p("t"), a[ZEPHYRUM],
            piscina, &s));
        CREDO_AEQUALIS_S32 (s, -(s32)I);
        /* h radicem prope a habens: 1000000t^2 - 1999998 (sqrt(1.999998)) */
        CREDO_VERUM (radix_signum_polynomii(_p("1000000t^2 - 1999998"),
            a[I],
            piscina, &s));
        CREDO_AEQUALIS_S32 (s, I);
        CREDO_FALSUM (radix_signum_polynomii(_p("t^-2"), a[I], piscina,
            &s));
        /* radix cuius intervallum in aliis radicibus terminatur: sqrt3 ex
         * 2T_12(x/2) - 2 in (1, 2), 1 et 2 radices. h radicem communem
         * habens (et terminum 2): 0; h solum in terminis evanescens: -1 */
        {
            RadixRealis* radices_duodecim;
                    i32  numerus_duodecim = ZEPHYRUM;

            CREDO_VERUM (_radices("t^12 - 12t^10 + 54t^8 - 112t^6 + 105t^4 - 36t^2",
                &radices_duodecim, &numerus_duodecim));
            CREDO_AEQUALIS_I32 (numerus_duodecim, VII);
            CREDO_VERUM (radix_signum_polynomii(_p("t^3 - 2t^2 - 3t + 6"),
                radices_duodecim[V], piscina, &s));
            CREDO_AEQUALIS_S32 (s, ZEPHYRUM);
            CREDO_VERUM (radix_signum_polynomii(_p("t^2 - 3t + 2"),
                radices_duodecim[V],
                piscina, &s));
            CREDO_AEQUALIS_S32 (s, -(s32)I);
            CREDO_VERUM (radix_signum_polynomii(_p("t^2 - 1"),
                radices_duodecim[I], piscina,
                &s));                                    /* -sqrt3: 2 */
            CREDO_AEQUALIS_S32 (s, I);
        }
        /* angustatio infra 1e-30 */
        CREDO_VERUM (fractio_ex_magnis(magnus_ex_s64(I),
            magnus_potentia(
            magnus_ex_s64(X), XXX, piscina), piscina, &latitudo));
        CREDO_VERUM (radix_angusta(a[I], latitudo, piscina, &angusta));
        CREDO_VERUM (fractio_compara(fractio_subtrahe(angusta.supra,
            angusta.infra, piscina), latitudo, piscina) < 0);
        CREDO_VERUM (radix_compara(angusta, a[I], piscina, &s)
            && s == 0);
    }

    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
