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
#include "chorda_aedificator.h"
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

/* frustum iteratum (<repetitio>): textus nullus, fons in sectione */
#define EMENDATIO_CAUSA_REPETITIONIS \
    "repetitio (frustum iteratum): muta sectionem fontis"

/* nodus textus frusti SCRIPTI 'fi' (index inter frusta scripta, ordo
 * STML - CarmenFrustum.scriptum) textum novum accipit, scriptor
 * non-pulcher
 * (reliqua octetim), liber novus TOTUS iudicatur; custodia: textus
 * frusti relectus = textus scriptus. *relectum = frustum in *relectus;
 * 'offset' vitiis solum */
interior b32
_librum_scribere (
                    chorda   liber,
                    chorda   carmen,
                    chorda   pars,
                       s32   fi,
                       i32   offset,
                    chorda   textus,
                   Piscina*  piscina,
                   Carmina*  relectus,
                    chorda*  scriptus,
    constans CarmenFrustum** relectum,
            CarminisVitium*  vitium)
{
           StmlResultus  r;
              StmlNodus* nodus;
                 chorda* valor;
        constans Carmen* k;
    constans CarmenPars* p;

    r = stml_legere(liber, piscina, internamentum_creare(piscina));
    nodus = r.successus ? _textum_frusti(r.elementum_radix, carmen,
                                         pars, fi)
                        : NIHIL;
    si (nodus == NIHIL)
    {
        redde _vitium(vitium, carmen, pars, fi, ZEPHYRUM,
            "textus crudus poscitur ('<textus!>')");
    }
    valor = (chorda*)piscina_allocare(piscina, magnitudo(chorda));
    si (valor == NIHIL)
    {
        redde _vitium(vitium, carmen, pars, fi, offset,
            "piscina exhausta");
    }
    *valor        = textus;
    nodus->valor  = valor;
    *scriptus     = stml_scribere(r.radix, piscina, FALSUM);
    si (!carmina_legere(*scriptus, piscina, relectus, vitium))
    {
        redde FALSUM;
    }
    k          = carmina_invenire(relectus, carmen);
    p          = k ? _partem_invenire(k, pars) : NIHIL;
    *relectum  = NIHIL;
    {
        i32 j;

        per (j = ZEPHYRUM; p && j < p->numerus_frustorum; j++)
        {
            si (p->frusta[j].scriptum == fi)
            {
                *relectum = &p->frusta[j];
            }
        }
    }
    si (   *relectum == NIHIL || !chorda_aequalis((*relectum)->textus,
                                               textus))
    {
        redde _vitium(vitium, carmen, pars, fi, offset,
            "scriptor textum frusti mutavit");
    }
    redde VERUM;
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
                    chorda  scriptus;
    constans CarmenFrustum* f2;

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
    unitas   = carmen_tempus_mensurae(k, initium);
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
    si (fr->repetitio >= ZEPHYRUM)
    {
        redde _vitium(vitium, carmen, pars, fi, ZEPHYRUM,
            EMENDATIO_CAUSA_REPETITIONIS);
    }
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

    /* arbor, iudicium, custodia */
    si (!_librum_scribere(liber, carmen, pars, fr->scriptum, n->offset,
                          textus, piscina, &relectus, &scriptus, &f2,
                          vitium))
    {
        redde FALSUM;
    }
    si (f2->numerus != fr->numerus)
    {
        redde _vitium(vitium, carmen, pars, fi, n->offset,
            "notae ceterae mutatae (octava non restituta)");
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
    *novus                  = scriptus;
    mutatio->frustum        = fi;
    mutatio->offset         = (i32)s0;
    mutatio->vetus.datum    = fr->textus.datum + s0;
    mutatio->vetus.mensura  = e0 - s0;
    mutatio->novum          = medium;
    redde VERUM;
}


/* ==================================================
 * Mensura tota (E2)
 * ================================================== */

interior b32
_spatium (
    i8 s)
{
    redde s == ' ' || s == '\t' || s == '\n' || s == '\r';
}

/* offset lineae '|' ad pulsum (intra frustum), ab offset 'ab':
 * ultima aut prima; -I si nulla */
interior s32
_linea (
    constans CarmenFrustum* f,
                       s64  pulsus,
                       b32  ultima,
                       i32  ab)
{
    s32 inventa = -I;
    i32 i;

    per (i = ZEPHYRUM; i < f->numerus_linearum; i++)
    {
        si (f->lineae[i].pulsus == pulsus && f->lineae[i].offset >= ab)
        {
            inventa = (s32)f->lineae[i].offset;
            si (!ultima)
            {
                frange;
            }
        }
    }
    redde inventa;
}

/* longitudo MML ('l'): 1..64 cum punctis 0..2 - ordinaria semper ex
 * mensura scripta, ergo semper repraesentabilis */
interior b32
_longitudinem_scribere (
                   i32  pulsus,
     ChordaAedificator* a)
{
    i32 v;
    i32 p;

    per (v = I; v <= LXIV; v *= II)
    {
        i32 basis  = (i32)CANTUS_PULSUS_INTEGRAE / v;
        i32 l      = basis;
        i32 adde   = basis / II;

        per (p = ZEPHYRUM; p <= II; p++)
        {
            si (l == pulsus)
            {
                redde chorda_aedificator_appendere_i32(a, v)
                    && chorda_aedificator_appendere_repetita(a, '.',
                                                             p);
            }
            l     += adde;
            adde  /= II;
        }
    }
    redde FALSUM;
}

b32
emendatio_mensuram_invenire (
     constans Carmen* carmen,
              chorda  pars,
                 i32  mensura,
    EmendatioMensura* inventa,
      CarminisVitium* vitium)
{
       constans CarmenPars* p = _partem_invenire(carmen, pars);
    constans CarmenFrustum* f;
                       s64  b0;
                       s64  b1;
                       s32  fi = -I;
                       s32  l0;
                       s32  l1;
                       i32  initium;
                       i32  finis;
                       i32  i;
              CantusVitium  cv;

    si (p == NIHIL)
    {
        redde _vitium(vitium, carmen->titulus, pars, -I, ZEPHYRUM,
            "pars ignota");
    }
    si (mensura < I)
    {
        redde _vitium(vitium, carmen->titulus, pars, -I, ZEPHYRUM,
            "mensura nulla (1 = prima)");
    }
    b0 = carmen_pulsus_mensurae(carmen, mensura);
    b1 = carmen_pulsus_mensurae(carmen, mensura + I);
    per (i = ZEPHYRUM; i < p->numerus_frustorum; i++)
    {
        f = &p->frusta[i];
        si (f->pulsus > b0 && f->pulsus < b1)
        {
            redde _vitium(vitium, carmen->titulus, pars, (s32)i,
                ZEPHYRUM, "frustum intra mensuram incipit");
        }
        si (f->pulsus <= b0 && b0 < f->pulsus + f->longitudo)
        {
            fi = (s32)i;
            frange;
        }
    }
    si (fi < ZEPHYRUM)
    {
        redde _vitium(vitium, carmen->titulus, pars, -I, ZEPHYRUM,
            "mensura sine frusto (silentium)");
    }
    f = &p->frusta[fi];
    si (f->repetitio >= ZEPHYRUM)
    {
        redde _vitium(vitium, carmen->titulus, pars, fi, ZEPHYRUM,
            EMENDATIO_CAUSA_REPETITIONIS);
    }
    si (b1 > f->pulsus + f->longitudo)
    {
        redde _vitium(vitium, carmen->titulus, pars, fi, ZEPHYRUM,
            "mensura non intra frustum unum");
    }
    l0 = _linea(f, b0 - f->pulsus, VERUM, ZEPHYRUM);
    si (l0 < ZEPHYRUM && b0 != f->pulsus)
    {
        redde _vitium(vitium, carmen->titulus, pars, fi, ZEPHYRUM,
            "linea '|' ad initium mensurae non scripta");
    }
    initium  = l0 < ZEPHYRUM ? ZEPHYRUM : (i32)l0 + I;
    l1       = _linea(f, b1 - f->pulsus, FALSUM, initium);
    si (l1 < ZEPHYRUM && b1 != f->pulsus + f->longitudo)
    {
        redde _vitium(vitium, carmen->titulus, pars, fi, initium,
            "linea '|' ad finem mensurae non scripta");
    }
    finis = l1 < ZEPHYRUM ? f->textus.mensura : (i32)l1;
    dum (initium < finis && _spatium(f->textus.datum[initium]))
    {
        initium++;
    }
    dum (finis > initium && _spatium(f->textus.datum[finis - I]))
    {
        finis--;
    }
    si (!cantus_status_ad(f->textus, initium, &inventa->status, &cv))
    {
        redde _vitium(vitium, carmen->titulus, pars, fi, cv.offset,
            cv.causa);
    }
    inventa->frustum         = fi;
    inventa->initium         = initium;
    inventa->finis           = finis;
    inventa->textus.datum    = f->textus.datum + initium;
    inventa->textus.mensura  = finis - initium;
    redde VERUM;
}

/* notae extra mensuram [r0, r1) (pulsus intra frustum) eaedem in
 * frusto novo, ordine; lineae eaedem */
interior b32
_extra_eaedem (
    constans CarmenFrustum* a,
    constans CarmenFrustum* b,
                       s64  r0,
                       s64  r1)
{
    i32 i;
    i32 j = ZEPHYRUM;

    si (a->numerus_linearum != b->numerus_linearum)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < a->numerus_linearum; i++)
    {
        si (a->lineae[i].pulsus != b->lineae[i].pulsus)
        {
            redde FALSUM;
        }
    }
    i = ZEPHYRUM;
    dum (VERUM)
    {
        constans CarmenNota* x;
        constans CarmenNota* y;

        dum (   i < a->numerus && a->notae[i].pulsus >= r0
             && a->notae[i].pulsus < r1)
        {
            i++;
        }
        dum (   j < b->numerus && b->notae[j].pulsus >= r0
             && b->notae[j].pulsus < r1)
        {
            j++;
        }
        si (i == a->numerus || j == b->numerus)
        {
            redde i == a->numerus && j == b->numerus;
        }
        x = &a->notae[i++];
        y = &b->notae[j++];
        si (   x->pulsus != y->pulsus || x->longitudo != y->longitudo
            || x->nota   != y->nota || x->lucrum != y->lucrum
            || !chorda_aequalis(x->praeparatum, y->praeparatum))
        {
            redde FALSUM;
        }
    }
}

