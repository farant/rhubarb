/* carmen.c - Carmen (vide include/carmen.h)
 *
 * ORDO LECTIONIS carminis: attributa (tempus, metrum, clavis
 * initialia), deinde mutationes metri ORDINE (positio per mensuram ex
 * mappa metri iam lecta), deinde tempus et clavis (mappa metri plena),
 * deinde partes. Vitium ubicumque = liber vacuus.
 */
#include "carmen.h"
#include "xar.h"
#include "carmina_infixum.h"
#include "internamentum.h"
#include "stml.h"
#include <string.h>

#define CARMEN_TEMPUS_ORDINARIUM CXX


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
_vacua (
    vacuum)
{
    chorda c;

    c.mensura  = ZEPHYRUM;
    c.datum    = NIHIL;
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

/* [a-z0-9_]+ */
interior b32
_signum_validum (
    chorda s)
{
    i32 i;

    si (s.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        character x = (character)s.datum[i];

        si (!(   (x >= 'a' && x <= 'z') || (x >= '0' && x <= '9')
              || x == '_'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_elementum_est (
             StmlNodus* nodus,
    constans character* titulus)
{
    redde nodus->genus == STML_NODUS_ELEMENTUM
        && nodus->titulus != NIHIL
        && chorda_aequalis_literis(*nodus->titulus, titulus);
}

/* numerus integer attributi; FALSUM si absens aut non numerus */
interior b32
_s64_legere (
             StmlNodus* nodus,
    constans character* titulus,
                   s64* valor)
{
    chorda* a = stml_attributum_capere(nodus, titulus);

    redde a && chorda_ut_s64(*a, valor);
}

/* "N/D": N 1..64, D potentia II 1..64 */
interior b32
_metrum_legere (
          chorda  valor,
    CarmenMetrum* metrum)
{
    chorda n;
    chorda d;
       s32 numerator;
       s32 denominator;
       i32 i;

    per (i = ZEPHYRUM; i < valor.mensura && valor.datum[i] != '/'; i++)
    {
    }
    si (i == valor.mensura)
    {
        redde FALSUM;
    }
    n.datum    = valor.datum;
    n.mensura  = i;
    d.datum    = valor.datum + i + I;
    d.mensura  = valor.mensura - i - I;
    si (   !chorda_ut_s32(n, &numerator)
        || !chorda_ut_s32(d, &denominator)
        || numerator < I || numerator > LXIV || denominator < I
        || denominator > LXIV || (denominator & (denominator - I)) != 0)
    {
        redde FALSUM;
    }
    metrum->numerator    = (i32)numerator;
    metrum->denominator  = (i32)denominator;
    redde VERUM;
}

/* "a minor", "f# maior", "bb minor" -> quintae (circulus quintarum,
 * ut MusicXML 'fifths'): maior ex littera, '#' +VII, 'b' -VII, minor
 * -III; extra -VII..VII vitium */
interior b32
_clavem_legere (
          chorda  valor,
    CarmenClavis* clavis)
{
    hic_manens constans s32 quintae_litterarum[VII] = {
        III, V, ZEPHYRUM, II, IV, -I, I };   /* a b c d e f g */
       s32 q;
       i32 i = I;
    chorda modus;

    si (   valor.mensura < III || valor.datum[ZEPHYRUM] < 'a'
        || valor.datum[ZEPHYRUM] > 'g')
    {
        redde FALSUM;
    }
    q = quintae_litterarum[valor.datum[ZEPHYRUM] - 'a'];
    si (valor.datum[i] == '#')
    {
        q += VII;
        i++;
    }
    alioquin si (valor.datum[i] == 'b')
    {
        q -= VII;
        i++;
    }
    si (i >= valor.mensura || valor.datum[i] != ' ')
    {
        redde FALSUM;
    }
    modus.datum    = valor.datum + i + I;
    modus.mensura  = valor.mensura - i - I;
    si (chorda_aequalis_literis(modus, "minor"))
    {
        q              -= III;
        clavis->minor  = VERUM;
    }
    alioquin si (chorda_aequalis_literis(modus, "maior"))
    {
        clavis->minor = FALSUM;
    }
    alioquin
    {
        redde FALSUM;
    }
    si (q < -VII || q > VII)
    {
        redde FALSUM;
    }
    clavis->quintae = q;
    redde VERUM;
}

interior b32
_tempus_legere (
    chorda  valor,
       i32* tempus)
{
    s32 t;

    si (!chorda_ut_s32(valor, &t) || t < XX || t > CD)
    {
        redde FALSUM;
    }
    *tempus = (i32)t;
    redde VERUM;
}

/* pulsus mensurae metri dati */
interior s64
_longitudo_mensurae (
    constans CarmenMetrum* m)
{
    redde (s64)CANTUS_PULSUS_INTEGRAE * (s64)m->numerator
        / (s64)m->denominator;
}

/* initium mensurae ex metris [0, numerus) */
interior s64
_pulsus_mensurae (
    constans CarmenMetrum* metra,
                      i32  numerus,
                      i32  mensura)
{
    s64 pulsus = ZEPHYRUM;
    s64 numeri = I;   /* mensura cuius initium 'pulsus' est */
    i32 k;

    si (mensura <= I || numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        s64 l = _longitudo_mensurae(&metra[k]);
        s64 n;

        si (k + I < numerus)
        {
            n = (metra[k + I].pulsus - metra[k].pulsus) / l;
            si (numeri + n > (s64)mensura)
            {
                redde pulsus + ((s64)mensura - numeri) * l;
            }
            pulsus = metra[k + I].pulsus;
            numeri += n;
        }
        alioquin
        {
            redde pulsus + ((s64)mensura - numeri) * l;
        }
    }
    redde pulsus;
}

/* positio elementi: mensura="N" (>= 1) aut pulsus="N" (>= 0), altera
 * sola. FALSUM: causa in *causa */
interior b32
_positionem_legere (
             StmlNodus*  nodus,
       constans Carmen*  carmen,
                   b32   pulsus_admissus,
                   s64*  positio,
    constans character** causa)
{
    b32 m = stml_attributum_habet(nodus, "mensura");
    b32 p = stml_attributum_habet(nodus, "pulsus");
    b32 t = stml_attributum_habet(nodus, "ictus");
    s64 v;

    si (m == p || ((p || t) && !pulsus_admissus))
    {
        *causa =
            pulsus_admissus ? "positio: mensura aut pulsus, altera sola"
                                 : "mutatio metri per mensuram sola";
        redde FALSUM;
    }
    si (t && !m)
    {
        *causa = "ictus cum mensura solum";
        redde FALSUM;
    }
    si (m)
    {
        s64 x;

        si (   !_s64_legere(nodus, "mensura", &v) || v < I
            || v > CMXCIX * M)
        {
            *causa = "mensura non numerus >= 1";
            redde FALSUM;
        }
        *positio = _pulsus_mensurae(carmen->metra,
            carmen->numerus_metrorum,
            (i32)v);
        si (t)
        {
            s64 ictus     = carmen_tempus_mensurae(carmen, *positio);
            s64 longitudo = _pulsus_mensurae(carmen->metra,
                carmen->numerus_metrorum, (i32)v + I) - *positio;

            si (   !_s64_legere(nodus, "ictus", &x) || x < I
                || (x - I) * ictus >= longitudo)
            {
                *causa = "ictus: 1..tempora mensurae";
                redde FALSUM;
            }
            *positio += (x - I) * ictus;
        }
        redde VERUM;
    }
    si (!_s64_legere(nodus, "pulsus", &v) || v < ZEPHYRUM)
    {
        *causa = "pulsus non numerus >= 0";
        redde FALSUM;
    }
    *positio = v;
    redde VERUM;
}

/* offset primi 't' in MML (extra '@titulus'); -1 si nullum */
interior s32
_tempus_in_textu (
    chorda textus)
{
    i32 i = ZEPHYRUM;

    dum (i < textus.mensura)
    {
        character x = (character)textus.datum[i];

        si (x == '@')
        {
            dum (i < textus.mensura && textus.datum[i] != ' ')
            {
                i++;
            }
            perge;
        }
        si (x == 't' || x == 'T')
        {
            redde (s32)i;
        }
        i++;
    }
    redde -I;
}


/* ==================================================
 * Partes
 * ================================================== */

interior b32
_frustum_legere (
               StmlNodus* nodus,
         constans Carmen* carmen,
                  chorda  titulus_partis,
                     s32  index,
                 Piscina* piscina,
           CarmenFrustum* frustum,
          CarminisVitium* vitium)
{
          StmlNodus* textus = NIHIL;
          Cantilena  k;
       CantusVitium  cv;
 constans character* causa = NIHIL;
                i32  i;
                s32  t;

    si (!_positionem_legere(nodus, carmen, VERUM, &frustum->pulsus,
                            &causa))
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, causa);
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(nodus, i);

        si (l->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (!_elementum_est(l, "textus") || textus != NIHIL)
        {
            redde _vitium(vitium, carmen->titulus, titulus_partis,
                index,
                ZEPHYRUM, "in frusto solum textus unus");
        }
        textus = l;
    }
    si (textus == NIHIL)
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, "frustum sine textu");
    }
    frustum->textus  = stml_textus_valor(textus, piscina);
    t                = _tempus_in_textu(frustum->textus);
    si (t >= ZEPHYRUM)
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            (i32)t, "'t' in frusto: tempus carminis est");
    }
    si (!cantus_legere(frustum->textus, piscina, &k, &cv))
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            cv.offset, cv.causa);
    }
    si (k.numerus == ZEPHYRUM)
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, "frustum sine notis");
    }
    frustum->notae = (CarmenNota*)piscina_allocare(piscina,
        (memoriae_index)k.numerus * magnitudo(CarmenNota));
    si (frustum->notae == NIHIL)
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, "piscina exhausta");
    }
    per (i = ZEPHYRUM; i < k.numerus; i++)
    {
        CarmenNota* n = &frustum->notae[i];

        n->pulsus       = k.notae[i].pulsus;
        n->longitudo    = k.notae[i].pulsus_longitudo;
        n->nota         = k.notae[i].nota;
        n->lucrum       = k.notae[i].lucrum;
        n->littera      = k.notae[i].littera;
        n->praeparatum  = k.notae[i].praeparatum;
        n->offset       = k.notae[i].offset;
    }
    frustum->numerus           = k.numerus;
    frustum->longitudo         = k.pulsus;
    frustum->lineae            = k.lineae;
    frustum->numerus_linearum  = k.numerus_linearum;
    redde VERUM;
}

