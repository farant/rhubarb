/* probatio_extensio.c - Probationes corporum numerorum algebraicorum
 *
 * Oracula a codice independentia: (1) polynomia minima nota (Psi_5 =
 * t^2 + t - 1, Psi_7 = t^3 + t^2 - 2t - 1, Psi_9 = t^3 - 3t + 1, ...),
 * gradus phi(n)/2 per numerationem gcd, radices 2 cos(2 pi j/n) in f64;
 * (2) norma et vestigium in f64 ut productum et summa super radices
 * conjugatas (corpora cosinus), et forma clausa a^2 - d b^2 (corpora
 * quadratica, etiam imaginaria); (3) identitates corporis super
 * elementa fortuita (inversa, associativitas, distributivitas, N
 * multiplicativa, Tr additiva, textus reversibilis); (4) facta: phi^2 =
 * phi + 1, N(1 + sqrt 2) = -1, Vandermonde super Q(sqrt 5) = productum
 * differentiarum.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "chorda.h"
#include "sors.h"
#include "magnus.h"
#include "fractio.h"
#include "polynomium.h"
#include "anulus.h"
#include "matrix.h"
#include "extensio.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens Piscina* piscina;

interior Polynomium
_p (
    constans character* textus)
{
    Polynomium p = polynomium_nullum();

    si (!polynomium_ex_chorda(chorda_ex_literis(textus, piscina), 't',
            piscina, &p))
    {
        imprimere("FRACTA: polynomium malum: %s\n", textus);
    }
    redde p;
}

interior Algebraicus
_a (
     constans Extensio* k,
    constans character* textus)
{
    Algebraicus a;

    a.corpus = NIHIL;
    si (!algebraicus_ex_chorda(k, chorda_ex_literis(textus, piscina),
        piscina, &a))
    {
        imprimere("FRACTA: elementum malum: %s\n", textus);
    }
    redde a;
}

interior b32
_textus_est (
             Algebraicus  a,
      constans character* textus)
{
    redde chorda_aequalis_literis(algebraicus_ad_chordam(a, piscina),
        textus);
}

interior i32
_divisor_communis (
    i32 a,
    i32 b)
{
    dum (b != ZEPHYRUM)
    {
        i32 r = a % b;

        a = b;
        b = r;
    }
    redde a;
}

/* magnus -> f64 per textum decimalem (numeri magni extra s64) */
interior f64
_magnus_ad_f64 (
    Magnus m)
{
    redde strtod(chorda_ut_cstr(magnus_ad_chordam(m, piscina), piscina),
        NIHIL);
}

interior f64
_fractio_ad_f64 (
    Fractio q)
{
    redde _magnus_ad_f64(fractio_numerator(q)) / _magnus_ad_f64(
        fractio_denominator(q));
}

/* valor elementi ad alpha = x (f64) */
interior f64
_valor (
    Algebraicus a,
            f64 x,
            i32 d)
{
    f64 summa     = 0.0;
    f64 potentia  = 1.0;
    i32 j;

    per (j = ZEPHYRUM; j < d; j++)
    {
        summa = summa + _fractio_ad_f64(algebraicus_coefficiens(a, j,
            piscina)) * potentia;
        potentia = potentia * x;
    }
    redde summa;
}

/* elementum fortuitum: coefficientes [-9, 9], denominator [1, 9] */
interior Algebraicus
_fortuitum (
    constans Extensio* k,
                 Sors* s)
{
         Magnus c[XXIV];
     Polynomium p = polynomium_nullum();
    Algebraicus a;
            i32 j;
            i32 d = extensio_gradus(k);

    per (j = ZEPHYRUM; j < d; j++)
    {
        c[j] = magnus_ex_s64((s64)sors_inter(s, -IX, IX));
    }
    (vacuum)polynomium_ex_coefficientibus(c, d, ZEPHYRUM, piscina, &p);
    a.corpus = NIHIL;
    (vacuum)algebraicus_ex_polynomio(k, p,
        magnus_ex_s64((s64)sors_inter(
        s, I, IX)), piscina, &a);
    redde a;
}

interior b32
_unum_est (
    Algebraicus a)
{
    Fractio q = fractio_ex_s64(ZEPHYRUM);

    redde algebraicus_est_rationalis(a, piscina, &q)
        && fractio_aequalis(q, fractio_ex_s64(I));
}

