/* probatio_dependentia.c - probationes dependentiae (knotapel D130,
 * plan.md "API sketch")
 *
 * Oracula a bibliotheca independentia, in hac probatione scripta:
 * (1) dominatio per viam capitum (non per tempora percursus);
 * (2) cruces per paria omnia arcuum, arcu radicis ut (0, r) incluso;
 * (3) lacunae per proventum ordinatum; nidificatio per quaternas
 *     omnes l1 < l2 < r1 < r2 et paria omnia subarborum disiunctarum
 *     (non per fratres); gradus arcus per componentes subgraphi inducti
 *     collectionibus disiunctis (non per capita extra intervallum);
 * (4) nucleus planaris per subcollectiones omnes arcuum;
 * (5) componentes per relaxationem iteratam.
 * Characterizationes: arbores sanae n verborum = n^(n-1) (radicatae
 * signatae); exspectatae 1, 2, 7, 30, 143, 728 (n = 1..6); duae viae
 * proiectivitatis congruunt; proiectiva => planaris => bene nidificata;
 * figurae Kuhlmann et Nivre 2006 (2a, 3a-c); lex 1/3 per ordines
 * omnes; inversio ordinis mensuras servat.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "sors.h"
#include "dependentia.h"
#include <stdio.h>
#include <string.h>

#define MAXIMUS XX

/* ---------- oracula ---------- */

interior b32
_o_dominat (
    constans i32* capita,
             i32  u,
             i32  v)
{
    dum (v != ZEPHYRUM)
    {
        si (v == u)
        {
            redde VERUM;
        }
        v = capita[v - I];
    }
    redde FALSUM;
}

interior b32
_o_secant (
    i32 a,
    i32 b,
    i32 c,
    i32 e)
{
    i32 t;

    si (a > b)
    {
        t = a;
        a = b;
        b = t;
    }
    si (c > e)
    {
        t = c;
        c = e;
        e = t;
    }
    redde (b32)((a < c && c < b && b < e) || (c < a && a < e && e < b));
}

/* cruces oraculi: arcus k = (capita[k - 1], k); arcus radicis (0, r) */
interior vacuum
_o_cruces (
         constans i32* capita,
                  i32  n,
    DependentiaCruces* o)
{
    i32 k;
    i32 q;
    b32 sectus[MAXIMUS + I];

    memset(o, ZEPHYRUM, magnitudo(*o));
    memset(sectus, ZEPHYRUM, magnitudo(sectus));
    per (k = I; k <= n; k++)
    {
        si (capita[k - I] == ZEPHYRUM)
        {
            perge;
        }
        per (q = I; q <= n; q++)
        {
            i32 h = capita[k - I];
            i32 g = capita[q - I];

            si (g == ZEPHYRUM)
            {
                /* q radix: arcus (0, q) */
                si (_o_secant(h, k, ZEPHYRUM, q))
                {
                    o->cruces_radicis = o->cruces_radicis + I;
                }
                perge;
            }
            si (q <= k)
            {
                perge;
            }
            si (h != g && h != q && k != g && k != q)
            {
                o->paria_disiuncta = o->paria_disiuncta + I;
            }
            si (_o_secant(h, k, g, q))
            {
                o->cruces = o->cruces + I;
                sectus[k] = VERUM;
                sectus[q] = VERUM;
            }
        }
    }
    per (k = I; k <= n; k++)
    {
        si (sectus[k])
        {
            o->arcus_secti = o->arcus_secti + I;
        }
    }
    o->planaris = (b32)(o->cruces == ZEPHYRUM);
    o->proiectiva = (b32)(o->planaris
        && o->cruces_radicis == ZEPHYRUM);
}

interior i32
_o_lacunae (
    constans i32* capita,
             i32  n,
             i32  u)
{
    i32 p;
    i32 prior    = ZEPHYRUM;
    i32 lacunae  = ZEPHYRUM;

    per (p = I; p <= n; p++)
    {
        si (_o_dominat(capita, u, p))
        {
            si (prior != ZEPHYRUM && p - prior > I)
            {
                lacunae = lacunae + I;
            }
            prior = p;
        }
    }
    redde lacunae;
}

