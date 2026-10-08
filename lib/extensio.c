/* extensio.c - Corpora numerorum algebraicorum exacta (vide
 * include/extensio.h)
 */
#include "extensio.h"
#include "cyclotomia.h"
#include "matrix.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

structura Extensio {
           i32 gradus;           /* d */
    Polynomium f;                /* monicus, gradus d */
           s32 radix;            /* index radicis realis electae; -1 */
           i32 radices_reales;
           /* radix electa in (infra, supra), f(infra) f(supra) < 0 */
       Fractio infra;
       Fractio supra;
           /* alpha^-1 = inversa_numerator(alpha) / inversa_denominator,
            * si f(0) != 0 */
           b32 invertibilis;
    Polynomium inversa_numerator;
        Magnus inversa_denominator;
        Anulus anulus;
};


/* ==================================================
 * Elementa interna
 * ================================================== */

interior Algebraicus
_elementum (
     constans Extensio* k,
            Polynomium  numerator,
                Magnus  denominator)
{
    Algebraicus a;

    a.corpus       = k;
    a.numerator    = numerator;
    a.denominator  = denominator;
    redde a;
}

interior Algebraicus
_invalidum (vacuum)
{
    redde _elementum(NIHIL, polynomium_nullum(), magnus_ex_s64(I));
}

/* corpus commune duorum; NIHIL si mixta aut invalida */
interior constans Extensio*
_commune (
    Algebraicus a,
    Algebraicus b)
{
    redde a.corpus == b.corpus ? a.corpus : NIHIL;
}

/* officina: piscina temporaria operationis gravis (inversa, norma,
 * potentiae magnae); exitus in piscinam vocantis transcribitur, ne
 * sordes in ea maneant (recensio E1: inversa d = 24 ~1.5 MB sordium) */
interior Piscina*
_officina_aperire (vacuum)
{
    redde piscina_generare_dynamicum("extensio_officina", 65536);
}

/* elementum (in officina) -> piscina vocantis */
interior Algebraicus
_transcribere (
    Algebraicus  a,
        Piscina* piscina)
{
    redde _elementum(a.corpus, polynomium_transcribe(a.numerator,
        piscina), magnus_transcribe(a.denominator, piscina));
}

/* p (exponentes >= 0) modulo f: Horner a summo, t^d -> -(f - t^d)
 * (f monicus). Gradus exitus < d. */
interior Polynomium
_reducere (
     constans Extensio* k,
            Polynomium  p,
               Piscina* piscina)
{
        Magnus* alveus;
    Polynomium  exitus = polynomium_nullum();
           s32  e;
           i32  j;
           i32  d = k->gradus;

    si (polynomium_est_nullum(p))
    {
        redde exitus;
    }
    si (polynomium_gradus_summus(p) < (s32)d)
    {
        redde p;
    }
    alveus = (Magnus*)piscina_allocare(piscina, (memoriae_index)d
        * magnitudo(Magnus));
    per (j = ZEPHYRUM; j < d; j++)
    {
        alveus[j] = magnus_ex_s64(ZEPHYRUM);
    }
    per (e = polynomium_gradus_summus(p); e >= ZEPHYRUM; e--)
    {
        Magnus summus = alveus[d - I];

        /* alveus *= t */
        per (j = d - I; j > ZEPHYRUM; j--)
        {
            alveus[j] = alveus[j - I];
        }
        alveus[ZEPHYRUM] = magnus_ex_s64(ZEPHYRUM);
        si (magnus_signum(summus) != ZEPHYRUM)
        {
            per (j = ZEPHYRUM; j < d; j++)
            {
                Magnus fj = polynomium_coefficiens(k->f, (s32)j);

                si (magnus_signum(fj) != ZEPHYRUM)
                {
                    alveus[j] = magnus_subtrahe(alveus[j],
                        magnus_multiplica(summus, fj, piscina),
                        piscina);
                }
            }
        }
        alveus[ZEPHYRUM] = magnus_adde(alveus[ZEPHYRUM],
            polynomium_coefficiens(p, e), piscina);
    }
    (vacuum)polynomium_ex_coefficientibus(alveus, d, ZEPHYRUM, piscina,
        &exitus);
    redde exitus;
}

/* numerator reductus / denominator (non nullus) -> forma canonica */
interior Algebraicus
_normalizare (
     constans Extensio* k,
            Polynomium  numerator,
                Magnus  denominator,
               Piscina* piscina)
{
    Magnus g;

    si (polynomium_est_nullum(numerator))
    {
        redde _elementum(k, polynomium_nullum(), magnus_ex_s64(I));
    }
    g = magnus_divisor_communis(polynomium_contentum(numerator,
        piscina),
        denominator, piscina);
    si (magnus_compara(g, magnus_ex_s64(I)) != ZEPHYRUM)
    {
        Magnus residuum;

        (vacuum)polynomium_divide_exacte(numerator,
            polynomium_constans(g, piscina), piscina, &numerator);
        (vacuum)magnus_divide(denominator, g, piscina, &denominator,
            &residuum);
    }
    si (magnus_signum(denominator) < ZEPHYRUM)
    {
        numerator    = polynomium_nega(numerator, piscina);
        denominator  = magnus_nega(denominator, piscina);
    }
    redde _elementum(k, numerator, denominator);
}

/* matrix multiplicationis numeratoris (integra, d x d): columna j =
 * coefficientes numerator * alpha^j */
interior b32
_matrix_multiplicationis (
     constans Extensio* k,
            Polynomium  numerator,
               Piscina* piscina,
                Matrix* exitus)
{
    Polynomium columna = numerator;
           i32 i;
           i32 j;

    si (!matrix_nulla(&ANULUS_INTEGRORUM, k->gradus, k->gradus, piscina,
        exitus))
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < k->gradus; j++)
    {
        si (j > ZEPHYRUM)
        {
            Polynomium translata = polynomium_nullum();

            si (!polynomium_translata(columna, I, piscina, &translata))
            {
                redde FALSUM;
            }
            columna = _reducere(k, translata, piscina);
        }
        per (i = ZEPHYRUM; i < k->gradus; i++)
        {
            Magnus c = polynomium_coefficiens(columna, (s32)i);

            matrix_pone(exitus, i, j, &c);
        }
    }
    redde VERUM;
}


/* ==================================================
 * Anulus Q(alpha): elementa Algebraicus eiusdem corporis
 * ================================================== */

#define CORPUS_ANULI(anulus) ((constans Extensio*)(anulus)->contextus)
#define ELEMENTUM(x) (*(constans Algebraicus*)(x))

interior vacuum
_an_nullum (
    constans Anulus* anulus,
             vacuum* exitus)
{
    *(Algebraicus*)exitus = _elementum(CORPUS_ANULI(anulus),
        polynomium_nullum(), magnus_ex_s64(I));
}

interior vacuum
_an_unum (
    constans Anulus* anulus,
            Piscina* piscina,
             vacuum* exitus)
{
    *(Algebraicus*)exitus = algebraicus_ex_fractione(CORPUS_ANULI(
        anulus),
        fractio_ex_s64(I), piscina);
}

interior b32
_an_est_nullum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    redde ((constans Algebraicus*)a)->corpus == CORPUS_ANULI(anulus)
        && algebraicus_est_nullum(ELEMENTUM(a));
}

interior b32
_an_parvum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    redde _an_est_nullum(anulus, a);
}

interior b32
_an_aequalis (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b)
{
    redde ((constans Algebraicus*)a)->corpus == CORPUS_ANULI(anulus)
        && algebraicus_aequalis(ELEMENTUM(a), ELEMENTUM(b));
}

/* exitus validus et huius corporis? */
interior b32
_an_bonum (
    constans Anulus* anulus,
         Algebraicus c,
             vacuum* exitus)
{
    si (c.corpus != CORPUS_ANULI(anulus))
    {
        redde FALSUM;
    }
    *(Algebraicus*)exitus = c;
    redde VERUM;
}

interior b32
_an_adde (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    redde _an_bonum(anulus, algebraicus_adde(ELEMENTUM(a), ELEMENTUM(b),
        piscina), exitus);
}

interior b32
_an_subtrahe (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    redde _an_bonum(anulus, algebraicus_subtrahe(ELEMENTUM(a),
        ELEMENTUM(b), piscina), exitus);
}

interior b32
_an_multiplica (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    redde _an_bonum(anulus, algebraicus_multiplica(ELEMENTUM(a),
        ELEMENTUM(b), piscina), exitus);
}

interior b32
_an_divide_exacte (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    redde ((constans Algebraicus*)a)->corpus == CORPUS_ANULI(anulus)
        && algebraicus_divide(ELEMENTUM(a), ELEMENTUM(b), piscina,
        (Algebraicus*)exitus);
}

interior vacuum
_an_transcribe (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
             vacuum* exitus)
{
    constans Algebraicus* x = (constans Algebraicus*)a;

    (vacuum)anulus;
    *(Algebraicus*)exitus = _elementum(x->corpus, polynomium_transcribe(
        x->numerator, piscina), magnus_transcribe(x->denominator,
        piscina));
}

interior chorda
_an_ad_chordam (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina)
{
    (vacuum)anulus;
    redde algebraicus_ad_chordam(ELEMENTUM(a), piscina);
}

interior b32
_an_ex_chorda (
    constans Anulus* anulus,
              chorda  textus,
            Piscina*  piscina,
             vacuum*  exitus)
{
    redde algebraicus_ex_chorda(CORPUS_ANULI(anulus), textus, piscina,
        (Algebraicus*)exitus);
}


/* ==================================================
 * Corpora
 * ================================================== */


/* ==================================================
 * Sturm et isolatio radicum (exacta, super Z et Q)
 * ================================================== */

/* p' (exponentes >= 0) */
interior Polynomium
_derivata (
    Polynomium  p,
       Piscina* piscina)
{
        Magnus* c;
    Polynomium  exitus = polynomium_nullum();
           s32  summus;
           s32  e;

    si (polynomium_est_nullum(p) || polynomium_gradus_summus(p) < I)
    {
        redde exitus;
    }
    summus  = polynomium_gradus_summus(p);
    c       = (Magnus*)piscina_allocare(piscina, (memoriae_index)summus
        * magnitudo(Magnus));
    per (e = I; e <= summus; e++)
    {
        c[e - I] = magnus_multiplica(polynomium_coefficiens(p, e),
            magnus_ex_s64((s64)e), piscina);
    }
    (vacuum)polynomium_ex_coefficientibus(c, (i32)summus, ZEPHYRUM,
        piscina, &exitus);
    redde exitus;
}

/* p / contentum (signum servatur) */
interior Polynomium
_primitivum (
    Polynomium  p,
       Piscina* piscina)
{
    Magnus g;

    si (polynomium_est_nullum(p))
    {
        redde p;
    }
    g = polynomium_contentum(p, piscina);
    si (magnus_compara(g, magnus_ex_s64(I)) != ZEPHYRUM)
    {
        (vacuum)polynomium_divide_exacte(p, polynomium_constans(g,
            piscina), piscina, &p);
    }
    redde p;
}

/* residuum pseudo scala POSITIVA: |lc(b)|^k a mod b, signum ergo
 * servatum (catena Sturm valet) */
