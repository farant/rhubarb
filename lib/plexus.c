/* plexus.c - Greges braidarum EXACTE (vide include/plexus.h)
 */
#include "plexus.h"
#include <stdio.h>
#include <string.h>

/* longitudo maxima verbi repraesentabilis */
#define PLEXUS_LONGITUDO_SUMMA ((s64)0x7FFFFFFFL)


/* ==================================================
 * Auxilia
 * ================================================== */

interior s32*
_litterae (
        i32  numerus,
    Piscina* piscina)
{
    /* numerus 0: alveus unius litterae, ne NIHIL reddatur */
    redde (s32*)piscina_allocare(piscina, (memoriae_index)(numerus
        > ZEPHYRUM ? numerus : I) * magnitudo(s32));
}

/* littera valida in B_fila: 1 <= |littera| < fila */
interior b32
_littera_valida (
    s32 littera,
    i32 fila)
{
    redde littera != ZEPHYRUM && littera < (s32)fila
        && littera > -(s32)fila;
}

/* index positionis (ab 0) litterae: |littera| - 1 */
interior i32
_index (
    s32 littera)
{
    redde (i32)(littera > ZEPHYRUM ? littera : -littera) - I;
}

/* limes vocantis intra longitudinem repraesentabilem */
interior s64
_limes (
    i64 limes)
{
    redde limes > (i64)PLEXUS_LONGITUDO_SUMMA ? PLEXUS_LONGITUDO_SUMMA
        : (s64)limes;
}


/* ==================================================
 * Verba
 * ================================================== */

b32
plexus_creare (
                i32  fila,
     constans s32*   litterae,
                i32  numerus,
            Piscina* piscina,
             Plexus* exitus)
{
    Plexus p;
       i32 k;

    si (   fila < II || fila > PLEXUS_FILA_MAXIMA
        || (s64)numerus > PLEXUS_LONGITUDO_SUMMA
        || (numerus > ZEPHYRUM && litterae == NIHIL))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (!_littera_valida(litterae[k], fila))
        {
            redde FALSUM;
        }
    }
    p.fila       = fila;
    p.longitudo  = numerus;
    p.litterae   = _litterae(numerus, piscina);
    si (numerus > ZEPHYRUM)
    {
        memcpy(p.litterae, litterae, (size_t)numerus * magnitudo(s32));
    }
    *exitus = p;
    redde VERUM;
}

b32
plexus_ex_chorda (
         i32  fila,
      chorda  textus,
     Piscina* piscina,
      Plexus* exitus)
{
    PiscinaNotatio  nota;
               s32* litterae;
               i32  numerus  = ZEPHYRUM;
               i32  k        = ZEPHYRUM;
               b32  bene;

    si (   fila < II || fila > PLEXUS_FILA_MAXIMA
        || (textus.mensura > ZEPHYRUM && textus.datum == NIHIL))
    {
        redde FALSUM;
    }
    nota = piscina_notare(piscina);
    /* litterae non plures quam (mensura + 1) / 2 */
    litterae = _litterae(textus.mensura / II + I, piscina);
    dum (k < textus.mensura)
    {
        s32 signum  = I;
        s64 valor   = ZEPHYRUM;
        i32 cifrae  = ZEPHYRUM;

        si (textus.datum[k] == ' ')
        {
            k++;
            perge;
        }
        si (textus.datum[k] == '-')
        {
            signum = -I;
            k++;
        }
        dum (   k < textus.mensura && textus.datum[k] >= '0'
             && textus.datum[k] <= '9')
        {
            valor = valor * X + (s64)(textus.datum[k] - '0');
            si (valor >= (s64)fila)
            {
                piscina_reficere(piscina, nota);
                redde FALSUM;
            }
            cifrae++;
            k++;
        }
        si (   cifrae == ZEPHYRUM || valor == ZEPHYRUM
            || (k < textus.mensura && textus.datum[k] != ' '))
        {
            piscina_reficere(piscina, nota);
            redde FALSUM;
        }
        litterae[numerus++] = signum * (s32)valor;
    }
    /* litterae temporariae manent sub plexu (piscina non reficitur post
     * creationem: plexus eas post notam allocat) */
    bene = plexus_creare(fila, litterae, numerus, piscina, exitus);
    si (!bene)
    {
        piscina_reficere(piscina, nota);
    }
    redde bene;
}

