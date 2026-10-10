/* agmen.c - Arithmetica integra per agmina (SIMD)
 *
 * Duo strata (vide agmen.h):
 *   1. viae scalares relatae (agminis_*_scalaris) - ansae sub pragma
 *      NON vectorizatae: oraculum probationum;
 *   2. nuclei publici - C PURUM, idem calculus, ansae liberae quas clang
 *      (-O2) ipse vectorizat. Nullae intrinsecae: NEON manu scriptum
 *      clang 1.0-1.2x tantum vicit (lib/agmen.worklog.md, mensura).
 * Ansa nuclei publici notam 'AGMEN VECTORIZANDA' gerit; porta
 * agmen-vectorizatio (tools/agmen_vectorizatio.sh) rubet si ansa notata
 * vectorizari desinit aut ansa alia (relata) vectorizatur.
 *
 * MONTGOMERY (R = 2^32, p impar < 2^31). redc(T) = T R^-1 mod p pro
 * T < p R: q = (T mod R) p' mod R (p' = -p^-1), (T + q p) / R exacte
 * divisibile, < 2p; una subtractio condicionalis. Cum p < 2^31,
 * T + q p < 2 p R < 2^64: i64 sufficit, sine 128 bitis (C89).
 *   a b mod p      = redc(redc(a b) r2)        (r2 = R^2 mod p)
 *   c R mod p      = redc(c r2)                (c in forma Montgomery)
 *   a c mod p      = redc(a (c R mod p))       (c fixum: redc unum)
 *   sum a b mod p  = redc((sum redc(a b) mod p) r2)
 * Auxilia 'interior' sunt: clang ea in ansas inserit, aliter ansa
 * vocationem continet et non vectorizatur.
 * Vide lib/agmen.worklog.md.
 */
#include "agmen.h"
#include "agmen_interna.h"

/* Ansae viae relatae sub clang NON vectorizantur: '#pragma clang loop'
 * ante quamque, scriptum expresse (macro cum _Pragma zero lexemata
 * parit - invocatio vacua, quam canon silvae in corpore vetat). */


/* ==================================================
 * Modulus
 * ================================================== */

b32
agmen_modulus (
              i32  p,
     AgmenModulus* exitus)
{
    i32 inversa;
    i64 r;
    s32 k;

    si ((p & I) == ZEPHYRUM || p < III || p >= 0x80000000U)
    {
        redde FALSUM;
    }
    /* Newton modulo 2^32: p p = 1 mod 8 (III bita recta), quisque
     * gradus bita duplicat: 3 -> 6 -> 12 -> 24 -> 48 */
    inversa = p;
    per (k = ZEPHYRUM; k < IV; k++)
    {
        inversa = inversa * (II - p * inversa);
    }
    r                  = ((i64)I << XXXII) % (i64)p;
    exitus->p          = p;
    exitus->p_inversa  = ZEPHYRUM - inversa;
    exitus->r2         = (i32)((r * r) % (i64)p);
    redde VERUM;
}


/* ==================================================
 * Auxilia scalaria
 * ================================================== */

/* t R^-1 mod p, t < p R */
interior i32
_reducere (
                      i64  t,
    constans AgmenModulus* modulus)
{
    i32 q = (i32)t * modulus->p_inversa;
    i32 r = (i32)((t + (i64)q * (i64)modulus->p) >> XXXII);

    si (r >= modulus->p)
    {
        r -= modulus->p;
    }
    redde r;
}

interior i32
_adde (
    i32 a,
    i32 b,
    i32 p)
{
    i32 s = a + b;     /* < 2p < 2^32 */

    si (s >= p)
    {
        s -= p;
    }
    redde s;
}

interior i32
_subtrahe (
    i32 a,
    i32 b,
    i32 p)
{
    i32 d = a - b;     /* modulo 2^32 */

    si (a < b)
    {
        d += p;
    }
    redde d;
}

interior i32
_multiplica (
                      i32  a,
                      i32  b,
    constans AgmenModulus* modulus)
{
    redde _reducere((i64)_reducere((i64)a * (i64)b,
        modulus) * (i64)modulus->r2,
        modulus);
}


