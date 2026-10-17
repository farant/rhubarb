/* plexus.c - Greges braidarum EXACTE (vide include/plexus.h)
 */
#include "plexus.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <stdlib.h>
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

/* contractiones piscinarum propriarum (plexus_contractiones) */
hic_manens i64 _contractiones = ZEPHYRUM;

/* apex usus piscinarum propriarum (plexus_apex_proprius) */
hic_manens memoriae_index _apex_proprius = ZEPHYRUM;

/* omnis piscina propria per haec destruitur aut vacatur: apex eius
 * prius notatur */
interior vacuum
_apicem_notare (
    constans Piscina* piscina)
{
    memoriae_index apex = piscina_summa_apex_usus(piscina);

    si (apex > _apex_proprius)
    {
        _apex_proprius = apex;
    }
}

interior vacuum
_propriam_destruere (
    Piscina* piscina)
{
    _apicem_notare(piscina);
    piscina_destruere(piscina);
}

interior vacuum
_propriam_vacare (
    Piscina* piscina)
{
    _apicem_notare(piscina);
    piscina_vacare(piscina);
}

/* sicut _litterae, sed NIHIL si memoria deficit (piscinae propriae) */
interior s32*
_litterae_conari (
        i32  numerus,
    Piscina* piscina)
{
    redde (s32*)piscina_conari_allocare(piscina,
        (memoriae_index)(numerus
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
    /* plexus in alveo analysis ipso: omnis littera iam probata (0 <
     * |valor| < fila) */
    exitus->fila       = fila;
    exitus->longitudo  = numerus;
    exitus->litterae   = litterae;
    redde VERUM;
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

b32
plexus_validus (
    Plexus p)
{
    i32 k;

    si (   p.fila < II || p.fila > PLEXUS_FILA_MAXIMA
        || (s64)p.longitudo > PLEXUS_LONGITUDO_SUMMA
        || (p.longitudo > ZEPHYRUM && p.litterae == NIHIL))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        si (!_littera_valida(p.litterae[k], p.fila))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
plexus_permutatio (
    Plexus  p,
       i32* imago)
{
    i32 k;

    si (!plexus_validus(p))
    {
        redde FALSUM;
    }
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
    redde VERUM;
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
    memoriae_index  basis;      /* usus post contractionem ultimam */
} ImaginesLiberae;

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
    acervus   = _litterae_conari((i32)longitudo, piscina);
    si (acervus == NIHIL)
    {
        redde FALSUM;
    }
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

/* copia; conari: memoria deficiens -> FALSUM (piscinae propriae),
 * aliter fatalis (piscina vocantis) */
interior b32
_copia (
    VerbumLiberum  w,
              b32  conari,
          Piscina* piscina,
    VerbumLiberum* exitus)
{
    VerbumLiberum c;

    c.longitudo  = w.longitudo;
    c.litterae   = conari ? _litterae_conari(w.longitudo, piscina)
        : _litterae(w.longitudo, piscina);
    si (c.litterae == NIHIL)
    {
        redde FALSUM;
    }
    si (w.longitudo > ZEPHYRUM)
    {
        memcpy(c.litterae, w.litterae, (size_t)w.longitudo
            * magnitudo(s32));
    }
    *exitus = c;
    redde VERUM;
}

/* w^-1 in piscina propria; FALSUM si memoria deficit */
interior b32
_inversum (
    VerbumLiberum  w,
          Piscina* piscina,
    VerbumLiberum* exitus)
{
    VerbumLiberum c;
              i32 k;

    c.longitudo  = w.longitudo;
    c.litterae   = _litterae_conari(w.longitudo, piscina);
    si (c.litterae == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < w.longitudo; k++)
    {
        c.litterae[k] = -w.litterae[w.longitudo - I - k];
    }
    *exitus = c;
    redde VERUM;
}

/* omnes imagines in piscinam novam transferuntur, vetus destruitur:
 * memoria imaginibus vivis proportionalis, non historiae */
interior b32
_contrahere (
    ImaginesLiberae* m)
{
           Piscina* nova = piscina_generare_dynamicum("plexus_imagines",
               (memoriae_index)m->summa * magnitudo(s32) + 4096);
     VerbumLiberum* imagines;
               i32  k;

    si (nova == NIHIL)
    {
        redde FALSUM;
    }
    imagines = (VerbumLiberum*)piscina_conari_allocare(nova,
        (memoriae_index)m->fila * magnitudo(VerbumLiberum));
    si (imagines == NIHIL)
    {
        _propriam_destruere(nova);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < m->fila; k++)
    {
        si (!_copia(m->imagines[k], VERUM, nova, &imagines[k]))
        {
            _propriam_destruere(nova);
            redde FALSUM;
        }
    }
    _propriam_destruere(m->piscina);
    m->piscina   = nova;
    m->imagines  = imagines;
    m->basis     = piscina_summa_usus(nova);
    _contractiones++;
    redde VERUM;
}

/* phi_p in m; FALSUM si limes superatus aut memoria deficit.
 * m->piscina (si non NIHIL) a vocante destruenda. */
interior b32
_imagines_computare (
             Plexus  p,
                s64  limes,
    ImaginesLiberae* m)
{
    i32 k;

    m->fila      = p.fila;
    m->summa     = (s64)p.fila;
    m->piscina   = NIHIL;
    m->imagines  = NIHIL;
    m->basis     = ZEPHYRUM;
    si (m->summa > limes)
    {
        redde FALSUM;
    }
    m->piscina = piscina_generare_dynamicum("plexus_imagines", 4096);
    si (m->piscina == NIHIL)
    {
        redde FALSUM;
    }
    m->imagines = (VerbumLiberum*)piscina_conari_allocare(m->piscina,
        (memoriae_index)p.fila * magnitudo(VerbumLiberum));
    si (m->imagines == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < p.fila; k++)
    {
        m->imagines[k].longitudo  = I;
        m->imagines[k].litterae   = _litterae_conari(I, m->piscina);
        si (m->imagines[k].litterae == NIHIL)
        {
            redde FALSUM;
        }
        m->imagines[k].litterae[ZEPHYRUM] = (s32)(k + I);
    }
    m->basis = piscina_summa_usus(m->piscina);
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
            VerbumLiberum secunda_inversa;

            si (!_inversum(secunda, m->piscina, &secunda_inversa))
            {
                redde FALSUM;
            }
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
        /* purgamenta: contractio cum piscina quater crevit post
         * initium aut contractionem ultimam (onus fixum per filum in
         * basi: recensio M1, olim omni littera supra ~8192 fila) */
        si (piscina_summa_usus(m->piscina) > IV * m->basis + 65536)
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
    ImaginesLiberae m;
                b32 bene;

    si (!plexus_validus(p) || j < I || j > p.fila)
    {
        redde FALSUM;
    }
    bene = _imagines_computare(p, _limes(limes), &m);
    si (bene)
    {
        (vacuum)_copia(m.imagines[j - I], FALSUM, piscina, exitus);
    }
    si (m.piscina != NIHIL)
    {
        _propriam_destruere(m.piscina);
    }
    redde bene;
}

b32
plexus_imagines_liberae (
            Plexus   p,
               i64   limes,
           Piscina*  piscina,
     VerbumLiberum** exitus)
{
    ImaginesLiberae m;
                b32 bene;
                i32 k;

    si (!plexus_validus(p))
    {
        redde FALSUM;
    }
    bene = _imagines_computare(p, _limes(limes), &m);
    si (bene)
    {
        VerbumLiberum* imagines = (VerbumLiberum*)piscina_allocare(
            piscina, (memoriae_index)p.fila * magnitudo(VerbumLiberum));

        per (k = ZEPHYRUM; k < p.fila; k++)
        {
            (vacuum)_copia(m.imagines[k], FALSUM, piscina,
                &imagines[k]);
        }
        *exitus = imagines;
    }
    si (m.piscina != NIHIL)
    {
        _propriam_destruere(m.piscina);
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
         ImaginesLiberae  m;
                     i32* imago;
                     b32  bene = VERUM;
                     b32  idem = VERUM;
                     i32  k;

    si (p.fila != q.fila || !plexus_validus(p) || !plexus_validus(q))
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
    /* r vacuum: aequales sine actione (recensio L2: etiam limite infra
     * fila) */
    si (r.longitudo == ZEPHYRUM)
    {
        piscina_reficere(piscina, nota);
        *aequales = VERUM;
        redde VERUM;
    }
    /* invariantia necessaria (exacta): summa exponentium, permutatio */
    si (plexus_summa_exponentium(r) != ZEPHYRUM)
    {
        idem = FALSUM;
    }
    alioquin
    {
        imago = (i32*)piscina_allocare(piscina, (memoriae_index)r.fila
            * magnitudo(i32));
        (vacuum)plexus_permutatio(r, imago);
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
            _propriam_destruere(m.piscina);
        }
    }
    piscina_reficere(piscina, nota);
    si (bene)
    {
        *aequales = idem;
    }
    redde bene;
}

b32
plexus_in_maiore (
     Plexus  p,
        i32  fila,
        i32  translatio,
    Piscina* piscina,
     Plexus* exitus)
{
    Plexus q;
       i32 k;

    si (   !plexus_validus(p) || fila < II || fila > PLEXUS_FILA_MAXIMA
        || (i64)p.fila + (i64)translatio > (i64)fila)
    {
        redde FALSUM;
    }
    q.fila       = fila;
    q.longitudo  = p.longitudo;
    q.litterae   = _litterae(p.longitudo, piscina);
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        q.litterae[k] = p.litterae[k] > ZEPHYRUM
            ? p.litterae[k] + (s32)translatio
            : p.litterae[k] - (s32)translatio;
    }
    *exitus = q;
    redde VERUM;
}


/* ==================================================
 * Actio Dynnikov
 * ================================================== */

/* f+ = max(f, 0) */
interior Magnus
_positiva (
    Magnus f)
{
    redde magnus_signum(f) > ZEPHYRUM ? f : magnus_ex_s64(ZEPHYRUM);
}

/* f- = min(f, 0) */
interior Magnus
_negativa (
    Magnus f)
{
    redde magnus_signum(f) < ZEPHYRUM ? f : magnus_ex_s64(ZEPHYRUM);
}

b32
ansa_valida (
    Ansa u)
{
    redde u.fila >= III && u.fila <= PLEXUS_FILA_MAXIMA && u.a != NIHIL
        && u.b != NIHIL;
}

b32
ansa_ex_coordinatis (
                i32  fila,
    constans Magnus* a,
    constans Magnus* b,
            Piscina* piscina,
               Ansa* exitus)
{
    Ansa u;
     i32 k;

    si (   fila < III || fila > PLEXUS_FILA_MAXIMA || a == NIHIL
        || b == NIHIL)
    {
        redde FALSUM;
    }
    u.fila  = fila;
    u.a     = (Magnus*)piscina_allocare(piscina, (memoriae_index)(fila
        - II) * magnitudo(Magnus));
    u.b     = (Magnus*)piscina_allocare(piscina, (memoriae_index)(fila
        - II) * magnitudo(Magnus));
    per (k = ZEPHYRUM; k < fila - II; k++)
    {
        u.a[k] = magnus_transcribe(a[k], piscina);
        u.b[k] = magnus_transcribe(b[k], piscina);
    }
    *exitus = u;
    redde VERUM;
}

b32
ansa_circa (
         i32  fila,
         i32  k,
     Piscina* piscina,
        Ansa* exitus)
{
    Ansa u;
     i32 j;

    si (fila < III || fila > PLEXUS_FILA_MAXIMA || k < I || k >= fila)
    {
        redde FALSUM;
    }
    u.fila  = fila;
    u.a     = (Magnus*)piscina_allocare(piscina, (memoriae_index)(fila
        - II) * magnitudo(Magnus));
    u.b     = (Magnus*)piscina_allocare(piscina, (memoriae_index)(fila
        - II) * magnitudo(Magnus));
    per (j = ZEPHYRUM; j < fila - II; j++)
    {
        u.a[j] = magnus_ex_s64(ZEPHYRUM);
        u.b[j] = magnus_ex_s64(ZEPHYRUM);
    }
    /* nu_k = 2, cetera 0: b_i = (nu_i - nu_{i+1}) / 2 (aeq. 3) */
    si (k <= fila - II)
    {
        u.b[k - I] = magnus_ex_s64(I);
    }
    si (k >= II)
    {
        u.b[k - II] = magnus_ex_s64(-I);
    }
    *exitus = u;
    redde VERUM;
}

/* una littera in (a, b) loco, aeq. (8)-(15); indices fontis j (1 ..
 * n-2) -> a[j-1]. piscina pro valoribus magnis. */
interior vacuum
_ansam_littera (
        Magnus* a,
        Magnus* b,
           i32  fila,
           s32  littera,
       Piscina* piscina)
{
       i32 i = (i32)(littera > ZEPHYRUM ? littera : -littera);
       i32 m = fila - II;    /* n - 2: index fontis ultimus */
    Magnus nb;
    Magnus na;

    si (i == I && littera > ZEPHYRUM)
    {
        /* (10): b1' = a1 + b1+, a1' = -b1 + (b1')+ */
        nb = magnus_adde(a[ZEPHYRUM], _positiva(b[ZEPHYRUM]), piscina);
        na = magnus_adde(magnus_nega(b[ZEPHYRUM], piscina),
            _positiva(nb), piscina);
        a[ZEPHYRUM] = na;
        b[ZEPHYRUM] = nb;
    }
    alioquin si (i == I)
    {
        /* (14): b1' = b1+ - a1, a1' = b1 - (b1')+ */
        nb = magnus_subtrahe(_positiva(b[ZEPHYRUM]), a[ZEPHYRUM],
            piscina);
        na = magnus_subtrahe(b[ZEPHYRUM], _positiva(nb), piscina);
        a[ZEPHYRUM] = na;
        b[ZEPHYRUM] = nb;
    }
    alioquin si (i == fila - I && littera > ZEPHYRUM)
    {
        /* (11): b' = a + b-, a' = -b + (b')- (index n-2) */
        nb = magnus_adde(a[m - I], _negativa(b[m - I]), piscina);
        na = magnus_adde(magnus_nega(b[m - I], piscina), _negativa(nb),
            piscina);
        a[m - I] = na;
        b[m - I] = nb;
    }
    alioquin si (i == fila - I)
    {
        /* (15): b' = b- - a, a' = b - (b')- */
        nb = magnus_subtrahe(_negativa(b[m - I]), a[m - I], piscina);
        na = magnus_subtrahe(b[m - I], _negativa(nb), piscina);
        a[m - I] = na;
        b[m - I] = nb;
    }
    alioquin
    {
        /* 2 <= i <= n-2: indices fontis i-1 (p) et i (q) */
        Magnus a_prior    = a[i - II];
        Magnus a_sequens  = a[i - I];
        Magnus b_prior    = b[i - II];
        Magnus b_sequens  = b[i - I];
        Magnus c;

        si (littera > ZEPHYRUM)
        {
            /* (8)-(9): c = a_p - a_q - b_q+ + b_p- */
            c = magnus_adde(magnus_subtrahe(magnus_subtrahe(a_prior,
                a_sequens,
                piscina), _positiva(b_sequens), piscina),
                _negativa(b_prior),
                piscina);
            a[i - II] = magnus_subtrahe(magnus_subtrahe(a_prior,
                _positiva(b_prior), piscina), _positiva(magnus_adde(
                _positiva(b_sequens), c, piscina)), piscina);
            b[i - II] = magnus_adde(b_sequens, _negativa(c), piscina);
            a[i - I] = magnus_subtrahe(magnus_subtrahe(a_sequens,
                _negativa(b_sequens), piscina),
                _negativa(magnus_subtrahe(
                _negativa(b_prior), c, piscina)), piscina);
            b[i - I] = magnus_subtrahe(b_prior, _negativa(c), piscina);
        }
        alioquin
        {
            /* (12)-(13): d = a_p - a_q + b_q+ - b_p- */
            c = magnus_subtrahe(magnus_adde(magnus_subtrahe(a_prior,
                a_sequens,
                piscina), _positiva(b_sequens), piscina),
                _negativa(b_prior),
                piscina);
            a[i - II] = magnus_adde(magnus_adde(a_prior,
                _positiva(b_prior),
                piscina),
                _positiva(magnus_subtrahe(_positiva(b_sequens), c,
                piscina)), piscina);
            b[i - II] = magnus_subtrahe(b_sequens, _positiva(c),
                piscina);
            a[i - I] = magnus_adde(magnus_adde(a_sequens,
                _negativa(b_sequens),
                piscina), _negativa(magnus_adde(_negativa(b_prior), c,
                piscina)), piscina);
            b[i - I] = magnus_adde(b_prior, _positiva(c), piscina);
        }
    }
}

/* coordinatae in piscinam novam transcribuntur, vetus destruitur */
interior b32
_ansam_contrahere (
      Magnus*  a,
      Magnus*  b,
         i32   numerus,
     Piscina** piscina)
{
     Piscina* nova = piscina_generare_dynamicum("plexus_ansa", 4096);
         i32  k;

    si (nova == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        a[k] = magnus_transcribe(a[k], nova);
        b[k] = magnus_transcribe(b[k], nova);
    }
    _propriam_destruere(*piscina);
    *piscina = nova;
    _contractiones++;
    redde VERUM;
}

b32
ansa_agere (
        Ansa  u,
      Plexus  p,
     Piscina* piscina,
        Ansa* exitus)
{
          Piscina* temporaria;
           Magnus* a;
           Magnus* b;
              i32  numerus = u.fila - II;
              i32  k;
             Ansa  v;
   memoriae_index  basis;
   memoriae_index  limen;

    si (!ansa_valida(u) || p.fila != u.fila || !plexus_validus(p))
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("plexus_ansa", 4096);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    /* coordinatae currentes in alveo vocantis (non temporario: alveus
     * contractionem superstes esse debet) */
    a = (Magnus*)piscina_allocare(piscina, (memoriae_index)numerus
        * magnitudo(Magnus));
    b = (Magnus*)piscina_allocare(piscina, (memoriae_index)numerus
        * magnitudo(Magnus));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        a[k] = u.a[k];
        b[k] = u.b[k];
    }
    basis = piscina_summa_usus(temporaria);
    limen = (memoriae_index)numerus * IV * magnitudo(Magnus);
    si (limen < 1048576UL)
    {
        limen = 1048576UL;
    }
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        _ansam_littera(a, b, u.fila, p.litterae[k], temporaria);
        /* purgamenta: inspectio omni littera (recensio T5 M2: olim
         * post max(numerus, 256) litteras - verbum fila brevius numquam
         * contrahebatur); contractio cum piscina quater crevit post
         * contractionem ultimam et max(1 MB, 4 numerus Magnus) superat:
         * transcriptio O(numerus) amortizata */
        {
            si (piscina_summa_usus(temporaria) > IV * basis + limen)
            {
                si (!_ansam_contrahere(a, b, numerus, &temporaria))
                {
                    _propriam_destruere(temporaria);
                    redde FALSUM;
                }
                basis = piscina_summa_usus(temporaria);
            }
        }
    }
    v.fila  = u.fila;
    v.a     = a;
    v.b     = b;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        v.a[k] = magnus_transcribe(a[k], piscina);
        v.b[k] = magnus_transcribe(b[k], piscina);
    }
    _propriam_destruere(temporaria);
    *exitus = v;
    redde VERUM;
}