interior chorda
_litterae_ad_chordam (
      constans s32* litterae,
               i32  numerus,
           Piscina* piscina)
{
    chorda  exitus;
        i8* alveus;
       i32  k;
       i32  positio = ZEPHYRUM;

    /* " -2147483647" ad summum XII characteres per litteram */
    alveus = (i8*)piscina_allocare(piscina,
        (memoriae_index)numerus * XII
        + I);
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        positio += (i32)sprintf((character*)alveus + positio,
            k == ZEPHYRUM ? "%d" : " %d", (integer)litterae[k]);
    }
    exitus.mensura  = positio;
    exitus.datum    = alveus;
    redde exitus;
}

chorda
plexus_ad_chordam (
     Plexus  p,
    Piscina* piscina)
{
    redde _litterae_ad_chordam(p.litterae, p.longitudo, piscina);
}

chorda
verbum_liberum_ad_chordam (
    VerbumLiberum  w,
          Piscina* piscina)
{
    redde _litterae_ad_chordam(w.litterae, w.longitudo, piscina);
}

Plexus
plexus_inversum (
     Plexus  p,
    Piscina* piscina)
{
    Plexus q;
       i32 k;

    q.fila       = p.fila;
    q.longitudo  = p.longitudo;
    q.litterae   = _litterae(p.longitudo, piscina);
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        q.litterae[k] = -p.litterae[p.longitudo - I - k];
    }
    redde q;
}

b32
plexus_iungere (
     Plexus  p,
     Plexus  q,
    Piscina* piscina,
     Plexus* exitus)
{
    Plexus r;

    si (   p.fila != q.fila
        || (s64)p.longitudo + (s64)q.longitudo > PLEXUS_LONGITUDO_SUMMA)
    {
        redde FALSUM;
    }
    r.fila       = p.fila;
    r.longitudo  = p.longitudo + q.longitudo;
    r.litterae   = _litterae(r.longitudo, piscina);
    si (p.longitudo > ZEPHYRUM)
    {
        memcpy(r.litterae, p.litterae, (size_t)p.longitudo
            * magnitudo(s32));
    }
    si (q.longitudo > ZEPHYRUM)
    {
        memcpy(r.litterae + p.longitudo, q.litterae, (size_t)q.longitudo
            * magnitudo(s32));
    }
    *exitus = r;
    redde VERUM;
}

/* reductio libera per acervum: littera inversa summi eum delet;
 * redde altitudinem novam */
interior i32
_reducere_in (
      constans s32* fons,
               i32  numerus,
               s32* acervus,
               i32  altitudo)
{
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (altitudo > ZEPHYRUM && acervus[altitudo - I] == -fons[k])
        {
            altitudo--;
        }
        alioquin
        {
            acervus[altitudo++] = fons[k];
        }
    }
    redde altitudo;
}

Plexus
plexus_reducere (
     Plexus  p,
    Piscina* piscina)
{
    Plexus q;

    q.fila      = p.fila;
    q.litterae  = _litterae(p.longitudo, piscina);
    q.longitudo  = _reducere_in(p.litterae, p.longitudo, q.litterae,
        ZEPHYRUM);
    redde q;
}

s64
plexus_summa_exponentium (
    Plexus p)
{
    s64 summa = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        summa += p.litterae[k] > ZEPHYRUM ? (s64)I : -(s64)I;
    }
    redde summa;
}

vacuum
plexus_permutatio (
    Plexus  p,
       i32* imago)
{
    i32 k;

    per (k = ZEPHYRUM; k < p.fila; k++)
    {
        imago[k] = k;
    }
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        i32 i           = _index(p.litterae[k]);
        i32 commutatum  = imago[i];

        imago[i]      = imago[i + I];
        imago[i + I]  = commutatum;
    }
}

b32
plexus_dimidia_torsio (
         i32  fila,
     Piscina* piscina,
      Plexus* exitus)
{
    Plexus p;
       i32 gradus;
       i32 k;
       i32 positio = ZEPHYRUM;

    si (fila < II || fila > PLEXUS_FILA_DIMIDIAE_TORSIONIS)
    {
        redde FALSUM;
    }
    p.fila       = fila;
    p.longitudo  = (i32)(((i64)fila * (i64)(fila - I)) / II);
    p.litterae   = _litterae(p.longitudo, piscina);
    per (gradus = fila - I; gradus >= I; gradus--)
    {
        per (k = I; k <= gradus; k++)
        {
            p.litterae[positio++] = (s32)k;
        }
    }
    *exitus = p;
    redde VERUM;
}


/* ==================================================
 * Actio in gregem liberum
 * ================================================== */