/* ==================================================
 * 1. Viae scalares relatae
 * ================================================== */

vacuum
agminis_adde_modulo_scalaris (
    constans          i32* a,
    constans          i32* b,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus)
{
    i32 i;

#if defined(__clang__)
#pragma clang loop vectorize(disable) interleave(disable)
#endif
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        exitus[i] = _adde(a[i], b[i], modulus->p);
    }
}

vacuum
agminis_subtrahe_modulo_scalaris (
    constans          i32* a,
    constans          i32* b,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus)
{
    i32 i;

#if defined(__clang__)
#pragma clang loop vectorize(disable) interleave(disable)
#endif
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        exitus[i] = _subtrahe(a[i], b[i], modulus->p);
    }
}

vacuum
agminis_multiplica_modulo_scalaris (
    constans          i32* a,
    constans          i32* b,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus)
{
    i32 i;

#if defined(__clang__)
#pragma clang loop vectorize(disable) interleave(disable)
#endif
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        exitus[i] = _multiplica(a[i], b[i], modulus);
    }
}

vacuum
agminis_multiplica_adde_modulo_scalaris (
                      i32  c,
    constans          i32* a,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus)
{
    i32 c_montgomery = _reducere((i64)c * (i64)modulus->r2, modulus);
    i32 i;

#if defined(__clang__)
#pragma clang loop vectorize(disable) interleave(disable)
#endif
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        exitus[i] = _adde(exitus[i],
            _reducere((i64)a[i] * (i64)c_montgomery, modulus),
            modulus->p);
    }
}

i32
agminis_productum_internum_modulo_scalaris (
    constans          i32* a,
    constans          i32* b,
                      i32  numerus,
    constans AgmenModulus* modulus)
{
    i64 summa = ZEPHYRUM;     /* termini < 2^31: sine redundantia */
    i32 i;

#if defined(__clang__)
#pragma clang loop vectorize(disable) interleave(disable)
#endif
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        summa += _reducere((i64)a[i] * (i64)b[i], modulus);
    }
    redde _reducere((summa % (i64)modulus->p) * (i64)modulus->r2,
        modulus);
}


/* ==================================================
 * 2. Nuclei publici (C purum, a compilatore vectorizati)
 * ================================================== */

vacuum
agmen_adde_modulo (
    constans          i32* a,
    constans          i32* b,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus)
{
    i32 p = modulus->p;
    i32 i;

    /* AGMEN VECTORIZANDA: adde */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        exitus[i] = _adde(a[i], b[i], p);
    }
}

vacuum
agmen_subtrahe_modulo (
    constans          i32* a,
    constans          i32* b,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus)
{
    i32 p = modulus->p;
    i32 i;

    /* AGMEN VECTORIZANDA: subtrahe */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        exitus[i] = _subtrahe(a[i], b[i], p);
    }
}

vacuum
agmen_multiplica_modulo (
    constans          i32* a,
    constans          i32* b,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus)
{
    i32 i;

    /* AGMEN VECTORIZANDA: multiplica */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        exitus[i] = _multiplica(a[i], b[i], modulus);
    }
}

vacuum
agmen_multiplica_adde_modulo (
                      i32  c,
    constans          i32* a,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus)
{
    i32 c_montgomery = _reducere((i64)c * (i64)modulus->r2, modulus);
    i32 i;

    /* AGMEN VECTORIZANDA: multiplica_adde */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        exitus[i] = _adde(exitus[i],
            _reducere((i64)a[i] * (i64)c_montgomery, modulus),
            modulus->p);
    }
}

i32
agmen_productum_internum_modulo (
    constans          i32* a,
    constans          i32* b,
                      i32  numerus,
    constans AgmenModulus* modulus)
{
    i64 summa = ZEPHYRUM;     /* termini < 2^31: sine redundantia */
    i32 i;

    /* AGMEN VECTORIZANDA: productum_internum */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        summa += _reducere((i64)a[i] * (i64)b[i], modulus);
    }
    redde _reducere((summa % (i64)modulus->p) * (i64)modulus->r2,
        modulus);
}
