/* extensio.c - Corpora numerorum algebraicorum exacta (vide
 * include/extensio.h)
 */
#include "extensio.h"
#include "cyclotomia.h"
#include "matrix.h"
#include <stdio.h>
#include <string.h>

structura Extensio {
           i32 gradus;           /* d */
    Polynomium f;                /* monicus, gradus d */
           s32 radix;            /* index radicis realis electae; -1 */
           i32 radices_reales;
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

/* f monicus, gradus >= 1 (a vocante probatum) */
interior Extensio*
_creare (
    Polynomium  f,
           s32  radix,
           i32  radices_reales,
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
           s64 absolutum = d < ZEPHYRUM ? -d : d;
           s64 p;
    Polynomium f = polynomium_nullum();
        Magnus c[III];

    si (d == ZEPHYRUM || d == I || absolutum >= (s64)0x80000000L)
    {
        redde NIHIL;
    }
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
    /* radices -sqrt d < +sqrt d: index 1 */
    redde _creare(f, d > ZEPHYRUM ? I : -I, d
        > ZEPHYRUM ? II : ZEPHYRUM,
        piscina);
}

Extensio*
extensio_cosinus (
         i32  n,
     Piscina* piscina)
{
    Polynomium  phi       = polynomium_nullum();
    Polynomium  reliquum  = polynomium_nullum();
    Polynomium  binomium  = polynomium_nullum();
    Polynomium  f         = polynomium_nullum();
        Magnus* c;
           i32  m;
           s32  j;

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
        redde _creare(f, ZEPHYRUM, I, piscina);
    }
    si (!polynomium_cyclotomicum(n, piscina, &phi))
    {
        redde NIHIL;
    }
    /* Phi_n palindromicum gradus 2m:
     * t^-m Phi_n = sum c_j (t + 1/t)^j */
    m = (i32)polynomium_gradus_summus(phi) / II;
    si (   !polynomium_translata(phi, -(s32)m, piscina, &reliquum)
        || !polynomium_ex_chorda(chorda_ex_literis("t + t^-1", piscina),
        't', piscina, &binomium))
    {
        redde NIHIL;
    }
    c = (Magnus*)piscina_allocare(piscina, (memoriae_index)(m + I)
        * magnitudo(Magnus));
    per (j = (s32)m; j >= ZEPHYRUM; j--)
    {
        Polynomium potentia = polynomium_nullum();

        c[j] = polynomium_coefficiens(reliquum, j);
        si (magnus_signum(c[j]) == ZEPHYRUM)
        {
            perge;
        }
        si (!polynomium_potentia(binomium, (i32)j, piscina, &potentia))
        {
            redde NIHIL;
        }
        reliquum = polynomium_subtrahe(reliquum,
            polynomium_multiplica_scalari(potentia, c[j], piscina),
            piscina);
    }
    si (!polynomium_est_nullum(reliquum))
    {
        redde NIHIL;
    }
    (vacuum)polynomium_ex_coefficientibus(c, m + I, ZEPHYRUM, piscina,
        &f);
    /* radices 2 cos(2 pi j/n), j unitas: omnes reales, alpha maxima */
    redde _creare(f, (s32)m - I, m, piscina);
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
        *exitus = _normalizare(k, _reducere(k, p, piscina), denominator,
            piscina);
        redde VERUM;
    }
    /* p = t^imus q, q exponentibus >= 0:
     * q(alpha) * (alpha^-1)^(-imus) */
    {
         Polynomium q = polynomium_nullum();
        Algebraicus inversa;
        Algebraicus potentia;

        si (   !k->invertibilis
            || !polynomium_translata(p, -imus, piscina, &q))
        {
            redde FALSUM;
        }
        inversa = _elementum(k, k->inversa_numerator,
            k->inversa_denominator);
        si (!algebraicus_potentia(inversa, -imus, piscina, &potentia))
        {
            redde FALSUM;
        }
        *exitus = algebraicus_multiplica(_normalizare(k, _reducere(k, q,
            piscina), denominator, piscina), potentia, piscina);
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
    /* Cramer: M x = e_0, M matrix multiplicationis numeratoris; x_i =
     * det(M_i) / det(M), M_i = M columna i per e_0 substituta */
    {
            Matrix  m;
            Magnus  det = magnus_ex_s64(ZEPHYRUM);
            Magnus* x;
            Magnus  unum    = magnus_ex_s64(I);
            Magnus  nullum  = magnus_ex_s64(ZEPHYRUM);
               i32  i;
               i32  j;
        Polynomium  numerator = polynomium_nullum();

        si (   !_matrix_multiplicationis(k, a.numerator, piscina, &m)
            || !matrix_determinans(m, piscina, &det))
        {
            redde FALSUM;
        }
        si (magnus_signum(det) == ZEPHYRUM)
        {
            redde FALSUM;
        }
        x = (Magnus*)piscina_allocare(piscina, (memoriae_index)k->gradus
            * magnitudo(Magnus));
        per (i = ZEPHYRUM; i < k->gradus; i++)
        {
            Matrix mi;

            si (!_matrix_multiplicationis(k, a.numerator, piscina, &mi))
            {
                redde FALSUM;
            }
            per (j = ZEPHYRUM; j < k->gradus; j++)
            {
                matrix_pone(&mi, j, i, j == ZEPHYRUM ? &unum : &nullum);
            }
            si (!matrix_determinans(mi, piscina, &x[i]))
            {
                redde FALSUM;
            }
            x[i] = magnus_multiplica(x[i], a.denominator, piscina);
        }
        (vacuum)polynomium_ex_coefficientibus(x, k->gradus, ZEPHYRUM,
            piscina, &numerator);
        *exitus = _normalizare(k, numerator, det, piscina);
        redde VERUM;
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
    Matrix m;
    Magnus det = magnus_ex_s64(ZEPHYRUM);

    si (a.corpus == NIHIL)
    {
        redde FALSUM;
    }
    si (polynomium_est_nullum(a.numerator))
    {
        *exitus = fractio_ex_s64(ZEPHYRUM);
        redde VERUM;
    }
    si (   !_matrix_multiplicationis(a.corpus, a.numerator, piscina, &m)
        || !matrix_determinans(m, piscina, &det))
    {
        redde FALSUM;
    }
    redde fractio_ex_magnis(det, magnus_potentia(a.denominator,
        a.corpus->gradus, piscina), piscina, exitus);
}

b32
algebraicus_vestigium (
    Algebraicus  a,
        Piscina* piscina,
        Fractio* exitus)
{
    Matrix m;
    Magnus summa = magnus_ex_s64(ZEPHYRUM);
       i32 j;

    si (a.corpus == NIHIL)
    {
        redde FALSUM;
    }
    si (!_matrix_multiplicationis(a.corpus, a.numerator, piscina, &m))
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < a.corpus->gradus; j++)
    {
        summa = magnus_adde(summa,
            *(constans Magnus*)matrix_elementum(m,
            j, j), piscina);
    }
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
      Magnus denominator  = magnus_ex_s64(I);
  Polynomium p            = polynomium_nullum();

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
    }
    si (!polynomium_ex_chorda(pars, 'a', piscina, &p))
    {
        redde FALSUM;
    }
    redde algebraicus_ex_polynomio(k, p, denominator, piscina, exitus);
}
