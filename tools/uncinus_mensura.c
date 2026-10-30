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
 *   [-sectio I|II|III|IV]
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
#include "tabula_nodorum.h"
#include <stdio.h>
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
       Sors* s,
        i32  n,
        i32* longitudo,
    Piscina* piscina)
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
                : _plexus_fortuitus(&s, V, &c, piscina);
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
            pd = _plexus_fortuitus(&s, fila[f], &longitudo, piscina);
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

s32
principale (
               s32  argc,
    constans character* constans* argv)
{
    constans character* sectio = NIHIL;
                   s32  i;

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
