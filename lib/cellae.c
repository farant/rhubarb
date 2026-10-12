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
            i32  signum_visitationis;  /* separatio: visitatio currens */
            i32  paritas_visitationis;
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
                    i8* visum_par;        /* cella: bit paritatis visae */
                   i32* variabilis;       /* cella -> variabilis SAT + 1 */
                   i32  capacitas_cellarum;
                   i32  visitatio;        /* signum separationis currentis */
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
    a->visum_par           = NIHIL;
    a->variabilis          = NIHIL;
    a->capacitas_cellarum  = ZEPHYRUM;
    a->visitatio           = ZEPHYRUM;
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

/* punctum (sine k): genus, tan^2 h, paritas axium. Clavis in opus
 * (piscina temporaria vocantis) aut, opus NIHIL, in piscina
 * activationis cum refectione si inventum: inventio nihil relinquit;
 * solum punctum novum (clave transcripta) manet. */
interior Punctum*
_punctum (
    Activatio* a,
    Quaternio  s,
      Piscina* opus)
{
           Piscina* piscina  = a->piscina;
    PiscinaNotatio  nota     = piscina_notare(piscina);
            chorda  clavis;
            vacuum* inventum = NIHIL;
           Punctum* p;
               b32  a_nulla;
               b32  v_nulla;

    si (!_clavis(s, opus != NIHIL ? opus : piscina, &clavis))
    {
        redde NIHIL;
    }
    si (tabula_dispersa_invenire(a->puncta, clavis, &inventum))
    {
        si (opus == NIHIL)
        {
            piscina_reficere(piscina, nota);
        }
        redde (Punctum*)inventum;
    }
    clavis = chorda_transcribere(clavis, piscina);
    p = (Punctum*)piscina_allocare(piscina, magnitudo(Punctum));
    p->signum_visitationis = ZEPHYRUM;
    p->paritas_visitationis = ZEPHYRUM;
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

/* cella puncti ad k (memoria per k in puncto) */
interior b32
_cella_puncti (
    Activatio* activatio,
      Punctum* p,
          i32  k,
        Cella* exitus)
{
     CellaK* ck;
        i32  nv = activatio->numerus_axium + I;
        i32  sectiones[II];
        i32  n_sec = I;
        i32  j;
        i32  m;

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

b32
activatio_cella (
     Activatio* activatio,
     Quaternio  summa,
           i32  k,
         Cella* exitus)
{
    Punctum* p;

    si (   k < I
        || quaternio_anulus(summa) != activatio->catalogus->anulus)
    {
        redde FALSUM;
    }
    p = _punctum(activatio, summa, NIHIL);
    si (p == NIHIL)
    {
        redde FALSUM;
    }
    redde _cella_puncti(activatio, p, k, exitus);
}


/* ==================================================
 * Separabilitas
 * ================================================== */

#define CELLAE_LIMES_DECISIONUM 200000L

/* alvei per cellam ad k (nulli; per indices tactos purgantur) */
interior b32
_alvei_cellarum (
    Activatio* a,
          i32  k)
{
    i32 opus = k * (a->numerus_axium + I);
    i32 j;

    si (opus <= a->capacitas_cellarum)
    {
        redde VERUM;
    }
    a->visum_par = (i8*)piscina_allocare(a->piscina,
        (memoriae_index)opus);
    a->variabilis = (i32*)piscina_allocare(a->piscina,
        (memoriae_index)opus * magnitudo(i32));
    si (a->visum_par == NIHIL || a->variabilis == NIHIL)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < opus; j++)
    {
        a->visum_par[j]   = ZEPHYRUM;
        a->variabilis[j]  = ZEPHYRUM;
    }
    a->capacitas_cellarum = opus;
    redde VERUM;
}

/* cella regulae fixae: sector superior/inferior, axis ultima/prima */
interior i32
_cella_fixa (
    constans Cella* c,
               i32  nv,
               b32  superior,
               b32  ultima)
{
    i32 sector  = c->ligatae[ZEPHYRUM] / nv;
    i32 axis    = c->ligatae[ZEPHYRUM] % nv;
    i32 j;

    per (j = I; j < c->numerus; j++)
    {
        i32 s = c->ligatae[j] / nv;
        i32 x = c->ligatae[j] % nv;

        si (superior ? s > sector : s < sector) sector = s;
        si (ultima ? x > axis : x < axis) axis = x;
    }
    redde sector * nv + axis;
}

/* SAT (D121/D124): variabilis = pars cellae (paritas assignata); clausula
 * per punctum: aliqua cellarum eius paritatem puncti habet */
nomen structura {
             i32  numerus_clausularum;
    constans i32* volitum;
    constans i32* initium;
    constans i32* longitudo;
    constans i32* literae;
             s32* valor;                  /* -1 incertum, 0, 1 */
             i32* vestigium;
             i32  altitudo;
            long  decisiones;
} Sat;

interior b32
_sat_propaga (
    Sat* t)
{
    b32 mutatum = VERUM;

    dum (mutatum)
    {
        i32 m;

        mutatum = FALSUM;
        per (m = ZEPHYRUM; m < t->numerus_clausularum; m++)
        {
            i32 volitum  = t->volitum[m];
            s32 liber    = -(s32)I;
            i32 liberi   = ZEPHYRUM;
            b32 satis    = FALSUM;
            i32 j;

            per (j = ZEPHYRUM; j < t->longitudo[m] && !satis; j++)
            {
                i32 v = t->literae[t->initium[m] + j];

                si (t->valor[v] == (s32)volitum)
                {
                    satis = VERUM;
                } alioquin si (t->valor[v] < ZEPHYRUM)
                {
                    si (liber != (s32)v)
                    {
                        liberi++;
                    }
                    liber = (s32)v;
                }
            }
            si (satis)
            {
                perge;
            }
            si (liberi == ZEPHYRUM)
            {
                redde FALSUM;
            }
            si (liberi == I)
            {
                t->valor[liber]              = (s32)volitum;
                t->vestigium[t->altitudo++]  = (i32)liber;
                mutatum                      = VERUM;
            }
        }
    }
    redde VERUM;
}

/* 1 satisfactum, 0 non, -1 limes decisionum */
interior s32
_sat_solve (
    Sat* t)
{
    i32 nota       = t->altitudo;
    s32 eligendus  = -(s32)I;
    i32 volitum    = ZEPHYRUM;
    s32 r;
    i32 m;

    si (!_sat_propaga(t))
    {
        r = ZEPHYRUM;
        salta finis;
    }
    per (m = ZEPHYRUM; m < t->numerus_clausularum && eligendus < 0; m++)
    {
        b32 satis       = FALSUM;
        s32 candidatus  = -(s32)I;
        i32 j;

        per (j = ZEPHYRUM; j < t->longitudo[m]; j++)
        {
            i32 v = t->literae[t->initium[m] + j];

            si (t->valor[v] == (s32)t->volitum[m])
            {
                satis = VERUM;
                frange;
            }
            si (t->valor[v] < ZEPHYRUM && candidatus < 0)
            {
                candidatus = (s32)v;
            }
        }
        si (!satis)
        {
            eligendus  = candidatus;
            volitum    = t->volitum[m];
        }
    }
    si (eligendus < 0)
    {
        redde I;
    }
    si (++t->decisiones > CELLAE_LIMES_DECISIONUM)
    {
        r = -(s32)I;
        salta finis;
    }
    t->valor[eligendus]          = (s32)volitum;
    t->vestigium[t->altitudo++]  = (i32)eligendus;
    r                            = _sat_solve(t);
    si (r != ZEPHYRUM)
    {
        salta finis;
    }
    t->valor[eligendus]  = (s32)(I - volitum);
    r                    = _sat_solve(t);
finis:
    si (r != I)
    {
        dum (t->altitudo > nota)
        {
            t->valor[t->vestigium[--t->altitudo]] = -(s32)I;
        }
    }
    redde r;
}

interior b32
_separat_intra (
       Activatio* a,
    constans i32* pondera,
             i32  n,
             i32  k,
             i32  modus,
         Piscina* opus,
             b32* exitus)
{
          i32   nv        = a->numerus_axium + I;
          i32   vectores  = I;
          i32   numerus   = ZEPHYRUM;
      Punctum** puncta;
          i32*  paritates;
          i32*  tacta;
          i32   numerus_tactorum = ZEPHYRUM;
          i32   v;
          i32   j;
          b32   separat = VERUM;
    Quaternio   nullum;

    per (j = ZEPHYRUM; j < n; j++)
    {
        vectores *= III;
    }
    puncta = (Punctum**)piscina_allocare(opus, (memoriae_index)vectores
        * magnitudo(Punctum*));
    paritates = (i32*)piscina_allocare(opus, (memoriae_index)vectores
        * magnitudo(i32));
    si (   puncta == NIHIL || paritates == NIHIL
        || !quaternio_nullum(a->catalogus->anulus, opus, &nullum)
        || !_alvei_cellarum(a, k))
    {
        redde FALSUM;
    }
    /* vectores plicati: coefficiens primus non nullus +1 (et nullus) */
    per (v = ZEPHYRUM; v < vectores; v++)
    {
        Quaternio s        = nullum;
              i32 x        = v;
              i32 par      = ZEPHYRUM;
              b32 primus   = VERUM;
              b32 negatus  = FALSUM;

        per (j = ZEPHYRUM; j < n && !negatus; j++)
        {
                  i32 digitus  = x % III;
            Quaternio q        = a->catalogus->elementa[pondera[j]];

            x /= III;
            si (digitus == ZEPHYRUM)
            {
                perge;
            }
            si (primus && digitus == II)
            {
                negatus = VERUM;
                perge;
            }
            primus  = FALSUM;
            par     ^= I;
            si (!(digitus == I ? quaternio_adde(s, q, opus, &s)
                : quaternio_subtrahe(s, q, opus, &s)))
            {
                redde FALSUM;
            }
        }
        si (negatus)
        {
            perge;
        }
        puncta[numerus] = _punctum(a, s, opus);
        si (puncta[numerus] == NIHIL)
        {
            redde FALSUM;
        }
        paritates[numerus++] = par;
    }
    tacta = (i32*)piscina_allocare(opus, (memoriae_index)numerus
        * (memoriae_index)II * (memoriae_index)nv * magnitudo(i32));
    si (tacta == NIHIL)
    {
        redde FALSUM;
    }
    si (modus != CELLAE_POSSIBILIS)
    {
        per (v = ZEPHYRUM; v < numerus && separat; v++)
        {
                   Cella  c;
                     i32  cellae[I];
            constans i32* lista      = cellae;
                     i32  longitudo  = I;

            si (!_cella_puncti(a, puncta[v], k, &c))
            {
                redde FALSUM;
            }
            si (modus == CELLAE_REGULA)
            {
                cellae[ZEPHYRUM] = c.regula;
            } alioquin si (modus == CELLAE_ROBUSTA)
            {
                lista      = c.ligatae;
                longitudo  = c.numerus;
            } alioquin
            {
                cellae[ZEPHYRUM] = _cella_fixa(&c, nv,
                    (b32)((modus - III) / II), (b32)((modus - III)
                        % II));
            }
            per (j = ZEPHYRUM; j < longitudo && separat; j++)
            {
                i32 cella = lista[j];

                si (a->visum_par[cella] == ZEPHYRUM)
                {
                    tacta[numerus_tactorum++] = cella;
                }
                si (a->visum_par[cella] & (I << (I - paritates[v])))
                {
                    separat = FALSUM;
                }
                a->visum_par[cella] = (i8)(a->visum_par[cella]
                    | (I << paritates[v]));
            }
        }
        per (j = ZEPHYRUM; j < numerus_tactorum; j++)
        {
            a->visum_par[tacta[j]] = ZEPHYRUM;
        }
        *exitus = separat;
        redde VERUM;
    }
    /* possibilis: puncta distincta, paritas una per punctum */
    {
        Sat  t;
        i32* volitum = (i32*)piscina_allocare(opus,
            (memoriae_index)numerus
            * magnitudo(i32));
        i32* initium = (i32*)piscina_allocare(opus,
            (memoriae_index)numerus
            * magnitudo(i32));
        i32* longitudo = (i32*)piscina_allocare(opus,
            (memoriae_index)numerus * magnitudo(i32));
        i32* literae = (i32*)piscina_allocare(opus,
            (memoriae_index)numerus
            * (memoriae_index)II * (memoriae_index)nv * magnitudo(i32));
        i32 clausulae          = ZEPHYRUM;
        i32 numerus_literarum  = ZEPHYRUM;
        i32 variabiles         = ZEPHYRUM;
        s32 r                  = I;

        a->visitatio++;
        per (v = ZEPHYRUM; v < numerus && separat; v++)
        {
            Punctum* p = puncta[v];
              Cella  c;

            si (p->signum_visitationis == a->visitatio)
            {
                si (p->paritas_visitationis != paritates[v])
                {
                    separat = FALSUM;
                }
                perge;
            }
            p->signum_visitationis   = a->visitatio;
            p->paritas_visitationis  = paritates[v];
            si (!_cella_puncti(a, p, k, &c))
            {
                redde FALSUM;
            }
            volitum[clausulae]    = paritates[v];
            initium[clausulae]    = numerus_literarum;
            longitudo[clausulae]  = c.numerus;
            per (j = ZEPHYRUM; j < c.numerus; j++)
            {
                i32 cella = c.ligatae[j];

                si (a->variabilis[cella] == ZEPHYRUM)
                {
                    a->variabilis[cella]       = ++variabiles;
                    tacta[numerus_tactorum++]  = cella;
                }
                literae[numerus_literarum++] = a->variabilis[cella] - I;
            }
            clausulae++;
        }
        si (separat)
        {
            t.numerus_clausularum  = clausulae;
            t.volitum              = volitum;
            t.initium              = initium;
            t.longitudo            = longitudo;
            t.literae              = literae;
            t.valor = (s32*)piscina_allocare(opus, (memoriae_index)
                (variabiles + I) * magnitudo(s32));
            t.vestigium = (i32*)piscina_allocare(opus, (memoriae_index)
                (variabiles + I) * magnitudo(i32));
            t.altitudo    = ZEPHYRUM;
            t.decisiones  = 0L;
            per (j = ZEPHYRUM; j < variabiles; j++)
            {
                t.valor[j] = -(s32)I;
            }
            r = _sat_solve(&t);
        }
        per (j = ZEPHYRUM; j < numerus_tactorum; j++)
        {
            a->variabilis[tacta[j]] = ZEPHYRUM;
        }
        si (r < ZEPHYRUM)
        {
            redde FALSUM;              /* limes decisionum: indecisum */
        }
        *exitus = separat && r == I;
        redde VERUM;
    }
}

b32
activatio_separat (
       Activatio* activatio,
    constans i32* pondera,
             i32  n,
             i32  k,
             i32  modus,
             b32* exitus)
{
    Piscina* opus;
        b32  verdictum = FALSUM;
        b32  bene;
        i32  j;

    si (n == ZEPHYRUM || n > VII || k < I || modus > CELLAE_FIXA(I, I))
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < n; j++)
    {
        si (pondera[j] >= activatio->catalogus->numerus)
        {
            redde FALSUM;
        }
    }
    opus = piscina_generare_dynamicum("cellae_separatio",
        (memoriae_index)65536);
    si (opus == NIHIL)
    {
        redde FALSUM;
    }
    bene = _separat_intra(activatio, pondera, n, k, modus, opus,
        &verdictum);
    piscina_destruere(opus);
    si (bene)
    {
        *exitus = verdictum;
    }
    redde bene;
}


