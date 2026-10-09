/* agmen.h - Arithmetica integra per agmina (SIMD), EXACTA
 *
 * Agmen: acies eodem gradu incedens. Operationes super ordines
 * integrorum quas compilator in lanas vectoris (NEON in arm64) vertit.
 * Effectus BIT PRO BIT idem ac via scalaris (congruentia.h = oraculum).
 * SOLUM INTEGRA: fluitantia rotundationem et contractionem FMA
 * mutarent (agenda A1).
 *
 * Nulla piscina, nulla allocatio: vocans memoriam praebet. exitus
 * aequalis argumento (in loco) licet; tegumentum partiale non.
 * Residua canonica intrant et exeunt (0 <= x < p), ut congruentia;
 * contractus non custoditur (ansa calida): residuum >= p effectum
 * falsum dat.
 *
 * VECTORIZATIO: nuclei C purum sunt, scripti ut clang (-O2) eos ipse
 * vectorizet (NEON in arm64, SSE/AVX in x86) - nullae intrinsecae,
 * nulla electio machinae. NEON manu scriptum clang 1.0-1.2x tantum
 * vicit (mensura: tools/agmen_mensura.c). Quia celeritas sic invisibilis
 * est, porta agmen-vectorizatio rubet si ansa nuclei vectorizari
 * desinit - aliter probationes virides manerent dum celeritas perit.
 *
 * USUS:
 *   AgmenModulus m;
 *   si (agmen_modulus(congruentia_primus_infra(0x80000000U), &m))
 *       agmen_multiplica_modulo(a, b, exitus, numerus, &m);
 *
 * Vide lib/agmen.worklog.md.
 */
/* <aedilis corpus="lib/agmen.c"/> */
#ifndef AGMEN_H
#define AGMEN_H

#include "latina.h"


/* ==================================================
 * Modulus
 * ================================================== */

/* Modulus praeparatus (Montgomery, R = 2^32): semel computatur, saepe
 * adhibetur. Membra a vocante non mutanda. */
nomen structura {
    i32 p;            /* impar, 3 <= p < 2^31 */
    i32 p_inversa;    /* -p^-1 mod 2^32 */
    i32 r2;           /* 2^64 mod p */
} AgmenModulus;

/* FALSUM si p par, p < 3, aut p >= 2^31 (exitus non tangitur). Primi:
 * congruentia_primus_infra(0x80000000U), deinde infra eum. */
b32
agmen_modulus (
              i32  p,
     AgmenModulus* exitus);


/* ==================================================
 * Arithmetica modularis super ordines
 * ================================================== */

/* exitus[i] = a[i] + b[i] mod p */
vacuum
agmen_adde_modulo (
    constans          i32* a,
    constans          i32* b,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus);

/* exitus[i] = a[i] - b[i] mod p */
vacuum
agmen_subtrahe_modulo (
    constans          i32* a,
    constans          i32* b,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus);

/* exitus[i] = a[i] * b[i] mod p */
vacuum
agmen_multiplica_modulo (
    constans          i32* a,
    constans          i32* b,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus);

/* exitus[i] = exitus[i] + c * a[i] mod p - nucleus eliminationis (c
 * quoque residuum canonicum, 0 <= c < p) */
vacuum
agmen_multiplica_adde_modulo (
                      i32  c,
    constans          i32* a,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus);

/* sum_i a[i] * b[i] mod p; numerus 0 -> 0 */
i32
agmen_productum_internum_modulo (
    constans          i32* a,
    constans          i32* b,
                      i32  numerus,
    constans AgmenModulus* modulus);

#endif /* AGMEN_H */