interior Polynomium
_residuum (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina)
{
       s32 gradus_b   = polynomium_gradus_summus(b);
    Magnus lc_b       = polynomium_coefficiens(b, gradus_b);
    Magnus absolutum  = magnus_absolutum(lc_b, piscina);
       s32 signum_b   = magnus_signum(lc_b);
       s32 gradus_initium;
       s32 iteratio;

    si (polynomium_est_nullum(a))
    {
        redde a;
    }
    /* quisque gradus terminum summum delet: summum gradus_a - gradus_b
     * + 1 passus. Limes STRUCTURALIS - vitium (terminus non
     * deletus) ansam infinitam cum memoria crescente fieret (planta
     * E11: 80 GB). */
    gradus_initium = polynomium_gradus_summus(a);
    per (iteratio = ZEPHYRUM; iteratio <= gradus_initium - gradus_b
        && !polynomium_est_nullum(a)
        && polynomium_gradus_summus(a) >= gradus_b; iteratio++)
    {
               s32 gradus_a   = polynomium_gradus_summus(a);
            Magnus lc_a       = polynomium_coefficiens(a, gradus_a);
        Polynomium translata  = polynomium_nullum();

        (vacuum)polynomium_translata(b, gradus_a - gradus_b, piscina,
            &translata);
        si (signum_b < ZEPHYRUM)
        {
            lc_a = magnus_nega(lc_a, piscina);
        }
        a = polynomium_subtrahe(polynomium_multiplica_scalari(a,
            absolutum, piscina),
            polynomium_multiplica_scalari(translata,
            lc_a, piscina), piscina);
    }
    redde a;
}

/* catena Sturm: S0 = f, S1 = f', S(k+1) = -prem(S(k-1), S(k)),
 * primitiva. Ultimum = gcd(f, f') usque ad scalam. */
interior Polynomium*
_catena_sturm (
    Polynomium  f,
       Piscina* piscina,
           i32* numerus)
{
    Polynomium* catena;
           i32  n;

    catena = (Polynomium*)piscina_allocare(piscina, (memoriae_index)(
        polynomium_gradus_summus(f) + II) * magnitudo(Polynomium));
    catena[ZEPHYRUM]  = f;
    catena[I]         = _primitivum(_derivata(f, piscina), piscina);
    n                 = II;
    /* capacitas gradus + 2: gradus stricte decrescunt (limes
     * structuralis, non solum mathematicus) */
    dum (   !polynomium_est_nullum(catena[n - I])
         && n < (i32)polynomium_gradus_summus(f) + II)
    {
        Polynomium r = _residuum(catena[n - II], catena[n - I],
            piscina);

        si (polynomium_est_nullum(r))
        {
            frange;
        }
        catena[n++] = _primitivum(polynomium_nega(r, piscina), piscina);
    }
    *numerus = n;
    redde catena;
}

/* signum q^g p(n/q) = sum c_i n^i q^(g-i) (q > 0): Horner homogeneus in
 * Z, sine fractionibus nec gcd (recensio II M2); sordes reficiuntur -
 * solum signum redditur */
interior s32
_signum_numeri (
    Polynomium  p,
        Magnus  n,
        Magnus  q,
       Piscina* piscina)
{
    PiscinaNotatio nota = piscina_notare(piscina);
            Magnus summa;
            Magnus potentia_q = magnus_ex_s64(I);
               s32 e;
               s32 s;

    si (polynomium_est_nullum(p))
    {
        redde ZEPHYRUM;
    }
    e      = polynomium_gradus_summus(p);
    summa  = polynomium_coefficiens(p, e);
    per (e = e - I; e >= ZEPHYRUM; e--)
    {
        potentia_q  = magnus_multiplica(potentia_q, q, piscina);
        summa       = magnus_adde(magnus_multiplica(summa, n, piscina),
            magnus_multiplica(polynomium_coefficiens(p, e), potentia_q,
            piscina), piscina);
    }
    s = magnus_signum(summa);
    piscina_reficere(piscina, nota);
    redde s;
}

interior s32
_signum_ad (
    Polynomium  p,
       Fractio  x,
       Piscina* piscina)
{
    redde _signum_numeri(p, fractio_numerator(x),
        fractio_denominator(x),
        piscina);
}

/* p(n/q) EXACTUM per Horner homogeneum in Z: N = sum c_i n^i q^(g-i),
 * valor N / q^g (fractio una, gcd unum) */
interior Fractio
_valor_numeri (
    Polynomium  p,
        Magnus  n,
        Magnus  q,
       Piscina* piscina)
{
    Magnus summa;
    Magnus potentia_q  = magnus_ex_s64(I);
   Fractio valor       = fractio_ex_s64(ZEPHYRUM);
       s32 e;

    si (polynomium_est_nullum(p))
    {
        redde valor;
    }
    e      = polynomium_gradus_summus(p);
    summa  = polynomium_coefficiens(p, e);
    per (e = e - I; e >= ZEPHYRUM; e--)
    {
        potentia_q  = magnus_multiplica(potentia_q, q, piscina);
        summa       = magnus_adde(magnus_multiplica(summa, n, piscina),
            magnus_multiplica(polynomium_coefficiens(p, e), potentia_q,
            piscina), piscina);
    }
    (vacuum)fractio_ex_magnis(summa, potentia_q, piscina, &valor);
    redde valor;
}

/* x = n / 2^k? *exponens = k. Intervalla radicum omnia dyadica sunt
 * (integri, /2^20, bisectiones a limite integro) */
interior b32
_denominator_binarius (
    Fractio  x,
    Piscina* piscina,
        i32* exponens)
{
    Magnus q     = fractio_denominator(x);
       i32 bita  = magnus_bitorum(q);

    *exponens = bita - I;
    redde magnus_aequalis(q, magnus_potentia(magnus_ex_s64(II), bita
        - I,
        piscina));
}

/* D = sum |c_i| i R^(i-1), R = max(|infra|, |supra|): limes |num'|
 * super intervallum TRIVIALITER validus (nulla arithmetica
 * intervallorum, cuius anguli omissi D infra verum darent - plantae
 * E17/E28); laxitas passus paucos (log) addit. */
interior Fractio
_derivata_limes (
    Polynomium  num,
       Fractio  infra,
       Fractio  supra,
       Piscina* piscina)
{
    Fractio radius    = fractio_absolutum(infra, piscina);
    Fractio summa     = fractio_ex_s64(ZEPHYRUM);
    Fractio potestas  = fractio_ex_s64(I);
        s32 e;

    si (fractio_compara(fractio_absolutum(supra, piscina), radius,
        piscina) > ZEPHYRUM)
    {
        radius = fractio_absolutum(supra, piscina);
    }
    per (e = I; e <= polynomium_gradus_summus(num); e++)
    {
        Magnus c = magnus_absolutum(polynomium_coefficiens(num, e),
            piscina);

        c = magnus_multiplica(c, magnus_ex_s64((s64)e), piscina);
        summa = fractio_adde(summa,
            fractio_multiplica(fractio_ex_magno(c),
            potestas, piscina), piscina);
        potestas = fractio_multiplica(potestas, radius, piscina);
    }
    redde summa;
}

/* (infra, supra) dyadica -> A / Q, C / Q cum Q = 2^S communi. FALSUM
 * si non dyadica (invarians: omnia intervalla radicum dyadica) */
interior b32
_intervallum_binarium (
    Fractio  infra,
    Fractio  supra,
    Piscina* piscina,
     Magnus* numerus_infra,
     Magnus* numerus_supra,
     Magnus* quantum)
{
    i32 exponens_infra;
    i32 exponens_supra;
    i32 exponens;

    si (   !_denominator_binarius(infra, piscina, &exponens_infra)
        || !_denominator_binarius(supra, piscina, &exponens_supra))
    {
        redde FALSUM;
    }
    exponens = exponens_infra > exponens_supra ? exponens_infra
        : exponens_supra;
    *quantum = magnus_potentia(magnus_ex_s64(II), exponens, piscina);
    *numerus_infra = magnus_multiplica(fractio_numerator(infra),
        magnus_potentia(magnus_ex_s64(II), exponens - exponens_infra,
        piscina), piscina);
    *numerus_supra = magnus_multiplica(fractio_numerator(supra),
        magnus_potentia(magnus_ex_s64(II), exponens - exponens_supra,
        piscina), piscina);
    redde VERUM;
}

/* limes superior log2 |x| (bita numeratoris - bita denominatoris + 1);
 * x nullum -> -(1 << 20) */
interior s32
_bita_fractionis (
    Fractio x)
{
    si (fractio_signum(x) == ZEPHYRUM)
    {
        redde -(s32)0x100000L;
    }
    redde (s32)magnus_bitorum(fractio_numerator(x))
        - (s32)magnus_bitorum(fractio_denominator(x)) + I;
}

interior i32
_bita_i32 (
    i32 n)
{
    i32 bita = ZEPHYRUM;

    dum (n != ZEPHYRUM)
    {
        bita++;
        n = n >> I;
    }
    redde bita;
}

/* profunditas bisectionum SUFFICIENS ab intervallo (-B, B] ad radices
 * separatas: separatio radicum f liberi quadratis >= sqrt 3 d^-(d+2)/2
 * |f|_2^(1-d) (Mahler-Mignotte; |disc| >= 1 in Z), ergo log2(2B/sep)
 * + margo. Limes STRUCTURALIS ex datis, non constans (recensio II M1:
 * MM fixum radices propinquas Mignotte refutabat). */
interior i32
_profunditas (
    Polynomium f)
{
    i32 d        = (i32)polynomium_gradus_summus(f);
    i32 maximum  = ZEPHYRUM;
    i32 norma;
    s32 e;

    per (e = ZEPHYRUM; e <= (s32)d; e++)
    {
        i32 b = magnus_bitorum(polynomium_coefficiens(f, e));

        si (b > maximum)
        {
            maximum = b;
        }
    }
    /* |f|_2 <= sqrt(d + 1) max |f_i| */
    norma = maximum + _bita_i32(d + I) / II + I;
    redde (maximum + II) + ((d + II) * _bita_i32(d)) / II + (d - I)
        * norma + VIII;
}

/* signum p ad +infinitum (directio +1) aut -infinitum (-1) */
interior s32
_signum_infinitum (
    Polynomium p,
           s32 directio)
{
    s32 summus  = polynomium_gradus_summus(p);
    s32 s       = magnus_signum(polynomium_coefficiens(p, summus));

    si (directio < ZEPHYRUM && (summus & I))
    {
        s = -s;
    }
    redde s;
}

/* mutationes signi catenae ad x (aut ad infinitum si directio != 0) */
interior i32
_variationes (
    constans Polynomium* catena,
                    i32  numerus,
                Fractio  x,
                    s32  directio,
                Piscina* piscina)
{
    i32 mutationes  = ZEPHYRUM;
    s32 prius       = ZEPHYRUM;
    i32 j;

    per (j = ZEPHYRUM; j < numerus; j++)
    {
        s32 s = directio != ZEPHYRUM
            ? _signum_infinitum(catena[j], directio)
            : _signum_ad(catena[j], x, piscina);

        si (s == ZEPHYRUM)
        {
            perge;
        }
        si (prius != ZEPHYRUM && s != prius)
        {
            mutationes++;
        }
        prius = s;
    }
    redde mutationes;
}

