/* probatio_partitio_aestimatio.c - Probationes iudicii retenti (T35 b)
 *
 * Tres crates manu computatae (planum T35 b, oratio-spec par. 7):
 * I   interactio quam catena avida non videt (columnae singulae IX,
 *     infimum earum XII);
 * II  vetum sortium (divisio lucrum VI sed sors C cadit VII -> V);
 * III paritates et mutabilitas - et DETECTOR CIRCULARITATIS:
 *     disciplina sortem retentam includens recti III pro I daret;
 * IV  grex CAPTIVUS (T36 a, decisio LV): ordines omnes intra sortem
 *     unam - limine II columna inaestimabilis, limine IV non.
 * T36 a: habitus gregum (coactus | ordinatus | apertus; par disciplinae
 * = apertus, suffragium basis est non gregis), margo, retenti et recti
 * per gregem et sortem, optiones catenae cum semine.
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
       PartitioCatenaOptiones  o;
                          i32  s;
                          i32  g;

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
            /* T36 a: S VI O II per sortem retentam -> margo IV,
             * non unanimis -> ordinatus; captivi nulli */
            CREDO_AEQUALIS_I32 (e->habitus[ZEPHYRUM],
                (i32)PARTITIO_HABITUS_ORDINATUS);
            CREDO_AEQUALIS_I32 (e->margo[ZEPHYRUM], IV);
            CREDO_AEQUALIS_I32 (e->greges_captivi, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (e->captivus[ZEPHYRUM], III);
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
                /* T36 a: pl S IV unanimis (margo IV), sg par (0) */
                CREDO_AEQUALIS_I32 (e->margo[ZEPHYRUM * III + s], IV);
                CREDO_AEQUALIS_I32 (e->margo[I * III + s], ZEPHYRUM);
            }
            CREDO_AEQUALIS_I32 (e->habitus[ZEPHYRUM],
                (i32)PARTITIO_HABITUS_COACTUS);
            CREDO_AEQUALIS_I32 (e->habitus[I],
                (i32)PARTITIO_HABITUS_APERTUS);
        }

        imprimere("  formae\n");
        e = partitio_aestimare(piscina, columna_formae, aurum, sortes,
            II);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            _summam_probare(e, IX, IX, ZEPHYRUM, (s32)ZEPHYRUM, DCCL);
            /* versus S S unanimis, prosa O S par */
            CREDO_AEQUALIS_I32 (e->habitus[ZEPHYRUM],
                (i32)PARTITIO_HABITUS_COACTUS);
            CREDO_AEQUALIS_I32 (e->habitus[I],
                (i32)PARTITIO_HABITUS_APERTUS);
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
            /* T36 a: grex quisque ordinem unum per sortem - disciplina
             * II unanimis -> coactus, margo II, retenti I recti I */
            per (g = ZEPHYRUM; g < IV; g++)
            {
                CREDO_AEQUALIS_I32 (e->habitus[g],
                    (i32)PARTITIO_HABITUS_COACTUS);
                CREDO_FALSUM (e->mutabilis[g]);
                per (s = ZEPHYRUM; s < III; s++)
                {
                    CREDO_AEQUALIS_I32 (e->margo[g * III + s], II);
                    CREDO_AEQUALIS_I32 (e->retenti[g * III + s], I);
                    CREDO_AEQUALIS_I32 (e->recti_gregum[g * III + s],
                        I);
                }
            }
        }

        imprimere("  catena avida interactionem non videt\n");
        columnae[ZEPHYRUM]  = columna_numeri;
        columnae[I]         = columna_formae;
        partitio_catena_optiones_initium(&o);
        o.limen           = II;
        o.lucrum_minimum  = I;
        c = partitio_catenam_struere(piscina, columnae, II, aurum,
            sortes, &o);
        CREDO_NON_NIHIL (c);
        si (c != NIHIL)
        {
            CREDO_AEQUALIS_I32 (c->numerus, ZEPHYRUM);
            CREDO_NON_NIHIL (c->initium);
            CREDO_NON_NIHIL (c->semen);
            si (c->initium != NIHIL)
            {
                CREDO_AEQUALIS_I32 (c->initium->summa.recti, IX);
            }
            si (c->semen != NIHIL)
            {
                CREDO_VERUM (partitio_aequalis(c->semen, una));
            }
        }
        o.sortes_vetant = FALSUM;
        c = partitio_catenam_struere(piscina, columnae, II, aurum,
            sortes, &o);
        CREDO_NON_NIHIL (c);
        si (c != NIHIL)
        {
            CREDO_AEQUALIS_I32 (c->numerus, ZEPHYRUM);
        }

        imprimere("  catena SEMINATA numeris: formae accipitur\n");
        o.sortes_vetant  = VERUM;
        o.semen          = columna_numeri;
        c = partitio_catenam_struere(piscina, columnae, II, aurum,
            sortes, &o);
        CREDO_NON_NIHIL (c);
        si (c != NIHIL)
        {
            CREDO_VERUM (c->semen == columna_numeri);
            CREDO_AEQUALIS_I32 (c->initium->summa.recti, IX);
            CREDO_AEQUALIS_I32 (c->numerus, I);
            si (c->numerus == I)
            {
                CREDO_AEQUALIS_I32 (c->columnae[ZEPHYRUM], I);
                CREDO_AEQUALIS_I32 (
                    c->aestimationes[ZEPHYRUM]->summa.recti, XII);
                CREDO_VERUM (partitio_aequalis(c->partes[ZEPHYRUM],
                    infimum));
            }
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
            /* nullus grex suffragatur -> apertus ubique */
            per (g = ZEPHYRUM; g < IV; g++)
            {
                CREDO_AEQUALIS_I32 (e->habitus[g],
                    (i32)PARTITIO_HABITUS_APERTUS);
            }
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
            /* T36 a: grex quisque = sors sua -> captivus omnis, sine
             * suffragio sorte sua retenta -> apertus */
            CREDO_AEQUALIS_I32 (e->greges_captivi, III);
            CREDO_AEQUALIS_I32 (e->ordines_captivi, XII);
            per (g = ZEPHYRUM; g < III; g++)
            {
                CREDO_AEQUALIS_I32 (e->captivus[g], g);
                CREDO_AEQUALIS_I32 (e->habitus[g],
                    (i32)PARTITIO_HABITUS_APERTUS);
            }
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
       PartitioCatenaOptiones  o;
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
            /* T36 a: u disciplina O V S III (margo II) retentis A, B;
             * O VIII (margo VIII) retenta C - unanimis in C sola ->
             * ordinatus; v S IX, IX, X ubique unanimis -> coactus */
            CREDO_AEQUALIS_I32 (e->margo[ZEPHYRUM * III], II);
            CREDO_AEQUALIS_I32 (e->margo[ZEPHYRUM * III + I], II);
            CREDO_AEQUALIS_I32 (e->margo[ZEPHYRUM * III + II], VIII);
            CREDO_AEQUALIS_I32 (e->margo[I * III], IX);
            CREDO_AEQUALIS_I32 (e->margo[I * III + I], IX);
            CREDO_AEQUALIS_I32 (e->margo[I * III + II], X);
            CREDO_AEQUALIS_I32 (e->habitus[ZEPHYRUM],
                (i32)PARTITIO_HABITUS_ORDINATUS);
            CREDO_AEQUALIS_I32 (e->habitus[I],
                (i32)PARTITIO_HABITUS_COACTUS);
            CREDO_FALSUM (e->mutabilis[ZEPHYRUM]);
            CREDO_FALSUM (e->mutabilis[I]);
            CREDO_AEQUALIS_I32 (e->greges_captivi, ZEPHYRUM);
        }

        imprimere("  una\n");
        e = partitio_aestimare(piscina, una, aurum, sortes, II);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            _summam_probare(e, XVII, XVII, ZEPHYRUM, (s32)ZEPHYRUM,
                653);
            CREDO_AEQUALIS_I32 (e->habitus[ZEPHYRUM],
                (i32)PARTITIO_HABITUS_ORDINATUS);
        }

        columnae[ZEPHYRUM]  = divisio;
        columnae[I]         = una;
        partitio_catena_optiones_initium(&o);
        o.limen           = II;
        o.lucrum_minimum  = I;
        imprimere("  catena vetans\n");
        c = partitio_catenam_struere(piscina, columnae, II, aurum,
            sortes, &o);
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
        o.sortes_vetant = FALSUM;
        c = partitio_catenam_struere(piscina, columnae, II, aurum,
            sortes, &o);
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
            /* T36 a: suffragia discordia -> apertus et mutabilis;
             * margo II, II, 0 (par retenta C) */
            CREDO_AEQUALIS_I32 (e->habitus[ZEPHYRUM],
                (i32)PARTITIO_HABITUS_APERTUS);
            CREDO_VERUM (e->mutabilis[ZEPHYRUM]);
            CREDO_AEQUALIS_I32 (e->margo[ZEPHYRUM], II);
            CREDO_AEQUALIS_I32 (e->margo[I], II);
            CREDO_AEQUALIS_I32 (e->margo[II], ZEPHYRUM);
        }
    }


    /* ==================================================
     * CRATIS IV - grex CAPTIVUS (T36 a, decisio LV): XI ordines,
     *   columna x = 0 | y = I, aurum O = 0 S = I, sortes A B C
     *   A: x O, x O, x S, y S, y S; B: y S, y S, y O; C: y S, y O, y S
     *   x: ordines III omnes in A -> captivus (sors 0); y liber (III)
     *   retenta A: disciplina S IV O II -> basis S, basis III;
     *     x sine disciplina -> inaestimati III, respondet S -> I;
     *     y S IV O II (margo II) -> S -> II; recti III
     *   retenta B: O III S V -> S, basis II; x O II S I (margo I) ->
     *     O, retenti 0; y S IV O I (margo III) -> S -> II; recti II
     *   retenta C: idem -> basis II, recti II
     *   summa recti VII basis VII lucrum 0 inaestimati III; puritas
     *   intra (II + VI) * M / XI = 727; mutabiles 0
     *   INAESTIMABILIS limine II per captivum (2 * III < XI: regula
     *   dimidii sola FALSUM diceret); limine IV FALSUM (III < IV)
     *   habitus x apertus (A sine suffragio), y ordinatus
     * ================================================== */

    {
        constans i32 columnae_notae[XI] = {
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, I, I, I, I, I, I, I, I };
        constans i32 auri_notae[XI] = {
            ZEPHYRUM, ZEPHYRUM, I, I, I, I, I, ZEPHYRUM, I, ZEPHYRUM,
            I };
        constans i32 sortium_notae[XI] = {
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, I, I, I,
            II, II, II };
        constans i32 retenti_x[III]  = { III, ZEPHYRUM, ZEPHYRUM };
        constans i32 retenti_y[III]  = { II, III, III };
        constans i32 recti_x[III]    = { I, ZEPHYRUM, ZEPHYRUM };
        constans i32 recti_y[III]    = { II, II, II };
        constans i32 margo_x[III]    = { ZEPHYRUM, I, I };
        constans i32 margo_y[III]    = { II, III, III };

                    Partitio* columna;
                    Partitio* aurum;
                    Partitio* sortes;
          PartitioAestimatio* e;
                         i32  s;

        imprimere("\n--- Probans cratem IV (grex captivus) ---\n");
        columna  = partitio_ex_notis(piscina, XI, columnae_notae);
        aurum    = partitio_ex_notis(piscina, XI, auri_notae);
        sortes   = partitio_ex_notis(piscina, XI, sortium_notae);
        CREDO_NON_NIHIL (columna);
        CREDO_NON_NIHIL (aurum);
        CREDO_NON_NIHIL (sortes);
        e = partitio_aestimare(piscina, columna, aurum, sortes, II);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            _sortem_probare(e, ZEPHYRUM, V, III, III, III);
            _sortem_probare(e, I, III, II, II, ZEPHYRUM);
            _sortem_probare(e, II, III, II, II, ZEPHYRUM);
            _summam_probare(e, VII, VII, III, (s32)ZEPHYRUM, 727);
            CREDO_AEQUALIS_I32 (e->greges_mutabiles, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (e->captivus[ZEPHYRUM], ZEPHYRUM);
            CREDO_AEQUALIS_I32 (e->captivus[I], III);
            CREDO_AEQUALIS_I32 (e->greges_captivi, I);
            CREDO_AEQUALIS_I32 (e->ordines_captivi, III);
            CREDO_VERUM (e->inaestimabilis);
            CREDO_AEQUALIS_I32 (e->habitus[ZEPHYRUM],
                (i32)PARTITIO_HABITUS_APERTUS);
            CREDO_AEQUALIS_I32 (e->habitus[I],
                (i32)PARTITIO_HABITUS_ORDINATUS);
            CREDO_FALSUM (e->mutabilis[ZEPHYRUM]);
            CREDO_FALSUM (e->mutabilis[I]);
            per (s = ZEPHYRUM; s < III; s++)
            {
                CREDO_AEQUALIS_I32 (e->retenti[ZEPHYRUM * III + s],
                    retenti_x[s]);
                CREDO_AEQUALIS_I32 (e->retenti[I * III + s],
                    retenti_y[s]);
                CREDO_AEQUALIS_I32 (e->recti_gregum[ZEPHYRUM * III + s],
                    recti_x[s]);
                CREDO_AEQUALIS_I32 (e->recti_gregum[I * III + s],
                    recti_y[s]);
                CREDO_AEQUALIS_I32 (e->margo[ZEPHYRUM * III + s],
                    margo_x[s]);
                CREDO_AEQUALIS_I32 (e->margo[I * III + s], margo_y[s]);
            }
        }

        imprimere("  limen IV: captivus sub limine non signat\n");
        e = partitio_aestimare(piscina, columna, aurum, sortes, IV);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL)
        {
            _summam_probare(e, VII, VII, III, (s32)ZEPHYRUM, 727);
            CREDO_AEQUALIS_I32 (e->greges_captivi, I);
            CREDO_AEQUALIS_I32 (e->ordines_captivi, III);
            CREDO_FALSUM (e->inaestimabilis);
        }
    }


    /* ==================================================
     * RECUSATIONES: ordines inaequales, columna NIHIL, optiones
     * NIHIL, semen ordinum inaequalium
     * ================================================== */

    {
         constans Partitio* nulla[I] = { NIHIL };
         constans Partitio* unae[I];
                  Partitio* a;
                  Partitio* b;
    PartitioCatenaOptiones  o;

        imprimere("\n--- Probans recusationes ---\n");
        a = partitio_una(piscina, IV);
        b = partitio_una(piscina, III);
        CREDO_NON_NIHIL (a);
        CREDO_NON_NIHIL (b);
        unae[ZEPHYRUM] = a;
        partitio_catena_optiones_initium(&o);
        CREDO_AEQUALIS_I32 (o.limen, XX);
        CREDO_AEQUALIS_I32 (o.lucrum_minimum, ZEPHYRUM);
        CREDO_VERUM (o.sortes_vetant);
        CREDO_NIHIL (o.semen);
        CREDO_NIHIL (partitio_aestimare(piscina, a, b, a, II));
        CREDO_NIHIL (partitio_aestimare(piscina, a, a, NIHIL, II));
        CREDO_NIHIL (partitio_catenam_struere(piscina, nulla, I, a, a,
            &o));
        CREDO_NIHIL (partitio_catenam_struere(piscina, unae, I, a, a,
            NIHIL));
        o.semen = b;
        CREDO_NIHIL (partitio_catenam_struere(piscina, unae, I, a, a,
            &o));
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
