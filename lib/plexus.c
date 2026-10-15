/* plexus.c - Greges braidarum EXACTE (vide include/plexus.h)
 */
#include "plexus.h"
#include "chorda_aedificator.h"
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
    ImaginesLiberae* m)
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
    ImaginesLiberae m;
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
         ImaginesLiberae  m;
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

    si (   fila < II || fila > PLEXUS_FILA_MAXIMA
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
    piscina_destruere(*piscina);
    *piscina = nova;
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
         i32  ab_ultima = ZEPHYRUM;
        Ansa  v;

    si (p.fila != u.fila || u.fila < III)
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
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        _ansam_littera(a, b, u.fila, p.litterae[k], temporaria);
        /* purgamenta: contractio post 256 litteras (aut numerum) si
         * piscina temporaria > 1 MB */
        si (++ab_ultima >= (numerus > CCLVI ? numerus : CCLVI))
        {
            ab_ultima = ZEPHYRUM;
            si (   piscina_summa_usus(temporaria) > 1048576UL
                && !_ansam_contrahere(a, b, numerus, &temporaria))
            {
                piscina_destruere(temporaria);
                redde FALSUM;
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
    piscina_destruere(temporaria);
    *exitus = v;
    redde VERUM;
}

Magnus
ansa_sectiones (
        Ansa  u,
     Piscina* piscina)
{
     Piscina* t = piscina_generare_dynamicum("plexus_sectiones", 4096);
         i32  m = u.fila - II;
         i32  k;
      Magnus  summa_b;
      Magnus  maximum;
      Magnus  b_prima;
      Magnus  b_ultima;
      Magnus  longitudo;

    /* temporaria in piscina propria t (si deficit, in vocantis) */
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
        piscina_destruere(t);
    }
    redde longitudo;
}

b32
ansae_aequales (
    Ansa u,
    Ansa v)
{
    i32 k;

    si (u.fila != v.fila)
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
    PiscinaNotatio nota;
            Plexus r;
            Plexus maior;
               i32 fila = p.fila + II;
               i32 k;
               b32 idem = VERUM;

    si (p.fila != q.fila || fila > PLEXUS_FILA_MAXIMA)
    {
        redde FALSUM;
    }
    nota = piscina_notare(piscina);
    si (   !plexus_iungere(p, plexus_inversum(q, piscina), piscina, &r)
        || !plexus_in_maiore(plexus_reducere(r, piscina), fila, I,
        piscina, &maior))
    {
        piscina_reficere(piscina, nota);
        redde FALSUM;
    }
    per (k = I; idem && k < fila; k++)
    {
        Ansa e;
        Ansa imago;

        si (   !ansa_circa(fila, k, piscina, &e)
            || !ansa_agere(e, maior, piscina, &imago))
        {
            piscina_reficere(piscina, nota);
            redde FALSUM;
        }
        idem = ansae_aequales(e, imago);
    }
    piscina_reficere(piscina, nota);
    *aequales = idem;
    redde VERUM;
}
