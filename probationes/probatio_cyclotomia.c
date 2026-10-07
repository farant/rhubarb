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

/* elementum fortuitum, coefficientes in [-9, 9] */
interior Cyclotomicus
_fortuitum (
    constans Cyclotomia* r,
                   Sors* s)
{
    Cyclotomicus a = cyclotomicus_nullum();
             i32 j;

    per (j = ZEPHYRUM; j < cyclotomia_gradus(r); j++)
    {
        a = cyclotomicus_adde(a, cyclotomicus_multiplica(r,
            cyclotomicus_integer(r, magnus_ex_s64(sors_inter(s, -IX,
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
            Cyclotomicus  summa = cyclotomicus_nullum();
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
            si (   !_integer_est(cyclotomicus_potentia(r, z, n,
                piscina), I)
                || !_integer_est(cyclotomicus_multiplica(r, z,
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
            si (   n > I && !magnus_aequalis(cyclotomicus_norma(r,
                    cyclotomicus_subtrahe(cyclotomicus_integer(r,
                    magnus_ex_s64(I), piscina), z, piscina), piscina),
                    magnus_ex_s64(p != ZEPHYRUM ? (s64)p : I)))
            {
                normae_bene = FALSUM;
            }
            si (   !magnus_aequalis(cyclotomicus_vestigium(r, z,
                piscina),
                    magnus_ex_s64(_mu(n)))
                || !magnus_aequalis(cyclotomicus_vestigium(r,
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
                Cyclotomicus ab = cyclotomicus_multiplica(r, a, b,
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
                si (   !cyclotomicus_automorphismus(r, a,
                    (s32)unitas_secunda,
                    piscina,
                        &x)
                    || !cyclotomicus_automorphismus(r, x,
                    (s32)unitas_prima,
                    piscina,
                        &x)
                    || !cyclotomicus_automorphismus(r, a,
                    (s32)(unitas_prima * unitas_secunda),
                        piscina, &y)
                    || !cyclotomicus_aequalis(x, y))
                {
                    galois_bene = FALSUM;
                }
                si (   !magnus_aequalis(cyclotomicus_norma(r, ab,
                    piscina),
                        magnus_multiplica(cyclotomicus_norma(r, a,
                        piscina),
                        cyclotomicus_norma(r, b, piscina), piscina))
                    || !magnus_aequalis(cyclotomicus_vestigium(r,
                        cyclotomicus_adde(a, b, piscina), piscina),
                        magnus_adde(cyclotomicus_vestigium(r, a,
                        piscina),
                        cyclotomicus_vestigium(r, b, piscina),
                        piscina)))
                {
                    normae_bene = FALSUM;
                }
                si (   !cyclotomicus_est_nullum(b)
                    && (!cyclotomicus_divide_exacte(r, ab, b, piscina,
                            &quotiens)
                        || !cyclotomicus_aequalis(quotiens, a)))
                {
                    divisio_bene = FALSUM;
                }
                si (   !cyclotomicus_ex_chorda(r,
                    cyclotomicus_ad_chordam(r,
                        a, piscina), piscina, &x)
                    || !cyclotomicus_aequalis(x, a))
                {
                    textus_bene = FALSUM;
                }
            }
            /* 1 / (1 - zeta) non integer (n = p^k: N = p) */
            si (   p != ZEPHYRUM
                && cyclotomicus_divide_exacte(r, cyclotomicus_integer(r,
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
        Cyclotomicus  w           = cyclotomicus_nullum();
                 s32  signum      = ZEPHYRUM;
                 i32  k           = ZEPHYRUM;

        imprimere("\n--- Probans summas Gauss, zeta_8, radices ---\n");
        per (q = ZEPHYRUM; q < V; q++)
        {
                     i32  p = primi[q];
              Cyclotomia* r = cyclotomia_creare(p, piscina);
            Cyclotomicus  g = cyclotomicus_nullum();
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
            si (!_integer_est(cyclotomicus_multiplica(r, g, g, piscina),
                    p % IV == I ? (s64)p : -(s64)p))
            {
                gauss_bene = FALSUM;
            }
        }
        CREDO_VERUM (gauss_bene);
        /* (zeta_8 + zeta_8^-1)^2 = 2 (radix 2); zeta_8^4 = -1 */
        w = cyclotomicus_adde(z8, cyclotomicus_radix(r8, -I, piscina),
            piscina);
        CREDO_VERUM (_integer_est(cyclotomicus_multiplica(r8, w, w,
            piscina),
            II));
        CREDO_VERUM (_integer_est(cyclotomicus_potentia(r8, z8, IV,
            piscina),
            -I));
        /* substitutio Laurent: t^-1 + t ad t = zeta_8^2 = i: 0 */
        CREDO_VERUM (cyclotomicus_ex_polynomio(r8, _p("t^-1 + t"), II,
            piscina, &w) && cyclotomicus_est_nullum(w));
        /* est_radix: zeta_5^3 (+, 3); -zeta_5^3 (-, 3); -zeta_8^3 =
         * zeta_8^7 (+, 7); 1 + zeta non radix */
        CREDO_VERUM (cyclotomicus_est_radix(r5, cyclotomicus_radix(r5,
            III,
            piscina), &signum, &k) && signum == I && k == III);
        CREDO_VERUM (cyclotomicus_est_radix(r5, cyclotomicus_nega(
            cyclotomicus_radix(r5, III, piscina), piscina), &signum, &k)
            && signum == -I && k == III);
        CREDO_VERUM (cyclotomicus_est_radix(r8, cyclotomicus_nega(
            cyclotomicus_radix(r8, III, piscina), piscina), &signum, &k)
            && signum == I && k == VII);
        CREDO_FALSUM (cyclotomicus_est_radix(r8, cyclotomicus_adde(z8,
            cyclotomicus_integer(r8, magnus_ex_s64(I), piscina),
            piscina),
            &signum, &k));
        /* |zeta^3|^2 = 1; |1 + i|^2 = 2 */
        CREDO_VERUM (_integer_est(cyclotomicus_modulus_quadratus(r8,
            cyclotomicus_radix(r8, III, piscina), piscina), I));
        CREDO_VERUM (_integer_est(cyclotomicus_modulus_quadratus(r8,
            cyclotomicus_adde(cyclotomicus_integer(r8, magnus_ex_s64(I),
            piscina), cyclotomicus_radix(r8, II, piscina), piscina),
            piscina), II));
        /* ostensio */
        CREDO_CHORDA_AEQUALIS_LITERIS (cyclotomicus_ad_ostendendum(r8,
            z8,
            IV, piscina), "0.7071 + 0.7071i");
        CREDO_CHORDA_AEQUALIS_LITERIS (cyclotomicus_ad_ostendendum(r8,
            cyclotomicus_radix(r8, II, piscina), IV, piscina),
            "0.0000 + 1.0000i");
        CREDO_CHORDA_AEQUALIS_LITERIS (cyclotomicus_ad_ostendendum(r8,
            cyclotomicus_radix(r8, V, piscina), II, piscina),
            "-0.71 - 0.71i");
        CREDO_CHORDA_AEQUALIS_LITERIS (cyclotomicus_ad_chordam(r8,
            cyclotomicus_radix(r8, V, piscina), piscina), "-z");
        /* matrix: Vandermonde nodorum 1, zeta, zeta^2 in Z[zeta_5]:
         * det = prod_{i < j} (x_j - x_i) */
        {
            constans Anulus* a5 = cyclotomia_anulus(r5);
                     Matrix  m;
               Cyclotomicus  det = cyclotomicus_nullum();
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
                    Cyclotomicus v = cyclotomicus_potentia(r5, x[i], j,
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
                    expectatum = cyclotomicus_multiplica(r5, expectatum,
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
