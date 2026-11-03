/* uncinus_mensura.c - MENSURA uncinus Kauffman per modulum vertex
 *
 * T5 (project-specs/contractio-plan.md): ubi tempus uncinus per rete
 * consumitur, et quomodo crescit.
 *   I    contra summam statuum ad c = 12, 16, 20, 24 (summae connexae
 *        nodorum tabulae et clausurae tricarum fortuitae): tempora et
 *        aequalitas
 *   II   latitudo ordinis avidi super omnes nodos tabulae (per c)
 *   III  scala summarum connexarum c = 50 .. 800: aedificatio, tempus
 *        per punctum, numerus contractionum (numeratus), totum,
 *        ns per multiplicationem, octeti piscinae
 *   IV   clausurae tricarum n = 4 .. 12 filorum: latitudo, officina,
 *        tempus per punctum, contractiones et totum PRAEDICTA (totum
 *        cursum solum si coniectura < LX s)
 *   V    (solum per -sectio V) trica una fortuita: -fila N
 *        -longitudo L -limes S (cursus totus si coniectura < S
 *        secundis) -semen K
 *   VI   (solum per -sectio VI; temperley-celeritas-plan) via TL, A/B:
 *        nucleus VETUS (79e7fad9, ex API temperley publica aedificatus)
 *        contra laqueus_modulus_temperley_aestimare in eodem binario,
 *        paria ABBA (-paria P, ordinarie IX) in primo maximo = 1 mod 4
 *        infra 2^31, valor vetus = novus in omni puncto; mediana
 *        rationum et ambitus. -fila N (ordinarie VIII..XVI) -longitudo L
 *   VII  (solum per -sectio VII) tricae probationum viae: K* = CLXXXV x
 *        multiplicationes vertex / multiplicationes temperley (pretium
 *        temperley infra K* -> temperley), officina et octeti
 *   VIII (solum per -sectio VIII) uncinus TL totus: -fila N -longitudo L,
 *        tempus processoris
 *   IX   (solum per -sectio IX) porta G1: nucleus in loco reductione
 *        UNA (p < 2^31) contra bibliothecam (reductiones duae), A/B
 *   X    (solum per -sectio X) porta G2: reductio Montgomery contra
 *        bibliothecam (post G1: reductio una per %), A/B
 *   XI   (solum per -sectio XI) acceptatio: XLV uncini D136 per
 *        laqueus_uncinus_ex_plexu, lineae formae brackets.txt
 *   XII  (solum per -sectio XII) verba tricarum D136 ad comparationem
 *        externam: "VERBUM n c | g1 g2 ..." (g > 0 = sigma_g
 *        positivum), omnes XLV aut una per -fila N -longitudo L
 *   XIII (solum per -sectio XIII; interpolatio-verificatio-plan T0)
 *        fundamentum verificationis SINE aestimatione: fenestra a
 *        priori K, limes, k_max (TL et vertex), k verus ex uncino noto
 *        (-uncini VIA, forma brackets.txt, usque ad IV), m, aestimationes
 *        PROBATUM/VERIFICATUM, sumptus vertex; XLV + 14 x 801 aut una
 *   XIV  (solum per -sectio XIV) cribrum: primi in [2^30, 2^31) per
 *        residuum mod 4, summa contra pi(2^31) - pi(2^30)
 * Tempus = tempus PROCESSORIS (clock). Driver bibliothecae
 * (laqueus_modulus_vertex_uncinus); aestimationes et primi ex
 * numeratoribus structurae (laqueus_modulus_vertex_vocationes); sectio
 * I effectum cum laqueus_uncinus_ex_pd comparat. Columna limes = bita
 * limitis coefficientium (T5a), bita = coefficiens maximus verus.
 *
 * Clausura tricae per laqueus_pd_ex_plexu.
 *
 * Usus: ./bin/aedilis tools/uncinus_mensura.c && bash
 *   build/aedilis/uncinus_mensura/struere.sh &&
 *   ./build/aedilis/uncinus_mensura/uncinus_mensura [-machina]
 *   [-sectio I|II|III|IV|V|VI|VII|VIII|IX|X|XI|XII|XIII|XIV]
 *   -machina: lineae "MENSURA <sectio> clavis=valor ..."
 */
#include "latina.h"
#include "piscina.h"
#include "sors.h"
#include "magnus.h"
#include "polynomium.h"
#include "interpolatio.h"
#include "contractio.h"
#include "laqueus.h"
#include "temperley.h"
#include "congruentia.h"
#include "tabula_nodorum.h"
#include "filum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define OCTETI_MAXIMI ((i64)0x80000000UL)   /* 2 GiB officina */

interior b32 machina = FALSUM;

interior f64
_secundae (
    clock_t initium)
{
    redde (f64)(clock() - initium) / (f64)CLOCKS_PER_SEC;
}


/* ==================================================
 * Codices: summae connexae et clausurae tricarum
 * ================================================== */

interior s32
_signum (
    constans i32* x,
             i32  m)
{
    redde x[I] == x[III] % m + I ? (s32)I : -(s32)I;
}

interior b32
_intrans (
    constans i32* x,
             i32  j,
             i32  m)
{
    s32 signum = _signum(x, m);

    redde j == ZEPHYRUM || (j == III && signum > ZEPHYRUM)
        || (j == I && signum < ZEPHYRUM);
}

/* K1 # K2 (vide probatio_laqueus_rete.c) */
interior i32*
_summa_connexa (
    constans i32* pd_sinister,
             i32  c1,
    constans i32* pd_dexter,
             i32  c2,
         Piscina* piscina)
{
    i32* novum = (i32*)piscina_allocare(piscina, (memoriae_index)IV
        * (c1 + c2) * magnitudo(i32));
    i32 m1 = II * c1;
    i32 m2 = II * c2;
    i32 k;

    per (k = ZEPHYRUM; k < IV * c1; k++)
    {
        novum[k] = pd_sinister[k] == m1 && _intrans(pd_sinister + IV
            * (k / IV), k % IV, m1) ? m1 + m2 : pd_sinister[k];
    }
    per (k = ZEPHYRUM; k < IV * c2; k++)
    {
        novum[IV * c1 + k] = pd_dexter[k] == m2 && _intrans(pd_dexter
            + IV * (k / IV), k % IV, m2) ? m1 : pd_dexter[k] + m1;
    }
    redde novum;
}

/* summa connexa nodorum tabulae fortuitorum cum EXACTE c transitibus
 * (partes III..XIII) */
interior constans i32*
_summa_fortuita (
       Sors* s,
        i32  c,
    Piscina* piscina)
{
    per (;;)
    {
        constans i32* pd         = NIHIL;
                 i32  transitus  = ZEPHYRUM;

        dum (transitus < c)
        {
                              i32  reliquum = c - transitus;
            constans NodusTabulae* n;

            si (reliquum < III)
            {
                frange;
            }
            fac
            {
                n = tabula_nodorum_nodus(sors_intra(s,
                    tabula_nodorum_numerus()));
            }
            dum (   n->transitus < III || n->transitus > reliquum
                || (reliquum != n->transitus
                    && reliquum - n->transitus < III));
            si (pd == NIHIL)
            {
                pd = tabula_nodorum_pd(n, piscina);
            }
            alioquin
            {
                pd = _summa_connexa(pd, transitus,
                    tabula_nodorum_pd(n, piscina), n->transitus,
                    piscina);
            }
            transitus += n->transitus;
        }
        si (transitus == c)
        {
            redde pd;
        }
    }
}

/* clausura tricae per bibliothecam (laqueus_pd_ex_plexu; sigma
 * positivum = transitus positivus); NIHIL si refutata */
interior i32*
_plexus_pd (
    constans s32* verbum,
             i32  longitudo,
             i32  n,
         Piscina* piscina)
{
    i32* pd;
    i32  transitus;

    redde laqueus_pd_ex_plexu(verbum, longitudo, n, piscina, &pd,
        &transitus) ? pd : NIHIL;
}

/* verbum fortuitum cuius permutatio cyclus n est (nodus). PARITAS:
 * cyclus n signum (-1)^(n-1) habet, quisque generator transpositio
 * est - ergo longitudo === n - 1 (modulo 2) necessaria; longitudo
 * impar fili numero pari, par impari. *longitudo ad paritatem
 * aptatur (+1 si opus). Conatus irriti memoriam per notare/reficere
 * reddunt; post M conatus NIHIL (numquam ansa infinita). 2026-10-27:
 * forma prior sine paritate et sine reficere IV filis, L transitibus
 * in aeternum quaerebat, piscina usque ad ~120 GB crescente. */
interior i32*
_plexus_fortuitus (
       Sors*  s,
        i32   n,
        i32*  longitudo,
    Piscina*  piscina,
        s32** verbum_exitus)
{
    s32* verbum;
    i32  conatus;

    si ((*longitudo + n - I) % II != ZEPHYRUM)
    {
        (*longitudo)++;
    }
    verbum = (s32*)piscina_allocare(piscina, (memoriae_index)*longitudo
        * magnitudo(s32));
    per (conatus = ZEPHYRUM; conatus < M; conatus++)
    {
        PiscinaNotatio  nota = piscina_notare(piscina);
                   i32* pd;
                   i32  k;

        per (k = ZEPHYRUM; k < *longitudo; k++)
        {
            s32 g = (s32)(I + sors_intra(s, n - I));

            verbum[k] = sors_intra(s, II) == ZEPHYRUM ? g : -g;
        }
        pd = _plexus_pd(verbum, *longitudo, n, piscina);
        si (pd != NIHIL)
        {
            si (verbum_exitus != NIHIL)
            {
                *verbum_exitus = verbum;
            }
            redde pd;
        }
        piscina_reficere(piscina, nota);
    }
    redde NIHIL;
}


/* ==================================================
 * Driver numerans (idem ac laqueus _uncinus_per_rete)
 * ================================================== */

interior i32
_radix (
     i32* pater,
     i32  x)
{
    dum (pater[x] != x)
    {
        pater[x]  = pater[pater[x]];
        x         = pater[x];
    }
    redde x;
}

interior s64
_modulo_quattuor (
    s64 x)
{
    s64 r = x % (s64)IV;

    redde r < ZEPHYRUM ? r + (s64)IV : r;
}

/* fenestra Kauffman in classe 3w, ut laqueus_modulus_vertex_creare */
interior vacuum
_fenestra (
    constans i32* pd,
             i32  c,
         Piscina* piscina,
             s32* imus,
             s32* summus)
{
    i32  m     = II * c;
    i32* pater = (i32*)piscina_allocare(piscina, (memoriae_index)m
        * magnitudo(i32));
    i32 ansae_a  = ZEPHYRUM;
    i32 ansae_b  = ZEPHYRUM;
    s32 w        = ZEPHYRUM;
    s32 classis;
    i32 l;
    i32 k;
    i32 status;

    per (status = ZEPHYRUM; status < II; status++)
    {
        per (l = ZEPHYRUM; l < m; l++)
        {
            pater[l] = l;
        }
        per (k = ZEPHYRUM; k < c; k++)
        {
            constans i32* x = pd + IV * k;

            si (status == ZEPHYRUM)
            {
                pater[_radix(pater, x[0] - I)] = _radix(pater, x[I]
                    - I);
                pater[_radix(pater, x[II] - I)] = _radix(pater, x[III]
                    - I);
            }
            alioquin
            {
                pater[_radix(pater, x[0] - I)] = _radix(pater, x[III]
                    - I);
                pater[_radix(pater, x[I] - I)] = _radix(pater, x[II]
                    - I);
            }
        }
        per (l = ZEPHYRUM; l < m; l++)
        {
            si (_radix(pater, l) == l)
            {
                si (status == ZEPHYRUM)
                {
                    ansae_a++;
                }
                alioquin
                {
                    ansae_b++;
                }
            }
        }
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        w += _signum(pd + IV * k, m);
    }
    classis  = (s32)_modulo_quattuor((s64)III * (s64)w);
    *imus    = -(s32)c - II * (s32)ansae_b + II;
    *imus    += (s32)_modulo_quattuor((s64)classis - (s64)*imus);
    *summus  = (s32)c + II * (s32)ansae_a - II;
    *summus  -= (s32)_modulo_quattuor((s64)*summus - (s64)classis);
}

/* uncinus per driver BIBLIOTHECAE (laqueus_modulus_vertex_uncinus) in
 * structura nova: aestimationes et primi ex numeratoribus eius (T5a:
 * limes per arbores extensas). FALSUM si refutatum. */