/* 1 + max |f_i| (f monicus): omnes radices in (-B, B) */
interior Fractio
_limes_cauchy (
    Polynomium  f,
       Piscina* piscina)
{
    Magnus maximum = magnus_ex_s64(ZEPHYRUM);
       s32 e;

    per (e = ZEPHYRUM; e < polynomium_gradus_summus(f); e++)
    {
        Magnus c = magnus_absolutum(polynomium_coefficiens(f, e),
            piscina);

        si (magnus_compara(c, maximum) > ZEPHYRUM)
        {
            maximum = c;
        }
    }
    redde fractio_ex_magno(magnus_adde(maximum, magnus_ex_s64(I),
        piscina));
}

interior Fractio
_medium (
    Fractio  a,
    Fractio  b,
    Piscina* piscina)
{
    Fractio exitus = fractio_ex_s64(ZEPHYRUM);

    (vacuum)fractio_divide(fractio_adde(a, b, piscina), fractio_ex_s64(
        II), piscina, &exitus);
    redde exitus;
}

/* intervallum pendens isolationis */
nomen structura {
    Fractio infra;
    Fractio supra;
        i32 variationes_infra;
        i32 variationes_supra;
        i32 profunditas;
} IntervallumPendens;

/* radices in (infra, supra] ordine crescente in alveos (capacitas d);
 * FALSUM si profunditas sufficiens (_profunditas) superata. ITERATIVA
 * cum acervo explicito (recensio III M1: recursio Mignotte a =
 * 10^1000 XVII milia tabularum acervi exhausit, SIGSEGV): sinistra
 * prius, dextra pendens - acervus <= limes + 1 tabulae. */
interior b32
_separare_intervallum (
    constans Polynomium* catena,
                    i32  numerus,
                Fractio  infra,
                Fractio  supra,
                    i32  variationes_infra,
                    i32  variationes_supra,
                    i32  limes,
                Fractio* radices_infra,
                Fractio* radices_supra,
                    i32* inventae,
                Piscina* piscina)
{
    IntervallumPendens* acervus;
                   i32  altitudo = ZEPHYRUM;

    acervus = (IntervallumPendens*)piscina_allocare(piscina,
        (memoriae_index)(limes
        + III) * magnitudo(IntervallumPendens));
    acervus[ZEPHYRUM].infra              = infra;
    acervus[ZEPHYRUM].supra              = supra;
    acervus[ZEPHYRUM].variationes_infra  = variationes_infra;
    acervus[ZEPHYRUM].variationes_supra  = variationes_supra;
    acervus[ZEPHYRUM].profunditas        = ZEPHYRUM;
    altitudo                             = I;
    dum (altitudo > ZEPHYRUM)
    {
        IntervallumPendens hoc = acervus[--altitudo];
                       i32 radices = hoc.variationes_infra
                           - hoc.variationes_supra;

        si (radices == ZEPHYRUM)
        {
            perge;
        }
        si (radices == I)
        {
            radices_infra[*inventae] = hoc.infra;
            radices_supra[*inventae] = hoc.supra;
            (*inventae)++;
            perge;
        }
        si (hoc.profunditas >= limes || altitudo + II > limes + III)
        {
            redde FALSUM;
        }
        {
            Fractio medium = _medium(hoc.infra, hoc.supra, piscina);
                i32 v = _variationes(catena, numerus, medium, ZEPHYRUM,
                    piscina);

            /* dextra prius in acervum, ut sinistra prius tractetur */
            acervus[altitudo].infra              = medium;
            acervus[altitudo].supra              = hoc.supra;
            acervus[altitudo].variationes_infra  = v;
            acervus[altitudo].variationes_supra =
                hoc.variationes_supra;
            acervus[altitudo].profunditas = hoc.profunditas + I;
            altitudo++;
            acervus[altitudo].infra = hoc.infra;
            acervus[altitudo].supra = medium;
            acervus[altitudo].variationes_infra =
                hoc.variationes_infra;
            acervus[altitudo].variationes_supra  = v;
            acervus[altitudo].profunditas        = hoc.profunditas + I;
            altitudo++;
        }
    }
    redde VERUM;
}

/* omnes radices reales distinctae f, ordine crescente: intervalla
 * (infra, supra], una radix in quoque; catena Sturm a vocante data */
interior b32
_separare (
             Polynomium   f,
    constans Polynomium*  catena,
                    i32   numerus,
                Piscina*  piscina,
                Fractio** radices_infra,
                Fractio** radices_supra,
                    i32*  inventae)
{
       Fractio limes      = _limes_cauchy(f, piscina);
       Fractio infra      = fractio_nega(limes, piscina);
           i32 capacitas  = (i32)polynomium_gradus_summus(f);

    *radices_infra  = (Fractio*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(Fractio));
    *radices_supra  = (Fractio*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(Fractio));
    *inventae       = ZEPHYRUM;
    redde _separare_intervallum(catena, numerus, infra, limes,
        _variationes(catena, numerus, infra, ZEPHYRUM, piscina),
        _variationes(catena, numerus, limes, ZEPHYRUM, piscina),
        _profunditas(f), *radices_infra, *radices_supra, inventae,
        piscina);
}

/* (infra, supra) cum f(infra) f(supra) < 0 per signum f bisecare donec
 * latitudo < latitudo_maxima. FALSUM si f(medium) = 0 (radix
 * rationalis: *rationalis = medium). */
interior b32
_angustare (
    Polynomium  f,
       Fractio* infra,
       Fractio* supra,
       Fractio  latitudo_maxima,
       Fractio* rationalis,
       Piscina* piscina)
{
    s32 signum_supra = _signum_ad(f, *supra, piscina);
    /* limes STRUCTURALIS: latitudo initialis / latitudo_maxima
     * (bita) */
    s32 limes = _bita_fractionis(fractio_subtrahe(*supra, *infra,
        piscina)) - _bita_fractionis(latitudo_maxima) + IV;
    s32 iteratio;

    per (iteratio = ZEPHYRUM; iteratio < limes; iteratio++)
    {
        Fractio medium;
            s32 s;

        si (fractio_compara(fractio_subtrahe(*supra, *infra, piscina),
            latitudo_maxima, piscina) < ZEPHYRUM)
        {
            redde VERUM;
        }
        medium  = _medium(*infra, *supra, piscina);
        s       = _signum_ad(f, medium, piscina);
        si (s == ZEPHYRUM)
        {
            *rationalis = medium;
            redde FALSUM;
        }
        si (s == signum_supra)
        {
            *supra = medium;
        }
        alioquin
        {
            *infra = medium;
        }
    }
    redde fractio_compara(fractio_subtrahe(*supra, *infra, piscina),
        latitudo_maxima, piscina) < ZEPHYRUM;
}

/* radices f > x, x = p/q, SI omnes radices f reales (aliter limes
 * superior tantum): mutationes signi coefficientium q^d f((p + w)/q)
 * = translatio Taylor polynomii integri P(x) = sum f_i q^(d-i) x^i per
 * p (regula Descartes). In officina; numerus solus redditur. */
interior i32
_radices_supra (
    Polynomium  f,
       Fractio  x,
       Piscina* piscina)
{
      Piscina* officina = _officina_aperire();
       Magnus* c;
       Magnus  p         = fractio_numerator(x);
       Magnus  q         = fractio_denominator(x);
       Magnus  potentia  = magnus_ex_s64(I);
          i32  d         = (i32)polynomium_gradus_summus(f);
          i32  i;
          i32  j;
          i32  mutationes  = ZEPHYRUM;
          s32  prius       = ZEPHYRUM;

    (vacuum)piscina;
    si (officina == NIHIL)
    {
        /* numerus impossibilis: vocans ad isolationem cadit */
        redde d + I;
    }
    c = (Magnus*)piscina_allocare(officina, (memoriae_index)(d + I)
        * magnitudo(Magnus));
    per (i = d; ; i--)
    {
        c[i] = magnus_multiplica(polynomium_coefficiens(f, (s32)i),
            potentia, officina);
        potentia = magnus_multiplica(potentia, q, officina);
        si (i == ZEPHYRUM)
        {
            frange;
        }
    }
    per (i = ZEPHYRUM; i < d; i++)
    {
        per (j = d - I; ; j--)
        {
            c[j] = magnus_adde(c[j], magnus_multiplica(p, c[j + I],
                officina), officina);
            si (j == i)
            {
                frange;
            }
        }
    }
    per (i = ZEPHYRUM; i <= d; i++)
    {
        s32 s = magnus_signum(c[i]);

        si (s == ZEPHYRUM)
        {
            perge;
        }
        si (prius != ZEPHYRUM && s != prius)
        {
            mutationes++;
        }
        prius = s;
    }
    piscina_destruere(officina);
    redde mutationes;
}

