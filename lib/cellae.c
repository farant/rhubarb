/* cellae.c - Cellae activationis combinatae super catalogum exactum
 *
 * Catalogus: clausura BFS ordine D82/D84 (vide cellae.h). Duplicata per
 * tabulam dispersam: clavis = textus repraesentantis {q, -q} (pars
 * prima non nulla positiva), textus anuli canonicus (probatur).
 * Candidatum refutatum (iam adest) piscinam ad notam reficit.
 * Vide lib/cellae.worklog.md.
 */
#include "cellae.h"
#include "tabula_dispersa.h"
#include "extensio.h"
#include "radices.h"

structura Catalogus {
    constans Anulus* anulus;
          Quaternio* elementa;
                i32* profunditates;
                s32* torsiones;
                i32  numerus;
                i32  capacitas;
                i32  gradus;
                b32  truncatus;
};


/* ==================================================
 * Auxilia
 * ================================================== */

/* -q */
interior b32
_negatum (
     Quaternio  q,
       Piscina* piscina,
     Quaternio* exitus)
{
    Quaternio nullum;

    si (!quaternio_nullum(quaternio_anulus(q), piscina, &nullum))
    {
        redde FALSUM;
    }
    redde quaternio_subtrahe(nullum, q, piscina, exitus);
}

/* clavis rotationis: textus q aut -q, pars prima non nulla positiva */
interior b32
_clavis (
     Quaternio  q,
       Piscina* piscina,
        chorda* exitus)
{
     constans Anulus* anulus = quaternio_anulus(q);
                 i32  k;

    per (k = ZEPHYRUM; k < IV; k++)
    {
        constans vacuum* pars = quaternio_pars(q, k);

        si (!anulus->est_nullum(anulus, pars))
        {
            s32 signum = ZEPHYRUM;

            si (!anulus->signum(anulus, pars, piscina, &signum))
            {
                redde FALSUM;
            }
            si (signum < ZEPHYRUM && !_negatum(q, piscina, &q))
            {
                redde FALSUM;
            }
            frange;
        }
    }
    *exitus = quaternio_ad_chordam(q, piscina);
    redde VERUM;
}

/* locus pro elemento novo; FALSUM si limes (truncatus notatur ab
 * vocante) */
interior b32
_capacitas (
    Catalogus* c,
          i32  limes,
      Piscina* piscina)
{
    si (limes > ZEPHYRUM && c->numerus >= limes)
    {
        redde FALSUM;
    }
    si (c->numerus >= c->capacitas)
    {
              i32  nova = c->capacitas * II;
        Quaternio* e = (Quaternio*)piscina_allocare(piscina,
            (memoriae_index)nova * magnitudo(Quaternio));
        i32* p = (i32*)piscina_allocare(piscina,
            (memoriae_index)nova * magnitudo(i32));
        s32* t = (s32*)piscina_allocare(piscina,
            (memoriae_index)nova * magnitudo(s32));
        i32 k;

        per (k = ZEPHYRUM; k < c->numerus; k++)
        {
            e[k] = c->elementa[k];
            p[k] = c->profunditates[k];
            t[k] = c->torsiones[k];
        }
        c->elementa       = e;
        c->profunditates  = p;
        c->torsiones      = t;
        c->capacitas      = nova;
    }
    redde VERUM;
}

/* q addere si nondum adest; *additum: VERUM si additum. Refutatum
 * (adest) piscinam ad notam reficit. */
interior b32
_addere (
          Catalogus* c,
     TabulaDispersa* tabula,
          Quaternio  q,
                i32  profunditas,
                s32  torsio,
                i32  limes,
            Piscina* piscina,
     PiscinaNotatio  nota)
{
    chorda clavis;

    si (!_clavis(q, piscina, &clavis))
    {
        redde FALSUM;
    }
    si (tabula_dispersa_continet(tabula, clavis))
    {
        piscina_reficere(piscina, nota);
        redde VERUM;
    }
    si (!_capacitas(c, limes, piscina))
    {
        c->truncatus = VERUM;
        piscina_reficere(piscina, nota);
        redde VERUM;
    }
    si (!tabula_dispersa_inserere(tabula, clavis, NIHIL))
    {
        redde FALSUM;
    }
    c->elementa[c->numerus]       = q;
    c->profunditates[c->numerus]  = profunditas;
    c->torsiones[c->numerus]      = torsio;
    c->numerus++;
    redde VERUM;
}