Magnus
ansa_sectiones (
        Ansa  u,
     Piscina* piscina)
{
     Piscina* t;
         i32  m = u.fila - II;
         i32  k;
      Magnus  summa_b;
      Magnus  maximum;
      Magnus  b_prima;
      Magnus  b_ultima;
      Magnus  longitudo;

    /* ansa invalida: 0 (vide caput) */
    si (!ansa_valida(u))
    {
        redde magnus_ex_s64(ZEPHYRUM);
    }
    /* temporaria in piscina propria t (si deficit, in vocantis) */
    t = piscina_generare_dynamicum("plexus_sectiones", 4096);
    si (t == NIHIL)
    {
        t = piscina;
    }
    summa_b = magnus_ex_s64(ZEPHYRUM);
    maximum = magnus_ex_s64(ZEPHYRUM);
    /* b_0 = -max_i (|a_i| + b_i+ + sum_{j<i} b_j)  (aeq. 6) */
    per (k = ZEPHYRUM; k < m; k++)
    {
        Magnus terminus =
            magnus_adde(magnus_adde(magnus_absolutum(u.a[k],
            t), _positiva(u.b[k]), t), summa_b, t);

        si (   k == ZEPHYRUM
            || magnus_compara(terminus, maximum) > ZEPHYRUM)
        {
            maximum = terminus;
        }
        summa_b = magnus_adde(summa_b, u.b[k], t);
    }
    b_prima   = magnus_nega(maximum, t);
    b_ultima  = magnus_subtrahe(magnus_nega(b_prima, t), summa_b, t);
    /* L = |a_1| + |a_{n-2}| + sum |a_{i+1} - a_i| + sum_{0..n-1} |b_i|
     * (aeq. 5) */
    longitudo = magnus_adde(magnus_absolutum(u.a[ZEPHYRUM], t),
        magnus_absolutum(u.a[m - I], t), t);
    per (k = ZEPHYRUM; k + I < m; k++)
    {
        longitudo = magnus_adde(longitudo, magnus_absolutum(
            magnus_subtrahe(u.a[k + I], u.a[k], t), t), t);
    }
    per (k = ZEPHYRUM; k < m; k++)
    {
        longitudo = magnus_adde(longitudo, magnus_absolutum(u.b[k], t),
            t);
    }
    longitudo = magnus_adde(longitudo, magnus_absolutum(b_prima, t), t);
    longitudo = magnus_adde(longitudo, magnus_absolutum(b_ultima, t),
        t);
    si (t != piscina)
    {
        longitudo = magnus_transcribe(longitudo, piscina);
        _propriam_destruere(t);
    }
    redde longitudo;
}