/* f monicus, gradus >= 1 (a vocante probatum) */
interior Extensio*
_creare (
             Polynomium  f,
                    s32  radix,
                    i32  radices_reales,
       constans Fractio* candidatum,
                    b32  certificatum,
                Piscina* piscina)
{
      Extensio* k;
        chorda  textus_f;
     character* titulus;
        Magnus  f0;

    k = (Extensio*)piscina_allocare(piscina, magnitudo(Extensio));
    k->gradus = (i32)polynomium_gradus_summus(f);
    k->f = f;
    k->radix = radix;
    k->radices_reales = radices_reales;
    k->infra = fractio_ex_s64(ZEPHYRUM);
    k->supra = fractio_ex_s64(ZEPHYRUM);
    si (radix >= ZEPHYRUM && k->gradus >= II)
    {
        /* radix electa isolata, latitudo < 2^-16; f irreducibilis:
         * extrema numquam radices */
         Fractio* radices_infra;
         Fractio* radices_supra;
             i32  inventae    = ZEPHYRUM;
         Fractio  latitudo    = fractio_ex_s64(ZEPHYRUM);
         Fractio  rationalis  = fractio_ex_s64(ZEPHYRUM);

        (vacuum)fractio_ex_s64_s64(I, 0x10000L, piscina, &latitudo);
        /* candidatum: aut CERTIFICATUM (ex isolatione Sturm vocantis:
         * radix INDICIS sola intus) aut familiae nominatae (omnes
         * radices reales): Descartes EXACTUS - radices > infra = d -
         * radix, radices > supra = d - radix - 1. Catena Sturm hic
         * gradu 498 2.8 GB edebat (crescit ut d^3); translatio Taylor
         * O(d^2) multiplicationibus parvis. */
        si (   candidatum != NIHIL && !certificatum
            && (   radices_reales != k->gradus
                || _radices_supra(f, candidatum[ZEPHYRUM], piscina)
                != k->gradus - (i32)radix
                || _radices_supra(f, candidatum[I], piscina)
                != k->gradus - (i32)radix - I))
        {
            /* familia nominata certificatum non implet: NIHIL CLAMANS,
             * non isolatio tota tacita (catena Sturm gradu 200 GB edit;
             * plantae E20/E24 recensionis) - solum si cos()
             * bibliothecae C prave erraret */
            redde NIHIL;
        }
        si (candidatum != NIHIL)
        {
            k->infra = candidatum[ZEPHYRUM];
            k->supra = candidatum[I];
        }
        alioquin
        {
            /* isolatio tota in officina; intervallum solum servatur */
               Piscina* officina = _officina_aperire();
            Polynomium* catena;
                   i32  numerus;
                   b32  inventa;

            si (officina == NIHIL)
            {
                redde NIHIL;
            }
            catena   = _catena_sturm(f, officina, &numerus);
            inventa  = _separare(f, catena, numerus, officina,
                &radices_infra, &radices_supra, &inventae)
                && (i32)radix < inventae;
            si (inventa)
            {
                k->infra = fractio_transcribe(radices_infra[radix],
                    piscina);
                k->supra = fractio_transcribe(radices_supra[radix],
                    piscina);
            }
            piscina_destruere(officina);
            si (!inventa)
            {
                redde NIHIL;
            }
        }
        si (   _signum_ad(f, k->supra, piscina) == ZEPHYRUM
            || _signum_ad(f, k->infra, piscina) == ZEPHYRUM
            || !_angustare(f, &k->infra, &k->supra, latitudo,
            &rationalis, piscina))
        {
            redde NIHIL;
        }
    }
    /* alpha (alpha^(d-1) + f_(d-1) alpha^(d-2) + ... + f_1) = -f_0 */
    f0                      = polynomium_coefficiens(f, ZEPHYRUM);
    k->invertibilis         = magnus_signum(f0) != ZEPHYRUM;
    k->inversa_numerator    = polynomium_nullum();
    k->inversa_denominator  = magnus_ex_s64(I);
    si (k->invertibilis)
    {
        Polynomium cauda = polynomium_subtrahe(f,
            polynomium_constans(f0,
            piscina), piscina);

        (vacuum)polynomium_translata(cauda, -I, piscina, &cauda);
        k->inversa_numerator    = polynomium_nega(cauda, piscina);
        k->inversa_denominator  = f0;
        si (magnus_signum(f0) < ZEPHYRUM)
        {
            k->inversa_numerator    = cauda;
            k->inversa_denominator  = magnus_nega(f0, piscina);
        }
    }
    /* titulus "Q(a), a^2 - 5" */
    textus_f  = polynomium_ad_chordam(f, 'a', piscina);
    titulus   = (character*)piscina_allocare(piscina, (memoriae_index)(
        textus_f.mensura + XII));
    sprintf(titulus, "Q(a), ");
    memcpy(titulus + VI, textus_f.datum, (size_t)textus_f.mensura);
    titulus[VI + textus_f.mensura]  = '\0';
    k->anulus.titulus               = titulus;
    k->anulus.mensura               = magnitudo(Algebraicus);
    k->anulus.corpus                = VERUM;
    k->anulus.nullum                = _an_nullum;
    k->anulus.unum                  = _an_unum;
    k->anulus.est_nullum            = _an_est_nullum;
    k->anulus.parvum                = _an_parvum;
    k->anulus.aequalis              = _an_aequalis;
    k->anulus.adde                  = _an_adde;
    k->anulus.subtrahe              = _an_subtrahe;
    k->anulus.multiplica            = _an_multiplica;
    k->anulus.divide_exacte         = _an_divide_exacte;
    k->anulus.transcribe            = _an_transcribe;
    k->anulus.ad_chordam            = _an_ad_chordam;
    k->anulus.ex_chorda             = _an_ex_chorda;
    k->anulus.divisor_communis      = NIHIL;
    k->anulus.divide_cum_residuo    = NIHIL;
    k->anulus.compara_normam        = NIHIL;
    k->anulus.contextus             = k;
    k->anulus.integrum              = VERUM;
    redde k;
}

Extensio*
extensio_quadratica (
         s64  d,
     Piscina* piscina)
{
           s64 absolutum;
           s64 p;
    Polynomium f = polynomium_nullum();
        Magnus c[III];

    /* fines ANTE negationem: -S64 minimum indefinitum (recensio E1,
     * F1) */
    si (   d == ZEPHYRUM || d == I || d >= (s64)0x80000000L
        || d <= -(s64)0x80000000L)
    {
        redde NIHIL;
    }
    absolutum = d < ZEPHYRUM ? -d : d;
    per (p = II; p * p <= absolutum; p++)
    {
        si (absolutum % (p * p) == ZEPHYRUM)
        {
            redde NIHIL;
        }
    }
    c[ZEPHYRUM]  = magnus_ex_s64(-d);
    c[I]         = magnus_ex_s64(ZEPHYRUM);
    c[II]        = magnus_ex_s64(I);
    (vacuum)polynomium_ex_coefficientibus(c, III, ZEPHYRUM, piscina,
        &f);
    /* radices -sqrt d < +sqrt d: index 1, in (r, r + 1], r = floor
     * sqrt d */
    si (d > ZEPHYRUM)
    {
        Fractio candidatum[II];
            s64 r = ZEPHYRUM;

        dum ((r + I) * (r + I) <= d)
        {
            r++;
        }
        candidatum[ZEPHYRUM]  = fractio_ex_s64(r);
        candidatum[I]         = fractio_ex_s64(r + I);
        redde _creare(f, I, II, candidatum, FALSUM, piscina);
    }
    redde _creare(f, -I, ZEPHYRUM, NIHIL, FALSUM, piscina);
}

Extensio*
extensio_cosinus (
         i32  n,
     Piscina* piscina)
{
    Polynomium phi  = polynomium_nullum();
    Polynomium f    = polynomium_nullum();
           i32 m;
           s32 j;

    si (n == ZEPHYRUM || n > CYCLOTOMIA_ORDO_MAXIMUS)
    {
        redde NIHIL;
    }
    si (n <= II)
    {
        /* 2 cos 2pi = 2, 2 cos pi = -2 */
        Magnus coefficientes[II];

        coefficientes[ZEPHYRUM]  = magnus_ex_s64(n == I ? -II : II);
        coefficientes[I]         = magnus_ex_s64(I);
        (vacuum)polynomium_ex_coefficientibus(coefficientes, II,
            ZEPHYRUM,
            piscina, &f);
        redde _creare(f, ZEPHYRUM, I, NIHIL, FALSUM, piscina);
    }
    si (!polynomium_cyclotomicum(n, piscina, &phi))
    {
        redde NIHIL;
    }
    /* t^-m Phi_n = phi_m + sum_{j >= 1} phi_(m+j) (t^j + t^-j), et
     * t^j + t^-j = C_j(t + 1/t): C_0 = 2, C_1 = x,
     * C_(j+1) = x C_j - C_(j-1)
     * (Chebyshev; O(m^2) - olim (t + 1/t)^j de novo, O(m^3), recensio
     * E1 F5). In officina; f solum transcribitur. */
    m = (i32)polynomium_gradus_summus(phi) / II;
    {
           Piscina* officina = _officina_aperire();
        Polynomium  psi;
        Polynomium  prior;
        Polynomium  currens = polynomium_nullum();

        si (officina == NIHIL)
        {
            redde NIHIL;
        }
        psi    = polynomium_constans(polynomium_coefficiens(phi,
            (s32)m),
            officina);
        prior  = polynomium_constans(magnus_ex_s64(II), officina);
        (vacuum)polynomium_monomium(magnus_ex_s64(I), I, officina,
            &currens);
        per (j = I; j <= (s32)m; j++)
        {
            Magnus c = polynomium_coefficiens(phi, (s32)m + j);

            si (magnus_signum(c) != ZEPHYRUM)
            {
                psi = polynomium_adde(psi,
                    polynomium_multiplica_scalari(
                    currens, c, officina), officina);
            }
            si (j < (s32)m)
            {
                Polynomium sequens = polynomium_nullum();

                (vacuum)polynomium_translata(currens, I, officina,
                    &sequens);
                sequens = polynomium_subtrahe(sequens, prior,
                    officina);
                prior    = currens;
                currens  = sequens;
            }
        }
        f = polynomium_transcribe(psi, piscina);
        piscina_destruere(officina);
    }
    /* radices 2 cos(2 pi j/n), j unitas: omnes reales, alpha maxima */
    {
        /* candidatum: 2 cos(2 pi/n) in f64 +- 2^-16 (denominator 2^20
         * parvus pro translatione Taylor; separatio a radice secunda >=
         * 2cos(2pi/n) - 2cos(4pi/n) ~ 1.2e-4 pro n <= M), per Descartes
         * verificatum in _creare */
        Fractio candidatum[II];
            f64 x = 2.0 * cos(2.0 * 3.14159265358979323846 / (f64)n);
            s64 centrum = (s64)floor(x * 1048576.0 + 0.5);

        (vacuum)fractio_ex_s64_s64(centrum - XVI, 0x100000L, piscina,
            &candidatum[ZEPHYRUM]);
        (vacuum)fractio_ex_s64_s64(centrum + XVI, 0x100000L, piscina,
            &candidatum[I]);
        redde _creare(f, (s32)m - I, m, candidatum, FALSUM, piscina);
    }
}

i32
extensio_gradus (
    constans Extensio* k)
{
    redde k->gradus;
}

Polynomium
extensio_polynomium (
    constans Extensio* k)
{
    redde k->f;
}

b32
extensio_ordinata (
    constans Extensio* k)
{
    redde k->radix >= ZEPHYRUM;
}

s32
extensio_radix (
    constans Extensio* k)
{
    redde k->radix;
}

/* f polynomium verum (exponentes >= 0) gradus >= 1? */
interior b32
_polynomium_verum (
    Polynomium f)
{
    redde !polynomium_est_nullum(f) && polynomium_gradus_imus(f)
        >= ZEPHYRUM && polynomium_gradus_summus(f) >= I;
}

b32
extensio_radices_reales (
    Polynomium  f,
       Piscina* piscina,
           i32* exitus)
{
       Piscina* officina;
    Polynomium* catena;
           i32  numerus;
       Fractio  nullum = fractio_ex_s64(ZEPHYRUM);

    si (!_polynomium_verum(f))
    {
        redde FALSUM;
    }
    (vacuum)piscina;
    officina = _officina_aperire();
    si (officina == NIHIL)
    {
        redde FALSUM;
    }
    catena   = _catena_sturm(f, officina, &numerus);
    *exitus  = _variationes(catena, numerus, nullum, -I, officina)
        - _variationes(catena, numerus, nullum, I, officina);
    piscina_destruere(officina);
    redde VERUM;
}

