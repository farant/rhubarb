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

/* elementum signatum */
interior Cyclotomicus
_elementum (
    constans Cyclotomia* r,
             Polynomium  p)
{
    Cyclotomicus a;

    a.anulus  = r;
    a.p       = p;
    redde a;
}

interior Cyclotomicus
_invalidum (vacuum)
{
    redde _elementum(NIHIL, polynomium_nullum());
}

/* anulus communis duorum; NIHIL si mixti aut invalidi */
interior constans Cyclotomia*
_communis (
    Cyclotomicus a,
    Cyclotomicus b)
{
    redde a.anulus == b.anulus ? a.anulus : NIHIL;
}

/* p(zeta^k) reductum: terminus c t^e -> c * zeta^((k e) mod n) */
interior Cyclotomicus
_reducere (
    constans Cyclotomia* r,
             Polynomium  p,
                    s32  k,
                Piscina* piscina)
{
    Polynomium  exitus = polynomium_nullum();
        Magnus* alveus;
           s32  e;
           i32  j;

    si (polynomium_est_nullum(p))
    {
        redde _elementum(r, exitus);
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
        &exitus);
    redde _elementum(r, exitus);
}


/* ==================================================
 * Anulus Z[zeta_n]: elementa Cyclotomicus eiusdem contextus
 * ================================================== */

#define CYCLO(anulus) ((constans Cyclotomia*)(anulus)->contextus)
#define ELEMENTUM(x) (*(constans Cyclotomicus*)(x))

interior vacuum
_an_nullum (
    constans Anulus* anulus,
             vacuum* exitus)
{
    *(Cyclotomicus*)exitus = cyclotomicus_nullum(CYCLO(anulus));
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
    redde ((constans Cyclotomicus*)a)->anulus == CYCLO(anulus)
        && cyclotomicus_est_nullum(ELEMENTUM(a));
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
    redde ((constans Cyclotomicus*)a)->anulus == CYCLO(anulus)
        && cyclotomicus_aequalis(ELEMENTUM(a), ELEMENTUM(b));
}

