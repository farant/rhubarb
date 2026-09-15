/* probatio_partitio_aestimatio.c - Probationes iudicii retenti (T35 b)
 *
 * Tres crates manu computatae (planum T35 b, oratio-spec par. 7):
 * I   interactio quam catena avida non videt (columnae singulae IX,
 *     infimum earum XII);
 * II  vetum sortium (divisio lucrum VI sed sors C cadit VII -> V);
 * III paritates et mutabilitas - et DETECTOR CIRCULARITATIS:
 *     disciplina sortem retentam includens recti III pro I daret.
 */
#include "latina.h"
#include "piscina.h"
#include "partitio.h"
#include "partitio_aestimatio.h"
#include "credo.h"
#include <stdio.h>

interior vacuum
_sortem_probare (
    constans PartitioAestimatio* e,
                            i32  s,
                            i32  ordines,
                            i32  recti,
                            i32  basis,
                            i32  inaestimati)
{
    imprimere("    sors %d\n", (integer)s);
    CREDO_AEQUALIS_I32 (e->sortes[s].ordines, ordines);
    CREDO_AEQUALIS_I32 (e->sortes[s].recti, recti);
    CREDO_AEQUALIS_I32 (e->sortes[s].basis, basis);
    CREDO_AEQUALIS_I32 (e->sortes[s].inaestimati, inaestimati);
}

interior vacuum
_summam_probare (
    constans PartitioAestimatio* e,
                            i32  recti,
                            i32  basis,
                            i32  inaestimati,
                            s32  lucrum,
                            i32  puritas_intra)
{
    CREDO_AEQUALIS_I32 (e->summa.recti, recti);
    CREDO_AEQUALIS_I32 (e->summa.basis, basis);
    CREDO_AEQUALIS_I32 (e->summa.inaestimati, inaestimati);
    CREDO_AEQUALIS_S32 (e->lucrum, lucrum);
    CREDO_AEQUALIS_I32 (e->puritas_intra, puritas_intra);
}