Extensio*
extensio_ex_polynomio (
    Polynomium  f,
           s32  radix,
       Piscina* piscina)
{
       Piscina* officina;
    Polynomium* catena;
           i32  numerus;
           i32  radices;
           s32  gradus;
       Fractio  candidatum[II];
           b32  candidatum_datum  = FALSUM;
           b32  bene              = VERUM;

    si (   !_polynomium_verum(f)
        || magnus_compara(polynomium_coefficiens(f,
        polynomium_gradus_summus(f)), magnus_ex_s64(I)) != ZEPHYRUM)
    {
        redde NIHIL;
    }
    gradus = polynomium_gradus_summus(f);
    /* omnia in officina, catena Sturm SEMEL (recensio II M2: olim
     * quater, gradu 40 1.4 GB in piscina vocantis); intervallum radicis
     * electae solum transcribitur */
    officina = _officina_aperire();
    si (officina == NIHIL)
    {
        redde NIHIL;
    }
    catena   = _catena_sturm(f, officina, &numerus);
    /* liber quadratis: ultimum catenae (gcd f, f') constans */
    bene     = polynomium_gradus_summus(catena[numerus - I])
        == ZEPHYRUM;
    radices  = (i32)(_variationes(catena, numerus, fractio_ex_s64(
        ZEPHYRUM), -I, officina) - _variationes(catena, numerus,
        fractio_ex_s64(ZEPHYRUM), I, officina));
    bene     = bene && radix >= -I && radix < (s32)radices;
    /* radix rationalis (f monicus: integra) -> reducibilis. Radices
     * reales omnes isolatae, ad latitudinem < 1 angustatae, integer
     * intra probatus. */
    si (bene && gradus > I && radices > ZEPHYRUM)
    {
         Fractio* radices_infra;
         Fractio* radices_supra;
             i32  inventae = ZEPHYRUM;
             i32  j;

        bene = _separare(f, catena, numerus, officina, &radices_infra,
            &radices_supra, &inventae) && inventae == radices;
        per (j = ZEPHYRUM; bene && j < inventae; j++)
        {
            Fractio infra       = radices_infra[j];
            Fractio supra       = radices_supra[j];
            Fractio rationalis  = fractio_ex_s64(ZEPHYRUM);
             Magnus pavimentum;

            /* (infra, supra]: radix in supra = rationalis; in infra =
             * vicinae, ibi capta */
            si (   _signum_ad(f, supra, officina) == ZEPHYRUM
                || _signum_ad(f, infra, officina) == ZEPHYRUM
                || !_angustare(f, &infra, &supra, fractio_ex_s64(I),
                &rationalis, officina))
            {
                bene = FALSUM;
                frange;
            }
            /* OMNES integri in (infra, supra] probantur (latitudo < 1:
             * unus summum) - correctio a latitudine non pendet */
            pavimentum = fractio_pavimentum(supra, officina);
            dum (   bene
                 && fractio_compara(fractio_ex_magno(pavimentum), infra,
                officina) > ZEPHYRUM)
            {
                si (_signum_ad(f, fractio_ex_magno(pavimentum),
                    officina)
                    == ZEPHYRUM)
                {
                    bene = FALSUM;
                }
                pavimentum = magnus_subtrahe(pavimentum,
                    magnus_ex_s64(I),
                    officina);
            }
            si (!bene)
            {
                frange;
            }
            si ((s32)j == radix)
            {
                candidatum[ZEPHYRUM] = fractio_transcribe(infra,
                    piscina);
                candidatum[I] = fractio_transcribe(supra,
                    piscina);
                candidatum_datum = VERUM;
            }
        }
    }
    piscina_destruere(officina);
    si (!bene)
    {
        redde NIHIL;
    }
    redde _creare(f, radix, radices,
        candidatum_datum ? candidatum : NIHIL,
        VERUM, piscina);
}

constans Anulus*
extensio_anulus (
    constans Extensio* k)
{
    redde &k->anulus;
}


/* ==================================================
 * Elementa
 * ================================================== */

Algebraicus
algebraicus_ex_fractione (
     constans Extensio* k,
               Fractio  q,
               Piscina* piscina)
{
    si (k == NIHIL)
    {
        redde _invalidum();
    }
    redde _normalizare(k, polynomium_constans(fractio_numerator(q),
        piscina), fractio_denominator(q), piscina);
}

Algebraicus
algebraicus_generator (
    constans Extensio* k,
              Piscina* piscina)
{
    Polynomium t = polynomium_nullum();

    si (k == NIHIL)
    {
        redde _invalidum();
    }
    (vacuum)polynomium_monomium(magnus_ex_s64(I), I, piscina, &t);
    redde _normalizare(k, _reducere(k, t, piscina), magnus_ex_s64(I),
        piscina);
}

/* t^g modulo f (g >= 0) per quadrata */
interior Polynomium
_potentia_t (
     constans Extensio* k,
                   s32  g,
               Piscina* officina)
{
    Polynomium summa = polynomium_constans(magnus_ex_s64(I), officina);
    Polynomium basis = polynomium_nullum();
    Polynomium productum = polynomium_nullum();

    (vacuum)polynomium_monomium(magnus_ex_s64(I), I, officina, &basis);
    basis = _reducere(k, basis, officina);
    dum (g > ZEPHYRUM)
    {
        si (g & I)
        {
            (vacuum)polynomium_multiplica(summa, basis, officina,
                &productum);
            summa = _reducere(k, productum, officina);
        }
        g = g >> I;
        si (g > ZEPHYRUM)
        {
            (vacuum)polynomium_multiplica(basis, basis, officina,
                &productum);
            basis = _reducere(k, productum, officina);
        }
    }
    redde summa;
}

/* acc t^g modulo f (acc reductum, g >= 0) */
interior Polynomium
_per_t (
     constans Extensio* k,
            Polynomium  acc,
                   s32  g,
               Piscina* officina)
{
    Polynomium productum = polynomium_nullum();

    si (g == ZEPHYRUM || polynomium_est_nullum(acc))
    {
        redde acc;
    }
    si (g <= (s32)(II * k->gradus))
    {
        (vacuum)polynomium_translata(acc, g, officina, &productum);
        redde _reducere(k, productum, officina);
    }
    (vacuum)polynomium_multiplica(acc, _potentia_t(k, g, officina),
        officina, &productum);
    redde _reducere(k, productum, officina);
}

/* q(alpha) / denominator, q exponentibus >= 0. Gradu parvo (< 2d)
 * Horner densus; aliter per terminos, alpha^e per potentias (log e
 * multiplicationes) in officina: "a^100000" olim 1.1 s et 427 MB per
 * Hornerum (passus unus per exponentem), "a^1073741823" numquam
 * redibat (recensio E1 F2). */
interior b32
_ex_positivo (
     constans Extensio* k,
            Polynomium  q,
                Magnus  denominator,
               Piscina* piscina,
           Algebraicus* exitus)
{
         Piscina* officinae[II];
  PiscinaNotatio  notae[II];
             i32  currens = ZEPHYRUM;
      Polynomium  acc;
             s32  prior = -I;
             s32  e;

    si (   polynomium_est_nullum(q)
        || polynomium_gradus_summus(q) < (s32)(II * k->gradus))
    {
        *exitus = _normalizare(k, _reducere(k, q, piscina), denominator,
            piscina);
        redde VERUM;
    }
    /* Horner SPARSUS in Z[t] modulo f: acc = acc t^saltus + c per
     * terminos non nullos a summo (recensio II M3: potentia per
     * terminum densum O(N log N) multiplicationum fecit). Saltus parvus
     * (<= 2d) = translatio et reductio; magnus = t^saltus per
     * quadrata. Duae officinae alternant: acc solum superest. */
    officinae[ZEPHYRUM]  = _officina_aperire();
    officinae[I]         = _officina_aperire();
    si (officinae[ZEPHYRUM] == NIHIL || officinae[I] == NIHIL)
    {
        si (officinae[ZEPHYRUM] != NIHIL)
        {
            piscina_destruere(officinae[ZEPHYRUM]);
        }
        si (officinae[I] != NIHIL)
        {
            piscina_destruere(officinae[I]);
        }
        redde FALSUM;
    }
    notae[ZEPHYRUM]  = piscina_notare(officinae[ZEPHYRUM]);
    notae[I]         = piscina_notare(officinae[I]);
    acc              = polynomium_nullum();
    per (e = polynomium_gradus_summus(q); e
        >= polynomium_gradus_imus(q);
        e--)
    {
        Magnus c = polynomium_coefficiens(q, e);

        si (magnus_signum(c) == ZEPHYRUM)
        {
            perge;
        }
        si (prior >= ZEPHYRUM)
        {
            acc = _per_t(k, acc, prior - e, officinae[currens]);
        }
        acc    = polynomium_adde(acc, polynomium_constans(c,
            officinae[currens]), officinae[currens]);
        prior  = e;
        acc    = polynomium_transcribe(acc, officinae[I - currens]);
        piscina_reficere(officinae[currens], notae[currens]);
        currens = I - currens;
    }
    acc = _per_t(k, acc, prior, officinae[currens]);
    *exitus = _transcribere(_normalizare(k, acc, denominator,
        officinae[currens]), piscina);
    piscina_destruere(officinae[ZEPHYRUM]);
    piscina_destruere(officinae[I]);
    redde VERUM;
}

