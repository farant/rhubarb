/* probatio_partitio.c - Probationes algebrae partitionum (T35 a)
 *
 * Casus manu computati (oratio-spec par. 7 "Design - T35", planum
 * project-specs/partitio-reticulum-plan.md), leges super familias
 * fortuitas seminatas, latitudo per Dilworth contra vim brutam
 * (methodus scrutinii knotapel, k <= XII).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "partitio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* generator congruentialis linearis - seminatus, deterministicus */
interior i32
_fortuitum (
    i32* status)
{
    *status = *status * 1103515245U + 12345U;
    redde (*status >> XVI) & 0x7FFFU;
}

/* partitio fortuita: notae in 0..modulus-1, modulus in 1..4 */
interior Partitio*
_partitio_fortuita (
     Piscina* piscina,
         i32* status,
         i32  numerus)
{
    i32 notae[XVI];
    i32 modulus;
    i32 i;

    modulus = I + _fortuitum(status) % IV;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        notae[i] = _fortuitum(status) % modulus;
    }
    redde partitio_ex_notis(piscina, numerus, notae);
}

/* antichain maxima inter unicas per vim brutam (scrutinium) */
interior i32
_latitudo_bruta (
    constans PartitioReticulum* r)
{
    i32 k       = r->numerus;
    i32 optima  = ZEPHYRUM;
    i32 larva;

    per (larva = I; larva < ((i32)I << k); larva++)
    {
        i32 i;
        i32 j;
        i32 membra  = ZEPHYRUM;
        b32 bona    = VERUM;

        per (i = ZEPHYRUM; i < k && bona; i++)
        {
            si ((larva & ((i32)I << i)) == ZEPHYRUM)
            {
                perge;
            }
            si (r->geminus[i] != i)
            {
                bona = FALSUM;
                frange;
            }
            membra = membra + I;
            per (j = i + I; j < k; j++)
            {
                si ((larva & ((i32)I << j)) == ZEPHYRUM)
                {
                    perge;
                }
                si (r->ordo[i * k + j] || r->ordo[j * k + i])
                {
                    bona = FALSUM;
                    frange;
                }
            }
        }
        si (bona && membra > optima)
        {
            optima = membra;
        }
    }
    redde optima;
}

