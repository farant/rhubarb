/* emendatio.c - Carmen per locum emendatum (vide include/emendatio.h)
 *
 * ORDO: liber vetus iudicatur (carmina_legere) -> nota ad locum in
 * exemplari quaeritur -> signum eius in textu frusti (offset notae)
 * substituitur -> nodus textus in arbore STML mutatur, scriptor
 * non-pulcher -> liber novus TOTUS iudicatur -> custodia: textus
 * frusti relectus idem ac scriptus, notae ceterae intactae (pulsus,
 * longitudo, altitudo), nota ipsa altitudinem novam habet.
 *
 * OCTAVA: octava MML signi ex nota vetere (littera scripta + alteratio:
 * basis naturalis / XII - I, ut cantus 'o4 c' = LX); differentia ad
 * octavam notae novae per '<' / '>' circumdatur et post signum
 * restituitur. Signa contraria adiacentia ('<' ante, '>' post)
 * tolluntur - emendatio iterata ad textum originalem redit.
 */

#include "emendatio.h"
#include "internamentum.h"
#include "partitura.h"
#include "stml.h"
#include <string.h>

/* octava MML: 0..8 (cantus.h) */
#define EMENDATIO_OCTAVA_SUMMA VIII

interior s32
_modulus (
    s32 a,
    s32 m)
{
    redde ((a % m) + m) % m;
}

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior b32
_vitium (
          CarminisVitium* vitium,
                  chorda  carmen,
                  chorda  pars,
                     s32  frustum,
                     i32  offset,
      constans character* causa)
{
    vitium->carmen   = carmen;
    vitium->pars     = pars;
    vitium->frustum  = frustum;
    vitium->offset   = offset;
    vitium->causa    = causa;
    redde FALSUM;
}


/* ==================================================
 * Locus
 * ================================================== */

/* numerus decimalis ad *i: I..IX digiti */
interior b32
_numerum_legere (
    chorda  textus,
       i32* i,
       s64* valor)
{
    i32 digiti = ZEPHYRUM;

    *valor = ZEPHYRUM;
    dum (   *i < textus.mensura && textus.datum[*i] >= '0'
         && textus.datum[*i] <= '9')
    {
        si (++digiti > IX)
        {
            redde FALSUM;
        }
        *valor = *valor * X + (s64)(textus.datum[*i] - '0');
        (*i)++;
    }
    redde digiti > ZEPHYRUM;
}

b32
emendatio_locum_legere (
            chorda  textus,
    EmendatioLocus* locus)
{
    i32 i = ZEPHYRUM;
    s64 mensura;
    s64 tempus;
    s64 pulsus = ZEPHYRUM;

    si (   !_numerum_legere(textus, &i, &mensura)
        || i >= textus.mensura || textus.datum[i] != ':')
    {
        redde FALSUM;
    }
    i++;
    si (!_numerum_legere(textus, &i, &tempus))
    {
        redde FALSUM;
    }
    si (i < textus.mensura && textus.datum[i] == '+')
    {
        i++;
        si (!_numerum_legere(textus, &i, &pulsus))
        {
            redde FALSUM;
        }
    }
    si (i != textus.mensura || mensura < I || tempus < I)
    {
        redde FALSUM;
    }
    locus->mensura  = (i32)mensura;
    locus->tempus   = (i32)tempus;
    locus->pulsus   = pulsus;
    redde VERUM;
}

/* tempus mensurae: 1/denominator; metra composita: tres octavae (ut
 * harmonia.c) */
interior s64
_unitas (
    constans Carmen* carmen,
                s64  initium)
{
    constans CarmenMetrum* m = &carmen->metra[ZEPHYRUM];
                      i32  i;

    per (i = ZEPHYRUM; i < carmen->numerus_metrorum; i++)
    {
        si (carmen->metra[i].pulsus <= initium)
        {
            m = &carmen->metra[i];
        }
    }
    si (   m->denominator     == VIII && m->numerator > III
        && m->numerator % III == ZEPHYRUM)
    {
        redde (s64)III * CANTUS_PULSUS_INTEGRAE / VIII;
    }
    redde (s64)CANTUS_PULSUS_INTEGRAE / (s64)m->denominator;
}


/* ==================================================
 * Arbor STML
 * ================================================== */

interior b32
_elementum_est (
             StmlNodus* nodus,
    constans character* titulus)
{
    redde nodus->genus == STML_NODUS_ELEMENTUM
        && nodus->titulus != NIHIL
        && chorda_aequalis_literis(*nodus->titulus, titulus);
}

