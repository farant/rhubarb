/* cellae_d119.h - auxilia communia probationum cellarum contra
 * tabulas exactas D119 (probatio_cellae.c, probatio_cellae_lenta.c)
 *
 * Tabulae D119 congelatae (knotapel/demo_120_surd_capacity/main.c:
 * D119_B zeta_8, D119_C 2I primi XXIV), catalogi zeta_8 et 2I,
 * oraculum concursus et _contra_d119. Sectio 'lentus' (n = V, VI) in
 * suite sua currit ut duae suitae parallele currant (budget B1 D8);
 * omnes probationes manent, sub veneno quoque.
 */
#ifndef CELLAE_D119_H
#define CELLAE_D119_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "extensio.h"
#include "quaternio.h"
#include "cellae.h"
#include <stdio.h>
#include <string.h>

interior Quaternio
_q (
       constans Anulus* anulus,
    constans character* textus,
               Piscina* piscina)
{
    Quaternio q;

    si (!quaternio_ex_chorda(anulus, chorda_ex_literis(textus, piscina),
        piscina, &q))
    {
        imprimere("FRACTA: quaternio malus: %s\n", textus);
        q.anulus = NIHIL;
        q.partes = NIHIL;
    }
    redde q;
}

/* generatores D82/D84: (g1, g1*, g2, g2*) */
interior vacuum
_generatores (
       constans Anulus* anulus,
    constans character* co,
    constans character* si_textus,
    constans character* si_negatum,
               Piscina* piscina,
             Quaternio* g)
{
    character textus[128];

    sprintf(textus, "[%s, %s, 0, 0]", co, si_textus);
    g[ZEPHYRUM] = _q(anulus, textus, piscina);
    sprintf(textus, "[%s, %s, 0, 0]", co, si_negatum);
    g[I] = _q(anulus, textus, piscina);
    sprintf(textus, "[%s, 0, 0, %s]", co, si_negatum);
    g[II] = _q(anulus, textus, piscina);
    sprintf(textus, "[%s, 0, 0, %s]", co, si_textus);
    g[III] = _q(anulus, textus, piscina);
}

hic_manens constans s32 TORSIONES[IV] = { I, -(s32)I, I, -(s32)I };

hic_manens constans i64 D119_ZETA[XII][VI] = {
    { 2024, 1480, 1456, 1456, 1938, 1217 },
    { 2024, 1907, 1799, 1799, 1970, 1217 },
    { 2024, 1494, 1462, 1462, 1968, 1217 },
    { 10626, 8010, 7908, 7696, 8678, 6770 },
    { 10626, 9723, 9486, 9402, 10046, 6770 },
    { 10626, 7156, 6797, 6611, 8666, 6770 },
    { 42504, 17201, 16479, 15497, 24714, 30822 },
    { 42504, 37835, 37309, 36707, 39072, 30822 },
    { 42504, 18368, 17282, 16514, 25625, 30822 },
    { 134596, 12983, 12268, 10070, 22979, 107035 },
    { 134596, 111290, 110495, 107663, 116323, 107035 },
    { 134596, 10031, 9635, 7911, 18245, 107035 } };
hic_manens constans i64 D119_AUREA[XII][VI] = {
    { 2024, 1580, 1580, 1580, 1960, 1181 },
    { 2024, 1930, 1930, 1930, 1976, 1181 },
    { 2024, 1596, 1596, 1596, 1994, 1181 },
    { 10626, 9114, 9030, 8958, 9399, 7225 },
    { 10626, 10055, 10050, 9983, 10252, 7225 },
    { 10626, 8089, 8031, 7953, 9408, 7225 },
    { 42504, 20805, 20453, 19693, 27983, 33885 },
    { 42504, 38688, 38702, 38044, 39557, 33885 },
    { 42504, 24333, 24022, 23499, 31738, 33885 },
    { 134596, 21679, 20936, 17034, 28850, 117888 },
    { 134596, 112556, 112469, 109058, 116439, 117888 },
    { 134596, 21237, 20998, 17547, 28484, 117888 } };
hic_manens constans i32 SCALA_D119[III] = { VI, XII, XXIV };

/* zeta_8: XXIV elementa (ut sectio zeta_8) */
interior Catalogus*
_zeta_octo_catalogus (
    Piscina* piscina)
{
     Quaternio  g[IV];
     Catalogus* cat = NIHIL;

    _generatores(extensio_anulus(extensio_quadratica(II, piscina)),
        "a/2", "a/2", "-a/2", piscina, g);
    si (!catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI, piscina,
        &cat))
    {
        redde NIHIL;
    }
    redde cat;
}

