/* congruentia.c - Arithmetica modularis exacta, moduli verbi (n < 2^32)
 *
 * Producta in i64 insignato: (n - 1)^2 < 2^64. Inversa per Euclidem
 * extensum in s64 (|coefficientes| <= n < 2^32). Primalitas per
 * Miller-Rabin cum basibus 2, 7, 61 - exacta pro omni n < 4759123141
 * (Jaeschke 1993), ergo pro omni i32. Reconstructio Sinica per Garner
 * super magnum. Vide lib/congruentia.worklog.md.
 */
#include "congruentia.h"


/* ==================================================
 * Reductio
 * ================================================== */

i32
congruentia_ex_s64 (
    s64 x,
    i32 n)
{
    i64 modulus;
    i64 residuum;

    si (n == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (x >= ZEPHYRUM)
    {
        redde (i32)((i64)x % (i64)n);
    }
    /* |x| sine exundatione etiam pro S64 imo */
    modulus   = (i64)(-(x + I)) + (i64)I;
    residuum  = modulus % (i64)n;
    redde residuum == ZEPHYRUM ? ZEPHYRUM : (i32)((i64)n - residuum);
}

i32
congruentia_ex_magno (
    Magnus x,
       i32 n)
{
    redde magnus_residuum_parvum(x, n);
}


/* ==================================================
 * Arithmetica
 * ================================================== */

i32
congruentia_adde (
    i32 a,
    i32 b,
    i32 n)
{
    i64 summa = (i64)a + (i64)b;

    si (summa >= (i64)n)
    {
        summa -= (i64)n;
    }
    redde (i32)summa;
}

i32
congruentia_subtrahe (
    i32 a,
    i32 b,
    i32 n)
{
    redde a >= b ? a - b : (i32)((i64)a + (i64)n - (i64)b);
}

i32
congruentia_multiplica (
    i32 a,
    i32 b,
    i32 n)
{
    redde (i32)(((i64)a * (i64)b) % (i64)n);
}

i32
congruentia_potentia (
    i32 a,
    i64 e,
    i32 n)
{
    i32 effectus  = (n == I) ? ZEPHYRUM : I;
    i32 basis     = a;

    dum (e > ZEPHYRUM)
    {
        si (e & (i64)I)
        {
            effectus = congruentia_multiplica(effectus, basis, n);
        }
        e >>= I;
        si (e > ZEPHYRUM)
        {
            basis = congruentia_multiplica(basis, basis, n);
        }
    }
    redde effectus;
}

b32
congruentia_inversa (
     i32  a,
     i32  n,
     i32* exitus)
{
    s64 r0 = (s64)n;
    s64 r1 = (s64)a;
    s64 t0 = ZEPHYRUM;
    s64 t1 = I;

    si (n < II)
    {
        redde FALSUM;
    }
    /* invarians: r_k = t_k a (mod n); |t_k| <= n */
    dum (r1 != ZEPHYRUM)
    {
        s64 quotiens  = r0 / r1;   /* r0, r1 >= 0: C89 definitum */
        s64 r2        = r0 - quotiens * r1;
        s64 t2        = t0 - quotiens * t1;

        r0 = r1;
        r1 = r2;
        t0 = t1;
        t1 = t2;
    }
    si (r0 != I)
    {
        redde FALSUM;
    }
    *exitus = congruentia_ex_s64(t0, n);
    redde VERUM;
}

s64
congruentia_symmetrica (
    i32 a,
    i32 n)
{
    redde a > n / II ? (s64)a - (s64)n : (s64)a;
}


/* ==================================================
 * Primi
 * ================================================== */

/* testis Miller-Rabin: n - 1 = d 2^s, d impar; VERUM si n probabiliter
 * primus ad basim b */
interior b32
_testis (
    i32 n,
    i32 basis,
    i32 d,
    i32 s)
{
    i32 x = congruentia_potentia(basis % n, (i64)d, n);
    i32 k;

    si (x == I || x == n - I || basis % n == ZEPHYRUM)
    {
        redde VERUM;
    }
    per (k = I; k < s; k++)
    {
        x = congruentia_multiplica(x, x, n);
        si (x == n - I)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
congruentia_est_primus (
    i32 n)
{
    i32 d;
    i32 s = ZEPHYRUM;

    si (n < II)
    {
        redde FALSUM;
    }
    si (n < IV)
    {
        redde VERUM;
    }
    si (n % II == ZEPHYRUM)
    {
        redde FALSUM;
    }
    d = n - I;
    dum (d % II == ZEPHYRUM)
    {
        d /= II;
        s++;
    }
    redde _testis(n, II, d, s) && _testis(n, VII, d, s)
        && _testis(n, LXI, d, s);
}

i32
congruentia_primus_infra (
    i32 limes)
{
    i32 n;

    si (limes <= II)
    {
        redde ZEPHYRUM;
    }
    si (limes == III)
    {
        redde II;
    }
    /* impar maximus < limes */
    n = (limes - I) % II == ZEPHYRUM ? limes - II : limes - I;
    dum (n >= III)
    {
        si (congruentia_est_primus(n))
        {
            redde n;
        }
        n -= II;
    }
    redde II;
}


/* ==================================================
 * Reconstructio Sinica
 * ================================================== */

b32
congruentia_restitue (
    constans i32* residua,
    constans i32* moduli,
             i32  numerus,
             b32  symmetricus,
         Piscina* piscina,
          Magnus* exitus)
{
    Magnus x;
    Magnus productum;
       i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (moduli[k] < II || residua[k] >= moduli[k])
        {
            redde FALSUM;
        }
    }
    si (numerus == ZEPHYRUM)
    {
        *exitus = magnus_ex_s64(ZEPHYRUM);
        redde VERUM;
    }
    /* Garner: x_k = x_(k-1) + M_(k-1) t, t = (r_k - x_(k-1)) M^-1 mod
     * m_k; M^-1 existit sse moduli coprimi */
    x          = magnus_ex_s64((s64)residua[ZEPHYRUM]);
    productum  = magnus_ex_s64((s64)moduli[ZEPHYRUM]);
    per (k = I; k < numerus; k++)
    {
        i32 m = moduli[k];
        i32 inversa;
        i32 t;

        si (!congruentia_inversa(magnus_residuum_parvum(productum, m),
            m,
            &inversa))
        {
            redde FALSUM;
        }
        t = congruentia_multiplica(congruentia_subtrahe(residua[k],
            magnus_residuum_parvum(x, m), m), inversa, m);
        x = magnus_adde(x, magnus_multiplica(productum, magnus_ex_s64(
            (s64)t), piscina), piscina);
        productum = magnus_multiplica(productum, magnus_ex_s64((s64)m),
            piscina);
    }
    /* symmetricus: x > M/2 sse 2x > M */
    si (   symmetricus
        && magnus_compara(magnus_multiplica(x, magnus_ex_s64(II),
        piscina),
            productum) > ZEPHYRUM)
    {
        x = magnus_subtrahe(x, productum, piscina);
    }
    *exitus = x;
    redde VERUM;
}