b32
emendatio_mensuram_substituere (
               chorda  liber,
               chorda  carmen,
               chorda  pars,
                  i32  mensura,
               chorda  textus,
              Piscina* piscina,
               chorda* novus,
     EmendatioMutatio* mutatio,
       CarminisVitium* vitium)
{
                   Carmina  vetus;
                   Carmina  relectus;
           constans Carmen* k;
    constans CarmenFrustum* fr;
    constans CarmenFrustum* f2;
          EmendatioMensura  m;
                    chorda  nova = textus;
                    chorda  candidatus;
                    chorda  medium;
                    chorda  scriptus;
              CantusStatus  vetus_finis;
              CantusStatus  novus_finis;
              CantusVitium  cv;
                 Cantilena  cn;
         ChordaAedificator* a;
                       s64  r0;
                       s64  r1;
                       b32  desinit;
                       i32  i;

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
    si (!emendatio_mensuram_invenire(k, pars, mensura, &m, vitium))
    {
        redde FALSUM;
    }
    fr = &_partem_invenire(k, pars)->frusta[m.frustum];
    r0 = carmen_pulsus_mensurae(k, mensura) - fr->pulsus;
    r1 = carmen_pulsus_mensurae(k, mensura + I) - fr->pulsus;

    /* textus novus: spatia orae abscissa, sine '|' */
    dum (nova.mensura > ZEPHYRUM && _spatium(nova.datum[ZEPHYRUM]))
    {
        nova.datum++;
        nova.mensura--;
    }
    dum (   nova.mensura > ZEPHYRUM
         && _spatium(nova.datum[nova.mensura - I]))
    {
        nova.mensura--;
    }
    per (i = ZEPHYRUM; i < nova.mensura; i++)
    {
        si (nova.datum[i] == '|')
        {
            redde _vitium(vitium, carmen, pars, m.frustum, m.initium,
                "textus novus sine '|'");
        }
    }

    /* status: finis veteris, finis novi (in statu initii lecti) */
    (vacuum)cantus_status_ad(fr->textus, m.finis, &vetus_finis, &cv);
    candidatus = _textus_novus(piscina, fr->textus, m.initium, m.finis,
                               nova);
    si (!cantus_status_ad(candidatus, m.initium + nova.mensura,
                          &novus_finis, &cv))
    {
        redde _vitium(vitium, carmen, pars, m.frustum, cv.offset,
            cv.causa);
    }

    /* textus novus + restitutio (nihil si frustum hic desinit) */
    desinit = r1 == fr->longitudo;
    a = chorda_aedificator_creare(piscina,
        (memoriae_index)(nova.mensura + LXIV));
    si (a == NIHIL || !chorda_aedificator_appendere_chorda(a, nova))
    {
        redde _vitium(vitium, carmen, pars, m.frustum, m.initium,
            "piscina exhausta");
    }
    si (!desinit && novus_finis.octava != vetus_finis.octava)
    {
        si (   vetus_finis.octava >= ZEPHYRUM
            && vetus_finis.octava <= EMENDATIO_OCTAVA_SUMMA)
        {
            (vacuum)chorda_aedificator_appendere_literis(a, " o");
            (vacuum)chorda_aedificator_appendere_s32(a,
                vetus_finis.octava);
        }
        alioquin
        {
            s32 d = vetus_finis.octava - novus_finis.octava;

            (vacuum)chorda_aedificator_appendere_character(a, ' ');
            (vacuum)chorda_aedificator_appendere_repetita(a,
                d > ZEPHYRUM ? '>' : '<',
                (i32)(d > ZEPHYRUM ? d : -d));
        }
    }
    si (!desinit && novus_finis.ordinaria != vetus_finis.ordinaria)
    {
        (vacuum)chorda_aedificator_appendere_literis(a, " l");
        (vacuum)_longitudinem_scribere(vetus_finis.ordinaria, a);
    }
    si (!desinit && novus_finis.lucrum != vetus_finis.lucrum)
    {
        (vacuum)chorda_aedificator_appendere_literis(a, " v");
        (vacuum)chorda_aedificator_appendere_i32(a, vetus_finis.lucrum);
    }
    si (   !desinit && !chorda_aequalis(novus_finis.praeparatum,
                                     vetus_finis.praeparatum))
    {
        si (vetus_finis.praeparatum.mensura == ZEPHYRUM)
        {
            redde _vitium(vitium, carmen, pars, m.frustum, m.initium,
                "'@' novum notas sequentes mutaret (MML '@' revocare "
                "nequit)");
        }
        (vacuum)chorda_aedificator_appendere_literis(a, " @");
        (vacuum)chorda_aedificator_appendere_chorda(a,
            vetus_finis.praeparatum);
    }
    medium      = chorda_aedificator_finire(a);
    candidatus  = _textus_novus(piscina, fr->textus, m.initium, m.finis,
                                medium);

    /* longitudo: frustum totum idem */
    si (!cantus_legere(candidatus, piscina, &cn, &cv))
    {
        redde _vitium(vitium, carmen, pars, m.frustum, cv.offset,
            cv.causa);
    }
    si (cn.pulsus != fr->longitudo)
    {
        redde _vitium(vitium, carmen, pars, m.frustum, m.initium,
            "longitudo textus novi mensuram non aequat");
    }

    /* arbor, iudicium, custodia */
    si (!_librum_scribere(liber, carmen, pars, fr->scriptum, m.initium,
                          candidatus, piscina, &relectus, &scriptus,
                          &f2, vitium))
    {
        redde FALSUM;
    }
    si (!_extra_eaedem(fr, f2, r0, r1))
    {
        redde _vitium(vitium, carmen, pars, m.frustum, m.initium,
            "notae extra mensuram mutatae (status non restitutus)");
    }
    *novus            = scriptus;
    mutatio->frustum  = m.frustum;
    mutatio->offset   = m.initium;
    mutatio->vetus    = m.textus;
    mutatio->novum    = medium;
    redde VERUM;
}