interior b32
_uncinus_bibliothecae (
          constans i32*  pd,
                   i32   c,
               Piscina*  piscina,
    LaqueusModulusVertex** mv_exitus,
                   i32*  aestimationes,
                   i32*  primi,
            Polynomium*  exitus)
{
    LaqueusModulusVertex* mv = laqueus_modulus_vertex_creare(pd, c,
        OCTETI_MAXIMI, piscina);

    *mv_exitus = mv;
    si (   mv == NIHIL
        || !laqueus_modulus_vertex_uncinus(mv, piscina, exitus))
    {
        redde FALSUM;
    }
    laqueus_modulus_vertex_vocationes(mv, aestimationes, primi);
    redde VERUM;
}


/* ==================================================
 * Sectiones
 * ================================================== */

/* tempus per punctum: medium super k puncta (omega 2..k+1), primo
 * 998244353 - NON primo primo driveris (4294967197), ut mutationes
 * primi in cursu sequenti omnes numerentur */
interior f64
_tempus_puncti (
    LaqueusModulusVertex* mv,
                     i32  k)
{
    clock_t t0 = clock();
        i32 a;
        i32 v;
        i32 j;

    per (j = ZEPHYRUM; j < k; j++)
    {
        (vacuum)laqueus_modulus_vertex_aestimare(mv, j + II,
            (i32)998244353UL, &a, &v);
    }
    redde _secundae(t0) / (f64)k;
}

interior vacuum
_sectio_comparationis (vacuum)
{
    constans i32 magnitudines[] = { XII, XVI, XX, XXIV };
            Sors s;
             i32 t;
             i32 genus;

    sors_seminare(&s, (i64)MMXXVI, ZEPHYRUM);
    si (!machina)
    {
        imprimere("I. contra summam statuum (tempus processoris, s)\n");
        imprimere("   genus   c   status     rete   puncta primi  "
            "latitudo  aequalis\n");
    }
    per (t = ZEPHYRUM; t < IV; t++)
    {
        per (genus = ZEPHYRUM; genus < II; genus++)
        {
            Piscina* piscina = piscina_generare_dynamicum("mensura",
                1048576);
                     i32  c       = magnitudines[t];
            constans i32* pd  = genus == ZEPHYRUM
                ? _summa_fortuita(&s, c, piscina)
                : _plexus_fortuitus(&s, V, &c, piscina, NIHIL);
                   Polynomium  status;
                   Polynomium  rete;
         LaqueusModulusVertex* mv      = NIHIL;
                          i32  puncta  = ZEPHYRUM;
                          i32  primi   = ZEPHYRUM;
                      clock_t  t0;
                          f64  tempus_status;
                          f64  tempus_rete;
                          b32  aequalis;
            ContractioSumptus  sumptus;

            t0 = clock();
            aequalis = laqueus_uncinus_ex_pd(pd, c, piscina, &status);
            tempus_status = _secundae(t0);
            t0 = clock();
            aequalis = aequalis && _uncinus_bibliothecae(pd, c, piscina,
                &mv, &puncta, &primi, &rete)
                && polynomium_aequalis(rete, status);
            tempus_rete  = _secundae(t0);
            sumptus      = laqueus_modulus_vertex_sumptus(mv);
            si (machina)
            {
                imprimere("MENSURA I genus=%s c=%u status_s=%.4f "
                    "rete_s=%.4f puncta=%u primi=%u latitudo=%u "
                    "aequalis=%u\n", genus == ZEPHYRUM ? "summa"
                    : "plexus5", c, tempus_status, tempus_rete,
                    puncta, primi, sumptus.latitudo,
                    (i32)aequalis);
            }
            alioquin
            {
                imprimere("   %-7s %2u  %8.3f %8.4f   %5u  %4u    %4u     "
                    " %s\n", genus == ZEPHYRUM ? "summa" : "plexus5", c,
                    tempus_status, tempus_rete, puncta, primi,
                    sumptus.latitudo, aequalis ? "ita" : "NON");
            }
            piscina_destruere(piscina);
        }
    }
}

interior vacuum
_sectio_latitudinis (vacuum)
{
    i32 numerus[XIV];
    i32 maxima[XIV];
    i64 summa[XIV];
    i64 apex[XIV];
    f64 tempus[XIV];
    i32 k;
    i32 c;

    per (c = ZEPHYRUM; c < XIV; c++)
    {
        numerus[c]  = ZEPHYRUM;
        maxima[c]   = ZEPHYRUM;
        summa[c]    = ZEPHYRUM;
        apex[c]     = ZEPHYRUM;
        tempus[c]   = 0.0;
    }
    per (k = ZEPHYRUM; k < tabula_nodorum_numerus(); k++)
    {
        constans NodusTabulae* n = tabula_nodorum_nodus(k);
                      Piscina* piscina;
         LaqueusModulusVertex* mv;
            ContractioSumptus  sumptus;
                      clock_t  t0;

        si (n->transitus < III)
        {
            perge;
        }
        piscina  = piscina_generare_dynamicum("mensura", 65536);
        t0       = clock();
        mv      = laqueus_modulus_vertex_creare(tabula_nodorum_pd(n,
            piscina), n->transitus, OCTETI_MAXIMI, piscina);
        tempus[n->transitus]  += _secundae(t0);
        sumptus               = laqueus_modulus_vertex_sumptus(mv);
        numerus[n->transitus]++;
        summa[n->transitus] += sumptus.latitudo;
        si (sumptus.latitudo > maxima[n->transitus])
        {
            maxima[n->transitus] = sumptus.latitudo;
        }
        si (sumptus.apex > apex[n->transitus])
        {
            apex[n->transitus] = sumptus.apex;
        }
        piscina_destruere(piscina);
    }
    si (!machina)
    {
        imprimere("\nII. latitudo ordinis avidi, nodi tabulae\n");
        imprimere("    c  nodi  latitudo media  maxima  apex max  "
            "aedificatio (ms/nodum)\n");
    }
    per (c = III; c < XIV; c++)
    {
        si (machina)
        {
            imprimere("MENSURA II c=%u nodi=%u latitudo_media=%.3f "
                "latitudo_max=%u apex_max=%llu aedificatio_ms=%.4f\n",
                c,
                numerus[c], (f64)summa[c] / (f64)numerus[c], maxima[c],
                (unsigned long long)apex[c],
                1000.0 * tempus[c] / (f64)numerus[c]);
        }
        alioquin
        {
            imprimere("   %2u  %5u   %8.2f        %3u   %8llu   %.4f\n",
                c,
                numerus[c], (f64)summa[c] / (f64)numerus[c], maxima[c],
                (unsigned long long)apex[c],
                1000.0 * tempus[c] / (f64)numerus[c]);
        }
    }
}

/* mensura una retis: aedificatio, tempus puncti, (totum si coniectura
 * < limes) */
interior vacuum
_metiri (
    constans character* sectio,
    constans character* titulus,
         constans i32* pd,
                  i32  c,
                  f64  limes_s,
              Piscina* piscina)
{
                 clock_t  t0 = clock();
    LaqueusModulusVertex* mv = laqueus_modulus_vertex_creare(pd, c,
        OCTETI_MAXIMI, piscina);
                  f64 aedificatio = _secundae(t0);
    ContractioSumptus sumptus;
                  f64 punctum;
                  f64 coniectura;
                  f64 totum          = -1.0;
                  i32 contractiones  = ZEPHYRUM;
                  i32 primi          = ZEPHYRUM;
       memoriae_index usus_ante;
       memoriae_index usus = ZEPHYRUM;
                  s32 imus;
                  s32 summus;
                  i32 bita = ZEPHYRUM;
                  i32 limes;
                  i32 candidatus;

    si (mv == NIHIL)
    {
        sumptus = contractio_sumptus(NIHIL);
        imprimere("%s %s c=%u: officina refutata (> 2 GiB) aut codex "
            "invalidus\n", sectio, titulus, c);
        redde;
    }
    sumptus = laqueus_modulus_vertex_sumptus(mv);
    punctum = _tempus_puncti(mv, III);
    _fenestra(pd, c, piscina, &imus, &summus);
    limes = laqueus_modulus_vertex_limes(mv);
    candidatus =
        contractio_ordo_candidatus(laqueus_modulus_vertex_ordo(mv));
    /* coniectura: (K + 3) puncta per primum, primi = ceil((limes +
     * 1)/31) + 1 (limes T5a) */
    coniectura = punctum * (f64)((summus - imus) / IV + I + II)
        * (f64)((limes + I + XXX) / XXXI + I);
    si (coniectura < limes_s)
    {
        Polynomium uncinus;
               i32 contractiones_ante;
               i32 primi_ante;
               s32 e;

        /* structura sondae reutitur (aedificatio extra tempus et
         * piscinam mensam); numeratores ut differentiae */
        laqueus_modulus_vertex_vocationes(mv, &contractiones_ante,
            &primi_ante);
        usus_ante  = piscina_summa_usus(piscina);
        t0         = clock();
        si (laqueus_modulus_vertex_uncinus(mv, piscina, &uncinus))
        {
            totum  = _secundae(t0);
            laqueus_modulus_vertex_vocationes(mv, &contractiones,
                &primi);
            contractiones  -= contractiones_ante;
            primi          -= primi_ante;
            usus           = piscina_summa_usus(piscina) - usus_ante;
            per (e = polynomium_gradus_imus(uncinus);
                 e <= polynomium_gradus_summus(uncinus); e++)
            {
                i32 b = magnus_bitorum(polynomium_coefficiens(uncinus,
                    e));

                si (b > bita)
                {
                    bita = b;
                }
            }
        }
    }
    si (machina)
    {
        imprimere("MENSURA %s titulus=%s c=%u latitudo=%u apex=%llu "
            "octeti=%llu multiplicationes=%llu aedificatio_s=%.4f "
            "punctum_s=%.6f ns_mult=%.3f praedictio_s=%.2f totum_s=%.3f "
            "contractiones=%u primi=%u octeti_piscinae=%llu bita=%u "
            "limes=%u candidatus=%u\n",
            sectio, titulus, c, sumptus.latitudo,
            (unsigned long long)sumptus.apex,
            (unsigned long long)sumptus.octeti_officinae,
            (unsigned long long)sumptus.multiplicationes, aedificatio,
            punctum, 1e9 * punctum / (f64)sumptus.multiplicationes,
            coniectura, totum, contractiones, primi,
            (unsigned long long)usus, bita, limes, candidatus);
    }
    alioquin
    {
        imprimere("   %-10s %5u  %3u  %9llu  %8.3f  %10.5f  %6.2f  "
            "%9.2f  ", titulus, c, sumptus.latitudo,
            (unsigned long long)sumptus.apex, 1000.0 * aedificatio,
            punctum, 1e9 * punctum / (f64)sumptus.multiplicationes,
            coniectura);
        si (totum >= 0.0)
        {
            imprimere("%8.2f  %6u  %3u  %8.1f  %4u  %4u  %3u\n", totum,
                contractiones, primi, (f64)usus / 1048576.0, bita,
                limes, candidatus);
        }
        alioquin
        {
            imprimere("       -                              %4u  %3u\n",
                limes, candidatus);
        }
    }
    fflush(stdout);
}

interior vacuum
_titulus_columnarum (
    constans character* titulus)
{
    si (!machina)
    {
        imprimere("\n%s\n", titulus);
        imprimere("   reticulum      c  lat       apex   aedif ms   "
            "punctum s  ns/mult  praedictio s  totum s  contr  primi  "
            "MB pisc  bita  limes  cand\n");
    }
}

interior vacuum
_sectio_scalae (vacuum)
{
    constans i32 magnitudines[] = { L, C, CC, CD, DCCC };
            Sors s;
             i32 t;

    sors_seminare(&s, (i64)MMXXVII, ZEPHYRUM);
    _titulus_columnarum("III. scala: summae connexae nodorum tabulae");
    per (t = ZEPHYRUM; t < V; t++)
    {
        Piscina* piscina = piscina_generare_dynamicum("mensura",
            1048576);

        _metiri("III", "summa", _summa_fortuita(&s, magnitudines[t],
            piscina), magnitudines[t], 120.0, piscina);
        piscina_destruere(piscina);
    }
}

interior vacuum
_sectio_tricarum (vacuum)
{
    constans i32 fila[]          = { IV, VI, VIII, X, XII };
    constans i32 longitudines[]  = { L, C, CC };
            Sors s;
             i32 f;
             i32 l;

    sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
    fflush(stdout);
    _titulus_columnarum("IV. clausurae tricarum fortuitarum (titulus = "
        "fila)");
    per (f = ZEPHYRUM; f < V; f++)
    {
        per (l = ZEPHYRUM; l < III; l++)
        {
            Piscina* piscina =
                piscina_generare_dynamicum("mensura",
                1048576);
               character  titulus[XXXII];
                     i32  longitudo = longitudines[l];
            constans i32* pd;

            sprintf(titulus, "plexus%u", fila[f]);
            pd = _plexus_fortuitus(&s, fila[f], &longitudo, piscina,
                NIHIL);
            si (pd == NIHIL)
            {
                imprimere("   %s L=%u: nullum verbum nodi post M conatus\n",
                    titulus, longitudo);
            }
            alioquin
            {
                _metiri("IV", titulus, pd, longitudo, 60.0, piscina);
            }
            fflush(stdout);
            piscina_destruere(piscina);
        }
    }
}

