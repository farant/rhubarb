/* probatio_cyclotomia.c - Probationes integrorum cyclotomicorum
 *
 * Oracula: (1) polynomia cyclotomica nota (Phi_105 primum coefficientem
 * -2 habet, ad t^7 et t^41; gradus = phi(n) per numerationem gcd;
 * Phi_n(1) = p si n = p^k, aliter 1; Phi_2p(t) = Phi_p(-t)); (2)
 * identitates anuli (zeta^n = 1, summa radicum 0, N(1 - zeta) =
 * Phi_n(1), Tr(zeta) = mu(n), summae Gauss g^2 = (-1/p) p, sigma_a
 * sigma_b = sigma_ab, norma multiplicativa); (3) facta nodorum in
 * tabula tota (XIIDCCCCLXVI nodi): V(e^(2 pi i/3)) = 1, V(i) =
 * (-1)^Arf cum Arf ex det mod 8, |V(-1)| = det.
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
#include "cyclotomia.h"
#include "tabula_nodorum.h"
#include <stdio.h>
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

interior i32
_phi (
    i32 n)
{
    i32 k;
    i32 c = ZEPHYRUM;

    per (k = I; k <= n; k++)
    {
        si (_divisor_communis(k, n) == I)
        {
            c++;
        }
    }
    redde c;
}

/* mu(n): 0 si quadratum dividit, aliter (-1)^(numerus primorum) */
interior s32
_mu (
    i32 n)
{
    i32 p;
    s32 m = I;

    per (p = II; p * p <= n; p++)
    {
        si (n % p == ZEPHYRUM)
        {
            n = n / p;
            si (n % p == ZEPHYRUM)
            {
                redde ZEPHYRUM;
            }
            m = -m;
        }
    }
    si (n > I)
    {
        m = -m;
    }
    redde m;
}

/* n = p^k? reddit p, aliter 0 */
interior i32
_potentia_primi (
    i32 n)
{
    i32 p;

    per (p = II; p <= n; p++)
    {
        si (n % p == ZEPHYRUM)
        {
            dum (n % p == ZEPHYRUM)
            {
                n = n / p;
            }
            redde n == I ? p : ZEPHYRUM;
        }
    }
    redde ZEPHYRUM;
}

interior b32
_integer_est (
    Cyclotomicus a,
             s64 valor)
{
    Magnus m = magnus_ex_s64(ZEPHYRUM);

    redde cyclotomicus_est_integer(a, &m) && magnus_aequalis(m,
        magnus_ex_s64(valor));
}

/* p(-t): coefficiens t^e signo (-1)^e */
interior Polynomium
_variabilis_negata (
    Polynomium p)
{
    Polynomium summa = polynomium_nullum();
           s32 e;

    per (e = polynomium_gradus_imus(p); !polynomium_est_nullum(p)
        && e <= polynomium_gradus_summus(p); e++)
    {
        Polynomium terminus  = polynomium_nullum();
            Magnus c         = polynomium_coefficiens(p, e);

        si (e % II != ZEPHYRUM)
        {
            c = magnus_nega(c, piscina);
        }
        (vacuum)polynomium_monomium(c, e, piscina, &terminus);
        summa = polynomium_adde(summa, terminus, piscina);
    }
    redde summa;
}

/* norma, vestigium ut Magnus (FALSUM -> -1 et nuntius: numquam in
 * probationibus validis) */
interior Magnus
_norma (
    Cyclotomicus  a,
         Piscina* p)
{
    Magnus m = magnus_ex_s64(-I);

    si (!cyclotomicus_norma(a, p, &m))
    {
        imprimere("FRACTA: norma refutata\n");
    }
    redde m;
}

interior Magnus
_vestigium (
    Cyclotomicus  a,
         Piscina* p)
{
    Magnus m = magnus_ex_s64(-I);

    si (!cyclotomicus_vestigium(a, p, &m))
    {
        imprimere("FRACTA: vestigium refutatum\n");
    }
    redde m;
}

/* elementum fortuitum, coefficientes in [-9, 9] */
interior Cyclotomicus
_fortuitum (
    constans Cyclotomia* r,
                   Sors* s)
{
    Cyclotomicus a = cyclotomicus_nullum(r);
             i32 j;

    per (j = ZEPHYRUM; j < cyclotomia_gradus(r); j++)
    {
        a = cyclotomicus_adde(a,
            cyclotomicus_multiplica(cyclotomicus_integer(r,
            magnus_ex_s64(sors_inter(s, -IX,
            IX)),
            piscina), cyclotomicus_radix(r, (s32)j, piscina), piscina),
            piscina);
    }
    redde a;
}

