/* probatio_agmen.c - Probationes arithmeticae per agmina
 *
 * Tria oracula per elementum: functio publica (a compilatore
 * vectorizata), via scalaris relata (agmen_interna.h, non vectorizata),
 * congruentia.h (residuum per '%', algorithmus alienus - non
 * Montgomery). Vectorizatio ipsa: porta agmen-vectorizatio. Longitudines 0..17 (omnes
 * caudae lanarum IV), 64, 1000, 1024; ordines casu et extremi (0, p-1,
 * mixti); in loco (exitus == a, exitus == b); custos post finem
 * intacta; moduli 3 .. 2^31-1 (etiam impares compositi). Productum
 * internum longum: (p-1)^2 = 1, ergo summa = numerus mod p. Vide
 * lib/agmen.worklog.md.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "sors.h"
#include "congruentia.h"
#include "agmen.h"
#include "agmen_interna.h"
#include <stdio.h>

#define MAXIMUS     1024
#define LONGUS      100000
#define CUSTOS  0xDEADBEEFU

interior Piscina* piscina;

interior i32 ordo_a[MAXIMUS + I];
interior i32 ordo_b[MAXIMUS + I];
interior i32 exitus_publicus[MAXIMUS + I];
interior i32 exitus_scalaris[MAXIMUS + I];
interior i32 exitus_oraculi[MAXIMUS + I];
interior i32 ordo_longus[LONGUS];

/* modus: 0 casu, 1 omnia 0, 2 omnia p-1, 3 extrema mixta */
interior vacuum
_implere (
     i32* ordo,
     i32  numerus,
     i32  p,
     s32  modus,
    Sors* s)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        commutatio (modus)
        {
            casus ZEPHYRUM:
                ordo[i] = sors_intra(s, p);
                frange;
            casus I:
                ordo[i] = ZEPHYRUM;
                frange;
            casus II:
                ordo[i] = p - I;
                frange;
            ordinarius:
            {
                i32 extrema[IV];

                extrema[ZEPHYRUM] = ZEPHYRUM;
                extrema[I] = I;
                extrema[II] = p - I;
                extrema[III] = p - II;
                ordo[i] = extrema[(i + sors_intra(s, IV)) % IV];
                frange;
            }
        }
    }
}