/* 2I: generatores (s, s*, t, t*) ordine D94/D120 build_2i, clausura
 * eadem BFS (1, generatores, deinde producta dextra; +-q idem, ut
 * find_2i: LX puncta); primi XXIV */
interior Catalogus*
_aurea_prima (
    Piscina* piscina)
{
         constans Anulus* q5 = extensio_anulus(extensio_quadratica(V,
             piscina));
                  Quaternio  g[IV];
                  Catalogus* plenus  = NIHIL;
                  Catalogus* pars    = NIHIL;
                        i32  indices[XXIV];
                        i32  i;
    hic_manens constans s32  NULLAE[IV] = { 0, 0, 0, 0 };

    g[ZEPHYRUM]  = _q(q5, "[1/2, 1/2, 1/2, 1/2]", piscina);
    g[I]         = _q(q5, "[1/2, -1/2, -1/2, -1/2]", piscina);
    g[II]        = _q(q5, "[(a + 1)/4, (a - 1)/4, 1/2, 0]", piscina);
    g[III]       = _q(q5, "[(a + 1)/4, (1 - a)/4, -1/2, 0]", piscina);
    si (   !catalogus_clausura(g, NULLAE, IV, ZEPHYRUM, ZEPHYRUM,
            piscina, &plenus)
        || catalogus_numerus(plenus) != LX)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < XXIV; i++)
    {
        indices[i] = i;
    }
    si (!catalogus_pars(plenus, indices, XXIV, piscina, &pars))
    {
        redde NIHIL;
    }
    redde pars;
}

/* oraculum concursus (via altera): 2^n summae per quaternio_adde/
 * _subtrahe, paria valoris diversi exacte aequalia (D94: summa eadem;
 * D82 etiam opposita) */
interior b32
_o_concursus (
    constans Catalogus* cat,
          constans i32* pondera,
                   i32  n,
           constans i8* tabula,
                   b32  plicata,
               Piscina* piscina,
                   b32* exitus)
{
    Quaternio summae[CCLVI];
          i32 numerus = (i32)(I << n);
          i32 v;
          i32 w;
          i32 j;

    *exitus = FALSUM;
    per (v = ZEPHYRUM; v < numerus; v++)
    {
        si (!quaternio_nullum(quaternio_anulus(catalogus_elementum(cat,
            ZEPHYRUM)), piscina, &summae[v]))
        {
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < n; j++)
        {
            Quaternio q = catalogus_elementum(cat, pondera[j]);

            si (!(((v >> j) & I)
                ? quaternio_adde(summae[v], q, piscina, &summae[v])
                : quaternio_subtrahe(summae[v], q, piscina,
                &summae[v])))
            {
                redde FALSUM;
            }
        }
    }
    per (v = ZEPHYRUM; v < numerus; v++)
    {
        per (w = v + I; w < numerus; w++)
        {
            Quaternio summa;

            si (tabula[v] == tabula[w])
            {
                perge;
            }
            si (quaternio_aequalis(summae[v], summae[w]))
            {
                *exitus = VERUM;
                redde VERUM;
            }
            si (!plicata)
            {
                perge;
            }
            si (!quaternio_adde(summae[v], summae[w], piscina, &summa))
            {
                redde FALSUM;
            }
            si (quaternio_est_nullum(summa))
            {
                *exitus = VERUM;
                redde VERUM;
            }
        }
    }
    redde VERUM;
}

nomen structura {
    i64 copiae;
    i64 regula;
    i64 robusta;
    i64 possibilis;
    i64 ligata;
    i64 indecisa;
    i64 concursus;
    i64 concursus_oraculi;    /* oraculo (si adhibitum) */
    i64 discrepantiae;        /* concursus != oraculum */
    i64 contradictiones;      /* concursus cum verdicto vero */
} NumeriIudicii;

/* copia proxima n ponderum ex [0, numerus) ordine lexicographico
 * (D119); FALSUM post ultimam */
interior b32
_copia_proxima (
     i32* pondera,
     i32  n,
     i32  numerus)
{
    i32 j = n;
    i32 l;

    dum (j > ZEPHYRUM && pondera[j - I] == numerus - n + j - I)
    {
        j--;
    }
    si (j == ZEPHYRUM)
    {
        redde FALSUM;
    }
    pondera[j - I]++;
    per (l = j; l < n; l++)
    {
        pondera[l] = pondera[l - I] + I;
    }
    redde VERUM;
}