/* ==================================================
 * Quaestiones
 * ================================================== */

/* copia mascarum (u64, 0 vacuum) per apertam inscriptionem */
nomen structura {
    i64* claves;
    i64  capacitas;
    i64  numerus;
} Larvae;

interior b32
_larvae_initium (
       Larvae* m,
          i64  capacitas,
      Piscina* piscina)
{
    i64 j;

    m->capacitas  = capacitas;
    m->numerus    = 0;
    m->claves = (i64*)piscina_allocare(piscina,
        (memoriae_index)capacitas
        * magnitudo(i64));
    si (m->claves == NIHIL)
    {
        redde FALSUM;
    }
    per (j = 0; j < capacitas; j++)
    {
        m->claves[j] = 0;
    }
    redde VERUM;
}

interior i64
_larvae_locus (
    constans Larvae* m,
                i64  x)
{
    i64 h = (i64)((x * (i64)0x9E3779B97F4A7C15ULL) >> 17)
        % m->capacitas;

    dum (m->claves[h] != 0 && m->claves[h] != x)
    {
        h = (h + 1) % m->capacitas;
    }
    redde h;
}

interior b32
_larvae_continent (
    constans Larvae* m,
                i64  x)
{
    redde m->claves[_larvae_locus(m, x)] == x;
}