/* <reverberatio aula mollities latitudo lucrum/>: omnia 0..1,
 * ordinaria 0.5 0.5 1 0.25; causa NIHIL si bene */
interior constans character*
_reverberationem_legere (
                 StmlNodus* nodus,
    SonusReverberatioForma* forma,
                       f32* lucrum)
{
    hic_manens constans character* constans CAMPI_AULAE[IV] = {
        "aula", "mollities", "latitudo", "lucrum"
    };
    f32* loca[IV];
    i32  i;
    i32  k;

    forma->aula       = 0.5f;
    forma->mollities  = 0.5f;
    forma->latitudo   = 1.0f;
    *lucrum           = 0.25f;
    loca[0]           = &forma->aula;
    loca[1]           = &forma->mollities;
    loca[2]           = &forma->latitudo;
    loca[3]           = lucrum;
    per (i = ZEPHYRUM; nodus->attributa != NIHIL
                       && i < (i32)xar_numerus(nodus->attributa); i++)
    {
        StmlAttributum* t = (StmlAttributum*)xar_obtinere(
            nodus->attributa, i);
                    f64 valor;

        per (k = ZEPHYRUM; k < IV; k++)
        {
            si (chorda_aequalis_literis(*t->titulus, CAMPI_AULAE[k]))
            {
                frange;
            }
        }
        si (k == IV)
        {
            redde "reverberatio: attributum ignotum";
        }
        si (   t->valor == NIHIL || !chorda_ut_f64(*t->valor, &valor)
            || valor < 0.0 || valor > 1.0)
        {
            redde "reverberatio: valor extra 0..1";
        }
        *loca[k] = (f32)valor;
    }
    redde NIHIL;
}