/* ==================================================
 * Clausura
 * ================================================== */

b32
catalogus_clausura (
    constans Quaternio*  generatores,
          constans s32*  torsiones,
                   i32   numerus,
                   i32   limes,
                   i32   gradus_maximi,
               Piscina*  piscina,
             Catalogus** exitus)
{
     constans Anulus* anulus;
           Catalogus* c;
      TabulaDispersa* tabula;
           Quaternio  unum;
                 i32  g;
                 i32  r;
                 i32  prior;

    si (   numerus   == ZEPHYRUM || generatores == NIHIL
        || torsiones == NIHIL)
    {
        redde FALSUM;
    }
    anulus = quaternio_anulus(generatores[ZEPHYRUM]);
    si (anulus == NIHIL || anulus->signum == NIHIL)
    {
        redde FALSUM;
    }
    per (g = I; g < numerus; g++)
    {
        si (quaternio_anulus(generatores[g]) != anulus)
        {
            redde FALSUM;
        }
    }
    c = (Catalogus*)piscina_allocare(piscina, magnitudo(Catalogus));
    tabula = tabula_dispersa_creare_chorda(piscina, (i32)MMXLVIII);
    si (   c == NIHIL || tabula == NIHIL
        || !quaternio_unum(anulus, piscina,
        &unum))
    {
        redde FALSUM;
    }
    c->anulus     = anulus;
    c->numerus    = ZEPHYRUM;
    c->capacitas  = (i32)LXIV;
    c->gradus     = ZEPHYRUM;
    c->truncatus  = FALSUM;
    c->elementa       = (Quaternio*)piscina_allocare(piscina,
        (memoriae_index)c->capacitas * magnitudo(Quaternio));
    c->profunditates  = (i32*)piscina_allocare(piscina,
        (memoriae_index)c->capacitas * magnitudo(i32));
    c->torsiones      = (s32*)piscina_allocare(piscina,
        (memoriae_index)c->capacitas * magnitudo(s32));
    /* gradus 0: 1, deinde generatores */
    si (!_addere(c, tabula, unum, ZEPHYRUM, ZEPHYRUM, limes, piscina,
        piscina_notare(piscina)))
    {
        redde FALSUM;
    }
    per (g = ZEPHYRUM; g < numerus; g++)
    {
        si (!_addere(c, tabula, generatores[g], ZEPHYRUM, torsiones[g],
            limes, piscina, piscina_notare(piscina)))
        {
            redde FALSUM;
        }
    }
    c->gradus  = I;
    r          = I;
    fac
    {
        i32 i;

        prior = c->numerus;
        per (i = ZEPHYRUM; i < prior; i++)
        {
            per (g = ZEPHYRUM; g < numerus; g++)
            {
                PiscinaNotatio nota = piscina_notare(piscina);
                     Quaternio productum;

                si (!quaternio_multiplica(c->elementa[i],
                    generatores[g],
                    piscina, &productum))
                {
                    redde FALSUM;
                }
                si (!_addere(c, tabula, productum, r, c->torsiones[i]
                    + torsiones[g], limes, piscina, nota))
                {
                    redde FALSUM;
                }
            }
        }
        si (c->numerus > prior)
        {
            c->gradus++;
        }
        r++;
    } dum (c->numerus > prior && (gradus_maximi == ZEPHYRUM
        || c->gradus < gradus_maximi));
    *exitus = c;
    redde VERUM;
}


/* ==================================================
 * Lectio
 * ================================================== */

i32
catalogus_numerus (
    constans Catalogus* catalogus)
{
    redde catalogus->numerus;
}

Quaternio
catalogus_elementum (
    constans Catalogus* catalogus,
                   i32  i)
{
    Quaternio nullus;

    si (i < catalogus->numerus)
    {
        redde catalogus->elementa[i];
    }
    nullus.anulus = NIHIL;
    nullus.partes = NIHIL;
    redde nullus;
}

i32
catalogus_profunditas (
    constans Catalogus* catalogus,
                   i32  i)
{
    redde i < catalogus->numerus ? catalogus->profunditates[i]
        : (i32)-1;
}