/* V: trica una fortuita, magnitudine data (nodi magni). Non in cursu
 * ordinario - solum per -sectio V. */
interior vacuum
_sectio_singuli (
    i32 fila,
    i32 longitudo,
    f64 limes_s,
    i64 semen)
{
           Piscina* piscina = piscina_generare_dynamicum("mensura",
               1048576);
              Sors  s;
         character  titulus[XXXII];
      constans i32* pd;

    sors_seminare(&s, semen, ZEPHYRUM);
    _titulus_columnarum("V. trica una fortuita (titulus = fila)");
    sprintf(titulus, "plexus%u", fila);
    pd = _plexus_fortuitus(&s, fila, &longitudo, piscina, NIHIL);
    si (pd == NIHIL)
    {
        imprimere("   %s L=%u: nullum verbum nodi post M conatus\n",
            titulus, longitudo);
    }
    alioquin
    {
        _metiri("V", titulus, pd, longitudo, limes_s, piscina);
    }
    fflush(stdout);
    piscina_destruere(piscina);
}


/* ==================================================
 * VI-VIII: via Temperley-Lieb (temperley-celeritas-plan)
 * ================================================== */

/* nucleus VETUS (laqueus 79e7fad9, aestimare post punctum electum),
 * congelatus ad mensuram A/B: tabulae ex API temperley publica */
nomen structura {
             i32   fila;
             i32   longitudo;
    constans s32*  verbum;
             i32   moduli;
             i32*  dimensiones;
             i32** imagines;
              i8** genera;
             i32*  alveus_a;
             i32*  alveus_b;
} NucleusVetus;

interior b32
_vetus_creare (
     constans s32* verbum,
              i32  longitudo,
              i32  fila,
          Piscina* piscina,
     NucleusVetus* nv)
{
    i64 maxima = ZEPHYRUM;
    i32 k;
    i32 i;
    i32 t;

    nv->fila       = fila;
    nv->longitudo  = longitudo;
    nv->verbum     = verbum;
    nv->moduli     = fila / II + I;
    nv->dimensiones = (i32*)piscina_allocare(piscina,
        (memoriae_index)nv->moduli * magnitudo(i32));
    nv->imagines   = (i32**)piscina_allocare(piscina,
        (memoriae_index)nv->moduli * magnitudo(i32*));
    nv->genera     = (i8**)piscina_allocare(piscina,
        (memoriae_index)nv->moduli * magnitudo(i8*));
    per (k = ZEPHYRUM; k < nv->moduli; k++)
    {
        TemperleyModulus* m = temperley_modulus_creare(fila, fila % II
            + II * k, piscina);
        i32 d;
        i32 loca;

        si (m == NIHIL)
        {
            redde FALSUM;
        }
        d                   = temperley_modulus_dimensio(m);
        loca                = (fila - I) * d;
        nv->dimensiones[k]  = d;
        nv->imagines[k] = (i32*)piscina_allocare(piscina,
            (memoriae_index)(loca
                > ZEPHYRUM ? loca : I) * magnitudo(i32));
        nv->genera[k] = (i8*)piscina_allocare(piscina,
            (memoriae_index)(loca
                > ZEPHYRUM ? loca : I) * magnitudo(i8));
        per (i = I; i < fila; i++)
        {
            per (t = ZEPHYRUM; t < d; t++)
            {
                i32 imago = ZEPHYRUM;
                s32 genus = temperley_actio(m, i, t, &imago);

                nv->imagines[k][(i - I) * d + t]  = imago;
                nv->genera[k][(i - I) * d + t]    = (i8)genus;
            }
        }
        si ((i64)d > maxima)
        {
            maxima = (i64)d;
        }
    }
    nv->alveus_a = (i32*)piscina_allocare(piscina,
        (memoriae_index)(maxima
        * maxima) * magnitudo(i32));
    nv->alveus_b = (i32*)piscina_allocare(piscina,
        (memoriae_index)(maxima
        * maxima) * magnitudo(i32));
    redde VERUM;
}

/* valor <clausura> in A modulo p: corpus laqueus_modulus_temperley_
 * aestimare (79e7fad9) post _punctum_eligere, verbatim in sensu */
interior i32
_vetus_valor (
    NucleusVetus* nv,
             i32  a,
             i32  a_inversa,
             i32  delta,
             i32  p)
{
    i32 pondera[TEMPERLEY_FILA_MAXIMA + III];
    i32 summa = ZEPHYRUM;
    i32 delta_inversa;
    i32 k;
    i32 j;

    pondera[ZEPHYRUM]  = I;
    pondera[I]         = delta;
    per (j = I; j + I <= nv->fila; j++)
    {
        pondera[j + I] =
            congruentia_subtrahe(congruentia_multiplica(delta,
            pondera[j], p), pondera[j - I], p);
    }
    per (k = ZEPHYRUM; k < nv->moduli; k++)
    {
        i32  d          = nv->dimensiones[k];
        i32  cellae     = d * d;
        i32* matrix     = nv->alveus_a;
        i32* nova       = nv->alveus_b;
        i32  vestigium  = ZEPHYRUM;
        i32  e;
        i32  t;
        s32  littera_index;

        per (e = ZEPHYRUM; e < cellae; e++)
        {
            matrix[e] = ZEPHYRUM;
        }
        per (t = ZEPHYRUM; t < d; t++)
        {
            matrix[t * d + t] = I;
        }
        per (littera_index = (s32)nv->longitudo - I; littera_index
            >= ZEPHYRUM; littera_index--)
        {
            s32 littera = nv->verbum[littera_index];
            i32 i = (i32)(littera > ZEPHYRUM ? littera : -littera) - I;
            constans i32* imagines = nv->imagines[k] + i * d;
            constans i8* genera = nv->genera[k] + i * d;
            i32 c1 = littera > ZEPHYRUM ? a : a_inversa;
            i32 c2 = littera > ZEPHYRUM ? a_inversa : a;
            i32* permutatio;

            per (e = ZEPHYRUM; e < cellae; e++)
            {
                nova[e] = (i32)(((i64)c1 * (i64)matrix[e]) % (i64)p);
            }
            per (t = ZEPHYRUM; t < d; t++)
            {
                         i32  factor;
                         i32* destinatio;
                constans i32* fons;
                         i32  columna;

                si ((s32)genera[t] == TEMPERLEY_NIHIL)
                {
                    perge;
                }
                factor = (s32)genera[t] == TEMPERLEY_DELTA
                    ? (i32)(((i64)c2 * (i64)delta) % (i64)p) : c2;
                destinatio  = nova + imagines[t] * d;
                fons        = matrix + t * d;
                per (columna = ZEPHYRUM; columna < d; columna++)
                {
                    destinatio[columna] =
                        (i32)(((i64)destinatio[columna]
                        + (i64)factor * (i64)fons[columna]) % (i64)p);
                }
            }
            permutatio  = matrix;
            matrix      = nova;
            nova        = permutatio;
        }
        per (t = ZEPHYRUM; t < d; t++)
        {
            vestigium = congruentia_adde(vestigium, matrix[t * d + t],
                p);
        }
        summa = congruentia_adde(summa, congruentia_multiplica(
            pondera[nv->fila % II + II * k], vestigium, p), p);
    }
    (vacuum)congruentia_inversa(delta, p, &delta_inversa);
    redde congruentia_multiplica(summa, delta_inversa, p);
}

interior i32
_primus_infra_2_31 (vacuum)
{
    i32 p = (i32)0x80000000UL;

    fac
    {
        p = congruentia_primus_infra(p);
    }
    dum (p % IV != I);
    redde p;
}

/* mediana (insertione ordinata, n parvum) */
interior f64
_mediana (
    f64* x,
    i32  n)
{
    i32 i;
    i32 j;

    per (i = I; i < n; i++)
    {
        f64 v = x[i];

        j = i;
        dum (j > ZEPHYRUM && x[j - I] > v)
        {
            x[j] = x[j - I];
            j--;
        }
        x[j] = v;
    }
    redde n % II == I ? x[n / II] : (x[n / II - I] + x[n / II]) / 2.0;
}

#define PARIA_MAXIMA XXXII

interior vacuum
_ab_metiri (
    i32 fila,
    i32 longitudo_petita,
    i32 paria,
    i32 p)
{
    Piscina* piscina = piscina_generare_dynamicum("mensura_ab",
        1048576);
                       Sors  s;
                        i32  longitudo  = longitudo_petita;
                        s32* verbum     = NIHIL;
               constans i32* pd;
               NucleusVetus  nv;
    LaqueusModulusTemperley* mt;
                        f64  rationes[PARIA_MAXIMA];
                        f64  tempora_vetera[PARIA_MAXIMA];
                        f64  tempora_nova[PARIA_MAXIMA];
                        f64  r_min              = 0.0;
                        f64  r_max              = 0.0;
                        f64  summa_quadratorum  = 0.0;
                        i32  errores            = ZEPHYRUM;
                        i32  k;

    sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
    pd = _plexus_fortuitus(&s, fila, &longitudo, piscina, &verbum);
    mt = pd == NIHIL ? NIHIL : laqueus_modulus_temperley_creare(verbum,
        longitudo, fila, (i64)0x80000000UL, piscina);
    si (mt == NIHIL || !_vetus_creare(verbum, longitudo, fila, piscina,
        &nv))
    {
        imprimere("   VI n=%u c=%u: trica aut structura refutata\n",
            fila,
            longitudo);
        piscina_destruere(piscina);
        redde;
    }
    per (k = ZEPHYRUM; k < nv.moduli; k++)
    {
        summa_quadratorum += (f64)nv.dimensiones[k] * (f64)nv.dimensiones[k];
    }
    per (k = ZEPHYRUM; k < paria; k++)
    {
            i32 a            = ZEPHYRUM;
            i32 valor_novus  = ZEPHYRUM;
            i32 valor_vetus  = ZEPHYRUM;
            i32 a_inversa;
            i32 delta;
            f64 tv = 0.0;
            f64 tn = 0.0;
        clock_t t0;
            i32 omega = (i32)(k + II);
            i32 ordo;

        /* punctum ab via nova (communi _punctum_eligere), deinde
         * A^-1 et delta = -A^2 - A^-2 hic */
        si (laqueus_modulus_temperley_aestimare(mt, omega, p, &a,
            &valor_novus) != INTERPOLATIO_UTILE)
        {
            errores++;
            perge;
        }
        (vacuum)congruentia_inversa(a, p, &a_inversa);
        delta = congruentia_subtrahe(ZEPHYRUM, congruentia_adde(
            congruentia_multiplica(a, a, p), congruentia_multiplica(
            a_inversa, a_inversa, p), p), p);
        /* ABBA: par par vetus primum, impar novus primum */
        per (ordo = ZEPHYRUM; ordo < II; ordo++)
        {
            b32 vetus_nunc = (k % II == ZEPHYRUM) == (ordo == ZEPHYRUM);
            i32 punctum = ZEPHYRUM;
            i32 v = ZEPHYRUM;

            t0 = clock();
            si (vetus_nunc)
            {
                valor_vetus = _vetus_valor(&nv, a, a_inversa, delta, p);
                tv = _secundae(t0);
            }
            alioquin
            {
                (vacuum)laqueus_modulus_temperley_aestimare(mt, omega,
                    p,
                    &punctum, &v);
                tn = _secundae(t0);
                si (v != valor_novus || punctum != a)
                {
                    errores++;
                }
            }
        }
        si (valor_vetus != valor_novus)
        {
            errores++;
        }
        tempora_vetera[k]  = tv;
        tempora_nova[k]    = tn;
        rationes[k]        = tn > 0.0 ? tv / tn : 0.0;
        si (k == ZEPHYRUM || rationes[k] < r_min)
        {
            r_min = rationes[k];
        }
        si (k == ZEPHYRUM || rationes[k] > r_max)
        {
            r_max = rationes[k];
        }
    }
    {
        f64 mv             = _mediana(tempora_vetera, paria);
        f64 mn             = _mediana(tempora_nova, paria);
        f64 ratio_mediana  = _mediana(rationes, paria);

        si (machina)
        {
            imprimere("MENSURA VI fila=%u c=%u p=%u paria=%u vetus_s=%.6f "
                "novus_s=%.6f ratio=%.4f ratio_min=%.4f ratio_max=%.4f "
                "ns_vetus=%.4f ns_novus=%.4f errores=%u\n", fila,
                longitudo, p, paria, mv, mn, ratio_mediana, r_min,
                r_max,
                mv * 1e9 / ((f64)longitudo * summa_quadratorum),
                mn * 1e9
                / ((f64)longitudo * summa_quadratorum), errores);
        }
        alioquin
        {
            imprimere("   n=%2u c=%u  vetus %9.4f s  novus %9.4f s  "
                "ratio %.3f [%.3f, %.3f]  ns/(c Catalan) %.3f / %.3f  "
                "%s\n", fila, longitudo, mv, mn, ratio_mediana, r_min,
                r_max,
                mv * 1e9 / ((f64)longitudo * summa_quadratorum),
                mn * 1e9
                / ((f64)longitudo * summa_quadratorum), errores
                == ZEPHYRUM
                ? "valores aequales" : "VALORES DIFFERUNT");
        }
    }
    fflush(stdout);
    piscina_destruere(piscina);
}

