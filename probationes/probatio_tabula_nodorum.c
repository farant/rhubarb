/* probatio_tabula_nodorum.c - Probationes tabulae nodorum
 *
 * Oracula: (1) numeri symmetriae ex KnotInfo (CC reversibiles, XXIX
 * chirales, XIII plene amphichirales, VII negative, nodus trivialis);
 * (2) theoremata - Alexander semper symmetricus, Jones nodi
 * amphichiralis J(t) = J(1/t); (3) omnis nodus ex codice PD per
 * laqueus recomputatus = tabula; (4) polynomia KnotInfo litteris
 * transcripta; (5) agnitio: 5_1 et 10_132 (par notum, Alexander et
 * Jones communes), nodus aniculae et nodus quadratus ut compositi,
 * nodus trivialis solus (nullus nodus non trivialis <= X transituum
 * Alexander 1 habet; primi sunt 11n34, 11n42).
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "chorda.h"
#include "polynomium.h"
#include "laqueus.h"
#include "tabula_nodorum.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina* piscina;

interior Polynomium
_p (
    constans character* textus)
{
    Polynomium p = polynomium_nullum();

    si (!polynomium_ex_chorda(chorda_ex_literis(textus, piscina), 't',
            piscina, &p))
    {
        imprimere("FRACTA: polynomium malum: %s\n", textus);
    }
    redde p;
}

interior Polynomium
_alexander (
    constans character* titulus)
{
    redde _p(tabula_nodorum_quaere(titulus)->alexander);
}

interior Polynomium
_jones (
    constans character* titulus)
{
    redde _p(tabula_nodorum_quaere(titulus)->jones);
}

/* agnitio continet (primus, speculum) solum (secundus NIHIL)? */
interior b32
_continet_primum (
               Agnitio* a,
                   i32  n,
    constans character* titulus,
                   b32  speculum)
{
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        si (a[k].secundus == NIHIL && a[k].primus_speculum == speculum
            && strcmp(a[k].primus->titulus, titulus) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

s32 principale (vacuum)
{
    piscina = piscina_generare_dynamicum("probatio_tabula_nodorum",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * QUAESTIO ET FONS
     * ================================================== */

    {
        constans NodusTabulae* n;

        imprimere("\n--- Probans quaestionem et fontem ---\n");
        CREDO_AEQUALIS_I32 (tabula_nodorum_numerus(), CCL);
        CREDO_NIHIL (tabula_nodorum_nodus(CCL));
        CREDO_NON_NIHIL (tabula_nodorum_nodus(CCXLIX));
        n = tabula_nodorum_quaere("8_20");
        CREDO_NON_NIHIL (n);
        CREDO_VERUM (n != NIHIL
            && strcmp(n->titulus, "8_20") == ZEPHYRUM
            && n->transitus == VIII);
        CREDO_NIHIL (tabula_nodorum_quaere("11n34"));
        CREDO_NIHIL (tabula_nodorum_quaere("8_20 "));
        CREDO_VERUM (strcmp(tabula_nodorum_nodus(ZEPHYRUM)->titulus,
            "0_1")
            == ZEPHYRUM);
        CREDO_NIHIL (tabula_nodorum_pd(tabula_nodorum_nodus(ZEPHYRUM)));
        n = tabula_nodorum_quaere("3_1");
        CREDO_VERUM (n != NIHIL && tabula_nodorum_pd(n)[ZEPHYRUM] == I
            && tabula_nodorum_pd(n)[XI] == II);
        CREDO_VERUM (strstr(TABULA_NODORUM_FONS, "KnotInfo") != NIHIL
            && strstr(TABULA_NODORUM_FONS, "2026.10.5") != NIHIL
            && strstr(TABULA_NODORUM_FONS,
            "eb511ebc61204bc1c6d700d9da42257d"
            "cbb0369cb6756e89ed978a7821867c7d")
            != NIHIL);
    }


    /* ==================================================
     * SYMMETRIA (numeri KnotInfo)
     * ================================================== */

    {
        i32 numeri[VI] = { ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM };
        i32 per_transitus[XI] = { ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM };
        i32 k;
        b32 intra = VERUM;

        imprimere("\n--- Probans symmetriam et numeros ---\n");
        per (k = ZEPHYRUM; k < CCL; k++)
        {
            constans NodusTabulae* n = tabula_nodorum_nodus(k);

            si (   n->symmetria > TABULA_NODORUM_AMPHICHIRALIS_POSITIVA
                || n->transitus > X)
            {
                intra = FALSUM;
                perge;
            }
            numeri[n->symmetria]++;
            per_transitus[n->transitus]++;
        }
        CREDO_VERUM (intra);
        CREDO_AEQUALIS_I32 (numeri[TABULA_NODORUM_NULLA], I);
        CREDO_AEQUALIS_I32 (numeri[TABULA_NODORUM_REVERSIBILIS], CC);
        CREDO_AEQUALIS_I32 (numeri[TABULA_NODORUM_CHIRALIS], XXIX);
        CREDO_AEQUALIS_I32 (numeri[TABULA_NODORUM_AMPHICHIRALIS_PLENA],
            XIII);
        CREDO_AEQUALIS_I32 (numeri[
            TABULA_NODORUM_AMPHICHIRALIS_NEGATIVA],
            VII);
        CREDO_AEQUALIS_I32 (numeri[
            TABULA_NODORUM_AMPHICHIRALIS_POSITIVA],
            ZEPHYRUM);
        /* nodi primi per transitus (OEIS A002863): 1, 1, 2, 3, 7, 21,
         * 49, 165 */
        CREDO_AEQUALIS_I32 (per_transitus[ZEPHYRUM], I);
        CREDO_AEQUALIS_I32 (per_transitus[III], I);
        CREDO_AEQUALIS_I32 (per_transitus[IV], I);
        CREDO_AEQUALIS_I32 (per_transitus[V], II);
        CREDO_AEQUALIS_I32 (per_transitus[VI], III);
        CREDO_AEQUALIS_I32 (per_transitus[VII], VII);
        CREDO_AEQUALIS_I32 (per_transitus[VIII], XXI);
        CREDO_AEQUALIS_I32 (per_transitus[IX], XLIX);
        CREDO_AEQUALIS_I32 (per_transitus[X], CLXV);
        CREDO_VERUM (tabula_nodorum_amphichiralis(tabula_nodorum_quaere(
            "0_1"))
                && tabula_nodorum_amphichiralis(tabula_nodorum_quaere(
            "4_1"))
                && tabula_nodorum_amphichiralis(tabula_nodorum_quaere(
            "8_17"))
            && !tabula_nodorum_amphichiralis(tabula_nodorum_quaere(
            "3_1"))
                && !tabula_nodorum_amphichiralis(tabula_nodorum_quaere(
            "9_42")));
    }


    /* ==================================================
     * POLYNOMIA: KnotInfo litteris, theoremata, recomputatio
     * ================================================== */

    {
         TabulaNodorum* t = tabula_nodorum_aperire(piscina);
                   i32  k;
                   b32  alexander_symmetrici       = VERUM;
                   b32  amphichirales_symmetrici   = VERUM;
                   b32  ex_pd_concordes            = VERUM;
                   i32  chirales_jones_symmetrico  = ZEPHYRUM;
                   b32  chirales_noti              = VERUM;
        /* chiralitas Jones invisibilis: 9_42, 10_48, 10_71, 10_91,
         * 10_104, 10_125 (nodi chirales Jones symmetrico) */
        constans character* chirales[VI] = { "9_42", "10_48", "10_71",
            "10_91", "10_104", "10_125" };

        imprimere("\n--- Probans polynomia ---\n");
        CREDO_NON_NIHIL (t);
        /* KnotInfo, knotinfo_data_complete.csv 2026.10.5 */
        CREDO_VERUM (polynomium_aequalis(_jones("3_1"),
            _p("t + t^3 - t^4")));
        CREDO_VERUM (polynomium_aequalis(_jones("4_1"),
            _p("t^-2 - t^-1 + 1 - t + t^2")));
        CREDO_VERUM (polynomium_aequalis(_alexander("4_1"),
            _p("1 - 3t + t^2")));
        CREDO_VERUM (polynomium_aequalis(_jones("8_20"),
            _p("-t^-5 + t^-4 - t^-3 + 2t^-2 - t^-1 + 2 - t")));
        CREDO_VERUM (polynomium_aequalis(_alexander("8_20"),
            _p("1 - 2t + 3t^2 - 2t^3 + t^4")));
        CREDO_VERUM (polynomium_aequalis(_alexander("5_1"),
            _alexander("10_132")));
        per (k = ZEPHYRUM; k < CCL; k++)
        {
            constans NodusTabulae* n     = tabula_nodorum_nodus(k);
                   PiscinaNotatio  nota  = piscina_notare(piscina);
                       Polynomium  a     = _p(n->alexander);
                       Polynomium  j     = _p(n->jones);
                       Polynomium  a2    = polynomium_nullum();
                       Polynomium  j2    = polynomium_nullum();

            si (!polynomium_est_symmetricum(a))
            {
                alexander_symmetrici = FALSUM;
            }
            si (   tabula_nodorum_amphichiralis(n)
                && !polynomium_aequalis(j, polynomium_inversum(j,
                piscina)))
            {
                amphichirales_symmetrici = FALSUM;
            }
            si (   !tabula_nodorum_amphichiralis(n)
                && polynomium_aequalis(j, polynomium_inversum(j,
                piscina)))
            {
                i32 m;
                b32 notus = FALSUM;

                per (m = ZEPHYRUM; m < VI; m++)
                {
                    si (strcmp(chirales[m], n->titulus) == ZEPHYRUM)
                    {
                        notus = VERUM;
                    }
                }
                si (!notus)
                {
                    chirales_noti = FALSUM;
                }
                chirales_jones_symmetrico++;
            }
            si (   n->transitus > ZEPHYRUM && (
                   !laqueus_alexander_ex_pd(tabula_nodorum_pd(n),
                    n->transitus, piscina, &a2)
                || !polynomium_normale(a2, piscina, &a2)
                || !polynomium_aequalis(a, a2)
                || !laqueus_jones_ex_pd(tabula_nodorum_pd(n),
                n->transitus,
                    piscina, &j2)
                || !polynomium_aequalis(j, j2)))
            {
                imprimere("  recomputatio discrepat: %s\n", n->titulus);
                ex_pd_concordes = FALSUM;
            }
            piscina_reficere(piscina, nota);
        }
        CREDO_VERUM (alexander_symmetrici);
        CREDO_VERUM (amphichirales_symmetrici);
        CREDO_VERUM (ex_pd_concordes);
        CREDO_AEQUALIS_I32 (chirales_jones_symmetrico, VI);
        CREDO_VERUM (chirales_noti);
    }


    /* ==================================================
     * AGNITIO
     * ================================================== */

    {
         TabulaNodorum* t = tabula_nodorum_aperire(piscina);
               Agnitio  a[XVI];
                   i32  n;
                   i32  k;
                   b32  omnes_primi    = VERUM;
                   b32  omnia_specula  = VERUM;
            Polynomium  j31            = _jones("3_1");
            Polynomium  a31 =
                _alexander("3_1");
            Polynomium productum    = polynomium_nullum();
            Polynomium productum_j  = polynomium_nullum();

        imprimere("\n--- Probans agnitionem ---\n");
        CREDO_NON_NIHIL (t);

        /* nodus trivialis solus */
        n = tabula_nodorum_agnoscere(t, _p("1"), _p("1"), piscina, a,
            XVI);
        CREDO_AEQUALIS_I32 (n, I);
        CREDO_VERUM (_continet_primum(a, n, "0_1", FALSUM));

        /* trifolium et speculum eius */
        n = tabula_nodorum_agnoscere(t, a31, j31, piscina, a, XVI);
        CREDO_AEQUALIS_I32 (n, I);
        CREDO_VERUM (_continet_primum(a, n, "3_1", FALSUM));
        n = tabula_nodorum_agnoscere(t, a31, polynomium_inversum(j31,
            piscina), piscina, a, XVI);
        CREDO_AEQUALIS_I32 (n, I);
        CREDO_VERUM (_continet_primum(a, n, "3_1", VERUM));
        /* Alexander ad +-t^k: forma quaelibet */
        n = tabula_nodorum_agnoscere(t, _p("-t^-1 + 1 - t"), j31,
            piscina,
            a, XVI);
        CREDO_AEQUALIS_I32 (n, I);

        /* amphichiralis semel */
        n = tabula_nodorum_agnoscere(t, _alexander("4_1"),
            _jones("4_1"),
            piscina,
            a, XVI);
        CREDO_AEQUALIS_I32 (n, I);
        CREDO_VERUM (_continet_primum(a, n, "4_1", FALSUM));

        /* 5_1 et 10_132: invariantes communes, ambo redduntur */
        n = tabula_nodorum_agnoscere(t, _alexander("5_1"),
            _jones("5_1"),
            piscina,
            a, XVI);
        CREDO_AEQUALIS_I32 (n, II);
        CREDO_VERUM (_continet_primum(a, n, "5_1", FALSUM));
        CREDO_VERUM (_continet_primum(a, n, "10_132", FALSUM)
            || _continet_primum(a, n, "10_132", VERUM));
        /* coincidentiae Jones tabulae Rolfsen notae (Alexander quoque
         * communis): par quisque bis agnoscitur */
        {
            constans character* paria[V][II] = { { "5_1", "10_132" },
                { "8_8", "10_129" }, { "8_16", "10_156" },
                { "10_25", "10_56" }, { "10_40", "10_103" } };
                           i32 m;
                           b32 paria_bene = VERUM;

            per (m = ZEPHYRUM; m < V; m++)
            {
                constans NodusTabulae* x = tabula_nodorum_quaere(
                    paria[m][ZEPHYRUM]);

                n = tabula_nodorum_agnoscere(t, _p(x->alexander),
                    _p(x->jones), piscina, a, XVI);
                si (   n != II
                    || !_continet_primum(a, n, paria[m][ZEPHYRUM],
                        FALSUM)
                    || (!_continet_primum(a, n, paria[m][I], FALSUM)
                        && !_continet_primum(a, n, paria[m][I], VERUM)))
                {
                    imprimere("  par non agnitum: %s %s (%u)\n",
                        paria[m][ZEPHYRUM], paria[m][I], n);
                    paria_bene = FALSUM;
                }
            }
            CREDO_VERUM (paria_bene);
        }
        /* maximus: numerus totus, scriptura limitata */
        a[I].primus = NIHIL;
        n = tabula_nodorum_agnoscere(t, _alexander("5_1"),
            _jones("5_1"),
            piscina,
            a, I);
        CREDO_AEQUALIS_I32 (n, II);
        CREDO_NIHIL (a[I].primus);
        CREDO_AEQUALIS_I32 (tabula_nodorum_agnoscere(t, _p("1"),
            _p("1"),
            piscina, a, ZEPHYRUM), I);

        /* nodus aniculae (3_1 # 3_1) et quadratus (3_1 # 3_1*):
         * Alexander idem (= 8_20 Alexander quoque), Jones differt */
        CREDO_VERUM (polynomium_multiplica(a31, a31, piscina,
            &productum));
        CREDO_VERUM (polynomium_multiplica(j31, j31, piscina,
            &productum_j));
        n = tabula_nodorum_agnoscere(t, productum, productum_j, piscina,
            a,
            XVI);
        CREDO_AEQUALIS_I32 (n, I);
        CREDO_VERUM (n == I && a[ZEPHYRUM].secundus != NIHIL
            && strcmp(a[ZEPHYRUM].primus->titulus, "3_1") == ZEPHYRUM
            && strcmp(a[ZEPHYRUM].secundus->titulus, "3_1") == ZEPHYRUM
            && !a[ZEPHYRUM].primus_speculum
            && !a[ZEPHYRUM].secundus_speculum);
        CREDO_VERUM (polynomium_multiplica(j31, polynomium_inversum(j31,
            piscina), piscina, &productum_j));
        n = tabula_nodorum_agnoscere(t, productum, productum_j, piscina,
            a,
            XVI);
        CREDO_AEQUALIS_I32 (n, I);
        CREDO_VERUM (n == I && a[ZEPHYRUM].secundus != NIHIL
            && !a[ZEPHYRUM].primus_speculum
            && a[ZEPHYRUM].secundus_speculum);
        CREDO_VERUM (polynomium_aequalis(productum,
            _alexander("8_20")));

        /* 3_1 # 4_1: ordo tabulae, primus <= secundus */
        CREDO_VERUM (polynomium_multiplica(a31,
            _alexander("4_1"), piscina, &productum));
        CREDO_VERUM (polynomium_multiplica(j31,
            _jones("4_1"), piscina, &productum_j));
        n = tabula_nodorum_agnoscere(t, productum, productum_j, piscina,
            a,
            XVI);
        CREDO_AEQUALIS_I32 (n, I);
        CREDO_VERUM (n == I && a[ZEPHYRUM].secundus != NIHIL
            && strcmp(a[ZEPHYRUM].primus->titulus, "3_1") == ZEPHYRUM
            && strcmp(a[ZEPHYRUM].secundus->titulus, "4_1")
                == ZEPHYRUM);

        /* nihil: Alexander alienum */
        CREDO_AEQUALIS_I32 (tabula_nodorum_agnoscere(t, _p("t^2 + 1"),
            j31, piscina, a, XVI), ZEPHYRUM);

        /* omnis nodus se ipsum agnoscit, et speculum suum */
        per (k = ZEPHYRUM; k < CCL; k++)
        {
            constans NodusTabulae* nodus  = tabula_nodorum_nodus(k);
                   PiscinaNotatio  nota   = piscina_notare(piscina);
                       Polynomium  alexander_nodi =
                           _p(nodus->alexander);
                       Polynomium jones_nodi = _p(nodus->jones);

            n = tabula_nodorum_agnoscere(t, alexander_nodi, jones_nodi,
                piscina, a, XVI);
            si (!_continet_primum(a, n, nodus->titulus, FALSUM))
            {
                imprimere("  non agnitus: %s\n", nodus->titulus);
                omnes_primi = FALSUM;
            }
            n = tabula_nodorum_agnoscere(t, alexander_nodi,
                polynomium_inversum(jones_nodi,
                piscina), piscina, a, XVI);
            si (!_continet_primum(a, n, nodus->titulus,
                    !tabula_nodorum_amphichiralis(nodus)))
            {
                imprimere("  speculum non agnitum: %s\n",
                    nodus->titulus);
                omnia_specula = FALSUM;
            }
            piscina_reficere(piscina, nota);
        }
        CREDO_VERUM (omnes_primi);
        CREDO_VERUM (omnia_specula);
    }

    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