/* imagines generatorum x_1 .. x_n (indice 0 .. n-1) in piscina propria;
 * summa = longitudo summa omnium */
nomen structura {
              i32  fila;
    VerbumLiberum* imagines;
              s64  summa;
          Piscina* piscina;
} _Imagines;

/* a b c (c^-1 si c_inversum) libere reductum; FALSUM si longitudo ante
 * reductionem limitem superat */
interior b32
_productum (
    constans VerbumLiberum* a,
    constans VerbumLiberum* b,
    constans VerbumLiberum* c,
                       b32  c_inversum,
                       s64  limes,
                   Piscina* piscina,
             VerbumLiberum* exitus)
{
    s64 longitudo = (s64)a->longitudo + (s64)b->longitudo
        + (s64)c->longitudo;
    s32* acervus;
    i32  altitudo;
    s32  k;

    si (longitudo > limes)
    {
        redde FALSUM;
    }
    acervus   = _litterae((i32)longitudo, piscina);
    altitudo  = _reducere_in(a->litterae, a->longitudo, acervus,
        ZEPHYRUM);
    altitudo  = _reducere_in(b->litterae, b->longitudo, acervus,
        altitudo);
    si (c_inversum)
    {
        per (k = (s32)c->longitudo - I; k >= ZEPHYRUM; k--)
        {
            s32 littera = -c->litterae[k];

            si (   altitudo > ZEPHYRUM
                && acervus[altitudo - I] == -littera)
            {
                altitudo--;
            }
            alioquin
            {
                acervus[altitudo++] = littera;
            }
        }
    }
    alioquin
    {
        altitudo = _reducere_in(c->litterae, c->longitudo, acervus,
            altitudo);
    }
    exitus->longitudo  = altitudo;
    exitus->litterae   = acervus;
    redde VERUM;
}

interior VerbumLiberum
_copia (
    VerbumLiberum  w,
          Piscina* piscina)
{
    VerbumLiberum c;

    c.longitudo  = w.longitudo;
    c.litterae   = _litterae(w.longitudo, piscina);
    si (w.longitudo > ZEPHYRUM)
    {
        memcpy(c.litterae, w.litterae, (size_t)w.longitudo
            * magnitudo(s32));
    }
    redde c;
}

interior VerbumLiberum
_inversum (
    VerbumLiberum  w,
          Piscina* piscina)
{
    VerbumLiberum c;
              i32 k;

    c.longitudo  = w.longitudo;
    c.litterae   = _litterae(w.longitudo, piscina);
    per (k = ZEPHYRUM; k < w.longitudo; k++)
    {
        c.litterae[k] = -w.litterae[w.longitudo - I - k];
    }
    redde c;
}

/* omnes imagines in piscinam novam transferuntur, vetus destruitur:
 * memoria imaginibus vivis proportionalis, non historiae */
interior b32
_contrahere (
    _Imagines* m)
{
           Piscina* nova = piscina_generare_dynamicum("plexus_imagines",
               (memoriae_index)m->summa * magnitudo(s32) + 4096);
     VerbumLiberum* imagines;
               i32  k;

    si (nova == NIHIL)
    {
        redde FALSUM;
    }
    imagines = (VerbumLiberum*)piscina_allocare(nova,
        (memoriae_index)m->fila * magnitudo(VerbumLiberum));
    per (k = ZEPHYRUM; k < m->fila; k++)
    {
        imagines[k] = _copia(m->imagines[k], nova);
    }
    piscina_destruere(m->piscina);
    m->piscina   = nova;
    m->imagines  = imagines;
    redde VERUM;
}

/* phi_p in m; FALSUM si limes superatus aut memoria deficit.
 * m->piscina (si non NIHIL) a vocante destruenda. */