interior b32
_o_bene_nidificata (
    constans i32* capita,
             i32  n)
{
    i32 u;
    i32 v;
    i32 l1;
    i32 l2;
    i32 r1;
    i32 r2;

    per (u = I; u <= n; u++)
    {
        per (v = I; v <= n; v++)
        {
            si (   u == v || _o_dominat(capita, u, v)
                || _o_dominat(capita, v, u))
            {
                perge;
            }
            per (l1 = I; l1 <= n; l1++)
            {
                si (!_o_dominat(capita, u, l1))
                {
                    perge;
                }
                per (l2 = l1 + I; l2 <= n; l2++)
                {
                    si (!_o_dominat(capita, v, l2))
                    {
                        perge;
                    }
                    per (r1 = l2 + I; r1 <= n; r1++)
                    {
                        si (!_o_dominat(capita, u, r1))
                        {
                            perge;
                        }
                        per (r2 = r1 + I; r2 <= n; r2++)
                        {
                            si (_o_dominat(capita, v, r2))
                            {
                                redde FALSUM;
                            }
                        }
                    }
                }
            }
        }
    }
    redde VERUM;
}

interior i32
_o_radix_collectionis (
    i32* parens,
    i32  x)
{
    dum (parens[x] != x)
    {
        x = parens[x];
    }
    redde x;
}

/* gradus arcus in verbum d: componentes subgraphi inducti per
 * collectiones disiunctas; radix componentis = verbum cuius caput non
 * in eadem componente */
interior i32
_o_gradus_arcus (
    constans i32* capita,
             i32  n,
             i32  d)
{
    i32 h        = capita[d - I];
    i32 initium  = h < d ? h : d;
    i32 finis    = h < d ? d : h;
    i32 parens[MAXIMUS + I];
    i32 p;
    i32 gradus   = ZEPHYRUM;

    (vacuum)n;
    per (p = initium; p <= finis; p++)
    {
        parens[p] = p;
    }
    per (p = initium; p <= finis; p++)
    {
        i32 c = capita[p - I];

        si (c >= initium && c <= finis && c != ZEPHYRUM)
        {
            parens[_o_radix_collectionis(parens, p)] =
                _o_radix_collectionis(parens, c);
        }
    }
    per (p = initium; p <= finis; p++)
    {
        i32 c        = capita[p - I];
        b32 radix_c  = (b32)(c == ZEPHYRUM || c < initium || c > finis
            || _o_radix_collectionis(parens, c)
            != _o_radix_collectionis(parens, p));

        si (radix_c && !_o_dominat(capita, h, p))
        {
            gradus = gradus + I;
        }
    }
    redde gradus;
}

interior vacuum
_o_mensura (
          constans i32* capita,
                   i32  n,
                   i32* lacunae,
    DependentiaMensura* o)
{
    i32 k;

    memset(o, ZEPHYRUM, magnitudo(*o));
    per (k = I; k <= n; k++)
    {
        lacunae[k - I] = _o_lacunae(capita, n, k);
        si (lacunae[k - I] > o->gradus_lacunarum)
        {
            o->gradus_lacunarum = lacunae[k - I];
        }
        si (capita[k - I] != ZEPHYRUM)
        {
            i32 g = _o_gradus_arcus(capita, n, k);

            si (g > o->gradus_arcuum)
            {
                o->gradus_arcuum = g;
            }
        }
    }
    o->bene_nidificata  = _o_bene_nidificata(capita, n);
    o->proiectiva       = (b32)(o->gradus_lacunarum == ZEPHYRUM);
}

/* nucleus per subcollectiones omnes arcuum realium (m <= XVI) */
interior s32
_o_nucleus (
    constans i32* capita,
             i32  n)
{
    i32 arcus[MAXIMUS];
    i32 m = ZEPHYRUM;
    i32 k;
    i32 masca;
    s32 optimum = ZEPHYRUM;

    per (k = I; k <= n; k++)
    {
        si (capita[k - I] != ZEPHYRUM)
        {
            arcus[m]  = k;
            m         = m + I;
        }
    }
    per (masca = ZEPHYRUM; masca < ((i32)I << m); masca++)
    {
        i32 i;
        i32 j;
        s32 numerus  = ZEPHYRUM;
        b32 bona     = VERUM;

        per (i = ZEPHYRUM; i < m && bona; i++)
        {
            si (!(masca & ((i32)I << i)))
            {
                perge;
            }
            numerus = numerus + I;
            per (j = i + I; j < m; j++)
            {
                si (   (masca & ((i32)I << j))
                    && _o_secant(capita[arcus[i] - I], arcus[i],
                    capita[arcus[j] - I], arcus[j]))
                {
                    bona = FALSUM;
                    frange;
                }
            }
        }
        si (bona && numerus > optimum)
        {
            optimum = numerus;
        }
    }
    redde optimum;
}

