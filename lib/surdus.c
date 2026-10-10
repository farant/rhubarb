/* surdus.c - Surdi: summae radicum quadratarum, signum exactum
 *
 * BASIS: S mascula super primos (bitum i <-> primi[i]); sqrt(P_S) sqrt(P_T)
 * = P_{S & T} sqrt(P_{S ^ T}), ergo productum: c[S ^ T] += a[S] b[T]
 * producta[S & T].
 *
 * CUSTODIA s64: ante multiplicationem bita maxima computantur; si
 * bA + bB + bP + k <= 63 (|termini| < 2^(bA+bB+bP), 2^k termini per
 * coefficientem) via celeris sine custodia, aliter per operationem
 * custodita (divisio) et FALSUM in excessu.
 *
 * FILTRUM (gradus 2), u = 2^-53, n = 2^k <= 8 termini:
 *   terminus t_S = fl(fl(c_S) r_S), r_S = fl(sqrt(P_S)) (P_S < 2^45
 *   exacte repraesentabile, sqrt IEEE recte rotundata): tres
 *   rotundationes, |t_S - c_S sqrt(P_S)| <= 3.01 u |c_S sqrt(P_S)|;
 *   summa recursiva n terminorum: <= (n-1) 1.01 u sum|t_S|.
 *   Ergo |fl(x) - x| <= (n + 3) u M, M = sum|t_S| (fere). Limes adhibitus
 *   E = 2 (n + 4) u M: factor II M ipsum rotundatum et ordinis secundi
 *   terminos tegit. |fl(x)| > E -> signum exactum. FMA (contractio)
 *   rotundationem unam tollit, numquam addit: limes manet.
 *   Valores integri: nullus subfluxus; |c| < 2^63, r < 2^23: nullus
 *   superfluxus.
 *
 * QUADRATIO (gradus 3): x = a + b sqrt(p_{k-1}), a, b in spatio k-1;
 * signa a, b recursive (cum filtro); opposita -> signum(a) signum(a^2 -
 * p b^2) (numquam 0: aliter sqrt(p) in spatio minore esset).
 * Vide lib/surdus.worklog.md.
 */
#include "surdus.h"
#include "surdus_interna.h"
#include <math.h>

#define S64_MAXIMUS  0x7FFFFFFFFFFFFFFFLL
#define S64_MINIMUS  (-S64_MAXIMUS - 1LL)
#define UNITAS_ROTUNDATIONIS  1.1102230246251565e-16   /* 2^-53 */


/* ==================================================
 * Auxilia
 * ================================================== */

interior i64
_magnitudo (
    s64 x)
{
    redde x < 0 ? (i64)0 - (i64)x : (i64)x;
}

/* numerus bitorum: 0 -> 0, 1 -> 1, 2^62 -> 63 */
interior s32
_bita (
    i64 x)
{
    s32 n = ZEPHYRUM;

    si (x >> XXXII)
    {
        n += XXXII;
        x >>= XXXII;
    }
    si (x >> XVI)
    {
        n += XVI;
        x >>= XVI;
    }
    si (x >> VIII)
    {
        n += VIII;
        x >>= VIII;
    }
    si (x >> IV)
    {
        n += IV;
        x >>= IV;
    }
    dum (x)
    {
        n++;
        x >>= I;
    }
    redde n;
}

interior b32
_adde_tuta (
    s64  a,
    s64  b,
    s64* r)
{
    si (   (b > 0 && a > S64_MAXIMUS - b)
        || (b < 0 && a < S64_MINIMUS - b))
    {
        redde FALSUM;
    }
    *r = a + b;
    redde VERUM;
}

/* a - b sine negatione: -(-2^63) indefinitum esset (comportamentum
 * indefinitum quod nulla probatio certe capit - planta 'custodia -2^63
 * omissa' superstes erat, compilatore casu recte agente) */
interior b32
_subtrahe_tuta (
    s64  a,
    s64  b,
    s64* r)
{
    si (   (b < 0 && a > S64_MAXIMUS + b)
        || (b > 0 && a < S64_MINIMUS + b))
    {
        redde FALSUM;
    }
    *r = a - b;
    redde VERUM;
}

/* |a b| <= 2^63 - 1, aliter FALSUM (etiam -2^63 exactum recusatur) */
interior b32
_multiplica_tuta (
    s64  a,
    s64  b,
    s64* r)
{
    i64 ma = _magnitudo(a);
    i64 mb = _magnitudo(b);

    si (ma == 0 || mb == 0)
    {
        *r = ZEPHYRUM;
        redde VERUM;
    }
    si (ma > (i64)S64_MAXIMUS / mb)
    {
        redde FALSUM;
    }
    *r = a * b;
    redde VERUM;
}

interior s32
_bita_maxima (
    constans Surdus* x,
                s32  n)
{
    i64 maxima = 0;
    s32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        i64 m = _magnitudo(x->c[i]);

        si (m > maxima)
        {
            maxima = m;
        }
    }
    redde _bita(maxima);
}

