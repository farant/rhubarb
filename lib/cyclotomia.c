/* cyclotomia.c - Integri cyclotomici exacti (vide include/cyclotomia.h)
 */
#include "cyclotomia.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

structura Cyclotomia {
             i32  n;
             i32  phi;
      Polynomium  phi_n;          /* Phi_n, monicum, gradus phi */
      Polynomium* potentiae;      /* zeta^k reducta, k = 0..n-1 */
      /* -zeta^k: est_radix sine piscina */
      Polynomium* potentiae_negatae;
             i32* unitates;       /* j in [1, n], gcd(j, n) = 1 */
             i32  numerus_unitatum;
          Anulus  anulus;
};

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

/* t^d - 1 */
interior b32
_t_d_minus_unum (
           i32  d,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium monomium = polynomium_nullum();

    si (!polynomium_monomium(magnus_ex_s64(I), (s32)d, piscina,
        &monomium))
    {
        redde FALSUM;
    }
    *exitus = polynomium_subtrahe(monomium, polynomium_constans(
        magnus_ex_s64(I), piscina), piscina);
    redde VERUM;
}

b32
polynomium_cyclotomicum (
           i32  n,
       Piscina* piscina,
    Polynomium* exitus)
{
           i32 divisores[CC];
    Polynomium phi[CC];
           i32 numerus = ZEPHYRUM;
           i32 a;
           i32 b;

    si (n == ZEPHYRUM || n > CYCLOTOMIA_ORDO_MAXIMUS)
    {
        redde FALSUM;
    }
    per (a = I; a <= n; a++)
    {
        si (n % a == ZEPHYRUM)
        {
            divisores[numerus++] = a;
        }
    }
    /* ordine crescente: Phi_d = (t^d - 1) / prod_{e | d, e < d}
     * Phi_e */
    per (a = ZEPHYRUM; a < numerus; a++)
    {
        Polynomium q = polynomium_nullum();

        si (!_t_d_minus_unum(divisores[a], piscina, &q))
        {
            redde FALSUM;
        }
        per (b = ZEPHYRUM; b < a; b++)
        {
            si (   divisores[a] % divisores[b] == ZEPHYRUM
                && !polynomium_divide_exacte(q, phi[b], piscina, &q))
            {
                redde FALSUM;
            }
        }
        phi[a] = q;
    }
    *exitus = phi[numerus - I];
    redde VERUM;
}

/* summa c * potentiae[m] in alveum (phi coefficientes) */
interior vacuum
_addere_potentiam (
    constans Cyclotomia* r,
                 Magnus* alveus,
                 Magnus  c,
                    i32  m,
                Piscina* piscina)
{
    Polynomium pm = r->potentiae[m];
           s32 j;

    per (j = polynomium_gradus_imus(pm); !polynomium_est_nullum(pm)
        && j <= polynomium_gradus_summus(pm); j++)
    {
        Magnus d = polynomium_coefficiens(pm, j);

        si (magnus_signum(d) != ZEPHYRUM)
        {
            alveus[j] = magnus_adde(alveus[j], magnus_multiplica(c, d,
                piscina), piscina);
        }
    }
}

/* p(zeta^k) reductum: terminus c t^e -> c * zeta^((k e) mod n) */
interior Cyclotomicus
_reducere (
    constans Cyclotomia* r,
             Polynomium  p,
                    s32  k,
                Piscina* piscina)
{
    Cyclotomicus  exitus;
          Magnus* alveus;
             s32  e;
             i32  j;

    exitus.p = polynomium_nullum();
    si (polynomium_est_nullum(p))
    {
        redde exitus;
    }
    alveus = (Magnus*)piscina_allocare(piscina, (memoriae_index)r->phi
        * magnitudo(Magnus));
    per (j = ZEPHYRUM; j < r->phi; j++)
    {
        alveus[j] = magnus_ex_s64(ZEPHYRUM);
    }
    per (e = polynomium_gradus_imus(p); e
        <= polynomium_gradus_summus(p);
        e++)
    {
        Magnus c = polynomium_coefficiens(p, e);
           s64 m;

        si (magnus_signum(c) == ZEPHYRUM)
        {
            perge;
        }
        m = ((s64)k * (s64)e) % (s64)r->n;
        si (m < ZEPHYRUM)
        {
            m = m + (s64)r->n;
        }
        _addere_potentiam(r, alveus, c, (i32)m, piscina);
    }
    (vacuum)polynomium_ex_coefficientibus(alveus, r->phi, ZEPHYRUM,
        piscina,
        &exitus.p);
    redde exitus;
}


