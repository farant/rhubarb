/* partitio.c - algebra partitionum (T35 a; vide partitio.h) */
#include "partitio.h"
#include "tabula_dispersa.h"
#include <string.h>


/* ==================================================
 * Adiutores interni
 * ================================================== */

interior memoriae_index
_longitudo_tabulae (
               i32 numerus,
    memoriae_index magnitudo_elementi)
{
    redde (memoriae_index)(numerus > ZEPHYRUM ? numerus : I)
        * magnitudo_elementi;
}

interior Partitio*
_partitio_creare (
     Piscina* piscina,
         i32  numerus)
{
          Partitio* p;
    memoriae_index  longitudo;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    longitudo = _longitudo_tabulae(numerus, magnitudo(i32));
    p = (Partitio*)piscina_allocare(piscina, magnitudo(Partitio));
    si (p == NIHIL)
    {
        redde NIHIL;
    }
    p->numerus         = numerus;
    p->numerus_gregum  = ZEPHYRUM;
    p->grex            = (i32*)piscina_allocare(piscina, longitudo);
    p->magnitudines    = (i32*)piscina_allocare(piscina, longitudo);
    p->primus          = (i32*)piscina_allocare(piscina, longitudo);
    si (   p->grex   == NIHIL || p->magnitudines == NIHIL
        || p->primus == NIHIL)
    {
        redde NIHIL;
    }
    memset(p->magnitudines, ZEPHYRUM, longitudo);
    redde p;
}

/* CANONICATOR UNUS: claves[i] = octeti ordinis i; grex = ordo primae
 * apparitionis clavis. Omnes constructores et infimum per eum eunt. */
interior Partitio*
_partitio_ex_clavibus (
              Piscina* piscina,
                  i32  numerus,
      constans chorda* claves)
{
           Partitio* p;
     TabulaDispersa* tabula;
                i32* identitates;
                i32  i;

    p = _partitio_creare(piscina, numerus);
    si (p == NIHIL)
    {
        redde NIHIL;
    }
    tabula = tabula_dispersa_creare_chorda(piscina,
        numerus > XVI ? numerus * II : XXXII);
    identitates = (i32*)piscina_allocare(piscina,
        _longitudo_tabulae(numerus, magnitudo(i32)));
    si (tabula == NIHIL || identitates == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        vacuum* valor;

        si (tabula_dispersa_invenire(tabula, claves[i], &valor))
        {
            p->grex[i] = *(i32*)valor;
        }
        alioquin
        {
            identitates[p->numerus_gregum] = p->numerus_gregum;
            si (!tabula_dispersa_inserere(tabula, claves[i],
                &identitates[p->numerus_gregum]))
            {
                redde NIHIL;
            }
            p->primus[p->numerus_gregum]  = i;
            p->grex[i]                    = p->numerus_gregum;
            p->numerus_gregum             = p->numerus_gregum + I;
        }
        p->magnitudines[p->grex[i]] = p->magnitudines[p->grex[i]] + I;
    }
    redde p;
}

/* claves ex octetis: octeti[i * latitudo] ordinis i (memcpy - nulla
 * dependentia ab alignatione piscinae) */
interior chorda*
_claves_ex_octetis (
     Piscina* piscina,
         i32  numerus,
          i8* octeti,
         i32  latitudo)
{
    chorda* claves;
       i32  i;

    claves = (chorda*)piscina_allocare(piscina,
        _longitudo_tabulae(numerus, magnitudo(chorda)));
    si (claves == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        claves[i].mensura = latitudo;
        claves[i].datum   = octeti + (memoriae_index)i
            * (memoriae_index)latitudo;
    }
    redde claves;
}

interior i32
_radix (
     i32* parentes,
     i32  i)
{
    dum (parentes[i] != i)
    {
        parentes[i]  = parentes[parentes[i]];
        i            = parentes[i];
    }
    redde i;
}