/* VERUM si novum additum */
interior b32
_larvae_adde (
     Larvae* m,
        i64  x,
    Piscina* piscina)
{
    i64 h;

    si (2 * (m->numerus + 1) > m->capacitas)
    {
        Larvae nova;
           i64 j;

        si (!_larvae_initium(&nova, 2 * m->capacitas + 16, piscina))
        {
            redde FALSUM;
        }
        per (j = 0; j < m->capacitas; j++)
        {
            si (m->claves[j] != 0)
            {
                nova.claves[_larvae_locus(&nova, m->claves[j])]
                    = m->claves[j];
                nova.numerus++;
            }
        }
        *m = nova;
    }
    h = _larvae_locus(m, x);
    si (m->claves[h] == x)
    {
        redde FALSUM;
    }
    m->claves[h] = x;
    m->numerus++;
    redde VERUM;
}

/* indices permissi */
interior i32
_permissi (
    constans Activatio*  a,
     constans Quaestio*  q,
               Piscina*  piscina,
                   i32** exitus)
{
    i32 n = q->indices != NIHIL ? q->numerus_indicum
        : a->catalogus->numerus;
    i32* x = (i32*)piscina_allocare(piscina, (memoriae_index)(n + I)
        * magnitudo(i32));
    i32 j;

    per (j = ZEPHYRUM; j < n; j++)
    {
        x[j] = q->indices != NIHIL ? q->indices[j] : j;
    }
    *exitus = x;
    redde n;
}