/* ==================================================
 * Anulus Z[zeta_n]: elementa Cyclotomicus
 * ================================================== */

#define CYCLO(anulus) ((constans Cyclotomia*)(anulus)->contextus)

interior vacuum
_an_nullum (
    constans Anulus* anulus,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Cyclotomicus*)exitus = cyclotomicus_nullum();
}

interior vacuum
_an_unum (
    constans Anulus* anulus,
           Piscina* piscina,
            vacuum* exitus)
{
    *(Cyclotomicus*)exitus = cyclotomicus_integer(CYCLO(anulus),
        magnus_ex_s64(I), piscina);
}

interior b32
_an_est_nullum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    (vacuum)anulus;
    redde cyclotomicus_est_nullum(*(constans Cyclotomicus*)a);
}

interior b32
_an_parvum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    (vacuum)anulus;
    redde cyclotomicus_est_nullum(*(constans Cyclotomicus*)a);
}

interior b32
_an_aequalis (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b)
{
    (vacuum)anulus;
    redde cyclotomicus_aequalis(*(constans Cyclotomicus*)a,
        *(constans Cyclotomicus*)b);
}

interior b32
_an_adde (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Cyclotomicus*)exitus =
        cyclotomicus_adde(*(constans Cyclotomicus*)a,
        *(constans Cyclotomicus*)b, piscina);
    redde VERUM;
}

interior b32
_an_subtrahe (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Cyclotomicus*)exitus = cyclotomicus_subtrahe(
        *(constans Cyclotomicus*)a, *(constans Cyclotomicus*)b,
        piscina);
    redde VERUM;
}

interior b32
_an_multiplica (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
           Piscina* piscina,
            vacuum* exitus)
{
    *(Cyclotomicus*)exitus = cyclotomicus_multiplica(CYCLO(anulus),
        *(constans Cyclotomicus*)a, *(constans Cyclotomicus*)b,
        piscina);
    redde VERUM;
}

interior b32
_an_divide_exacte (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
           Piscina* piscina,
            vacuum* exitus)
{
    redde cyclotomicus_divide_exacte(CYCLO(anulus),
        *(constans Cyclotomicus*)a, *(constans Cyclotomicus*)b, piscina,
        (Cyclotomicus*)exitus);
}

interior vacuum
_an_transcribe (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    ((Cyclotomicus*)exitus)->p = polynomium_transcribe(
        ((constans Cyclotomicus*)a)->p, piscina);
}

interior chorda
_an_ad_chordam (
    constans Anulus* anulus,
    constans vacuum* a,
           Piscina* piscina)
{
    redde cyclotomicus_ad_chordam(CYCLO(anulus),
        *(constans Cyclotomicus*)a, piscina);
}

interior b32
_an_ex_chorda (
    constans Anulus* anulus,
             chorda  textus,
           Piscina*  piscina,
            vacuum*  exitus)
{
    redde cyclotomicus_ex_chorda(CYCLO(anulus), textus, piscina,
        (Cyclotomicus*)exitus);
}


/* ==================================================
 * Contextus
 * ================================================== */