/* productum in spatio primorum k priorum */
interior b32
_multiplica_k (
    constans SurdiSpatium* sp,
    constans       Surdus* a,
    constans       Surdus* b,
                      s32  k,
                   Surdus* exitus)
{
       s32 n = (s32)I << k;
    Surdus r;
       s32 s;
       s32 t;

    per (s = ZEPHYRUM; s < VIII; s++)
    {
        r.c[s] = ZEPHYRUM;
    }
    si (_bita_maxima(a, n) + _bita_maxima(b, n)
            + _bita((i64)sp->producta[n - I]) + k <= LXIII)
    {
        /* via celeris: nullus excessus possibilis */
        per (s = ZEPHYRUM; s < n; s++)
        {
            si (a->c[s] == 0)
            {
                perge;
            }
            per (t = ZEPHYRUM; t < n; t++)
            {
                r.c[s ^ t] += a->c[s] * b->c[t] * sp->producta[s & t];
            }
        }
    }
    alioquin
    {
        per (s = ZEPHYRUM; s < n; s++)
        {
            per (t = ZEPHYRUM; t < n; t++)
            {
                s64 terminus;

                si (   !_multiplica_tuta(a->c[s], b->c[t], &terminus)
                    || !_multiplica_tuta(terminus, sp->producta[s & t],
                        &terminus)
                    || !_adde_tuta(r.c[s ^ t], terminus, &r.c[s ^ t]))
                {
                    redde FALSUM;
                }
            }
        }
    }
    *exitus = r;
    redde VERUM;
}


/* ==================================================
 * Spatium
 * ================================================== */

interior b32
_est_primus (
    s32 p)
{
    s32 d;

    si (p < II)
    {
        redde FALSUM;
    }
    /* d <= p/d: d*d numquam formatur (p prope 2^31 excederet s32) */
    per (d = II; d <= p / d; d++)
    {
        si (p % d == 0)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
surdi_spatium (
    constans          s32* primi,
                      s32  numerus,
             SurdiSpatium* exitus)
{
    SurdiSpatium sp;
             s32 i;
             s32 s;

    si (numerus < 0 || numerus > SURDUS_RADICES_MAXIMAE)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (   primi[i] >= 32768 || !_est_primus(primi[i])
            || (i > 0 && primi[i] <= primi[i - I]))
        {
            redde FALSUM;
        }
        sp.primi[i] = primi[i];
    }
    per (; i < III; i++)
    {
        sp.primi[i] = ZEPHYRUM;
    }
    sp.numerus = numerus;
    per (s = ZEPHYRUM; s < VIII; s++)
    {
        s64 productum = I;

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            si (s & (I << i))
            {
                productum *= sp.primi[i];
            }
        }
        sp.producta[s] = s < ((s32)I << numerus) ? productum : ZEPHYRUM;
        sp.radices[s] = sqrt((f64)sp.producta[s]);
    }
    *exitus = sp;
    redde VERUM;
}


/* ==================================================
 * Surdus
 * ================================================== */

Surdus
surdus_ex_s64 (
    s64 x)
{
    Surdus r;
       s32 s;

    per (s = ZEPHYRUM; s < VIII; s++)
    {
        r.c[s] = ZEPHYRUM;
    }
    r.c[ZEPHYRUM] = x;
    redde r;
}

b32
surdus_basis (
    constans SurdiSpatium* sp,
                      i32  selectio,
                      s64  c,
                   Surdus* exitus)
{
    si (selectio >= ((i32)I << sp->numerus))
    {
        redde FALSUM;
    }
    *exitus              = surdus_ex_s64(ZEPHYRUM);
    exitus->c[selectio]  = c;
    redde VERUM;
}