s32 principale (vacuum)
{
    piscina = piscina_generare_dynamicum("probatio_extensio", 1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * CORPORA QUADRATICA
     * ================================================== */

    {
           Extensio* k5            = extensio_quadratica(V, piscina);
           Extensio* k_minus_unum  = extensio_quadratica(-I, piscina);
        Algebraicus  r;
        Algebraicus  aureus;
        Algebraicus  x;
            Fractio  q = fractio_ex_s64(ZEPHYRUM);

        imprimere("\n--- Corpora quadratica ---\n");
        CREDO_NON_NIHIL (k5);
        CREDO_NON_NIHIL (k_minus_unum);
        CREDO_VERUM (polynomium_aequalis(extensio_polynomium(k5),
            _p("t^2 - 5")));
        CREDO_AEQUALIS_I32 (extensio_gradus(k5), II);
        CREDO_VERUM (extensio_ordinata(k5));
        CREDO_FALSUM (extensio_ordinata(k_minus_unum));
        /* refutationes: 0, 1, non liberi quadratis, nimis magni */
        CREDO_NIHIL (extensio_quadratica(ZEPHYRUM, piscina));
        CREDO_NIHIL (extensio_quadratica(I, piscina));
        CREDO_NIHIL (extensio_quadratica(IV, piscina));
        CREDO_NIHIL (extensio_quadratica(XII, piscina));
        CREDO_NIHIL (extensio_quadratica(-VIII, piscina));
        CREDO_NIHIL (extensio_quadratica((s64)0x80000000L, piscina));
        CREDO_NON_NIHIL (extensio_quadratica((s64)0x7FFFFFFFL,
            piscina));
        /* sqrt 5 ^ 2 = 5, i^2 = -1 */
        r = algebraicus_generator(k5, piscina);
        x = algebraicus_multiplica(r, r, piscina);
        CREDO_VERUM (algebraicus_est_rationalis(x, piscina, &q));
        CREDO_VERUM (fractio_aequalis(q, fractio_ex_s64(V)));
        x = algebraicus_generator(k_minus_unum, piscina);
        x = algebraicus_multiplica(x, x, piscina);
        CREDO_VERUM (algebraicus_est_rationalis(x, piscina, &q));
        CREDO_VERUM (fractio_aequalis(q, fractio_ex_s64(-I)));
        /* aureus: phi^2 = phi + 1, N = -1, Tr = 1, 1/phi = phi - 1 */
        aureus = _a(k5, "(a + 1)/2");
        CREDO_VERUM (_textus_est(aureus, "(a + 1)/2"));
        CREDO_VERUM (algebraicus_aequalis(algebraicus_multiplica(aureus,
            aureus, piscina), algebraicus_adde(aureus, _a(k5, "1"),
            piscina)));
        CREDO_VERUM (algebraicus_norma(aureus, piscina, &q)
            && fractio_aequalis(q, fractio_ex_s64(-I)));
        CREDO_VERUM (algebraicus_vestigium(aureus, piscina, &q)
            && fractio_aequalis(q, fractio_ex_s64(I)));
        CREDO_VERUM (algebraicus_inversum(aureus, piscina, &x)
            && algebraicus_aequalis(x, algebraicus_subtrahe(aureus,
            _a(k5,
            "1"), piscina)));
        /* N(1 + sqrt 2) = -1 (unitas), Tr(sqrt 2) = 0 */
        {
            Extensio* k2 = extensio_quadratica(II, piscina);

            CREDO_VERUM (algebraicus_norma(_a(k2, "a + 1"), piscina, &q)
                && fractio_aequalis(q, fractio_ex_s64(-I)));
            CREDO_VERUM (algebraicus_vestigium(_a(k2, "a"), piscina, &q)
                && fractio_est_integra(q) && fractio_signum(q)
                == ZEPHYRUM);
            /* Laurent, f(0) > 0 et < 0: 1/i = -i, 1/sqrt(-3) =
             * -sqrt(-3)/3 */
            CREDO_VERUM (_textus_est(_a(k_minus_unum, "a^-1"), "-a"));
            CREDO_VERUM (_textus_est(_a(extensio_quadratica(-III,
                piscina),
                "a^-1"), "-a/3"));
            /* Laurent: sqrt 2 + 1/sqrt 2 = 3 sqrt 2 / 2 */
            CREDO_VERUM (_textus_est(_a(k2, "a + a^-1"), "3a/2"));
        }
        /* textus */
        CREDO_VERUM (_textus_est(_a(k5, "3/6"), "1/2"));
        CREDO_VERUM (_textus_est(_a(k5, "a^3"), "5a"));
        CREDO_VERUM (_textus_est(_a(k5, "(2a + 4)/-6"), "(-a - 2)/3"));
        CREDO_VERUM (_textus_est(_a(k5, "0/7"), "0"));
        CREDO_FALSUM (algebraicus_ex_chorda(k5, chorda_ex_literis("a/0",
            piscina), piscina, &x));
        CREDO_FALSUM (algebraicus_ex_chorda(k5,
            chorda_ex_literis("b + 1",
            piscina), piscina, &x));
    }


    /* ==================================================
     * CORPORA COSINUS
     * ================================================== */

    {
        imprimere("\n--- Corpora cosinus ---\n");
        CREDO_VERUM (polynomium_aequalis(extensio_polynomium(
            extensio_cosinus(I, piscina)), _p("t - 2")));
        CREDO_VERUM (polynomium_aequalis(extensio_polynomium(
            extensio_cosinus(II, piscina)), _p("t + 2")));
        CREDO_VERUM (polynomium_aequalis(extensio_polynomium(
            extensio_cosinus(III, piscina)), _p("t + 1")));
        CREDO_VERUM (polynomium_aequalis(extensio_polynomium(
            extensio_cosinus(IV, piscina)), _p("t")));
        CREDO_VERUM (polynomium_aequalis(extensio_polynomium(
            extensio_cosinus(VI, piscina)), _p("t - 1")));
        CREDO_VERUM (polynomium_aequalis(extensio_polynomium(
            extensio_cosinus(V, piscina)), _p("t^2 + t - 1")));
        CREDO_VERUM (polynomium_aequalis(extensio_polynomium(
            extensio_cosinus(VII, piscina)), _p("t^3 + t^2 - 2t - 1")));
        CREDO_VERUM (polynomium_aequalis(extensio_polynomium(
            extensio_cosinus(VIII, piscina)), _p("t^2 - 2")));
        CREDO_VERUM (polynomium_aequalis(extensio_polynomium(
            extensio_cosinus(IX, piscina)), _p("t^3 - 3t + 1")));
        CREDO_VERUM (polynomium_aequalis(extensio_polynomium(
            extensio_cosinus(X, piscina)), _p("t^2 - t - 1")));
        CREDO_VERUM (polynomium_aequalis(extensio_polynomium(
            extensio_cosinus(XII, piscina)), _p("t^2 - 3")));
        CREDO_NIHIL (extensio_cosinus(ZEPHYRUM, piscina));
        CREDO_NIHIL (extensio_cosinus(M + I, piscina));
        CREDO_VERUM (extensio_ordinata(extensio_cosinus(VII, piscina)));
        /* alpha = 2 cos(2 pi/7) radix MAXIMA ex III: index 2 */
        CREDO_AEQUALIS_S32 (extensio_radix(extensio_cosinus(VII,
            piscina)),
            II);
        CREDO_AEQUALIS_S32 (extensio_radix(extensio_quadratica(V,
            piscina)), I);
        CREDO_AEQUALIS_S32 (extensio_radix(extensio_quadratica(-V,
            piscina)), -I);
        /* gradus phi(n)/2 et radices 2 cos(2 pi j/n), n <= 120 */
        {
            b32 gradus_bene   = VERUM;
            b32 radices_bene  = VERUM;
            i32 n;

            per (n = III; n <= CXX; n++)
            {
                PiscinaNotatio  nota  = piscina_notare(piscina);
                      Extensio* k     = extensio_cosinus(n,
                          piscina);
                    Polynomium f    = extensio_polynomium(k);
                           i32 phi  = ZEPHYRUM;
                           i32 j;

                per (j = I; j <= n; j++)
                {
                    si (_divisor_communis(j, n) == I)
                    {
                        phi++;
                    }
                }
                si (extensio_gradus(k) != phi / II)
                {
                    gradus_bene = FALSUM;
                }
                per (j = I; II * j < n; j++)
                {
                    f64 x = 2.0 * cos(2.0 * 3.14159265358979323846
                        * (f64)j / (f64)n);
                    f64 valor        = 0.0;
                    f64 magnitudo_f  = 0.0;
                    s32 e;

                    si (_divisor_communis(j, n) != I)
                    {
                        perge;
                    }
                    per (e = polynomium_gradus_summus(f); e >= ZEPHYRUM;
                        e--)
                    {
                        s64 c = ZEPHYRUM;

                        (vacuum)magnus_ad_s64(polynomium_coefficiens(f,
                            e), &c);
                        valor = valor * x + (f64)c;
                        magnitudo_f = magnitudo_f * fabs(x) + fabs(
                            (f64)c);
                    }
                    si (fabs(valor) > 1e-9 * (magnitudo_f + 1.0))
                    {
                        radices_bene = FALSUM;
                    }
                }
                piscina_reficere(piscina, nota);
            }
            CREDO_VERUM (gradus_bene);
            CREDO_VERUM (radices_bene);
        }
        /* N(alpha) et Tr(alpha) in Q(cos 2pi/7): 1 et -1 */
        {
               Extensio* k7     = extensio_cosinus(VII, piscina);
                Fractio  q      = fractio_ex_s64(ZEPHYRUM);
            Algebraicus  alpha  = algebraicus_generator(k7, piscina);

            CREDO_VERUM (algebraicus_norma(alpha, piscina, &q)
                && fractio_aequalis(q, fractio_ex_s64(I)));
            CREDO_VERUM (algebraicus_vestigium(alpha, piscina, &q)
                && fractio_aequalis(q, fractio_ex_s64(-I)));
        }
    }


    /* ==================================================
     * IDENTITATES ET ORACULA F64
     * ================================================== */

    {
        hic_manens constans s64 quadratica[] = { II, III, V, VII, -I,
            -III,
            -V };
        hic_manens constans i32 cosinus[] = { III, IV, VI, V, VII, IX,
            XI, XIII, XVI, XXIV, XXXV };
        Sors s;
         b32 inversa_bene              = VERUM;
         b32 anulus_bene               = VERUM;
         b32 norma_bene                = VERUM;
         b32 norma_oraculum_bene       = VERUM;
         b32 vestigium_oraculum_bene   = VERUM;
         b32 vestigium_bene            = VERUM;
         b32 textus_bene               = VERUM;
         b32 potentia_bene             = VERUM;
         b32 rationales_bene           = VERUM;
         b32 exponentes_negativi_bene  = VERUM;
         i32 numerus_corporum          = VII + XI;
         i32 c;

        imprimere("\n--- Identitates (elementa fortuita) ---\n");
        sors_seminare(&s, CXXIII, I);
        per (c = ZEPHYRUM; c < numerus_corporum; c++)
        {
            Extensio* k = c < VII ? extensio_quadratica(quadratica[c],
                piscina) : extensio_cosinus(cosinus[c - VII], piscina);
            i32 d = extensio_gradus(k);
            i32 iteratio;

            per (iteratio = ZEPHYRUM; iteratio < XL; iteratio++)
            {
                PiscinaNotatio nota  = piscina_notare(piscina);
                   Algebraicus a     = _fortuitum(k, &s);
                   Algebraicus b     = _fortuitum(k, &s);
                   Algebraicus e     = _fortuitum(k, &s);
                   Algebraicus x;
                   Algebraicus y;
                       Fractio na   = fractio_ex_s64(ZEPHYRUM);
                       Fractio nb   = fractio_ex_s64(ZEPHYRUM);
                       Fractio nc   = fractio_ex_s64(ZEPHYRUM);
                       Fractio ta   = fractio_ex_s64(ZEPHYRUM);
                       Fractio tb   = fractio_ex_s64(ZEPHYRUM);
                       Fractio tab  = fractio_ex_s64(ZEPHYRUM);

                /* a * (1/a) = 1 */
                si (!algebraicus_est_nullum(a))
                {
                    si (   !algebraicus_inversum(a, piscina, &x)
                        || !_unum_est(algebraicus_multiplica(a, x,
                        piscina)))
                    {
                        inversa_bene = FALSUM;
                    }
                    /* a^-3 a^3 = 1 */
                    si (   !algebraicus_potentia(a, -III, piscina, &x)
                        || !algebraicus_potentia(a, III, piscina, &y)
                        || !_unum_est(algebraicus_multiplica(x, y,
                        piscina)))
                    {
                        potentia_bene = FALSUM;
                    }
                }
                /* (ab)e = a(be), a(b + e) = ab + ae, (a + b) - b = a */
                si (   !algebraicus_aequalis(algebraicus_multiplica(
                    algebraicus_multiplica(a, b, piscina), e, piscina),
                    algebraicus_multiplica(a, algebraicus_multiplica(b,
                    e,
                    piscina), piscina))
                    || !algebraicus_aequalis(algebraicus_multiplica(a,
                    algebraicus_adde(b, e, piscina), piscina),
                    algebraicus_adde(algebraicus_multiplica(a, b,
                    piscina),
                    algebraicus_multiplica(a, e, piscina), piscina))
                    || !algebraicus_aequalis(algebraicus_subtrahe(
                    algebraicus_adde(a, b, piscina), b, piscina), a))
                {
                    anulus_bene = FALSUM;
                }
                /* N multiplicativa, Tr additiva */
                si (   !algebraicus_norma(a, piscina, &na)
                    || !algebraicus_norma(b, piscina, &nb)
                    || !algebraicus_norma(algebraicus_multiplica(a, b,
                    piscina), piscina, &nc)
                    || !fractio_aequalis(nc,
                    fractio_multiplica(na, nb,
                    piscina)))
                {
                    norma_bene = FALSUM;
                }
                si (   !algebraicus_vestigium(a, piscina, &ta)
                    || !algebraicus_vestigium(b, piscina, &tb)
                    || !algebraicus_vestigium(algebraicus_adde(a, b,
                    piscina), piscina, &tab)
                    || !fractio_aequalis(tab, fractio_adde(ta, tb,
                    piscina)))
                {
                    vestigium_bene = FALSUM;
                }
                /* oraculum externum normae et vestigii */
                si (c < VII)
                {
                    /* a = p + q sqrt d: N = p^2 - d q^2, Tr = 2p */
                    Fractio p = algebraicus_coefficiens(a, ZEPHYRUM,
                        piscina);
                    Fractio qq = algebraicus_coefficiens(a, I, piscina);
                    Fractio n = fractio_subtrahe(fractio_multiplica(p,
                        p,
                        piscina), fractio_multiplica(fractio_ex_s64(
                        quadratica[c]), fractio_multiplica(qq, qq,
                        piscina), piscina), piscina);

                    si (!fractio_aequalis(n, na))
                    {
                        norma_oraculum_bene = FALSUM;
                    }
                    si (!fractio_aequalis(fractio_multiplica(
                        fractio_ex_s64(II), p, piscina), ta))
                    {
                        vestigium_oraculum_bene = FALSUM;
                    }
                }
                alioquin
                {
                    /* productum et summa super 2 cos(2 pi j/n) */
                    i32 n          = cosinus[c - VII];
                    f64 productum  = 1.0;
                    f64 summa      = 0.0;
                    i32 j;

                    per (j = I; II * j < n; j++)
                    {
                        f64 x64;

                        si (_divisor_communis(j, n) != I)
                        {
                            perge;
                        }
                        x64 = _valor(a, 2.0 * cos(2.0
                            * 3.14159265358979323846 * (f64)j / (f64)n),
                            d);
                        productum  = productum * x64;
                        summa      = summa + x64;
                    }
                    si (fabs(productum - _fractio_ad_f64(na)) > 1e-7
                        * (fabs(productum) + 1.0))
                    {
                        norma_oraculum_bene = FALSUM;
                    }
                    si (fabs(summa - _fractio_ad_f64(ta))
                        > 1e-9 * (fabs(
                        summa) + 1.0))
                    {
                        vestigium_oraculum_bene = FALSUM;
                    }
                }
                /* textus reversibilis */
                si (   !algebraicus_ex_chorda(k,
                    algebraicus_ad_chordam(a,
                    piscina), piscina, &x)
                    || !algebraicus_aequalis(x, a))
                {
                    textus_bene = FALSUM;
                }
                piscina_reficere(piscina, nota);
            }
            /* alpha^-1 (Laurent) * alpha = 1, et a^-2 = 1/(a^2) */
            {
                Algebraicus inversa;
                Algebraicus alpha = algebraicus_generator(k, piscina);
                Algebraicus quadratum_inversum;
                Algebraicus x;

                inversa.corpus             = NIHIL;
                quadratum_inversum.corpus  = NIHIL;
                /* alpha = 0 (cos 2pi/4): Laurent refutatur, alibi
                 * probatum */
                si (   !algebraicus_est_nullum(alpha)
                    && (   !algebraicus_ex_chorda(k,
                    chorda_ex_literis("a^-1",
                    piscina), piscina, &inversa)
                    || !_unum_est(algebraicus_multiplica(inversa, alpha,
                    piscina))
                    || !algebraicus_ex_chorda(k, chorda_ex_literis(
                    "a^-2 + 1", piscina), piscina, &quadratum_inversum)
                    || !algebraicus_inversum(algebraicus_multiplica(
                    alpha, alpha, piscina), piscina, &x)
                    || !algebraicus_aequalis(quadratum_inversum,
                    algebraicus_adde(x, _a(k, "1"), piscina))))
                {
                    exponentes_negativi_bene = FALSUM;
                }
            }
            /* rationales: N(q) = q^d, Tr(q) = d q */
            {
                    Fractio q           = fractio_ex_s64(ZEPHYRUM);
                    Fractio n           = fractio_ex_s64(ZEPHYRUM);
                    Fractio t           = fractio_ex_s64(ZEPHYRUM);
                    Fractio q_potentia  = fractio_ex_s64(ZEPHYRUM);
                Algebraicus elementum_q;

                (vacuum)fractio_ex_s64_s64(-III, VII, piscina, &q);
                (vacuum)fractio_potentia(q, (s32)d, piscina,
                    &q_potentia);
                elementum_q = algebraicus_ex_fractione(k, q, piscina);

                si (   !algebraicus_norma(elementum_q, piscina, &n)
                    || !fractio_aequalis(n, q_potentia)
                    || !algebraicus_vestigium(elementum_q, piscina, &t)
                    || !fractio_aequalis(t, fractio_multiplica(
                    fractio_ex_s64((s64)d), q, piscina)))
                {
                    rationales_bene = FALSUM;
                }
            }
        }
        CREDO_VERUM (inversa_bene);
        CREDO_VERUM (potentia_bene);
        CREDO_VERUM (anulus_bene);
        CREDO_VERUM (norma_bene);
        CREDO_VERUM (norma_oraculum_bene);
        CREDO_VERUM (vestigium_oraculum_bene);
        CREDO_VERUM (vestigium_bene);
        CREDO_VERUM (textus_bene);
        CREDO_VERUM (rationales_bene);
        CREDO_VERUM (exponentes_negativi_bene);
    }


    /* ==================================================
     * REFUTATIONES ET CORPORA MIXTA
     * ================================================== */

    {
           Extensio* k2 = extensio_quadratica(II, piscina);
           Extensio* k3 = extensio_quadratica(III, piscina);
        Algebraicus  a2 = algebraicus_generator(k2, piscina);
        Algebraicus  a3 = algebraicus_generator(k3, piscina);
        Algebraicus  x;
            Fractio  q = fractio_ex_s64(ZEPHYRUM);

        imprimere("\n--- Refutationes ---\n");
        CREDO_FALSUM (algebraicus_aequalis(a2, a3));
        CREDO_FALSUM (algebraicus_est_validum(algebraicus_adde(a2, a3,
            piscina)));
        CREDO_FALSUM (algebraicus_est_validum(algebraicus_multiplica(a2,
            a3, piscina)));
        CREDO_FALSUM (algebraicus_divide(a2, a3, piscina, &x));
        CREDO_FALSUM (algebraicus_inversum(_a(k2, "0"), piscina, &x));
        CREDO_FALSUM (algebraicus_potentia(_a(k2, "0"), -I, piscina,
            &x));
        CREDO_FALSUM (algebraicus_norma(algebraicus_adde(a2, a3,
            piscina),
            piscina, &q));
        CREDO_VERUM (_textus_est(algebraicus_adde(a2, a3, piscina),
            "invalidum"));
        /* alpha = 0 (cos 2pi/4): Laurent refutatur */
        CREDO_FALSUM (algebraicus_ex_chorda(extensio_cosinus(IV,
            piscina),
            chorda_ex_literis("a^-1", piscina), piscina, &x));
    }


    /* ==================================================
     * MATRIX SUPER Q(sqrt 5)
     * ================================================== */

    {
               Extensio* k5      = extensio_quadratica(V,
                   piscina);
        constans Anulus* corpus  = extensio_anulus(k5);
            Algebraicus  nodi[IV];
            Algebraicus  det;
            Algebraicus  productum;
                 Matrix  m;
                    i32  i;
                    i32  j;
                    i32  gradus = ZEPHYRUM;

        imprimere("\n--- Matrix super Q(sqrt 5) ---\n");
        CREDO_VERUM (corpus->corpus);
        /* Vandermonde nodis 1, phi, phi^2, 1/phi: det = prod_{i<j}
         * (x_j - x_i) */
        nodi[ZEPHYRUM] = _a(k5, "1");
        nodi[I] = _a(k5, "(a + 1)/2");
        nodi[II] = algebraicus_multiplica(nodi[I], nodi[I], piscina);
        (vacuum)algebraicus_inversum(nodi[I], piscina, &nodi[III]);
        CREDO_VERUM (matrix_nulla(corpus, IV, IV, piscina, &m));
        per (i = ZEPHYRUM; i < IV; i++)
        {
            per (j = ZEPHYRUM; j < IV; j++)
            {
                Algebraicus v;

                (vacuum)algebraicus_potentia(nodi[i], (s32)j, piscina,
                    &v);
                matrix_pone(&m, i, j, &v);
            }
        }
        productum = _a(k5, "1");
        per (i = ZEPHYRUM; i < IV; i++)
        {
            per (j = i + I; j < IV; j++)
            {
                productum = algebraicus_multiplica(productum,
                    algebraicus_subtrahe(nodi[j], nodi[i], piscina),
                    piscina);
            }
        }
        CREDO_VERUM (matrix_determinans(m, piscina, &det));
        CREDO_VERUM (algebraicus_aequalis(det, productum));
        CREDO_FALSUM (algebraicus_est_nullum(det));
        /* singularis: linea 3 = linea 1 + phi linea 2 */
        per (j = ZEPHYRUM; j < IV; j++)
        {
            Algebraicus v = algebraicus_adde(*(constans Algebraicus*)
                matrix_elementum(m, I, j),
                algebraicus_multiplica(nodi[I],
                *(constans Algebraicus*)matrix_elementum(m, II, j),
                piscina), piscina);

            matrix_pone(&m, III, j, &v);
        }
        CREDO_VERUM (matrix_gradus(m, piscina, &gradus));
        CREDO_AEQUALIS_I32 (gradus, III);
        /* elementum alterius corporis refutatur */
        {
            Algebraicus alienum =
                algebraicus_generator(extensio_quadratica(
                II, piscina), piscina);

            matrix_pone(&m, ZEPHYRUM, ZEPHYRUM, &alienum);
            CREDO_FALSUM (matrix_determinans(m, piscina, &det));
        }
    }


    /* ==================================================
     * RECENSIO E1: LACUNAE CLAUSAE
     * ================================================== */

    {
           Extensio* k5   = extensio_quadratica(V, piscina);
           Extensio* k_i  = extensio_quadratica(-I, piscina);
        Algebraicus  a    = algebraicus_generator(k5, piscina);
        Algebraicus  x;
            Fractio  q = fractio_ex_s64(ZEPHYRUM);

        imprimere("\n--- Recensio E1 ---\n");
        /* F1: -2^63 refutatur sine negatione indefinita; fines -2^31 */
        CREDO_NIHIL (extensio_quadratica(-(s64)0x7FFFFFFFFFFFFFFFL - I,
            piscina));
        CREDO_NIHIL (extensio_quadratica(-(s64)0x80000000L, piscina));
        CREDO_NON_NIHIL (extensio_quadratica(-(s64)0x7FFFFFFFL,
            piscina));
        /* M17/M16: aequalitas intra corpus unum FALSUM reddere
         * potest */
        CREDO_FALSUM (algebraicus_aequalis(a, _a(k5, "a + 1")));
        CREDO_FALSUM (algebraicus_aequalis(a, _a(k5, "a/2")));
        CREDO_FALSUM (algebraicus_aequalis(a, _a(k5, "-a")));
        CREDO_VERUM (algebraicus_aequalis(a, _a(k5, "2a/2")));
        /* M18: sqrt 5 non rationalis */
        CREDO_FALSUM (algebraicus_est_rationalis(a, piscina, &q));
        /* M51: nega canonicum */
        x = algebraicus_nega(_a(k5, "(a + 1)/2"), piscina);
        CREDO_VERUM (_textus_est(x, "(-a - 1)/2"));
        CREDO_VERUM (algebraicus_aequalis(algebraicus_nega(x, piscina),
            _a(k5, "(a + 1)/2")));
        /* M49: generator corporis gradus 1 reductus: 2cos(2pi/3) =
         * -1 */
        CREDO_VERUM (algebraicus_est_rationalis(algebraicus_generator(
            extensio_cosinus(III, piscina), piscina), piscina, &q)
            && fractio_aequalis(q, fractio_ex_s64(-I)));
        CREDO_VERUM (algebraicus_est_rationalis(algebraicus_generator(
            extensio_cosinus(I, piscina), piscina), piscina, &q)
            && fractio_aequalis(q, fractio_ex_s64(II)));
        /* M46: denominator nullus per ex_polynomio refutatur */
        CREDO_FALSUM (algebraicus_ex_polynomio(k5, _p("t + 1"),
            magnus_ex_s64(ZEPHYRUM), piscina, &x));
        /* F4: "P/D" sine parenthesibus solum termino uno */
        CREDO_FALSUM (algebraicus_ex_chorda(k5, chorda_ex_literis(
            "a + 1/2", piscina), piscina, &x));
        CREDO_VERUM (_textus_est(_a(k5, "3a/2"), "3a/2"));
        CREDO_VERUM (_textus_est(_a(k5, "(a + 1)/2"), "(a + 1)/2"));
        /* F2: potentiae magnae per potentias, non Hornerum densum */
        CREDO_VERUM (_textus_est(_a(k_i, "a^1073741823"), "-a"));
        {
               Extensio* k2     = extensio_quadratica(II, piscina);
            Algebraicus  alpha  = algebraicus_generator(k2, piscina);

            CREDO_VERUM (algebraicus_potentia(alpha, (s32)MMM * XXX,
                piscina, &x));
            CREDO_VERUM (algebraicus_aequalis(x, _a(k2, "a^90000")));
            CREDO_VERUM (algebraicus_aequalis(_a(k2, "a^90001 + a^3"),
                algebraicus_adde(algebraicus_multiplica(x, alpha,
                piscina), _a(k2, "2a"), piscina)));
        }
        /* F3: inversa gradu XXI (Q(cos 2pi/49)) per nucleum */
        {
               Extensio* k49 = extensio_cosinus(XLIX, piscina);
            Algebraicus  b = _a(k49,
                "(5a^20 - 15a^7 + a - 10)/5");

            CREDO_AEQUALIS_I32 (extensio_gradus(k49), XXI);
            CREDO_VERUM (algebraicus_inversum(b, piscina, &x)
                && _unum_est(algebraicus_multiplica(b, x, piscina)));
        }
        /* M53/F5: n = M admittitur (gradus 200), Chebyshev O(m^2) */
        {
            Extensio* k_maximum = extensio_cosinus(M, piscina);

            CREDO_NON_NIHIL (k_maximum);
            CREDO_AEQUALIS_I32 (extensio_gradus(k_maximum), CC);
        }
        /* radix maxima per Descartes certificata, non per catenam Sturm
         * (gradu 498 olim 2.8 GB) neque per isolationem totam: n = 997
         * ~0.1 s; via tarda > LX s - mora MM ms eam capit, et
         * memoriam fugitivam processu filio terminat */
        CREDO_NON_PENDET ((vacuum)extensio_cosinus(CMXCVII, piscina),
            MM);
        /* F2: potentia magna per potentias (~0.1 s), non per Hornerum
         * densum (passus unus per exponentem: "a^200000" ~5 s) */
        CREDO_NON_PENDET ((vacuum)algebraicus_ex_chorda(k5,
            chorda_ex_literis("a^200000 + 1", piscina), piscina, &x),
            MM);
        /* M13/M62: matrix cum elemento alieno: adde, multiplica,
         * aequalis refutant */
        {
            constans Anulus* corpus = extensio_anulus(k5);
                Algebraicus  alienum = algebraicus_generator(
                    extensio_quadratica(III, piscina), piscina);
            Matrix m1;
            Matrix m2;
            Matrix summa;
               b32 aequales;

            (vacuum)matrix_identitas(corpus, II, piscina, &m1);
            (vacuum)matrix_identitas(corpus, II, piscina, &m2);
            matrix_pone(&m1, ZEPHYRUM, I, &alienum);
            matrix_pone(&m2, ZEPHYRUM, I, &alienum);
            CREDO_FALSUM (matrix_adde(m1, m2, piscina, &summa));
            CREDO_FALSUM (matrix_multiplica(m1, m2, piscina, &summa));
            aequales = matrix_aequalis(m1, m2);
            CREDO_FALSUM (aequales);
        }
    }


    /* ==================================================
     * ORDO: STURM, RADICES, SIGNUM
     * ================================================== */

    {
                i32 n       = ZEPHYRUM;
                s32 signum  = ZEPHYRUM;
        Algebraicus x;

        imprimere("\n--- Radices reales (Sturm) ---\n");
        CREDO_VERUM (extensio_radices_reales(_p("t^2 - 2"), piscina, &n)
            && n == II);
        CREDO_VERUM (extensio_radices_reales(_p("t^2 + 1"), piscina, &n)
            && n == ZEPHYRUM);
        CREDO_VERUM (extensio_radices_reales(_p("t^3 - 2"), piscina, &n)
            && n == I);
        CREDO_VERUM (extensio_radices_reales(_p("t^3 - 6t^2 + 11t - 6"),
            piscina, &n) && n == III);
        CREDO_VERUM (extensio_radices_reales(_p("t^5 - t - 1"), piscina,
            &n) && n == I);
        /* non liber quadratis: radices DISTINCTAE */
        CREDO_VERUM (extensio_radices_reales(_p("t^4 - 4t^2 + 4"),
            piscina, &n) && n == II);
        CREDO_FALSUM (extensio_radices_reales(_p("0"), piscina, &n));
        CREDO_FALSUM (extensio_radices_reales(_p("5"), piscina, &n));
        CREDO_FALSUM (extensio_radices_reales(_p("t^-1 + 1"), piscina,
            &n));
        /* Psi_n: omnes phi(n)/2 radices reales */
        {
            b32 psi_bene = VERUM;
            i32 m;

            per (m = III; m <= LX; m++)
            {
                PiscinaNotatio  nota  = piscina_notare(piscina);
                      Extensio* k     = extensio_cosinus(m,
                          piscina);

                si (   !extensio_radices_reales(extensio_polynomium(k),
                    piscina, &n)
                    || n != (i32)extensio_gradus(k))
                {
                    psi_bene = FALSUM;
                }
                piscina_reficere(piscina, nota);
            }
            CREDO_VERUM (psi_bene);
        }

        imprimere("\n--- extensio_ex_polynomio ---\n");
        CREDO_NIHIL (extensio_ex_polynomio(_p("2t^2 - 1"), ZEPHYRUM,
            piscina));
        CREDO_NIHIL (extensio_ex_polynomio(_p("t^2 - 4"), -I, piscina));
        CREDO_NIHIL (extensio_ex_polynomio(_p("t^2"), -I, piscina));
        CREDO_NIHIL (extensio_ex_polynomio(_p("t^3 - t"), -I, piscina));
        CREDO_NIHIL (extensio_ex_polynomio(_p("t^4 - 4t^2 + 4"), -I,
            piscina));
        CREDO_NIHIL (extensio_ex_polynomio(_p("t^3 - 7t + 6"), -I,
            piscina));
        /* (t - 3)(t^2 - 2): radix integra 3 numquam medium
         * bisectionis */
        CREDO_NIHIL (extensio_ex_polynomio(_p("t^3 - 3t^2 - 2t + 6"),
            -I,
            piscina));
        CREDO_NIHIL (extensio_ex_polynomio(_p("t^2 - 2"), II, piscina));
        CREDO_NIHIL (extensio_ex_polynomio(_p("t^2 - 2"), -II,
            piscina));
        CREDO_NIHIL (extensio_ex_polynomio(_p("t^2 + 1"), ZEPHYRUM,
            piscina));
        CREDO_NIHIL (extensio_ex_polynomio(_p("t^-2 + 1"), -I,
            piscina));
        CREDO_NON_NIHIL (extensio_ex_polynomio(_p("t + 7"), ZEPHYRUM,
            piscina));
        /* radix cubica 2: alpha^3 = 2, 1.2599 < alpha < 1.26 */
        {
            Extensio* k = extensio_ex_polynomio(_p("t^3 - 2"), ZEPHYRUM,
                piscina);
            Algebraicus alpha = algebraicus_generator(k, piscina);
                Fractio cubus = fractio_ex_s64(ZEPHYRUM);

            CREDO_NON_NIHIL (k);
            CREDO_AEQUALIS_S32 (extensio_radix(k), ZEPHYRUM);
            CREDO_VERUM (algebraicus_potentia(alpha, III, piscina, &x));
            CREDO_VERUM (algebraicus_est_rationalis(x, piscina, &cubus)
                && fractio_aequalis(cubus, fractio_ex_s64(II)));
            CREDO_VERUM (algebraicus_compara(alpha, _a(k,
                "12599/10000"),
                piscina, &signum) && signum == I);
            CREDO_VERUM (algebraicus_compara(alpha, _a(k, "63/50"),
                piscina, &signum) && signum == -I);
            CREDO_VERUM (algebraicus_compara(alpha, alpha, piscina,
                &signum) && signum == ZEPHYRUM);
        }
        /* t^2 - 2: radix 0 = -sqrt 2, radix 1 = +sqrt 2 */
        CREDO_VERUM (algebraicus_signum(algebraicus_generator(
            extensio_ex_polynomio(_p("t^2 - 2"), ZEPHYRUM, piscina),
            piscina), piscina, &signum) && signum == -I);
        CREDO_VERUM (algebraicus_signum(algebraicus_generator(
            extensio_ex_polynomio(_p("t^2 - 2"), I, piscina), piscina),
            piscina, &signum) && signum == I);
        /* sine ordine: refutatur */
        CREDO_FALSUM (algebraicus_signum(algebraicus_generator(
            extensio_quadratica(-I, piscina), piscina), piscina,
            &signum));
        /* reducibilis gradu 4 (vocans asseruit): acceptum; divisor
         * nullius refutatur, signum testimonio nullius refutat */
        {
            Extensio* r = extensio_ex_polynomio(_p("t^4 + 3t^2 + 2"),
                -I,
                piscina);
            Extensio* q = extensio_ex_polynomio(_p("t^4 - 5t^2 + 6"),
                III,
                piscina);

            CREDO_NON_NIHIL (r);
            CREDO_FALSUM (algebraicus_inversum(_a(r, "a^2 + 1"),
                piscina,
                &x));
            CREDO_NON_NIHIL (q);
            CREDO_FALSUM (algebraicus_signum(_a(q, "a^2 - 3"), piscina,
                &signum));
            /* sqrt 3 > 1.732 */
            CREDO_VERUM (algebraicus_compara(_a(q, "a"), _a(q,
                "1732/1000"),
                piscina, &signum) && signum == I);
        }
    }

    {
        /* Pell: (1 + sqrt 2)^k = p + q sqrt 2, p - q sqrt 2 = (1 -
         * sqrt 2)^k: signum (-1)^k, |.| ~ 0.414^k (k = 100: 1e-38) */
           Extensio* k2          = extensio_quadratica(II, piscina);
           Extensio* k5          = extensio_quadratica(V, piscina);
        Algebraicus  u2          = _a(k2, "1 + a");
        Algebraicus  aureus      = _a(k5, "(a + 1)/2");
                b32  pell_bene   = VERUM;
                b32  aurei_bene  = VERUM;
                s32  signum      = ZEPHYRUM;
                s32  e;

        imprimere("\n--- Signum prope nullum ---\n");
        per (e = I; e <= C; e++)
        {
            PiscinaNotatio nota = piscina_notare(piscina);
               Algebraicus potentia;
               Algebraicus conjugatum;
                       s32 expectatum = (e & I) ? -I : I;
                   Fractio p;
                   Fractio q;

            /* conjugatum: p - q alpha */
            (vacuum)algebraicus_potentia(u2, e, piscina, &potentia);
            p = algebraicus_coefficiens(potentia, ZEPHYRUM, piscina);
            q = algebraicus_coefficiens(potentia, I, piscina);
            conjugatum =
                algebraicus_subtrahe(algebraicus_ex_fractione(k2,
                p, piscina),
                algebraicus_multiplica(algebraicus_ex_fractione(
                k2, q, piscina), algebraicus_generator(k2, piscina),
                piscina), piscina);
            si (   !algebraicus_signum(conjugatum, piscina, &signum)
                || signum != expectatum)
            {
                pell_bene = FALSUM;
            }
            /* phi^k conjugatum = (-1/phi)^k */
            (vacuum)algebraicus_potentia(aureus, e, piscina, &potentia);
            p = algebraicus_coefficiens(potentia, ZEPHYRUM, piscina);
            q = algebraicus_coefficiens(potentia, I, piscina);
            conjugatum =
                algebraicus_subtrahe(algebraicus_ex_fractione(k5,
                p, piscina),
                algebraicus_multiplica(algebraicus_ex_fractione(
                k5, q, piscina), algebraicus_generator(k5, piscina),
                piscina), piscina);
            si (   !algebraicus_signum(conjugatum, piscina, &signum)
                || signum != expectatum)
            {
                aurei_bene = FALSUM;
            }
            piscina_reficere(piscina, nota);
        }
        CREDO_VERUM (pell_bene);
        CREDO_VERUM (aurei_bene);
    }

    {
        /* signum fortuitum contra f64 ad radicem notam */
        hic_manens constans character* polynomia[] = { "t^2 - 2",
            "t^2 - 3", "t^2 + t - 1", "t^3 - 2", "t^5 - t - 1",
            "t^4 - 10t^2 + 1" };
        hic_manens constans s32 indices[] = { I, I, I, ZEPHYRUM,
            ZEPHYRUM,
            III };
        hic_manens constans f64 radices[] = { 1.4142135623730951,
            1.7320508075688772, 0.6180339887498949, 1.2599210498948732,
            1.1673039782614187, 3.1462643699419726 };
        Sors s;
         b32 signum_bene           = VERUM;
         b32 compara_inversa_bene  = VERUM;
         i32 probata               = ZEPHYRUM;
         i32 c;

        imprimere("\n--- Signum fortuitum (oraculum f64) ---\n");
        sors_seminare(&s, CCCXXI, II);
        per (c = ZEPHYRUM; c < VI; c++)
        {
            Extensio* k = extensio_ex_polynomio(_p(polynomia[c]),
                indices[c], piscina);
            i32 iteratio;

            si (k == NIHIL)
            {
                signum_bene = FALSUM;
                perge;
            }
            per (iteratio = ZEPHYRUM; iteratio < LX; iteratio++)
            {
                PiscinaNotatio nota  = piscina_notare(piscina);
                   Algebraicus a     = _fortuitum(k, &s);
                   Algebraicus b     = _fortuitum(k, &s);
                           f64 valor;
                           s32 sa = ZEPHYRUM;
                           s32 ab = ZEPHYRUM;
                           s32 ba = ZEPHYRUM;

                valor = _valor(a, radices[c], extensio_gradus(k));
                si (fabs(valor) > 1e-6)
                {
                    probata++;
                    si (   !algebraicus_signum(a, piscina, &sa)
                        || sa != (valor > 0.0 ? I : -I))
                    {
                        signum_bene = FALSUM;
                    }
                }
                si (   !algebraicus_compara(a, b, piscina, &ab)
                    || !algebraicus_compara(b, a, piscina, &ba)
                    || ab != -ba)
                {
                    compara_inversa_bene = FALSUM;
                }
                piscina_reficere(piscina, nota);
            }
        }
        CREDO_VERUM (signum_bene);
        CREDO_VERUM (compara_inversa_bene);
        CREDO_MAIOR_I32 (probata, CCC);
    }


    /* ==================================================
     * RECENSIO II (E2b): FORMA CENTRATA, PROFUNDITAS, SUMPTUS
     * ================================================== */

    {
        /* T1/N12: signa prope nullum in gradu 3 (via generalis, non
         * forma clausa gradus 2), radice NEGATIVA et coefficientibus
         * signorum mixtorum: (alpha - c)^e, c rationalis > 1e-3 ab
         * alpha, signum exspectatum sign(alpha - c)^e */
        hic_manens constans character* polynomia[] = { "t^3 - 3t + 1",
            "t^3 - 3t + 1", "t^3 - 3t + 1", "t^3 + t^2 - 2t - 1" };
        hic_manens constans s32 indices[] = { ZEPHYRUM, I, II,
            ZEPHYRUM };
        hic_manens constans f64 radices[] = { -1.8793852415718167,
            0.34729635533386069, 1.5320888862379560,
                -1.8019377358048383 };
        hic_manens constans character* constantes[] = { "-1879/1000",
            "-188/100", "347/1000", "348/1000", "1532/1000",
                "1533/1000",
            "-1802/1000", "-1801/1000" };
        b32 signa_bene   = VERUM;
        b32 sordes_bene  = VERUM;
        i32 probata      = ZEPHYRUM;
        i32 c;

        imprimere("\n--- Recensio II: gradus 3, prope nullum ---\n");
        per (c = ZEPHYRUM; c < IV; c++)
        {
            Extensio* k = extensio_ex_polynomio(_p(polynomia[c]),
                indices[c], piscina);
            i32 j;

            si (k == NIHIL)
            {
                signa_bene = FALSUM;
                perge;
            }
            per (j = ZEPHYRUM; j < VIII; j++)
            {
                Algebraicus differentia;
                        f64 constans_f64;
                        s32 basis;
                        s32 e;

                {
                    Fractio q = fractio_ex_s64(ZEPHYRUM);

                    (vacuum)fractio_ex_chorda(chorda_ex_literis(
                        constantes[j], piscina), piscina, &q);
                    differentia = algebraicus_subtrahe(
                        algebraicus_generator(k, piscina),
                        algebraicus_ex_fractione(k, q, piscina),
                        piscina);
                    constans_f64 = _fractio_ad_f64(q);
                }
                si (fabs(radices[c] - constans_f64) < 1e-4)
                {
                    perge;
                }
                basis = radices[c] > constans_f64 ? I : -I;
                per (e = I; e <= LX; e = e + VII)
                {
                    PiscinaNotatio nota = piscina_notare(piscina);
                       Algebraicus potentia;
                               s32 signum = ZEPHYRUM;
                               s32 expectatum;
                    memoriae_index ante;

                    expectatum = (basis < ZEPHYRUM && (e & I)) ? -I : I;

                    (vacuum)algebraicus_potentia(differentia, e,
                        piscina,
                        &potentia);
                    ante = piscina_summa_usus(piscina);
                    si (   !algebraicus_signum(potentia, piscina,
                        &signum)
                        || signum != expectatum)
                    {
                        signa_bene = FALSUM;
                    }
                    /* L1: signum nihil in piscina vocantis relinquit */
                    si (piscina_summa_usus(piscina) != ante)
                    {
                        sordes_bene = FALSUM;
                    }
                    probata++;
                    piscina_reficere(piscina, nota);
                }
            }
        }
        CREDO_VERUM (signa_bene);
        CREDO_VERUM (sordes_bene);
        CREDO_MAIOR_I32 (probata, C);
    }

    {
        /* N03/M1: Mignotte t^5 - 2(a t - 1)^2 (Eisenstein ad 2), a =
         * 10^6: radices r0 < 1/a < r1 ~1e-21 distantes; profunditas ex
         * separatione, non MM */
        Polynomium  f          = polynomium_nullum();
        Polynomium  linea      = polynomium_nullum();
        Polynomium  quadratum  = polynomium_nullum();
          Extensio* k0;
          Extensio* k1;
           Fractio  inversum  = fractio_ex_s64(ZEPHYRUM);
               s32  signum    = ZEPHYRUM;
               i32  n         = ZEPHYRUM;

        imprimere("\n--- Recensio II: Mignotte ---\n");
        linea = _p("1000000t - 1");
        (vacuum)polynomium_multiplica(linea, linea, piscina,
            &quadratum);
        f = polynomium_subtrahe(_p("t^5"),
            polynomium_multiplica_scalari(
            quadratum, magnus_ex_s64(II), piscina), piscina);
        CREDO_VERUM (extensio_radices_reales(f, piscina, &n)
            && n == III);
        CREDO_NON_NIHIL (extensio_ex_polynomio(f, -I, piscina));
        k0 = extensio_ex_polynomio(f, ZEPHYRUM, piscina);
        k1 = extensio_ex_polynomio(f, I, piscina);
        CREDO_NON_NIHIL (k0);
        CREDO_NON_NIHIL (k1);
        (vacuum)fractio_ex_s64_s64(I, 1000000L, piscina, &inversum);
        CREDO_VERUM (algebraicus_compara(algebraicus_generator(k0,
            piscina), algebraicus_ex_fractione(k0, inversum, piscina),
            piscina, &signum) && signum == -I);
        CREDO_VERUM (algebraicus_compara(algebraicus_generator(k1,
            piscina), algebraicus_ex_fractione(k1, inversum, piscina),
            piscina, &signum) && signum == I);
    }

    {
        /* N20: altus cum denominatore; N30: radix integra (t - r)(t^2
         * - 2), r = 2..40, semper refutatur */
           Extensio* k5 = extensio_quadratica(V, piscina);
        Algebraicus  x;
            Fractio  q              = fractio_ex_s64(ZEPHYRUM);
                b32  integrae_bene  = VERUM;
                s64  r;

        imprimere("\n--- Recensio II: denominator, integrae ---\n");
        CREDO_VERUM (algebraicus_ex_chorda(k5, chorda_ex_literis(
            "(a^20 + 1)/2", piscina), piscina, &x)
            && algebraicus_est_rationalis(x, piscina, &q)
            && fractio_aequalis(q, fractio_ex_s64(4882813L)));
        per (r = II; r <= XL; r++)
        {
            PiscinaNotatio nota       = piscina_notare(piscina);
                Polynomium productum  = polynomium_nullum();
                    Magnus c[II];
                Polynomium factor = polynomium_nullum();

            c[ZEPHYRUM]  = magnus_ex_s64(-r);
            c[I]         = magnus_ex_s64(I);
            (vacuum)polynomium_ex_coefficientibus(c, II, ZEPHYRUM,
                piscina, &factor);
            (vacuum)polynomium_multiplica(factor, _p("t^2 - 2"),
                piscina,
                &productum);
            si (extensio_ex_polynomio(productum, -I, piscina) != NIHIL)
            {
                integrae_bene = FALSUM;
            }
            piscina_reficere(piscina, nota);
        }
        CREDO_VERUM (integrae_bene);
    }

    {
        /* M2, M3, H1: sumptus sub mora (processus filius) */
           Extensio* k2 = extensio_quadratica(II, piscina);
           Extensio* k5 = extensio_quadratica(V, piscina);
        Algebraicus  x;
        Algebraicus  potentia;
                s32  signum  = ZEPHYRUM;
         Polynomium  f       = _p("1");
          character* textus;
          character* cursor;
                i32  j;

        imprimere("\n--- Recensio II: sumptus ---\n");
        /* H1: (1 - sqrt 2)^1000 > 0, forma clausa (olim FALSUM post
         * MM bisectiones) */
        (vacuum)algebraicus_potentia(_a(k2, "1 - a"), M, piscina,
            &potentia);
        CREDO_VERUM (algebraicus_signum(potentia, piscina, &signum)
            && signum == I);
        /* M2: prod (t - 2i) + 1, gradus 40, omnes radices reales */
        per (j = I; j <= XL; j++)
        {
                Magnus c[II];
            Polynomium factor = polynomium_nullum();

            c[ZEPHYRUM]  = magnus_ex_s64(-(s64)(II * j));
            c[I]         = magnus_ex_s64(I);
            (vacuum)polynomium_ex_coefficientibus(c, II, ZEPHYRUM,
                piscina, &factor);
            (vacuum)polynomium_multiplica(f, factor, piscina, &f);
        }
        f = polynomium_adde(f, _p("1"), piscina);
        CREDO_NON_PENDET ((vacuum)extensio_ex_polynomio(f, XX, piscina),
            MM);
        /* M3: densum gradus 20000 (Horner sparsus) */
        textus = (character*)piscina_allocare(piscina, (memoriae_index)
            CCC * M);
        cursor = textus;
        per (j = XX * M; j >= I; j--)
        {
            cursor = cursor + sprintf(cursor, "a^%u + ", j);
        }
        sprintf(cursor, "1");
        CREDO_NON_PENDET ((vacuum)algebraicus_ex_chorda(k5,
            chorda_ex_literis(textus, piscina), piscina, &x), MM);
    }

    {
        /* E17/E28 (N12): limes derivatae D in forma centrata. c =
         * approximatio XVII digitorum radicis: |alpha - c| ~ 1e-16,
         * signum ignotum sed per elementum LINEARE certum (derivata
         * constans, D exactum). Tum (alpha - c)^e (alpha^2 + 5) signum
         * sign(alpha - c)^e habere DEBET; D infra verum (Horner
         * intervallorum cum angulis omissis) signum falsum dat. */
        hic_manens constans character* polynomia[] = { "t^3 - 3t + 1",
            "t^3 - 3t + 1", "t^3 - 3t + 1", "t^3 + t^2 - 2t - 1",
            "t^5 - t - 1", "t^4 - 10t^2 + 1" };
        hic_manens constans s32 indices[] = { ZEPHYRUM, I, II, ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM };
        hic_manens constans character* propinquae[] = {
            "-18793852415718167/10000000000000000",
            "34729635533386069/100000000000000000",
            "15320888862379561/10000000000000000",
            "-18019377358048383/10000000000000000",
            "11673039782614187/10000000000000000",
            "-31462643699419726/10000000000000000" };
        b32 constantia_bene = VERUM;
        i32 c;

        imprimere("\n--- Recensio II: limes derivatae (N12) ---\n");
        per (c = ZEPHYRUM; c < VI; c++)
        {
            Extensio* k = extensio_ex_polynomio(_p(polynomia[c]),
                indices[c], piscina);
            Algebraicus differentia;
            Algebraicus positivum;
                Fractio q      = fractio_ex_s64(ZEPHYRUM);
                    s32 basis  = ZEPHYRUM;
                    s32 e;

            si (k == NIHIL)
            {
                constantia_bene = FALSUM;
                perge;
            }
            (vacuum)fractio_ex_chorda(chorda_ex_literis(
                propinquae[c], piscina), piscina, &q);
            differentia = algebraicus_subtrahe(algebraicus_generator(k,
                piscina), algebraicus_ex_fractione(k, q, piscina),
                piscina);
            positivum = _a(k, "a^2 + 5");
            si (   !algebraicus_signum(differentia, piscina, &basis)
                || basis == ZEPHYRUM)
            {
                constantia_bene = FALSUM;
                perge;
            }
            per (e = II; e <= VII; e++)
            {
                Algebraicus potentia;
                        s32 signum = ZEPHYRUM;
                        s32 expectatum;

                expectatum = (basis < ZEPHYRUM && (e & I)) ? -I : I;
                (vacuum)algebraicus_potentia(differentia, e, piscina,
                    &potentia);
                si (   !algebraicus_signum(algebraicus_multiplica(
                    potentia, positivum, piscina), piscina, &signum)
                    || signum != expectatum)
                {
                    constantia_bene = FALSUM;
                }
            }
        }
        CREDO_VERUM (constantia_bene);
    }

    {
        /* E25: potentiae altae per officinas - in piscina vocantis
         * solum effectus manet (Horner densus sordes O(N)
         * relinqueret) */
              Extensio* k_i  = extensio_quadratica(-I, piscina);
            Polynomium  p    = polynomium_nullum();
           Algebraicus  x;
        memoriae_index  ante;

        CREDO_VERUM (polynomium_ex_chorda(chorda_ex_literis(
            "t^200000 + 1", piscina), 't', piscina, &p));
        ante = piscina_summa_usus(piscina);
        CREDO_VERUM (algebraicus_ex_polynomio(k_i, p, magnus_ex_s64(I),
            piscina, &x));
        CREDO_MINOR_I32 ((i32)(piscina_summa_usus(piscina) - ante),
            IV * M);
        /* i^200000 + 1 = 2 */
        CREDO_VERUM (_textus_est(x, "2"));
    }

    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