Cyclotomia*
cyclotomia_creare (
         i32  n,
     Piscina* piscina)
{
     Cyclotomia* r;
         Magnus* alveus;
      character* titulus;
            i32  k;
            i32  j;

    si (n == ZEPHYRUM || n > CYCLOTOMIA_ORDO_MAXIMUS)
    {
        redde NIHIL;
    }
    r = (Cyclotomia*)piscina_allocare(piscina, magnitudo(Cyclotomia));
    r->n = n;
    si (!polynomium_cyclotomicum(n, piscina, &r->phi_n))
    {
        redde NIHIL;
    }
    r->phi = (i32)polynomium_gradus_summus(r->phi_n);
    /* tabula potentiarum: zeta^k = t^k pro k < phi; deinde
     * t * zeta^(k-1) reductum per Phi_n monicum */
    r->potentiae = (Polynomium*)piscina_allocare(piscina,
        (memoriae_index)n
        * magnitudo(Polynomium));
    alveus = (Magnus*)piscina_allocare(piscina, (memoriae_index)(r->phi
        + I) * magnitudo(Magnus));
    per (j = ZEPHYRUM; j <= r->phi; j++)
    {
        alveus[j] = magnus_ex_s64(ZEPHYRUM);
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (k == ZEPHYRUM)
        {
            alveus[ZEPHYRUM] = magnus_ex_s64(I);
        }
        alioquin
        {
            Magnus summus;

            /* translatio per t */
            per (j = r->phi; j > ZEPHYRUM; j--)
            {
                alveus[j] = alveus[j - I];
            }
            alveus[ZEPHYRUM]  = magnus_ex_s64(ZEPHYRUM);
            summus            = alveus[r->phi];
            si (magnus_signum(summus) != ZEPHYRUM)
            {
                per (j = ZEPHYRUM; j < r->phi; j++)
                {
                    alveus[j] = magnus_subtrahe(alveus[j],
                        magnus_multiplica(summus,
                        polynomium_coefficiens(
                        r->phi_n, (s32)j), piscina), piscina);
                }
                alveus[r->phi] = magnus_ex_s64(ZEPHYRUM);
            }
        }
        (vacuum)polynomium_ex_coefficientibus(alveus, r->phi, ZEPHYRUM,
            piscina, &r->potentiae[k]);
    }
    r->potentiae_negatae = (Polynomium*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(Polynomium));
    per (k = ZEPHYRUM; k < n; k++)
    {
        r->potentiae_negatae[k] = polynomium_nega(r->potentiae[k],
            piscina);
    }
    r->unitates = (i32*)piscina_allocare(piscina, (memoriae_index)(n
        + I)
        * magnitudo(i32));
    r->numerus_unitatum = ZEPHYRUM;
    per (j = I; j <= n; j++)
    {
        si (_divisor_communis(j, n) == I)
        {
            r->unitates[r->numerus_unitatum++] = j;
        }
    }
    titulus = (character*)piscina_allocare(piscina,
        (memoriae_index)XXXII);
    sprintf(titulus, "Z[zeta_%u]", n);
    r->anulus.titulus             = titulus;
    r->anulus.mensura             = magnitudo(Cyclotomicus);
    r->anulus.corpus              = FALSUM;
    r->anulus.nullum              = _an_nullum;
    r->anulus.unum                = _an_unum;
    r->anulus.est_nullum          = _an_est_nullum;
    r->anulus.parvum              = _an_parvum;
    r->anulus.aequalis            = _an_aequalis;
    r->anulus.adde                = _an_adde;
    r->anulus.subtrahe            = _an_subtrahe;
    r->anulus.multiplica          = _an_multiplica;
    r->anulus.divide_exacte       = _an_divide_exacte;
    r->anulus.transcribe          = _an_transcribe;
    r->anulus.ad_chordam          = _an_ad_chordam;
    r->anulus.ex_chorda           = _an_ex_chorda;
    r->anulus.divisor_communis    = NIHIL;
    r->anulus.divide_cum_residuo  = NIHIL;
    r->anulus.compara_normam      = NIHIL;
    r->anulus.contextus           = r;
    r->anulus.integrum            = VERUM;
    redde r;
}