s32
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_partitio_aestimatio",
        65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * CRATIS I - interactio (XII ordines, sortes A B C)
     *   ordo sortis quisque: pl versus S | sg versus S |
     *   sg prosa O | pl prosa S  (S = 0, O = I)
     *   una: disciplina S VI O II -> basis S, recti III
     *   numeri: pl S semper; sg in disciplina S II O II par
     *   -> basis S: recti IX; formae idem; infimum IV greges
     *   binis ordinibus disciplinae: recti XII, lucrum III
     *   limen III: greges infimi suffragium non ferunt
     * ================================================== */

    {
        constans i32 numeri_notae[XII] = {
            I, ZEPHYRUM, ZEPHYRUM, I,
            I, ZEPHYRUM, ZEPHYRUM, I,
            I, ZEPHYRUM, ZEPHYRUM, I };
        constans i32 formae_notae[XII] = {
            ZEPHYRUM, ZEPHYRUM, I, I,
            ZEPHYRUM, ZEPHYRUM, I, I,
            ZEPHYRUM, ZEPHYRUM, I, I };
        constans i32 auri_notae[XII] = {
            ZEPHYRUM, ZEPHYRUM, I, ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM, I, ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM, I, ZEPHYRUM };
        constans i32 sortium_notae[XII] = {
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            I, I, I, I,
            II, II, II, II };

                    Partitio* columna_numeri;
                    Partitio* columna_formae;
                    Partitio* aurum;
                    Partitio* sortes;
                    Partitio* una;
                    Partitio* infimum;
          PartitioAestimatio* e;
              PartitioCatena* c;
           constans Partitio* columnae[II];
                         i32  s;

        imprimere("\n--- Probans cratem I (interactio) ---\n");
        columna_numeri = partitio_ex_notis(piscina, XII, numeri_notae);
        columna_formae = partitio_ex_notis(piscina, XII, formae_notae);
        aurum = partitio_ex_notis(piscina, XII, auri_notae);
        sortes = partitio_ex_notis(piscina, XII, sortium_notae);
        una = partitio_una(piscina, XII);
        infimum        = partitio_infimum(piscina, columna_numeri,
            columna_formae);
        CREDO_NON_NIHIL (columna_numeri);
        CREDO_NON_NIHIL (columna_formae);
        CREDO_NON_NIHIL (aurum);
        CREDO_NON_NIHIL (sortes);
        CREDO_NON_NIHIL (una);
        CREDO_NON_NIHIL (infimum);

        imprimere("  una\n");
        e = partitio_aestimare(piscina, una, aurum, sortes, II);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            CREDO_AEQUALIS_I32 (e->numerus_sortium, III);
            CREDO_AEQUALIS_I32 (e->numerus_aureorum, II);
            CREDO_AEQUALIS_I32 (e->summa.ordines, XII);
            per (s = ZEPHYRUM; s < III; s++)
            {
                _sortem_probare(e, s, IV, III, III, ZEPHYRUM);
            }
            _summam_probare(e, IX, IX, ZEPHYRUM, (s32)ZEPHYRUM, DCCL);
            CREDO_AEQUALIS_I32 (e->greges_mutabiles, ZEPHYRUM);
            CREDO_FALSUM (e->inaestimabilis);
        }

        imprimere("  numeri (sg par in disciplina -> basis)\n");
        e = partitio_aestimare(piscina, columna_numeri, aurum, sortes,
            II);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            _summam_probare(e, IX, IX, ZEPHYRUM, (s32)ZEPHYRUM, DCCL);
            CREDO_AEQUALIS_I32 (e->greges_mutabiles, ZEPHYRUM);
            per (s = ZEPHYRUM; s < III; s++)
            {
                /* grex I (sg) suffragium basis S = 0 */
                CREDO_AEQUALIS_I32 (e->suffragia[I * III + s],
                    ZEPHYRUM);
            }
        }

        imprimere("  formae\n");
        e = partitio_aestimare(piscina, columna_formae, aurum, sortes,
            II);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            _summam_probare(e, IX, IX, ZEPHYRUM, (s32)ZEPHYRUM, DCCL);
        }

        imprimere("  infimum numeri et formae\n");
        e = partitio_aestimare(piscina, infimum, aurum, sortes, II);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            CREDO_AEQUALIS_I32 (e->numerus_gregum, IV);
            per (s = ZEPHYRUM; s < III; s++)
            {
                _sortem_probare(e, s, IV, IV, III, ZEPHYRUM);
            }
            _summam_probare(e, XII, IX, ZEPHYRUM, (s32)III, M);
            CREDO_FALSUM (e->inaestimabilis);
        }

        imprimere("  catena avida interactionem non videt\n");
        columnae[ZEPHYRUM]  = columna_numeri;
        columnae[I]         = columna_formae;
        c = partitio_catenam_struere(piscina, columnae, II, aurum,
            sortes, II, I, VERUM);
        CREDO_NON_NIHIL (c);
        si (c != NIHIL)
        {
            CREDO_AEQUALIS_I32 (c->numerus, ZEPHYRUM);
            CREDO_NON_NIHIL (c->initium);
            si (c->initium != NIHIL)
            {
                CREDO_AEQUALIS_I32 (c->initium->summa.recti, IX);
            }
        }
        c = partitio_catenam_struere(piscina, columnae, II, aurum,
            sortes, II, I, FALSUM);
        CREDO_NON_NIHIL (c);
        si (c != NIHIL)
        {
            CREDO_AEQUALIS_I32 (c->numerus, ZEPHYRUM);
        }

        imprimere("  limen III\n");
        e = partitio_aestimare(piscina, infimum, aurum, sortes, III);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            per (s = ZEPHYRUM; s < III; s++)
            {
                _sortem_probare(e, s, IV, III, III, IV);
            }
            _summam_probare(e, IX, IX, XII, (s32)ZEPHYRUM, M);
            CREDO_VERUM (e->inaestimabilis);
        }
        e = partitio_aestimare(piscina, columna_numeri, aurum, sortes,
            III);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            _summam_probare(e, IX, IX, ZEPHYRUM, (s32)ZEPHYRUM, DCCL);
            CREDO_FALSUM (e->inaestimabilis);
        }

        imprimere("  sortes ipsae ut candidata (inaestimabilis)\n");
        e = partitio_aestimare(piscina, sortes, aurum, sortes, II);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            CREDO_AEQUALIS_I32 (e->numerus_gregum, III);
            _summam_probare(e, IX, IX, XII, (s32)ZEPHYRUM, DCCL);
            CREDO_VERUM (e->inaestimabilis);
        }
    }


    /* ==================================================
     * CRATIS II - vetum sortium (XXVI ordines, O = 0, S = I)
     *   A: u O O O O, v S S S S S;  B idem;
     *   C: u S S S O, v S S S S
     *   retenta A: disciplina S XII O V -> basis S; u O V S III
     *   -> O; v S IX -> S; recti IX basis V; B idem;
     *   retenta C: u O VIII -> O (recti I), v S X (recti IV):
     *   recti V basis VII; summa XXIII / XVII, lucrum VI;
     *   puritas intra (IX + XIV) / XXVI = 884
     *   catena vetans: divisio cadit in C, una lucrum 0 -> 0
     *   catena sine veto: divisio accepta, deinde 0 -> I
     * ================================================== */

    {
        constans i32 divisionis_notae[XXVI] = {
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, I, I, I, I, I,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, I, I, I, I, I,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, I, I, I, I };
        constans i32 auri_notae[XXVI] = {
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, I, I, I, I, I,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, I, I, I, I, I,
            I, I, I, ZEPHYRUM, I, I, I, I };
        constans i32 sortium_notae[XXVI] = {
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            I, I, I, I, I, I, I, I, I,
            II, II, II, II, II, II, II, II };

                    Partitio* divisio;
                    Partitio* aurum;
                    Partitio* sortes;
                    Partitio* una;
          PartitioAestimatio* e;
              PartitioCatena* c;
           constans Partitio* columnae[II];
                         i32  s;

        imprimere("\n--- Probans cratem II (vetum sortium) ---\n");
        divisio  = partitio_ex_notis(piscina, XXVI, divisionis_notae);
        aurum    = partitio_ex_notis(piscina, XXVI, auri_notae);
        sortes   = partitio_ex_notis(piscina, XXVI, sortium_notae);
        una      = partitio_una(piscina, XXVI);
        CREDO_NON_NIHIL (divisio);
        CREDO_NON_NIHIL (aurum);
        CREDO_NON_NIHIL (sortes);
        CREDO_NON_NIHIL (una);

        imprimere("  divisio\n");
        e = partitio_aestimare(piscina, divisio, aurum, sortes, II);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            _sortem_probare(e, ZEPHYRUM, IX, IX, V, ZEPHYRUM);
            _sortem_probare(e, I, IX, IX, V, ZEPHYRUM);
            _sortem_probare(e, II, VIII, V, VII, ZEPHYRUM);
            _summam_probare(e, XXIII, XVII, ZEPHYRUM, (s32)VI,
                884);
            CREDO_AEQUALIS_I32 (e->greges_mutabiles, ZEPHYRUM);
            per (s = ZEPHYRUM; s < III; s++)
            {
                CREDO_AEQUALIS_I32 (e->suffragia[ZEPHYRUM * III + s],
                    ZEPHYRUM);
                CREDO_AEQUALIS_I32 (e->suffragia[I * III + s], I);
            }
        }

        imprimere("  una\n");
        e = partitio_aestimare(piscina, una, aurum, sortes, II);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            _summam_probare(e, XVII, XVII, ZEPHYRUM, (s32)ZEPHYRUM,
                653);
        }

        columnae[ZEPHYRUM]  = divisio;
        columnae[I]         = una;
        imprimere("  catena vetans\n");
        c = partitio_catenam_struere(piscina, columnae, II, aurum,
            sortes, II, I, VERUM);
        CREDO_NON_NIHIL (c);
        si (c != NIHIL)
        {
            CREDO_AEQUALIS_I32 (c->numerus, ZEPHYRUM);
            CREDO_NON_NIHIL (c->initium);
            si (c->initium != NIHIL)
            {
                CREDO_AEQUALIS_I32 (c->initium->summa.recti, XVII);
            }
        }
        imprimere("  catena sine veto\n");
        c = partitio_catenam_struere(piscina, columnae, II, aurum,
            sortes, II, I, FALSUM);
        CREDO_NON_NIHIL (c);
        si (c != NIHIL)
        {
            CREDO_AEQUALIS_I32 (c->numerus, I);
            si (c->numerus == I)
            {
                CREDO_AEQUALIS_I32 (c->columnae[ZEPHYRUM], ZEPHYRUM);
                CREDO_AEQUALIS_I32 (
                    c->aestimationes[ZEPHYRUM]->summa.recti, XXIII);
                CREDO_AEQUALIS_S32 (c->aestimationes[ZEPHYRUM]->lucrum,
                    (s32)VI);
                CREDO_VERUM (partitio_aequalis(c->partes[ZEPHYRUM],
                    divisio));
            }
        }
    }


    /* ==================================================
     * CRATIS III - paritates, mutabilitas, circularitas
     *   VI ordines, grex unus; aurum A: S S | B: O O | C: S O
     *   retenta A: disciplina O III S I -> O; recti 0 basis 0
     *   retenta B: S III O I -> S; recti 0 basis 0
     *   retenta C: S II O II par -> basis S (index minimus),
     *   grex par -> basis S; recti I basis I
     *   suffragia O S S -> mutabilis I
     *   CIRCULARITAS: disciplina = totales S III O III par in
     *   omni sorte -> S ubique -> recti II + 0 + I = III
     * ================================================== */

    {
        constans i32 auri_notae[VI] = {
            ZEPHYRUM, ZEPHYRUM, I, I, ZEPHYRUM, I };
        constans i32 sortium_notae[VI] = {
            ZEPHYRUM, ZEPHYRUM, I, I, II, II };

                    Partitio* aurum;
                    Partitio* sortes;
                    Partitio* una;
          PartitioAestimatio* e;

        imprimere("\n--- Probans cratem III (paritates) ---\n");
        aurum   = partitio_ex_notis(piscina, VI, auri_notae);
        sortes  = partitio_ex_notis(piscina, VI, sortium_notae);
        una     = partitio_una(piscina, VI);
        CREDO_NON_NIHIL (aurum);
        CREDO_NON_NIHIL (sortes);
        CREDO_NON_NIHIL (una);
        e = partitio_aestimare(piscina, una, aurum, sortes, II);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            _sortem_probare(e, ZEPHYRUM, II, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM);
            _sortem_probare(e, I, II, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
            _sortem_probare(e, II, II, I, I, ZEPHYRUM);
            _summam_probare(e, I, I, ZEPHYRUM, (s32)ZEPHYRUM, D);
            CREDO_AEQUALIS_I32 (e->greges_mutabiles, I);
            CREDO_AEQUALIS_I32 (e->suffragia[ZEPHYRUM], I);
            CREDO_AEQUALIS_I32 (e->suffragia[I], ZEPHYRUM);
            CREDO_AEQUALIS_I32 (e->suffragia[II], ZEPHYRUM);
        }
    }


    /* ==================================================
     * RECUSATIONES: ordines inaequales, columna NIHIL
     * ================================================== */

    {
         constans Partitio* nulla[I] = { NIHIL };
                  Partitio* a;
                  Partitio* b;

        imprimere("\n--- Probans recusationes ---\n");
        a = partitio_una(piscina, IV);
        b = partitio_una(piscina, III);
        CREDO_NON_NIHIL (a);
        CREDO_NON_NIHIL (b);
        CREDO_NIHIL (partitio_aestimare(piscina, a, b, a, II));
        CREDO_NIHIL (partitio_aestimare(piscina, a, a, NIHIL, II));
        CREDO_NIHIL (partitio_catenam_struere(piscina, nulla, I, a, a,
            II, I, VERUM));
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