s32
catalogus_torsio (
    constans Catalogus* catalogus,
                   i32  i)
{
    redde i < catalogus->numerus ? catalogus->torsiones[i] : ZEPHYRUM;
}

b32
catalogus_nullum (
    constans Catalogus* catalogus,
                   i32  i)
{
    si (i >= catalogus->numerus)
    {
        redde FALSUM;
    }
    redde catalogus->anulus->est_nullum(catalogus->anulus,
        quaternio_pars(catalogus->elementa[i], ZEPHYRUM));
}

b32
catalogus_truncatus (
    constans Catalogus* catalogus)
{
    redde catalogus->truncatus;
}

i32
catalogus_gradus (
    constans Catalogus* catalogus)
{
    redde catalogus->gradus;
}

b32
catalogus_pars (
    constans Catalogus*  catalogus,
          constans i32*  indices,
                   i32   numerus,
               Piscina*  piscina,
             Catalogus** exitus)
{
     Catalogus* c;
            i8* visus;
           i32  k;

    si (numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    visus = (i8*)piscina_allocare(piscina,
        (memoriae_index)catalogus->numerus);
    per (k = ZEPHYRUM; k < catalogus->numerus; k++)
    {
        visus[k] = ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (indices[k] >= catalogus->numerus || visus[indices[k]])
        {
            redde FALSUM;
        }
        visus[indices[k]] = I;
    }
    c = (Catalogus*)piscina_allocare(piscina, magnitudo(Catalogus));
    c->anulus = catalogus->anulus;
    c->numerus = numerus;
    c->capacitas = numerus;
    c->gradus = catalogus->gradus;
    c->truncatus = catalogus->truncatus;
    c->elementa       = (Quaternio*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(Quaternio));
    c->profunditates  = (i32*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(i32));
    c->torsiones      = (s32*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(s32));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        c->elementa[k]       = catalogus->elementa[indices[k]];
        c->profunditates[k]  = catalogus->profunditates[indices[k]];
        c->torsiones[k]      = catalogus->torsiones[indices[k]];
    }
    *exitus = c;
    redde VERUM;
}


/* ==================================================
 * Activatio
 * ================================================== */

#define CELLAE_K_MAXIMA 64             /* k distincti in memoria puncti */

#define GENUS_NULLUM    0              /* s = 0 */
#define GENUS_REALE     1              /* v = 0 */
#define GENUS_NULLA     2              /* a = 0 */
#define GENUS_GENERALE  3

nomen structura {
            i32  k;
            i32  regula;
            i32  numerus;
            i32* ligatae;
} CellaK;

nomen structura {
            i32  genus;
    RadixRealis  tangens;              /* tan^2 h (genus generale) */
            i32  numerus_axium;        /* paritas axium */
            i32* axes;
            i32  numerus_k;
         CellaK  per_k[CELLAE_K_MAXIMA];
} Punctum;

nomen structura {
            i32  k;
            i32  numerus;              /* termini m < k/2 */
    RadixRealis* termini;
} Termini;

structura Activatio {
    constans Catalogus* catalogus;
             Quaternio* axes;
                   i32  numerus_axium;
                   b32  truncata;
        TabulaDispersa* puncta;         /* clavis -> Punctum* */
               Termini  termini[CELLAE_K_MAXIMA];
                   i32  numerus_terminorum;
               Piscina* piscina;
};

interior constans Algebraicus*
_pars (
    Quaternio q,
          i32 k)
{
    redde (constans Algebraicus*)quaternio_pars(q, k);
}

/* clavis axis: pars vectoria per partem primam non nullam divisa */
interior b32
_clavis_axis (
    Quaternio  q,
      Piscina* piscina,
       chorda* exitus)
{
    Algebraicus inversum;
            i32 primus = I;
            i32 k;
         chorda c;

    dum (primus <= III && algebraicus_est_nullum(*_pars(q, primus)))
    {
        primus++;
    }
    si (   primus > III
        || !algebraicus_inversum(*_pars(q, primus), piscina,
        &inversum))
    {
        redde FALSUM;
    }
    c = chorda_ex_literis("", piscina);
    per (k = I; k <= III; k++)
    {
        c = chorda_concatenare(c, algebraicus_ad_chordam(
            algebraicus_multiplica(*_pars(q, k), inversum, piscina),
            piscina), piscina);
        c = chorda_concatenare(c, chorda_ex_literis("|", piscina),
            piscina);
    }
    *exitus = c;
    redde VERUM;
}