/* tria ordines aequales, et custodes post finem intacti */
interior b32
_comparare (
    constans character* operatio,
                   i32  p,
                   i32  numerus)
{
    i32 i;

    si (   exitus_publicus[numerus] != CUSTOS
        || exitus_scalaris[numerus] != CUSTOS)
    {
        imprimere("  FRACTA %s p=%u n=%u: custos mutatus\n", operatio,
            p, numerus);
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (   exitus_publicus[i] != exitus_oraculi[i]
            || exitus_scalaris[i] != exitus_oraculi[i])
        {
            imprimere("  FRACTA %s p=%u n=%u i=%u: publicus %u scalaris %u "
                "oraculum %u (a=%u b=%u)\n", operatio, p, numerus, i,
                exitus_publicus[i], exitus_scalaris[i],
                exitus_oraculi[i],
                ordo_a[i], ordo_b[i]);
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_custodes_ponere (
    i32 numerus)
{
    exitus_publicus[numerus] = CUSTOS;
    exitus_scalaris[numerus] = CUSTOS;
}

/* omnes operationes pro uno modulo, una longitudine, uno modo */
interior b32
_probare (
    constans AgmenModulus* m,
                      i32  numerus,
                      s32  modus,
                     Sors* s)
{
    i32 p     = m->p;
    b32 bene  = VERUM;
    i32 i;
    i32 k;

    _implere(ordo_a, numerus, p, modus, s);
    _implere(ordo_b, numerus, p, (modus + I) % IV, s);

    /* adde */
    _custodes_ponere(numerus);
    agmen_adde_modulo(ordo_a, ordo_b, exitus_publicus, numerus, m);
    agminis_adde_modulo_scalaris(ordo_a, ordo_b, exitus_scalaris,
        numerus,
        m);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        exitus_oraculi[i] = congruentia_adde(ordo_a[i], ordo_b[i], p);
    }
    bene = bene && _comparare("adde", p, numerus);

    /* subtrahe */
    _custodes_ponere(numerus);
    agmen_subtrahe_modulo(ordo_a, ordo_b, exitus_publicus, numerus, m);
    agminis_subtrahe_modulo_scalaris(ordo_a, ordo_b, exitus_scalaris,
        numerus, m);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        exitus_oraculi[i] = congruentia_subtrahe(ordo_a[i], ordo_b[i],
            p);
    }
    bene = bene && _comparare("subtrahe", p, numerus);

    /* multiplica */
    _custodes_ponere(numerus);
    agmen_multiplica_modulo(ordo_a, ordo_b, exitus_publicus, numerus,
        m);
    agminis_multiplica_modulo_scalaris(ordo_a, ordo_b, exitus_scalaris,
        numerus, m);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        exitus_oraculi[i] = congruentia_multiplica(ordo_a[i], ordo_b[i],
            p);
    }
    bene = bene && _comparare("multiplica", p, numerus);

    /* in loco: exitus == a, deinde exitus == b (multiplica et subtrahe:
     * ordo argumentorum refert) */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        exitus_publicus[i] = ordo_a[i];
        exitus_scalaris[i] = ordo_b[i];
    }
    _custodes_ponere(numerus);
    agmen_subtrahe_modulo(exitus_publicus, ordo_b, exitus_publicus,
        numerus, m);
    agmen_multiplica_modulo(ordo_a, exitus_scalaris, exitus_scalaris,
        numerus, m);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (   exitus_publicus[i]
                != congruentia_subtrahe(ordo_a[i], ordo_b[i], p)
            || exitus_scalaris[i]
                != congruentia_multiplica(ordo_a[i], ordo_b[i], p))
        {
            imprimere("  FRACTA in loco p=%u n=%u i=%u\n", p, numerus,
                i);
            bene = FALSUM;
            frange;
        }
    }

    /* multiplica_adde: c casu et extrema */
    per (k = ZEPHYRUM; k < IV; k++)
    {
        i32 c = k == ZEPHYRUM ? sors_intra(s, p)
            : k == I ? ZEPHYRUM : k == II ? I : p - I;

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            exitus_publicus[i] = ordo_b[i];
            exitus_scalaris[i] = ordo_b[i];
            exitus_oraculi[i]  = congruentia_adde(ordo_b[i],
                congruentia_multiplica(c, ordo_a[i], p), p);
        }
        _custodes_ponere(numerus);
        agmen_multiplica_adde_modulo(c, ordo_a, exitus_publicus,
            numerus, m);
        agminis_multiplica_adde_modulo_scalaris(c, ordo_a,
            exitus_scalaris,
            numerus, m);
        bene = bene && _comparare("multiplica_adde", p, numerus);
    }

    /* productum internum */
    {
        i32 oraculum = ZEPHYRUM;
        i32 publicus = agmen_productum_internum_modulo(ordo_a, ordo_b,
            numerus, m);
        i32 scalaris =
            agminis_productum_internum_modulo_scalaris(ordo_a,
            ordo_b, numerus, m);

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            oraculum = congruentia_adde(oraculum,
                congruentia_multiplica(ordo_a[i], ordo_b[i], p), p);
        }
        si (publicus != oraculum || scalaris != oraculum)
        {
            imprimere("  FRACTA productum p=%u n=%u: publicus %u scalaris "
                "%u oraculum %u\n", p, numerus, publicus, scalaris,
                oraculum);
            bene = FALSUM;
        }
    }
    redde bene;
}

