/* quaternio.c - Quaterniones super quemlibet anulum (vide
 * include/quaternio.h)
 */
#include "quaternio.h"
#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior i8*
_alveus (
     constans Anulus* anulus,
                 i32  numerus,
             Piscina* piscina)
{
    redde (i8*)piscina_allocare(piscina, (memoriae_index)numerus
        * anulus->mensura);
}

interior i8*
_pars (
    Quaternio q,
          i32 index)
{
    redde q.partes + (memoriae_index)index * q.anulus->mensura;
}

/* anulus communis; NIHIL si mixti aut nulli */
interior constans Anulus*
_communis (
    Quaternio p,
    Quaternio q)
{
    redde (p.anulus != NIHIL
        && p.anulus == q.anulus) ? p.anulus : NIHIL;
}

/* exitus = sum_k signa[k] x[k] y[k] (IV termini) */
interior b32
_summa (
    constans Anulus*       anulus,
          constans s32*    signa,
    constans vacuum* constans* x,
    constans vacuum* constans* y,
             Piscina*       piscina,
              vacuum*       exitus)
{
     i8* summa      = _alveus(anulus, I, piscina);
     i8* altera     = _alveus(anulus, I, piscina);
     i8* productum  = _alveus(anulus, I, piscina);
    i32  k;

    anulus->nullum(anulus, summa);
    /* summa et altera alternant: nullus alveus novus per terminum */
    per (k = ZEPHYRUM; k < IV; k++)
    {
        i8* commutatum;

        si (!anulus->multiplica(anulus, x[k], y[k], piscina, productum))
        {
            redde FALSUM;
        }
        si (signa[k] > ZEPHYRUM)
        {
            si (!anulus->adde(anulus, summa, productum, piscina,
                altera))
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            si (!anulus->subtrahe(anulus, summa, productum, piscina,
                altera))
            {
                redde FALSUM;
            }
        }
        commutatum  = summa;
        summa       = altera;
        altera      = commutatum;
    }
    memcpy(exitus, summa, (size_t)anulus->mensura);
    redde VERUM;
}

/* x * y in alveum novum */
interior i8*
_productum (
    constans Anulus* anulus,
    constans vacuum* x,
    constans vacuum* y,
            Piscina* piscina)
{
    i8* exitus = _alveus(anulus, I, piscina);

    si (!anulus->multiplica(anulus, x, y, piscina, exitus))
    {
        redde NIHIL;
    }
    redde exitus;
}

/* x + y (signum > 0) aut x - y in alveum novum */
interior i8*
_summa_duorum (
     constans Anulus* anulus,
     constans vacuum* x,
     constans vacuum* y,
                 s32  signum,
             Piscina* piscina)
{
    i8* exitus = _alveus(anulus, I, piscina);

    si (x == NIHIL || y == NIHIL)
    {
        redde NIHIL;
    }
    si (signum > ZEPHYRUM ? !anulus->adde(anulus, x, y, piscina, exitus)
        : !anulus->subtrahe(anulus, x, y, piscina, exitus))
    {
        redde NIHIL;
    }
    redde exitus;
}

/* signum per hamum anuli; FALSUM si NIHIL aut refutatum */
interior b32
_signum (
    constans Anulus* anulus,
    constans vacuum* x,
            Piscina* piscina,
                s32* exitus)
{
    redde anulus->signum != NIHIL && x != NIHIL
        && anulus->signum(anulus, x, piscina, exitus);
}


/* ==================================================
 * Creatio et lectio
 * ================================================== */

b32
quaternio_ex_partibus (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
    constans vacuum* c,
    constans vacuum* d,
            Piscina* piscina,
          Quaternio* exitus)
{
     constans vacuum* partes[IV];
                 i32  k;

    si (anulus == NIHIL)
    {
        redde FALSUM;
    }
    partes[ZEPHYRUM]  = a;
    partes[I]         = b;
    partes[II]        = c;
    partes[III]       = d;
    exitus->anulus    = anulus;
    exitus->partes    = _alveus(anulus, IV, piscina);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        anulus->transcribe(anulus, partes[k], piscina, _pars(*exitus,
            k));
    }
    redde VERUM;
}

b32
quaternio_nullum (
    constans Anulus* anulus,
            Piscina* piscina,
          Quaternio* exitus)
{
    i32 k;

    si (anulus == NIHIL)
    {
        redde FALSUM;
    }
    exitus->anulus = anulus;
    exitus->partes = _alveus(anulus, IV, piscina);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        anulus->nullum(anulus, _pars(*exitus, k));
    }
    redde VERUM;
}