interior b32
_partem_legere (
               StmlNodus* nodus,
         constans Carmen* carmen,
                 Piscina* piscina,
              CarmenPars* pars,
          CarminisVitium* vitium)
{
    chorda* a;
       f64  lucrum;
       i32  numerus = ZEPHYRUM;
       i32  i;
       i32  j;

    a = stml_attributum_capere(nodus, "titulus");
    si (a == NIHIL || !_signum_validum(*a))
    {
        redde _vitium(vitium, carmen->titulus, _vacua(), -I, ZEPHYRUM,
            "pars sine titulo [a-z0-9_]+");
    }
    pars->titulus  = *a;
    a              = stml_attributum_capere(nodus, "instrumentum");
    si (a == NIHIL || !_signum_validum(*a))
    {
        redde _vitium(vitium, carmen->titulus, pars->titulus, -I,
            ZEPHYRUM, "pars sine instrumento [a-z0-9_]+");
    }
    pars->instrumentum  = *a;
    pars->lucrum        = 1.0f;
    a                   = stml_attributum_capere(nodus, "lucrum");
    si (a != NIHIL)
    {
        si (!chorda_ut_f64(*a, &lucrum) || lucrum < 0.0 || lucrum > 1.0)
        {
            redde _vitium(vitium, carmen->titulus, pars->titulus, -I,
                ZEPHYRUM, "lucrum extra 0..1");
        }
        pars->lucrum = (f32)lucrum;
    }
    pars->positio  = 0.0f;
    a              = stml_attributum_capere(nodus, "positio");
    si (a != NIHIL)
    {
        si (   !chorda_ut_f64(*a, &lucrum) || lucrum < -1.0
            || lucrum > 1.0)
        {
            redde _vitium(vitium, carmen->titulus, pars->titulus, -I,
                ZEPHYRUM, "positio extra -1..1");
        }
        pars->positio = (f32)lucrum;
    }
    pars->reverberatio  = 0.0f;
    a                   = stml_attributum_capere(nodus, "reverberatio");
    si (a != NIHIL)
    {
        si (!chorda_ut_f64(*a, &lucrum) || lucrum < 0.0 || lucrum > 1.0)
        {
            redde _vitium(vitium, carmen->titulus, pars->titulus, -I,
                ZEPHYRUM, "reverberatio extra 0..1");
        }
        pars->reverberatio = (f32)lucrum;
    }
    pars->mutus = stml_attributum_habet(nodus, "mutus");
    pars->solus = stml_attributum_habet(nodus, "solus");
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(nodus, i);

        si (l->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (!_elementum_est(l, "frustum"))
        {
            redde _vitium(vitium, carmen->titulus, pars->titulus, -I,
                ZEPHYRUM, "in parte solum frusta");
        }
        numerus++;
    }
    pars->frusta             = NIHIL;
    pars->numerus_frustorum  = ZEPHYRUM;
    si (numerus == ZEPHYRUM)
    {
        redde VERUM;
    }
    pars->frusta = (CarmenFrustum*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(CarmenFrustum));
    si (pars->frusta == NIHIL)
    {
        redde _vitium(vitium, carmen->titulus, pars->titulus, -I,
            ZEPHYRUM, "piscina exhausta");
    }
    j = ZEPHYRUM;
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(nodus, i);

        si (l->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (!_frustum_legere(l, carmen, pars->titulus, (s32)j, piscina,
                             &pars->frusta[j], vitium))
        {
            redde FALSUM;
        }
        /* ordine positionis, non se tegentes */
        si (   j > ZEPHYRUM
            && pars->frusta[j].pulsus < pars->frusta[j - I].pulsus
                                      + pars->frusta[j - I].longitudo)
        {
            redde _vitium(vitium, carmen->titulus, pars->titulus,
                (s32)j,
                ZEPHYRUM,
                "frusta se tegunt aut ordine positionis non sunt");
        }
        j++;
    }
    pars->numerus_frustorum = numerus;
    redde VERUM;
}


/* ==================================================
 * Carmen
 * ================================================== */

/* liberi generis dati numerantur */
interior i32
_numerare (
             StmlNodus* nodus,
    constans character* titulus)
{
    i32 n = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        si (_elementum_est(stml_liberum_ad_indicem(nodus, i), titulus))
        {
            n++;
        }
    }
    redde n;
}

/* mutatio (tempus, metrum, clavis) ad positionem: eadem positione
 * ultimam substituit, ordine crescente aliter vitium. Reddit indicem
 * scribendum aut -1 */
interior s32
_locus_mutationis (
    s64 ultimus,
    i32 numerus,
    s64 positio)
{
    si (positio == ultimus)
    {
        redde (s32)numerus - I;
    }
    si (positio < ultimus)
    {
        redde -I;
    }
    redde (s32)numerus;
}

/* <tempus> scriptum: valor (gradus) et usque="M" (clivus ad initium
 * mensurae M, post positionem). Ordine positionis; nulla mutatio
 * intra clivum priorem. Clivus eadem positione qua gradus prior eum
 * servat (initium clivi), gradus eadem positione substituit */
interior b32
_tempus_scriptum_legere (
              StmlNodus*  n,
                 Carmen*  k,
                    s64   positio,
     constans character** causa)
{
    constans CarmenTempus* ultimum =
        &k->tempora_scripta[k->numerus_temporum_scriptorum - I];
                   chorda* a      = stml_attributum_capere(n, "valor");
                      s64  usque  = ZEPHYRUM;
                      s64  u;
                      s32  l;

    si (stml_attributum_habet(n, "usque"))
    {
        si (   !_s64_legere(n, "usque", &u) || u < I
            || u > CMXCIX * M)
        {
            *causa = "tempus: usque non numerus mensurae";
            redde FALSUM;
        }
        usque = _pulsus_mensurae(k->metra, k->numerus_metrorum, (i32)u);
        si (usque <= positio)
        {
            *causa = "tempus: usque post initium clivi";
            redde FALSUM;
        }
    }
    si (ultimum->usque != ZEPHYRUM && positio < ultimum->usque)
    {
        *causa = "tempus: mutatio intra clivum";
        redde FALSUM;
    }
    l = _locus_mutationis(ultimum->pulsus,
        k->numerus_temporum_scriptorum, positio);
    si (   l       >= ZEPHYRUM && usque != ZEPHYRUM
        && positio == ultimum->pulsus)
    {
        l = (s32)k->numerus_temporum_scriptorum;
    }
    si (   l < ZEPHYRUM || !a
        || !_tempus_legere(*a, &k->tempora_scripta[l].tempus))
    {
        *causa = "tempus: valor 20..400, ordine positionis";
        redde FALSUM;
    }
    k->tempora_scripta[l].pulsus  = positio;
    k->tempora_scripta[l].usque   = usque;
    si (l == (s32)k->numerus_temporum_scriptorum)
    {
        k->numerus_temporum_scriptorum++;
    }
    redde VERUM;
}