interior vacuum
_sectio_ab (
    i32 fila,
    i32 longitudo,
    i32 paria)
{
    i32 p = _primus_infra_2_31();
    i32 n;

    si (paria > PARIA_MAXIMA)
    {
        paria = PARIA_MAXIMA;
    }
    imprimere("\n%s\n",
        "VI. via TL, A/B: nucleus vetus (79e7fad9) contra "
        "bibliothecam");
    imprimere("   p = %u, paria ABBA = %u\n", p, paria);
    si (fila != ZEPHYRUM)
    {
        _ab_metiri(fila, longitudo, paria, p);
        redde;
    }
    per (n = VIII; n <= XVI; n++)
    {
        _ab_metiri(n, longitudo, paria, p);
    }
}

/* VII: K* tricarum probationum viae (probatio_laqueus_rete) */
interior vacuum
_k_metiri (
    constans character* titulus,
         constans s32* verbum,
                   i32  longitudo,
                   i32  fila,
              Piscina* piscina)
{
                 LaqueusVia via = LAQUEUS_VIA_NULLA;
          ContractioSumptus sv;
    LaqueusTemperleySumptus st;
                        f64 k_stella;

    si (!laqueus_via_eligere(verbum, longitudo, fila,
        CONTRACTIO_SATURATUM,
        piscina, &via, &sv, &st))
    {
        imprimere("   %-22s refutata\n", titulus);
        redde;
    }
    k_stella = st.multiplicationes > ZEPHYRUM ? 185.0
        * (f64)sv.multiplicationes / (f64)st.multiplicationes : 0.0;
    imprimere("   %-22s via %-9s K* %10.2f  vertex mult %12llu officina "
        "%10llu  TL mult %12llu octeti %10llu\n", titulus,
        via == LAQUEUS_VIA_VERTEX ? "vertex" : (via
        == LAQUEUS_VIA_TEMPERLEY ? "temperley" : "nulla"), k_stella,
        (unsigned long long)sv.multiplicationes,
        (unsigned long long)sv.octeti_officinae,
        (unsigned long long)st.multiplicationes,
        (unsigned long long)st.octeti);
}

interior vacuum
_sectio_k (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("mensura_k", 1048576);
    constans i32 tricae[VIII][II] = { { XII, CI }, { XII, CCI },
        { VIII, CCI }, { X, CCI }, { X, CDI }, { XII, CDI },
        { VIII, DCCCI }, { XII, DCCCI } };
    constans s32 trica_inversa[XXXII] = { -VI, -II, III, IV, -IV, -III,
        II,
        VI,
        V, -III, -I, II, -VI, -IV, -IV, VI, -III, -VI, III, -VI, III,
            IV,
        III, IV, -II, II, III, III, -III, I, -II, VI };
          s32 sedecim[XVI];
    character titulus[XXXII];
          i32 k;

    imprimere("\n%s\n", "VII. tricae probationum viae: K* (pretium TL "
        "infra K* -> temperley)");
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        PiscinaNotatio  nota = piscina_notare(piscina);
                  Sors  s;
                   i32  longitudo  = (i32)tricae[k][I];
                   s32* verbum     = NIHIL;

        sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
        si (_plexus_fortuitus(&s, (i32)tricae[k][ZEPHYRUM], &longitudo,
            piscina, &verbum) != NIHIL)
        {
            sprintf(titulus, "%u x %u", tricae[k][ZEPHYRUM], longitudo);
            _k_metiri(titulus, verbum, longitudo,
                (i32)tricae[k][ZEPHYRUM],
                piscina);
        }
        piscina_reficere(piscina, nota);
    }
    _k_metiri("7 x 32 (trica_inversa)", trica_inversa, XXXII, VII,
        piscina);
    per (k = ZEPHYRUM; k < XVI; k++)
    {
        sedecim[k] = (s32)(k + I);
    }
    _k_metiri("16: s1..s15", sedecim, XV, XVI, piscina);
    _k_metiri("17: s1..s16", sedecim, XVI, XVII, piscina);
    fflush(stdout);
    piscina_destruere(piscina);
}

/* VIII: uncinus TL totus (tempus processoris) */
interior vacuum
_sectio_uncini_tl (
    i32 fila,
    i32 longitudo)
{
    Piscina* piscina = piscina_generare_dynamicum("mensura_tl",
        1048576);
                       Sors  s;
                        s32* verbum = NIHIL;
    LaqueusModulusTemperley* mt;
                 Polynomium  f;
                    clock_t  t0;

    imprimere("\n%s\n", "VIII. uncinus TL totus");
    sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
    si (_plexus_fortuitus(&s, fila, &longitudo, piscina, &verbum)
        == NIHIL)
    {
        imprimere("   n=%u: nullum verbum nodi\n", fila);
        piscina_destruere(piscina);
        redde;
    }
    mt = laqueus_modulus_temperley_creare(verbum, longitudo, fila,
        (i64)0x80000000UL, piscina);
    t0 = clock();
    si (   mt == NIHIL
        || !laqueus_modulus_temperley_uncinus(mt, piscina, &f))
    {
        imprimere("   n=%u c=%u: REFUTATUM\n", fila, longitudo);
    }
    alioquin
    {
        s32 e;
        i32 bita       = ZEPHYRUM;
        f64 secundae   = _secundae(t0);
        s32 scriptura  = ZEPHYRUM;
        i32 k;

        per (e = polynomium_gradus_imus(f); e
            <= polynomium_gradus_summus(f);
            e++)
        {
            i32 b = magnus_bitorum(polynomium_coefficiens(f, e));

            si (b > bita)
            {
                bita = b;
            }
        }
        imprimere("   n=%u c=%u: %.2f s processoris, A^[%d, %d], "
            "coefficiens maximus %u bita, limes %u\n", fila, longitudo,
            secundae, polynomium_gradus_imus(f),
            polynomium_gradus_summus(f), bita,
            laqueus_modulus_temperley_limes(mt));
        /* linea forma brackets.txt (knotapel D136) ad comparationem */
        per (k = ZEPHYRUM; k < longitudo; k++)
        {
            scriptura += verbum[k] > ZEPHYRUM ? (s32)I : -(s32)I;
        }
        imprimere("UNCINUS plexus%u x %u | n %u | c %u | writhe %d | A^%d ..",
            fila, longitudo, fila, longitudo, scriptura,
            polynomium_gradus_imus(f));
        per (e = polynomium_gradus_imus(f); e
            <= polynomium_gradus_summus(f);
            e++)
        {
            chorda t = magnus_ad_chordam(polynomium_coefficiens(f, e),
                piscina);

            imprimere(" %.*s", (integer)t.mensura,
                (constans character*)t.datum);
        }
        imprimere("\n");
    }
    fflush(stdout);
    piscina_destruere(piscina);
}

/* IX: porta G1 (temperley-celeritas-plan T2) - nucleus in loco cum
 * reductione UNA per % (p < 2^31: alpha M + c S < 2^63) contra
 * bibliothecam, A/B in eodem binario. Cum mensus est, bibliotheca erat
 * nucleus T1 (reductiones duae); post G1 et G2 bibliotheca Montgomery
 * habet, et haec sectio P4 metitur (ratio = bibliotheca / G1). */
nomen structura {
             i32   fila;
             i32   longitudo;
    constans s32*  verbum;
             s32   scriptura;
             i32   moduli;
             i32*  dimensiones;
             i32*  arcus;
             i32** ordines;
              i8** numeri;
             i32** fontes;
             i32*  alveus;
             i32*  linea;
} NucleusUnus;

interior b32
_unus_creare (
     constans s32* verbum,
              i32  longitudo,
              i32  fila,
          Piscina* piscina,
      NucleusUnus* nu)
{
    i32 maxima = ZEPHYRUM;
    i32 k;
    i32 i;
    i32 s;

    nu->fila       = fila;
    nu->longitudo  = longitudo;
    nu->verbum     = verbum;
    nu->scriptura  = ZEPHYRUM;
    per (k = ZEPHYRUM; k < longitudo; k++)
    {
        nu->scriptura += verbum[k] > ZEPHYRUM ? (s32)I : -(s32)I;
    }
    nu->moduli = fila / II + I;
    nu->dimensiones = (i32*)piscina_allocare(piscina,
        (memoriae_index)nu->moduli * magnitudo(i32));
    nu->arcus = (i32*)piscina_allocare(piscina,
        (memoriae_index)nu->moduli * magnitudo(i32));
    nu->ordines = (i32**)piscina_allocare(piscina,
        (memoriae_index)nu->moduli * magnitudo(i32*));
    nu->numeri = (i8**)piscina_allocare(piscina,
        (memoriae_index)nu->moduli * magnitudo(i8*));
    nu->fontes = (i32**)piscina_allocare(piscina,
        (memoriae_index)nu->moduli * magnitudo(i32*));
    per (k = ZEPHYRUM; k < nu->moduli; k++)
    {
        TemperleyModulus* m = temperley_modulus_creare(fila, fila % II
            + II * k, piscina);
        i32  d;
        i32  arcus = ZEPHYRUM;
        i32* locus;

        si (m == NIHIL)
        {
            redde FALSUM;
        }
        d                   = temperley_modulus_dimensio(m);
        nu->dimensiones[k]  = d;
        si (d > maxima)
        {
            maxima = d;
        }
        /* ordines DELTA litterae I numerantur (idem in omni littera) */
        per (s = ZEPHYRUM; s < d && fila > I; s++)
        {
            i32 imago = ZEPHYRUM;

            si (temperley_actio(m, I, s, &imago) == TEMPERLEY_DELTA)
            {
                arcus++;
            }
        }
        nu->arcus[k] = arcus;
        nu->ordines[k] = (i32*)piscina_allocare(piscina,
            (memoriae_index)(
            (fila - I) * arcus + I) * magnitudo(i32));
        nu->numeri[k] = (i8*)piscina_allocare(piscina, (memoriae_index)(
            (fila - I) * arcus + I) * magnitudo(i8));
        nu->fontes[k] = (i32*)piscina_allocare(piscina,
            (memoriae_index)(
            (fila - I) * arcus * fila + I) * magnitudo(i32));
        locus = (i32*)piscina_allocare(piscina, (memoriae_index)(d + I)
            * magnitudo(i32));
        per (i = I; i < fila; i++)
        {
            i32* ordines  = nu->ordines[k] + (i - I) * arcus;
             i8* numeri   = nu->numeri[k] + (i - I) * arcus;
            i32* fontes   = nu->fontes[k] + (i - I) * arcus * fila;
            i32  numerus  = ZEPHYRUM;

            per (s = ZEPHYRUM; s < d; s++)
            {
                i32 imago = ZEPHYRUM;

                si (temperley_actio(m, i, s, &imago) == TEMPERLEY_DELTA)
                {
                    locus[s]          = numerus;
                    ordines[numerus]  = s;
                    numeri[numerus]   = (i8)ZEPHYRUM;
                    numerus++;
                }
            }
            per (s = ZEPHYRUM; s < d; s++)
            {
                i32 imago = ZEPHYRUM;

                si (temperley_actio(m, i, s, &imago) == TEMPERLEY_UNUM)
                {
                    i32 e = locus[imago];

                    fontes[e * fila + (i32)numeri[e]] = s;
                    numeri[e]++;
                }
            }
        }
    }
    nu->alveus = (i32*)piscina_allocare(piscina, (memoriae_index)maxima
        * (memoriae_index)maxima * magnitudo(i32));
    nu->linea = (i32*)piscina_allocare(piscina, (memoriae_index)maxima
        * magnitudo(i32));
    redde VERUM;
}