b32
activatio_ex_catalogo (
    constans Catalogus*  catalogus,
                   i32   axes_maximae,
               Piscina*  piscina,
             Activatio** exitus)
{
            Activatio* a;
       TabulaDispersa* visae;
    constans Extensio* corpus;
                  i32  i;

    si (catalogus->numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* anulus extensionis? contextus = corpus et extensio_anulus(corpus)
     * idem anulus (alii anuli: Algebraicus non sunt, non legendi) */
    corpus = (constans Extensio*)catalogus->anulus->contextus;
    si (   corpus                  == NIHIL
        || extensio_anulus(corpus) != catalogus->anulus
        || extensio_gradus(corpus) != II || !extensio_ordinata(corpus))
    {
        redde FALSUM;
    }
    a = (Activatio*)piscina_allocare(piscina, magnitudo(Activatio));
    visae = tabula_dispersa_creare_chorda(piscina, (i32)MMXLVIII);
    a->puncta = tabula_dispersa_creare_chorda(piscina, (i32)MMXLVIII);
    a->axes = (Quaternio*)piscina_allocare(piscina,
        (memoriae_index)catalogus->numerus * magnitudo(Quaternio));
    a->catalogus           = catalogus;
    a->numerus_axium       = ZEPHYRUM;
    a->truncata            = FALSUM;
    a->numerus_terminorum  = ZEPHYRUM;
    a->piscina             = piscina;
    per (i = ZEPHYRUM; i < catalogus->numerus; i++)
    {
        Quaternio q = catalogus->elementa[i];
           chorda clavis;

        si (   algebraicus_est_nullum(*_pars(q, I))
            && algebraicus_est_nullum(*_pars(q, II))
            && algebraicus_est_nullum(*_pars(q, III)))
        {
            perge;                     /* +-1: sine axe */
        }
        si (!_clavis_axis(q, piscina, &clavis))
        {
            redde FALSUM;
        }
        si (tabula_dispersa_continet(visae, clavis))
        {
            perge;
        }
        si (axes_maximae > ZEPHYRUM && a->numerus_axium >= axes_maximae)
        {
            a->truncata = VERUM;
            perge;
        }
        (vacuum)tabula_dispersa_inserere(visae, clavis, NIHIL);
        a->axes[a->numerus_axium++] = q;
    }
    si (a->numerus_axium == ZEPHYRUM)
    {
        redde FALSUM;
    }
    *exitus = a;
    redde VERUM;
}

i32
activatio_axes (
    constans Activatio* activatio)
{
    redde activatio->numerus_axium;
}

b32
activatio_truncata (
    constans Activatio* activatio)
{
    redde activatio->truncata;
}

/* S_k(u) = sum_l C(k, 2l+1) (-1)^l u^l: radices tan^2(m pi/k), m = 1 ..
 * (k-1)/2, crescentes (D123) */
interior Termini*
_termini (
    Activatio* a,
          i32  k)
{
     Termini* t;
         i32  j;

    per (j = ZEPHYRUM; j < a->numerus_terminorum; j++)
    {
        si (a->termini[j].k == k)
        {
            redde &a->termini[j];
        }
    }
    si (a->numerus_terminorum >= CELLAE_K_MAXIMA)
    {
        redde NIHIL;
    }
    t           = &a->termini[a->numerus_terminorum];
    t->k        = k;
    t->numerus  = (k - I) / II;
    t->termini  = NIHIL;
    si (t->numerus > ZEPHYRUM)
    {
        Magnus* c = (Magnus*)piscina_allocare(a->piscina,
            (memoriae_index)(t->numerus + I) * magnitudo(Magnus));
            Magnus binomium  = magnus_ex_s64(I);
        Polynomium s         = polynomium_nullum();
               i32 n         = ZEPHYRUM;
               i32 m;

        per (m = I; m <= k; m++)
        {
            (vacuum)magnus_divide(magnus_multiplica(binomium,
                magnus_ex_s64((s64)(k - m + I)), a->piscina),
                magnus_ex_s64((s64)m), a->piscina, &binomium, NIHIL);
            si (m % II == I && (m - I) / II <= t->numerus)
            {
                c[(m - I) / II] = ((m - I) / II) % II == I
                    ? magnus_nega(binomium, a->piscina) : binomium;
            }
        }
        si (   !polynomium_ex_coefficientibus(c, t->numerus + I,
            ZEPHYRUM,
                a->piscina, &s)
            || !radices_reales(s, a->piscina, &t->termini, &n)
            || (i32)n != t->numerus)
        {
            redde NIHIL;
        }
    }
    a->numerus_terminorum++;
    redde t;
}

/* tan^2 h ut radix: t^2 - Tr t + N (corpus quadraticum) */
interior b32
_tangens (
    Algebraicus  t,
        Piscina* piscina,
    RadixRealis* exitus)
{
        Fractio  q          = fractio_ex_s64(ZEPHYRUM);
        Fractio  vestigium  = fractio_ex_s64(ZEPHYRUM);
        Fractio  norma      = fractio_ex_s64(ZEPHYRUM);
         Magnus  d;
         Magnus  c[III];
     Polynomium  f         = polynomium_nullum();
    RadixRealis* r         = NIHIL;
            i32  n         = ZEPHYRUM;
            s32  signum    = ZEPHYRUM;
        Fractio  dimidium  = fractio_ex_s64(ZEPHYRUM);

    si (algebraicus_est_rationalis(t, piscina, &q))
    {
        *exitus = radix_ex_fractione(q, piscina);
        redde VERUM;
    }
    si (   !algebraicus_vestigium(t, piscina, &vestigium)
        || !algebraicus_norma(t, piscina, &norma))
    {
        redde FALSUM;
    }
    d = magnus_multiplica(fractio_denominator(vestigium),
        fractio_denominator(norma), piscina);
    c[II] = d;
    c[I] = magnus_nega(fractio_numerator(fractio_multiplica(vestigium,
        fractio_ex_magno(d), piscina)), piscina);
    c[ZEPHYRUM] = fractio_numerator(fractio_multiplica(norma,
        fractio_ex_magno(d), piscina));
    si (   !polynomium_ex_coefficientibus(c, III, ZEPHYRUM, piscina, &f)
        || !radices_reales(f, piscina, &r, &n) || n != II
        || !fractio_ex_s64_s64(I, II, piscina, &dimidium)
        || !algebraicus_signum(algebraicus_subtrahe(t,
            algebraicus_ex_fractione(algebraicus_corpus(t),
            fractio_multiplica(vestigium, dimidium, piscina), piscina),
            piscina), piscina, &signum))
    {
        redde FALSUM;
    }
    *exitus = r[signum > ZEPHYRUM ? I : ZEPHYRUM];
    redde VERUM;
}

/* punctum (sine k): genus, tan^2 h, paritas axium */
interior Punctum*
_punctum (
    Activatio* a,
    Quaternio  s)
{
    Piscina* piscina = a->piscina;
     chorda  clavis;
     vacuum* inventum = NIHIL;
    Punctum* p;
        b32  a_nulla;
        b32  v_nulla;

    si (!_clavis(s, piscina, &clavis))
    {
        redde NIHIL;
    }
    si (tabula_dispersa_invenire(a->puncta, clavis, &inventum))
    {
        redde (Punctum*)inventum;
    }
    p = (Punctum*)piscina_allocare(piscina, magnitudo(Punctum));
    p->numerus_k = ZEPHYRUM;
    p->numerus_axium = ZEPHYRUM;
    p->axes = NIHIL;
    a_nulla = algebraicus_est_nullum(*_pars(s, ZEPHYRUM));
    v_nulla = algebraicus_est_nullum(*_pars(s, I))
        && algebraicus_est_nullum(*_pars(s, II))
        && algebraicus_est_nullum(*_pars(s, III));
    si (v_nulla)
    {
        p->genus = a_nulla ? GENUS_NULLUM : GENUS_REALE;
    } alioquin
    {
        p->axes = (i32*)piscina_allocare(piscina,
            (memoriae_index)a->numerus_axium * magnitudo(i32));
        si (!quaternio_proximi(s, a->axes, a->numerus_axium, VERUM,
            piscina, p->axes, &p->numerus_axium))
        {
            redde NIHIL;
        }
        si (a_nulla)
        {
            p->genus = GENUS_NULLA;
        } alioquin
        {
            Algebraicus v2 = algebraicus_adde(algebraicus_adde(
                algebraicus_multiplica(*_pars(s, I), *_pars(s, I),
                piscina),
                algebraicus_multiplica(*_pars(s, II), *_pars(s, II),
                piscina), piscina), algebraicus_multiplica(*_pars(s,
                III),
                *_pars(s, III), piscina), piscina);
            Algebraicus t;

            p->genus = GENUS_GENERALE;
            si (   !algebraicus_divide(v2, algebraicus_multiplica(
                    *_pars(s, ZEPHYRUM), *_pars(s, ZEPHYRUM), piscina),
                    piscina, &t)
                || !_tangens(t, piscina, &p->tangens))
            {
                redde NIHIL;
            }
        }
    }
    si (!tabula_dispersa_inserere(a->puncta, clavis, p))
    {
        redde NIHIL;
    }
    redde p;
}

b32
activatio_cella (
     Activatio* activatio,
     Quaternio  summa,
           i32  k,
         Cella* exitus)
{
    Punctum* p;
     CellaK* ck;
        i32  nv = activatio->numerus_axium + I;
        i32  sectiones[II];
        i32  n_sec = I;
        i32  j;
        i32  m;

    si (   k < I
        || quaternio_anulus(summa) != activatio->catalogus->anulus)
    {
        redde FALSUM;
    }
    p = _punctum(activatio, summa);
    si (p == NIHIL)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < p->numerus_k; j++)
    {
        si (p->per_k[j].k == k)
        {
            exitus->regula   = p->per_k[j].regula;
            exitus->numerus  = p->per_k[j].numerus;
            exitus->ligatae  = p->per_k[j].ligatae;
            redde VERUM;
        }
    }
    si (p->numerus_k >= CELLAE_K_MAXIMA)
    {
        redde FALSUM;
    }
    ck     = &p->per_k[p->numerus_k];
    ck->k  = k;
    si (p->genus == GENUS_NULLUM || p->genus == GENUS_REALE)
    {
        ck->ligatae = (i32*)piscina_allocare(activatio->piscina,
            magnitudo(i32));
        ck->regula = p->genus == GENUS_NULLUM
            ? (k - I) * nv + activatio->numerus_axium
            : activatio->numerus_axium;
        ck->ligatae[ZEPHYRUM]  = ck->regula;
        ck->numerus            = I;
    } alioquin
    {
        si (p->genus == GENUS_NULLA)
        {
            sectiones[ZEPHYRUM] = k / II;
            si (k % II == ZEPHYRUM)
            {
                sectiones[I]  = k / II - I;
                n_sec         = II;
            }
        } alioquin
        {
             Termini* t    = _termini(activatio, k);
                 b32  par  = FALSUM;

            si (t == NIHIL)
            {
                redde FALSUM;
            }
            sectiones[ZEPHYRUM] = ZEPHYRUM;
            per (m = I; m <= t->numerus; m++)
            {
                s32 s = ZEPHYRUM;

                si (!radix_compara(p->tangens, t->termini[m - I],
                    activatio->piscina, &s))
                {
                    redde FALSUM;
                }
                si (s < ZEPHYRUM)
                {
                    frange;
                }
                sectiones[ZEPHYRUM]  = m;
                par                  = s == ZEPHYRUM;
            }
            si (par)
            {
                sectiones[I]  = sectiones[ZEPHYRUM] - I;
                n_sec         = II;
            }
        }
        ck->numerus = n_sec * p->numerus_axium;
        ck->ligatae = (i32*)piscina_allocare(activatio->piscina,
            (memoriae_index)ck->numerus * magnitudo(i32));
        ck->regula = sectiones[ZEPHYRUM] * nv + p->axes[ZEPHYRUM];
        per (m = ZEPHYRUM; m < n_sec; m++)
        {
            per (j = ZEPHYRUM; j < p->numerus_axium; j++)
            {
                ck->ligatae[m * p->numerus_axium + j] =
                    sectiones[m] * nv
                    + p->axes[j];
            }
        }
    }
    p->numerus_k++;
    exitus->regula   = ck->regula;
    exitus->numerus  = ck->numerus;
    exitus->ligatae  = ck->ligatae;
    redde VERUM;
}