s32 principale (vacuum)
{
    piscina = piscina_generare_dynamicum("probatio_agmen", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * MODULUS
     * ================================================== */

    {
        constans i32 recusati[] = { 0U, 1U, 2U, 4U, 1000000U,
            0x80000000U,
            0x80000001U, 0xFFFFFFFFU, 0xFFFFFFFBU };
        constans i32 accepti[]  = { 3U, 5U, 9U, 65537U, 1000000007U,
            2147483629U, 2147483645U, 2147483647U };
        i32 k;

        imprimere("\n--- Probans modulum ---\n");
        per (k = ZEPHYRUM; k < magnitudo(recusati)
            / magnitudo(recusati[0]);
            k++)
        {
            AgmenModulus m;

            m.p          = CUSTOS;
            m.p_inversa  = CUSTOS;
            m.r2         = CUSTOS;
            CREDO_FALSUM (agmen_modulus(recusati[k], &m));
            CREDO_AEQUALIS_I32 (m.p, CUSTOS);
            CREDO_AEQUALIS_I32 (m.r2, CUSTOS);
        }
        per (k = ZEPHYRUM; k < magnitudo(accepti)
            / magnitudo(accepti[0]);
            k++)
        {
            AgmenModulus m;
                     i32 p = accepti[k];

            CREDO_VERUM (agmen_modulus(p, &m));
            CREDO_AEQUALIS_I32 (m.p, p);
            /* p p' = -1 mod 2^32 */
            CREDO_AEQUALIS_I32 (p * m.p_inversa + I, ZEPHYRUM);
            /* r2 = 2^64 mod p, per potentiam (algorithmus alius) */
            CREDO_AEQUALIS_I32 (m.r2, congruentia_potentia(II, LXIV,
                p));
        }
        CREDO_AEQUALIS_I32 (congruentia_primus_infra(0x80000000U),
            2147483647U);
    }


    /* ==================================================
     * OPERATIONES CONTRA ORACULA
     * ================================================== */

    {
        constans i32 moduli[] = { 3U, 5U, 7U, 9U, 15U, 65537U,
            1000000007U,
            2147483629U, 2147483645U, 2147483647U };
        constans i32 longitudines_magnae[] = { 64U, 1000U, MAXIMUS };
                Sors s;
                 i32 k;
                 b32 bene           = VERUM;
                 i32 casus_probati  = ZEPHYRUM;

        imprimere("\n--- Probans operationes contra oracula ---\n");
        sors_seminare(&s, 2026ULL, III);
        per (k = ZEPHYRUM; k < magnitudo(moduli) / magnitudo(moduli[0]);
            k++)
        {
            AgmenModulus m;
                     i32 numerus;
                     s32 modus;
                     i32 j;

            CREDO_VERUM (agmen_modulus(moduli[k], &m));
            per (modus = ZEPHYRUM; modus < IV; modus++)
            {
                per (numerus = ZEPHYRUM; numerus <= XVII; numerus++)
                {
                    bene = bene && _probare(&m, numerus, modus, &s);
                    casus_probati++;
                }
                per (j = ZEPHYRUM; j < III; j++)
                {
                    bene = bene
                        && _probare(&m, longitudines_magnae[j], modus,
                        &s);
                    casus_probati++;
                }
            }
        }
        imprimere("  casus: %u\n", casus_probati);
        CREDO_VERUM (bene);
    }


    /* ==================================================
     * PRODUCTUM INTERNUM LONGUM
     * ================================================== */

    {
        constans i32 moduli[] = { 3U, 1000000007U, 2147483647U };
                 i32 k;

        imprimere("\n--- Probans productum internum longum ---\n");
        per (k = ZEPHYRUM; k < magnitudo(moduli) / magnitudo(moduli[0]);
            k++)
        {
            AgmenModulus m;
                     i32 p = moduli[k];
                     i32 i;

            CREDO_VERUM (agmen_modulus(p, &m));
            per (i = ZEPHYRUM; i < LONGUS; i++)
            {
                ordo_longus[i] = p - I;
            }
            /* (p-1)^2 = 1 mod p: summa = numerus mod p */
            CREDO_AEQUALIS_I32 (agmen_productum_internum_modulo(ordo_longus,
                ordo_longus, LONGUS, &m), LONGUS % p);
            CREDO_AEQUALIS_I32 (agminis_productum_internum_modulo_scalaris(
                ordo_longus, ordo_longus, LONGUS, &m), LONGUS % p);
            CREDO_AEQUALIS_I32 (agmen_productum_internum_modulo(ordo_longus,
                ordo_longus, LONGUS - III, &m), (LONGUS - III) % p);
        }
    }

    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