/* gradum mappae sonandi addit; eadem positione ultimum substituit */
interior vacuum
_gradum_addere (
    CantusTempus* tempora,
             i32* numerus,
             s64  pulsus,
             i32  tempus)
{
    si (*numerus > ZEPHYRUM && tempora[*numerus - I].pulsus == pulsus)
    {
        tempora[*numerus - I].tempus = tempus;
        redde;
    }
    tempora[*numerus].pulsus   = pulsus;
    tempora[*numerus].tempus   = tempus;
    tempora[*numerus].quadrum  = ZEPHYRUM;
    (*numerus)++;
}

/* mappa sonandi ex scriptis: gradus ut sunt; clivus = gradus per
 * CARMEN_GRADUS_TEMPORIS, tempus medii gradus (linearis in pulsibus,
 * rotundatum), deinde valor ad usque. FALSUM: piscina exhausta */
interior b32
_mappam_sonandi (
     Carmen* k,
    Piscina* piscina)
{
    i32 numerus    = ZEPHYRUM;
    i32 capacitas  = ZEPHYRUM;
    i32 prius      = CARMEN_TEMPUS_ORDINARIUM;
    i32 i;

    per (i = ZEPHYRUM; i < k->numerus_temporum_scriptorum; i++)
    {
        constans CarmenTempus* t = &k->tempora_scripta[i];

        capacitas += I;
        si (t->usque != ZEPHYRUM)
        {
            capacitas += (i32)((t->usque - t->pulsus
                + CARMEN_GRADUS_TEMPORIS - I) / CARMEN_GRADUS_TEMPORIS);
        }
    }
    k->tempora = (CantusTempus*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(CantusTempus));
    si (!k->tempora)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < k->numerus_temporum_scriptorum; i++)
    {
        constans CarmenTempus* t = &k->tempora_scripta[i];
                          s64  p;

        si (t->usque == ZEPHYRUM)
        {
            _gradum_addere(k->tempora, &numerus, t->pulsus, t->tempus);
            prius = t->tempus;
            perge;
        }
        per (p = t->pulsus; p < t->usque; p += CARMEN_GRADUS_TEMPORIS)
        {
            s64 q = p + CARMEN_GRADUS_TEMPORIS < t->usque
                  ? p + CARMEN_GRADUS_TEMPORIS : t->usque;
            f64 medium = (f64)(p + q) / 2.0 - (f64)t->pulsus;
            f64 valor  = (f64)prius + (f64)((s32)t->tempus - (s32)prius)
                       * medium / (f64)(t->usque - t->pulsus);

            _gradum_addere(k->tempora, &numerus, p,
                           (i32)(valor + 0.5));
        }
        _gradum_addere(k->tempora, &numerus, t->usque, t->tempus);
        prius = t->tempus;
    }
    k->numerus_temporum = numerus;
    cantus_tempora_computare(k->tempora, k->numerus_temporum);
    redde VERUM;
}


/* ==================================================
 * Instrumenta (libri et carminis)
 * ================================================== */

/* campus TibiaParametra per titulum attributi; fines inclusivi */
nomen structura {
     constans character* titulus;
                    f32  infima;
                    f32  summa;
} CampusInstrumenti;

hic_manens constans CampusInstrumenti CAMPI[XX] = {
    { "officium",    0.0f, 1.0f     },
    { "impetus",     0.0f, 60.0f    },
    { "lapsus",      0.0f, 60.0f    },
    { "tenor",       0.0f, 1.0f     },
    { "remissio",    0.0f, 60.0f    },
    { "vibratus",    0.0f, 12.0f    },
    { "celeritas",   0.0f, 50.0f    },
    { "gravia",      0.0f, 24000.0f },
    { "amplitudo",   0.0f, 1.0f     },
    { "decrementum", 0.0f, 60.0f    },
    { "claritas",    0.0f, 1.0f     },
    { "ratio",       0.0f, 32.0f    },
    { "index",       0.0f, 50.0f    },
    { "genus",       0.0f, 0.0f     },   /* verbum */
    { "forma",       0.0f, 0.0f     },   /* verbum */
    { "vocalis",     0.0f, 4.0f     },   /* verbum aut numerus */
    { "flatus",      0.0f, 1.0f     },
    { "resonans",    0.0f, 20000.0f },
    { "acumen",      0.0f, 20.0f    },
    { "motus",     -48.0f, 48.0f    }
};

hic_manens constans character* VOCALES[V] = {
    "a", "e", "i", "o", "u"
};
hic_manens constans character* FORMAE[IV] = {
    "pulsus", "serra", "triangulum", "sinus"
};

/* campus k -> locus in parametris */
interior f32*
_campus (
    TibiaParametra* p,
               i32  k)
{
    f32* campi[XX];

    campi[0]   = &p->officium;
    campi[1]   = &p->involucrum.impetus;
    campi[2]   = &p->involucrum.lapsus;
    campi[3]   = &p->involucrum.tenor;
    campi[4]   = &p->involucrum.remissio;
    campi[5]   = &p->vibratus;
    campi[6]   = &p->vibratus_celeritas;
    campi[7]   = &p->gravia;
    campi[8]   = &p->amplitudo;
    campi[9]   = &p->decrementum;
    campi[10]  = &p->claritas;
    campi[11]  = &p->ratio;
    campi[12]  = &p->index;
    campi[13]  = &p->genus;
    campi[14]  = &p->forma;
    campi[15]  = &p->vocalis;
    campi[16]  = &p->flatus;
    campi[17]  = &p->resonans;
    campi[18]  = &p->acumen;
    campi[19]  = &p->motus;
    redde campi[k];
}

/* verbum in tabula -> index; -1 si ignotum */
interior s32
_verbum (
                  chorda   v,
      constans character** tabula,
                     i32   numerus)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (chorda_aequalis_literis(v, tabula[i]))
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* verbum generis (tibia_generis_titulus) -> TibiaGenus; -1 si
 * ignotum */
interior s32
_genus_verbi (
    chorda v)
{
    i32 g;

    per (g = ZEPHYRUM; tibia_generis_titulus((TibiaGenus)g) != NIHIL;
         g++)
    {
        si (chorda_aequalis_literis(v, tibia_generis_titulus(
            (TibiaGenus)g)))
        {
            redde (s32)g;
        }
    }
    redde -I;
}