/* valor in A modulo p (p < 2^31): reductio UNA per elementum DELTA */
interior i32
_unus_valor (
    NucleusUnus* nu,
            i32  a,
            i32  a_inversa,
            i32  delta,
            i32  p)
{
    i32 pondera[TEMPERLEY_FILA_MAXIMA + III];
    i32 alpha[II];
    i32 factor[II];
    i32 quadratum = congruentia_multiplica(a, a, p);
    i32 quadratum_inversum = congruentia_multiplica(a_inversa,
        a_inversa,
        p);
    i32 summa = ZEPHYRUM;
    i32 delta_inversa;
    i32 k;
    i32 j;

    factor[ZEPHYRUM]  = quadratum_inversum;
    factor[I]         = quadratum;
    alpha[ZEPHYRUM] = congruentia_subtrahe(ZEPHYRUM,
        congruentia_multiplica(
        quadratum_inversum, quadratum_inversum, p), p);
    alpha[I] = congruentia_subtrahe(ZEPHYRUM, congruentia_multiplica(
        quadratum, quadratum, p), p);
    pondera[ZEPHYRUM]  = I;
    pondera[I]         = delta;
    per (j = I; j + I <= nu->fila; j++)
    {
        pondera[j + I] =
            congruentia_subtrahe(congruentia_multiplica(delta,
            pondera[j], p), pondera[j - I], p);
    }
    per (k = ZEPHYRUM; k < nu->moduli; k++)
    {
        i32  d          = nu->dimensiones[k];
        i32  arcus      = nu->arcus[k];
        i32  fila       = nu->fila;
        i32* matrix     = nu->alveus;
        i32* linea      = nu->linea;
        i32  vestigium  = ZEPHYRUM;
        i32  e;
        i32  t;
        s32  littera_index;

        per (e = ZEPHYRUM; e < d * d; e++)
        {
            matrix[e] = ZEPHYRUM;
        }
        per (t = ZEPHYRUM; t < d; t++)
        {
            matrix[t * d + t] = I;
        }
        per (littera_index = (s32)nu->longitudo - I; littera_index
            >= ZEPHYRUM; littera_index--)
        {
            s32 littera = nu->verbum[littera_index];
            i32 i = (i32)(littera > ZEPHYRUM ? littera : -littera) - I;
            i32 signum = littera > ZEPHYRUM ? ZEPHYRUM : I;
            i64 al = (i64)alpha[signum];
            i64 c = (i64)factor[signum];
            constans i32* ordines = nu->ordines[k] + i * arcus;
            constans i8* numeri = nu->numeri[k] + i * arcus;
            constans i32* fontes = nu->fontes[k] + i * arcus * fila;
            i32 r;

            per (r = ZEPHYRUM; r < arcus; r++)
            {
                i32* destinatio  = matrix + ordines[r] * d;
                i32  numerus     = (i32)numeri[r];
                i32  q;
                i32  columna;

                per (columna = ZEPHYRUM; columna < d; columna++)
                {
                    linea[columna] = ZEPHYRUM;
                }
                per (q = ZEPHYRUM; q < numerus; q++)
                {
                    constans i32* fons = matrix + fontes[r * fila
                        + q] * d;

                    per (columna = ZEPHYRUM; columna < d; columna++)
                    {
                        i32 x = linea[columna] + fons[columna];

                        linea[columna] = x >= p ? x - p : x;
                    }
                }
                /* p < 2^31: alpha M + c S < 2 (p-1)^2 < 2^63 */
                per (columna = ZEPHYRUM; columna < d; columna++)
                {
                    destinatio[columna] =
                        (i32)((al * (i64)destinatio[columna]
                        + c * (i64)linea[columna]) % (i64)p);
                }
            }
        }
        per (t = ZEPHYRUM; t < d; t++)
        {
            vestigium = congruentia_adde(vestigium, matrix[t * d + t],
                p);
        }
        summa = congruentia_adde(summa, congruentia_multiplica(
            pondera[nu->fila % II + II * k], vestigium, p), p);
    }
    summa = congruentia_multiplica(summa, nu->scriptura >= ZEPHYRUM
        ? congruentia_potentia(a, (i64)nu->scriptura, p)
        : congruentia_potentia(a_inversa, (i64)(-nu->scriptura), p), p);
    (vacuum)congruentia_inversa(delta, p, &delta_inversa);
    redde congruentia_multiplica(summa, delta_inversa, p);
}

interior vacuum
_g1_metiri (
    i32 fila,
    i32 longitudo_petita,
    i32 paria,
    i32 p)
{
    Piscina* piscina = piscina_generare_dynamicum("mensura_g1",
        1048576);
                       Sors  s;
                        i32  longitudo  = longitudo_petita;
                        s32* verbum     = NIHIL;
                NucleusUnus  nu;
    LaqueusModulusTemperley* mt;
                        f64  rationes[PARIA_MAXIMA];
                        f64  tempora_bibliothecae[PARIA_MAXIMA];
                        f64  tempora_unius[PARIA_MAXIMA];
                        f64  r_min    = 0.0;
                        f64  r_max    = 0.0;
                        i32  errores  = ZEPHYRUM;
                        i32  k;

    sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
    mt = _plexus_fortuitus(&s, fila, &longitudo, piscina, &verbum)
        == NIHIL
        ? NIHIL : laqueus_modulus_temperley_creare(verbum, longitudo,
        fila,
        (i64)0x80000000UL, piscina);
    si (mt == NIHIL || !_unus_creare(verbum, longitudo, fila, piscina,
        &nu))
    {
        imprimere("   IX n=%u c=%u: trica aut structura refutata\n",
            fila,
            longitudo);
        piscina_destruere(piscina);
        redde;
    }
    per (k = ZEPHYRUM; k < paria; k++)
    {
            i32 a            = ZEPHYRUM;
            i32 valor        = ZEPHYRUM;
            i32 valor_unius  = ZEPHYRUM;
            i32 a_inversa;
            i32 delta;
            f64 tb = 0.0;
            f64 tu = 0.0;
        clock_t t0;
            i32 omega = (i32)(k + II);
            i32 ordo;

        si (laqueus_modulus_temperley_aestimare(mt, omega, p, &a,
            &valor)
            != INTERPOLATIO_UTILE)
        {
            errores++;
            perge;
        }
        (vacuum)congruentia_inversa(a, p, &a_inversa);
        delta = congruentia_subtrahe(ZEPHYRUM, congruentia_adde(
            congruentia_multiplica(a, a, p), congruentia_multiplica(
            a_inversa, a_inversa, p), p), p);
        per (ordo = ZEPHYRUM; ordo < II; ordo++)
        {
            b32 bibliotheca_nunc = (k % II == ZEPHYRUM) == (ordo
                == ZEPHYRUM);
            i32 punctum  = ZEPHYRUM;
            i32 v        = ZEPHYRUM;

            t0 = clock();
            si (bibliotheca_nunc)
            {
                (vacuum)laqueus_modulus_temperley_aestimare(mt, omega,
                    p,
                    &punctum, &v);
                tb = _secundae(t0);
                si (v != valor)
                {
                    errores++;
                }
            }
            alioquin
            {
                valor_unius  = _unus_valor(&nu, a, a_inversa, delta, p);
                tu           = _secundae(t0);
            }
        }
        si (valor_unius != valor)
        {
            errores++;
        }
        tempora_bibliothecae[k]  = tb;
        tempora_unius[k]         = tu;
        rationes[k]              = tu > 0.0 ? tb / tu : 0.0;
        si (k == ZEPHYRUM || rationes[k] < r_min)
        {
            r_min = rationes[k];
        }
        si (k == ZEPHYRUM || rationes[k] > r_max)
        {
            r_max = rationes[k];
        }
    }
    imprimere("   n=%2u c=%u  bibliotheca (nucleus currens) %9.4f s  unus (G1) "
        "%9.4f s  ratio %.3f [%.3f, %.3f]  %s\n", fila, longitudo,
        _mediana(tempora_bibliothecae, paria), _mediana(tempora_unius,
        paria), _mediana(rationes, paria), r_min, r_max, errores
        == ZEPHYRUM ? "valores aequales" : "VALORES DIFFERUNT");
    fflush(stdout);
    piscina_destruere(piscina);
}

interior vacuum
_sectio_g1 (
    i32 fila,
    i32 longitudo,
    i32 paria)
{
    i32 p = _primus_infra_2_31();

    si (paria > PARIA_MAXIMA)
    {
        paria = PARIA_MAXIMA;
    }
    imprimere("\nIX. porta G1: reductio una (p < 2^31) contra bibliothecam\n");
    imprimere("   p = %u, paria ABBA = %u\n", p, paria);
    si (fila != ZEPHYRUM)
    {
        _g1_metiri(fila, longitudo, paria, p);
        redde;
    }
    _g1_metiri(XII, longitudo, paria, p);
    _g1_metiri(XVI, longitudo, paria, p);
}

/* X: porta G2 (temperley-celeritas-plan T2) - reductio Montgomery (R =
 * 2^32 in alpha et c inclusa, matrix in forma normali) contra
 * bibliothecam (G1: reductio una per %), A/B in eodem binario */
interior i32
_montgomery_inversa (
    i32 p)
{
    i32 x = p;          /* p x = 1 mod 8 pro p impari */
    i32 k;

    per (k = ZEPHYRUM; k < IV; k++)
    {
        x = x * ((i32)II - p * x);
    }
    redde (i32)ZEPHYRUM - x;     /* p' = -p^-1 mod 2^32 */
}

interior i32
_montgomery_valor (
    NucleusUnus* nu,
            i32  a,
            i32  a_inversa,
            i32  delta,
            i32  p)
{
    i32 pondera[TEMPERLEY_FILA_MAXIMA + III];
    i64 alpha_r[II];
    i64 factor_r[II];
    i32 quadratum = congruentia_multiplica(a, a, p);
    i32 quadratum_inversum = congruentia_multiplica(a_inversa,
        a_inversa,
        p);
    i32 p_prima  = _montgomery_inversa(p);
    i32 summa    = ZEPHYRUM;
    i32 delta_inversa;
    i32 k;
    i32 j;

    factor_r[ZEPHYRUM]  = ((i64)quadratum_inversum << XXXII) % (i64)p;
    factor_r[I]         = ((i64)quadratum << XXXII) % (i64)p;
    alpha_r[ZEPHYRUM] = ((i64)congruentia_subtrahe(ZEPHYRUM,
        congruentia_multiplica(quadratum_inversum, quadratum_inversum,
        p),
        p) << XXXII) % (i64)p;
    alpha_r[I] = ((i64)congruentia_subtrahe(ZEPHYRUM,
        congruentia_multiplica(
        quadratum, quadratum, p), p) << XXXII) % (i64)p;
    pondera[ZEPHYRUM]  = I;
    pondera[I]         = delta;
    per (j = I; j + I <= nu->fila; j++)
    {
        pondera[j + I] =
            congruentia_subtrahe(congruentia_multiplica(delta,
            pondera[j], p), pondera[j - I], p);
    }
    per (k = ZEPHYRUM; k < nu->moduli; k++)
    {
        i32  d          = nu->dimensiones[k];
        i32  arcus      = nu->arcus[k];
        i32  fila       = nu->fila;
        i32* matrix     = nu->alveus;
        i32* linea      = nu->linea;
        i32  vestigium  = ZEPHYRUM;
        i32  e;
        i32  t;
        s32  littera_index;

        per (e = ZEPHYRUM; e < d * d; e++)
        {
            matrix[e] = ZEPHYRUM;
        }
        per (t = ZEPHYRUM; t < d; t++)
        {
            matrix[t * d + t] = I;
        }
        per (littera_index = (s32)nu->longitudo - I; littera_index
            >= ZEPHYRUM; littera_index--)
        {
            s32 littera = nu->verbum[littera_index];
            i32 i = (i32)(littera > ZEPHYRUM ? littera : -littera) - I;
            i32 signum = littera > ZEPHYRUM ? ZEPHYRUM : I;
            i64 al = alpha_r[signum];
            i64 c = factor_r[signum];
            constans i32* ordines = nu->ordines[k] + i * arcus;
            constans i8* numeri = nu->numeri[k] + i * arcus;
            constans i32* fontes = nu->fontes[k] + i * arcus * fila;
            i32 r;

            per (r = ZEPHYRUM; r < arcus; r++)
            {
                i32* destinatio  = matrix + ordines[r] * d;
                i32  numerus     = (i32)numeri[r];
                i32  q;
                i32  columna;

                per (columna = ZEPHYRUM; columna < d; columna++)
                {
                    linea[columna] = ZEPHYRUM;
                }
                per (q = ZEPHYRUM; q < numerus; q++)
                {
                    constans i32* fons = matrix + fontes[r * fila
                        + q] * d;

                    per (columna = ZEPHYRUM; columna < d; columna++)
                    {
                        i32 x = linea[columna] + fons[columna];

                        linea[columna] = x >= p ? x - p : x;
                    }
                }
                /* REDC(t), t = alpha R M + c R S < 2^63; t + m p < 2^64 */
                per (columna = ZEPHYRUM; columna < d; columna++)
                {
                    i64 tt = al * (i64)destinatio[columna] + c
                        * (i64)linea[columna];
                    i32 m = (i32)tt * p_prima;
                    i64 u = (tt + (i64)m * (i64)p) >> XXXII;

                    destinatio[columna] = (i32)(u >= (i64)p ? u
                        - (i64)p : u);
                }
            }
        }
        per (t = ZEPHYRUM; t < d; t++)
        {
            vestigium = congruentia_adde(vestigium, matrix[t * d + t],
                p);
        }
        summa = congruentia_adde(summa, congruentia_multiplica(
            pondera[nu->fila % II + II * k], vestigium, p), p);
    }
    summa = congruentia_multiplica(summa, nu->scriptura >= ZEPHYRUM
        ? congruentia_potentia(a, (i64)nu->scriptura, p)
        : congruentia_potentia(a_inversa, (i64)(-nu->scriptura), p), p);
    (vacuum)congruentia_inversa(delta, p, &delta_inversa);
    redde congruentia_multiplica(summa, delta_inversa, p);
}