/* exitus validus et huius anuli? */
interior b32
_an_bonum (
    constans Anulus* anulus,
        Cyclotomicus c,
             vacuum* exitus)
{
    si (c.anulus != CYCLO(anulus))
    {
        redde FALSUM;
    }
    *(Cyclotomicus*)exitus = c;
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
    redde _an_bonum(anulus, cyclotomicus_adde(ELEMENTUM(a),
        ELEMENTUM(b),
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
    redde _an_bonum(anulus, cyclotomicus_subtrahe(ELEMENTUM(a),
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
    redde _an_bonum(anulus, cyclotomicus_multiplica(ELEMENTUM(a),
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
    redde ((constans Cyclotomicus*)a)->anulus == CYCLO(anulus)
        && cyclotomicus_divide_exacte(ELEMENTUM(a), ELEMENTUM(b),
        piscina,
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
    *(Cyclotomicus*)exitus =
        _elementum(((constans Cyclotomicus*)a)->anulus,
        polynomium_transcribe(((constans Cyclotomicus*)a)->p, piscina));
}

interior chorda
_an_ad_chordam (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina)
{
    (vacuum)anulus;
    redde cyclotomicus_ad_chordam(ELEMENTUM(a), piscina);
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
    r->anulus.signum              = NIHIL;
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
cyclotomicus_nullum (
    constans Cyclotomia* r)
{
    redde _elementum(r, polynomium_nullum());
}

Cyclotomicus
cyclotomicus_integer (
    constans Cyclotomia* r,
                 Magnus  c,
                Piscina* piscina)
{
    redde _elementum(r, polynomium_constans(c, piscina));
}

Cyclotomicus
cyclotomicus_radix (
    constans Cyclotomia* r,
                    s32  k,
                Piscina* piscina)
{
    s64 m;

    (vacuum)piscina;
    si (r == NIHIL)
    {
        redde _invalidum();
    }
    m = (s64)k % (s64)r->n;
    si (m < ZEPHYRUM)
    {
        m = m + (s64)r->n;
    }
    redde _elementum(r, r->potentiae[(i32)m]);
}

b32
cyclotomicus_ex_polynomio (
    constans Cyclotomia* r,
             Polynomium  p,
                    s32  k,
                Piscina* piscina,
           Cyclotomicus* exitus)
{
    si (r == NIHIL)
    {
        redde FALSUM;
    }
    *exitus = _reducere(r, p, k, piscina);
    redde VERUM;
}

b32
cyclotomicus_ex_s64 (
    constans Cyclotomia* r,
           constans s64* c,
                    i32  numerus,
                Piscina* piscina,
           Cyclotomicus* exitus)
{
        Magnus* alveus;
           i32  k;
    Polynomium  p = polynomium_nullum();

    si (r == NIHIL)
    {
        redde FALSUM;
    }
    si (numerus == ZEPHYRUM)
    {
        *exitus = cyclotomicus_nullum(r);
        redde VERUM;
    }
    alveus = (Magnus*)piscina_allocare(piscina, (memoriae_index)numerus
        * magnitudo(Magnus));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        alveus[k] = magnus_ex_s64(c[k]);
    }
    si (!polynomium_ex_coefficientibus(alveus, numerus, ZEPHYRUM,
        piscina,
            &p))
    {
        redde FALSUM;
    }
    *exitus = _reducere(r, p, I, piscina);
    redde VERUM;
}


/* ==================================================
 * Lectio
 * ================================================== */

b32
cyclotomicus_est_validum (
    Cyclotomicus a)
{
    redde a.anulus != NIHIL;
}

constans Cyclotomia*
cyclotomicus_anulus (
    Cyclotomicus a)
{
    redde a.anulus;
}

Magnus
cyclotomicus_coefficiens (
    Cyclotomicus a,
             i32 j)
{
    si (a.anulus == NIHIL)
    {
        redde magnus_ex_s64(ZEPHYRUM);
    }
    redde polynomium_coefficiens(a.p, (s32)j);
}

b32
cyclotomicus_ad_s64 (
    Cyclotomicus  a,
             s64* exitus)
{
    i32 j;

    si (a.anulus == NIHIL)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < a.anulus->phi; j++)
    {
        exitus[j] = ZEPHYRUM;
        si (!magnus_ad_s64(polynomium_coefficiens(a.p, (s32)j),
                &exitus[j]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Arithmetica
 * ================================================== */

b32
cyclotomicus_est_nullum (
    Cyclotomicus a)
{
    redde a.anulus != NIHIL && polynomium_est_nullum(a.p);
}

b32
cyclotomicus_aequalis (
    Cyclotomicus a,
    Cyclotomicus b)
{
    redde _communis(a, b) != NIHIL && polynomium_aequalis(a.p, b.p);
}

b32
cyclotomicus_est_integer (
    Cyclotomicus  a,
          Magnus* valor)
{
    si (a.anulus == NIHIL)
    {
        redde FALSUM;
    }
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
    constans Cyclotomia* r = _communis(a, b);

    si (r == NIHIL)
    {
        redde _invalidum();
    }
    redde _elementum(r, polynomium_adde(a.p, b.p, piscina));
}

Cyclotomicus
cyclotomicus_subtrahe (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina)
{
    constans Cyclotomia* r = _communis(a, b);

    si (r == NIHIL)
    {
        redde _invalidum();
    }
    redde _elementum(r, polynomium_subtrahe(a.p, b.p, piscina));
}

Cyclotomicus
cyclotomicus_nega (
    Cyclotomicus  a,
         Piscina* piscina)
{
    si (a.anulus == NIHIL)
    {
        redde _invalidum();
    }
    redde _elementum(a.anulus, polynomium_nega(a.p, piscina));
}

Cyclotomicus
cyclotomicus_multiplica (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina)
{
     constans Cyclotomia* r          = _communis(a, b);
              Polynomium  productum  = polynomium_nullum();

    si (r == NIHIL)
    {
        redde _invalidum();
    }
    /* gradus < 2 phi: exponentes semper intra fines */
    (vacuum)polynomium_multiplica(a.p, b.p, piscina, &productum);
    redde _reducere(r, productum, I, piscina);
}

Cyclotomicus
cyclotomicus_potentia (
    Cyclotomicus  a,
             i32  e,
         Piscina* piscina)
{
    Cyclotomicus summa;

    si (a.anulus == NIHIL)
    {
        redde _invalidum();
    }
    summa = cyclotomicus_integer(a.anulus, magnus_ex_s64(I), piscina);
    dum (e > ZEPHYRUM)
    {
        si (e & I)
        {
            summa = cyclotomicus_multiplica(summa, a, piscina);
        }
        e = e >> I;
        si (e > ZEPHYRUM)
        {
            a = cyclotomicus_multiplica(a, a, piscina);
        }
    }
    redde summa;
}

b32
cyclotomicus_automorphismus (
    Cyclotomicus  a,
             s32  j,
         Piscina* piscina,
    Cyclotomicus* exitus)
{
     constans Cyclotomia* r = a.anulus;
                     s64  m;

    si (r == NIHIL)
    {
        redde FALSUM;
    }
    m = (s64)j % (s64)r->n;
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
    Cyclotomicus  a,
         Piscina* piscina)
{
    si (a.anulus == NIHIL)
    {
        redde _invalidum();
    }
    redde _reducere(a.anulus, a.p, -I, piscina);
}

/* prod_{j unitas, j != 1} sigma_j(a) */
interior Cyclotomicus
_conjugata_producta (
    Cyclotomicus  a,
         Piscina* piscina)
{
     constans Cyclotomia* r = a.anulus;
            Cyclotomicus  productum = cyclotomicus_integer(r,
                magnus_ex_s64(I), piscina);
                     i32 u;

    per (u = ZEPHYRUM; u < r->numerus_unitatum; u++)
    {
        si (r->unitates[u] % r->n == I % r->n)
        {
            perge;
        }
        productum = cyclotomicus_multiplica(productum, _reducere(r, a.p,
            (s32)r->unitates[u], piscina), piscina);
    }
    redde productum;
}

b32
cyclotomicus_norma (
     Cyclotomicus  a,
          Piscina* piscina,
           Magnus* exitus)
{
    si (a.anulus == NIHIL)
    {
        redde FALSUM;
    }
    redde cyclotomicus_est_integer(cyclotomicus_multiplica(a,
        _conjugata_producta(a, piscina), piscina), exitus);
}

b32
cyclotomicus_vestigium (
     Cyclotomicus  a,
          Piscina* piscina,
           Magnus* exitus)
{
     constans Cyclotomia* r = a.anulus;
            Cyclotomicus  summa;
                     i32  u;

    si (r == NIHIL)
    {
        redde FALSUM;
    }
    summa = cyclotomicus_nullum(r);
    per (u = ZEPHYRUM; u < r->numerus_unitatum; u++)
    {
        summa = cyclotomicus_adde(summa, _reducere(r, a.p,
            (s32)r->unitates[u], piscina), piscina);
    }
    redde cyclotomicus_est_integer(summa, exitus);
}

b32
cyclotomicus_divide_exacte (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina,
    Cyclotomicus* exitus)
{
     constans Cyclotomia* r = _communis(a, b);
            Cyclotomicus  c;
                  Magnus  n = magnus_ex_s64(ZEPHYRUM);
                  Magnus* alveus;
                     s32  e;

    si (r == NIHIL || cyclotomicus_est_nullum(b))
    {
        redde FALSUM;
    }
    /* productum conjugatorum semel: N(b) = b * productum (recensio:
     * olim bis computatum) */
    {
        Cyclotomicus productum = _conjugata_producta(b, piscina);

        (vacuum)cyclotomicus_est_integer(cyclotomicus_multiplica(b,
            productum, piscina), &n);
        c = cyclotomicus_multiplica(a, productum, piscina);
    }
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
            piscina,
                &q, &residuum)
            || magnus_signum(residuum) != ZEPHYRUM)
        {
            redde FALSUM;
        }
        alveus[e] = q;
    }
    exitus->anulus = r;
    (vacuum)polynomium_ex_coefficientibus(alveus, r->phi, ZEPHYRUM,
        piscina,
        &exitus->p);
    redde VERUM;
}

Cyclotomicus
cyclotomicus_modulus_quadratus (
    Cyclotomicus  a,
         Piscina* piscina)
{
    redde cyclotomicus_multiplica(a, cyclotomicus_conjugatum(a,
        piscina),
        piscina);
}

b32
cyclotomicus_est_radix (
    Cyclotomicus  a,
             s32* signum,
             i32* k)
{
     constans Cyclotomia* r = a.anulus;
                     i32  m;

    si (r == NIHIL)
    {
        redde FALSUM;
    }
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
    Cyclotomicus  a,
         Piscina* piscina)
{
    si (a.anulus == NIHIL)
    {
        redde chorda_transcribere(chorda_ex_literis("invalidum",
            piscina),
            piscina);
    }
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

    si (r == NIHIL || !polynomium_ex_chorda(textus, 'z', piscina, &p))
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
    /* textus decimalis longus: mantissa (XVII digiti) et exponens,
     * ne truncatio 10^300 in 10^254 vertat (recensio) */
    textus = magnus_ad_chordam(c, piscina);
    {
        i32 signum = textus.mensura > ZEPHYRUM && textus.datum[ZEPHYRUM]
            == '-' ? I : ZEPHYRUM;
        i32 digiti = (i32)textus.mensura - signum;
        i32 k;

        n = ZEPHYRUM;
        si (signum)
        {
            alveus[n++] = '-';
        }
        alveus[n++] = (character)textus.datum[signum];
        alveus[n++] = '.';
        per (k = I; k < digiti && k < XVII; k++)
        {
            alveus[n++] = (character)textus.datum[signum + k];
        }
        sprintf(alveus + n, "e%u", digiti - I);
    }
    redde strtod(alveus, NIHIL);
}

chorda
cyclotomicus_ad_ostendendum (
    Cyclotomicus  a,
             i32  digiti,
         Piscina* piscina)
{
          f64 re = 0.0;
          f64 im = 0.0;
          f64 limes;
          s32 e;
    /* |x| >= 10^15 per %e scribitur: longitudo finita (C89 snprintf
     * caret; olim %.*f numeri magni alveum CXXVIII excedebat) */
    character alveus[CCLVI];
    character pars_re[CXXVIII];
    character pars_im[CXXVIII];

    si (a.anulus == NIHIL)
    {
        redde chorda_transcribere(chorda_ex_literis("invalidum",
            piscina),
            piscina);
    }
    si (!polynomium_est_nullum(a.p))
    {
        per (e = polynomium_gradus_imus(a.p);
            e <= polynomium_gradus_summus(a.p); e++)
        {
            f64 c = _ad_f64(polynomium_coefficiens(a.p, e), piscina);
            f64 angulus = 2.0 * 3.14159265358979323846 * (f64)e
                / (f64)a.anulus->n;

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
    sprintf(pars_re, fabs(re) >= 1e15 ? "%.*e" : "%.*f",
        (integer)digiti,
        re);
    sprintf(pars_im, fabs(im) >= 1e15 ? "%.*e" : "%.*f",
        (integer)digiti,
        fabs(im));
    sprintf(alveus, "%s %c %si", pars_re, im < 0.0 ? '-' : '+',
        pars_im);
    redde chorda_transcribere(chorda_ex_literis(alveus, piscina),
        piscina);
}