interior vacuum
_unire (
     i32* parentes,
     i32  a,
     i32  b)
{
    i32 radix_a;
    i32 radix_b;

    radix_a = _radix(parentes, a);
    radix_b = _radix(parentes, b);
    si (radix_a < radix_b)
    {
        parentes[radix_b] = radix_a;
    }
    alioquin si (radix_b < radix_a)
    {
        parentes[radix_a] = radix_b;
    }
}

/* Kuhn: via augens ab unica i per ordinem strictum */
interior b32
_augere (
     constans b32* stricte,
              i32  k,
              i32* socius_dexter,
              b32* visus,
              i32  i)
{
    i32 j;

    per (j = ZEPHYRUM; j < k; j++)
    {
        si (!stricte[i * k + j] || visus[j])
        {
            perge;
        }
        visus[j] = VERUM;
        si (   socius_dexter[j] == k
            || _augere(stricte, k, socius_dexter, visus,
            socius_dexter[j]))
        {
            socius_dexter[j] = i;
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Constructores
 * ================================================== */

Partitio*
partitio_ex_notis (
          Piscina* piscina,
              i32  numerus,
     constans i32* notae)
{
        i8* octeti;
    chorda* claves;
       i32  i;

    si (piscina == NIHIL || (notae == NIHIL && numerus > ZEPHYRUM))
    {
        redde NIHIL;
    }
    octeti = (i8*)piscina_allocare(piscina,
        _longitudo_tabulae(numerus, magnitudo(i32)));
    si (octeti == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        memcpy(octeti + (memoriae_index)i * magnitudo(i32), &notae[i],
            magnitudo(i32));
    }
    claves = _claves_ex_octetis(piscina, numerus, octeti,
        (i32)magnitudo(i32));
    si (claves == NIHIL)
    {
        redde NIHIL;
    }
    redde _partitio_ex_clavibus(piscina, numerus, claves);
}

Partitio*
partitio_ex_chordis (
             Piscina* piscina,
                 i32  numerus,
     constans chorda* valores)
{
    si (piscina == NIHIL || (valores == NIHIL && numerus > ZEPHYRUM))
    {
        redde NIHIL;
    }
    redde _partitio_ex_clavibus(piscina, numerus, valores);
}

Partitio*
partitio_una (
     Piscina* piscina,
         i32  numerus)
{
    Partitio* p;
         i32  i;

    p = _partitio_creare(piscina, numerus);
    si (p == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        p->grex[i] = ZEPHYRUM;
    }
    si (numerus > ZEPHYRUM)
    {
        p->numerus_gregum          = I;
        p->primus[ZEPHYRUM]        = ZEPHYRUM;
        p->magnitudines[ZEPHYRUM]  = numerus;
    }
    redde p;
}

Partitio*
partitio_discreta (
     Piscina* piscina,
         i32  numerus)
{
    Partitio* p;
         i32  i;

    p = _partitio_creare(piscina, numerus);
    si (p == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        p->grex[i]          = i;
        p->primus[i]        = i;
        p->magnitudines[i]  = I;
    }
    p->numerus_gregum = numerus;
    redde p;
}


/* ==================================================
 * Ordo et operationes
 * ================================================== */

b32
partitio_subtilior (
    constans Partitio* a,
    constans Partitio* b)
{
    i32 i;

    si (a == NIHIL || b == NIHIL || a->numerus != b->numerus)
    {
        redde FALSUM;
    }
    /* ordo quisque gregem b repraesentantis sui in a habet */
    per (i = ZEPHYRUM; i < a->numerus; i++)
    {
        si (b->grex[i] != b->grex[a->primus[a->grex[i]]])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
partitio_aequalis (
    constans Partitio* a,
    constans Partitio* b)
{
    si (   a                 == NIHIL || b == NIHIL
        || a->numerus        != b->numerus
        || a->numerus_gregum != b->numerus_gregum)
    {
        redde FALSUM;
    }
    si (a->numerus == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (b32)(memcmp(a->grex, b->grex,
        (memoriae_index)a->numerus * magnitudo(i32)) == ZEPHYRUM);
}

Partitio*
partitio_infimum (
              Piscina* piscina,
    constans Partitio* a,
    constans Partitio* b)
{
        i8* octeti;
    chorda* claves;
       i32  i;

    si (   piscina    == NIHIL || a == NIHIL || b == NIHIL
        || a->numerus != b->numerus)
    {
        redde NIHIL;
    }
    octeti = (i8*)piscina_allocare(piscina,
        _longitudo_tabulae(a->numerus, magnitudo(i64)));
    si (octeti == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < a->numerus; i++)
    {
        i64 clavis;

        clavis = (i64)a->grex[i] * (i64)b->numerus_gregum
            + (i64)b->grex[i];
        memcpy(octeti + (memoriae_index)i * magnitudo(i64), &clavis,
            magnitudo(i64));
    }
    claves = _claves_ex_octetis(piscina, a->numerus, octeti,
        (i32)magnitudo(i64));
    si (claves == NIHIL)
    {
        redde NIHIL;
    }
    redde _partitio_ex_clavibus(piscina, a->numerus, claves);
}

Partitio*
partitio_supremum (
              Piscina* piscina,
    constans Partitio* a,
    constans Partitio* b)
{
    i32* parentes;
    i32* notae;
    i32  i;

    si (   piscina    == NIHIL || a == NIHIL || b == NIHIL
        || a->numerus != b->numerus)
    {
        redde NIHIL;
    }
    parentes = (i32*)piscina_allocare(piscina,
        _longitudo_tabulae(a->numerus, magnitudo(i32)));
    notae    = (i32*)piscina_allocare(piscina,
        _longitudo_tabulae(a->numerus, magnitudo(i32)));
    si (parentes == NIHIL || notae == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < a->numerus; i++)
    {
        parentes[i] = i;
    }
    /* ordo quisque cum repraesentantibus gregum suorum in a et b */
    per (i = ZEPHYRUM; i < a->numerus; i++)
    {
        _unire(parentes, i, a->primus[a->grex[i]]);
        _unire(parentes, i, b->primus[b->grex[i]]);
    }
    per (i = ZEPHYRUM; i < a->numerus; i++)
    {
        notae[i] = _radix(parentes, i);
    }
    redde partitio_ex_notis(piscina, a->numerus, notae);
}


/* ==================================================
 * Reticulum
 * ================================================== */

PartitioReticulum*
partitio_reticulum_struere (
                      Piscina* piscina,
    constans Partitio* constans* partes,
                           i32 k)
{
    PartitioReticulum* r;
                  b32* stricte;
                  i32* altitudines;
                  i32* socius_dexter;
                  b32* visus;
       memoriae_index  quadratum;
                  i32  i;
                  i32  j;
                  i32  m;
                  i32  gradus;
                  i32  unicae;
                  i32  congruentia;

    si (piscina == NIHIL || (partes == NIHIL && k > ZEPHYRUM))
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < k; i++)
    {
        si (   partes[i]          == NIHIL
            || partes[i]->numerus != partes[ZEPHYRUM]->numerus)
        {
            redde NIHIL;
        }
    }
    quadratum = _longitudo_tabulae(k, magnitudo(b32))
        * (memoriae_index)(k > ZEPHYRUM ? k : I);
    r = (PartitioReticulum*)piscina_allocare(piscina,
        magnitudo(PartitioReticulum));
    si (r == NIHIL)
    {
        redde NIHIL;
    }
    r->numerus  = k;
    r->ordo     = (b32*)piscina_allocare(piscina, quadratum);
    r->tegit    = (b32*)piscina_allocare(piscina, quadratum);
    r->geminus     = (i32*)piscina_allocare(piscina,
        _longitudo_tabulae(k, magnitudo(i32)));
    stricte       = (b32*)piscina_allocare(piscina, quadratum);
    altitudines   = (i32*)piscina_allocare(piscina,
        _longitudo_tabulae(k, magnitudo(i32)));
    socius_dexter = (i32*)piscina_allocare(piscina,
        _longitudo_tabulae(k, magnitudo(i32)));
    visus         = (b32*)piscina_allocare(piscina,
        _longitudo_tabulae(k, magnitudo(b32)));
    si (   r->ordo == NIHIL || r->tegit == NIHIL || r->geminus == NIHIL
        || stricte == NIHIL || altitudines == NIHIL
        || socius_dexter == NIHIL || visus == NIHIL)
    {
        redde NIHIL;
    }
    memset(r->ordo, ZEPHYRUM, quadratum);
    memset(r->tegit, ZEPHYRUM, quadratum);
    memset(stricte, ZEPHYRUM, quadratum);

    /* I. ordo subtilitatis */
    per (i = ZEPHYRUM; i < k; i++)
    {
        per (j = ZEPHYRUM; j < k; j++)
        {
            r->ordo[i * k + j] = (b32)(i != j
                && partitio_subtilior(partes[i], partes[j]));
        }
    }

    /* II. duplices: subtilior utrimque = aequalis (antisymmetria) */
    per (i = ZEPHYRUM; i < k; i++)
    {
        r->geminus[i] = i;
        per (j = ZEPHYRUM; j < i; j++)
        {
            si (r->ordo[i * k + j] && r->ordo[j * k + i])
            {
                r->geminus[i] = j;
                frange;
            }
        }
    }

    /* III. ordo strictus inter unicas */
    unicae = ZEPHYRUM;
    per (i = ZEPHYRUM; i < k; i++)
    {
        si (r->geminus[i] == i)
        {
            unicae = unicae + I;
        }
        per (j = ZEPHYRUM; j < k; j++)
        {
            stricte[i * k + j] = (b32)(   r->geminus[i] == i
                                       && r->geminus[j] == j
                                       && r->ordo[i * k + j]
                                       && !r->ordo[j * k + i]);
        }
    }

    /* IV. tegmina: strictum sine medio */
    per (i = ZEPHYRUM; i < k; i++)
    {
        per (j = ZEPHYRUM; j < k; j++)
        {
            b32 medium = FALSUM;

            si (!stricte[i * k + j])
            {
                perge;
            }
            per (m = ZEPHYRUM; m < k; m++)
            {
                si (stricte[i * k + m] && stricte[m * k + j])
                {
                    medium = VERUM;
                    frange;
                }
            }
            r->tegit[i * k + j] = (b32)!medium;
        }
    }

    /* V. altitudo: relaxatio k vicibus */
    per (i = ZEPHYRUM; i < k; i++)
    {
        altitudines[i] = ZEPHYRUM;
    }
    per (gradus = ZEPHYRUM; gradus < k; gradus++)
    {
        per (i = ZEPHYRUM; i < k; i++)
        {
            per (j = ZEPHYRUM; j < k; j++)
            {
                si (   stricte[i * k + j]
                    && altitudines[j] < altitudines[i] + I)
                {
                    altitudines[j] = altitudines[i] + I;
                }
            }
        }
    }
    r->altitudo = ZEPHYRUM;
    per (i = ZEPHYRUM; i < k; i++)
    {
        si (altitudines[i] > r->altitudo)
        {
            r->altitudo = altitudines[i];
        }
    }

    /* VI. latitudo per Dilworth: unicae - congruentia maxima */
    per (j = ZEPHYRUM; j < k; j++)
    {
        socius_dexter[j] = k;
    }
    congruentia = ZEPHYRUM;
    per (i = ZEPHYRUM; i < k; i++)
    {
        si (r->geminus[i] != i)
        {
            perge;
        }
        memset(visus, ZEPHYRUM, _longitudo_tabulae(k, magnitudo(b32)));
        si (_augere(stricte, k, socius_dexter, visus, i))
        {
            congruentia = congruentia + I;
        }
    }
    r->latitudo = unicae - congruentia;
    redde r;
}