/* "N..M" MIDI 0..127, N <= M */
interior b32
_ambitum_legere (
    chorda  v,
       i32* infima,
       i32* summa)
{
    i32 i;
    s64 a;
    s64 b;

    per (i = ZEPHYRUM; i + I < v.mensura; i++)
    {
        si (v.datum[i] == '.' && v.datum[i + I] == '.')
        {
            chorda x = chorda_sectio(v, ZEPHYRUM, i);
            chorda y = chorda_sectio(v, i + II, v.mensura);

            si (   !chorda_ut_s64(x, &a) || !chorda_ut_s64(y, &b)
                || a < ZEPHYRUM || b > CXXVII || a > b)
            {
                redde FALSUM;
            }
            *infima  = (i32)a;
            *summa   = (i32)b;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* "r:a:d r:a:d ..." (spatiis separati, 1..TIBIA_PARTIALES_NUMERUS):
 * ratio (0, 32], amplitudo [0, 1], decrementum [0, 60]; omnes
 * substituit, reliqui nulli */
interior b32
_partiales_legere (
             chorda  v,
     TibiaParametra* p)
{
    TibiaPartialis q[TIBIA_PARTIALES_NUMERUS];
               i32 n = ZEPHYRUM;
               i32 i = ZEPHYRUM;

    memset(q, ZEPHYRUM, magnitudo(q));
    dum (i < v.mensura)
    {
           i32 initium;
           i32 divisiones[II];
           i32 numerus_divisionum = ZEPHYRUM;
           f64 r;
           f64 a;
           f64 d;
        chorda partes[III];

        si (v.datum[i] == ' ')
        {
            i++;
            perge;
        }
        initium = i;
        dum (i < v.mensura && v.datum[i] != ' ')
        {
            si (v.datum[i] == ':')
            {
                si (numerus_divisionum == II)
                {
                    redde FALSUM;
                }
                divisiones[numerus_divisionum++] = i;
            }
            i++;
        }
        si (numerus_divisionum != II || n == TIBIA_PARTIALES_NUMERUS)
        {
            redde FALSUM;
        }
        partes[ZEPHYRUM]  = chorda_sectio(v, initium, divisiones[0]);
        partes[I]         = chorda_sectio(v, divisiones[0] + I,
                                          divisiones[I]);
        partes[II]        = chorda_sectio(v, divisiones[I] + I, i);
        si (   !chorda_ut_f64(partes[ZEPHYRUM], &r)
            || !chorda_ut_f64(partes[I], &a)
            || !chorda_ut_f64(partes[II], &d)
            || r <= 0.0 || r > 32.0 || a < 0.0 || a > 1.0
            || d < 0.0 || d > 60.0)
        {
            redde FALSUM;
        }
        q[n].ratio        = (f32)r;
        q[n].amplitudo    = (f32)a;
        q[n].decrementum  = (f32)d;
        n++;
    }
    si (n == ZEPHYRUM)
    {
        redde FALSUM;
    }
    memcpy(p->partiales, q, magnitudo(q));
    redde VERUM;
}

/* instrumentum ex nodo; priora (eodem gradu et libri) = bases licitae.
 * vitium cum carmine (vacuo in libro) */
interior b32
_instrumentum_legere (
                      StmlNodus* nodus,
    constans CarmenInstrumentum* priora,
                            i32  numerus,
                         chorda  carmen,
             CarmenInstrumentum* x,
                 CarminisVitium* vitium)
{
    chorda* a      = stml_attributum_capere(nodus, "titulus");
    chorda* basis  = stml_attributum_capere(nodus, "basis");
       i32  i;
       i32  k;

    si (a == NIHIL || !_signum_validum(*a))
    {
        redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
            "instrumentum sine titulo [a-z0-9_]+");
    }
    x->titulus  = *a;
    x->infima   = ZEPHYRUM;
    x->summa    = CXXVII;
    memset(&x->parametra, ZEPHYRUM, magnitudo(TibiaParametra));
    x->parametra.forma             = (f32)SONUS_UNDA_SINUS;
    x->parametra.officium          = 0.5f;
    x->parametra.involucrum.tenor  = 1.0f;
    x->parametra.amplitudo         = 0.5f;
    si (basis != NIHIL)
    {
         constans CarmenInstrumentum* b = NIHIL;
                           character  t[LXIV];

        per (i = numerus; i > ZEPHYRUM && b == NIHIL; i--)
        {
            si (chorda_aequalis(priora[i - I].titulus, *basis))
            {
                b = &priora[i - I];
            }
        }
        si (b != NIHIL)
        {
            x->parametra  = b->parametra;
            x->infima     = b->infima;
            x->summa      = b->summa;
        }
        alioquin si (   basis->mensura > ZEPHYRUM
                     && basis->mensura < (i32)magnitudo(t))
        {
            memcpy(t, basis->datum, (size_t)basis->mensura);
            t[basis->mensura] = '\0';
            si (tibia_praeparatum(t) == NIHIL)
            {
                redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                    "instrumentum: basis ignota");
            }
            x->parametra = *tibia_praeparatum(t);
        }
        alioquin
        {
            redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                "instrumentum: basis ignota");
        }
    }
    /* attributa omnia: nota aut vitium */
    per (i = ZEPHYRUM; nodus->attributa != NIHIL
                       && i < (i32)xar_numerus(nodus->attributa); i++)
    {
        StmlAttributum* t = (StmlAttributum*)xar_obtinere(
            nodus->attributa, i);
                    f64 valor;
                    s32 w;

        si (   chorda_aequalis_literis(*t->titulus, "titulus")
            || chorda_aequalis_literis(*t->titulus, "basis"))
        {
            perge;
        }
        /* valor vacuus ("") in stml = NIHIL */
        si (t->valor == NIHIL)
        {
            redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                "instrumentum: valor malus aut extra fines");
        }
        si (chorda_aequalis_literis(*t->titulus, "partiales"))
        {
            si (!_partiales_legere(*t->valor, &x->parametra))
            {
                redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                    "instrumentum: valor malus aut extra fines");
            }
            perge;
        }
        si (chorda_aequalis_literis(*t->titulus, "ambitus"))
        {
            si (!_ambitum_legere(*t->valor, &x->infima, &x->summa))
            {
                redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                    "instrumentum: valor malus aut extra fines");
            }
            perge;
        }
        per (k = ZEPHYRUM; k < XX; k++)
        {
            si (chorda_aequalis_literis(*t->titulus, CAMPI[k].titulus))
            {
                frange;
            }
        }
        si (k == XX)
        {
            redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                "instrumentum: attributum ignotum");
        }
        /* vocalis: verbum (a e i o u) aut numerus infra */
        w = k == XV ? _verbum(*t->valor, VOCALES, V) : -I;
        si (w >= ZEPHYRUM)
        {
            *_campus(&x->parametra, k) = (f32)w;
            perge;
        }
        si (k == XIII || k == XIV)
        {
            w = k == XIII ? _genus_verbi(*t->valor)
                          : _verbum(*t->valor, FORMAE, IV);
            si (w < ZEPHYRUM)
            {
                redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                    "instrumentum: valor malus aut extra fines");
            }
            *_campus(&x->parametra, k) = (f32)w;
            perge;
        }
        si (   !chorda_ut_f64(*t->valor, &valor)
            || valor < (f64)CAMPI[k].infima
            || valor > (f64)CAMPI[k].summa)
        {
            redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                "instrumentum: valor malus aut extra fines");
        }
        *_campus(&x->parametra, k) = (f32)valor;
    }
    redde VERUM;
}