/* ==================================================
 * Copia et transpositio (palette #4)
 * ================================================== */

/* ordo diesium in signatura: F C G D A E B (indices C = 0) */
hic_manens constans s32 ORDO_DIESIUM[VII] = {
    III, ZEPHYRUM, IV, I, V, II, VI
};
/* semitonia litterarum C D E F G A B */
hic_manens constans s32 SEMITONIA_LITTERARUM[VII] = {
    ZEPHYRUM, II, IV, V, VII, IX, XI
};
/* pausae: valores maximi primum (punctati simplices) */
hic_manens constans s64 PAUSAE[XIV] = {
    5760, 3840, 2880, 1920, 1440, 960, 720, 480, 360, 240, 180, 120, 90,
    60
};

/* alteratio litterae in signatura (quintae -7..7) */
interior s32
_signatura (
    s32 quintae,
    s32 littera)
{
    s32 i;

    per (i = ZEPHYRUM; i < quintae; i++)
    {
        si (ORDO_DIESIUM[i] == littera)
        {
            redde I;
        }
    }
    per (i = ZEPHYRUM; i < -quintae; i++)
    {
        si (ORDO_DIESIUM[VI - i] == littera)
        {
            redde -I;
        }
    }
    redde ZEPHYRUM;
}

/* clavis ad pulsum (prima si nulla prior) */
interior constans CarmenClavis*
_clavis (
    constans Carmen* carmen,
                s64  pulsus)
{
    constans CarmenClavis* c = NIHIL;
                      i32  i;

    per (i = ZEPHYRUM; i < carmen->numerus_clavium; i++)
    {
        si (c == NIHIL || carmen->claves[i].pulsus <= pulsus)
        {
            c = &carmen->claves[i];
        }
    }
    redde c;
}