interior vacuum
_g2_metiri (
    i32 fila,
    i32 longitudo_petita,
    i32 paria,
    i32 p)
{
    Piscina* piscina = piscina_generare_dynamicum("mensura_g2",
        1048576);
                       Sors  s;
                        i32  longitudo  = longitudo_petita;
                        s32* verbum     = NIHIL;
                NucleusUnus  nu;
    LaqueusModulusTemperley* mt;
                        f64  rationes[PARIA_MAXIMA];
                        f64  tempora_bibliothecae[PARIA_MAXIMA];
                        f64  tempora_montgomery[PARIA_MAXIMA];
                        f64  r_min    = 0.0;
                        f64  r_max    = 0.0;
                        i32  errores  = ZEPHYRUM;
                        i32  k;

    sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
    mt = _plexus_fortuitus(&s, fila, &longitudo, piscina, &verbum)
        == NIHIL
        ? NIHIL : laqueus_modulus_temperley_creare(verbum, longitudo,
        fila,
        (i64)0x80000000UL, piscina);
    si (mt == NIHIL || !_unus_creare(verbum, longitudo, fila, piscina,
        &nu))
    {
        imprimere("   X n=%u c=%u: trica aut structura refutata\n",
            fila,
            longitudo);
        piscina_destruere(piscina);
        redde;
    }
    per (k = ZEPHYRUM; k < paria; k++)
    {
            i32 a                 = ZEPHYRUM;
            i32 valor             = ZEPHYRUM;
            i32 valor_montgomery  = ZEPHYRUM;
            i32 a_inversa;
            i32 delta;
            f64 tb = 0.0;
            f64 tm = 0.0;
        clock_t t0;
            i32 omega = (i32)(k + II);
            i32 ordo;

        si (laqueus_modulus_temperley_aestimare(mt, omega, p, &a,
            &valor)
            != INTERPOLATIO_UTILE)
        {
            errores++;
            perge;
        }
        (vacuum)congruentia_inversa(a, p, &a_inversa);
        delta = congruentia_subtrahe(ZEPHYRUM, congruentia_adde(
            congruentia_multiplica(a, a, p), congruentia_multiplica(
            a_inversa, a_inversa, p), p), p);
        per (ordo = ZEPHYRUM; ordo < II; ordo++)
        {
            b32 bibliotheca_nunc = (k % II == ZEPHYRUM) == (ordo
                == ZEPHYRUM);
            i32 punctum  = ZEPHYRUM;
            i32 v        = ZEPHYRUM;

            t0 = clock();
            si (bibliotheca_nunc)
            {
                (vacuum)laqueus_modulus_temperley_aestimare(mt, omega,
                    p,
                    &punctum, &v);
                tb = _secundae(t0);
                si (v != valor)
                {
                    errores++;
                }
            }
            alioquin
            {
                valor_montgomery = _montgomery_valor(&nu, a, a_inversa,
                    delta, p);
                tm = _secundae(t0);
            }
        }
        si (valor_montgomery != valor)
        {
            errores++;
        }
        tempora_bibliothecae[k]  = tb;
        tempora_montgomery[k]    = tm;
        rationes[k]              = tm > 0.0 ? tb / tm : 0.0;
        si (k == ZEPHYRUM || rationes[k] < r_min)
        {
            r_min = rationes[k];
        }
        si (k == ZEPHYRUM || rationes[k] > r_max)
        {
            r_max = rationes[k];
        }
    }
    imprimere("   n=%2u c=%u  bibliotheca (%%) %9.4f s  montgomery %9.4f s  "
        "ratio %.3f [%.3f, %.3f]  %s\n", fila, longitudo,
        _mediana(tempora_bibliothecae, paria),
        _mediana(tempora_montgomery,
        paria), _mediana(rationes, paria), r_min, r_max, errores
        == ZEPHYRUM ? "valores aequales" : "VALORES DIFFERUNT");
    fflush(stdout);
    piscina_destruere(piscina);
}

interior vacuum
_sectio_g2 (
    i32 fila,
    i32 longitudo,
    i32 paria)
{
    i32 p = _primus_infra_2_31();

    si (paria > PARIA_MAXIMA)
    {
        paria = PARIA_MAXIMA;
    }
    imprimere("\nX. porta G2: Montgomery contra bibliothecam (G1)\n");
    imprimere("   p = %u, paria ABBA = %u\n", p, paria);
    si (fila != ZEPHYRUM)
    {
        _g2_metiri(fila, longitudo, paria, p);
        redde;
    }
    _g2_metiri(XII, longitudo, paria, p);
    _g2_metiri(XVI, longitudo, paria, p);
}

/* trica k (0 .. XLIV) tabulae D136: XL parvae (III..XII fila x XX, L,
 * CI, CCI) deinde V magnae */
interior vacuum
_trica_probationis (
    i32  k,
    i32* fila,
    i32* longitudo)
{
    constans i32 longitudines[IV] = { XX, L, CI, CCI };
    constans i32 magnae[V][II] = { { VIII, CDI }, { VIII, DCCCI },
        { X, CDI }, { XII, CDI }, { XII, DCCCI } };

    *fila = k < XL ? III + k / IV : (i32)magnae[k - XL][ZEPHYRUM];
    *longitudo = k < XL ? longitudines[k % IV] : (i32)magnae[k
        - XL][I];
}

/* XI: acceptatio (temperley-celeritas-plan T3) - omnes XLV uncini
 * D136 per laqueus_uncinus_ex_plexu (via electa), lineae formae
 * brackets.txt (knotapel D136) ad comparationem integram */
interior vacuum
_sectio_probationis_finalis (vacuum)
{
    i32 k;

    imprimere("\nXI. acceptatio: uncini D136 per viam electam\n");
    per (k = ZEPHYRUM; k < XL + V; k++)
    {
        Piscina* piscina = piscina_generare_dynamicum("mensura_acc",
            1048576);
              Sors  s;
               i32  fila;
               i32  longitudo;
               s32* verbum = NIHIL;
        Polynomium  f;
        LaqueusVia  via = LAQUEUS_VIA_NULLA;
           clock_t  t0;

        _trica_probationis(k, &fila, &longitudo);
        sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
        si (_plexus_fortuitus(&s, fila, &longitudo, piscina, &verbum)
            == NIHIL)
        {
            imprimere("   %u x %u: nullum verbum\n", fila, longitudo);
            piscina_destruere(piscina);
            perge;
        }
        t0 = clock();
        si (!laqueus_uncinus_ex_plexu(verbum, longitudo, fila,
            CONTRACTIO_SATURATUM, piscina, &f, &via))
        {
            imprimere("   %u x %u: REFUTATUM\n", fila, longitudo);
        }
        alioquin
        {
            s32 e;
            s32 scriptura = ZEPHYRUM;
            i32 j;

            per (j = ZEPHYRUM; j < longitudo; j++)
            {
                scriptura += verbum[j] > ZEPHYRUM ? (s32)I : -(s32)I;
            }
            imprimere("   %u x %u: via %s, %.2f s\n", fila, longitudo,
                via == LAQUEUS_VIA_VERTEX ? "vertex" : "temperley",
                _secundae(t0));
            imprimere("UNCINUS plexus%u x %u | n %u | c %u | writhe %d | "
                "A^%d ..", fila, longitudo, fila, longitudo, scriptura,
                polynomium_gradus_imus(f));
            per (e = polynomium_gradus_imus(f); e
                <= polynomium_gradus_summus(f); e++)
            {
                chorda t = magnus_ad_chordam(polynomium_coefficiens(f,
                    e),
                    piscina);

                imprimere(" %.*s", (integer)t.mensura,
                    (constans character*)t.datum);
            }
            imprimere("\n");
        }
        fflush(stdout);
        piscina_destruere(piscina);
    }
}

/* XII: verba tricarum (generator idem ac VIII et XI: semen MMXXVIII
 * per (n, L)) - instrumenta externa (Regina) eas ipsa in PD vertunt.
 * fila == ZEPHYRUM: omnes XLV tricae D136; aliter una. */
interior vacuum
_sectio_verborum (
    i32 fila_data,
    i32 longitudo_data)
{
    i32 k;
    i32 numerus = fila_data == ZEPHYRUM ? XL + V : I;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        Piscina* piscina = piscina_generare_dynamicum("mensura_verba",
            1048576);
            Sors  s;
             i32  fila       = fila_data;
             i32  longitudo  = longitudo_data;
             s32* verbum     = NIHIL;
             i32  j;

        si (fila_data == ZEPHYRUM)
        {
            _trica_probationis(k, &fila, &longitudo);
        }
        sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
        si (_plexus_fortuitus(&s, fila, &longitudo, piscina, &verbum)
            == NIHIL)
        {
            imprimere("   %u x %u: nullum verbum\n", fila, longitudo);
            piscina_destruere(piscina);
            perge;
        }
        imprimere("VERBUM %u %u |", fila, longitudo);
        per (j = ZEPHYRUM; j < longitudo; j++)
        {
            imprimere(" %d", verbum[j]);
        }
        imprimere("\n");
        piscina_destruere(piscina);
    }
}


/* ==================================================
 * XIII-XIV: fundamentum verificationis (interpolatio-verificatio-plan
 * T0) - numeri sine aestimatione ulla
 * ================================================== */

#define PRIMI_SERIEI_MAXIMI 160      /* limes <= 2c bita; 2c/30 + 2 */
#define TEXTUS_UNCINORUM_MAXIMI IV

/* floor(log2 p), p >= 2 (idem ac interpolatio _bita_infra) */
interior i32
_bita_primi (
    i32 p)
{
    i32 bita = ZEPHYRUM;

    dum (p > I)
    {
        p >>= I;
        bita++;
    }
    redde bita;
}

/* primus maximus < limes, = 1 (mod 4), aptus: (p - 1)/4 >= necessaria
 * - idem ac interpolatio _primus_proximus cum residuo I, modulo IV,
 * passu IV (mdc(4, p - 1) = 4); 0 si nullus */
interior i32
_primus_seriei (
    s64 limes,
    s64 necessaria)
{
    s64 n = limes - (s64)I;

    n = n - ((n % (s64)IV - (s64)I + (s64)IV) % (s64)IV);
    dum (n >= (s64)V)
    {
        si (   congruentia_est_primus((i32)n)
            && (n - (s64)I) / (s64)IV >= necessaria)
        {
            redde (i32)n;
        }
        n -= (s64)IV;
    }
    redde ZEPHYRUM;
}

nomen structura {
    i32 k_maximus;   /* primi restitutionis PROBATUM, sine addito */
    i32 k_verus;     /* primus k: 2 max|c| < M_k (0 si ignotus) */
    i32 k_magnus;    /* primus k: M_k >= 2^(limes + 1) exacte (L2) */
    b32 violatus;    /* max|c| >= 2^limes: limes probatus fallit */
    i32 primi[PRIMI_SERIEI_MAXIMI];
} SeriesPrimorum;

