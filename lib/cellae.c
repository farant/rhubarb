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