/* componentes per relaxationem: nota = minimum verbi in componente,
 * deinde renumeratio ordine primi verbi */
interior s32
_o_componentes (
    constans i32* capita,
             i32  n,
             s32* componens)
{
    i32 nota[MAXIMUS + I];
    s32 nova[MAXIMUS + I];
    i32 k;
    i32 q;
    b32 mutatum = VERUM;
    s32 numerus = ZEPHYRUM;

    per (k = I; k <= n; k++)
    {
        nota[k] = k;
        nova[k] = (s32)-I;
    }
    dum (mutatum)
    {
        mutatum = FALSUM;
        per (k = I; k <= n; k++)
        {
            per (q = I; q <= n; q++)
            {
                si (   capita[k - I] == ZEPHYRUM
                    || capita[q - I] == ZEPHYRUM
                    || !_o_secant(capita[k - I], k, capita[q - I], q))
                {
                    perge;
                }
                si (nota[q] < nota[k])
                {
                    nota[k] = nota[q];
                    mutatum = VERUM;
                }
            }
        }
    }
    per (k = I; k <= n; k++)
    {
        si (capita[k - I] == ZEPHYRUM)
        {
            componens[k - I] = (s32)-I;
            perge;
        }
        si (nova[nota[k]] < ZEPHYRUM)
        {
            nova[nota[k]]  = numerus;
            numerus        = numerus + I;
        }
        componens[k - I] = nova[nota[k]];
    }
    redde numerus;
}

/* ---------- auxilia ---------- */

interior DependentiaArbor
_arbor (
    constans i32* capita,
             i32  n)
{
    DependentiaArbor a;

    a.numerus  = n;
    a.capita   = capita;
    redde a;
}

/* capita sequentia (odometer per 0..n); FALSUM post ultimum */
interior b32
_sequens (
    i32* capita,
    i32  n)
{
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        si (capita[k] < n)
        {
            capita[k] = capita[k] + I;
            redde VERUM;
        }
        capita[k] = ZEPHYRUM;
    }
    redde FALSUM;
}

/* arbor fortuita sana: ordo fortuitus, primus radix, ceteri capita ex
 * prioribus */
interior vacuum
_fortuita (
    Sors* sors,
     i32  n,
     i32* capita)
{
    i32 ordo[MAXIMUS];
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        ordo[k] = k + I;
    }
    per (k = n - I; k > ZEPHYRUM; k--)
    {
        i32 j = sors_intra(sors, k + I);
        i32 t = ordo[k];

        ordo[k] = ordo[j];
        ordo[j] = t;
    }
    capita[ordo[ZEPHYRUM] - I] = ZEPHYRUM;
    per (k = I; k < n; k++)
    {
        capita[ordo[k] - I] = ordo[sors_intra(sors, k)];
    }
}

/* comparatio plena cum oraculis; VERUM si omnia congruunt */
interior b32
_comparare (
         Piscina* piscina,
    constans i32* capita,
             i32  n,
             b32  nucleus)
{
      DependentiaArbor a = _arbor(capita, n);
     DependentiaCruces c;
     DependentiaCruces cruces_oraculi;
    DependentiaMensura m;
    DependentiaMensura mensura_oraculi;
                   i32 lacunae[MAXIMUS];
                   i32 lacunae_oraculi[MAXIMUS];
                   s32 componens[MAXIMUS];
                   s32 componens_oraculi[MAXIMUS];
                   i32 k;
                   b32 bona = VERUM;

    si (   !dependentia_cruces(piscina, &a, &c)
        || !dependentia_mensurare(piscina, &a, lacunae, &m))
    {
        redde FALSUM;
    }
    _o_cruces(capita, n, &cruces_oraculi);
    _o_mensura(capita, n, lacunae_oraculi, &mensura_oraculi);
    si (   c.cruces          != cruces_oraculi.cruces
        || c.cruces_radicis  != cruces_oraculi.cruces_radicis
        || c.arcus_secti     != cruces_oraculi.arcus_secti
        || c.paria_disiuncta != cruces_oraculi.paria_disiuncta
        || c.planaris        != cruces_oraculi.planaris
        || c.proiectiva      != cruces_oraculi.proiectiva)
    {
        bona = FALSUM;
    }
    si (   m.gradus_lacunarum != mensura_oraculi.gradus_lacunarum
        || m.gradus_arcuum    != mensura_oraculi.gradus_arcuum
        || m.bene_nidificata  != mensura_oraculi.bene_nidificata
        || m.proiectiva       != mensura_oraculi.proiectiva)
    {
        bona = FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (lacunae[k] != lacunae_oraculi[k])
        {
            bona = FALSUM;
        }
    }
    si (dependentia_componentes(piscina, &a, componens)
        != _o_componentes(capita, n, componens_oraculi))
    {
        bona = FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (componens[k] != componens_oraculi[k])
        {
            bona = FALSUM;
        }
    }
    si (   nucleus && dependentia_nucleus_planaris(piscina, &a)
        != _o_nucleus(capita, n))
    {
        bona = FALSUM;
    }
    si (!bona)
    {
        imprimere("  discrepantia, capita:");
        per (k = ZEPHYRUM; k < n; k++)
        {
            imprimere(" %u", (insignatus integer)capita[k]);
        }
        imprimere("\n");
    }
    redde bona;
}