/* series ut interpolatio_polynomium: primi descendentes infra
 * limes_primorum donec summa floor(log2 p) >= limes + 1, deinde
 * ADDITUS (primi[k_maximus]). maximus: |c| maximus uncini noti, aut
 * NIHIL. FALSUM si primi desunt aut series nimis longa. */
interior b32
_seriem_computare (
                 s64  limes_primorum,
                 i32  limes,
                 s64  necessaria,
     constans Magnus* maximus,
             Piscina* piscina,
      SeriesPrimorum* series)
{
       s64 bita       = ZEPHYRUM;
       s64 p          = limes_primorum;
    Magnus productum  = magnus_ex_s64((s64)I);
    Magnus bis_maximus;
       i32 k    = ZEPHYRUM;
       i32 j;

    dum (bita < (s64)limes + (s64)I)
    {
        si (k + I >= PRIMI_SERIEI_MAXIMI)
        {
            redde FALSUM;
        }
        p = (s64)_primus_seriei(p, necessaria);
        si (p == ZEPHYRUM)
        {
            redde FALSUM;
        }
        series->primi[k++]  = (i32)p;
        bita                += (s64)_bita_primi((i32)p);
    }
    series->k_maximus  = k;
    series->primi[k]   = _primus_seriei(p, necessaria);
    si (series->primi[k] == ZEPHYRUM)
    {
        redde FALSUM;
    }
    series->k_verus   = ZEPHYRUM;
    series->k_magnus  = ZEPHYRUM;
    series->violatus  = maximus != NIHIL
        && magnus_bitorum(*maximus) > limes;
    bis_maximus = maximus != NIHIL ? magnus_adde(*maximus, *maximus,
        piscina)
        : productum;
    per (j = ZEPHYRUM; j < k; j++)
    {
        productum = magnus_multiplica(productum,
            magnus_ex_s64((s64)series->primi[j]),
            piscina);
        si (   maximus != NIHIL && series->k_verus == ZEPHYRUM
            && magnus_compara(bis_maximus, productum) < ZEPHYRUM)
        {
            series->k_verus = j + I;
        }
        si (   series->k_magnus          == ZEPHYRUM
            && magnus_bitorum(productum) >= limes + II)
        {
            series->k_magnus = j + I;
        }
    }
    redde VERUM;
}

/* integer signatus ex datis[*positio..]: '-' optionale, digiti */
interior b32
_integrum_legere (
     constans i8* data,
             i32  finis,
             i32* positio,
             s32* valor)
{
    s32 signum  = I;
    s32 v       = ZEPHYRUM;
    i32 initium;

    si (*positio < finis && data[*positio] == '-')
    {
        signum = -(s32)I;
        (*positio)++;
    }
    initium = *positio;
    dum (   *positio < finis && data[*positio] >= '0'
         && data[*positio] <= '9')
    {
        v = v * (s32)X + (s32)(data[*positio] - '0');
        (*positio)++;
    }
    *valor = signum * v;
    redde *positio > initium;
}

/* positio primae occurrentiae signi in [initium, finis); finis si
 * nulla */
interior i32
_quaerere_in_linea (
           constans i8* data,
                   i32  initium,
                   i32  finis,
    constans character* signum)
{
    i32 mensura = (i32)strlen(signum);
    i32 k;

    per (k = initium; k + mensura <= finis; k++)
    {
        si (memcmp(data + k, signum, (size_t)mensura) == ZEPHYRUM)
        {
            redde k;
        }
    }
    redde finis;
}

/* linea "plexusN x L | n N | c L | writhe W | A^lo .. c c ..." (forma
 * brackets.txt) in textibus: scriptura, imus verus, numerus
 * coefficientium (exponentes consecutivi), |c| maximus. FALSUM si
 * nulla aut malformata. */
interior b32
_uncinum_notum_legere (
    constans chorda* textus,
                i32  numerus_textuum,
                i32  fila,
                i32  longitudo,
            Piscina* piscina,
                s32* scriptura,
                s32* imus_verus,
                i32* numerus,
             Magnus* maximus)
{
    character praefixum[LXIV];
          i32 mensura_praefixi;
          i32 t;

    sprintf(praefixum, "plexus%u x %u |", fila, longitudo);
    mensura_praefixi = (i32)strlen(praefixum);
    per (t = ZEPHYRUM; t < numerus_textuum; t++)
    {
         i8* data     = textus[t].datum;
        i32  finis    = textus[t].mensura;
        i32  initium  = ZEPHYRUM;

        dum (initium < finis)
        {
            i32 terminus = initium;
            i32 k;

            dum (terminus < finis && data[terminus] != '\n')
            {
                terminus++;
            }
            si (   terminus - initium >= mensura_praefixi
                && memcmp(data + initium, praefixum,
                (size_t)mensura_praefixi) == ZEPHYRUM)
            {
                k = _quaerere_in_linea(data, initium, terminus,
                    "writhe ") + VII;
                si (   k > terminus
                    || !_integrum_legere(data, terminus, &k,
                    scriptura))
                {
                    redde FALSUM;
                }
                k = _quaerere_in_linea(data, k, terminus, "A^") + II;
                si (   k > terminus
                    || !_integrum_legere(data, terminus, &k,
                    imus_verus))
                {
                    redde FALSUM;
                }
                k = _quaerere_in_linea(data, k, terminus, "..") + II;
                *numerus = ZEPHYRUM;
                *maximus = magnus_ex_s64(ZEPHYRUM);
                dum (k < terminus)
                {
                    chorda signum_c;
                    Magnus c;
                       i32 initium_signi;

                    dum (k < terminus && data[k] == ' ')
                    {
                        k++;
                    }
                    si (k >= terminus)
                    {
                        frange;
                    }
                    initium_signi = k;
                    dum (k < terminus && data[k] != ' ')
                    {
                        k++;
                    }
                    signum_c.datum    = data + initium_signi;
                    signum_c.mensura  = k - initium_signi;
                    si (!magnus_ex_chorda(signum_c, piscina, &c))
                    {
                        redde FALSUM;
                    }
                    c = magnus_absolutum(c, piscina);
                    si (magnus_compara(c, *maximus) > ZEPHYRUM)
                    {
                        *maximus = c;
                    }
                    (*numerus)++;
                }
                redde *numerus > ZEPHYRUM;
            }
            initium = terminus + I;
        }
    }
    redde FALSUM;
}

/* m: minimus cum (k_maximus - 1) eps1^m <= 2^-s (s = LXIV), EXACTE:
 * eps1 = a/b = c (K - 1)/(2^30 - r) + floor((limes + 1)/30) / 2^24,
 * c = 8, r = 9; comparatio (k_maximus - 1) a^m 2^s <= b^m in Magnus
 * (T0: recipe integra floor/ceil recensionis M3 m uno nimis dat in XIII
 * tricis). k_maximus <= 1: 0 (nullus gradus ante limitem). *exponens =
 * floor(-log2 eps1) et *epsilon (f64) descriptiva tantum. */
interior i32
_probationes_necessariae (
        i32  coefficientes,
        i32  limes,
        i32  k_maximus,
    Piscina* piscina,
        i32* exponens,
        f64* epsilon)
{
    constans s64 basis      = (s64)0x40000000UL - (s64)IX;   /* 2^30 - r */
    constans s64 duae_xxiv  = (s64)0x1000000UL;          /* 2^24 */
             f64 periculum = 8.0 * (f64)(coefficientes - I) / (f64)basis
                 + (f64)((limes + I) / XXX) / (f64)duae_xxiv;
       f64 t = 1.0;
       i32 e = ZEPHYRUM;
       i32 m;
    Magnus a;
    Magnus b;
    Magnus sinistra;
    Magnus dextra;

    dum (periculum <= t / 2.0 && e < LXIV)
    {
        t /= 2.0;
        e++;
    }
    *exponens  = e;
    *epsilon   = periculum;
    si (k_maximus <= I)
    {
        redde ZEPHYRUM;
    }
    a = magnus_adde(magnus_ex_s64((s64)VIII * (s64)(coefficientes - I)
        * duae_xxiv), magnus_ex_s64((s64)((limes + I) / XXX) * basis),
        piscina);
    b = magnus_ex_s64(basis * duae_xxiv);
    /* sinistra = (k - 1) 2^64 a^m, dextra = b^m */
    sinistra = magnus_multiplica(magnus_ex_s64((s64)(k_maximus - I)),
        magnus_multiplica(magnus_ex_s64((s64)0x100000000UL),
        magnus_ex_s64((s64)0x100000000UL), piscina), piscina);
    dextra = magnus_ex_s64((s64)I);
    per (m = I; m <= LXIV; m++)
    {
        sinistra  = magnus_multiplica(sinistra, a, piscina);
        dextra    = magnus_multiplica(dextra, b, piscina);
        si (magnus_compara(sinistra, dextra) <= ZEPHYRUM)
        {
            redde m;
        }
    }
    redde ZEPHYRUM;
}

/* XIII: una trica - fenestra a priori, limes, series TL (primi < 2^31)
 * et vertex (< 2^32), via electa (octeti sine limite, ut XI), sumptus
 * vertex, k verus ex uncino noto, aestimationes PROBATUM et
 * VERIFICATUM in via electa */
interior vacuum
_fundamentum_tricae (
             i32     fila,
             i32     longitudo_data,
    constans chorda* textus,
             i32     numerus_textuum)
{
                Piscina* piscina = piscina_generare_dynamicum(
                    "mensura_fundamentum", 1048576);
                   Sors  s;
                    i32  longitudo  = longitudo_data;
                    s32* verbum     = NIHIL;
                    i32* pd;
                    i32  transitus;
                    s32  imus;
                    s32  summus;
                    i32  limes;
                 Magnus  limes_exactus;
                    i32  coefficientes;
                    s64  necessaria;
                    s32  scriptura       = ZEPHYRUM;
                    s32  scriptura_nota  = ZEPHYRUM;
                    s32  imus_verus      = ZEPHYRUM;
                    i32  numerus_notus   = ZEPHYRUM;
                 Magnus  maximus;
                    b32  notus;
         SeriesPrimorum  series_temperley;
         SeriesPrimorum  series_vertex;
         SeriesPrimorum* electa;
             LaqueusVia  via = LAQUEUS_VIA_NULLA;
      ContractioSumptus  sv;
LaqueusTemperleySumptus  st;
                    i32  e;
                    i32  m;
                    f64  periculum;
                    i32  per_primum;
                    i32  k_finis;
                    i64  probatum;
                    i64  verificatum;
                    i64  principales;
                    i32  j;

    sors_seminare(&s, (i64)MMXXVIII, ZEPHYRUM);
    si (   _plexus_fortuitus(&s, fila, &longitudo, piscina, &verbum)
        == NIHIL
        || !laqueus_pd_ex_plexu(verbum, longitudo, fila, piscina, &pd,
        &transitus)
        || !laqueus_pd_fenestra(pd, transitus, piscina, &imus, &summus,
        &limes, &limes_exactus))
    {
        imprimere("   %u x %u: REFUTATUM (verbum, pd aut fenestra)\n",
            fila,
            longitudo);
        piscina_destruere(piscina);
        redde;
    }
    per (j = ZEPHYRUM; j < longitudo; j++)
    {
        scriptura += verbum[j] > ZEPHYRUM ? (s32)I : -(s32)I;
    }
    coefficientes  = (i32)((summus - imus) / IV) + I;
    necessaria     = (s64)coefficientes + (s64)II;
    per_primum     = coefficientes + II;
    notus = _uncinum_notum_legere(textus, numerus_textuum, fila,
        longitudo, piscina, &scriptura_nota, &imus_verus,
        &numerus_notus,
        &maximus);
    si (   !_seriem_computare((s64)0x80000000UL, limes, necessaria,
           notus ? &maximus : NIHIL, piscina, &series_temperley)
        || !_seriem_computare((s64)0xFFFFFFFFUL, limes, necessaria,
           notus ? &maximus : NIHIL, piscina, &series_vertex))
    {
        imprimere("   %u x %u: series primorum defecit\n", fila,
            longitudo);
        piscina_destruere(piscina);
        redde;
    }
    si (!laqueus_via_eligere(verbum, longitudo, fila,
        CONTRACTIO_SATURATUM, piscina, &via, &sv, &st))
    {
        imprimere("   %u x %u: via non eligibilis\n", fila, longitudo);
        piscina_destruere(piscina);
        redde;
    }
    electa = via
        == LAQUEUS_VIA_TEMPERLEY ? &series_temperley : &series_vertex;
    m = _probationes_necessariae(coefficientes, limes,
        electa->k_maximus,
        piscina, &e, &periculum);
    /* VERIFICATUM: k_verus primi si k_verus < k_maximus (probationes
     * sui in gradibus 1 .. k_verus, ultima transit, deinde m
     * certificantes), aliter ad limitem: k_maximus + ADDITUS,
     * probationes sui in gradibus 1 .. k_maximus - 1 (gradus k_maximus
     * probatus est) */
    k_finis = notus && electa->k_verus > ZEPHYRUM
        && electa->k_verus < electa->k_maximus ? electa->k_verus
        : electa->k_maximus + I;
    probatum     = (i64)(electa->k_maximus + I) * (i64)per_primum;
    verificatum  = (i64)k_finis * (i64)per_primum;
    principales  = verificatum + (i64)(k_finis <= electa->k_maximus
        ? k_finis : electa->k_maximus - I);
    imprimere("FUNDAMENTUM n=%u c=%u via=%s imus=%d summus=%d K=%u "
        "limes=%u scriptura=%d", fila, longitudo,
        via == LAQUEUS_VIA_TEMPERLEY ? "temperley" : "vertex", imus,
        summus, coefficientes, limes, scriptura);
    si (notus)
    {
        imprimere(" bita=%u imus_verus=%d summus_verus=%d scriptura_nota=%d"
            "%s%s", magnus_bitorum(maximus), imus_verus, imus_verus
            + (s32)numerus_notus - (s32)I, scriptura_nota,
            scriptura_nota != scriptura ? " SCRIPTURA_DIVERSA" : "",
            imus_verus < imus
                || imus_verus + (s32)numerus_notus - (s32)I
            > summus ? " EXTRA_FENESTRAM" : "");
    }
    alioquin
    {
        imprimere(" uncinus=ignotus");
    }
    imprimere(" kmax_tl=%u kmax_vx=%u kvera_tl=%u kvera_vx=%u kmagnus_tl=%u"
        " kmagnus_vx=%u%s e=%u m=%u eps=%.3e aest_probatum=%llu"
        " aest_verificatum=%llu aest_principales=%llu ratio=%.3f"
        " ratio_principales=%.3f vx_octeti=%llu vx_mult=%llu vx_lat=%u"
        " tl_octeti=%llu tl_mult=%llu\n",
        series_temperley.k_maximus, series_vertex.k_maximus,
        series_temperley.k_verus, series_vertex.k_verus,
        series_temperley.k_magnus,
        series_vertex.k_magnus, series_temperley.violatus
            || series_vertex.violatus ? " LIMES_VIOLATUS" : "",
        e, m, periculum,
        (unsigned long long)probatum, (unsigned long long)verificatum,
        (unsigned long long)principales,
        (f64)probatum / (f64)verificatum,
        (f64)probatum / (f64)principales,
        (unsigned long long)sv.octeti_officinae,
        (unsigned long long)sv.multiplicationes, sv.latitudo,
        (unsigned long long)st.octeti,
        (unsigned long long)st.multiplicationes);
    fflush(stdout);
    piscina_destruere(piscina);
}