/* index litterae tonicae (C = 0 .. B = 6) */
interior s32
_finalis_littera (
    constans CarmenClavis* c)
{
    character t = c
        != NIHIL ? partitura_clavis_littera(c)[ZEPHYRUM] : 'c';

    redde _modulus((s32)(t - 'a') + V, VII);
}

interior s32
_quintae (
    constans CarmenClavis* c)
{
    redde c != NIHIL ? c->quintae : ZEPHYRUM;
}

/* pausas [a, a + spatium) scribere; FALSUM si non expressibilis */
interior b32
_pausas_scribere (
    ChordaAedificator* a,
                  s64  spatium,
                  b32* primum)
{
    i32 k;

    per (k = ZEPHYRUM; k < XIV && spatium > ZEPHYRUM; k++)
    {
        dum (spatium >= PAUSAE[k])
        {
            si (!*primum)
            {
                (vacuum)chorda_aedificator_appendere_character(a, ' ');
            }
            *primum = FALSUM;
            (vacuum)chorda_aedificator_appendere_character(a, 'r');
            (vacuum)_longitudinem_scribere((i32)PAUSAE[k], a);
            spatium -= PAUSAE[k];
        }
    }
    redde spatium == ZEPHYRUM;
}

b32
emendatio_mensuram_scribere (
          constans Carmen* carmen,
                   chorda  pars,
                      i32  mensura,
    EmendatioTranspositio  modus,
                      s32  intervallum,
                      s64  pulsus_metae,
        ChordaAedificator* textus,
           CarminisVitium* vitium)
{
      constans CarmenPars* p = _partem_invenire(carmen, pars);
    constans CarmenClavis* fons;
    constans CarmenClavis* meta;
                      s64  b0;
                      s64  b1;
                      s64  cursor;
                      s32  octava = -C;
                      f32  lucrum = 1.0f;
                      b32  primum = VERUM;
                      i32  f;
                      i32  j;

    si (p == NIHIL)
    {
        redde _vitium(vitium, carmen->titulus, pars, -I, ZEPHYRUM,
            "pars ignota");
    }
    si (mensura < I)
    {
        redde _vitium(vitium, carmen->titulus, pars, -I, ZEPHYRUM,
            "mensura nulla (1 = prima)");
    }
    b0      = carmen_pulsus_mensurae(carmen, mensura);
    b1      = carmen_pulsus_mensurae(carmen, mensura + I);
    fons    = _clavis(carmen, b0);
    meta    = _clavis(carmen, pulsus_metae);
    cursor  = b0;
    per (f = ZEPHYRUM; f < p->numerus_frustorum; f++)
    {
        constans CarmenFrustum* fr = &p->frusta[f];

        si (   fr->repetitio >= ZEPHYRUM && fr->pulsus < b1
            && b0 < fr->pulsus + fr->longitudo)
        {
            redde _vitium(vitium, carmen->titulus, pars, (s32)f,
                ZEPHYRUM, EMENDATIO_CAUSA_REPETITIONIS);
        }

        per (j = ZEPHYRUM; j < fr->numerus; j++)
        {
            constans CarmenNota* n = &fr->notae[j];
                            s64  a = fr->pulsus + n->pulsus;
                            s64  e = a + n->longitudo;
                            s32  littera;
                            s32  alteratio;
                            s32  oct;
                            s32  altitudo;
                            s32  centesimae;

            si (e <= b0 || a >= b1)
            {
                perge;
            }
            si (a < b0 || e > b1)
            {
                redde _vitium(vitium, carmen->titulus, pars, (s32)f,
                    n->offset, "nota trans lineam mensurae");
            }
            si (n->praeparatum.mensura > ZEPHYRUM)
            {
                redde _vitium(vitium, carmen->titulus, pars, (s32)f,
                    n->offset, "sonitus '@' non transponitur");
            }
            si (!_pausas_scribere(textus, a - cursor, &primum))
            {
                redde _vitium(vitium, carmen->titulus, pars, (s32)f,
                    n->offset, "longitudo non expressibilis");
            }
            si (modus == EMENDATIO_SEMITONIA)
            {
                   CarmenNota t = *n;
                PartituraNota o;

                t.nota = (i32)((s32)n->nota + intervallum);
                t.littera = '\0';
                o = partitura_orthographia(carmen, &t, pulsus_metae);
                littera = _modulus((s32)(o.littera - 'A') + V, VII);
                alteratio = o.alteratio;
                oct = o.octava;
            }
            alioquin
            {
                PartituraNota o = partitura_orthographia(carmen, n, a);
                          s32 l = _modulus((s32)(o.littera - 'A') + V,
                                           VII);
                          s32 relativa = o.alteratio
                                       - _signatura(_quintae(fons), l);
                          s32 dt = _finalis_littera(meta)
                                 - _finalis_littera(fons);
                          s32 d;

                dt         = dt > III ? dt - VII
                                      : (dt < -III ? dt + VII : dt);
                d        = o.octava * VII + l + dt + intervallum;
                littera  = _modulus(d, VII);
                oct      = (d - littera) / VII;
                alteratio  = _signatura(_quintae(meta), littera)
                           + relativa;
            }
            si (alteratio > I || alteratio < -I)
            {
                redde _vitium(vitium, carmen->titulus, pars, (s32)f,
                    n->offset,
                    "MML alterationem unam scribit (# aut b)");
            }
            altitudo = (oct + I) * XII + SEMITONIA_LITTERARUM[littera]
                     + alteratio;
            si (   oct < ZEPHYRUM || oct > EMENDATIO_OCTAVA_SUMMA
                || altitudo < ZEPHYRUM || altitudo > CXXVII)
            {
                redde _vitium(vitium, carmen->titulus, pars, (s32)f,
                    n->offset, "octava extra MML (0..8)");
            }
            si (oct != octava)
            {
                si (!primum)
                {
                    (vacuum)chorda_aedificator_appendere_character(
                        textus, ' ');
                }
                primum = FALSUM;
                (vacuum)chorda_aedificator_appendere_character(textus,
                    'o');
                (vacuum)chorda_aedificator_appendere_s32(textus, oct);
                octava = oct;
            }
            si (n->lucrum != lucrum)
            {
                centesimae = (s32)(n->lucrum * (f32)C + 0.5f);
                (vacuum)chorda_aedificator_appendere_literis(textus,
                    primum ? "v" : " v");
                (vacuum)chorda_aedificator_appendere_s32(textus,
                    centesimae);
                primum = FALSUM;
                lucrum = n->lucrum;
            }
            si (!primum)
            {
                (vacuum)chorda_aedificator_appendere_character(textus,
                    ' ');
            }
            primum = FALSUM;
            (vacuum)chorda_aedificator_appendere_character(textus,
                (character)("cdefgab"[littera]));
            si (alteratio != ZEPHYRUM)
            {
                (vacuum)chorda_aedificator_appendere_character(textus,
                    alteratio > ZEPHYRUM ? '#' : '-');
            }
            si (!_longitudinem_scribere((i32)n->longitudo, textus))
            {
                redde _vitium(vitium, carmen->titulus, pars, (s32)f,
                    n->offset, "longitudo non expressibilis");
            }
            cursor = e;
        }
    }
    si (!_pausas_scribere(textus, b1 - cursor, &primum))
    {
        redde _vitium(vitium, carmen->titulus, pars, -I, ZEPHYRUM,
            "longitudo non expressibilis");
    }
    redde VERUM;
}