i32
cyclotomia_ordo (
    constans Cyclotomia* r)
{
    redde r->n;
}

i32
cyclotomia_gradus (
    constans Cyclotomia* r)
{
    redde r->phi;
}

Polynomium
cyclotomia_polynomium (
    constans Cyclotomia* r)
{
    redde r->phi_n;
}

constans Anulus*
cyclotomia_anulus (
    constans Cyclotomia* r)
{
    redde &r->anulus;
}


/* ==================================================
 * Constructio
 * ================================================== */

Cyclotomicus
cyclotomicus_nullum (vacuum)
{
    Cyclotomicus a;

    a.p = polynomium_nullum();
    redde a;
}

Cyclotomicus
cyclotomicus_integer (
    constans Cyclotomia* r,
                 Magnus  c,
                Piscina* piscina)
{
    Cyclotomicus a;

    (vacuum)r;
    a.p = polynomium_constans(c, piscina);
    redde a;
}

Cyclotomicus
cyclotomicus_radix (
    constans Cyclotomia* r,
                    s32  k,
                Piscina* piscina)
{
    s64 m = (s64)k % (s64)r->n;

    (vacuum)piscina;
    si (m < ZEPHYRUM)
    {
        m = m + (s64)r->n;
    }
    {
        Cyclotomicus a;

        a.p = r->potentiae[(i32)m];
        redde a;
    }
}

b32
cyclotomicus_ex_polynomio (
    constans Cyclotomia* r,
             Polynomium  p,
                    s32  k,
                Piscina* piscina,
           Cyclotomicus* exitus)
{
    *exitus = _reducere(r, p, k, piscina);
    redde VERUM;
}

Magnus
cyclotomicus_coefficiens (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                    i32  j)
{
    (vacuum)r;
    redde polynomium_coefficiens(a.p, (s32)j);
}


/* ==================================================
 * Arithmetica
 * ================================================== */

b32
cyclotomicus_est_nullum (
    Cyclotomicus a)
{
    redde polynomium_est_nullum(a.p);
}

b32
cyclotomicus_aequalis (
    Cyclotomicus a,
    Cyclotomicus b)
{
    redde polynomium_aequalis(a.p, b.p);
}