interior b32
_k_permissus (
    constans Activatio* a,
     constans Quaestio* q,
                   i32  k)
{
    redde q->cellae_maximae == ZEPHYRUM
        || k * (a->numerus_axium + I) <= q->cellae_maximae;
}

/* victorem servare (alveus crescens per n) */
interior b32
_servare (
       Capacitas* c,
             i32* capacitates,
             i32  n,
    constans i32* pondera,
             i32  k,
             i32  limes,
         Piscina* piscina)
{
     Victor* v;
        i32  j;

    si (c->servati[n] >= limes)
    {
        redde VERUM;
    }
    si (c->servati[n] >= capacitates[n])
    {
        i32 nova = capacitates[n] == ZEPHYRUM ? (i32)XVI
            : capacitates[n] * II;
        Victor* alveus = (Victor*)piscina_allocare(piscina,
            (memoriae_index)nova * magnitudo(Victor));

        si (alveus == NIHIL)
        {
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < c->servati[n]; j++)
        {
            alveus[j] = c->victores[n][j];
        }
        c->victores[n] = alveus;
        capacitates[n] = nova;
    }
    v = &c->victores[n][c->servati[n]++];
    per (j = ZEPHYRUM; j < CELLAE_N_MAXIMUS; j++)
    {
        v->pondera[j] = j < n ? pondera[j] : ZEPHYRUM;
    }
    v->n = n;
    v->k = k;
    redde VERUM;
}

