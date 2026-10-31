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
 *   [-sectio I|II|III|IV|V|VI|VII|VIII]
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
        i32 bita = ZEPHYRUM;

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
            _secundae(t0), polynomium_gradus_imus(f),
            polynomium_gradus_summus(f), bita,
            laqueus_modulus_temperley_limes(mt));
    }
    fflush(stdout);
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
