/* agmen_mensura.c - Mensura agminis: nanosecundae per elementum
 *
 * Tres viae eiusdem operationis comparantur:
 *   congruentia  '%' 64 bitorum per elementum (via domus olim)
 *   scalaris     Montgomery, ansa NON vectorizata (agmen_interna.h)
 *   agmen        Montgomery idem, C purum a clang vectorizatum (agmen.h)
 * NEON manu scriptum (sublatum 2026-10-09) agmen 1.0-1.2x tantum vicit -
 * numeri in lib/agmen.worklog.md; experimentum futurum contra 'agmen'
 * hic metiendum.
 * Ante mensuram effectus omnium viarum conferuntur (inaequales ->
 * RECUSAT). Tempus per clock() (C89; tempus processus). Ordines
 * IV * MXXIV elementorum (in L1 manent), iterationes ut quaeque via
 * ~0.2 s currat.
 *
 * Usus: ./bin/aedilis tools/agmen_mensura.c && bash
 *   build/aedilis/agmen_mensura/struere.sh &&
 *   ./build/aedilis/agmen_mensura/agmen_mensura
 */
#include "latina.h"
#include "agmen.h"
#include "agmen_interna.h"
#include "congruentia.h"
#include "sors.h"
#include <stdio.h>
#include <time.h>

#define NUMERUS     4096
#define ITERATIONES 20000

interior i32 ordo_a[NUMERUS];
interior i32 ordo_b[NUMERUS];
interior i32 exitus_i[NUMERUS];
interior i32 exitus_ii[NUMERUS];

/* consumptor: ne compilator calculum ut mortuum deleat */
interior volatilis i32 consumptor;


/* ==================================================
 * Via domus olim: '%' per elementum
 * ================================================== */

interior vacuum
_multiplica_congruentia (
    constans i32* a,
    constans i32* b,
             i32* exitus,
             i32  numerus,
             i32  p)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        exitus[i] = congruentia_multiplica(a[i], b[i], p);
    }
}

interior i32
_productum_congruentia (
    constans i32* a,
    constans i32* b,
             i32  numerus,
             i32  p)
{
    i32 summa = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        summa = congruentia_adde(summa, congruentia_multiplica(a[i],
            b[i],
            p), p);
    }
    redde summa;
}


/* ==================================================
 * Mensura
 * ================================================== */

interior f64
_ns_per_elementum (
    clock_t initium,
    clock_t finis)
{
    redde (f64)(finis - initium) / (f64)CLOCKS_PER_SEC * 1e9
        / ((f64)NUMERUS * (f64)ITERATIONES);
}

interior b32
_aequales (
    constans i32* x,
    constans i32* y)
{
    i32 i;

    per (i = ZEPHYRUM; i < NUMERUS; i++)
    {
        si (x[i] != y[i])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

s32
principale (vacuum)
{
    AgmenModulus m;
            Sors s;
             i32 p = congruentia_primus_infra(0x80000000U);
             i32 i;
             i32 k;
         clock_t t0;
             f64 ns[III];
             i32 producta[III];

    si (!agmen_modulus(p, &m))
    {
        imprimere("RECUSAT: modulus\n");
        redde I;
    }
    sors_seminare(&s, 2026ULL, I);
    per (i = ZEPHYRUM; i < NUMERUS; i++)
    {
        ordo_a[i] = sors_intra(&s, p);
        ordo_b[i] = sors_intra(&s, p);
    }
    imprimere("agmen mensura: p = %u, %u elementa, %u iterationes\n", p,
        NUMERUS, ITERATIONES);

    /* aequalitas ante mensuram */
    _multiplica_congruentia(ordo_a, ordo_b, exitus_i, NUMERUS, p);
    agmen_multiplica_modulo(ordo_a, ordo_b, exitus_ii, NUMERUS, &m);
    si (!_aequales(exitus_i, exitus_ii))
    {
        imprimere("RECUSAT: agmen != congruentia\n");
        redde I;
    }
    agminis_multiplica_modulo_scalaris(ordo_a, ordo_b, exitus_ii,
        NUMERUS,
        &m);
    si (!_aequales(exitus_i, exitus_ii))
    {
        imprimere("RECUSAT: scalaris != congruentia\n");
        redde I;
    }

    /* multiplica */
    t0 = clock();
    per (k = ZEPHYRUM; k < ITERATIONES; k++)
    {
        _multiplica_congruentia(ordo_a, ordo_b, exitus_i, NUMERUS, p);
        consumptor = exitus_i[k % NUMERUS];
    }
    ns[ZEPHYRUM]  = _ns_per_elementum(t0, clock());
    t0            = clock();
    per (k = ZEPHYRUM; k < ITERATIONES; k++)
    {
        agminis_multiplica_modulo_scalaris(ordo_a, ordo_b, exitus_i,
            NUMERUS,
            &m);
        consumptor = exitus_i[k % NUMERUS];
    }
    ns[I]  = _ns_per_elementum(t0, clock());
    t0     = clock();
    per (k = ZEPHYRUM; k < ITERATIONES; k++)
    {
        agmen_multiplica_modulo(ordo_a, ordo_b, exitus_i, NUMERUS, &m);
        consumptor = exitus_i[k % NUMERUS];
    }
    ns[II] = _ns_per_elementum(t0, clock());
    imprimere("\nmultiplica (ns/elementum)\n");
    imprimere("  congruentia %6.3f\n  scalaris    %6.3f\n  agmen       "
        "%6.3f   (%.1fx contra congruentia, %.1fx contra scalarem)\n",
        ns[ZEPHYRUM], ns[I], ns[II], ns[ZEPHYRUM] / ns[II], ns[I]
            / ns[II]);

    /* productum internum */
    t0 = clock();
    per (k = ZEPHYRUM; k < ITERATIONES; k++)
    {
        producta[ZEPHYRUM] = _productum_congruentia(ordo_a, ordo_b,
            NUMERUS,
            p);
        consumptor = producta[ZEPHYRUM];
    }
    ns[ZEPHYRUM]  = _ns_per_elementum(t0, clock());
    t0            = clock();
    per (k = ZEPHYRUM; k < ITERATIONES; k++)
    {
        producta[I] = agminis_productum_internum_modulo_scalaris(ordo_a,
            ordo_b, NUMERUS, &m);
        consumptor = producta[I];
    }
    ns[I]  = _ns_per_elementum(t0, clock());
    t0     = clock();
    per (k = ZEPHYRUM; k < ITERATIONES; k++)
    {
        producta[II] = agmen_productum_internum_modulo(ordo_a, ordo_b,
            NUMERUS, &m);
        consumptor = producta[II];
    }
    ns[II] = _ns_per_elementum(t0, clock());
    si (   producta[ZEPHYRUM] != producta[I]
        || producta[ZEPHYRUM] != producta[II])
    {
        imprimere("RECUSAT: producta interna inaequalia\n");
        redde I;
    }
    imprimere("\nproductum internum (ns/elementum)\n");
    imprimere("  congruentia %6.3f\n  scalaris    %6.3f\n  agmen       "
        "%6.3f   (%.1fx contra congruentia, %.1fx contra scalarem)\n",
        ns[ZEPHYRUM], ns[I], ns[II], ns[ZEPHYRUM] / ns[II], ns[I]
            / ns[II]);
    redde ZEPHYRUM;
}