b32
quaternio_unum (
    constans Anulus* anulus,
            Piscina* piscina,
          Quaternio* exitus)
{
    si (!quaternio_nullum(anulus, piscina, exitus))
    {
        redde FALSUM;
    }
    anulus->unum(anulus, piscina, _pars(*exitus, ZEPHYRUM));
    redde VERUM;
}

constans vacuum*
quaternio_pars (
    Quaternio q,
          i32 index)
{
    si (q.anulus == NIHIL || index > III)
    {
        redde NIHIL;
    }
    redde _pars(q, index);
}

constans Anulus*
quaternio_anulus (
    Quaternio q)
{
    redde q.anulus;
}


/* ==================================================
 * Arithmetica
 * ================================================== */

interior b32
_adde_subtrahe (
    Quaternio  p,
    Quaternio  q,
          s32  signum,
      Piscina* piscina,
    Quaternio* exitus)
{
           Quaternio  effectus;
     constans Anulus* anulus = _communis(p, q);
                 i32  k;

    si (anulus == NIHIL)
    {
        redde FALSUM;
    }
    effectus.anulus = anulus;
    effectus.partes = _alveus(anulus, IV, piscina);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        si (signum > ZEPHYRUM ? !anulus->adde(anulus, _pars(p, k),
            _pars(q,
            k), piscina, _pars(effectus, k)) : !anulus->subtrahe(anulus,
            _pars(p, k), _pars(q, k), piscina, _pars(effectus, k)))
        {
            redde FALSUM;
        }
    }
    *exitus = effectus;
    redde VERUM;
}

b32
quaternio_adde (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina,
    Quaternio* exitus)
{
    redde _adde_subtrahe(p, q, I, piscina, exitus);
}

b32
quaternio_subtrahe (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina,
    Quaternio* exitus)
{
    redde _adde_subtrahe(p, q, -I, piscina, exitus);
}

b32
quaternio_multiplica (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina,
    Quaternio* exitus)
{
    Quaternio effectus;
    /* a = a1a2 - b1b2 - c1c2 - d1d2
     * b = a1b2 + b1a2 + c1d2 - d1c2
     * c = a1c2 - b1d2 + c1a2 + d1b2
     * d = a1d2 + b1c2 - c1b2 + d1a2 */
    hic_manens constans s32 signa[IV][IV] = {
        { I, -I, -I, -I }, { I, I, I, -I }, { I, -I, I, I },
        { I, I, -I, I } };
    hic_manens constans i32 ordo[IV][IV] = {
        { ZEPHYRUM, I, II, III }, { I, ZEPHYRUM, III, II },
        { II, III, ZEPHYRUM, I }, { III, II, I, ZEPHYRUM } };
     constans Anulus* anulus = _communis(p, q);
     constans vacuum* x[IV];
     constans vacuum* y[IV];
                 i32  k;
                 i32  m;

    si (anulus == NIHIL)
    {
        redde FALSUM;
    }
    effectus.anulus = anulus;
    effectus.partes = _alveus(anulus, IV, piscina);
    per (m = ZEPHYRUM; m < IV; m++)
    {
        x[m] = _pars(p, m);
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        per (m = ZEPHYRUM; m < IV; m++)
        {
            y[m] = _pars(q, (i32)ordo[k][m]);
        }
        si (!_summa(anulus, signa[k], x, y, piscina, _pars(effectus,
            k)))
        {
            redde FALSUM;
        }
    }
    *exitus = effectus;
    redde VERUM;
}

b32
quaternio_scalari (
           Quaternio  q,
     constans vacuum* s,
             Piscina* piscina,
           Quaternio* exitus)
{
    Quaternio effectus;
          i32 k;

    si (q.anulus == NIHIL)
    {
        redde FALSUM;
    }
    effectus.anulus = q.anulus;
    effectus.partes = _alveus(q.anulus, IV, piscina);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        si (!q.anulus->multiplica(q.anulus, s, _pars(q, k), piscina,
            _pars(effectus, k)))
        {
            redde FALSUM;
        }
    }
    *exitus = effectus;
    redde VERUM;
}