s32
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_partitio", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * PROBARE: partitio_ex_notis - numeratio canonica
     * ================================================== */

    {
        constans i32  notae[IV] = { VII, III, VII, IX };
            Partitio* p;

        imprimere("\n--- Probans partitio_ex_notis"
            " (numeratio canonica) ---\n");
        p = partitio_ex_notis(piscina, IV, notae);
        CREDO_NON_NIHIL (p);
        si (p != NIHIL)
        {
            CREDO_AEQUALIS_I32 (p->numerus, IV);
            CREDO_AEQUALIS_I32 (p->numerus_gregum, III);
            CREDO_AEQUALIS_I32 (p->grex[ZEPHYRUM], ZEPHYRUM);
            CREDO_AEQUALIS_I32 (p->grex[I], I);
            CREDO_AEQUALIS_I32 (p->grex[II], ZEPHYRUM);
            CREDO_AEQUALIS_I32 (p->grex[III], II);
            CREDO_AEQUALIS_I32 (p->magnitudines[ZEPHYRUM], II);
            CREDO_AEQUALIS_I32 (p->magnitudines[I], I);
            CREDO_AEQUALIS_I32 (p->magnitudines[II], I);
            CREDO_AEQUALIS_I32 (p->primus[ZEPHYRUM], ZEPHYRUM);
            CREDO_AEQUALIS_I32 (p->primus[I], I);
            CREDO_AEQUALIS_I32 (p->primus[II], III);
        }
    }


    /* ==================================================
     * PROBARE: partitio_ex_chordis
     * ================================================== */

    {
          chorda  valores[IV];
        Partitio* p;

        imprimere("\n--- Probans partitio_ex_chordis ---\n");
        valores[ZEPHYRUM]  = chorda_ex_literis("nom", piscina);
        valores[I]         = chorda_ex_literis("acc", piscina);
        valores[II]        = chorda_ex_literis("nom", piscina);
        valores[III]       = chorda_ex_literis("gen", piscina);
        p                  = partitio_ex_chordis(piscina, IV, valores);
        CREDO_NON_NIHIL (p);
        si (p != NIHIL)
        {
            CREDO_AEQUALIS_I32 (p->numerus_gregum, III);
            CREDO_AEQUALIS_I32 (p->grex[ZEPHYRUM], ZEPHYRUM);
            CREDO_AEQUALIS_I32 (p->grex[I], I);
            CREDO_AEQUALIS_I32 (p->grex[II], ZEPHYRUM);
            CREDO_AEQUALIS_I32 (p->grex[III], II);
        }
    }


    /* ==================================================
     * PROBARE: familia manu computata (n = IV)
     *   P0 discreta, P1 {0,0,1,1}, P2 {0,1,0,1}, P3 {0,0,0,1},
     *   P4 una, P5 {5,5,9,9} == P1
     *   ordo stricte inter unicas: 0<1 0<2 0<3 0<4 1<4 2<4 3<4
     *   tegit: (0,1) (0,2) (0,3) (1,4) (2,4) (3,4) - non (0,4)
     *   altitudo II (0<1<4); latitudo III ({P1,P2,P3});
     *   congruentia maxima II (0-1, 1-4), unicae V - II = III
     * ================================================== */

    {
        constans i32 n1[IV]      = { ZEPHYRUM, ZEPHYRUM, I, I };
        constans i32 n2[IV]      = { ZEPHYRUM, I, ZEPHYRUM, I };
        constans i32 n3[IV]      = { ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, I };
        constans i32 n5[IV]      = { V, V, IX, IX };
        constans i32 infimi[IV]  = { ZEPHYRUM, ZEPHYRUM, I, II };

         constans Partitio* partes[VI];
                  Partitio* m;
         PartitioReticulum* r;
                       i32  i;
                       i32  j;

        imprimere("\n--- Probans familiam manu computatam ---\n");
        partes[ZEPHYRUM]  = partitio_discreta(piscina, IV);
        partes[I]         = partitio_ex_notis(piscina, IV, n1);
        partes[II]        = partitio_ex_notis(piscina, IV, n2);
        partes[III]       = partitio_ex_notis(piscina, IV, n3);
        partes[IV]        = partitio_una(piscina, IV);
        partes[V]         = partitio_ex_notis(piscina, IV, n5);
        per (i = ZEPHYRUM; i < VI; i++)
        {
            CREDO_NON_NIHIL (partes[i]);
        }

        CREDO_VERUM  (partitio_aequalis(partes[I], partes[V]));
        CREDO_VERUM  (partitio_subtilior(partes[I], partes[V]));
        CREDO_VERUM  (partitio_subtilior(partes[V], partes[I]));
        CREDO_FALSUM (partitio_subtilior(partes[I], partes[II]));
        CREDO_FALSUM (partitio_subtilior(partes[II], partes[I]));
        CREDO_FALSUM (partitio_subtilior(partes[I], partes[III]));
        CREDO_FALSUM (partitio_subtilior(partes[III], partes[I]));
        CREDO_FALSUM (partitio_aequalis(partes[I], partes[II]));
        per (i = ZEPHYRUM; i < VI; i++)
        {
            CREDO_VERUM (partitio_subtilior(partes[ZEPHYRUM],
                partes[i]));
            CREDO_VERUM (partitio_subtilior(partes[i], partes[IV]));
        }

        m = partitio_infimum(piscina, partes[I], partes[II]);
        CREDO_NON_NIHIL (m);
        CREDO_VERUM (partitio_aequalis(m, partes[ZEPHYRUM]));
        m = partitio_infimum(piscina, partes[I], partes[III]);
        CREDO_NON_NIHIL (m);
        si (m != NIHIL)
        {
            CREDO_AEQUALIS_I32 (m->numerus_gregum, III);
            per (i = ZEPHYRUM; i < IV; i++)
            {
                CREDO_AEQUALIS_I32 (m->grex[i], infimi[i]);
            }
        }
        m = partitio_supremum(piscina, partes[I], partes[II]);
        CREDO_NON_NIHIL (m);
        CREDO_VERUM (partitio_aequalis(m, partes[IV]));
        m = partitio_supremum(piscina, partes[I], partes[III]);
        CREDO_NON_NIHIL (m);
        CREDO_VERUM (partitio_aequalis(m, partes[IV]));

        r = partitio_reticulum_struere(piscina, partes, VI);
        CREDO_NON_NIHIL (r);
        si (r != NIHIL)
        {
            constans i32 geminus[VI] = { ZEPHYRUM, I, II, III, IV, I };
                     b32 tegit[XXXVI];

            memset(tegit, ZEPHYRUM, magnitudo(tegit));
            tegit[ZEPHYRUM * VI + I]    = VERUM;
            tegit[ZEPHYRUM * VI + II]   = VERUM;
            tegit[ZEPHYRUM * VI + III]  = VERUM;
            tegit[I * VI + IV]          = VERUM;
            tegit[II * VI + IV]         = VERUM;
            tegit[III * VI + IV]        = VERUM;

            CREDO_AEQUALIS_I32 (r->numerus, VI);
            per (i = ZEPHYRUM; i < VI; i++)
            {
                CREDO_AEQUALIS_I32 (r->geminus[i], geminus[i]);
                per (j = ZEPHYRUM; j < VI; j++)
                {
                    CREDO_AEQUALIS_I32 ((i32)(r->tegit[i * VI + j]
                        != FALSUM),
                        (i32)(tegit[i * VI + j] != FALSUM));
                    CREDO_AEQUALIS_I32 ((i32)(r->ordo[i * VI + j]
                        != FALSUM),
                        (i32)(i != j && partitio_subtilior(partes[i],
                            partes[j])));
                }
            }
            CREDO_AEQUALIS_I32 (r->altitudo, II);
            CREDO_AEQUALIS_I32 (r->latitudo, III);
            CREDO_AEQUALIS_I32 (_latitudo_bruta(r), III);
        }
    }


    /* ==================================================
     * PROBARE: numeri ordinum inaequales - recusatio
     * ================================================== */

    {
        constans i32  n1[IV] = { ZEPHYRUM, ZEPHYRUM, I, I };
            Partitio* a;
            Partitio* b;

        imprimere("\n--- Probans recusationem inaequalium ---\n");
        a = partitio_ex_notis(piscina, IV, n1);
        b = partitio_una(piscina, III);
        CREDO_NON_NIHIL (a);
        CREDO_NON_NIHIL (b);
        CREDO_FALSUM (partitio_subtilior(a, b));
        CREDO_FALSUM (partitio_aequalis(a, b));
        CREDO_NIHIL  (partitio_infimum(piscina, a, b));
        CREDO_NIHIL  (partitio_supremum(piscina, a, b));
        CREDO_NIHIL  (partitio_ex_notis(piscina, IV, NIHIL));
    }


    /* ==================================================
     * PROBARE: leges super familias fortuitas (CC, k = II..XII,
     * n = VII): infimum subtilius utraque, utraque subtiliores
     * supremo, idempotentia, commutatio, ordo partialis
     * (reflexivus, antisymmetricus, transitivus), ordo reticuli ==
     * subtilior, latitudo Dilworth == vis bruta
     * ================================================== */

    {
        i32 status                  = XLII;
        i32 familia;
        i32 paria                    = ZEPHYRUM;
        i32 violationes_infimi       = ZEPHYRUM;
        i32 violationes_supremi      = ZEPHYRUM;
        i32 violationes_ordinis      = ZEPHYRUM;
        i32 violationes_reticuli     = ZEPHYRUM;
        i32 violationes_latitudinis  = ZEPHYRUM;

        imprimere("\n--- Probans leges super familias fortuitas ---\n");
        per (familia = ZEPHYRUM; familia < CC; familia++)
        {
             constans Partitio* partes[XII];
                PiscinaNotatio  nota;
             PartitioReticulum* r;
                           i32  k;
                           i32  i;
                           i32  j;
                           i32  m;

            nota  = piscina_notare(piscina);
            k     = II + _fortuitum(&status) % XI;
            per (i = ZEPHYRUM; i < k; i++)
            {
                partes[i] = _partitio_fortuita(piscina, &status, VII);
            }
            per (i = ZEPHYRUM; i < k; i++)
            {
                constans Partitio* a = partes[i];

                si (   !partitio_subtilior(a, a)
                    || !partitio_aequalis(partitio_infimum(piscina, a,
                    a), a)
                    || !partitio_aequalis(partitio_supremum(piscina, a,
                    a), a))
                {
                    violationes_ordinis = violationes_ordinis + I;
                }
                per (j = ZEPHYRUM; j < k; j++)
                {
                    constans Partitio* b = partes[j];
                             Partitio* in = partitio_infimum(piscina, a,
                                 b);
                             Partitio* su = partitio_supremum(piscina,
                                 a, b);

                    paria = paria + I;
                    si (   in == NIHIL
                        || !partitio_subtilior(in, a)
                        || !partitio_subtilior(in, b)
                        || !partitio_aequalis(in,
                            partitio_infimum(piscina, b, a)))
                    {
                        violationes_infimi = violationes_infimi + I;
                    }
                    si (   su == NIHIL
                        || !partitio_subtilior(a, su)
                        || !partitio_subtilior(b, su)
                        || !partitio_aequalis(su,
                            partitio_supremum(piscina, b, a)))
                    {
                        violationes_supremi = violationes_supremi + I;
                    }
                    si (   partitio_subtilior(a, b)
                        && partitio_subtilior(b, a)
                        && !partitio_aequalis(a, b))
                    {
                        violationes_ordinis = violationes_ordinis + I;
                    }
                    per (m = ZEPHYRUM; m < k; m++)
                    {
                        si (   partitio_subtilior(a, b)
                            && partitio_subtilior(b, partes[m])
                            && !partitio_subtilior(a, partes[m]))
                        {
                            violationes_ordinis = violationes_ordinis
                                + I;
                        }
                    }
                }
            }
            r = partitio_reticulum_struere(piscina, partes, k);
            si (r == NIHIL)
            {
                violationes_reticuli = violationes_reticuli + I;
            }
            alioquin
            {
                per (i = ZEPHYRUM; i < k; i++)
                {
                    per (j = ZEPHYRUM; j < k; j++)
                    {
                        b32 exspectatum = (b32)(i != j
                            && partitio_subtilior(partes[i],
                            partes[j]));

                        si ((r->ordo[i * k + j] != FALSUM)
                            != exspectatum)
                        {
                            violationes_reticuli = violationes_reticuli
                                + I;
                        }
                    }
                }
                si (r->latitudo != _latitudo_bruta(r))
                {
                    violationes_latitudinis = violationes_latitudinis
                        + I;
                }
            }
            piscina_reficere(piscina, nota);
        }
        imprimere("  paria probata %d\n", (integer)paria);
        CREDO_MAIOR_I32    (paria, M);
        CREDO_AEQUALIS_I32 (violationes_infimi, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (violationes_supremi, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (violationes_ordinis, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (violationes_reticuli, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (violationes_latitudinis, ZEPHYRUM);
    }


    /* ==================================================
     * Compendium
     * ================================================== */

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