b32
ansae_aequales (
    Ansa u,
    Ansa v)
{
    i32 k;

    si (u.fila != v.fila || !ansa_valida(u) || !ansa_valida(v))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k + II < u.fila; k++)
    {
        si (   !magnus_aequalis(u.a[k], v.a[k])
            || !magnus_aequalis(u.b[k], v.b[k]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

chorda
ansa_ad_chordam (
        Ansa  u,
     Piscina* piscina)
{
     ChordaAedificator* ae;
                   i32  k;

    si (!ansa_valida(u))
    {
        redde chorda_ex_literis("", piscina);
    }
    ae = chorda_aedificator_creare(piscina, CXXVIII);
    (vacuum)chorda_aedificator_appendere_character(ae, '(');
    per (k = ZEPHYRUM; k + II < u.fila; k++)
    {
        si (k > ZEPHYRUM)
        {
            (vacuum)chorda_aedificator_appendere_character(ae, ' ');
        }
        (vacuum)chorda_aedificator_appendere_chorda(ae,
            magnus_ad_chordam(u.a[k], piscina));
    }
    (vacuum)chorda_aedificator_appendere_literis(ae, "; ");
    per (k = ZEPHYRUM; k + II < u.fila; k++)
    {
        si (k > ZEPHYRUM)
        {
            (vacuum)chorda_aedificator_appendere_character(ae, ' ');
        }
        (vacuum)chorda_aedificator_appendere_chorda(ae,
            magnus_ad_chordam(u.b[k], piscina));
    }
    (vacuum)chorda_aedificator_appendere_character(ae, ')');
    redde chorda_aedificator_finire(ae);
}

b32
plexus_aequales_ansis (
     Plexus  p,
     Plexus  q,
    Piscina* piscina,
        b32* aequales)
{
    PiscinaNotatio  nota;
            Plexus  r;
            Plexus  maior;
               i32* imago;
               i32  fila = p.fila + II;
               i32  k;
               b32  idem = VERUM;

    si (   p.fila != q.fila || fila > PLEXUS_FILA_MAXIMA
        || !plexus_validus(p) || !plexus_validus(q))
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
    /* r vacuum, summa exponentium, permutatio: exacta, sine ansis
     * (recensio T5 M1: structura plexus_aequales) */
    si (r.longitudo == ZEPHYRUM)
    {
        piscina_reficere(piscina, nota);
        *aequales = VERUM;
        redde VERUM;
    }
    si (plexus_summa_exponentium(r) != ZEPHYRUM)
    {
        piscina_reficere(piscina, nota);
        *aequales = FALSUM;
        redde VERUM;
    }
    imago = (i32*)piscina_allocare(piscina, (memoriae_index)r.fila
        * magnitudo(i32));
    (vacuum)plexus_permutatio(r, imago);
    per (k = ZEPHYRUM; k < r.fila; k++)
    {
        si (imago[k] != k)
        {
            idem = FALSUM;
        }
    }
    si (!idem)
    {
        piscina_reficere(piscina, nota);
        *aequales = FALSUM;
        redde VERUM;
    }
    si (!plexus_in_maiore(r, fila, I, piscina, &maior))
    {
        piscina_reficere(piscina, nota);
        redde FALSUM;
    }
    /* omnis ansa sub nota sua: memoria O(fila), non O(fila^2) */
    per (k = I; idem && k < fila; k++)
    {
        PiscinaNotatio nota_ansae = piscina_notare(piscina);
                  Ansa e;
                  Ansa imago_ansae;

        si (   !ansa_circa(fila, k, piscina, &e)
            || !ansa_agere(e, maior, piscina, &imago_ansae))
        {
            piscina_reficere(piscina, nota);
            redde FALSUM;
        }
        idem = ansae_aequales(e, imago_ansae);
        piscina_reficere(piscina, nota_ansae);
    }
    piscina_reficere(piscina, nota);
    *aequales = idem;
    redde VERUM;
}

i64
plexus_contractiones (vacuum)
{
    redde _contractiones;
}


/* ==================================================
 * Repraesentatio Burau reducta
 * ================================================== */

/* columna c matricis (n-1) x (n-1) ordine linearum: m[r * d + c] */

/* una littera: B <- B G(littera), columna una mutatur. FALSUM si
 * exponens extra fines polynomii (2^30: verbum longissimum) */
interior b32
_burau_littera (
    Polynomium* m,
           i32  fila,
           s32  littera,
       Piscina* piscina)
{
           i32 d = fila - I;
           i32 i = (i32)(littera > ZEPHYRUM ? littera : -littera);
           i32 r;
           s32 k = littera > ZEPHYRUM ? I : -I;
    Polynomium x;

    si (fila == II)
    {
        /* [-t] aut [-t^-1] */
        si (!polynomium_translata(m[ZEPHYRUM], k, piscina, &x))
        {
            redde FALSUM;
        }
        m[ZEPHYRUM] = polynomium_nega(x, piscina);
        redde VERUM;
    }
    per (r = ZEPHYRUM; r < d; r++)
    {
        Polynomium* l = m + r * d;

        si (i == I)
        {
            /* G = [[-t, 0], [1, 1]]: c0' = c1 - t c0;
             * G^-1 = [[-t^-1, 0], [t^-1, 1]]: c0' = t^-1 (c1 - c0) */
            si (littera > ZEPHYRUM)
            {
                si (!polynomium_translata(l[ZEPHYRUM], I, piscina, &x))
                {
                    redde FALSUM;
                }
                l[ZEPHYRUM] = polynomium_subtrahe(l[I], x, piscina);
            }
            alioquin
            {
                si (!polynomium_translata(polynomium_subtrahe(l[I],
                    l[ZEPHYRUM], piscina), -I, piscina, &x))
                {
                    redde FALSUM;
                }
                l[ZEPHYRUM] = x;
            }
        }
        alioquin si (i == fila - I)
        {
            /* G = [[1, t], [0, -t]]: c1' = t (c0 - c1);
             * G^-1 = [[1, 1], [0, -t^-1]]: c1' = c0 - t^-1 c1 */
            si (littera > ZEPHYRUM)
            {
                si (!polynomium_translata(polynomium_subtrahe(l[d - II],
                    l[d - I], piscina), I, piscina, &x))
                {
                    redde FALSUM;
                }
                l[d - I] = x;
            }
            alioquin
            {
                si (!polynomium_translata(l[d - I], -I, piscina, &x))
                {
                    redde FALSUM;
                }
                l[d - I] = polynomium_subtrahe(l[d - II], x, piscina);
            }
        }
        alioquin
        {
            /* columnae i-2, i-1, i (ab 0); G: c1' = t (c0 - c1) + c2;
             * G^-1: c1' = c0 + t^-1 (c2 - c1) */
            i32 c0 = i - II;

            si (littera > ZEPHYRUM)
            {
                si (!polynomium_translata(polynomium_subtrahe(l[c0],
                    l[c0 + I], piscina), I, piscina, &x))
                {
                    redde FALSUM;
                }
                l[c0 + I] = polynomium_adde(x, l[c0 + II], piscina);
            }
            alioquin
            {
                si (!polynomium_translata(polynomium_subtrahe(l[c0
                    + II],
                    l[c0 + I], piscina), -I, piscina, &x))
                {
                    redde FALSUM;
                }
                l[c0 + I] = polynomium_adde(l[c0], x, piscina);
            }
        }
    }
    redde VERUM;
}

/* eadem ad t = -1 super Z: t -> -1, t^-1 -> -1 */
interior vacuum
_burau_littera_integra (
     Magnus* m,
        i32  fila,
        s32  littera,
    Piscina* piscina)
{
    i32 d = fila - I;
    i32 i = (i32)(littera > ZEPHYRUM ? littera : -littera);
    i32 r;

    si (fila == II)
    {
        redde;    /* -t = -t^-1 = 1 */
    }
    per (r = ZEPHYRUM; r < d; r++)
    {
        Magnus* l = m + r * d;

        si (i == I)
        {
            /* c0' = c0 + c1 (sigma), c0 - c1 (inversum) */
            l[ZEPHYRUM] = littera > ZEPHYRUM
                ? magnus_adde(l[ZEPHYRUM], l[I], piscina)
                : magnus_subtrahe(l[ZEPHYRUM], l[I], piscina);
        }
        alioquin si (i == fila - I)
        {
            /* c1' = c1 - c0 (sigma), c0 + c1 (inversum) */
            l[d - I] = littera > ZEPHYRUM
                ? magnus_subtrahe(l[d - I], l[d - II], piscina)
                : magnus_adde(l[d - II], l[d - I], piscina);
        }
        alioquin
        {
            /* c1' = c1 + c2 - c0 (sigma), c0 + c1 - c2 (inversum) */
            i32 c0 = i - II;

            l[c0 + I] = littera > ZEPHYRUM
                ? magnus_subtrahe(magnus_adde(l[c0 + I], l[c0 + II],
                piscina), l[c0], piscina)
                : magnus_subtrahe(magnus_adde(l[c0], l[c0 + I],
                piscina),
                l[c0 + II], piscina);
        }
    }
}

b32
plexus_burau (
     Plexus  p,
    Piscina* piscina,
     Matrix* exitus)
{
       Piscina* temporaria;
    Polynomium* m;
           i32  d = p.fila - I;
           i32  k;
           i32  r;
        Matrix  b;
memoriae_index  basis;

    si (!plexus_validus(p) || p.fila > PLEXUS_FILA_BURAU)
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("plexus_burau", 4096);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    m = (Polynomium*)piscina_allocare(temporaria, (memoriae_index)(d
        * d) * magnitudo(Polynomium));
    per (k = ZEPHYRUM; k < d * d; k++)
    {
        m[k] = (k / d == k % d)
            ? polynomium_constans(magnus_ex_s64(I), temporaria)
            : polynomium_nullum();
    }
    basis = piscina_summa_usus(temporaria);
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        si (!_burau_littera(m, p.fila, p.litterae[k], temporaria))
        {
            _propriam_destruere(temporaria);
            redde FALSUM;
        }
        /* purgamenta: contractio cum piscina quater crevit et 1 MB
         * superat (inspectio post LXIV litteras) */
        si (   (k % LXIV) == LXIII
            && piscina_summa_usus(temporaria) > IV * basis + 1048576UL)
        {
            Piscina* nova = piscina_generare_dynamicum("plexus_burau",
                4096);

            si (nova == NIHIL)
            {
                _propriam_destruere(temporaria);
                redde FALSUM;
            }
            {
                /* alveus matricis quoque in piscinam novam */
                Polynomium* m_nova = (Polynomium*)piscina_allocare(nova,
                    (memoriae_index)(d * d) * magnitudo(Polynomium));

                per (r = ZEPHYRUM; r < d * d; r++)
                {
                    m_nova[r] = polynomium_transcribe(m[r], nova);
                }
                m = m_nova;
            }
            _propriam_destruere(temporaria);
            temporaria  = nova;
            basis       = piscina_summa_usus(temporaria);
            _contractiones++;
        }
    }
    si (!matrix_nulla(&ANULUS_POLYNOMIORUM, d, d, piscina, &b))
    {
        _propriam_destruere(temporaria);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < d * d; k++)
    {
        Polynomium x = polynomium_transcribe(m[k], piscina);

        matrix_pone(&b, k / d, k % d, &x);
    }
    _propriam_destruere(temporaria);
    *exitus = b;
    redde VERUM;
}

b32
plexus_burau_minus_uno (
     Plexus  p,
    Piscina* piscina,
     Matrix* exitus)
{
       Piscina* temporaria;
        Magnus* m;
           i32  d = p.fila - I;
           i32  k;
           i32  r;
        Matrix  b;
memoriae_index  basis;

    si (!plexus_validus(p) || p.fila > PLEXUS_FILA_BURAU)
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("plexus_burau", 4096);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    m = (Magnus*)piscina_allocare(temporaria, (memoriae_index)(d * d)
        * magnitudo(Magnus));
    per (k = ZEPHYRUM; k < d * d; k++)
    {
        m[k] = magnus_ex_s64(k / d == k % d ? I : ZEPHYRUM);
    }
    basis = piscina_summa_usus(temporaria);
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        _burau_littera_integra(m, p.fila, p.litterae[k], temporaria);
        si (   (k % LXIV) == LXIII
            && piscina_summa_usus(temporaria) > IV * basis + 1048576UL)
        {
            Piscina* nova = piscina_generare_dynamicum("plexus_burau",
                4096);

            si (nova == NIHIL)
            {
                _propriam_destruere(temporaria);
                redde FALSUM;
            }
            {
                /* alveus matricis quoque in piscinam novam */
                Magnus* m_nova = (Magnus*)piscina_allocare(nova,
                    (memoriae_index)(d * d) * magnitudo(Magnus));

                per (r = ZEPHYRUM; r < d * d; r++)
                {
                    m_nova[r] = magnus_transcribe(m[r], nova);
                }
                m = m_nova;
            }
            _propriam_destruere(temporaria);
            temporaria  = nova;
            basis       = piscina_summa_usus(temporaria);
            _contractiones++;
        }
    }
    si (!matrix_nulla(&ANULUS_INTEGRORUM, d, d, piscina, &b))
    {
        _propriam_destruere(temporaria);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < d * d; k++)
    {
        Magnus x = magnus_transcribe(m[k], piscina);

        matrix_pone(&b, k / d, k % d, &x);
    }
    _propriam_destruere(temporaria);
    *exitus = b;
    redde VERUM;
}

b32
plexus_alexander (
         Plexus  p,
        Piscina* piscina,
     Polynomium* exitus)
{
    Piscina* t;
     Matrix  b;
     Matrix  unitas;
     Matrix  differentia;
 Polynomium  determinans;
 Polynomium  unum_minus_t;
 Polynomium  denominator;
 Polynomium  numerator;
 Polynomium  quotiens;
 Polynomium  effectum;
 Polynomium  t_n;
        b32  bene;

    si (!plexus_validus(p) || p.fila > PLEXUS_FILA_BURAU)
    {
        redde FALSUM;
    }
    t = piscina_generare_dynamicum("plexus_alexander", 4096);
    si (t == NIHIL)
    {
        redde FALSUM;
    }
    /* det(I - B) (1 - t) / (1 - t^n) */
    bene = plexus_burau(p, t, &b)
        && matrix_identitas(&ANULUS_POLYNOMIORUM, p.fila - I, t,
        &unitas)
        && matrix_subtrahe(unitas, b, t, &differentia)
        && matrix_determinans(differentia, t, &determinans)
        && polynomium_monomium(magnus_ex_s64(I), I, t, &t_n);
    si (bene)
    {
        unum_minus_t = polynomium_subtrahe(polynomium_constans(
            magnus_ex_s64(I), t), t_n, t);
        bene = polynomium_monomium(magnus_ex_s64(I), (s32)p.fila, t,
            &t_n)
            && polynomium_multiplica(determinans, unum_minus_t, t,
            &numerator);
    }
    si (bene)
    {
        denominator = polynomium_subtrahe(polynomium_constans(
            magnus_ex_s64(I), t), t_n, t);
        bene = polynomium_divide_exacte(numerator, denominator, t,
            &quotiens)
            && polynomium_normale(quotiens, t, &effectum);
    }
    si (bene)
    {
        *exitus = polynomium_transcribe(effectum, piscina);
    }
    _propriam_destruere(t);
    redde bene;
}

b32
plexus_burau_radix_maxima (
          Plexus  p,
         Piscina* piscina,
     RadixRealis* exitus)
{
         Piscina* t;
          Matrix  b;
          Matrix  matrix_propria;
      Polynomium  polynomium_proprium;
      Polynomium  polynomium_reflexum;
      Polynomium  h;
     RadixRealis* radices;
     RadixRealis  unum;
     RadixRealis  maxima;
             i32  numerus  = ZEPHYRUM;
             i32  d        = p.fila - I;
             i32  k;
             s32  signum  = ZEPHYRUM;
             b32  bene;

    si (!plexus_validus(p) || p.fila > PLEXUS_FILA_BURAU)
    {
        redde FALSUM;
    }
    t = piscina_generare_dynamicum("plexus_radix", 4096);
    si (t == NIHIL)
    {
        redde FALSUM;
    }
    /* chi(x) = det(x I - B(-1)) super Z[x] */
    bene = plexus_burau_minus_uno(p, t, &b)
        && matrix_nulla(&ANULUS_POLYNOMIORUM, d, d, t,
        &matrix_propria);
    per (k = ZEPHYRUM; bene && k < d * d; k++)
    {
               i32 linea    = k / d;
               i32 columna  = k % d;
        Polynomium x;

        x = polynomium_nega(polynomium_constans(*(constans Magnus*)
            matrix_elementum(b, linea, columna), t), t);
        si (linea == columna)
        {
            Polynomium monomium;

            bene = polynomium_monomium(magnus_ex_s64(I), I, t,
                &monomium);
            x = polynomium_adde(x, monomium, t);
        }
        matrix_pone(&matrix_propria, linea, columna, &x);
    }
    bene = bene
        && matrix_determinans(matrix_propria, t, &polynomium_proprium);
    si (bene)
    {
        /* chi(-x): coefficientes graduum imparium negantur */
           s32  summus = polynomium_gradus_summus(polynomium_proprium);
        Magnus* c      = (Magnus*)piscina_allocare(t,
            (memoriae_index)(summus + I) * magnitudo(Magnus));
        s32 e;

        per (e = ZEPHYRUM; e <= summus; e++)
        {
            Magnus coefficiens =
                polynomium_coefficiens(polynomium_proprium, e);

            c[e] = (e % II == I) ? magnus_nega(coefficiens, t)
                : coefficiens;
        }
        bene = polynomium_ex_coefficientibus(c, (i32)(summus + I),
            ZEPHYRUM, t, &polynomium_reflexum)
            && polynomium_multiplica(polynomium_proprium,
            polynomium_reflexum, t, &h)
            && radices_reales(h, t, &radices, &numerus);
    }
    si (bene)
    {
        /* radices h = +-lambda: maxima = max |lambda| realis;
         * minimum 1 */
        unum    = radix_ex_fractione(fractio_ex_s64(I), t);
        maxima  = unum;
        si (numerus > ZEPHYRUM)
        {
            bene = radix_compara(radices[numerus - I], unum, t,
                &signum);
            si (bene && signum > ZEPHYRUM)
            {
                maxima = radices[numerus - I];
            }
        }
    }
    si (bene)
    {
        RadixRealis copia;

        copia.f      = polynomium_transcribe(maxima.f, piscina);
        copia.infra  = fractio_transcribe(maxima.infra, piscina);
        copia.supra  = fractio_transcribe(maxima.supra, piscina);
        *exitus      = copia;
    }
    _propriam_destruere(t);
    redde bene;
}


/* ==================================================
 * Entropia per incrementum ansarum
 * ================================================== */

/* ansa in piscinam transcripta (coordinatae et alvei novi) */
interior Ansa
_ansam_transcribere (
        Ansa  u,
     Piscina* piscina)
{
    Ansa v;
     i32 k;

    v.fila  = u.fila;
    v.a     = (Magnus*)piscina_allocare(piscina, (memoriae_index)(u.fila
        - II) * magnitudo(Magnus));
    v.b     = (Magnus*)piscina_allocare(piscina, (memoriae_index)(u.fila
        - II) * magnitudo(Magnus));
    per (k = ZEPHYRUM; k + II < u.fila; k++)
    {
        v.a[k] = magnus_transcribe(u.a[k], piscina);
        v.b[k] = magnus_transcribe(u.b[k], piscina);
    }
    redde v;
}

b32
plexus_incrementum (
     Plexus   p,
       Ansa   u,
        i32   periodi,
    Piscina*  piscina,
     Magnus** exitus)
{
     Piscina* prior;
     Piscina* sequens;
      Magnus* longitudines;
        Ansa  currens;
         i32  m;
         b32  bene = VERUM;

    si (   !plexus_validus(p) || !ansa_valida(u) || p.fila != u.fila
        || periodi > 0x7FFFFFFEUL)
    {
        redde FALSUM;
    }
    prior    = piscina_generare_dynamicum("plexus_incrementum", 4096);
    sequens  = piscina_generare_dynamicum("plexus_incrementum", 4096);
    si (prior == NIHIL || sequens == NIHIL)
    {
        si (prior != NIHIL)
        {
            _propriam_destruere(prior);
        }
        si (sequens != NIHIL)
        {
            _propriam_destruere(sequens);
        }
        redde FALSUM;
    }
    longitudines = (Magnus*)piscina_allocare(piscina,
        (memoriae_index)(periodi + I) * magnitudo(Magnus));
    currens = _ansam_transcribere(u, prior);
    per (m = ZEPHYRUM; bene && m <= periodi; m++)
    {
        longitudines[m] = magnus_transcribe(ansa_sectiones(currens,
            prior), piscina);
        si (m < periodi)
        {
               Ansa  nova;
            Piscina* commutata;

            /* u_{m+1} in 'sequens'; 'prior' vacatur et commutatur */
            bene = ansa_agere(currens, p, sequens, &nova);
            si (bene)
            {
                _propriam_vacare(prior);
                commutata  = prior;
                prior      = sequens;
                sequens    = commutata;
                currens    = nova;
            }
        }
    }
    _propriam_destruere(prior);
    _propriam_destruere(sequens);
    si (bene)
    {
        *exitus = longitudines;
    }
    redde bene;
}

memoriae_index
plexus_apex_proprius (vacuum)
{
    redde _apex_proprius;
}

vacuum
plexus_apicem_purgare (vacuum)
{
    _apex_proprius = ZEPHYRUM;
}


/* ==================================================
 * Certificatum per Burau in radicibus unitatis
 * ================================================== */

/* phi(k) per divisionem (k parvus) */
interior i32
_phi (
    i32 k)
{
    i32 r = k;
    i32 n = k;
    i32 d;

    per (d = II; d * d <= n; d++)
    {
        si (n % d == ZEPHYRUM)
        {
            dum (n % d == ZEPHYRUM)
            {
                n /= d;
            }
            r -= r / d;
        }
    }
    si (n > I)
    {
        r -= r / n;
    }
    redde r;
}

b32
plexus_burau_radius_maior (
     Plexus  p,
        i32  ordo,
        b32* maior)
{
        Piscina* t;
     Cyclotomia* anulus;
         Matrix  b;
         Matrix  propria;
     Polynomium  proprium;
            i32  d = p.fila - I;
            i32  g;
            i32  dim;
            i32  a;
            i32  c;
            i32  i;
            i32  j;
            i32  k;
            b32  bene;
     Polynomium  monomium;

    si (!plexus_validus(p) || ordo < II || ordo > XII)
    {
        redde FALSUM;
    }
    g = _phi(ordo);
    si ((i64)d * (i64)g > (i64)PLEXUS_BURAU_CIRCULI)
    {
        redde FALSUM;
    }
    dim  = d * g;
    t    = piscina_generare_dynamicum("plexus_circulus", 4096);
    si (t == NIHIL)
    {
        redde FALSUM;
    }
    anulus = cyclotomia_creare(ordo, t);
    bene = anulus != NIHIL && plexus_burau(p, t, &b)
        && matrix_nulla(&ANULUS_POLYNOMIORUM, dim, dim, t, &propria)
        && polynomium_monomium(magnus_ex_s64(I), I, t, &monomium);
    /* xI - R: R[(a,i),(c,j)] = coefficiens zeta^i in B[a][c] zeta^j */
    per (a = ZEPHYRUM; bene && a < d; a++)
    {
        per (c = ZEPHYRUM; bene && c < d; c++)
        {
            Cyclotomicus elementum;

            bene = cyclotomicus_ex_polynomio(anulus,
                *(constans Polynomium*)matrix_elementum(b, a, c), I, t,
                &elementum);
            per (j = ZEPHYRUM; bene && j < g; j++)
            {
                Cyclotomicus columna =
                    cyclotomicus_multiplica(elementum,
                    cyclotomicus_radix(anulus, (s32)j, t), t);

                per (i = ZEPHYRUM; i < g; i++)
                {
                    Polynomium x = polynomium_nega(polynomium_constans(
                        cyclotomicus_coefficiens(columna, i), t), t);

                    si (a == c && i == j)
                    {
                        x = polynomium_adde(x, monomium, t);
                    }
                    matrix_pone(&propria, a * g + i, c * g + j, &x);
                }
            }
        }
    }
    bene = bene && matrix_determinans(propria, t, &proprium);
    /* Kronecker: factores cyclotomici dividuntur; residuum gradus > 0
     * -> radix |z| > 1 */
    per (k = I; bene && k <= II * (i32)dim * (i32)dim + II; k++)
    {
        Polynomium phi_k;
        Polynomium quotiens;
               i32 gradus_k = _phi(k);

        si ((s32)gradus_k > polynomium_gradus_summus(proprium))
        {
            perge;
        }
        si (!polynomium_cyclotomicum(k, t, &phi_k))
        {
            bene = FALSUM;
            frange;
        }
        dum (   polynomium_gradus_summus(proprium)
            >= (s32)gradus_k
             && polynomium_divide_exacte(proprium, phi_k, t,
             &quotiens))
        {
            proprium = quotiens;
        }
    }
    si (bene)
    {
        *maior = polynomium_gradus_summus(proprium) > ZEPHYRUM;
    }
    _propriam_destruere(t);
    redde bene;
}


/* ==================================================
 * Actio Dynnikov ut rete (nervus)
 * ================================================== */

/* instructio ex terminis parvis: V paria (fons, coefficiens),
 * coefficiens 0 terminum omittit. Registrum novum, aut -1 si *bene iam
 * FALSUM aut constructio refutat (tunc *bene = FALSUM). */
interior s32
_nervus_instructio (
          Nervus* rete,
     NervusGenus  genus,
             s32  f0,
             s32  c0,
             s32  f1,
             s32  c1,
             s32  f2,
             s32  c2,
             s32  f3,
             s32  c3,
             s32  f4,
             s32  c4,
             b32* bene)
{
       s32 fontes[V];
       s32 coef[V];
       s32 f[V];
    Magnus c[V];
       i32 k;
       i32 termini  = ZEPHYRUM;
       s32 novum    = -I;
       b32 factum;

    si (!*bene)
    {
        redde -I;
    }
    fontes[ZEPHYRUM]  = f0;
    fontes[I]         = f1;
    fontes[II]        = f2;
    fontes[III]       = f3;
    fontes[IV]        = f4;
    coef[ZEPHYRUM]    = c0;
    coef[I]           = c1;
    coef[II]          = c2;
    coef[III]         = c3;
    coef[IV]          = c4;
    per (k = ZEPHYRUM; k < V; k++)
    {
        si (coef[k] != ZEPHYRUM)
        {
            f[termini] = fontes[k];
            c[termini] = magnus_ex_s64((s64)coef[k]);
            termini++;
        }
    }
    si (genus == NERVUS_RELU)
    {
        factum = nervus_relu(rete, termini, f, c,
            magnus_ex_s64(ZEPHYRUM),
            &novum);
    }
    alioquin
    {
        factum = nervus_affinis(rete, termini, f, c,
            magnus_ex_s64(ZEPHYRUM), &novum);
    }
    si (!factum)
    {
        *bene = FALSUM;
        redde -I;
    }
    redde novum;
}

/* littera una: registra currentia coordinatarum (cur: a_j in cur[j-1],
 * b_j in cur[m+j-1]) legit et novat. Aeq. 8-15 (Thiffeault 2010),
 * f- = f - relu(f). */
interior b32
_nervus_littera (
     Nervus* rete,
        i32  fila,
        s32  littera,
        s32* cur)
{
    i32 m     = fila - II;
    i32 i     = (i32)(littera > ZEPHYRUM ? littera : -littera);
    b32 bene  = VERUM;

    si (i == I || i == fila - I)
    {
        /* sigma_1 (10, 14) et sigma_{n-1} (11, 15), indice j */
        i32 j = i == I ? I : m;
        s32 a = cur[j - I];
        s32 b = cur[m + j - I];
        s32 t1;
        s32 nb;
        s32 t2;
        s32 na;

        t1 = _nervus_instructio(rete, NERVUS_RELU, b, I, ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM, &bene);
        si (i == I && littera > ZEPHYRUM)
        {
            /* b' = a + b+,  a' = -b + (b')+ */
            nb = _nervus_instructio(rete, NERVUS_AFFINIS, a, I, t1, I,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, &bene);
            t2 = _nervus_instructio(rete, NERVUS_RELU, nb, I,
                ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, &bene);
            na = _nervus_instructio(rete, NERVUS_AFFINIS, b, -I, t2, I,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, &bene);
        }
        alioquin si (i == I)
        {
            /* b' = b+ - a,  a' = b - (b')+ */
            nb = _nervus_instructio(rete, NERVUS_AFFINIS, t1, I, a, -I,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, &bene);
            t2 = _nervus_instructio(rete, NERVUS_RELU, nb, I,
                ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, &bene);
            na = _nervus_instructio(rete, NERVUS_AFFINIS, b, I, t2, -I,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, &bene);
        }
        alioquin si (littera > ZEPHYRUM)
        {
            /* b' = a + b-,  a' = -b + (b')-   (f- = f - f+) */
            nb = _nervus_instructio(rete, NERVUS_AFFINIS, a, I, b, I,
                t1,
                -I, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
            t2 = _nervus_instructio(rete, NERVUS_RELU, nb, I,
                ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, &bene);
            na = _nervus_instructio(rete, NERVUS_AFFINIS, b, -I, nb, I,
                t2, -I, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
        }
        alioquin
        {
            /* b' = b- - a,  a' = b - (b')- */
            nb = _nervus_instructio(rete, NERVUS_AFFINIS, b, I, t1, -I,
                a,
                -I, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
            t2 = _nervus_instructio(rete, NERVUS_RELU, nb, I,
                ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, &bene);
            na = _nervus_instructio(rete, NERVUS_AFFINIS, b, I, nb, -I,
                t2, I, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
        }
        cur[j - I]      = na;
        cur[m + j - I]  = nb;
    }
    alioquin
    {
        /* mediae, p = i-1, q = i: positiva (8)-(9) cum c, negativa
         * (12)-(13) cum d; signum sg = +1 / -1 utrumque tegit */
        i32 p          = i - I;
        i32 q          = i;
        s32 a_prior    = cur[p - I];
        s32 a_sequens  = cur[q - I];
        s32 b_prior    = cur[m + p - I];
        s32 b_sequens  = cur[m + q - I];
        s32 sg         = littera > ZEPHYRUM ? I : -I;
        s32 pos        = littera > ZEPHYRUM ? I : ZEPHYRUM;
        s32 b_sequens_positiva;
        s32 b_prior_positiva;
        s32 d;
        s32 u;
        s32 d_positiva;
        s32 w_positiva;
        s32 a_prior_nova;
        s32 b_prior_nova;
        s32 pars;
        s32 a_sequens_nova;
        s32 b_sequens_nova;

        b_sequens_positiva = _nervus_instructio(rete, NERVUS_RELU,
            b_sequens, I,
            ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM, &bene);
        b_prior_positiva = _nervus_instructio(rete, NERVUS_RELU,
            b_prior, I,
            ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM, &bene);
        /* c = a_p - a_q - b_q+ + b_p - b_p+
         * d = a_p - a_q + b_q+ - b_p + b_p+ */
        d = _nervus_instructio(rete, NERVUS_AFFINIS, a_prior, I,
            a_sequens, -I,
            b_sequens_positiva,
            -sg, b_prior, sg, b_prior_positiva, -sg, &bene);
        /* (b_q+ + c)+  aut  (b_q+ - d)+ */
        u = _nervus_instructio(rete, NERVUS_RELU, b_sequens_positiva, I,
            d, sg,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            &bene);
        d_positiva = _nervus_instructio(rete, NERVUS_RELU, d, I,
            ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM, &bene);
        /* w = b_p - b_p+ - c  aut  b_p - b_p+ + d */
        w_positiva = _nervus_instructio(rete, NERVUS_RELU, b_prior, I,
            b_prior_positiva,
            -I,
            d, -sg, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
        /* a_p' = a_p - b_p+ - u  aut  a_p + b_p+ + u */
        a_prior_nova = _nervus_instructio(rete, NERVUS_AFFINIS, a_prior,
            I, b_prior_positiva,
            -sg,
            u, -sg, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
        /* b_p' = b_q + c - c+  aut  b_q - d+ */
        b_prior_nova = _nervus_instructio(rete, NERVUS_AFFINIS,
            b_sequens, I, d, pos,
            d_positiva,
            -I, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
        /* a_q' = [a_q - (b_q - b_q+) - (b_p - b_p+)] + c + w+  aut
         *       [a_q + (b_q - b_q+) + (b_p - b_p+)] + d - w+ */
        pars = _nervus_instructio(rete, NERVUS_AFFINIS, a_sequens, I,
            b_sequens, -sg,
            b_sequens_positiva, sg, b_prior, -sg, b_prior_positiva, sg,
            &bene);
        a_sequens_nova = _nervus_instructio(rete, NERVUS_AFFINIS, pars,
            I, d, I,
            w_positiva,
            sg, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
        /* b_q' = b_p - c + c+  aut  b_p + d+ */
        b_sequens_nova = _nervus_instructio(rete, NERVUS_AFFINIS,
            b_prior, I, d, -pos,
            d_positiva, I, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            &bene);
        cur[p - I]      = a_prior_nova;
        cur[m + p - I]  = b_prior_nova;
        cur[q - I]      = a_sequens_nova;
        cur[m + q - I]  = b_sequens_nova;
    }
    redde bene;
}

b32
plexus_nervus (
      Plexus   p,
     Piscina*  piscina,
      Nervus** exitus)
{
    Piscina* temporaria;
     Nervus* rete;
        s32* cur;
        i32  m;
        i32  k;
        b32  bene = VERUM;

    si (   !plexus_validus(p) || p.fila < III || piscina == NIHIL
        || exitus == NIHIL)
    {
        redde FALSUM;
    }
    m = p.fila - II;
    /* registra exacte: 2m, deinde IV per litteram extremam (sigma_1,
     * sigma_{n-1}), XI per mediam */
    {
        memoriae_index registra = (memoriae_index)II * m;

        per (k = ZEPHYRUM; k < p.longitudo; k++)
        {
            s32 littera = p.litterae[k];
            i32 i = (i32)(littera > ZEPHYRUM ? littera : -littera);

            registra += (i == I || i == p.fila - I) ? IV : XI;
        }
        si (registra > (memoriae_index)NERVUS_REGISTRA_MAXIMA)
        {
            redde FALSUM;
        }
    }
    si (!nervus_creare(II * m, piscina, &rete))
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("plexus_nervus",
        IV * MXXIV);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    cur = (s32*)piscina_allocare(temporaria,
        (memoriae_index)(II * m) * magnitudo(s32));
    per (k = ZEPHYRUM; k < II * m; k++)
    {
        cur[k] = (s32)k;
    }
    per (k = ZEPHYRUM; k < p.longitudo && bene; k++)
    {
        bene = _nervus_littera(rete, p.fila, p.litterae[k], cur);
    }
    si (bene)
    {
        bene = nervus_exitus(rete, II * m, cur);
    }
    piscina_destruere(temporaria);
    si (!bene)
    {
        redde FALSUM;
    }
    *exitus = rete;
    redde VERUM;
}


/* ==================================================
 * Census: clavis classis, periodicitas, enumeratio
 * ================================================== */

/* clavis in piscina data (temporaria vocantis) */
interior b32
_clavis (
      Plexus  p,
     Piscina* piscina,
      chorda* exitus)
{
                i32  fila = p.fila + II;
             Plexus  maior;
  ChordaAedificator* aedificator;
                i32  k;

    si (!plexus_validus(p) || p.fila > PLEXUS_FILA_MAXIMA - II)
    {
        redde FALSUM;
    }
    si (!plexus_in_maiore(p, fila, I, piscina, &maior))
    {
        redde FALSUM;
    }
    aedificator = chorda_aedificator_creare(piscina, LXIV);
    per (k = I; k < fila; k++)
    {
        Ansa e;
        Ansa imago;

        si (   !ansa_circa(fila, k, piscina, &e)
            || !ansa_agere(e, maior, piscina, &imago))
        {
            redde FALSUM;
        }
        (vacuum)chorda_aedificator_appendere_chorda(aedificator,
            ansa_ad_chordam(imago, piscina));
    }
    *exitus = chorda_aedificator_finire(aedificator);
    redde VERUM;
}

b32
plexus_clavis (
      Plexus  p,
     Piscina* piscina,
      chorda* exitus)
{
    Piscina* temporaria;
     chorda  clavis;
        b32  bene;

    si (piscina == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("plexus_clavis",
        IV * MXXIV);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    bene = _clavis(p, temporaria, &clavis);
    si (bene)
    {
        *exitus = chorda_transcribere(clavis, piscina);
    }
    piscina_destruere(temporaria);
    redde bene;
}

b32
plexus_periodus (
      Plexus  p,
     Piscina* piscina,
         i32* periodus)
{
    PiscinaNotatio nota;
               i32 n;
               s64 e;
               s64 torsiones;
               i32 k;
            Plexus torsio;
            Plexus plena;

    si (   !plexus_validus(p) || piscina == NIHIL || periodus == NIHIL
        || p.fila > PLEXUS_FILA_DIMIDIAE_TORSIONIS)
    {
        redde FALSUM;
    }
    n = p.fila;
    e = plexus_summa_exponentium(p);
    /* summa exponentium Delta^2 */
    torsiones  = (s64)n * (s64)(n - I);
    nota       = piscina_notare(piscina);
    si (   !plexus_dimidia_torsio(n, piscina, &torsio)
        || !plexus_iungere(torsio, torsio, piscina, &plena))
    {
        piscina_reficere(piscina, nota);
        redde FALSUM;
    }
    per (k = I; k <= n; k++)
    {
        PiscinaNotatio nota_k;
                   s64 j;
                Plexus sinistra;
                Plexus dextra;
                Plexus basis;
                   b32 aequales  = FALSUM;
                   b32 bene      = VERUM;
                   i32 t;

        si (n % k != ZEPHYRUM && (n - I) % k != ZEPHYRUM)
        {
            perge;
        }
        si (((s64)k * e) % torsiones != ZEPHYRUM)
        {
            perge;
        }
        j       = ((s64)k * e) / torsiones;
        nota_k  = piscina_notare(piscina);
        /* producta refutata (verba nimis longa) -> FALSUM, numquam
         * comparatio truncatorum */
        bene = plexus_creare(n, NIHIL, ZEPHYRUM, piscina, &sinistra);
        per (t = ZEPHYRUM; t < k && bene; t++)
        {
            bene = plexus_iungere(sinistra, p, piscina, &sinistra);
        }
        bene = bene && plexus_creare(n, NIHIL, ZEPHYRUM, piscina,
            &dextra);
        basis = j >= ZEPHYRUM ? plena : plexus_inversum(plena, piscina);
        per (t = ZEPHYRUM; bene
            && (s64)t < (j >= ZEPHYRUM ? j : -j); t++)
        {
            bene = plexus_iungere(dextra, basis, piscina, &dextra);
        }
        si (!bene || !plexus_aequales_ansis(sinistra, dextra, piscina,
            &aequales))
        {
            piscina_reficere(piscina, nota);
            redde FALSUM;
        }
        piscina_reficere(piscina, nota_k);
        si (aequales)
        {
            piscina_reficere(piscina, nota);
            *periodus = k;
            redde VERUM;
        }
    }
    piscina_reficere(piscina, nota);
    *periodus = ZEPHYRUM;
    redde VERUM;
}

/* ordo intra gradum: paria (clavis, index) per claves */
nomen structura {
    chorda clavis;
       i32 index;
} ParClavisPlexus;

interior s32
_comparare_paria (
    constans vacuum* a,
    constans vacuum* b)
{
    redde (s32)chorda_comparare(((constans ParClavisPlexus*)a)->clavis,
        ((constans ParClavisPlexus*)b)->clavis);
}

b32
plexus_enumerare (
             i32  fila,
             i32  longitudo_maxima,
             i32  classes_maximae,
         Piscina* piscina,
    PlexusCensus* exitus)
{
              Piscina* temporaria;
              Piscina* servata;     /* verba et claves usque ad finem */
       TabulaDispersa* visae;
               Plexus* verba;
               chorda* claves;
                  i32* initia;
      ParClavisPlexus* paria;
               Plexus* verba_tmp;
               chorda* claves_tmp;
                  i32  capacitas  = CCLVI;
                  i32  numerus    = ZEPHYRUM;
                  i32  gradus;
                  i32  k;
                  b32  bene = VERUM;
               Plexus  vacuum_verbum;
               chorda  clavis;

    si (   fila < II || fila > PLEXUS_FILA_MAXIMA - II
        || piscina == NIHIL
        || exitus  == NIHIL || longitudo_maxima >= classes_maximae
        || (s64)longitudo_maxima > PLEXUS_LONGITUDO_SUMMA)
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("plexus_enumerare", LXIV
        * MXXIV);
    servata = piscina_generare_dynamicum("plexus_enumerare_servata",
        LXIV * MXXIV);
    si (temporaria == NIHIL || servata == NIHIL)
    {
        si (temporaria != NIHIL)
        {
            piscina_destruere(temporaria);
        }
        si (servata != NIHIL)
        {
            piscina_destruere(servata);
        }
        redde FALSUM;
    }
    visae   = tabula_dispersa_creare_chorda(temporaria, MXXIV);
    verba   = (Plexus*)piscina_allocare(temporaria,
        (memoriae_index)capacitas * magnitudo(Plexus));
    claves  = (chorda*)piscina_allocare(temporaria,
        (memoriae_index)capacitas * magnitudo(chorda));
    initia  = (i32*)piscina_conari_allocare(temporaria,
        (memoriae_index)(longitudo_maxima + II) * magnitudo(i32));
    si (initia == NIHIL)
    {
        piscina_destruere(temporaria);
        piscina_destruere(servata);
        redde FALSUM;
    }
    /* gradus 0: identitas */
    (vacuum)plexus_creare(fila, NIHIL, ZEPHYRUM, servata,
        &vacuum_verbum);
    {
        PiscinaNotatio nota = piscina_notare(temporaria);

        bene = _clavis(vacuum_verbum, temporaria, &clavis);
        si (bene)
        {
            clavis = chorda_transcribere(clavis, servata);
        }
        piscina_reficere(temporaria, nota);
    }
    si (!bene)
    {
        piscina_destruere(temporaria);
        piscina_destruere(servata);
        redde FALSUM;
    }
    verba[ZEPHYRUM]   = vacuum_verbum;
    claves[ZEPHYRUM]  = clavis;
    (vacuum)tabula_dispersa_inserere(visae, clavis, NIHIL);
    numerus           = I;
    initia[ZEPHYRUM]  = ZEPHYRUM;
    per (gradus = I; gradus <= longitudo_maxima && bene; gradus++)
    {
        i32 initium_prius  = initia[gradus - I];
        i32 finis_prius    = numerus;
        i32 i;
        s32 littera;

        initia[gradus] = numerus;
        per (i = initium_prius; i < finis_prius && bene; i++)
        {
            per (littera = -(s32)(fila - I); littera <= (s32)(fila - I)
                && bene; littera++)
            {
                PiscinaNotatio nota;
                        Plexus unum;
                        Plexus verbum;

                si (littera == ZEPHYRUM)
                {
                    perge;
                }
                nota = piscina_notare(temporaria);
                (vacuum)plexus_creare(fila, &littera, I, temporaria,
                    &unum);
                (vacuum)plexus_iungere(verba[i], unum, temporaria,
                    &verbum);
                si (!_clavis(verbum, temporaria, &clavis))
                {
                    bene = FALSUM;
                    piscina_reficere(temporaria, nota);
                    frange;
                }
                si (tabula_dispersa_continet(visae, clavis))
                {
                    piscina_reficere(temporaria, nota);
                    perge;
                }
                /* classis nova: verbum et clavis servantur (vocanti
                 * solum post successum) */
                clavis = chorda_transcribere(clavis, servata);
                (vacuum)plexus_creare(fila, verbum.litterae,
                    verbum.longitudo, servata, &verbum);
                piscina_reficere(temporaria, nota);
                si (numerus >= classes_maximae)
                {
                    bene = FALSUM;
                    frange;
                }
                si (numerus == capacitas)
                {
                    i32 nova = II * capacitas;
                    i32 t;

                    verba_tmp = (Plexus*)piscina_allocare(temporaria,
                        (memoriae_index)nova * magnitudo(Plexus));
                    claves_tmp = (chorda*)piscina_allocare(temporaria,
                        (memoriae_index)nova * magnitudo(chorda));
                    per (t = ZEPHYRUM; t < numerus; t++)
                    {
                        verba_tmp[t]   = verba[t];
                        claves_tmp[t]  = claves[t];
                    }
                    verba      = verba_tmp;
                    claves     = claves_tmp;
                    capacitas  = nova;
                }
                verba[numerus]   = verbum;
                claves[numerus]  = clavis;
                (vacuum)tabula_dispersa_inserere(visae, clavis, NIHIL);
                numerus++;
            }
        }
        /* gradus novus ordine clavium */
        si (bene && numerus > initia[gradus])
        {
            i32  m = numerus - initia[gradus];
            i32  t;
         Plexus* v;
         chorda* c;

            paria = (ParClavisPlexus*)piscina_allocare(temporaria,
                (memoriae_index)m * magnitudo(ParClavisPlexus));
            v = (Plexus*)piscina_allocare(temporaria,
                (memoriae_index)m * magnitudo(Plexus));
            c = (chorda*)piscina_allocare(temporaria,
                (memoriae_index)m * magnitudo(chorda));
            per (t = ZEPHYRUM; t < m; t++)
            {
                paria[t].clavis  = claves[initia[gradus] + t];
                paria[t].index   = initia[gradus] + t;
            }
            qsort(paria, (memoriae_index)m, magnitudo(ParClavisPlexus),
                _comparare_paria);
            per (t = ZEPHYRUM; t < m; t++)
            {
                v[t] = verba[paria[t].index];
                c[t] = claves[paria[t].index];
            }
            per (t = ZEPHYRUM; t < m; t++)
            {
                verba[initia[gradus] + t]   = v[t];
                claves[initia[gradus] + t]  = c[t];
            }
        }
    }
    si (!bene)
    {
        piscina_destruere(temporaria);
        piscina_destruere(servata);
        redde FALSUM;
    }
    initia[longitudo_maxima + I]  = numerus;
    exitus->fila                  = fila;
    exitus->longitudo_maxima      = longitudo_maxima;
    exitus->numerus               = numerus;
    exitus->verba   = (Plexus*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(Plexus));
    exitus->claves  = (chorda*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(chorda));
    exitus->initia  = (i32*)piscina_allocare(piscina,
        (memoriae_index)(longitudo_maxima + II) * magnitudo(i32));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        (vacuum)plexus_creare(fila, verba[k].litterae,
            verba[k].longitudo, piscina, &exitus->verba[k]);
        exitus->claves[k] = chorda_transcribere(claves[k], piscina);
    }
    per (k = ZEPHYRUM; k <= longitudo_maxima + I; k++)
    {
        exitus->initia[k] = initia[k];
    }
    piscina_destruere(temporaria);
    piscina_destruere(servata);
    redde VERUM;
}