b32
quaternio_conjugatum (
    Quaternio  q,
      Piscina* piscina,
    Quaternio* exitus)
{
    Quaternio  effectus;
           i8* nullum;
          i32  k;

    si (q.anulus == NIHIL)
    {
        redde FALSUM;
    }
    nullum = _alveus(q.anulus, I, piscina);
    q.anulus->nullum(q.anulus, nullum);
    effectus.anulus = q.anulus;
    effectus.partes = _alveus(q.anulus, IV, piscina);
    q.anulus->transcribe(q.anulus, _pars(q, ZEPHYRUM), piscina, _pars(
        effectus, ZEPHYRUM));
    per (k = I; k < IV; k++)
    {
        si (!q.anulus->subtrahe(q.anulus, nullum, _pars(q, k), piscina,
            _pars(effectus, k)))
        {
            redde FALSUM;
        }
    }
    *exitus = effectus;
    redde VERUM;
}

b32
quaternio_norma (
    Quaternio  q,
      Piscina* piscina,
       vacuum* exitus)
{
    hic_manens constans s32  signa[IV] = { I, I, I, I };
            constans vacuum* x[IV];
                        i32  k;

    si (q.anulus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        x[k] = _pars(q, k);
    }
    redde _summa(q.anulus, signa, x, x, piscina, exitus);
}

b32
quaternio_inversum (
    Quaternio  q,
      Piscina* piscina,
    Quaternio* exitus)
{
    Quaternio  effectus;
           i8* norma;
    Quaternio  conjugatum;
          i32  k;

    si (q.anulus == NIHIL || !q.anulus->corpus)
    {
        redde FALSUM;
    }
    norma = _alveus(q.anulus, I, piscina);
    si (   !quaternio_norma(q, piscina, norma)
        || q.anulus->est_nullum(q.anulus, norma)
        || !quaternio_conjugatum(q, piscina, &conjugatum))
    {
        redde FALSUM;
    }
    effectus.anulus = q.anulus;
    effectus.partes = _alveus(q.anulus, IV, piscina);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        si (!q.anulus->divide_exacte(q.anulus, _pars(conjugatum, k),
            norma,
            piscina, _pars(effectus, k)))
        {
            redde FALSUM;
        }
    }
    *exitus = effectus;
    redde VERUM;
}