b32
cyclotomicus_est_integer (
    Cyclotomicus  a,
          Magnus* valor)
{
    si (polynomium_est_nullum(a.p))
    {
        si (valor != NIHIL)
        {
            *valor = magnus_ex_s64(ZEPHYRUM);
        }
        redde VERUM;
    }
    si (polynomium_gradus_summus(a.p) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (valor != NIHIL)
    {
        *valor = polynomium_coefficiens(a.p, ZEPHYRUM);
    }
    redde VERUM;
}

Cyclotomicus
cyclotomicus_adde (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina)
{
    Cyclotomicus c;

    c.p = polynomium_adde(a.p, b.p, piscina);
    redde c;
}

Cyclotomicus
cyclotomicus_subtrahe (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina)
{
    Cyclotomicus c;

    c.p = polynomium_subtrahe(a.p, b.p, piscina);
    redde c;
}

Cyclotomicus
cyclotomicus_nega (
    Cyclotomicus  a,
         Piscina* piscina)
{
    Cyclotomicus c;

    c.p = polynomium_nega(a.p, piscina);
    redde c;
}

Cyclotomicus
cyclotomicus_multiplica (
    constans Cyclotomia* r,
           Cyclotomicus  a,
           Cyclotomicus  b,
                Piscina* piscina)
{
    Polynomium productum = polynomium_nullum();

    /* gradus < 2 phi: exponentes semper intra fines */
    (vacuum)polynomium_multiplica(a.p, b.p, piscina, &productum);
    redde _reducere(r, productum, I, piscina);
}

Cyclotomicus
cyclotomicus_potentia (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                    i32  e,
                Piscina* piscina)
{
    Cyclotomicus summa = cyclotomicus_integer(r, magnus_ex_s64(I),
        piscina);

    dum (e > ZEPHYRUM)
    {
        si (e & I)
        {
            summa = cyclotomicus_multiplica(r, summa, a, piscina);
        }
        e = e >> I;
        si (e > ZEPHYRUM)
        {
            a = cyclotomicus_multiplica(r, a, a, piscina);
        }
    }
    redde summa;
}

b32
cyclotomicus_automorphismus (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                    s32  j,
                Piscina* piscina,
           Cyclotomicus* exitus)
{
    s64 m = (s64)j % (s64)r->n;

    si (m < ZEPHYRUM)
    {
        m = m + (s64)r->n;
    }
    si (r->n > I && _divisor_communis((i32)m, r->n) != I)
    {
        redde FALSUM;
    }
    *exitus = _reducere(r, a.p, j, piscina);
    redde VERUM;
}

Cyclotomicus
cyclotomicus_conjugatum (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                Piscina* piscina)
{
    redde _reducere(r, a.p, -I, piscina);
}

/* prod_{j unitas, j != 1} sigma_j(a) */
interior Cyclotomicus
_conjugata_producta (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                Piscina* piscina)
{
    Cyclotomicus productum = cyclotomicus_integer(r, magnus_ex_s64(I),
        piscina);
             i32 u;

    per (u = ZEPHYRUM; u < r->numerus_unitatum; u++)
    {
        si (r->unitates[u] % r->n == I % r->n)
        {
            perge;
        }
        productum = cyclotomicus_multiplica(r, productum, _reducere(r,
            a.p,
            (s32)r->unitates[u], piscina), piscina);
    }
    redde productum;
}

Magnus
cyclotomicus_norma (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                Piscina* piscina)
{
    Cyclotomicus n = cyclotomicus_multiplica(r, a,
        _conjugata_producta(r,
        a, piscina), piscina);
          Magnus valor = magnus_ex_s64(ZEPHYRUM);

    (vacuum)cyclotomicus_est_integer(n, &valor);
    redde valor;
}

Magnus
cyclotomicus_vestigium (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                Piscina* piscina)
{
    Cyclotomicus summa = cyclotomicus_nullum();
          Magnus valor = magnus_ex_s64(ZEPHYRUM);
             i32 u;

    per (u = ZEPHYRUM; u < r->numerus_unitatum; u++)
    {
        summa = cyclotomicus_adde(summa, _reducere(r, a.p,
            (s32)r->unitates[u], piscina), piscina);
    }
    (vacuum)cyclotomicus_est_integer(summa, &valor);
    redde valor;
}

b32
cyclotomicus_divide_exacte (
    constans Cyclotomia* r,
           Cyclotomicus  a,
           Cyclotomicus  b,
                Piscina* piscina,
           Cyclotomicus* exitus)
{
    Cyclotomicus  c;
          Magnus  n;
          Magnus* alveus;
             s32  e;

    si (cyclotomicus_est_nullum(b))
    {
        redde FALSUM;
    }
    n = cyclotomicus_norma(r, b, piscina);
    c = cyclotomicus_multiplica(r, a, _conjugata_producta(r, b,
        piscina),
        piscina);
    si (cyclotomicus_est_nullum(c))
    {
        *exitus = c;
        redde VERUM;
    }
    alveus = (Magnus*)piscina_allocare(piscina, (memoriae_index)r->phi
        * magnitudo(Magnus));
    per (e = ZEPHYRUM; e < (s32)r->phi; e++)
    {
        Magnus q;
        Magnus residuum;

        alveus[e] = magnus_ex_s64(ZEPHYRUM);
        si (   e < polynomium_gradus_imus(c.p)
            || e > polynomium_gradus_summus(c.p))
        {
            perge;
        }
        si (   !magnus_divide(polynomium_coefficiens(c.p, e), n,
            piscina, &q,
                &residuum)
            || magnus_signum(residuum) != ZEPHYRUM)
        {
            redde FALSUM;
        }
        alveus[e] = q;
    }
    (vacuum)polynomium_ex_coefficientibus(alveus, r->phi, ZEPHYRUM,
        piscina,
        &exitus->p);
    redde VERUM;
}

Cyclotomicus
cyclotomicus_modulus_quadratus (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                Piscina* piscina)
{
    redde cyclotomicus_multiplica(r, a, cyclotomicus_conjugatum(r, a,
        piscina), piscina);
}

b32
cyclotomicus_est_radix (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                    s32* signum,
                    i32* k)
{
    i32 m;

    per (m = ZEPHYRUM; m < r->n; m++)
    {
        si (polynomium_aequalis(a.p, r->potentiae[m]))
        {
            *signum  = I;
            *k       = m;
            redde VERUM;
        }
    }
    per (m = ZEPHYRUM; m < r->n; m++)
    {
        si (polynomium_aequalis(a.p, r->potentiae_negatae[m]))
        {
            *signum  = -I;
            *k       = m;
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Textus
 * ================================================== */

chorda
cyclotomicus_ad_chordam (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                Piscina* piscina)
{
    (vacuum)r;
    redde polynomium_ad_chordam(a.p, 'z', piscina);
}

b32
cyclotomicus_ex_chorda (
    constans Cyclotomia* r,
                 chorda  textus,
                Piscina* piscina,
           Cyclotomicus* exitus)
{
    Polynomium p = polynomium_nullum();

    si (!polynomium_ex_chorda(textus, 'z', piscina, &p))
    {
        redde FALSUM;
    }
    *exitus = _reducere(r, p, I, piscina);
    redde VERUM;
}

/* magnus -> f64 (ad ostendendum solum) */
interior f64
_ad_f64 (
     Magnus  c,
    Piscina* piscina)
{
       s64 parvus = ZEPHYRUM;
    chorda textus;
 character alveus[CCLVI];
       i32 n;

    si (magnus_ad_s64(c, &parvus))
    {
        redde (f64)parvus;
    }
    textus  = magnus_ad_chordam(c, piscina);
    n       = textus.mensura < CCLV ? textus.mensura : CCLV;
    memcpy(alveus, textus.datum, (size_t)n);
    alveus[n] = '\0';
    redde strtod(alveus, NIHIL);
}

chorda
cyclotomicus_ad_ostendendum (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                    i32  digiti,
                Piscina* piscina)
{
          f64 re = 0.0;
          f64 im = 0.0;
          f64 limes;
          s32 e;
    character alveus[CXXVIII];

    si (!polynomium_est_nullum(a.p))
    {
        per (e = polynomium_gradus_imus(a.p);
            e <= polynomium_gradus_summus(a.p); e++)
        {
            f64 c = _ad_f64(polynomium_coefficiens(a.p, e), piscina);
            f64 angulus = 2.0 * 3.14159265358979323846 * (f64)e
                / (f64)r->n;

            re = re + c * cos(angulus);
            im = im + c * sin(angulus);
        }
    }
    si (digiti > XV)
    {
        digiti = XV;
    }
    limes = 0.5 * pow(10.0, -(f64)digiti);
    si (fabs(re) < limes)
    {
        re = 0.0;
    }
    si (fabs(im) < limes)
    {
        im = 0.0;
    }
    sprintf(alveus, "%.*f %c %.*fi", (integer)digiti, re, im < 0.0 ? '-'
        : '+', (integer)digiti, fabs(im));
    redde chorda_transcribere(chorda_ex_literis(alveus, piscina),
        piscina);
}