/* XIII: omnes XLV tricae D136 et 14 x 801, aut una (-fila, -longitudo);
 * uncini noti ex viis (forma brackets.txt) */
interior vacuum
_sectio_fundamenti (
             i32                  fila_data,
             i32                  longitudo_data,
    constans character* constans* viae,
             i32                  numerus_viarum)
{
    Piscina* piscina = piscina_generare_dynamicum("mensura_textus",
        1048576);
      chorda textus[TEXTUS_UNCINORUM_MAXIMI];
         i32 k;

    per (k = ZEPHYRUM; k < numerus_viarum; k++)
    {
        textus[k] = filum_legere_totum(viae[k], piscina);
        imprimere("uncini noti: %s (%u octeti)\n", viae[k],
            textus[k].mensura);
    }
    imprimere("\nXIII. fundamentum verificationis (sine aestimatione)\n");
    si (fila_data != ZEPHYRUM)
    {
        _fundamentum_tricae(fila_data, longitudo_data, textus,
            numerus_viarum);
    }
    alioquin
    {
        per (k = ZEPHYRUM; k < XL + V; k++)
        {
            i32 fila;
            i32 longitudo;

            _trica_probationis(k, &fila, &longitudo);
            _fundamentum_tricae(fila, longitudo, textus,
                numerus_viarum);
        }
        _fundamentum_tricae(XIV, DCCCI, textus, numerus_viarum);
    }
    piscina_destruere(piscina);
}

/* XIV: cribrum segmentatum - primi in [2^30, 2^31) per residuum mod 4.
 * Probatio: summa = pi(2^31) - pi(2^30) = 105097565 - 54400028 =
 * 50697537 (OEIS A007053). */
interior vacuum
_sectio_cribri (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("mensura_cribrum",
        1048576);
    constans i64  imum       = (i64)0x40000000UL;
    constans i64  summum     = (i64)0x80000000UL;
    constans i64  segmentum  = (i64)0x400000UL;   /* 2^22 numeri */
    constans i32  radix      = 46341;             /* > sqrt(2^31) */
              i8* parvi;
              i8* compositi;
             i32* primi_parvi;
             i32  numerus_parvorum  = ZEPHYRUM;
             i64  unum              = ZEPHYRUM;
             i64  tria              = ZEPHYRUM;
             i64  initium;
             i32  j;
             i32  k;
         clock_t  t0 = clock();

    parvi = (i8*)piscina_allocare(piscina, (memoriae_index)(radix + I));
    primi_parvi = (i32*)piscina_allocare(piscina, (memoriae_index)radix
        * magnitudo(i32));
    compositi = (i8*)piscina_allocare(piscina,
        (memoriae_index)(segmentum
        / (i64)II));
    memset(parvi, ZEPHYRUM, (size_t)(radix + I));
    per (j = II; j <= radix; j++)
    {
        si (parvi[j] == ZEPHYRUM)
        {
            si (j > II)
            {
                primi_parvi[numerus_parvorum++] = j;
            }
            per (k = j * j; k <= radix; k += j)
            {
                parvi[k] = I;
            }
        }
    }
    per (initium = imum; initium < summum; initium += segmentum)
    {
        /* compositi[t] pro numero impari initium + 2t + 1 */
        memset(compositi, ZEPHYRUM, (size_t)(segmentum / (i64)II));
        per (j = ZEPHYRUM; j < numerus_parvorum; j++)
        {
            i64 p = (i64)primi_parvi[j];
            i64 q = ((initium + p - (i64)I) / p) * p;

            si (q % (i64)II == ZEPHYRUM)
            {
                q += p;
            }
            per (; q < initium + segmentum; q += (i64)II * p)
            {
                compositi[(q - initium - (i64)I) / (i64)II] = I;
            }
        }
        per (k = ZEPHYRUM; (i64)k < segmentum / (i64)II; k++)
        {
            si (compositi[k] == ZEPHYRUM)
            {
                si (((initium + (i64)II * (i64)k + (i64)I) % (i64)IV)
                    == (i64)I)
                {
                    unum++;
                }
                alioquin
                {
                    tria++;
                }
            }
        }
    }
    imprimere("\nXIV. cribrum [2^30, 2^31)\n");
    imprimere("CRIBRUM unum_mod_quattuor=%llu tria_mod_quattuor=%llu "
        "summa=%llu expectata=50697537 %s tempus_s=%.1f\n",
        (unsigned long long)unum, (unsigned long long)tria,
        (unsigned long long)(unum + tria),
        unum + tria == (i64)50697537UL ? "CONGRUIT" : "DISCORDAT",
        _secundae(t0));
    piscina_destruere(piscina);
}

s32
principale (
               s32  argc,
    constans character* constans* argv)
{
    constans character* sectio    = NIHIL;
                   s32  i;
                   i32  fila       = XII;
                   i32  longitudo  = DCCC;
                   f64  limes_s    = 60.0;
                   i64  semen      = (i64)MMXXVIII;
                   i32  paria      = IX;
                   b32  fila_data  = FALSUM;
    constans character* viae_uncinorum[TEXTUS_UNCINORUM_MAXIMI];
                   i32  numerus_viarum = ZEPHYRUM;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (   strcmp(argv[i], "-sectio") == ZEPHYRUM
                     && i + I < argc)
        {
            sectio = argv[++i];
        }
        alioquin si (   strcmp(argv[i], "-fila") == ZEPHYRUM
                     && i + I < argc)
        {
            fila       = (i32)strtoul(argv[++i], NIHIL, X);
            fila_data  = VERUM;
        }
        alioquin si (   strcmp(argv[i], "-longitudo") == ZEPHYRUM
                     && i + I < argc)
        {
            longitudo = (i32)strtoul(argv[++i], NIHIL, X);
        }
        alioquin si (   strcmp(argv[i], "-limes") == ZEPHYRUM
                     && i + I < argc)
        {
            limes_s = strtod(argv[++i], NIHIL);
        }
        alioquin si (   strcmp(argv[i], "-paria") == ZEPHYRUM
                     && i + I < argc)
        {
            paria = (i32)strtoul(argv[++i], NIHIL, X);
        }
        alioquin si (   strcmp(argv[i], "-semen") == ZEPHYRUM
                     && i + I < argc)
        {
            semen = (i64)strtoul(argv[++i], NIHIL, X);
        }
        alioquin si (   strcmp(argv[i], "-uncini") == ZEPHYRUM
                     && i + I < argc
                     && numerus_viarum < TEXTUS_UNCINORUM_MAXIMI)
        {
            viae_uncinorum[numerus_viarum++] = argv[++i];
        }
    }
    si (sectio != NIHIL && strcmp(sectio, "V") == ZEPHYRUM)
    {
        si (fila < II || longitudo < I)
        {
            imprimere("-fila >= 2 et -longitudo >= 1 requiruntur\n");
            redde I;
        }
        _sectio_singuli(fila, longitudo, limes_s, semen);
        redde ZEPHYRUM;
    }
    si (sectio != NIHIL && strcmp(sectio, "VI") == ZEPHYRUM)
    {
        _sectio_ab(fila_data ? fila : ZEPHYRUM, fila_data || longitudo
            != DCCC ? longitudo : DCCCI, paria < I ? I : paria);
        redde ZEPHYRUM;
    }
    si (sectio != NIHIL && strcmp(sectio, "VII") == ZEPHYRUM)
    {
        _sectio_k();
        redde ZEPHYRUM;
    }
    si (sectio != NIHIL && strcmp(sectio, "XI") == ZEPHYRUM)
    {
        _sectio_probationis_finalis();
        redde ZEPHYRUM;
    }
    si (sectio != NIHIL && strcmp(sectio, "XIII") == ZEPHYRUM)
    {
        _sectio_fundamenti(fila_data ? fila : ZEPHYRUM, longitudo
            == DCCC ? DCCCI : longitudo, viae_uncinorum,
            numerus_viarum);
        redde ZEPHYRUM;
    }
    si (sectio != NIHIL && strcmp(sectio, "XIV") == ZEPHYRUM)
    {
        _sectio_cribri();
        redde ZEPHYRUM;
    }
    si (sectio != NIHIL && strcmp(sectio, "XII") == ZEPHYRUM)
    {
        _sectio_verborum(fila_data ? fila : ZEPHYRUM, longitudo
            == DCCC ? DCCCI : longitudo);
        redde ZEPHYRUM;
    }
    si (sectio != NIHIL && strcmp(sectio, "X") == ZEPHYRUM)
    {
        _sectio_g2(fila_data ? fila : ZEPHYRUM, longitudo
            == DCCC ? DCCCI
            : longitudo, paria < I ? I : paria);
        redde ZEPHYRUM;
    }
    si (sectio != NIHIL && strcmp(sectio, "IX") == ZEPHYRUM)
    {
        _sectio_g1(fila_data ? fila : ZEPHYRUM, longitudo
            == DCCC ? DCCCI
            : longitudo, paria < I ? I : paria);
        redde ZEPHYRUM;
    }
    si (sectio != NIHIL && strcmp(sectio, "VIII") == ZEPHYRUM)
    {
        _sectio_uncini_tl(fila, longitudo == DCCC ? DCCCI : longitudo);
        redde ZEPHYRUM;
    }
    si (sectio == NIHIL || strcmp(sectio, "I") == ZEPHYRUM)
    {
        _sectio_comparationis();
    }
    si (sectio == NIHIL || strcmp(sectio, "II") == ZEPHYRUM)
    {
        _sectio_latitudinis();
    }
    si (sectio == NIHIL || strcmp(sectio, "III") == ZEPHYRUM)
    {
        _sectio_scalae();
    }
    si (sectio == NIHIL || strcmp(sectio, "IV") == ZEPHYRUM)
    {
        _sectio_tricarum();
    }
    redde ZEPHYRUM;
}