b32
surdus_est_nullum (
    Surdus x)
{
    s32 s;

    per (s = ZEPHYRUM; s < VIII; s++)
    {
        si (x.c[s] != 0)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
surdus_adde (
    Surdus  a,
    Surdus  b,
    Surdus* exitus)
{
    Surdus r;
       s32 s;

    per (s = ZEPHYRUM; s < VIII; s++)
    {
        si (!_adde_tuta(a.c[s], b.c[s], &r.c[s]))
        {
            redde FALSUM;
        }
    }
    *exitus = r;
    redde VERUM;
}

b32
surdus_subtrahe (
    Surdus  a,
    Surdus  b,
    Surdus* exitus)
{
    Surdus r;
       s32 s;

    per (s = ZEPHYRUM; s < VIII; s++)
    {
        si (!_subtrahe_tuta(a.c[s], b.c[s], &r.c[s]))
        {
            redde FALSUM;
        }
    }
    *exitus = r;
    redde VERUM;
}

b32
surdus_scala (
    Surdus  a,
       s64  c,
    Surdus* exitus)
{
    Surdus r;
       s32 s;

    per (s = ZEPHYRUM; s < VIII; s++)
    {
        si (!_multiplica_tuta(a.c[s], c, &r.c[s]))
        {
            redde FALSUM;
        }
    }
    *exitus = r;
    redde VERUM;
}

/* coefficiens extra basin spatii (S >= 2^k) non nullus? Tales
 * recusantur: aliter signum et productum eos ut 0 legerent, est_nullum
 * ut non nullos (recensio 2026-10-09, S3) */
interior b32
_extra_basin (
    constans SurdiSpatium* sp,
    constans       Surdus* x)
{
    s32 s;

    per (s = (s32)I << sp->numerus; s < VIII; s++)
    {
        si (x->c[s] != 0)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
surdus_multiplica (
    constans SurdiSpatium* sp,
                   Surdus  a,
                   Surdus  b,
                   Surdus* exitus)
{
    si (_extra_basin(sp, &a) || _extra_basin(sp, &b))
    {
        redde FALSUM;
    }
    redde _multiplica_k(sp, &a, &b, sp->numerus, exitus);
}


/* ==================================================
 * Signum
 * ================================================== */

interior b32
_signum_k (
    constans SurdiSpatium* sp,
    constans       Surdus* x,
                      s32  k,
                      b32  filtrum,
                      s32* signum,
                      s32* gradus)
{
       s32 n = (s32)I << k;
       s32 s;
       s32 sa;
       s32 sb;
       s32 sd;
       s32 ga = I;
       s32 gb = I;
       s32 gd = I;
    Surdus a;
    Surdus b;
    Surdus d;

    /* gradus 1: nullum, aut rationale */
    per (s = I; s < n; s++)
    {
        si (x->c[s] != 0)
        {
            frange;
        }
    }
    si (s == n)
    {
        *signum = (x->c[ZEPHYRUM] > 0) - (x->c[ZEPHYRUM] < 0);
        *gradus = I;
        redde VERUM;
    }
    /* gradus 2: filtrum duplex certificatum */
    si (filtrum)
    {
        f64 summa = 0.0;
        f64 moles = 0.0;

        per (s = ZEPHYRUM; s < n; s++)
        {
            f64 terminus = (f64)x->c[s] * sp->radices[s];

            summa += terminus;
            moles += fabs(terminus);
        }
        si (fabs(summa) > 2.0 * (f64)(n
            + IV) * UNITAS_ROTUNDATIONIS * moles)
        {
            *signum = summa > 0.0 ? I : -(s32)I;
            *gradus = II;
            redde VERUM;
        }
    }
    /* gradus 3: x = a + b sqrt(p_{k-1}) */
    per (s = ZEPHYRUM; s < VIII; s++)
    {
        a.c[s] = ZEPHYRUM;
        b.c[s] = ZEPHYRUM;
    }
    per (s = ZEPHYRUM; s < n / II; s++)
    {
        a.c[s] = x->c[s];
        b.c[s] = x->c[s + n / II];
    }
    si (   !_signum_k(sp, &a, k - I, filtrum, &sa, &ga)
        || !_signum_k(sp, &b, k - I, filtrum, &sb, &gb))
    {
        redde FALSUM;
    }
    *gradus = ga > gb ? ga : gb;
    si (sb == 0 || sa == sb)
    {
        *signum = sa;
        redde VERUM;
    }
    si (sa == 0)
    {
        *signum = sb;
        redde VERUM;
    }
    /* signa opposita: signum(a) signum(a^2 - p b^2) */
    si (   !_multiplica_k(sp, &a, &a, k - I, &a)
        || !_multiplica_k(sp, &b, &b, k - I, &b)
        || !surdus_scala(b, (s64)sp->primi[k - I], &b)
        || !surdus_subtrahe(a, b, &d)
        || !_signum_k(sp, &d, k - I, filtrum, &sd, &gd))
    {
        redde FALSUM;
    }
    *signum = sa * sd;
    *gradus = III;
    redde VERUM;
}

b32
surdi_signum_gradu (
    constans SurdiSpatium* sp,
                   Surdus  x,
                      b32  filtrum,
                      s32* exitus,
                      s32* gradus)
{
    s32 signum;
    s32 g;

    si (   _extra_basin(sp, &x)
        || !_signum_k(sp, &x, sp->numerus, filtrum, &signum, &g))
    {
        redde FALSUM;
    }
    *exitus = signum;
    si (gradus)
    {
        *gradus = g;
    }
    redde VERUM;
}

b32
surdus_signum (
    constans SurdiSpatium* sp,
                   Surdus  x,
                      s32* exitus)
{
    redde surdi_signum_gradu(sp, x, VERUM, exitus, NIHIL);
}

b32
surdus_compara (
    constans SurdiSpatium* sp,
                   Surdus  a,
                   Surdus  b,
                      s32* exitus)
{
    Surdus d;

    si (!surdus_subtrahe(a, b, &d))
    {
        redde FALSUM;
    }
    redde surdus_signum(sp, d, exitus);
}