/* mensurae carminis (ultima quae ante finem incipit) */
interior i32
_mensurae (
    constans Carmen* k)
{
    i32 m = I;

    dum (carmen_pulsus_mensurae(k, m + I) < k->pulsus)
    {
        m++;
    }
    redde m;
}

b32
emendatio_mensuras_copiare (
                     chorda  liber,
                     chorda  carmen,
    constans EmendatioCopia* copia,
                    Piscina* piscina,
                     chorda* novus,
                        i32* mutatae,
             CarminisVitium* vitium)
{
             Carmina  vetus;
     constans Carmen* k;
              chorda  meta = copia->pars_meta.mensura > ZEPHYRUM
                           ? copia->pars_meta : copia->pars_fons;
              chorda* textus;
                 i32* metae;
                 i32  n;
                 i32  r;
                 i32  i;
                 i32  numerus;
                 i32  repetitiones = copia->repetitiones > ZEPHYRUM
                                   ? copia->repetitiones : I;
              chorda liber_currens = liber;

    *mutatae = ZEPHYRUM;
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
    si (   copia->prima < I || copia->ultima < copia->prima
        || copia->ultima > _mensurae(k))
    {
        redde _vitium(vitium, carmen, copia->pars_fons, -I, ZEPHYRUM,
            "mensurae fontis extra carmen");
    }
    n        = copia->ultima - copia->prima + I;
    numerus  = n * repetitiones;
    textus   = (chorda*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(chorda));
    metae    = (i32*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(i32));
    si (textus == NIHIL || metae == NIHIL)
    {
        redde _vitium(vitium, carmen, meta, -I, ZEPHYRUM,
            "piscina exhausta");
    }
    /* textus omnes ex libro ORIGINALI */
    per (r = ZEPHYRUM; r < repetitiones; r++)
    {
        per (i = ZEPHYRUM; i < n; i++)
        {
                          i32  x = r * n + i;
            ChordaAedificator* a = chorda_aedificator_creare(piscina,
                CCLVI);

            metae[x] = copia->meta + x;
            si (   a == NIHIL
                || !emendatio_mensuram_scribere(k, copia->pars_fons,
                       copia->prima + i, copia->modus,
                       copia->intervallum * (s32)(r + I),
                       carmen_pulsus_mensurae(k, metae[x]), a, vitium))
            {
                redde FALSUM;
            }
            textus[x] = chorda_aedificator_finire(a);
        }
    }
    /* substitutiones ordine; meta in statu '@' recusatur */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
                 Carmina  currens;
         constans Carmen* currentis;
        EmendatioMensura  m;
                  chorda  proximus;
        EmendatioMutatio  mutatio;

        si (!carmina_legere(liber_currens, piscina, &currens, vitium))
        {
            redde FALSUM;
        }
        currentis = carmina_invenire(&currens, carmen);
        si (!emendatio_mensuram_invenire(currentis, meta, metae[i], &m,
            vitium))
        {
            redde FALSUM;
        }
        si (m.status.praeparatum.mensura > ZEPHYRUM)
        {
            redde _vitium(vitium, carmen, meta, m.frustum, m.initium,
                "meta in statu '@' (sonitus)");
        }
        si (!emendatio_mensuram_substituere(liber_currens, carmen, meta,
                metae[i], textus[i], piscina, &proximus, &mutatio,
                vitium))
        {
            redde FALSUM;
        }
        liber_currens = proximus;
    }
    *novus    = liber_currens;
    *mutatae  = numerus;
    redde VERUM;
}