/* ---------- sectiones ---------- */

interior vacuum
_probare_iudicium (
    CredoContextus* c)
{
                  i32 n;
                  i32 capita[MAXIMUS];
     DependentiaArbor a;
    constans      i32 radicatae[VII] = { ZEPHYRUM, I, II, IX, LXIV,
        DCXXV,
        (i32)(II * MMMDCCCLXXXVIII) };   /* 7776 = 6^5 */

    (vacuum)c;
    /* casus singuli */
    a = _arbor(capita, ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)dependentia_validare(&a),
        (s32)DEPENDENTIA_VACUA);
    CREDO_AEQUALIS_S32 ((s32)dependentia_validare(NIHIL),
        (s32)DEPENDENTIA_VACUA);
    capita[ZEPHYRUM]  = ZEPHYRUM;
    a                 = _arbor(capita, I);
    CREDO_AEQUALIS_S32 ((s32)dependentia_validare(&a),
        (s32)DEPENDENTIA_SANA);
    capita[ZEPHYRUM] = I;   /* sui caput */
    CREDO_AEQUALIS_S32 ((s32)dependentia_validare(&a),
        (s32)DEPENDENTIA_CAPUT_EXTRA);
    capita[ZEPHYRUM]  = II;
    capita[I]         = III;   /* extra */
    a                 = _arbor(capita, II);
    CREDO_AEQUALIS_S32 ((s32)dependentia_validare(&a),
        (s32)DEPENDENTIA_CAPUT_EXTRA);
    capita[ZEPHYRUM]  = II;
    capita[I]         = I;
    CREDO_AEQUALIS_S32 ((s32)dependentia_validare(&a),
        (s32)DEPENDENTIA_RADIX_NULLA);
    capita[ZEPHYRUM]  = ZEPHYRUM;
    capita[I]         = ZEPHYRUM;
    CREDO_AEQUALIS_S32 ((s32)dependentia_validare(&a),
        (s32)DEPENDENTIA_RADICES_PLURES);
    /* cyclus cum radice: 1 radix, 2 <-> 3 */
    capita[ZEPHYRUM]  = ZEPHYRUM;
    capita[I]         = III;
    capita[II]        = II;
    a                 = _arbor(capita, III);
    CREDO_AEQUALIS_S32 ((s32)dependentia_validare(&a),
        (s32)DEPENDENTIA_CYCLUS);

    /* numerus arborum sanarum = n^(n-1) per capita omnia 0..n */
    per (n = I; n <= VI; n++)
    {
        i32 sanae = ZEPHYRUM;

        memset(capita, ZEPHYRUM, magnitudo(capita));
        a = _arbor(capita, n);
        fac
        {
            si (dependentia_validare(&a) == DEPENDENTIA_SANA)
            {
                sanae = sanae + I;
            }
        }
        dum (_sequens(capita, n));
        CREDO_AEQUALIS_I32 (sanae, radicatae[n]);
    }
}