interior vacuum
_capacitas_initium (
    Capacitas* c)
{
    i32 n;

    per (n = ZEPHYRUM; n <= CELLAE_N_MAXIMUS; n++)
    {
        c->numerus[n]   = 0;
        c->servati[n]   = ZEPHYRUM;
        c->victores[n]  = NIHIL;
    }
    c->maximum = ZEPHYRUM;
}

interior vacuum
_capacitas_maximum (
    Capacitas* c)
{
    i32 n;

    c->maximum = ZEPHYRUM;
    per (n = III; n <= CELLAE_N_MAXIMUS; n++)
    {
        si (c->numerus[n] > 0)
        {
            c->maximum = II * n;
        }
    }
}

interior b32
_quaestio_valida (
    constans Activatio* a,
    constans Quaestio*  q)
{
    i32 j;

    si (   q->scala_k   == NIHIL || q->numerus_k == ZEPHYRUM
        || q->n_maximum == ZEPHYRUM || q->n_maximum > CELLAE_N_MAXIMUS
        || q->modus > CELLAE_FIXA(I, I))
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < q->numerus_k; j++)
    {
        si (q->scala_k[j] < I)
        {
            redde FALSUM;
        }
    }
    si (q->indices != NIHIL)
    {
        per (j = ZEPHYRUM; j < q->numerus_indicum; j++)
        {
            si (   q->indices[j] >= a->catalogus->numerus
                || (j > ZEPHYRUM && q->indices[j] <= q->indices[j - I]))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

interior b32
_exhaustiva_intra (
             Activatio* a,
     constans Quaestio* q,
               Piscina* piscina,
               Piscina* opus,
             Capacitas* c)
{
        i32* indices;
        i32  m = _permissi(a, q, opus, &indices);
     Larvae  coniunctio[CELLAE_N_MAXIMUS + I];
        i32  capacitates[CELLAE_N_MAXIMUS + I];
        i32  index_k;
        i32  n;

    si (m > LXIV)
    {
        redde FALSUM;
    }
    per (n = ZEPHYRUM; n <= CELLAE_N_MAXIMUS; n++)
    {
        capacitates[n] = ZEPHYRUM;
        si (!_larvae_initium(&coniunctio[n], (i64)64, opus))
        {
            redde FALSUM;
        }
    }
    per (index_k = ZEPHYRUM; index_k < q->numerus_k; index_k++)
    {
           i32  k = q->scala_k[index_k];
        Larvae  gradus[CELLAE_N_MAXIMUS + I];
           i64* lista[CELLAE_N_MAXIMUS + I];
           i64  numerus[CELLAE_N_MAXIMUS + I];
           i64  capacitas_listae[CELLAE_N_MAXIMUS + I];

        si (!_k_permissus(a, q, k))
        {
            perge;
        }
        per (n = I; n <= q->n_maximum; n++)
        {
            numerus[n]           = 0;
            capacitas_listae[n]  = 64;
            lista[n] = (i64*)piscina_allocare(opus, (memoriae_index)64
                * magnitudo(i64));
            si (   lista[n] == NIHIL
                || !_larvae_initium(&gradus[n], (i64)64,
                opus))
            {
                redde FALSUM;
            }
        }
        per (n = I; n <= q->n_maximum; n++)
        {
            i64 b;
            i64 fontes = n == I ? (i64)m : numerus[n - I];

            per (b = 0; b < fontes; b++)
            {
                i64 basis   = n == I ? 0 : lista[n - I][b];
                s32 summus  = -(s32)I;
                i32 j;

                si (n > I)
                {
                    per (j = ZEPHYRUM; j < m; j++)
                    {
                        si (basis & ((i64)1 << j))
                        {
                            summus = (s32)j;
                        }
                    }
                }
                per (j = (n == I ? (i32)b : (i32)(summus + I)); j
                    < m; j++)
                {
                    i64 candidatus  = basis | ((i64)1 << j);
                    b32 omnes       = VERUM;
                    b32 verdictum   = FALSUM;
                    i32 pondera[CELLAE_N_MAXIMUS];
                    i32 np = ZEPHYRUM;
                    i32 t;

                    si (n > I)
                    {
                        per (t = ZEPHYRUM; t < m && omnes; t++)
                        {
                            si (   (basis & ((i64)1 << t))
                                && !_larvae_continent(&gradus[n - I],
                                candidatus & ~((i64)1 << t)))
                            {
                                omnes = FALSUM;
                            }
                        }
                    }
                    si (!omnes)
                    {
                        perge;
                    }
                    per (t = ZEPHYRUM; t < m; t++)
                    {
                        si (candidatus & ((i64)1 << t))
                        {
                            pondera[np++] = indices[t];
                        }
                    }
                    si (!activatio_separat(a, pondera, n, k, q->modus,
                        &verdictum))
                    {
                        redde FALSUM;
                    }
                    si (!verdictum)
                    {
                        perge;
                    }
                    si (numerus[n] >= capacitas_listae[n])
                    {
                        i64* nova = (i64*)piscina_allocare(opus,
                            (memoriae_index)(2 * capacitas_listae[n])
                            * magnitudo(i64));
                        i64 u;

                        si (nova == NIHIL)
                        {
                            redde FALSUM;
                        }
                        per (u = 0; u < numerus[n]; u++)
                        {
                            nova[u] = lista[n][u];
                        }
                        lista[n]             = nova;
                        capacitas_listae[n]  *= 2;
                    }
                    lista[n][numerus[n]++] = candidatus;
                    (vacuum)_larvae_adde(&gradus[n], candidatus, opus);
                    si (_larvae_adde(&coniunctio[n], candidatus, opus))
                    {
                        c->numerus[n]++;
                        si (!_servare(c, capacitates, n, pondera, k,
                            q->limes_servandi, piscina))
                        {
                            redde FALSUM;
                        }
                    }
                    si (n == I)
                    {
                        frange;        /* singulum: j = b solum */
                    }
                }
            }
        }
    }
    _capacitas_maximum(c);
    redde VERUM;
}

b32
activatio_exhaustiva (
             Activatio* activatio,
     constans Quaestio* quaestio,
               Piscina* piscina,
             Capacitas* exitus)
{
    Piscina* opus;
  Capacitas  c;
        b32  bene;

    si (!_quaestio_valida(activatio, quaestio))
    {
        redde FALSUM;
    }
    opus = piscina_generare_dynamicum("cellae_quaestio",
        (memoriae_index)65536);
    si (opus == NIHIL)
    {
        redde FALSUM;
    }
    _capacitas_initium(&c);
    bene = _exhaustiva_intra(activatio, quaestio, piscina, opus, &c);
    piscina_destruere(opus);
    si (bene)
    {
        *exitus = c;
    }
    redde bene;
}

/* primus k scalae qui transit; *k 0 si nullus */
interior b32
_primus_k (
             Activatio* a,
     constans Quaestio* q,
          constans i32* pondera,
                   i32  n,
                   i32* k)
{
    i32 index_k;

    *k = ZEPHYRUM;
    per (index_k = ZEPHYRUM; index_k < q->numerus_k; index_k++)
    {
        b32 verdictum = FALSUM;

        si (!_k_permissus(a, q, q->scala_k[index_k]))
        {
            perge;
        }
        si (!activatio_separat(a, pondera, n, q->scala_k[index_k],
            q->modus,
            &verdictum))
        {
            redde FALSUM;
        }
        si (verdictum)
        {
            *k = q->scala_k[index_k];
            redde VERUM;
        }
    }
    redde VERUM;
}

interior b32
_avida_intra (
             Activatio* a,
     constans Quaestio* q,
               Piscina* piscina,
               Piscina* opus,
             Capacitas* c)
{
     i32* indices;
     i32  m = _permissi(a, q, opus, &indices);
     i32  limes = q->limes_triplicum == ZEPHYRUM
         || q->limes_triplicum > m
         ? m : q->limes_triplicum;
     i32 maximi = q->victores_maximi == ZEPHYRUM ? (i32)0x7fffffff
         : q->victores_maximi;
     i32 capacitates[CELLAE_N_MAXIMUS + I];
    /* victores omnes (positiones in indicibus), pro extensione */
     i32* positiones[CELLAE_N_MAXIMUS + I];
     i32  inventi[CELLAE_N_MAXIMUS + I];
     i32  n;
     i32  x;
     i32  y;
     i32  z;

    per (n = ZEPHYRUM; n <= CELLAE_N_MAXIMUS; n++)
    {
        capacitates[n]  = ZEPHYRUM;
        inventi[n]      = ZEPHYRUM;
        positiones[n]   = NIHIL;
    }
    si (q->n_maximum < III)
    {
        redde FALSUM;
    }
    /* positiones: alveus (maximi vel quot possibile) per n */
    per (n = III; n <= q->n_maximum; n++)
    {
        i32 spatium = maximi < (i32)4096 ? maximi : (i32)4096;

        positiones[n] = (i32*)piscina_allocare(opus, (memoriae_index)
            spatium * (memoriae_index)n * magnitudo(i32));
        si (positiones[n] == NIHIL)
        {
            redde FALSUM;
        }
    }
    per (x = ZEPHYRUM; x < limes
        && inventi[III] < maximi; x++) per (y = x + I; y < limes
                                           && inventi[III]
                                               < maximi; y++) per (z = y
                                                                  + I; z
                                                                  < limes
                                                                  && inventi[III]
                                                                      < maximi; z++)
                                                              {
        i32 pondera[III];
        i32 k = ZEPHYRUM;

        pondera[0] = indices[x];
        pondera[1] = indices[y];
        pondera[2] = indices[z];
        si (!_primus_k(a, q, pondera, III, &k))
        {
            redde FALSUM;
        }
        si (k > ZEPHYRUM)
        {
            si (inventi[III] >= (i32)4096)
            {
                redde FALSUM;
            }
            positiones[III][inventi[III] * III + 0] = x;
            positiones[III][inventi[III] * III + 1] = y;
            positiones[III][inventi[III] * III + 2] = z;
            inventi[III]++;
            c->numerus[III]++;
            si (!_servare(c, capacitates, III, pondera, k,
                q->limes_servandi, piscina))
            {
                redde FALSUM;
            }
        }
                                                              }
    per (n = IV; n <= q->n_maximum; n++)
    {
        i32 prior = n - I;
        i32 w;

        si (inventi[prior] == ZEPHYRUM)
        {
            frange;
        }
        per (w = ZEPHYRUM; w < inventi[prior]
            && inventi[n] < maximi; w++)
        {
            constans i32* basis = &positiones[prior][w * prior];
                     i32  e;

            per (e = ZEPHYRUM; e < m && inventi[n] < maximi; e++)
            {
                i32 nova[CELLAE_N_MAXIMUS];
                i32 pondera[CELLAE_N_MAXIMUS];
                i32 locus = prior;
                i32 t;
                i32 u;
                b32 inest            = FALSUM;
                b32 duplex_inventum  = FALSUM;
                i32 k                = ZEPHYRUM;

                per (t = ZEPHYRUM; t < prior; t++)
                {
                    si (basis[t] == e) inest = VERUM;
                }
                si (inest)
                {
                    perge;
                }
                per (t = ZEPHYRUM; t < prior; t++)
                {
                    si (e < basis[t])
                    {
                        locus = t;
                        frange;
                    }
                }
                per (t = ZEPHYRUM; t < locus; t++) nova[t] = basis[t];
                nova[locus] = e;
                per (t = locus; t < prior; t++) nova[t + I] = basis[t];
                per (u = ZEPHYRUM; u < inventi[n]
                    && !duplex_inventum; u++)
                {
                    b32 par = VERUM;

                    per (t = ZEPHYRUM; t < n && par; t++)
                    {
                        par = positiones[n][u * n + t] == nova[t];
                    }
                    duplex_inventum = par;
                }
                si (duplex_inventum)
                {
                    perge;
                }
                per (t = ZEPHYRUM; t < n; t++)
                {
                    pondera[t] = indices[nova[t]];
                }
                si (!_primus_k(a, q, pondera, n, &k))
                {
                    redde FALSUM;
                }
                si (k > ZEPHYRUM)
                {
                    si (inventi[n] >= (i32)4096)
                    {
                        redde FALSUM;
                    }
                    per (t = ZEPHYRUM; t < n; t++)
                    {
                        positiones[n][inventi[n] * n + t] = nova[t];
                    }
                    inventi[n]++;
                    c->numerus[n]++;
                    si (!_servare(c, capacitates, n, pondera, k,
                        q->limes_servandi, piscina))
                    {
                        redde FALSUM;
                    }
                }
            }
        }
    }
    _capacitas_maximum(c);
    redde VERUM;
}

b32
activatio_avida (
             Activatio* activatio,
     constans Quaestio* quaestio,
               Piscina* piscina,
             Capacitas* exitus)
{
    Piscina* opus;
  Capacitas  c;
        b32  bene;

    si (!_quaestio_valida(activatio, quaestio))
    {
        redde FALSUM;
    }
    opus = piscina_generare_dynamicum("cellae_quaestio",
        (memoriae_index)65536);
    si (opus == NIHIL)
    {
        redde FALSUM;
    }
    _capacitas_initium(&c);
    bene = _avida_intra(activatio, quaestio, piscina, opus, &c);
    piscina_destruere(opus);
    si (bene)
    {
        *exitus = c;
    }
    redde bene;
}