b32
algebraicus_ex_polynomio (
     constans Extensio* k,
            Polynomium  p,
                Magnus  denominator,
               Piscina* piscina,
           Algebraicus* exitus)
{
    s32 imus;

    si (k == NIHIL || magnus_signum(denominator) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (polynomium_est_nullum(p))
    {
        *exitus = _elementum(k, polynomium_nullum(), magnus_ex_s64(I));
        redde VERUM;
    }
    imus = polynomium_gradus_imus(p);
    si (imus >= ZEPHYRUM)
    {
        redde _ex_positivo(k, p, denominator, piscina, exitus);
    }
    /* p = t^imus q, q exponentibus >= 0:
     * q(alpha) * (alpha^-1)^(-imus) */
    {
         Polynomium q = polynomium_nullum();
        Algebraicus inversa;
        Algebraicus potentia;
        Algebraicus positivum;

        si (   !k->invertibilis
            || !polynomium_translata(p, -imus, piscina, &q))
        {
            redde FALSUM;
        }
        inversa = _elementum(k, k->inversa_numerator,
            k->inversa_denominator);
        si (   !algebraicus_potentia(inversa, -imus, piscina, &potentia)
            || !_ex_positivo(k, q, denominator, piscina, &positivum))
        {
            redde FALSUM;
        }
        *exitus = algebraicus_multiplica(positivum, potentia, piscina);
        redde VERUM;
    }
}

Fractio
algebraicus_coefficiens (
    Algebraicus  a,
            i32  j,
        Piscina* piscina)
{
    Fractio q = fractio_ex_s64(ZEPHYRUM);

    si (a.corpus == NIHIL || j >= a.corpus->gradus)
    {
        redde q;
    }
    (vacuum)fractio_ex_magnis(polynomium_coefficiens(a.numerator,
        (s32)j), a.denominator, piscina, &q);
    redde q;
}

b32
algebraicus_est_validum (
    Algebraicus a)
{
    redde a.corpus != NIHIL;
}

constans Extensio*
algebraicus_corpus (
    Algebraicus a)
{
    redde a.corpus;
}


/* ==================================================
 * Arithmetica
 * ================================================== */

Algebraicus
algebraicus_adde (
    Algebraicus  a,
    Algebraicus  b,
        Piscina* piscina)
{
    constans Extensio* k = _commune(a, b);

    si (k == NIHIL)
    {
        redde _invalidum();
    }
    redde _normalizare(k, polynomium_adde(polynomium_multiplica_scalari(
        a.numerator, b.denominator, piscina),
        polynomium_multiplica_scalari(b.numerator, a.denominator,
        piscina), piscina), magnus_multiplica(a.denominator,
        b.denominator, piscina), piscina);
}

Algebraicus
algebraicus_nega (
    Algebraicus  a,
        Piscina* piscina)
{
    si (a.corpus == NIHIL)
    {
        redde _invalidum();
    }
    redde _elementum(a.corpus, polynomium_nega(a.numerator, piscina),
        a.denominator);
}

Algebraicus
algebraicus_subtrahe (
    Algebraicus  a,
    Algebraicus  b,
        Piscina* piscina)
{
    si (_commune(a, b) == NIHIL)
    {
        redde _invalidum();
    }
    redde algebraicus_adde(a, algebraicus_nega(b, piscina), piscina);
}

Algebraicus
algebraicus_multiplica (
    Algebraicus  a,
    Algebraicus  b,
        Piscina* piscina)
{
    constans Extensio* k          = _commune(a, b);
           Polynomium  productum  = polynomium_nullum();

    si (k == NIHIL)
    {
        redde _invalidum();
    }
    /* gradus < 2d - 1: exponentes semper intra fines */
    (vacuum)polynomium_multiplica(a.numerator, b.numerator, piscina,
        &productum);
    redde _normalizare(k, _reducere(k, productum, piscina),
        magnus_multiplica(a.denominator, b.denominator, piscina),
        piscina);
}

/* numerator^-1 per nucleum [M | -e_0]; FALSUM si nucleus non unius
 * dimensionis aut scala nullum (divisor nullius: f reducibilis) */
interior b32
_inversa_per_nucleum (
     constans Extensio* k,
           Algebraicus  a,
               Piscina* officina,
               Piscina* piscina,
           Algebraicus* exitus)
{
        Matrix  m;
        Matrix  augmentata;
        Matrix  nucleus;
        Magnus  minus_unum = magnus_ex_s64(-I);
        Magnus  scala;
        Magnus* x;
           i32  i;
           i32  j;
    Polynomium  numerator = polynomium_nullum();

    si (   !_matrix_multiplicationis(k, a.numerator, officina, &m)
        || !matrix_nulla(&ANULUS_INTEGRORUM, k->gradus, k->gradus + I,
        officina, &augmentata))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < k->gradus; i++)
    {
        per (j = ZEPHYRUM; j < k->gradus; j++)
        {
            matrix_pone(&augmentata, i, j, matrix_elementum(m, i, j));
        }
    }
    matrix_pone(&augmentata, ZEPHYRUM, k->gradus, &minus_unum);
    si (   !matrix_nucleus(augmentata, officina, &nucleus)
        || matrix_columnae(nucleus) != I)
    {
        redde FALSUM;
    }
    scala = *(constans Magnus*)matrix_elementum(nucleus, k->gradus,
        ZEPHYRUM);
    si (magnus_signum(scala) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    x = (Magnus*)piscina_allocare(officina, (memoriae_index)k->gradus
        * magnitudo(Magnus));
    per (i = ZEPHYRUM; i < k->gradus; i++)
    {
        x[i] = magnus_multiplica(*(constans Magnus*)matrix_elementum(
            nucleus, i, ZEPHYRUM), a.denominator, officina);
    }
    (vacuum)polynomium_ex_coefficientibus(x, k->gradus, ZEPHYRUM,
        officina, &numerator);
    *exitus = _transcribere(_normalizare(k, numerator, scala,
        officina),
        piscina);
    redde VERUM;
}

b32
algebraicus_inversum (
    Algebraicus  a,
        Piscina* piscina,
    Algebraicus* exitus)
{
    constans Extensio* k = a.corpus;

    si (k == NIHIL || polynomium_est_nullum(a.numerator))
    {
        redde FALSUM;
    }
    si (k->gradus == I)
    {
        /* a = c / den: 1/a = den / c */
        *exitus = _normalizare(k, polynomium_constans(a.denominator,
            piscina), polynomium_coefficiens(a.numerator, ZEPHYRUM),
            piscina);
        redde VERUM;
    }
    si (k->gradus == II)
    {
        /* f = t^2 + b t + c, a = (p + q alpha)/den:
         * N(p + q alpha) = p^2 - b p q + c q^2,
         * (p + q alpha)^-1 = (p - b q - q alpha) / N */
            Magnus fb  = polynomium_coefficiens(k->f, I);
            Magnus fc  = polynomium_coefficiens(k->f, ZEPHYRUM);
            Magnus p   = polynomium_coefficiens(a.numerator, ZEPHYRUM);
            Magnus q   = polynomium_coefficiens(a.numerator, I);
            Magnus norma;
            Magnus c[II];
        Polynomium numerator = polynomium_nullum();

        norma = magnus_adde(magnus_subtrahe(magnus_multiplica(p, p,
            piscina), magnus_multiplica(fb, magnus_multiplica(p, q,
            piscina), piscina), piscina), magnus_multiplica(fc,
            magnus_multiplica(q, q, piscina), piscina), piscina);
        si (magnus_signum(norma) == ZEPHYRUM)
        {
            redde FALSUM;
        }
        c[ZEPHYRUM]  = magnus_multiplica(a.denominator,
            magnus_subtrahe(p,
            magnus_multiplica(fb, q, piscina), piscina), piscina);
        c[I]         = magnus_nega(magnus_multiplica(a.denominator, q,
            piscina), piscina);
        (vacuum)polynomium_ex_coefficientibus(c, II, ZEPHYRUM, piscina,
            &numerator);
        *exitus = _normalizare(k, numerator, norma, piscina);
        redde VERUM;
    }
    /* nucleus [M | -e_0] super Z: (x, scala), M x = scala e_0, ergo
     * numerator^-1 = x / scala. Solutio UNA sine fractionibus (olim
     * Cramer, d + 1 determinantes; recensio E1 F3: d = 24 1.1 s). In
     * officina; exitus transcribitur. */
    {
         Piscina* officina = _officina_aperire();
             b32  bene;

        si (officina == NIHIL)
        {
            redde FALSUM;
        }
        bene = _inversa_per_nucleum(k, a, officina, piscina, exitus);
        piscina_destruere(officina);
        redde bene;
    }
}

b32
algebraicus_divide (
    Algebraicus  a,
    Algebraicus  b,
        Piscina* piscina,
    Algebraicus* exitus)
{
    Algebraicus inversa;

    si (   _commune(a, b) == NIHIL || !algebraicus_inversum(b, piscina,
        &inversa))
    {
        redde FALSUM;
    }
    *exitus = algebraicus_multiplica(a, inversa, piscina);
    redde VERUM;
}

b32
algebraicus_potentia (
    Algebraicus  a,
            s32  e,
        Piscina* piscina,
    Algebraicus* exitus)
{
    Algebraicus summa;
            i32 n;

    si (a.corpus == NIHIL)
    {
        redde FALSUM;
    }
    si (e < ZEPHYRUM)
    {
        si (!algebraicus_inversum(a, piscina, &a))
        {
            redde FALSUM;
        }
        n = (i32)(-(e + I)) + I;
    }
    alioquin
    {
        n = (i32)e;
    }
    summa = algebraicus_ex_fractione(a.corpus, fractio_ex_s64(I),
        piscina);
    dum (n > ZEPHYRUM)
    {
        si (n & I)
        {
            summa = algebraicus_multiplica(summa, a, piscina);
        }
        n = n >> I;
        si (n > ZEPHYRUM)
        {
            a = algebraicus_multiplica(a, a, piscina);
        }
    }
    *exitus = summa;
    redde VERUM;
}


/* ==================================================
 * Exacta
 * ================================================== */

b32
algebraicus_aequalis (
    Algebraicus a,
    Algebraicus b)
{
    redde _commune(a, b) != NIHIL
        && polynomium_aequalis(a.numerator, b.numerator)
        && magnus_aequalis(a.denominator, b.denominator);
}

b32
algebraicus_est_nullum (
    Algebraicus a)
{
    redde a.corpus != NIHIL && polynomium_est_nullum(a.numerator);
}

b32
algebraicus_est_rationalis (
    Algebraicus  a,
        Piscina* piscina,
        Fractio* valor)
{
    si (a.corpus == NIHIL)
    {
        redde FALSUM;
    }
    si (   !polynomium_est_nullum(a.numerator)
        && polynomium_gradus_summus(a.numerator) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (valor != NIHIL)
    {
        *valor = algebraicus_coefficiens(a, ZEPHYRUM, piscina);
    }
    redde VERUM;
}

b32
algebraicus_norma (
    Algebraicus  a,
        Piscina* piscina,
        Fractio* exitus)
{
     Matrix  m;
     Magnus  det = magnus_ex_s64(ZEPHYRUM);
    Piscina* officina;
        b32  bene;

    si (a.corpus == NIHIL)
    {
        redde FALSUM;
    }
    si (polynomium_est_nullum(a.numerator))
    {
        *exitus = fractio_ex_s64(ZEPHYRUM);
        redde VERUM;
    }
    officina = _officina_aperire();
    si (officina == NIHIL)
    {
        redde FALSUM;
    }
    bene = _matrix_multiplicationis(a.corpus, a.numerator, officina, &m)
        && matrix_determinans(m, officina, &det);
    si (bene)
    {
        det = magnus_transcribe(det, piscina);
    }
    piscina_destruere(officina);
    redde bene && fractio_ex_magnis(det, magnus_potentia(a.denominator,
        a.corpus->gradus, piscina), piscina, exitus);
}

b32
algebraicus_vestigium (
    Algebraicus  a,
        Piscina* piscina,
        Fractio* exitus)
{
     Matrix  m;
     Magnus  summa = magnus_ex_s64(ZEPHYRUM);
        i32  j;
    Piscina* officina;

    si (a.corpus == NIHIL)
    {
        redde FALSUM;
    }
    officina = _officina_aperire();
    si (officina == NIHIL)
    {
        redde FALSUM;
    }
    si (!_matrix_multiplicationis(a.corpus, a.numerator, officina, &m))
    {
        piscina_destruere(officina);
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < a.corpus->gradus; j++)
    {
        summa = magnus_adde(summa,
            *(constans Magnus*)matrix_elementum(m,
            j, j), officina);
    }
    summa = magnus_transcribe(summa, piscina);
    piscina_destruere(officina);
    redde fractio_ex_magnis(summa, a.denominator, piscina, exitus);
}


/* ==================================================
 * Textus
 * ================================================== */

chorda
algebraicus_ad_chordam (
    Algebraicus  a,
        Piscina* piscina)
{
    chorda numerator;
    chorda exitus;
       b32 terminus_unus;

    si (a.corpus == NIHIL)
    {
        redde chorda_transcribere(chorda_ex_literis("invalidum",
            piscina),
            piscina);
    }
    numerator = polynomium_ad_chordam(a.numerator, 'a', piscina);
    si (magnus_compara(a.denominator, magnus_ex_s64(I)) == ZEPHYRUM)
    {
        redde numerator;
    }
    terminus_unus = polynomium_gradus_imus(a.numerator)
        == polynomium_gradus_summus(a.numerator);
    si (terminus_unus)
    {
        exitus = numerator;
    }
    alioquin
    {
        exitus =
            chorda_concatenare(chorda_concatenare(chorda_ex_literis(
            "(", piscina), numerator, piscina), chorda_ex_literis(")",
            piscina), piscina);
    }
    exitus = chorda_concatenare(exitus, chorda_ex_literis("/", piscina),
        piscina);
    redde chorda_concatenare(exitus, magnus_ad_chordam(a.denominator,
        piscina), piscina);
}

b32
algebraicus_ex_chorda (
     constans Extensio* k,
                chorda  textus,
               Piscina* piscina,
           Algebraicus* exitus)
{
         s32 vinculum;
      chorda pars;
      Magnus denominator    = magnus_ex_s64(I);
  Polynomium p              = polynomium_nullum();
         b32 unus_terminus  = FALSUM;

    si (k == NIHIL)
    {
        redde FALSUM;
    }
    textus    = chorda_praecidere(textus);
    vinculum  = chorda_invenire_ultimum_index(textus, chorda_ex_literis(
        "/", piscina));
    pars      = textus;
    si (vinculum >= ZEPHYRUM)
    {
        si (   !magnus_ex_chorda(chorda_praecidere(chorda_sectio(textus,
            (i32)vinculum + I, textus.mensura)), piscina, &denominator)
            || magnus_signum(denominator) == ZEPHYRUM)
        {
            redde FALSUM;
        }
        pars = chorda_praecidere(chorda_sectio(textus, ZEPHYRUM,
            (i32)vinculum));
        si (   pars.mensura >= II && pars.datum[ZEPHYRUM] == '('
            && pars.datum[pars.mensura - I] == ')')
        {
            pars = chorda_sectio(pars, I, pars.mensura - I);
        }
        alioquin
        {
            /* "P/D" sine parenthesibus: P terminus unus. "a + 1/2" non
             * (a + 1)/2 legitur sed refutatur (recensio E1, F4) */
            unus_terminus = VERUM;
        }
    }
    si (!polynomium_ex_chorda(pars, 'a', piscina, &p))
    {
        redde FALSUM;
    }
    si (   unus_terminus && !polynomium_est_nullum(p)
        && polynomium_gradus_imus(p) != polynomium_gradus_summus(p))
    {
        redde FALSUM;
    }
    redde algebraicus_ex_polynomio(k, p, denominator, piscina, exitus);
}


/* ==================================================
 * Ordo
 * ================================================== */

b32
algebraicus_signum (
    Algebraicus  a,
        Piscina* piscina,
            s32* exitus)
{
     constans Extensio* k = a.corpus;
               Fractio  infra;
               Fractio  supra;
               Fractio  limes_nullius;
               Fractio  derivata_maxima;
                   s32  signum_supra;
                   s32  limes_passuum;
                   s32  iteratio;
                   s32  proba_proxima;
                Magnus  numerus_infra;
                Magnus  numerus_supra;
                Magnus  quantum;
               Piscina* status;
               Piscina* opus;
        PiscinaNotatio  nota_status;
        PiscinaNotatio  nota_opus;
                   b32  exitus_bene = FALSUM;

    si (k == NIHIL || k->radix < ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* rationalis (et omne elementum corporis gradus 1): exactum */
    si (   polynomium_est_nullum(a.numerator)
        || polynomium_gradus_summus(a.numerator) == ZEPHYRUM)
    {
        *exitus = polynomium_est_nullum(a.numerator) ? ZEPHYRUM
            : magnus_signum(polynomium_coefficiens(a.numerator,
            ZEPHYRUM));
        redde VERUM;
    }
    /* gradus 2: forma clausa EXACTA. f = t^2 + b t + c, D = b^2 - 4c,
     * alpha = (-b + sigma sqrt D)/2 (sigma +1 radice maiore, index 1;
     * -1 minore, index 0); 2 num(alpha) = X + Y sqrt D, X = 2p - q b, Y
     * = sigma q. Signum per signa X, Y aut X^2 contra Y^2 D. Instans:
     * (1 - sqrt 2)^3000 per bisectiones 12 s. */
    si (k->gradus == II)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                Magnus fb    = polynomium_coefficiens(k->f, I);
                Magnus fc    = polynomium_coefficiens(k->f, ZEPHYRUM);
                Magnus p = polynomium_coefficiens(a.numerator,
                    ZEPHYRUM);
                Magnus q = polynomium_coefficiens(a.numerator, I);
                Magnus x;
                Magnus y;
                Magnus discriminans;
                   s32 sx;
                   s32 sy;
                   s32 s;

        discriminans = magnus_subtrahe(magnus_multiplica(fb, fb,
            piscina),
            magnus_multiplica(magnus_ex_s64(IV), fc, piscina), piscina);
        x = magnus_subtrahe(magnus_multiplica(magnus_ex_s64(II), p,
            piscina), magnus_multiplica(q, fb, piscina), piscina);
        y   = k->radix == I ? q : magnus_nega(q, piscina);
        sx  = magnus_signum(x);
        sy  = magnus_signum(y);
        si (sx >= ZEPHYRUM && sy >= ZEPHYRUM)
        {
            s = (sx > ZEPHYRUM || sy > ZEPHYRUM) ? I : ZEPHYRUM;
        }
        alioquin si (sx <= ZEPHYRUM && sy <= ZEPHYRUM)
        {
            s = -I;
        }
        alioquin
        {
            /* signa diversa: |X| contra |Y| sqrt D */
            s = magnus_compara(magnus_multiplica(x, x, piscina),
                magnus_multiplica(magnus_multiplica(y, y, piscina),
                discriminans, piscina)) * sx;
        }
        piscina_reficere(piscina, nota);
        si (s == ZEPHYRUM)
        {
            /* X^2 = Y^2 D: D quadratum, f reducibilis */
            redde FALSUM;
        }
        *exitus = s;
        redde VERUM;
    }
    /* FORMA CENTRATA (recensio II H1): |num(alpha) - num(m)| <= D w/2,
     * m medium, w latitudo, D >= sup |num'| super intervallum initiale
     * (et ergo omne sub-intervallum). |num(m)| > D w/2 signum decernit;
     * aliter |num(alpha)| <= D w, et D w < 1/M^(d-1) num(alpha) = 0
     * PROBAT (testimonium nullius: N(num(alpha)) integer, conjugatae <=
     * M = sum |c_i| B^i). Ergo passus log2(w0 D M^(d-1)) + O(1)
     * sufficiunt: limes COMPUTATUS, non MM fixum (quod
     * (1 - sqrt 2)^1000 validum refutabat). Status (infra, supra) in
     * officina una, opus in altera; nihil in piscina vocantis manet. */
    status  = _officina_aperire();
    opus    = _officina_aperire();
    si (status == NIHIL || opus == NIHIL)
    {
        si (status != NIHIL)
        {
            piscina_destruere(status);
        }
        si (opus != NIHIL)
        {
            piscina_destruere(opus);
        }
        redde FALSUM;
    }
    (vacuum)piscina;
    infra = fractio_transcribe(k->infra, status);
    supra = fractio_transcribe(k->supra, status);
    {
         Magnus limes = fractio_numerator(_limes_cauchy(k->f, status));
         Magnus summa = magnus_ex_s64(ZEPHYRUM);
         Magnus potentia = magnus_ex_s64(I);
            s32 e;

        per (e = ZEPHYRUM; e <= polynomium_gradus_summus(a.numerator);
            e++)
        {
            summa = magnus_adde(summa,
                magnus_multiplica(magnus_absolutum(
                polynomium_coefficiens(a.numerator, e), status),
                potentia, status), status);
            potentia = magnus_multiplica(potentia, limes, status);
        }
        (vacuum)fractio_ex_magnis(magnus_ex_s64(I),
            magnus_potentia(summa,
            k->gradus - I, status), status, &limes_nullius);
        derivata_maxima = _derivata_limes(a.numerator, infra, supra,
            status);
    }
    limes_passuum = _bita_fractionis(fractio_subtrahe(supra, infra,
        status)) + _bita_fractionis(derivata_maxima)
        - _bita_fractionis(limes_nullius) + VIII;
    si (limes_passuum < VIII)
    {
        limes_passuum = VIII;
    }
    /* intervallum DYADICUM: infra = A / Q, supra = C / Q, Q = 2^S.
     * Bisectio = additio et duplicatio, sine gcd (recensio III M2:
     * medium per fractiones gcd O(n^2) in omni passu). */
    si (!_intervallum_binarium(infra, supra, status, &numerus_infra,
        &numerus_supra,
        &quantum))
    {
        /* invarians violata (inattingibile): refutatio */
        piscina_destruere(status);
        piscina_destruere(opus);
        redde FALSUM;
    }
    signum_supra  = _signum_numeri(k->f, numerus_supra, quantum, opus);
    nota_status   = piscina_notare(status);
    nota_opus     = piscina_notare(opus);
    /* bisectio per signum f (Horner integer, vile); num solum ad
     * PUNCTA GEOMETRICA (passus 0, 1, 2, 4, 8, ... et limes) aestimatur
     * (recensio III M2: num in omni passu per fractiones, (alpha -
     * c)^500 gradu 9 220 s). Passus summum duplicantur; aestimationes
     * num log2. Ad limitem probatio semper fit: ibi D w aut signum
     * decernit aut nullum PROBAT. */
    proba_proxima = ZEPHYRUM;
    per (iteratio = ZEPHYRUM; iteratio <= limes_passuum; iteratio++)
    {
        Magnus medium = magnus_adde(numerus_infra, numerus_supra, opus);
        Magnus quantum_novum = magnus_multiplica(quantum, magnus_ex_s64(
            II), opus);
        Magnus infra_nova;
        Magnus supra_nova;
           s32 s;

        si (iteratio == proba_proxima || iteratio == limes_passuum)
        {
            Fractio latitudo = fractio_ex_s64(ZEPHYRUM);
            Fractio valor = _valor_numeri(a.numerator, medium,
                quantum_novum, opus);
            Fractio error;

            (vacuum)fractio_ex_magnis(magnus_subtrahe(numerus_supra,
                numerus_infra, opus), quantum, opus, &latitudo);
            error = fractio_multiplica(derivata_maxima, latitudo, opus);
            (vacuum)fractio_divide(error, fractio_ex_s64(II), opus,
                &error);
            si (fractio_compara(fractio_absolutum(valor, opus), error,
                opus) > ZEPHYRUM)
            {
                *exitus      = fractio_signum(valor);
                exitus_bene  = VERUM;
                frange;
            }
            si (fractio_compara(fractio_multiplica(derivata_maxima,
                latitudo, opus), limes_nullius, opus) < ZEPHYRUM)
            {
                /* num(alpha) = 0 PROBATUM: f reducibilis */
                frange;
            }
            si (iteratio == limes_passuum)
            {
                frange;
            }
            /* proxima probatio PRAEDICTA: |num| ~ |num(m)|, error
             * dimidiatur quoque passu, ergo log2(error / |num(m)|) + 2
             * passus
             * (recensio IV P1: duplicatio usque ad 2x passuum). Tecta
             * duplicatione (numquam peior); num(m) = 0 -> duplicatio.
             * Correctio a limite pendet, non ab hac praedictione. */
            {
                s32 duplicatio = proba_proxima == ZEPHYRUM ? I
                    : II * proba_proxima;
                s32 conjectura = duplicatio;

                si (fractio_signum(valor) != ZEPHYRUM)
                {
                    conjectura = (s32)iteratio + _bita_fractionis(error)
                        - _bita_fractionis(valor) + II;
                }
                si (conjectura <= (s32)iteratio)
                {
                    conjectura = (s32)iteratio + I;
                }
                proba_proxima = conjectura < duplicatio ? conjectura
                    : duplicatio;
            }
        }
        s = _signum_numeri(k->f, medium, quantum_novum, opus);
        si (s == ZEPHYRUM)
        {
            /* radix rationalis: f reducibilis */
            frange;
        }
        /* (A, C) / Q -> (2A, A + C) aut (A + C, 2C) / 2Q */
        infra_nova = s
            == signum_supra ? magnus_multiplica(numerus_infra,
            magnus_ex_s64(II), opus) : medium;
        supra_nova = s == signum_supra ? medium : magnus_multiplica(
            numerus_supra, magnus_ex_s64(II), opus);
        piscina_reficere(status, nota_status);
        numerus_infra  = magnus_transcribe(infra_nova, status);
        numerus_supra  = magnus_transcribe(supra_nova, status);
        quantum        = magnus_transcribe(quantum_novum, status);
        piscina_reficere(opus, nota_opus);
    }
    piscina_destruere(status);
    piscina_destruere(opus);
    redde exitus_bene;
}

b32
algebraicus_compara (
    Algebraicus  a,
    Algebraicus  b,
        Piscina* piscina,
            s32* exitus)
{
    si (_commune(a, b) == NIHIL)
    {
        redde FALSUM;
    }
    redde algebraicus_signum(algebraicus_subtrahe(a, b, piscina),
        piscina,
        exitus);
}


/* ==================================================
 * Ostensio decimalis CERTA
 * ================================================== */

/* q rationalis cum |a - q| <= epsilon (a non rationalis, corpus
 * ordinatum gradus >= 2): intervallum radicis bisecatur (dyadice, sicut
 * signum) donec D w / 2 <= epsilon den; tum num(m) / den. Forma
 * centrata: |num(alpha) - num(m)| <= D w / 2. */
interior b32
_propinquum (
    Algebraicus  a,
        Fractio  epsilon,
        Piscina* piscina,
        Fractio* exitus)
{
     constans Extensio* k       = a.corpus;
               Piscina* status  = _officina_aperire();
               Piscina* opus    = _officina_aperire();
        PiscinaNotatio  nota_status;
        PiscinaNotatio  nota_opus;
               Fractio  derivata_maxima;
               Fractio  scopus;
                Magnus  numerus_infra;
                Magnus  numerus_supra;
                Magnus  quantum;
                   s32  signum_supra;
                   s32  limes;
                   s32  iteratio;
                   b32  bene = FALSUM;

    si (status == NIHIL || opus == NIHIL)
    {
        si (status != NIHIL)
        {
            piscina_destruere(status);
        }
        si (opus != NIHIL)
        {
            piscina_destruere(opus);
        }
        redde FALSUM;
    }
    derivata_maxima = _derivata_limes(a.numerator, k->infra, k->supra,
        status);
    scopus = fractio_multiplica(epsilon,
        fractio_ex_magno(a.denominator),
        status);
    si (!_intervallum_binarium(k->infra, k->supra, status,
        &numerus_infra,
        &numerus_supra, &quantum))
    {
        piscina_destruere(status);
        piscina_destruere(opus);
        redde FALSUM;
    }
    /* limes STRUCTURALIS: log2(w0 D / scopus) + margo */
    limes = _bita_fractionis(fractio_subtrahe(k->supra, k->infra,
        status))
        + _bita_fractionis(derivata_maxima) - _bita_fractionis(scopus)
        + VIII;
    si (limes < VIII)
    {
        limes = VIII;
    }
    signum_supra  = _signum_numeri(k->f, numerus_supra, quantum, opus);
    nota_status   = piscina_notare(status);
    nota_opus     = piscina_notare(opus);
    per (iteratio = ZEPHYRUM; iteratio <= limes; iteratio++)
    {
        Magnus medium = magnus_adde(numerus_infra, numerus_supra, opus);
        Magnus quantum_novum = magnus_multiplica(quantum, magnus_ex_s64(
            II), opus);
        Fractio latitudo = fractio_ex_s64(ZEPHYRUM);
        Fractio error;
         Magnus infra_nova;
         Magnus supra_nova;
            s32 s;

        (vacuum)fractio_ex_magnis(magnus_subtrahe(numerus_supra,
            numerus_infra, opus), quantum, opus, &latitudo);
        error = fractio_multiplica(derivata_maxima, latitudo, opus);
        (vacuum)fractio_divide(error, fractio_ex_s64(II), opus, &error);
        si (fractio_compara(error, scopus, opus) <= ZEPHYRUM)
        {
            Fractio valor = _valor_numeri(a.numerator, medium,
                quantum_novum, opus);

            (vacuum)fractio_divide(valor, fractio_ex_magno(
                a.denominator), opus, &valor);
            *exitus  = fractio_transcribe(valor, piscina);
            bene     = VERUM;
            frange;
        }
        s = _signum_numeri(k->f, medium, quantum_novum, opus);
        si (s == ZEPHYRUM)
        {
            frange;
        }
        infra_nova = s
            == signum_supra ? magnus_multiplica(numerus_infra,
            magnus_ex_s64(II), opus) : medium;
        supra_nova = s == signum_supra ? medium : magnus_multiplica(
            numerus_supra, magnus_ex_s64(II), opus);
        piscina_reficere(status, nota_status);
        numerus_infra  = magnus_transcribe(infra_nova, status);
        numerus_supra  = magnus_transcribe(supra_nova, status);
        quantum        = magnus_transcribe(quantum_novum, status);
        piscina_reficere(opus, nota_opus);
    }
    piscina_destruere(status);
    piscina_destruere(opus);
    redde bene;
}

/* R / 10^k ut textus: "-1.4142"; R = 0 sine signo */
interior chorda
_textus_decimalis (
     Magnus  r,
        i32  digiti,
    Piscina* piscina)
{
       chorda textus = magnus_ad_chordam(magnus_absolutum(r, piscina),
           piscina);
          i32 longitudo = (i32)textus.mensura;
          i32 zephyra = longitudo <= digiti ? digiti + I - longitudo
              : ZEPHYRUM;
          i32  totum    = longitudo + zephyra;
          i32  positus  = ZEPHYRUM;
          i32  j;
    character* alveus;

    alveus = (character*)piscina_allocare(piscina,
        (memoriae_index)(totum
        + III));
    si (magnus_signum(r) < ZEPHYRUM)
    {
        alveus[positus++] = '-';
    }
    per (j = ZEPHYRUM; j < totum; j++)
    {
        si (digiti > ZEPHYRUM && j == totum - digiti)
        {
            alveus[positus++] = '.';
        }
        alveus[positus++] = j < zephyra ? '0'
            : (character)textus.datum[j - zephyra];
    }
    redde chorda_ex_buffer((i8*)alveus, positus);
}

chorda
algebraicus_ad_ostendendum (
    Algebraicus  a,
            i32  digiti,
        Piscina* piscina)
{
      constans Extensio* k = a.corpus;
                Piscina* officina;
                Fractio  q           = fractio_ex_s64(ZEPHYRUM);
                Fractio  propinquum  = fractio_ex_s64(ZEPHYRUM);
                Fractio  epsilon     = fractio_ex_s64(ZEPHYRUM);
                 Magnus  decem;
                 Magnus  r;
                 chorda  exitus;
                    i32  passus;

    si (k == NIHIL)
    {
        redde chorda_transcribere(chorda_ex_literis("invalidum",
            piscina), piscina);
    }
    officina = _officina_aperire();
    si (officina == NIHIL)
    {
        redde algebraicus_ad_chordam(a, piscina);
    }
    decem = magnus_potentia(magnus_ex_s64(X), digiti, officina);
    /* rationalis: rotundatio exacta (paritas in medio, sicut
     * fractio_rotunda) */
    si (algebraicus_est_rationalis(a, officina, &q))
    {
        r = fractio_rotunda(fractio_multiplica(q,
            fractio_ex_magno(decem),
            officina), officina);
        exitus = _textus_decimalis(r, digiti, piscina);
        piscina_destruere(officina);
        redde exitus;
    }
    /* sine ordine (e.g. Q(i)): nulla immersio realis electa - textus
     * exactus */
    si (k->radix < ZEPHYRUM || k->gradus < II)
    {
        piscina_destruere(officina);
        redde algebraicus_ad_chordam(a, piscina);
    }
    /* propinquum cum errore < 1/(4 10^k), deinde R = round(m 10^k)
     * CERTIFICATUR: signum(a 10^k - (R + 1/2)) < 0 < signum(a 10^k - (R
     * - 1/2)), exacte (testimonium nullius: a 10^k non rationalis
     * numquam dimidium integri). Correctio +-1 rara; passus IV
     * limes. */
    (vacuum)fractio_ex_magnis(magnus_ex_s64(I), magnus_multiplica(
        magnus_ex_s64(IV), decem, officina), officina, &epsilon);
    si (!_propinquum(a, epsilon, officina, &propinquum))
    {
        piscina_destruere(officina);
        redde algebraicus_ad_chordam(a, piscina);
    }
    r = fractio_rotunda(fractio_multiplica(propinquum,
        fractio_ex_magno(
        decem), officina), officina);
    per (passus = ZEPHYRUM; passus < IV; passus++)
    {
        Algebraicus scalatum = algebraicus_multiplica(a,
            algebraicus_ex_fractione(k, fractio_ex_magno(decem),
            officina), officina);
        Fractio dimidium      = fractio_ex_s64(ZEPHYRUM);
            s32 signum_supra  = ZEPHYRUM;
            s32 signum_infra  = ZEPHYRUM;

        (vacuum)fractio_ex_magnis(magnus_adde(magnus_multiplica(
            magnus_ex_s64(II), r, officina), magnus_ex_s64(I),
            officina),
            magnus_ex_s64(II), officina, &dimidium);
        si (!algebraicus_signum(algebraicus_subtrahe(scalatum,
            algebraicus_ex_fractione(k, dimidium, officina), officina),
            officina, &signum_supra))
        {
            frange;
        }
        (vacuum)fractio_ex_magnis(magnus_subtrahe(magnus_multiplica(
            magnus_ex_s64(II), r, officina), magnus_ex_s64(I),
            officina),
            magnus_ex_s64(II), officina, &dimidium);
        si (!algebraicus_signum(algebraicus_subtrahe(scalatum,
            algebraicus_ex_fractione(k, dimidium, officina), officina),
            officina, &signum_infra))
        {
            frange;
        }
        si (signum_infra > ZEPHYRUM && signum_supra < ZEPHYRUM)
        {
            exitus = _textus_decimalis(r, digiti, piscina);
            piscina_destruere(officina);
            redde exitus;
        }
        r = magnus_adde(r, magnus_ex_s64(signum_supra >= ZEPHYRUM ? I
            : -I), officina);
    }
    piscina_destruere(officina);
    redde algebraicus_ad_chordam(a, piscina);
}