interior b32
_imagines_computare (
       Plexus  p,
          s64  limes,
    _Imagines* m)
{
    i32 k;

    m->fila      = p.fila;
    m->summa     = (s64)p.fila;
    m->piscina   = NIHIL;
    m->imagines  = NIHIL;
    si (m->summa > limes)
    {
        redde FALSUM;
    }
    m->piscina = piscina_generare_dynamicum("plexus_imagines", 4096);
    si (m->piscina == NIHIL)
    {
        redde FALSUM;
    }
    m->imagines = (VerbumLiberum*)piscina_allocare(m->piscina,
        (memoriae_index)p.fila * magnitudo(VerbumLiberum));
    per (k = ZEPHYRUM; k < p.fila; k++)
    {
        m->imagines[k].longitudo           = I;
        m->imagines[k].litterae            = _litterae(I, m->piscina);
        m->imagines[k].litterae[ZEPHYRUM]  = (s32)(k + I);
    }
    /* phi := phi o phi_a, littera per litteram a sinistra (W = imagines
     * ante litteram):
     *   sigma_i:    W_i' = W_i W_{i+1} W_i^-1,  W_{i+1}' = W_i
     *   sigma_i^-1: W_i' = W_{i+1},
     *               W_{i+1}' = W_{i+1}^-1 W_i W_{i+1} */
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
                  s32 littera  = p.litterae[k];
                  i32 i        = _index(littera);
        VerbumLiberum prima    = m->imagines[i];
        VerbumLiberum secunda  = m->imagines[i + I];
        VerbumLiberum nova;
                  s64 summa_nova;

        si (littera > ZEPHYRUM)
        {
            si (!_productum(&prima, &secunda, &prima, VERUM, limes,
                m->piscina,
                &nova))
            {
                redde FALSUM;
            }
            m->imagines[i]      = nova;
            m->imagines[i + I]  = prima;
        }
        alioquin
        {
            VerbumLiberum secunda_inversa = _inversum(secunda,
                m->piscina);

            si (!_productum(&secunda_inversa, &prima, &secunda, FALSUM,
                limes,
                m->piscina, &nova))
            {
                redde FALSUM;
            }
            m->imagines[i]      = secunda;
            m->imagines[i + I]  = nova;
        }
        summa_nova = m->summa - (s64)prima.longitudo
            - (s64)secunda.longitudo
            + (s64)m->imagines[i].longitudo
            + (s64)m->imagines[i + I].longitudo;
        si (summa_nova > limes)
        {
            redde FALSUM;
        }
        m->summa = summa_nova;
        /* purgamenta: piscina quater imagines vivas superans
         * compactatur */
        si ((s64)piscina_summa_usus(m->piscina) > IV * m->summa
            * (s64)magnitudo(s32) + 65536)
        {
            si (!_contrahere(m))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

b32
plexus_imago_libera (
           Plexus  p,
              i32  j,
              i64  limes,
          Piscina* piscina,
    VerbumLiberum* exitus)
{
    _Imagines m;
          b32 bene;

    si (j < I || j > p.fila)
    {
        redde FALSUM;
    }
    bene = _imagines_computare(p, _limes(limes), &m);
    si (bene)
    {
        *exitus = _copia(m.imagines[j - I], piscina);
    }
    si (m.piscina != NIHIL)
    {
        piscina_destruere(m.piscina);
    }
    redde bene;
}

b32
plexus_aequales (
     Plexus  p,
     Plexus  q,
        i64  limes,
    Piscina* piscina,
        b32* aequales)
{
    PiscinaNotatio  nota;
            Plexus  r;
         _Imagines  m;
               i32* imago;
               b32  bene = VERUM;
               b32  idem = VERUM;
               i32  k;

    si (p.fila != q.fila)
    {
        redde FALSUM;
    }
    nota = piscina_notare(piscina);
    si (!plexus_iungere(p, plexus_inversum(q, piscina), piscina, &r))
    {
        piscina_reficere(piscina, nota);
        redde FALSUM;
    }
    r = plexus_reducere(r, piscina);
    /* invariantia necessaria (exacta): summa exponentium, permutatio */
    si (plexus_summa_exponentium(r) != ZEPHYRUM)
    {
        idem = FALSUM;
    }
    alioquin
    {
        imago = (i32*)piscina_allocare(piscina, (memoriae_index)r.fila
            * magnitudo(i32));
        plexus_permutatio(r, imago);
        per (k = ZEPHYRUM; k < r.fila; k++)
        {
            si (imago[k] != k)
            {
                idem = FALSUM;
            }
        }
    }
    si (idem)
    {
        bene = _imagines_computare(r, _limes(limes), &m);
        si (bene)
        {
            per (k = ZEPHYRUM; k < r.fila; k++)
            {
                si (   m.imagines[k].longitudo          != I
                    || m.imagines[k].litterae[ZEPHYRUM] != (s32)(k + I))
                {
                    idem = FALSUM;
                }
            }
        }
        si (m.piscina != NIHIL)
        {
            piscina_destruere(m.piscina);
        }
    }
    piscina_reficere(piscina, nota);
    si (bene)
    {
        *aequales = idem;
    }
    redde bene;
}