s32 principale (vacuum)
{
    piscina = piscina_generare_dynamicum("probatio_cyclotomia",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * POLYNOMIA CYCLOTOMICA
     * ================================================== */

    {
        Polynomium phi = polynomium_nullum();
        Polynomium psi = polynomium_nullum();
               i32 n;
               b32 gradus_bene   = VERUM;
               b32 valores_bene  = VERUM;
               b32 parvi         = VERUM;
               b32 duplices      = VERUM;

        imprimere("\n--- Probans polynomia cyclotomica ---\n");
        CREDO_VERUM (polynomium_cyclotomicum(I, piscina, &phi)
            && polynomium_aequalis(phi, _p("t - 1")));
        CREDO_VERUM (polynomium_cyclotomicum(II, piscina, &phi)
            && polynomium_aequalis(phi, _p("t + 1")));
        CREDO_VERUM (polynomium_cyclotomicum(VI, piscina, &phi)
            && polynomium_aequalis(phi, _p("t^2 - t + 1")));
        CREDO_VERUM (polynomium_cyclotomicum(VIII, piscina, &phi)
            && polynomium_aequalis(phi, _p("t^4 + 1")));
        CREDO_VERUM (polynomium_cyclotomicum(XII, piscina, &phi)
            && polynomium_aequalis(phi, _p("t^4 - t^2 + 1")));
        CREDO_FALSUM (polynomium_cyclotomicum(ZEPHYRUM, piscina, &phi));
        CREDO_FALSUM (polynomium_cyclotomicum(M + I, piscina, &phi));
        per (n = I; n <= CCC; n++)
        {
             PiscinaNotatio nota = piscina_notare(piscina);
                    Fractio valor;
                        i32 p = _potentia_primi(n);
                        s32 e;

            si (   !polynomium_cyclotomicum(n, piscina, &phi)
                || polynomium_gradus_summus(phi) != (s32)_phi(n)
                || polynomium_gradus_imus(phi)   != ZEPHYRUM)
            {
                gradus_bene = FALSUM;
                piscina_reficere(piscina, nota);
                perge;
            }
            /* Phi_n(1) = p (n = p^k), 1 aliter (n >= 2) */
            si (   n >= II
                && (!polynomium_valor(phi, fractio_ex_s64(I), piscina,
                        &valor)
                    || !magnus_aequalis(fractio_numerator(valor),
                        magnus_ex_s64(p != ZEPHYRUM ? (s64)p : I))))
            {
                valores_bene = FALSUM;
            }
            /* coefficientes in {-1, 0, 1} pro n < 105 */
            per (e = ZEPHYRUM; n < CV
                && e <= polynomium_gradus_summus(phi);
                e++)
            {
                s64 c = ZEPHYRUM;

                si (   !magnus_ad_s64(polynomium_coefficiens(phi, e),
                    &c)
                    || c < -I || c > I)
                {
                    parvi = FALSUM;
                }
            }
            /* Phi_2p(t) = Phi_p(-t), p primus impar */
            si (   p == n && p > II && II * n <= CCC
                && (!polynomium_cyclotomicum(II * n, piscina, &psi)
                    || !polynomium_aequalis(psi, _variabilis_negata(
                        phi))))
            {
                duplices = FALSUM;
            }
            piscina_reficere(piscina, nota);
        }
        CREDO_VERUM (gradus_bene);
        CREDO_VERUM (valores_bene);
        CREDO_VERUM (parvi);
        CREDO_VERUM (duplices);
        /* Phi_105: -2 ad t^7 et t^41, primum extra {-1, 0, 1} */
        CREDO_VERUM (polynomium_cyclotomicum(CV, piscina, &phi)
            && magnus_aequalis(polynomium_coefficiens(phi, VII),
                magnus_ex_s64(-II))
            && magnus_aequalis(polynomium_coefficiens(phi, XLI),
                magnus_ex_s64(-II)));
    }


    /* ==================================================
     * ANULUS Z[zeta_n]
     * ================================================== */

    {
        i32 ordines[X] = { I, II, III, IV, V, VIII, XII, XVI, XXIV,
            CV };
         i32 q;
         b32 radices_bene   = VERUM;
         b32 summae_bene    = VERUM;
         b32 normae_bene    = VERUM;
         b32 vestigia_bene  = VERUM;
         b32 galois_bene    = VERUM;
         b32 divisio_bene   = VERUM;
         b32 textus_bene    = VERUM;
        Sors s;

        imprimere("\n--- Probans anulum Z[zeta_n] ---\n");
        sors_seminare(&s, CXXIII, I);
        CREDO_NIHIL (cyclotomia_creare(ZEPHYRUM, piscina));
        CREDO_NIHIL (cyclotomia_creare(M + I, piscina));
        per (q = ZEPHYRUM; q < X; q++)
        {
                     i32  n = ordines[q];
              Cyclotomia* r = cyclotomia_creare(n, piscina);
            Cyclotomicus  z;
            Cyclotomicus  summa = cyclotomicus_nullum(r);
                 Fractio  phi_unum;
                     i32  k;
                     i32  t;
                     i32  p = _potentia_primi(n);

            si (r == NIHIL)
            {
                radices_bene = FALSUM;
                perge;
            }
            z = cyclotomicus_radix(r, I, piscina);
            /* zeta^n = 1, zeta^-1 zeta = 1, summa radicum = 0
             * (n > 1) */
            si (   !_integer_est(cyclotomicus_potentia(z, n,
                piscina), I)
                || !_integer_est(cyclotomicus_multiplica(z,
                    cyclotomicus_radix(r, -I, piscina), piscina), I))
            {
                radices_bene = FALSUM;
            }
            per (k = ZEPHYRUM; k < n; k++)
            {
                summa = cyclotomicus_adde(summa, cyclotomicus_radix(r,
                    (s32)k, piscina), piscina);
            }
            si (n > I && !cyclotomicus_est_nullum(summa))
            {
                summae_bene = FALSUM;
            }
            /* N(1 - zeta) = Phi_n(1); Tr(zeta) = mu(n); Tr(1) = phi */
            (vacuum)polynomium_valor(cyclotomia_polynomium(r),
                fractio_ex_s64(I), piscina, &phi_unum);
            si (   n > I && !magnus_aequalis(_norma(
                    cyclotomicus_subtrahe(cyclotomicus_integer(r,
                    magnus_ex_s64(I), piscina), z, piscina), piscina),
                    magnus_ex_s64(p != ZEPHYRUM ? (s64)p : I)))
            {
                normae_bene = FALSUM;
            }
            si (   !magnus_aequalis(_vestigium( z,
                piscina),
                    magnus_ex_s64(_mu(n)))
                || !magnus_aequalis(_vestigium(
                    cyclotomicus_integer(r, magnus_ex_s64(I), piscina),
                    piscina), magnus_ex_s64((s64)cyclotomia_gradus(r))))
            {
                vestigia_bene = FALSUM;
            }
            /* elementa fortuita: sigma_a sigma_b = sigma_ab, norma
             * multiplicativa, vestigium additivum, (a b) / b = a */
            per (t = ZEPHYRUM; t < (n > XXIV ? II : V); t++)
            {
                Cyclotomicus a = _fortuitum(r, &s);
                Cyclotomicus b = _fortuitum(r, &s);
                Cyclotomicus x;
                Cyclotomicus y;
                Cyclotomicus ab = cyclotomicus_multiplica(a, b,
                    piscina);
                Cyclotomicus quotiens;
                         i32 unitas_prima    = I;
                         i32 unitas_secunda  = I;

                per (k = II; k < n; k++)
                {
                    si (_divisor_communis(k, n) == I)
                    {
                        si (unitas_prima == I)
                        {
                            unitas_prima = k;
                        }
                        alioquin
                        {
                            unitas_secunda = k;
                        }
                    }
                }
                si (   !cyclotomicus_automorphismus(a,
                    (s32)unitas_secunda,
                    piscina,
                        &x)
                    || !cyclotomicus_automorphismus(x,
                    (s32)unitas_prima,
                    piscina,
                        &x)
                    || !cyclotomicus_automorphismus(a,
                    (s32)(unitas_prima * unitas_secunda),
                        piscina, &y)
                    || !cyclotomicus_aequalis(x, y))
                {
                    galois_bene = FALSUM;
                }
                si (   !magnus_aequalis(_norma( ab,
                    piscina),
                        magnus_multiplica(_norma( a,
                        piscina),
                        _norma( b, piscina), piscina))
                    || !magnus_aequalis(_vestigium(
                        cyclotomicus_adde(a, b, piscina), piscina),
                        magnus_adde(_vestigium( a,
                        piscina),
                        _vestigium( b, piscina),
                        piscina)))
                {
                    normae_bene = FALSUM;
                }
                si (   !cyclotomicus_est_nullum(b)
                    && (!cyclotomicus_divide_exacte(ab, b, piscina,
                            &quotiens)
                        || !cyclotomicus_aequalis(quotiens, a)))
                {
                    divisio_bene = FALSUM;
                }
                si (   !cyclotomicus_ex_chorda(r,
                    cyclotomicus_ad_chordam(a, piscina), piscina, &x)
                    || !cyclotomicus_aequalis(x, a))
                {
                    textus_bene = FALSUM;
                }
            }
            /* 1 / (1 - zeta) non integer (n = p^k: N = p) */
            si (   p != ZEPHYRUM
                && cyclotomicus_divide_exacte(cyclotomicus_integer(r,
                    magnus_ex_s64(I), piscina), cyclotomicus_subtrahe(
                    cyclotomicus_integer(r, magnus_ex_s64(I), piscina),
                    z,
                    piscina), piscina, &summa))
            {
                divisio_bene = FALSUM;
            }
        }
        CREDO_VERUM (radices_bene);
        CREDO_VERUM (summae_bene);
        CREDO_VERUM (normae_bene);
        CREDO_VERUM (vestigia_bene);
        CREDO_VERUM (galois_bene);
        CREDO_VERUM (divisio_bene);
        CREDO_VERUM (textus_bene);
    }

    /* summae Gauss, zeta_8, radices, ostensio, matrix */
    {
                 i32  primi[V] = { III, V, VII, XI, XIII };
                 i32  q;
                 b32  gauss_bene  = VERUM;
          Cyclotomia* r8          = cyclotomia_creare(VIII, piscina);
          Cyclotomia* r5          = cyclotomia_creare(V, piscina);
        Cyclotomicus  z8          = cyclotomicus_radix(r8, I, piscina);
        Cyclotomicus  w           = cyclotomicus_nullum(r8);
                 s32  signum      = ZEPHYRUM;
                 i32  k           = ZEPHYRUM;

        imprimere("\n--- Probans summas Gauss, zeta_8, radices ---\n");
        per (q = ZEPHYRUM; q < V; q++)
        {
                     i32  p = primi[q];
              Cyclotomia* r = cyclotomia_creare(p, piscina);
            Cyclotomicus  g = cyclotomicus_nullum(r);
                     i32  a;

            per (a = I; a < p; a++)
            {
                i32 x;
                b32 residuum = FALSUM;

                per (x = I; x < p; x++)
                {
                    si ((x * x) % p == a)
                    {
                        residuum = VERUM;
                    }
                }
                g = residuum ? cyclotomicus_adde(g,
                    cyclotomicus_radix(r,
                    (s32)a, piscina),
                    piscina) : cyclotomicus_subtrahe(g,
                    cyclotomicus_radix(r, (s32)a, piscina), piscina);
            }
            /* g^2 = p si p = 1 mod 4, -p si p = 3 mod 4 */
            si (!_integer_est(cyclotomicus_multiplica(g, g, piscina),
                    p % IV == I ? (s64)p : -(s64)p))
            {
                gauss_bene = FALSUM;
            }
        }
        CREDO_VERUM (gauss_bene);
        /* (zeta_8 + zeta_8^-1)^2 = 2 (radix 2); zeta_8^4 = -1 */
        w = cyclotomicus_adde(z8, cyclotomicus_radix(r8, -I, piscina),
            piscina);
        CREDO_VERUM (_integer_est(cyclotomicus_multiplica(w, w,
            piscina),
            II));
        CREDO_VERUM (_integer_est(cyclotomicus_potentia(z8, IV,
            piscina),
            -I));
        /* substitutio Laurent: t^-1 + t ad t = zeta_8^2 = i: 0 */
        CREDO_VERUM (cyclotomicus_ex_polynomio(r8, _p("t^-1 + t"), II,
            piscina, &w) && cyclotomicus_est_nullum(w));
        /* est_radix: zeta_5^3 (+, 3); -zeta_5^3 (-, 3); -zeta_8^3 =
         * zeta_8^7 (+, 7); 1 + zeta non radix */
        CREDO_VERUM (cyclotomicus_est_radix(cyclotomicus_radix(r5,
            III,
            piscina), &signum, &k) && signum == I && k == III);
        CREDO_VERUM (cyclotomicus_est_radix(cyclotomicus_nega(
            cyclotomicus_radix(r5, III, piscina), piscina), &signum, &k)
            && signum == -I && k == III);
        CREDO_VERUM (cyclotomicus_est_radix(cyclotomicus_nega(
            cyclotomicus_radix(r8, III, piscina), piscina), &signum, &k)
            && signum == I && k == VII);
        CREDO_FALSUM (cyclotomicus_est_radix(cyclotomicus_adde(z8,
            cyclotomicus_integer(r8, magnus_ex_s64(I), piscina),
            piscina),
            &signum, &k));
        /* |zeta^3|^2 = 1; |1 + i|^2 = 2 */
        w = cyclotomicus_radix(r8, III, piscina);
        CREDO_VERUM (_integer_est(cyclotomicus_modulus_quadratus(w,
            piscina), I));
        w = cyclotomicus_adde(cyclotomicus_integer(r8, magnus_ex_s64(I),
            piscina), cyclotomicus_radix(r8, II, piscina), piscina);
        CREDO_VERUM (_integer_est(cyclotomicus_modulus_quadratus(w,
            piscina), II));
        /* ostensio */
        CREDO_CHORDA_AEQUALIS_LITERIS (cyclotomicus_ad_ostendendum(z8,
            IV, piscina), "0.7071 + 0.7071i");
        w = cyclotomicus_radix(r8, II, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS (cyclotomicus_ad_ostendendum(w,
            IV,
            piscina), "0.0000 + 1.0000i");
        w = cyclotomicus_radix(r8, V, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS (cyclotomicus_ad_ostendendum(w,
            II,
            piscina), "-0.71 - 0.71i");
        CREDO_CHORDA_AEQUALIS_LITERIS (cyclotomicus_ad_chordam(w,
            piscina),
            "-z");
        /* recensio: automorphismus non identitas (sigma_3(zeta_8) =
         * zeta_8^3, sigma_-1 = conjugatum), non unitates refutatae */
        CREDO_VERUM (cyclotomicus_automorphismus(z8, III, piscina,
            &w)
            && cyclotomicus_aequalis(w, cyclotomicus_radix(r8, III,
                piscina)));
        CREDO_VERUM (cyclotomicus_automorphismus(z8, -I, piscina,
            &w)
            && cyclotomicus_aequalis(w, cyclotomicus_conjugatum(z8,
                piscina)));
        CREDO_FALSUM (cyclotomicus_automorphismus(z8, II, piscina,
            &w));
        CREDO_FALSUM (cyclotomicus_automorphismus(z8, ZEPHYRUM,
            piscina, &w));
        CREDO_FALSUM (cyclotomicus_automorphismus(z8, VIII, piscina,
            &w));
        /* est_integer: 1 + zeta non integer; 5 integer */
        CREDO_FALSUM (cyclotomicus_est_integer(cyclotomicus_adde(z8,
            cyclotomicus_integer(r8, magnus_ex_s64(I), piscina),
            piscina),
            NIHIL));
        CREDO_VERUM (_integer_est(cyclotomicus_integer(r8,
            magnus_ex_s64(V),
            piscina), V));
        /* ordo maximus M creatur */
        CREDO_VERUM (cyclotomia_creare(M, piscina) != NIHIL
            && cyclotomia_gradus(cyclotomia_creare(M, piscina)) == CD);
        /* ostensio: zeta_8^6 = -i (signum, -0 non scribitur); numeri
         * magni per notationem e, sine exundatione alvei; 10^300 non in
         * 10^254 truncatum */
        w = cyclotomicus_radix(r8, VI, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS (cyclotomicus_ad_ostendendum(w,
            IV,
            piscina), "0.0000 - 1.0000i");
        {
                  Magnus decies = magnus_ex_s64(X);
            Cyclotomicus x;
                  chorda textus;

            x = cyclotomicus_integer(r8, magnus_potentia(decies, C,
                piscina), piscina);
            textus = cyclotomicus_ad_ostendendum(x, XV, piscina);
            CREDO_CHORDA_AEQUALIS_LITERIS (textus,
                "1.000000000000000e+100 + 0.000000000000000i");
            x = cyclotomicus_integer(r8, magnus_potentia(decies, CCC,
                piscina), piscina);
            textus = cyclotomicus_ad_ostendendum(x, IV, piscina);
            CREDO_CHORDA_AEQUALIS_LITERIS (textus,
                "1.0000e+300 + 0.0000i");
            x = cyclotomicus_potentia(cyclotomicus_adde(z8,
                cyclotomicus_integer(r8, magnus_ex_s64(I), piscina),
                piscina), CCXXX, piscina);
            textus = cyclotomicus_ad_ostendendum(x, IV, piscina);
            CREDO_VERUM (textus.mensura < C && textus.mensura > X);
        }
        /* matrix: Vandermonde nodorum 1, zeta, zeta^2 in Z[zeta_5]:
         * det = prod_{i < j} (x_j - x_i) */
        {
            constans Anulus* a5 = cyclotomia_anulus(r5);
                     Matrix  m;
               Cyclotomicus  det = cyclotomicus_nullum(r5);
               Cyclotomicus  expectatum;
               Cyclotomicus  x[III];
                        i32  i;
                        i32  j;

            x[ZEPHYRUM] = cyclotomicus_integer(r5, magnus_ex_s64(I),
                piscina);
            x[I]   = cyclotomicus_radix(r5, I, piscina);
            x[II]  = cyclotomicus_radix(r5, II, piscina);
            (vacuum)matrix_nulla(a5, III, III, piscina, &m);
            per (i = ZEPHYRUM; i < III; i++)
            {
                per (j = ZEPHYRUM; j < III; j++)
                {
                    Cyclotomicus v = cyclotomicus_potentia(x[i], j,
                        piscina);

                    matrix_pone(&m, i, j, &v);
                }
            }
            expectatum = cyclotomicus_integer(r5, magnus_ex_s64(I),
                piscina);
            per (i = ZEPHYRUM; i < III; i++)
            {
                per (j = i + I; j < III; j++)
                {
                    expectatum = cyclotomicus_multiplica(expectatum,
                        cyclotomicus_subtrahe(x[j], x[i], piscina),
                        piscina);
                }
            }
            CREDO_VERUM (matrix_determinans(m, piscina, &det)
                && cyclotomicus_aequalis(det, expectatum));
            CREDO_VERUM (strcmp(a5->titulus, "Z[zeta_5]") == ZEPHYRUM);
        }
    }


    /* ==================================================
     * ANULI SIGNATI ET INTEROPERATIO s64 (recensio I)
     * ================================================== */

    {
          Cyclotomia* r8      = cyclotomia_creare(VIII, piscina);
          Cyclotomia* r16     = cyclotomia_creare(XVI, piscina);
          Cyclotomia* r5      = cyclotomia_creare(V, piscina);
        Cyclotomicus  z8      = cyclotomicus_radix(r8, I, piscina);
        Cyclotomicus  z16     = cyclotomicus_radix(r16, I, piscina);
        Cyclotomicus  mixtum  = cyclotomicus_adde(z8, z16, piscina);
        Cyclotomicus  x;
              Magnus  m       = magnus_ex_s64(ZEPHYRUM);
                 s32  signum  = ZEPHYRUM;
                 i32  k       = ZEPHYRUM;
                 s64  quattuor[IV];
                 s64  octo[V];
                 s64  lectum[IV];

        imprimere("\n--- Probans anulos signatos et s64 ---\n");
        per (k = ZEPHYRUM; k < V; k++)
        {
            si (k < IV)
            {
                quattuor[k]  = (s64)(k + I);
                lectum[k]    = ZEPHYRUM;
            }
            octo[k] = k == IV ? I : ZEPHYRUM;
        }
        k = ZEPHYRUM;
        /* olim aequalis(zeta_8, zeta_16) VERUM: idem polynomium t */
        CREDO_FALSUM (cyclotomicus_aequalis(z8, z16));
        CREDO_VERUM (cyclotomicus_anulus(z8) == r8
            && cyclotomicus_anulus(z16) == r16);
        /* mixtum = invalidum, propagatur, refutatur */
        CREDO_FALSUM (cyclotomicus_est_validum(mixtum));
        CREDO_FALSUM (cyclotomicus_est_validum(cyclotomicus_multiplica(
            mixtum, z8, piscina)));
        x = cyclotomicus_multiplica(z8, z16, piscina);
        CREDO_FALSUM (cyclotomicus_est_validum(x));
        CREDO_FALSUM (cyclotomicus_est_nullum(cyclotomicus_subtrahe(z8,
            z16,
            piscina)));
        CREDO_FALSUM (cyclotomicus_aequalis(mixtum, mixtum));
        CREDO_FALSUM (cyclotomicus_norma(mixtum, piscina, &m));
        CREDO_FALSUM (cyclotomicus_vestigium(mixtum, piscina, &m));
        CREDO_FALSUM (cyclotomicus_divide_exacte(z8, z16, piscina, &x));
        CREDO_FALSUM (cyclotomicus_automorphismus(mixtum, III, piscina,
            &x));
        CREDO_FALSUM (cyclotomicus_est_radix(mixtum, &signum, &k));
        CREDO_FALSUM (cyclotomicus_est_integer(mixtum, NIHIL));
        CREDO_CHORDA_AEQUALIS_LITERIS (cyclotomicus_ad_chordam(mixtum,
            piscina), "invalidum");
        CREDO_CHORDA_AEQUALIS_LITERIS (cyclotomicus_ad_ostendendum(
            mixtum, IV, piscina), "invalidum");
        /* matrix super Z[zeta_5] cum elemento Z[zeta_8]: determinans
         * refutatur (anulus elementa aliena refutat) */
        {
                  Matrix matrix_mixta;
            Cyclotomicus unum = cyclotomicus_integer(r5,
                magnus_ex_s64(I),
                piscina);
            Cyclotomicus det = cyclotomicus_nullum(r5);

            (vacuum)matrix_nulla(cyclotomia_anulus(r5), II, II, piscina,
                &matrix_mixta);
            matrix_pone(&matrix_mixta, ZEPHYRUM, ZEPHYRUM, &unum);
            matrix_pone(&matrix_mixta, ZEPHYRUM, I, &z8);
            matrix_pone(&matrix_mixta, I, ZEPHYRUM, &unum);
            matrix_pone(&matrix_mixta, I, I, &unum);
            CREDO_FALSUM (matrix_determinans(matrix_mixta, piscina,
                &det));
        }
        /* operationes sine divisione (productum, summa): solum anulus
         * elementa aliena refutat - determinans etiam per divisionem
         * */
        {
                  Matrix ma;
                  Matrix mb;
                  Matrix mc;
            Cyclotomicus unum = cyclotomicus_integer(r5,
                magnus_ex_s64(I),
                piscina);

            (vacuum)matrix_nulla(cyclotomia_anulus(r5), I, I, piscina,
                &ma);
            (vacuum)matrix_nulla(cyclotomia_anulus(r5), I, I, piscina,
                &mb);
            matrix_pone(&ma, ZEPHYRUM, ZEPHYRUM, &z8);
            matrix_pone(&mb, ZEPHYRUM, ZEPHYRUM, &unum);
            CREDO_FALSUM (matrix_multiplica(ma, mb, piscina, &mc));
            CREDO_FALSUM (matrix_adde(ma, mb, piscina, &mc));
        }
        /* s64: 1 + 2z + 3z^2 + 4z^3 (Cyc8 demonstrationum), itus et
         * reditus; z^4 = -1 reducitur; coefficiens magnus refutatur */
        CREDO_VERUM (cyclotomicus_ex_s64(r8, quattuor, IV,
            piscina, &x)
            && cyclotomicus_ad_s64(x, lectum) && lectum[ZEPHYRUM] == I
            && lectum[I] == II && lectum[II] == III
            && lectum[III] == IV);
        CREDO_CHORDA_AEQUALIS_LITERIS (cyclotomicus_ad_chordam(x,
            piscina),
            "4z^3 + 3z^2 + 2z + 1");
        CREDO_VERUM (cyclotomicus_ex_s64(r8, octo, V, piscina, &x)
            && _integer_est(x, -I));
        x = cyclotomicus_integer(r8, magnus_potentia(magnus_ex_s64(X),
            XXX,
            piscina), piscina);
        CREDO_FALSUM (cyclotomicus_ad_s64(x, lectum));
        CREDO_FALSUM (cyclotomicus_ad_s64(mixtum, lectum));
        /* immersio Z[zeta_8] -> Z[zeta_16] (k = 2) homomorphismus:
         * zeta_8 -> zeta_16^2, producta servantur */
        {
            Cyclotomicus a = cyclotomicus_adde(z8,
                cyclotomicus_integer(r8,
                magnus_ex_s64(III), piscina), piscina);
            Cyclotomicus b = cyclotomicus_potentia(z8, III, piscina);
            Cyclotomicus ia;
            Cyclotomicus ib;
            Cyclotomicus imago_producti;

            CREDO_VERUM (cyclotomicus_ex_polynomio(r16, z8.p, II,
                piscina,
                &x)
                    && cyclotomicus_aequalis(x, cyclotomicus_radix(r16,
                    II,
                piscina)));
            CREDO_VERUM (cyclotomicus_ex_polynomio(r16, a.p, II,
                piscina,
                &ia)
                && cyclotomicus_ex_polynomio(r16, b.p, II, piscina, &ib)
                && cyclotomicus_ex_polynomio(r16,
                cyclotomicus_multiplica(a,
                    b, piscina).p, II, piscina, &imago_producti)
                && cyclotomicus_aequalis(imago_producti,
                cyclotomicus_multiplica(ia,
                    ib, piscina)));
        }
    }


    /* ==================================================
     * NODI: Jones ad radices unitatis (tabula tota)
     * ================================================== */

    {
        Cyclotomia* r2 = cyclotomia_creare(II, piscina);
        Cyclotomia* r3 = cyclotomia_creare(III, piscina);
        Cyclotomia* r4 = cyclotomia_creare(IV, piscina);
               i32  k;
               b32  tres_bene  = VERUM;
               b32  arf_bene   = VERUM;
               b32  det_bene   = VERUM;
               i32  arf_unum   = ZEPHYRUM;

        imprimere("\n--- Probans Jones ad radices unitatis ---\n");
        per (k = ZEPHYRUM; k < tabula_nodorum_numerus(); k++)
        {
            constans NodusTabulae* n     = tabula_nodorum_nodus(k);
                   PiscinaNotatio  nota  = piscina_notare(piscina);
                       Polynomium  j     = _p(n->jones);
                       Polynomium  a     = _p(n->alexander);
                     Cyclotomicus  v;
                          Fractio  valor;
                           Magnus  det;
                           Magnus  v_unus  = magnus_ex_s64(ZEPHYRUM);
                              s64  d       = ZEPHYRUM;
                              s64  arf;

            /* V(e^(2 pi i/3)) = 1 */
            si (   !cyclotomicus_ex_polynomio(r3, j, I, piscina, &v)
                || !_integer_est(v, I))
            {
                tres_bene = FALSUM;
            }
            /* det = |Delta(-1)|, impar */
            (vacuum)polynomium_valor(a, fractio_ex_s64(-I), piscina,
                &valor);
            det = magnus_absolutum(fractio_numerator(valor), piscina);
            (vacuum)magnus_ad_s64(det, &d);
            /* Arf = 0 sse det = +-1 mod 8 (Levine) */
            arf = (d % VIII == I || d % VIII == VII) ? ZEPHYRUM : I;
            arf_unum = arf_unum + (i32)arf;
            /* V(i) = (-1)^Arf */
            si (   !cyclotomicus_ex_polynomio(r4, j, I, piscina, &v)
                || !_integer_est(v, arf == ZEPHYRUM ? I : -I))
            {
                arf_bene = FALSUM;
            }
            /* |V(-1)| = det */
            si (   !cyclotomicus_ex_polynomio(r2, j, I, piscina, &v)
                || !cyclotomicus_est_integer(v, &v_unus)
                || !magnus_aequalis(magnus_absolutum(v_unus, piscina),
                det))
            {
                det_bene = FALSUM;
            }
            piscina_reficere(piscina, nota);
        }
        CREDO_VERUM (tres_bene);
        CREDO_VERUM (arf_bene);
        CREDO_VERUM (det_bene);
        /* Arf non trivialis: nodi Arf 1 adsunt (3_1: det 3) */
        CREDO_MAIOR_I32 (arf_unum, ZEPHYRUM);
        imprimere("  nodi cum Arf 1: %u ex %u\n", arf_unum,
            tabula_nodorum_numerus());
    }

    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