/* liberum elementum 'genus' cuius titulus attributum = titulus */
interior StmlNodus*
_liberum_titulatum (
              StmlNodus* nodus,
     constans character* genus,
                 chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(nodus, i);
           chorda* t;

        si (!_elementum_est(l, genus))
        {
            perge;
        }
        t = stml_attributum_capere(l, "titulus");
        si (t != NIHIL && chorda_aequalis(*t, titulus))
        {
            redde l;
        }
    }
    redde NIHIL;
}

/* nodus textus frusti 'index' (ordine documenti = ordine exemplaris);
 * NIHIL si textus non crudus aut non simplex */
interior StmlNodus*
_textum_frusti (
     StmlNodus* radix,
        chorda  carmen,
        chorda  pars,
           s32  index)
{
     StmlNodus* n = radix;
           s32  k = ZEPHYRUM;
           i32  i;

    n = n ? _liberum_titulatum(n, "carmen", carmen) : NIHIL;
    n = n ? _liberum_titulatum(n, "pars", pars) : NIHIL;
    si (n == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(n); i++)
    {
        StmlNodus* f = stml_liberum_ad_indicem(n, i);
              i32  j;

        si (!_elementum_est(f, "frustum") || k++ != index)
        {
            perge;
        }
        per (j = ZEPHYRUM; j < stml_numerus_liberorum(f); j++)
        {
            StmlNodus* t = stml_liberum_ad_indicem(f, j);
            StmlNodus* v;

            si (!_elementum_est(t, "textus"))
            {
                perge;
            }
            si (!t->crudus || stml_numerus_liberorum(t) != I)
            {
                redde NIHIL;
            }
            v = stml_liberum_ad_indicem(t, ZEPHYRUM);
            redde v->genus == STML_NODUS_TEXTUS && v->valor ? v : NIHIL;
        }
    }
    redde NIHIL;
}


/* ==================================================
 * Exemplar
 * ================================================== */

interior constans CarmenPars*
_partem_invenire (
    constans Carmen* carmen,
             chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < carmen->numerus_partium; i++)
    {
        si (chorda_aequalis(carmen->partes[i].titulus, titulus))
        {
            redde &carmen->partes[i];
        }
    }
    redde NIHIL;
}

/* signum notae in textu frusti: littera, alteratio, longitudo,
 * puncta. *alteratio: '#' '+' = I, '-' = -I */
interior i32
_signi_finis (
    chorda  textus,
       i32  offset,
       s32* alteratio)
{
    i32 i = offset + I;

    *alteratio = ZEPHYRUM;
    si (   i < textus.mensura && (   textus.datum[i] == '#'
                               || textus.datum[i] == '+'))
    {
        *alteratio = I;
        i++;
    }
    alioquin si (i < textus.mensura && textus.datum[i] == '-')
    {
        *alteratio = -I;
        i++;
    }
    dum (   i < textus.mensura
         && (   (textus.datum[i] >= '0' && textus.datum[i] <= '9')
             || textus.datum[i] == '.'))
    {
        i++;
    }
    redde i;
}

interior chorda
_textus_novus (
    Piscina* piscina,
     chorda  textus,
        i32  initium,
        i32  finis,
     chorda  medium)
{
    chorda r;
       i32 n = initium + medium.mensura + (textus.mensura - finis);

    r.mensura  = n;
    r.datum    = (i8*)piscina_allocare(piscina, (memoriae_index)n);
    si (r.datum == NIHIL)
    {
        redde _vacua();
    }
    memcpy(r.datum, textus.datum, (size_t)initium);
    memcpy(r.datum + initium, medium.datum, (size_t)medium.mensura);
    memcpy(r.datum + initium + medium.mensura, textus.datum + finis,
           (size_t)(textus.mensura - finis));
    redde r;
}