interior vacuum
_probare_figuras (
    CredoContextus* c)
{
              Piscina* piscina = c->piscina;
     DependentiaArbor  a;
    DependentiaCruces  x;
   DependentiaMensura  m;
                  i32  lacunae[VI];
                  s32  componens[VI];
    /* Kuhlmann et Nivre, figurae ex textu reconstructae (proprietates
     * omnes quas textus dicit tenent) */
    constans i32 duae_a[III]  = { II, ZEPHYRUM, I };
    constans i32 tres_a[VI]   = { ZEPHYRUM, I, II, III, I, V };
    constans i32 tres_b[VI]   = { ZEPHYRUM, I, II, V, I, III };
    constans i32 tres_c[VI]   = { ZEPHYRUM, I, V, II, I, IV };
    /* manu: (1,3) x (2,4); radix 3 */
    constans i32 h1[IV]   = { III, IV, ZEPHYRUM, III };
    /* manu: (1,4) (2,5) (3,6) inter se secantes, (4,5) (4,6); radix 4 */
    constans i32 h2[VI]   = { IV, V, VI, ZEPHYRUM, IV, IV };

    /* 2a: planaris, non proiectiva; proventus verbi 1 = {1, 3} */
    a = _arbor(duae_a, III);
    CREDO_VERUM (dependentia_cruces(piscina, &a, &x));
    CREDO_VERUM (x.planaris);
    CREDO_FALSUM (x.proiectiva);
    CREDO_VERUM (dependentia_mensurare(piscina, &a, lacunae, &m));
    CREDO_FALSUM (m.proiectiva);
    CREDO_AEQUALIS_I32 (lacunae[ZEPHYRUM], I);
    CREDO_AEQUALIS_I32 (m.gradus_lacunarum, I);

    /* 3a: gd 0, ed 0, bene nidificata */
    a = _arbor(tres_a, VI);
    CREDO_VERUM (dependentia_mensurare(piscina, &a, lacunae, &m));
    CREDO_AEQUALIS_I32 (m.gradus_lacunarum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (m.gradus_arcuum, ZEPHYRUM);
    CREDO_VERUM (m.bene_nidificata);
    /* 3b: gd 1 (proventus i = 2: {2, 3, 6}), ed 1, bene nidificata */
    a = _arbor(tres_b, VI);
    CREDO_VERUM (dependentia_mensurare(piscina, &a, lacunae, &m));
    CREDO_AEQUALIS_I32 (lacunae[I], I);
    CREDO_AEQUALIS_I32 (m.gradus_lacunarum, I);
    CREDO_AEQUALIS_I32 (m.gradus_arcuum, I);
    CREDO_VERUM (m.bene_nidificata);
    /* 3c: gd 2 (proventus i: {2, 4, 6}), ed 1, NON bene nidificata */
    a = _arbor(tres_c, VI);
    CREDO_VERUM (dependentia_mensurare(piscina, &a, lacunae, &m));
    CREDO_AEQUALIS_I32 (lacunae[I], II);
    CREDO_AEQUALIS_I32 (m.gradus_lacunarum, II);
    CREDO_AEQUALIS_I32 (m.gradus_arcuum, I);
    CREDO_FALSUM (m.bene_nidificata);

    /* h1 */
    a = _arbor(h1, IV);
    CREDO_VERUM (dependentia_cruces(piscina, &a, &x));
    CREDO_AEQUALIS_S64 (x.cruces, (s64)I);
    CREDO_AEQUALIS_S64 (x.cruces_radicis, (s64)I);
    CREDO_AEQUALIS_I32 (x.arcus_secti, II);
    CREDO_AEQUALIS_S64 (x.paria_disiuncta, (s64)I);
    CREDO_FALSUM (x.planaris);
    CREDO_AEQUALIS_S32 (dependentia_nucleus_planaris(piscina, &a), II);
    CREDO_AEQUALIS_S32 (dependentia_componentes(piscina, &a, componens),
        II);
    CREDO_AEQUALIS_S32 (componens[ZEPHYRUM], ZEPHYRUM);
    CREDO_AEQUALIS_S32 (componens[I], ZEPHYRUM);
    CREDO_AEQUALIS_S32 (componens[II], (s32)-I);
    CREDO_AEQUALIS_S32 (componens[III], I);
    CREDO_VERUM (dependentia_mensurare(piscina, &a, lacunae, &m));
    CREDO_AEQUALIS_I32 (lacunae[III], I);   /* proventus 4 = {2, 4} */

    /* h2 */
    a = _arbor(h2, VI);
    CREDO_VERUM (dependentia_cruces(piscina, &a, &x));
    CREDO_AEQUALIS_S64 (x.cruces, (s64)IV);
    CREDO_AEQUALIS_S64 (x.cruces_radicis, (s64)II);
    CREDO_AEQUALIS_I32 (x.arcus_secti, IV);
    CREDO_AEQUALIS_S32 (dependentia_nucleus_planaris(piscina, &a), III);
    CREDO_AEQUALIS_S32 (dependentia_componentes(piscina, &a, componens),
        II);
}

interior vacuum
_probare_oracula (
    CredoContextus* c)
{
    Piscina* p = piscina_generare_dynamicum("dependentia_oracula",
        65536);
             i32 n;
             i32 capita[MAXIMUS];
             i32 discrepantiae  = ZEPHYRUM;
             i32 hierarchia     = ZEPHYRUM;
             i32 viae           = ZEPHYRUM;
    constans i32 exspectatae[VII] = { ZEPHYRUM, I, II, VII, XXX, CXLIII,
        DCCXXVIII };

    (vacuum)c;
    per (n = I; n <= VI; n++)
    {
        i32 numerus = ZEPHYRUM;

        memset(capita, ZEPHYRUM, magnitudo(capita));
        fac
        {
              DependentiaArbor a = _arbor(capita, n);
             DependentiaCruces x;
            DependentiaMensura m;

            si (dependentia_validare(&a) != DEPENDENTIA_SANA)
            {
                perge;
            }
            piscina_vacare(p);
            si (!_comparare(p, capita, n, VERUM))
            {
                discrepantiae = discrepantiae + I;
            }
            dependentia_cruces(p, &a, &x);
            dependentia_mensurare(p, &a, NIHIL, &m);
            si (x.proiectiva != m.proiectiva)
            {
                viae = viae + I;
            }
            si (   (x.proiectiva && !x.planaris)
                || (x.planaris && !m.bene_nidificata))
            {
                hierarchia = hierarchia + I;
            }
            si (m.proiectiva)
            {
                numerus = numerus + I;
            }
        }
        dum (_sequens(capita, n));
        CREDO_AEQUALIS_I32 (numerus, exspectatae[n]);
    }
    CREDO_AEQUALIS_I32 (discrepantiae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (viae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (hierarchia, ZEPHYRUM);
    piscina_destruere(p);
}

interior vacuum
_probare_fortuitas (
    CredoContextus* c)
{
    Piscina* p = piscina_generare_dynamicum("dependentia_fortuitae",
        65536);
        Sors sors;
         i32 k;
         i32 capita[MAXIMUS];
         i32 discrepantiae  = ZEPHYRUM;
         i32 non_planares   = ZEPHYRUM;

    (vacuum)c;
    sors_seminare(&sors, (i64)CXXX, ZEPHYRUM);
    per (k = ZEPHYRUM; k < MM; k++)
    {
                      i32 n = VII + sors_intra(&sors, XI);   /* 7..17 */
         DependentiaArbor a;
        DependentiaCruces x;

        _fortuita(&sors, n, capita);
        a = _arbor(capita, n);
        piscina_vacare(p);
        si (   dependentia_validare(&a) != DEPENDENTIA_SANA
            || !_comparare(p, capita, n, VERUM))
        {
            discrepantiae = discrepantiae + I;
        }
        dependentia_cruces(p, &a, &x);
        si (!x.planaris)
        {
            non_planares = non_planares + I;
        }
    }
    CREDO_AEQUALIS_I32 (discrepantiae, ZEPHYRUM);
    /* probatio non vacua: ordines fortuiti saepe secant */
    CREDO_MAIOR_I32 (non_planares, (i32)M);
    piscina_destruere(p);
}

/* permutatio sequens (ordo lexicographicus); FALSUM post ultimam */
interior b32
_permutatio_sequens (
    i32* s,
    i32  n)
{
    s32 i = (s32)n - II;
    s32 j;

    dum (i >= ZEPHYRUM && s[i] >= s[i + I])
    {
        i = i - I;
    }
    si (i < ZEPHYRUM)
    {
        redde FALSUM;
    }
    j = (s32)n - I;
    dum (s[j] <= s[i])
    {
        j = j - I;
    }
    {
        i32 t = s[i];

        s[i] = s[j];
        s[j] = t;
    }
    per (i = i + I, j = (s32)n - I; i < j; i++, j--)
    {
        i32 t = s[i];

        s[i] = s[j];
        s[j] = t;
    }
    redde VERUM;
}

/* summa crucium per ordines omnes == n! paria / 3 (exacte, 3 summa ==
 * n! paria) */
interior b32
_lex_tertiae (
         Piscina* p,
    constans i32* capita,
             i32  n)
{
    i32 sigma[MAXIMUS + I];
    i32 nova[MAXIMUS];
    i32 k;
    s64 summa       = ZEPHYRUM;
    s64 ordines     = ZEPHYRUM;
    s64 paria       = (s64)-I;
    b32 constans_p  = VERUM;

    per (k = ZEPHYRUM; k < n; k++)
    {
        sigma[k] = k + I;
    }
    fac
    {
         DependentiaArbor a;
        DependentiaCruces x;

        /* verbum k ad positionem sigma[k - 1] */
        per (k = I; k <= n; k++)
        {
            i32 h = capita[k - I];

            nova[sigma[k - I] - I] = h == ZEPHYRUM ? ZEPHYRUM
                : sigma[h - I];
        }
        a = _arbor(nova, n);
        piscina_vacare(p);
        si (!dependentia_cruces(p, &a, &x))
        {
            redde FALSUM;
        }
        si (paria < ZEPHYRUM)
        {
            paria = x.paria_disiuncta;
        }
        alioquin si (paria != x.paria_disiuncta)
        {
            constans_p = FALSUM;   /* paria ordine non pendent */
        }
        summa    = summa + x.cruces;
        ordines  = ordines + I;
    }
    dum (_permutatio_sequens(sigma, n));
    redde (b32)(constans_p && (s64)III * summa == ordines * paria);
}

interior vacuum
_probare_legem_tertiae (
    CredoContextus* c)
{
    Piscina* p = piscina_generare_dynamicum("dependentia_tertia",
        65536);
         i32 n;
         i32 k;
         i32 capita[MAXIMUS];
         i32 fractae = ZEPHYRUM;
         i32 arbores = ZEPHYRUM;
        Sors sors;

    (vacuum)c;
    per (n = I; n <= V; n++)
    {
        memset(capita, ZEPHYRUM, magnitudo(capita));
        fac
        {
            DependentiaArbor a = _arbor(capita, n);

            si (dependentia_validare(&a) != DEPENDENTIA_SANA)
            {
                perge;
            }
            arbores = arbores + I;
            si (!_lex_tertiae(p, capita, n))
            {
                fractae = fractae + I;
            }
        }
        dum (_sequens(capita, n));
    }
    sors_seminare(&sors, (i64)CXXX, I);
    per (k = ZEPHYRUM; k < XL; k++)
    {
        n = VI + sors_intra(&sors, II);   /* 6, 7 */
        _fortuita(&sors, n, capita);
        arbores = arbores + I;
        si (!_lex_tertiae(p, capita, n))
        {
            fractae = fractae + I;
        }
    }
    CREDO_AEQUALIS_I32 (arbores, (i32)(I + II + IX + LXIV + DCXXV
        + XL));
    CREDO_AEQUALIS_I32 (fractae, ZEPHYRUM);
    piscina_destruere(p);
}

interior vacuum
_probare_ordinem_versum (
    CredoContextus* c)
{
    Piscina* p = piscina_generare_dynamicum("dependentia_inversio",
        65536);
         i32 n;
         i32 capita[MAXIMUS];
         i32 inversa[MAXIMUS];
         i32 fractae = ZEPHYRUM;

    (vacuum)c;
    per (n = I; n <= VI; n++)
    {
        memset(capita, ZEPHYRUM, magnitudo(capita));
        fac
        {
              DependentiaArbor a = _arbor(capita, n);
              DependentiaArbor b;
             DependentiaCruces x;
             DependentiaCruces y;
            DependentiaMensura m;
            DependentiaMensura w;
                           i32 k;

            si (dependentia_validare(&a) != DEPENDENTIA_SANA)
            {
                perge;
            }
            /* verbum k ad positionem n + 1 - k */
            per (k = I; k <= n; k++)
            {
                i32 h = capita[k - I];

                inversa[n - k] = h == ZEPHYRUM ? ZEPHYRUM : n + I - h;
            }
            b = _arbor(inversa, n);
            piscina_vacare(p);
            dependentia_cruces(p, &a, &x);
            dependentia_cruces(p, &b, &y);
            dependentia_mensurare(p, &a, NIHIL, &m);
            dependentia_mensurare(p, &b, NIHIL, &w);
            si (   x.cruces           != y.cruces
                || x.cruces_radicis   != y.cruces_radicis
                || m.gradus_lacunarum != w.gradus_lacunarum
                || m.gradus_arcuum    != w.gradus_arcuum
                || m.bene_nidificata  != w.bene_nidificata
                || dependentia_nucleus_planaris(p, &a)
                != dependentia_nucleus_planaris(p, &b))
            {
                fractae = fractae + I;
            }
        }
        dum (_sequens(capita, n));
    }
    CREDO_AEQUALIS_I32 (fractae, ZEPHYRUM);
    piscina_destruere(p);
}

interior vacuum
_probare_verba_omissa (
    CredoContextus* c)
{
            Piscina* piscina = c->piscina;
    /* 1 <- 2 radix, 3 <- 2, 4 <- 3, 5 <- 3; 3 omisso: 4, 5 ad 2 */
    constans     i32 capita[V] = { II, ZEPHYRUM, II, III, III };
    constans     b32 tertium[V] = { FALSUM, FALSUM, VERUM, FALSUM,
        FALSUM };
    constans     b32 radix[V] = { FALSUM, VERUM, FALSUM, FALSUM,
        FALSUM };
    constans     b32 folia[V] = { VERUM, FALSUM, FALSUM, VERUM,
        FALSUM };
    DependentiaArbor a = _arbor(capita, V);
    DependentiaArbor e;
                 i32 contractiones = (i32)XCIX;

    CREDO_VERUM (dependentia_omittere(piscina, &a, tertium, &e,
        &contractiones));
    CREDO_AEQUALIS_I32 (e.numerus, IV);
    CREDO_AEQUALIS_I32 (contractiones, II);
    CREDO_AEQUALIS_I32 (e.capita[ZEPHYRUM], II);
    CREDO_AEQUALIS_I32 (e.capita[I], ZEPHYRUM);
    CREDO_AEQUALIS_I32 (e.capita[II], II);   /* olim 4 */
    CREDO_AEQUALIS_I32 (e.capita[III], II);  /* olim 5 */
    CREDO_AEQUALIS_S32 ((s32)dependentia_validare(&e),
        (s32)DEPENDENTIA_SANA);
    /* folia omissa: nulla contractio, renumeratio */
    CREDO_VERUM (dependentia_omittere(piscina, &a, folia, &e,
        &contractiones));
    CREDO_AEQUALIS_I32 (e.numerus, III);
    CREDO_AEQUALIS_I32 (contractiones, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (e.capita[ZEPHYRUM], ZEPHYRUM);   /* olim 2 */
    CREDO_AEQUALIS_I32 (e.capita[I], I);                 /* olim 3 */
    CREDO_AEQUALIS_I32 (e.capita[II], II);               /* olim 5 */
    /* radix omissa: FALSUM */
    CREDO_FALSUM (dependentia_omittere(piscina, &a, radix, &e,
        &contractiones));
}

interior vacuum
_probare_limites (
    CredoContextus* c)
{
              Piscina* piscina      = c->piscina;
    constans      i32  cyclus[III]  = { ZEPHYRUM, III, II };
    constans      b32  nulla[III]   = { FALSUM, FALSUM, FALSUM };
     DependentiaArbor  a            = _arbor(cyclus, III);
     DependentiaArbor  e;
    DependentiaCruces  x;
   DependentiaMensura  m;
                  s32  componens[III];

    CREDO_FALSUM (dependentia_cruces(piscina, &a, &x));
    CREDO_FALSUM (dependentia_mensurare(piscina, &a, NIHIL, &m));
    CREDO_AEQUALIS_S32 (dependentia_nucleus_planaris(piscina, &a),
        (s32)-I);
    CREDO_AEQUALIS_S32 (dependentia_componentes(piscina, &a, componens),
        (s32)-I);
    CREDO_FALSUM (dependentia_omittere(piscina, &a, nulla, &e, NIHIL));
    CREDO_FALSUM (dependentia_cruces(piscina, NIHIL, &x));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "validatio", _probare_iudicium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "figurae", _probare_figuras, NIHIL, NIHIL, NIHIL, NIHIL },
    { "oracula", _probare_oracula, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fortuitae", _probare_fortuitas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lex tertiae", _probare_legem_tertiae, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "inversio", _probare_ordinem_versum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "omissio", _probare_verba_omissa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "limites", _probare_limites, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("dependentia", SECTIONES);
}