/* omnes copiae n ponderum ex [0, numerus) ordine lexicographico (D119);
 * 'oraculum' (non NIHIL) = catalogus pro _o_concursus */
interior b32
_iudicare_omnes (
             Activatio* act,
                   i32  numerus,
                   i32  n,
           constans i8* tabula,
          constans i32* scala,
    constans Catalogus* oraculum,
               Piscina* piscina,
         NumeriIudicii* x)
{
    i32 pondera[VII];
    i32 j;

    memset(x, ZEPHYRUM, magnitudo(*x));
    per (j = ZEPHYRUM; j < n; j++)
    {
        pondera[j] = j;
    }
    fac
    {
        Iudicium iu;

        si (!activatio_iudica(act, pondera, n, scala, III, tabula, &iu))
        {
            redde FALSUM;
        }
        x->copiae++;
        x->regula      += iu.regula ? I : ZEPHYRUM;
        x->robusta     += iu.robusta ? I : ZEPHYRUM;
        x->possibilis  += iu.possibilis ? I : ZEPHYRUM;
        x->ligata      += iu.ligata ? I : ZEPHYRUM;
        x->indecisa    += iu.indecisa ? I : ZEPHYRUM;
        x->concursus   += iu.concursus ? I : ZEPHYRUM;
        si (iu.concursus && (iu.regula || iu.robusta || iu.possibilis))
        {
            x->contradictiones++;
        }
        si (oraculum != NIHIL)
        {
            PiscinaNotatio nota   = piscina_notare(piscina);
                       b32 verum  = FALSUM;

            si (!_o_concursus(oraculum, pondera, n, tabula, FALSUM,
                piscina, &verum))
            {
                redde FALSUM;
            }
            piscina_reficere(piscina, nota);
            x->concursus_oraculi  += verum ? I : ZEPHYRUM;
            x->discrepantiae      += verum != iu.concursus ? I
                : ZEPHYRUM;
        }
    } dum (_copia_proxima(pondera, n, numerus));
    redde VERUM;
}

interior constans i8*
_tabula_functionis (
         i32  n,
         i32  functio,
     Piscina* piscina)
{
    redde functio == ZEPHYRUM ? cellae_paritas(n, piscina)
        : functio == I ? cellae_et(n, piscina) : cellae_maior(n,
        piscina);
}

/* ordines tabulae D119 n_infra..n_supra contra activatio_iudica */
interior vacuum
_contra_d119 (
    CredoContextus* c,
         Activatio* act,
    constans i64 (*tabula)[VI],
       constans i32* scala,
         Catalogus* oraculum,
                i32 n_infra,
                i32 n_supra)
{
    i32 n;
    i32 functio;

    per (n = n_infra; n <= n_supra; n++)
    {
        per (functio = ZEPHYRUM; functio < III; functio++)
        {
             constans i64* ordo = tabula[(n - III) * III + functio];
              constans i8* tv = _tabula_functionis(n, functio,
                  c->piscina);
            NumeriIudicii x;

            CREDO_NECESSE (tv != NIHIL);
            CREDO_NECESSE (_iudicare_omnes(act, XXIV, n, tv, scala,
                oraculum, c->piscina, &x));
            CREDO_AEQUALIS_S64 ((s64)x.copiae, (s64)ordo[ZEPHYRUM]);
            CREDO_AEQUALIS_S64 ((s64)x.regula, (s64)ordo[II]);
            CREDO_AEQUALIS_S64 ((s64)x.robusta, (s64)ordo[III]);
            CREDO_AEQUALIS_S64 ((s64)x.possibilis, (s64)ordo[IV]);
            CREDO_AEQUALIS_S64 ((s64)x.ligata, (s64)ordo[V]);
            CREDO_AEQUALIS_S64 ((s64)x.indecisa, ZEPHYRUM);
            CREDO_AEQUALIS_S64 ((s64)x.contradictiones, ZEPHYRUM);
            /* concursus <= copiae sine possibili */
            CREDO_VERUM (x.concursus <= x.copiae - x.possibilis);
            si (oraculum != NIHIL)
            {
                CREDO_AEQUALIS_S64 ((s64)x.discrepantiae, ZEPHYRUM);
                CREDO_AEQUALIS_S64 ((s64)x.concursus,
                    (s64)x.concursus_oraculi);
            }
        }
    }
}

#endif /* CELLAE_D119_H */