b32
quaternio_aequalis (
    Quaternio p,
    Quaternio q)
{
     constans Anulus* anulus = _communis(p, q);
                 i32  k;

    si (anulus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        si (!anulus->aequalis(anulus, _pars(p, k), _pars(q, k)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
quaternio_est_nullum (
    Quaternio q)
{
    i32 k;

    si (q.anulus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        si (!q.anulus->est_nullum(q.anulus, _pars(q, k)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Rotationes
 * ================================================== */

b32
quaternio_rotare (
    Quaternio  q,
    Quaternio  v,
      Piscina* piscina,
    Quaternio* exitus)
{
    Quaternio productum;
    Quaternio conjugatum;

    redde quaternio_multiplica(q, v, piscina, &productum)
        && quaternio_conjugatum(q, piscina, &conjugatum)
        && quaternio_multiplica(productum, conjugatum, piscina, exitus);
}

b32
quaternio_matrix (
    Quaternio  q,
      Piscina* piscina,
       Matrix* exitus)
{
    /* N R = [[aa+bb-cc-dd, 2(bc-ad), 2(bd+ac)],
     *        [2(bc+ad), aa-bb+cc-dd, 2(cd-ab)],
     *        [2(bd-ac), 2(cd+ab), aa-bb-cc+dd]] */
     constans Anulus* r = q.anulus;
              Matrix  effectus;
                  i8* aa;
                  i8* bb;
                  i8* cc;
                  i8* dd;
                  i8* elementa[IX];
                 i32  k;

    si (r == NIHIL || !matrix_nulla(r, III, III, piscina, &effectus))
    {
        redde FALSUM;
    }
    aa = _productum(r, _pars(q, ZEPHYRUM), _pars(q, ZEPHYRUM), piscina);
    bb = _productum(r, _pars(q, I), _pars(q, I), piscina);
    cc = _productum(r, _pars(q, II), _pars(q, II), piscina);
    dd = _productum(r, _pars(q, III), _pars(q, III), piscina);
    {
        i8* bc = _productum(r, _pars(q, I), _pars(q, II), piscina);
        i8* ad = _productum(r, _pars(q, ZEPHYRUM), _pars(q, III),
            piscina);
        i8* bd = _productum(r, _pars(q, I), _pars(q, III), piscina);
        i8* ac = _productum(r, _pars(q, ZEPHYRUM), _pars(q, II),
            piscina);
        i8* cd = _productum(r, _pars(q, II), _pars(q, III), piscina);
        i8* ab = _productum(r, _pars(q, ZEPHYRUM), _pars(q, I),
            piscina);
         i8* duplum[VI];
        s32  signa[VI] = { -I, I, I, -I, -I, I };
         i8* sinistra[VI];
         i8* dextra[VI];

        /* 01: bc - ad, 02: bd + ac, 10: bc + ad, 12: cd - ab,
         * 20: bd - ac, 21: cd + ab */
        sinistra[ZEPHYRUM]  = bc;
        dextra[ZEPHYRUM]    = ad;
        sinistra[I]         = bd;
        dextra[I]           = ac;
        sinistra[II]        = bc;
        dextra[II]          = ad;
        sinistra[III]       = cd;
        dextra[III]         = ab;
        sinistra[IV]        = bd;
        dextra[IV]          = ac;
        sinistra[V]         = cd;
        dextra[V]           = ab;
        per (k = ZEPHYRUM; k < VI; k++)
        {
            i8* simplex = _summa_duorum(r, sinistra[k], dextra[k],
                signa[k],
                piscina);

            duplum[k] = _summa_duorum(r, simplex, simplex, I, piscina);
            si (duplum[k] == NIHIL)
            {
                redde FALSUM;
            }
        }
        elementa[I]    = duplum[ZEPHYRUM];
        elementa[II]   = duplum[I];
        elementa[III]  = duplum[II];
        elementa[V]    = duplum[III];
        elementa[VI]   = duplum[IV];
        elementa[VII]  = duplum[V];
    }
    /* diagonalis */
    elementa[ZEPHYRUM] = _summa_duorum(r, _summa_duorum(r, aa, bb, I,
        piscina), _summa_duorum(r, cc, dd, I, piscina), -I, piscina);
    elementa[IV] = _summa_duorum(r, _summa_duorum(r, aa, cc, I,
        piscina),
        _summa_duorum(r, bb, dd, I, piscina), -I, piscina);
    elementa[VIII] = _summa_duorum(r, _summa_duorum(r, aa, dd, I,
        piscina), _summa_duorum(r, bb, cc, I, piscina), -I, piscina);
    per (k = ZEPHYRUM; k < IX; k++)
    {
        si (elementa[k] == NIHIL)
        {
            redde FALSUM;
        }
        matrix_pone(&effectus, k / III, k % III, elementa[k]);
    }
    *exitus = effectus;
    redde VERUM;
}


/* ==================================================
 * Geometria exacta
 * ================================================== */

/* x_i y_j - x_j y_i == 0 (minor nullus) */
interior b32
_minor_nullus (
     constans Anulus* anulus,
           Quaternio  x,
           Quaternio  y,
                 i32  i,
                 i32  j,
             Piscina* piscina)
{
    i8* differentia = _summa_duorum(anulus, _productum(anulus, _pars(x,
        i),
        _pars(y, j), piscina), _productum(anulus, _pars(x, j), _pars(y,
        i),
        piscina), -I, piscina);

    redde differentia != NIHIL
        && anulus->est_nullum(anulus, differentia);
}

/* partes i0..3 omnes nullae? */
interior b32
_partes_nullae (
    Quaternio q,
          i32 initium)
{
    i32 k;

    per (k = initium; k < IV; k++)
    {
        si (!q.anulus->est_nullum(q.anulus, _pars(q, k)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_eadem_rotatio (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina)
{
     constans Anulus* anulus = _communis(p, q);
                 i32  i;
                 i32  j;

    si (   anulus == NIHIL || _partes_nullae(p, ZEPHYRUM)
        || _partes_nullae(q, ZEPHYRUM))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < IV; i++)
    {
        per (j = i + I; j < IV; j++)
        {
            si (!_minor_nullus(anulus, p, q, i, j, piscina))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

interior b32
_eadem_axis (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina)
{
    constans Anulus* anulus = _communis(p, q);

    si (anulus == NIHIL || _partes_nullae(p, I) || _partes_nullae(q, I))
    {
        redde FALSUM;
    }
    /* productum vectorium nullum */
    redde _minor_nullus(anulus, p, q, I, II, piscina)
        && _minor_nullus(anulus, p, q, II, III, piscina)
        && _minor_nullus(anulus, p, q, I, III, piscina);
}

interior b32
_compara_angulum (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina,
          s32* exitus)
{
    /* angulus maior <=> a^2 / N minor: signum(a_q^2 N_p - a_p^2 N_q) */
    constans Anulus* anulus = _communis(p, q);
                 i8* norma_p;
                 i8* norma_q;
                 i8* differentia;

    si (   anulus == NIHIL || anulus->signum == NIHIL
        || quaternio_est_nullum(p) || quaternio_est_nullum(q))
    {
        redde FALSUM;
    }
    norma_p = _alveus(anulus, I, piscina);
    norma_q = _alveus(anulus, I, piscina);
    si (   !quaternio_norma(p, piscina, norma_p)
        || !quaternio_norma(q, piscina, norma_q))
    {
        redde FALSUM;
    }
    differentia = _summa_duorum(anulus, _productum(anulus, _productum(
        anulus, _pars(q, ZEPHYRUM), _pars(q, ZEPHYRUM), piscina),
        norma_p,
        piscina), _productum(anulus, _productum(anulus, _pars(p,
        ZEPHYRUM),
        _pars(p, ZEPHYRUM), piscina), norma_q, piscina), -I, piscina);
    redde _signum(anulus, differentia, piscina, exitus);
}

/* productum scalare partium vectoriarum */
interior i8*
_productum_scalare (
     constans Anulus* anulus,
           Quaternio  x,
           Quaternio  y,
             Piscina* piscina)
{
    hic_manens constans s32  signa[IV] = { I, I, I, I };
            constans vacuum* sinistra[IV];
            constans vacuum* dextra[IV];
                         i8* nullum = _alveus(anulus, I, piscina);
                         i8* exitus = _alveus(anulus, I, piscina);
                        i32  k;

    anulus->nullum(anulus, nullum);
    sinistra[ZEPHYRUM]  = nullum;
    dextra[ZEPHYRUM]    = nullum;
    per (k = I; k < IV; k++)
    {
        sinistra[k]  = _pars(x, k);
        dextra[k]    = _pars(y, k);
    }
    si (!_summa(anulus, signa, sinistra, dextra, piscina, exitus))
    {
        redde NIHIL;
    }
    redde exitus;
}

/* mensura comparationis: [signum] (v.u)^2 multiplicata per
 * |u_alter|^2 */
interior i8*
_aestimatio (
     constans Anulus* anulus,
     constans vacuum* scalare,
                 s32  signum,
     constans vacuum* quadratum_alterius,
             Piscina* piscina)
{
    i8* quadratum = _productum(anulus, scalare, scalare, piscina);
    i8* exitus;
    i8* nullum;

    si (quadratum == NIHIL)
    {
        redde NIHIL;
    }
    exitus = _productum(anulus, quadratum, quadratum_alterius, piscina);
    si (signum >= ZEPHYRUM || exitus == NIHIL)
    {
        redde exitus;
    }
    nullum = _alveus(anulus, I, piscina);
    anulus->nullum(anulus, nullum);
    redde _summa_duorum(anulus, nullum, exitus, -I, piscina);
}

interior b32
_proximus (
             Quaternio  v,
    constans Quaternio* directiones,
                   i32  numerus,
                   b32  antipodes_idem,
               Piscina* piscina,
                   i32* index)
{
     constans Anulus* anulus             = v.anulus;
                  i8* scalare_optimum    = NIHIL;
                  i8* quadratum_optimum  = NIHIL;
                 s32  signum_optimum     = ZEPHYRUM;
                 i32  optimum            = ZEPHYRUM;
                 i32  k;

    si (   anulus  == NIHIL || anulus->signum == NIHIL
        || numerus == ZEPHYRUM
        || _partes_nullae(v, I))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
         i8* scalare;
         i8* quadratum;
        s32  signum = ZEPHYRUM;

        si (   _communis(v, directiones[k]) == NIHIL
            || _partes_nullae(directiones[k], I))
        {
            redde FALSUM;
        }
        scalare    = _productum_scalare(anulus, v, directiones[k],
            piscina);
        quadratum  = _productum_scalare(anulus, directiones[k],
            directiones[k], piscina);
        si (   scalare == NIHIL || quadratum == NIHIL
            || !_signum(anulus, scalare, piscina, &signum))
        {
            redde FALSUM;
        }
        si (antipodes_idem)
        {
            signum = I;
        }
        si (k == ZEPHYRUM)
        {
            scalare_optimum    = scalare;
            quadratum_optimum  = quadratum;
            signum_optimum     = signum;
            perge;
        }
        {
            /* s_k (v.u_k)^2 |u_o|^2 > s_o (v.u_o)^2 |u_k|^2 ? */
            i8* novum = _aestimatio(anulus, scalare, signum,
                quadratum_optimum, piscina);
            i8* vetus = _aestimatio(anulus, scalare_optimum,
                signum_optimum,
                quadratum, piscina);
            s32 comparatio = ZEPHYRUM;

            si (!_signum(anulus, _summa_duorum(anulus, novum, vetus, -I,
                piscina), piscina, &comparatio))
            {
                redde FALSUM;
            }
            si (comparatio > ZEPHYRUM)
            {
                scalare_optimum    = scalare;
                quadratum_optimum  = quadratum;
                signum_optimum     = signum;
                optimum            = k;
            }
        }
    }
    *index = optimum;
    redde VERUM;
}


/* ==================================================
 * Geometria publica: responsum solum (b32, signum, index), ergo
 * piscina vocantis ad notationem initii reficitur - nihil relinquit
 * (Voronoi super multas directiones sine purgatione vocantis)
 * ================================================== */

b32
quaternio_eadem_rotatio (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina)
{
    PiscinaNotatio nota = piscina_notare(piscina);
               b32 bene = _eadem_rotatio(p, q, piscina);

    piscina_reficere(piscina, nota);
    redde bene;
}

b32
quaternio_eadem_axis (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina)
{
    PiscinaNotatio nota = piscina_notare(piscina);
               b32 bene = _eadem_axis(p, q, piscina);

    piscina_reficere(piscina, nota);
    redde bene;
}

b32
quaternio_compara_angulum (
    Quaternio  p,
    Quaternio  q,
      Piscina* piscina,
          s32* exitus)
{
    PiscinaNotatio nota = piscina_notare(piscina);
               b32 bene = _compara_angulum(p, q, piscina, exitus);

    piscina_reficere(piscina, nota);
    redde bene;
}

b32
quaternio_proximus (
             Quaternio  v,
    constans Quaternio* directiones,
                   i32  numerus,
                   b32  antipodes_idem,
               Piscina* piscina,
                   i32* index)
{
    PiscinaNotatio nota  = piscina_notare(piscina);
               b32 bene  = _proximus(v, directiones, numerus,
                   antipodes_idem, piscina, index);

    piscina_reficere(piscina, nota);
    redde bene;
}


/* ==================================================
 * Textus
 * ================================================== */

chorda
quaternio_ad_chordam (
    Quaternio  q,
      Piscina* piscina)
{
    chorda exitus;
       i32 k;

    si (q.anulus == NIHIL)
    {
        redde chorda_transcribere(chorda_ex_literis("invalidum",
            piscina), piscina);
    }
    exitus = chorda_ex_literis("[", piscina);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        si (k > ZEPHYRUM)
        {
            exitus = chorda_concatenare(exitus, chorda_ex_literis(", ",
                piscina), piscina);
        }
        exitus = chorda_concatenare(exitus,
            q.anulus->ad_chordam(q.anulus,
            _pars(q, k), piscina), piscina);
    }
    redde chorda_concatenare(exitus, chorda_ex_literis("]", piscina),
        piscina);
}

b32
quaternio_ex_chorda (
     constans Anulus* anulus,
              chorda  textus,
             Piscina* piscina,
           Quaternio* exitus)
{
    Quaternio effectus;
          s32 initium = ZEPHYRUM;
          i32 k;
          i32 j;

    si (anulus == NIHIL)
    {
        redde FALSUM;
    }
    textus = chorda_praecidere(textus);
    si (   textus.mensura < II || textus.datum[ZEPHYRUM] != '['
        || textus.datum[textus.mensura - I] != ']')
    {
        redde FALSUM;
    }
    textus           = chorda_sectio(textus, I, textus.mensura - I);
    effectus.anulus  = anulus;
    effectus.partes  = _alveus(anulus, IV, piscina);
    k                = ZEPHYRUM;
    per (j = ZEPHYRUM; j <= textus.mensura; j++)
    {
        si (j == textus.mensura || textus.datum[j] == ',')
        {
            si (   k >= IV
                || !anulus->ex_chorda(anulus, chorda_praecidere(
                chorda_sectio(textus, (i32)initium, j)), piscina, _pars(
                effectus, k)))
            {
                redde FALSUM;
            }
            k++;
            initium = (s32)j + I;
        }
    }
    si (k != IV)
    {
        redde FALSUM;
    }
    *exitus = effectus;
    redde VERUM;
}