/* instrumenta nodi (filii 'instrumentum') in x[ab..]; bases: priora
 * eiusdem gradus et 'externa' (libri). Reddit numerum aut FALSUM per
 * vitium (numerus in *n) */
interior b32
_instrumenta_legere (
                      StmlNodus*  nodus,
    constans CarmenInstrumentum*  externa,
                            i32   numerus_externorum,
                         chorda   carmen,
                        Piscina*  piscina,
             CarmenInstrumentum** x,
                            i32*  n,
                 CarminisVitium*  vitium)
{
                   i32  numerus = _numerare(nodus, "instrumentum");
                   i32  i;
                   i32  j = ZEPHYRUM;
                   i32  m;
    CarmenInstrumentum* bases;

    *n = ZEPHYRUM;
    *x = (CarmenInstrumentum*)piscina_allocare(piscina,
        (memoriae_index)(numerus + numerus_externorum + I)
            * magnitudo(CarmenInstrumentum));
    bases = (CarmenInstrumentum*)piscina_allocare(piscina,
        (memoriae_index)(numerus + numerus_externorum + I)
            * magnitudo(CarmenInstrumentum));
    si (*x == NIHIL || bases == NIHIL)
    {
        redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
            "piscina exhausta");
    }
    /* bases: externa, deinde propria ordine (supra) */
    per (i = ZEPHYRUM; i < numerus_externorum; i++)
    {
        bases[i] = externa[i];
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(nodus, i);

        si (!_elementum_est(l, "instrumentum"))
        {
            perge;
        }
        si (!_instrumentum_legere(l, bases, numerus_externorum + j,
                                  carmen, &(*x)[j], vitium))
        {
            redde FALSUM;
        }
        per (m = ZEPHYRUM; m < j; m++)
        {
            si (chorda_aequalis((*x)[m].titulus, (*x)[j].titulus))
            {
                redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                    "instrumentum iteratum");
            }
        }
        bases[numerus_externorum + j] = (*x)[j];
        j++;
    }
    *n = j;
    redde VERUM;
}