b32
emendatio_notam_ponere (
                     chorda  liber,
                     chorda  carmen,
                     chorda  pars,
    constans EmendatioLocus* locus,
                     chorda  nota,
                    Piscina* piscina,
                     chorda* novus,
           EmendatioMutatio* mutatio,
             CarminisVitium* vitium)
{
                   Carmina  vetus;
                   Carmina  relectus;
           constans Carmen* k;
       constans CarmenPars* p;
    constans CarmenFrustum* fr               = NIHIL;
       constans CarmenNota* n                = NIHIL;
                       s32  fi               = -I;
                       s32  ni               = -I;
                       s32  tegens           = -I;
                       i32  offset_tegentis  = ZEPHYRUM;
                       s64  initium;
                       s64  finis;
                       s64  unitas;
                       s64  locus_pulsus;
             PartituraNota  pn;
                       i32  altitudo;
                       s32  alteratio_vetus;
                       s32  basis;
                       s32  delta;
                       i32  s0;
                       i32  e0;
                       i32  signi_finis;
                       i32  i;
                       i32  j;
                       i32  l;
                        i8* m;
                    chorda  medium;
                    chorda  textus;
              StmlResultus  r;
                 StmlNodus* nodus;
                    chorda* valor;
                    chorda  scriptus;

    si (!carmina_legere(liber, piscina, &vetus, vitium))
    {
        redde FALSUM;
    }
    k = carmina_invenire(&vetus, carmen);
    si (k == NIHIL)
    {
        redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
            "carmen ignotum");
    }
    p = _partem_invenire(k, pars);
    si (p == NIHIL)
    {
        redde _vitium(vitium, carmen, pars, -I, ZEPHYRUM,
            "pars ignota");
    }

    /* locus -> pulsus */
    si (   locus->mensura < I || locus->tempus < I
        || locus->pulsus < ZEPHYRUM)
    {
        redde _vitium(vitium, carmen, pars, -I, ZEPHYRUM,
            "locus malus");
    }
    initium  = carmen_pulsus_mensurae(k, (i32)locus->mensura);
    finis    = carmen_pulsus_mensurae(k, (i32)locus->mensura + I);
    unitas   = _unitas(k, initium);
    locus_pulsus = initium + (s64)(locus->tempus - I) * unitas
                 + locus->pulsus;
    si (locus->pulsus >= unitas || locus_pulsus >= finis)
    {
        redde _vitium(vitium, carmen, pars, -I, ZEPHYRUM,
            "locus extra mensuram (tempus aut pulsus additi)");
    }

    /* nota quae ibi incipit; aliter quae ibi sonat */
    per (i = ZEPHYRUM; i < p->numerus_frustorum && ni < ZEPHYRUM; i++)
    {
        constans CarmenFrustum* f = &p->frusta[i];

        per (j = ZEPHYRUM; j < f->numerus; j++)
        {
            s64 a = f->pulsus + f->notae[j].pulsus;

            si (a == locus_pulsus)
            {
                fi = (s32)i;
                ni = (s32)j;
                frange;
            }
            si (   a < locus_pulsus
                && locus_pulsus < a + f->notae[j].longitudo)
            {
                tegens           = (s32)i;
                offset_tegentis  = f->notae[j].offset;
            }
        }
    }
    si (ni < ZEPHYRUM && tegens >= ZEPHYRUM)
    {
        redde _vitium(vitium, carmen, pars, tegens, offset_tegentis,
            "nulla nota ad locum incipit (nota prior sonat)");
    }
    si (ni < ZEPHYRUM)
    {
        redde _vitium(vitium, carmen, pars, -I, ZEPHYRUM,
            "ad locum nulla nota (pausa)");
    }
    fr  = &p->frusta[fi];
    n   = &fr->notae[ni];
    si (n->praeparatum.mensura > ZEPHYRUM)
    {
        redde _vitium(vitium, carmen, pars, fi, n->offset,
            "sonitus '@' altitudine caret");
    }

    /* nota nova */
    si (!partitura_notam_legere(nota, &pn, &altitudo))
    {
        redde _vitium(vitium, carmen, pars, fi, n->offset,
            "nota nova mala (\"G2\", \"Bb3\", \"F#4\")");
    }
    si (pn.alteratio > I || pn.alteratio < -I)
    {
        redde _vitium(vitium, carmen, pars, fi, n->offset,
            "MML alterationem unam scribit (# aut b)");
    }
    si (pn.octava < ZEPHYRUM || pn.octava > EMENDATIO_OCTAVA_SUMMA)
    {
        redde _vitium(vitium, carmen, pars, fi, n->offset,
            "octava extra MML (0..8)");
    }

    /* signum vetus et octava eius */
    textus       = fr->textus;
    signi_finis  = _signi_finis(textus, n->offset, &alteratio_vetus);
    basis        = (s32)n->nota - alteratio_vetus;
    delta        = pn.octava
                 - ((basis - _modulus(basis, XII)) / XII - I);
    s0 = n->offset;
    e0 = signi_finis;
    dum (   delta > ZEPHYRUM && s0 > ZEPHYRUM && e0 < textus.mensura
         && textus.datum[s0 - I] == '<' && textus.datum[e0] == '>')
    {
        s0--;
        e0++;
        delta--;
    }
    dum (   delta < ZEPHYRUM && s0 > ZEPHYRUM && e0 < textus.mensura
         && textus.datum[s0 - I] == '>' && textus.datum[e0] == '<')
    {
        s0--;
        e0++;
        delta++;
    }

    /* signum novum: octavae, littera, alteratio, longitudo vetus,
     * octavae restitutae */
    l = (i32)(delta < ZEPHYRUM ? -delta : delta);
    medium.mensura = (i32)(II * l + II
                   + (signi_finis - n->offset));
    m = (i8*)piscina_allocare(piscina, (memoriae_index)medium.mensura);
    si (m == NIHIL)
    {
        redde _vitium(vitium, carmen, pars, fi, n->offset,
            "piscina exhausta");
    }
    medium.datum  = m;
    j             = ZEPHYRUM;
    per (i = ZEPHYRUM; i < l; i++)
    {
        m[j++] = (i8)(delta > ZEPHYRUM ? '>' : '<');
    }
    m[j++] = (i8)(pn.littera - 'A' + 'a');
    si (pn.alteratio != ZEPHYRUM)
    {
        m[j++] = (i8)(pn.alteratio > ZEPHYRUM ? '#' : '-');
    }
    i = n->offset + I + (alteratio_vetus != ZEPHYRUM ? I : ZEPHYRUM);
    per (; i < signi_finis; i++)
    {
        m[j++] = textus.datum[i];
    }
    per (i = ZEPHYRUM; i < l; i++)
    {
        m[j++] = (i8)(delta > ZEPHYRUM ? '<' : '>');
    }
    medium.mensura  = j;
    textus          = _textus_novus(piscina, textus, s0, e0, medium);
    si (textus.datum == NIHIL && textus.mensura == ZEPHYRUM)
    {
        redde _vitium(vitium, carmen, pars, fi, n->offset,
            "piscina exhausta");
    }

    /* arbor: nodus textus mutatur, scriptor non-pulcher */
    r = stml_legere(liber, piscina, internamentum_creare(piscina));
    nodus = r.successus ? _textum_frusti(r.elementum_radix, carmen,
        pars,
                                         fi)
                        : NIHIL;
    si (nodus == NIHIL)
    {
        redde _vitium(vitium, carmen, pars, fi, ZEPHYRUM,
            "textus crudus poscitur ('<textus!>')");
    }
    valor = (chorda*)piscina_allocare(piscina, magnitudo(chorda));
    si (valor == NIHIL)
    {
        redde _vitium(vitium, carmen, pars, fi, n->offset,
            "piscina exhausta");
    }
    *valor        = textus;
    nodus->valor  = valor;
    scriptus      = stml_scribere(r.radix, piscina, FALSUM);

    /* iudicium et custodia */
    si (!carmina_legere(scriptus, piscina, &relectus, vitium))
    {
        redde FALSUM;
    }
    {
            constans Carmen* k2 = carmina_invenire(&relectus, carmen);
        constans CarmenPars* p2 = k2 ? _partem_invenire(k2,
            pars) : NIHIL;
        constans CarmenFrustum* f2 = p2
            && fi < (s32)p2->numerus_frustorum
                                   ? &p2->frusta[fi] : NIHIL;

        si (   f2 == NIHIL || !chorda_aequalis(f2->textus, textus)
            || f2->numerus != fr->numerus)
        {
            redde _vitium(vitium, carmen, pars, fi, n->offset,
                "scriptor textum frusti mutavit");
        }
        per (i = ZEPHYRUM; i < fr->numerus; i++)
        {
            i32 sperata = (s32)i == ni ? altitudo : fr->notae[i].nota;

            si (   f2->notae[i].pulsus    != fr->notae[i].pulsus
                || f2->notae[i].longitudo != fr->notae[i].longitudo
                || f2->notae[i].nota      != sperata)
            {
                redde _vitium(vitium, carmen, pars, fi, n->offset,
                    "notae ceterae mutatae (octava non restituta)");
            }
        }
    }
    *novus                  = scriptus;
    mutatio->frustum        = fi;
    mutatio->offset         = (i32)s0;
    mutatio->vetus.datum    = fr->textus.datum + s0;
    mutatio->vetus.mensura  = e0 - s0;
    mutatio->novum          = medium;
    redde VERUM;
}