interior b32
_carmen_legere (
                          StmlNodus* nodus,
        constans CarmenInstrumentum* libri,
                                i32  numerus_libri,
    constans SonusReverberatioForma* aula_libri,
                                f32  lucrum_aulae_libri,
                            Piscina* piscina,
                             Carmen* k,
                     CarminisVitium* vitium)
{
                chorda* a;
                   i32  i;
                   i32  j;
                   s32  l;
                   s64  positio;
                   s64  finis;
    constans character* causa;
                   b32  aula_propria = FALSUM;

    memset(k, ZEPHYRUM, magnitudo(Carmen));
    k->reverberatio = *aula_libri;
    k->lucrum_reverberationis = lucrum_aulae_libri;
    a = stml_attributum_capere(nodus, "titulus");
    si (a == NIHIL || !_signum_validum(*a))
    {
        redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
            "carmen sine titulo [a-z0-9_]+");
    }
    k->titulus  = *a;
    k->tempora_scripta  = (CarmenTempus*)piscina_allocare(piscina,
        (memoriae_index)(_numerare(nodus, "tempus") + I)
        * magnitudo(CarmenTempus));
    k->metra    = (CarmenMetrum*)piscina_allocare(piscina,
        (memoriae_index)(_numerare(nodus, "metrum") + I)
        * magnitudo(CarmenMetrum));
    k->claves   = (CarmenClavis*)piscina_allocare(piscina,
        (memoriae_index)(_numerare(nodus, "clavis") + I)
        * magnitudo(CarmenClavis));
    si (!k->tempora_scripta || !k->metra || !k->claves)
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            "piscina exhausta");
    }
    /* initialia */
    k->tempora_scripta[ZEPHYRUM].pulsus = ZEPHYRUM;
    k->tempora_scripta[ZEPHYRUM].tempus = CARMEN_TEMPUS_ORDINARIUM;
    k->tempora_scripta[ZEPHYRUM].usque = ZEPHYRUM;
    k->metra[ZEPHYRUM].pulsus = ZEPHYRUM;
    k->metra[ZEPHYRUM].numerator = IV;
    k->metra[ZEPHYRUM].denominator = IV;
    k->claves[ZEPHYRUM].pulsus = ZEPHYRUM;
    k->claves[ZEPHYRUM].quintae = ZEPHYRUM;
    k->claves[ZEPHYRUM].minor = FALSUM;
    k->numerus_temporum_scriptorum = I;
    k->numerus_metrorum = I;
    k->numerus_clavium = I;
    a = stml_attributum_capere(nodus, "tempus");
    si (a && !_tempus_legere(*a, &k->tempora_scripta[ZEPHYRUM].tempus))
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            "tempus extra 20..400");
    }
    a = stml_attributum_capere(nodus, "metrum");
    si (a && !_metrum_legere(*a, &k->metra[ZEPHYRUM]))
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            "metrum non N/D (D potentia II)");
    }
    a = stml_attributum_capere(nodus, "clavis");
    si (a && !_clavem_legere(*a, &k->claves[ZEPHYRUM]))
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            "clavis non 'littera [#|b] maior|minor'");
    }
    /* elementa nota; mutationes metri ordine primae */
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* n = stml_liberum_ad_indicem(nodus, i);

        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (   !_elementum_est(n, "tempus")
            && !_elementum_est(n, "metrum")
            && !_elementum_est(n, "clavis")
            && !_elementum_est(n, "pars")
            && !_elementum_est(n, "instrumentum")
            && !_elementum_est(n, "reverberatio"))
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                "in carmine solum tempus, metrum, clavis, pars, "
                "instrumentum, reverberatio");
        }
        /* aula propria: tota substituit libri */
        si (_elementum_est(n, "reverberatio"))
        {
            si (aula_propria)
            {
                redde _vitium(vitium, k->titulus, _vacua(), -I,
                    ZEPHYRUM, "reverberatio iterata");
            }
            causa = _reverberationem_legere(n, &k->reverberatio,
                                            &k->lucrum_reverberationis);
            si (causa != NIHIL)
            {
                redde _vitium(vitium, k->titulus, _vacua(), -I,
                    ZEPHYRUM, causa);
            }
            aula_propria = VERUM;
            perge;
        }
        si (!_elementum_est(n, "metrum"))
        {
            perge;
        }
        a = stml_attributum_capere(n, "valor");
        si (!_positionem_legere(n, k, FALSUM, &positio, &causa))
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                causa);
        }
        l = _locus_mutationis(k->metra[k->numerus_metrorum - I].pulsus,
            k->numerus_metrorum, positio);
        si (l < ZEPHYRUM || !a || !_metrum_legere(*a, &k->metra[l]))
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                "metrum: valor N/D, ordine mensurae");
        }
        k->metra[l].pulsus = positio;
        si (l == (s32)k->numerus_metrorum)
        {
            k->numerus_metrorum++;
        }
    }
    /* tempus et clavis (mappa metri plena) */
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
         StmlNodus* n       = stml_liberum_ad_indicem(nodus, i);
               b32  tempus  = _elementum_est(n, "tempus");

        si (!tempus && !_elementum_est(n, "clavis"))
        {
            perge;
        }
        a = stml_attributum_capere(n, "valor");
        si (!_positionem_legere(n, k, VERUM, &positio, &causa))
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                causa);
        }
        si (tempus)
        {
            si (!_tempus_scriptum_legere(n, k, positio, &causa))
            {
                redde _vitium(vitium, k->titulus, _vacua(), -I,
                    ZEPHYRUM, causa);
            }
        }
        alioquin
        {
            l = _locus_mutationis(k->claves[k->numerus_clavium
                - I].pulsus,
                k->numerus_clavium, positio);
            si (   l < ZEPHYRUM || !a
                || !_clavem_legere(*a, &k->claves[l]))
            {
                redde _vitium(vitium, k->titulus, _vacua(), -I,
                    ZEPHYRUM,
                    "clavis: valor littera [#|b] maior|minor, ordine");
            }
            k->claves[l].pulsus = positio;
            si (l == (s32)k->numerus_clavium)
            {
                k->numerus_clavium++;
            }
        }
    }
    si (!_mappam_sonandi(k, piscina))
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            "piscina exhausta");
    }
    /* instrumenta: carminis propria (bases: libri et supra), deinde
     * libri - quaerendo propria vincunt */
    {
        CarmenInstrumentum* propria;
                       i32  n;

        si (!_instrumenta_legere(nodus, libri, numerus_libri,
            k->titulus,
                                 piscina, &propria, &n, vitium))
        {
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < numerus_libri; i++)
        {
            propria[n + i] = libri[i];
        }
        k->instrumenta             = propria;
        k->numerus_instrumentorum  = n + numerus_libri;
    }
    /* partes */
    k->numerus_partium = _numerare(nodus, "pars");
    si (k->numerus_partium > ZEPHYRUM)
    {
        k->partes = (CarmenPars*)piscina_allocare(piscina,
            (memoriae_index)k->numerus_partium * magnitudo(CarmenPars));
        si (!k->partes)
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                "piscina exhausta");
        }
    }
    j = ZEPHYRUM;
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
         StmlNodus* n = stml_liberum_ad_indicem(nodus, i);
               i32  m;

        si (!_elementum_est(n, "pars"))
        {
            perge;
        }
        si (!_partem_legere(n, k, piscina, &k->partes[j], vitium))
        {
            redde FALSUM;
        }
        per (m = ZEPHYRUM; m < j; m++)
        {
            si (chorda_aequalis(k->partes[m].titulus,
                                k->partes[j].titulus))
            {
                redde _vitium(vitium, k->titulus, k->partes[j].titulus,
                    -I, ZEPHYRUM, "pars iterata");
            }
        }
        j++;
    }
    /* longitudo: finis frusti ultimi */
    k->pulsus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        CarmenPars* p = &k->partes[i];

        si (p->numerus_frustorum > ZEPHYRUM)
        {
            finis = p->frusta[p->numerus_frustorum - I].pulsus
                  + p->frusta[p->numerus_frustorum - I].longitudo;
            si (finis > k->pulsus)
            {
                k->pulsus = finis;
            }
        }
    }
    redde VERUM;
}

b32
carmina_legere_cum (
                         chorda  stml,
    constans CarmenInstrumentum* instrumentarium,
                            i32  numerus_instrumentarii,
                        Piscina* piscina,
                        Carmina* liber,
                 CarminisVitium* vitium)
{
              StmlResultus  r;
                 StmlNodus* radix;
                    Carmen* carmina;
                       i32  numerus = ZEPHYRUM;
                       i32  i;
                       i32  j;
                       i32  m;
        CarmenInstrumentum* libri;
                       i32  numerus_libri;
        CarmenInstrumentum* visibilia;
    SonusReverberatioForma  aula;
                       f32  lucrum_aulae  = 0.0f;
                       b32  aula_libri    = FALSUM;

    aula.aula = 0.5f;
    aula.mollities = 0.5f;
    aula.latitudo = 1.0f;
    liber->carmina = NIHIL;
    liber->numerus = ZEPHYRUM;
    r = stml_legere(stml, piscina, internamentum_creare(piscina));
    si (!r.successus)
    {
        redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
            "STML fractum");
    }
    radix = r.elementum_radix;
    si (radix == NIHIL || !_elementum_est(radix, "carmina"))
    {
        redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
            "radix non est 'carmina'");
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
        StmlNodus* n = stml_liberum_ad_indicem(radix, i);

        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (_elementum_est(n, "instrumentum"))
        {
            perge;
        }
        /* aula libri: carminibus sine sua ordinaria */
        si (_elementum_est(n, "reverberatio"))
        {
            constans character* causa;

            si (aula_libri)
            {
                redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
                    "reverberatio iterata");
            }
            causa = _reverberationem_legere(n, &aula, &lucrum_aulae);
            si (causa != NIHIL)
            {
                redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
                    causa);
            }
            aula_libri = VERUM;
            perge;
        }
        si (!_elementum_est(n, "carmen"))
        {
            redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
                "in carminibus solum carmina, instrumenta, "
                "reverberatio");
        }
        numerus++;
    }
    /* instrumenta libri (ordine; basis supra et instrumentarium) ante
     * carmina; carminibus visibilia = libri, deinde instrumentarii */
    si (!_instrumenta_legere(radix, instrumentarium,
                             numerus_instrumentarii, _vacua(), piscina,
                             &libri, &numerus_libri, vitium))
    {
        redde FALSUM;
    }
    visibilia = (CarmenInstrumentum*)piscina_allocare(piscina,
        (memoriae_index)(numerus_libri + numerus_instrumentarii + I)
            * magnitudo(CarmenInstrumentum));
    si (visibilia == NIHIL)
    {
        redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
            "piscina exhausta");
    }
    per (i = ZEPHYRUM; i < numerus_libri; i++)
    {
        visibilia[i] = libri[i];
    }
    per (i = ZEPHYRUM; i < numerus_instrumentarii; i++)
    {
        visibilia[numerus_libri + i] = instrumentarium[i];
    }
    si (numerus == ZEPHYRUM)
    {
        redde VERUM;
    }
    carmina = (Carmen*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(Carmen));
    si (carmina == NIHIL)
    {
        redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
            "piscina exhausta");
    }
    j = ZEPHYRUM;
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
        StmlNodus* n = stml_liberum_ad_indicem(radix, i);

        si (   n->genus != STML_NODUS_ELEMENTUM
            || _elementum_est(n, "instrumentum")
            || _elementum_est(n, "reverberatio"))
        {
            perge;
        }
        si (!_carmen_legere(n, visibilia,
                            numerus_libri + numerus_instrumentarii,
                            &aula, lucrum_aulae,
                            piscina, &carmina[j], vitium))
        {
            redde FALSUM;
        }
        carmina[j].numerus_instrumentarii = numerus_instrumentarii;
        per (m = ZEPHYRUM; m < j; m++)
        {
            si (chorda_aequalis(carmina[m].titulus, carmina[j].titulus))
            {
                redde _vitium(vitium, carmina[j].titulus, _vacua(), -I,
                    ZEPHYRUM, "carmen iteratum");
            }
        }
        j++;
    }
    liber->carmina = carmina;
    liber->numerus = numerus;
    redde VERUM;
}

b32
carmina_legere (
            chorda  stml,
           Piscina* piscina,
           Carmina* liber,
    CarminisVitium* vitium)
{
    CarmenInstrumentum* instrumentarium;
                   i32  numerus;

    liber->carmina = NIHIL;
    liber->numerus = ZEPHYRUM;
    si (!instrumentarium_ordinarium(piscina, &instrumentarium, &numerus,
                                    vitium))
    {
        redde FALSUM;
    }
    redde carmina_legere_cum(stml, instrumentarium, numerus, piscina,
                             liber, vitium);
}

b32
instrumentarium_legere (
                 chorda   stml,
                Piscina*  piscina,
     CarmenInstrumentum** instrumenta,
                    i32*  numerus,
         CarminisVitium*  vitium)
{
    StmlResultus  r;
       StmlNodus* radix;
             i32  i;

    *instrumenta = NIHIL;
    *numerus = ZEPHYRUM;
    r = stml_legere(stml, piscina, internamentum_creare(piscina));
    si (!r.successus)
    {
        redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
            "STML fractum");
    }
    radix = r.elementum_radix;
    si (radix == NIHIL || !_elementum_est(radix, "instrumentarium"))
    {
        redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
            "radix non est 'instrumentarium'");
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
        StmlNodus* n = stml_liberum_ad_indicem(radix, i);

        si (   n->genus == STML_NODUS_ELEMENTUM
            && !_elementum_est(n, "instrumentum"))
        {
            redde _vitium(vitium, _vacua(), _vacua(), -I, ZEPHYRUM,
                "in instrumentario solum instrumenta");
        }
    }
    redde _instrumenta_legere(radix, NIHIL, ZEPHYRUM, _vacua(), piscina,
                              instrumenta, numerus, vitium);
}

b32
instrumentarium_ordinarium (
                Piscina*  piscina,
     CarmenInstrumentum** instrumenta,
                    i32*  numerus,
         CarminisVitium*  vitium)
{
    chorda textus = instrumentarium_infixum_textus(piscina);

    redde instrumentarium_legere(textus, piscina, instrumenta, numerus,
                                 vitium);
}

b32
carmina_ordinaria (
           Piscina* piscina,
           Carmina* liber,
    CarminisVitium* vitium)
{
    redde carmina_legere(carmina_infixum_textus(piscina), piscina,
        liber,
        vitium);
}

constans Carmen*
carmina_invenire (
    constans Carmina* liber,
              chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < liber->numerus; i++)
    {
        si (chorda_aequalis(liber->carmina[i].titulus, titulus))
        {
            redde &liber->carmina[i];
        }
    }
    redde NIHIL;
}

s64
carmen_pulsus_mensurae (
    constans Carmen* carmen,
                i32  mensura)
{
    redde _pulsus_mensurae(carmen->metra, carmen->numerus_metrorum,
        mensura);
}

s64
carmen_tempus_mensurae (
    constans Carmen* carmen,
                s64  pulsus)
{
    constans CarmenMetrum* m = &carmen->metra[ZEPHYRUM];
                      i32  i;

    per (i = ZEPHYRUM; i < carmen->numerus_metrorum; i++)
    {
        si (carmen->metra[i].pulsus <= pulsus)
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

constans CarmenInstrumentum*
carmen_instrumentum (
    constans Carmen* carmen,
             chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < carmen->numerus_instrumentorum; i++)
    {
        si (chorda_aequalis(carmen->instrumenta[i].titulus, titulus))
        {
            redde &carmen->instrumenta[i];
        }
    }
    redde NIHIL;
}
