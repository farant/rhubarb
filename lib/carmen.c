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
#include <math.h>
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

/* agogica nominata: nomen et pondera (summa = numerus) */
nomen structura {
    constans character* titulus;
                   i32  numerus;
                   f32  pondera[III];
} InaequalitasNominata;

hic_manens constans InaequalitasNominata INAEQUALITATES_NOMINATAE[] = {
    /* valse Vindobonensis: tempus I breve (II ante tempus venit), II
     * longum (III paulo post) - Bengtsson et Gabrielsson */
    { "vindobonensis", III, { 0.86f, 1.15f, 0.99f } }
};

/* pulsus mensurae metri dati */
interior s64
_longitudo_mensurae (
    constans CarmenMetrum* m)
{
    redde (s64)CANTUS_PULSUS_INTEGRAE * (s64)m->numerator
        / (s64)m->denominator;
}

/* agogica="nomen" aut "p1 p2 ... pK" (K 2..XVI, > 0): pondera normata
 * ad summam K; mensura metri in K partes pulsuum integras. Vacua
 * (sine attributo): aequale. FALSUM: causa in *causa */
interior b32
_inaequalitatem_legere (
                chorda   valor,
          CarmenMetrum*  m,
    constans character** causa)
{
    s64 l        = _longitudo_mensurae(m);
    f64 summa    = 0.0;
    i32 numerus  = ZEPHYRUM;
    i32 i        = ZEPHYRUM;
    i32 nomina  = (i32)(magnitudo(INAEQUALITATES_NOMINATAE)
                / magnitudo(INAEQUALITATES_NOMINATAE[ZEPHYRUM]));
    i32 j;

    m->inaequalitas = valor;
    per (j = ZEPHYRUM; j < nomina; j++)
    {
        constans InaequalitasNominata* a = &INAEQUALITATES_NOMINATAE[j];

        si (chorda_aequalis_literis(valor, a->titulus))
        {
            per (i = ZEPHYRUM; i < a->numerus; i++)
            {
                m->pondera[i]  = a->pondera[i];
                summa          += (f64)a->pondera[i];
            }
            numerus  = a->numerus;
            i        = (i32)valor.mensura;
        }
    }
    dum (i < (i32)valor.mensura)
    {
        chorda verbum;
           f64 x;
           i32 initium;

        dum (i < (i32)valor.mensura && valor.datum[i] == ' ')
        {
            i++;
        }
        si (i >= (i32)valor.mensura)
        {
            frange;
        }
        initium = i;
        dum (i < (i32)valor.mensura && valor.datum[i] != ' ')
        {
            i++;
        }
        verbum.datum    = valor.datum + initium;
        verbum.mensura  = (i32)(i - initium);
        si (   numerus >= CARMEN_PONDERA_MAXIMA
            || !chorda_ut_f64(verbum, &x) || !(x > 0.0))
        {
            *causa = "agogica: nomen aut pondera > 0 (II..XVI)";
            redde FALSUM;
        }
        m->pondera[numerus++]  = (f32)x;
        summa                  += x;
    }
    si (numerus < II || l % (s64)numerus != ZEPHYRUM)
    {
        *causa = "agogica: II..XVI pondera, mensura in partes integras";
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        m->pondera[j] = (f32)((f64)m->pondera[j] * (f64)numerus
            / summa);
    }
    m->numerus_ponderum = numerus;
    redde VERUM;
}

/* agogica elementi (attributum 'agogica') aut aequale si deest */
interior b32
_inaequalitatem_attributi (
             StmlNodus*  nodus,
          CarmenMetrum*  m,
    constans character** causa)
{
    chorda* a = stml_attributum_capere(nodus, "agogica");

    m->numerus_ponderum      = ZEPHYRUM;
    m->inaequalitas.datum    = NIHIL;
    m->inaequalitas.mensura  = ZEPHYRUM;
    redde a == NIHIL || _inaequalitatem_legere(*a, m, causa);
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
        n->illapsus     = k.notae[i].illapsus;
        n->continuatur  = k.notae[i].continuatur;
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

/* figura nominata (<figura>, libri aut carminis) */
nomen structura {
             chorda titulus;
             chorda textus;
    ComitatusFigura figura;
} FiguraNominata;

interior b32
_ambitum_legere (
    chorda  v,
       i32* infima,
       i32* summa);

/* initia mensurarum ab 'ab': ab, deinde initia mensurarum sequentium.
 * numerus > 0: tot mensurae (numerus + 1 initia); aliter mensurae usque
 * ad 'ad' (initium ultimum = ad). NIHIL: piscina exhausta */
interior s64*
_initia_mensurarum (
    constans Carmen* k,
                s64  ab,
                s64  ad,
                i32  numerus,
            Piscina* piscina,
                i32* numerus_mensurarum)
{
    i32  m = I;
    i32  n = ZEPHYRUM;
    s64* initia;
    i32  j;

    dum (carmen_pulsus_mensurae(k, (i32)m) <= ab)
    {
        m++;
    }
    si (numerus > ZEPHYRUM)
    {
        n = numerus;
    }
    alioquin
    {
        per (j = m; carmen_pulsus_mensurae(k, (i32)j) < ad; j++)
        {
            n++;
        }
        n++;
    }
    initia = (s64*)piscina_allocare(piscina,
        (memoriae_index)(n + I) * magnitudo(s64));
    si (initia == NIHIL)
    {
        redde NIHIL;
    }
    initia[ZEPHYRUM] = ab;
    per (j = I; j <= n; j++)
    {
        initia[j] = numerus == ZEPHYRUM && j == n
            ? ad : carmen_pulsus_mensurae(k, (i32)(m + j - I));
    }
    *numerus_mensurarum = n;
    redde initia;
}

/* <comitatus figura mensura [usque] [ambitus] [bassus]/>: frustum
 * generatum ex acordis carminis (comitatus_componere) */
interior b32
_comitatum_legere (
                  StmlNodus* nodus,
            constans Carmen* carmen,
    constans FiguraNominata* figurae,
                        i32  numerus_figurarum,
                        b32  bassus_sonat,
                     chorda  titulus_partis,
                        s32  index,
                    Piscina* piscina,
              CarmenFrustum* frustum,
             CarminisVitium* vitium)
{
    constans FiguraNominata* figura = NIHIL;
           ComitatusAmbitus  ambitus;
            ComitatusVitium  cv;
              ComitatusNota* notae;
         constans character* causa = NIHIL;
                     chorda* a;
                        s64* initia;
                        s64  ab;
                        s64  ad;
                        s64  u;
                        i32  mensurae;
                        i32  numerus;
                        i32  i;

    memset(frustum, ZEPHYRUM, magnitudo(*frustum));
    a = stml_attributum_capere(nodus, "figura");
    per (i = ZEPHYRUM; a != NIHIL && i < numerus_figurarum; i++)
    {
        si (chorda_aequalis(figurae[i].titulus, *a))
        {
            figura = &figurae[i];
            frange;
        }
    }
    si (figura == NIHIL)
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, "comitatus: figura ignota");
    }
    si (!_positionem_legere(nodus, carmen, VERUM, &ab, &causa))
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, causa);
    }
    si (stml_attributum_habet(nodus, "usque"))
    {
        si (   !_s64_legere(nodus, "usque", &u) || u < I
            || u > CMXCIX * M)
        {
            redde _vitium(vitium, carmen->titulus, titulus_partis,
                index,
                ZEPHYRUM, "comitatus: usque numerus mensurae");
        }
        ad = _pulsus_mensurae(carmen->metra, carmen->numerus_metrorum,
                              (i32)u);
    }
    alioquin si (carmen->numerus_acordorum > ZEPHYRUM)
    {
        ad = carmen->acorda[carmen->numerus_acordorum - I].pulsus
            + carmen->acorda[carmen->numerus_acordorum - I].longitudo;
    }
    alioquin
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, "comitatus sine acordis");
    }
    si (ad <= ab)
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, "comitatus: usque post initium");
    }
    ambitus.infima        = LV;
    ambitus.summa         = LXXVI;
    ambitus.bassi_infima  = XXXVI;
    ambitus.bassi_summa   = LVII;
    a                     = stml_attributum_capere(nodus, "ambitus");
    si (   a != NIHIL
        && !_ambitum_legere(*a, &ambitus.infima, &ambitus.summa))
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, "comitatus: ambitus N..M (MIDI)");
    }
    a = stml_attributum_capere(nodus, "bassus");
    si (   a != NIHIL
        && !_ambitum_legere(*a, &ambitus.bassi_infima,
                            &ambitus.bassi_summa))
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, "comitatus: bassus N..M (MIDI)");
    }
    initia = _initia_mensurarum(carmen, ab, ad, ZEPHYRUM, piscina,
                                &mensurae);
    si (initia == NIHIL)
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, "piscina exhausta");
    }
    si (!comitatus_componere(&figura->figura, carmen->acorda,
                             carmen->numerus_acordorum, initia,
                             mensurae,
                             &ambitus, bassus_sonat, piscina, &notae,
                             &numerus, &cv))
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            cv.offset, cv.causa);
    }
    si (numerus == ZEPHYRUM)
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, "comitatus sine notis");
    }
    frustum->notae = (CarmenNota*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(CarmenNota));
    si (frustum->notae == NIHIL)
    {
        redde _vitium(vitium, carmen->titulus, titulus_partis, index,
            ZEPHYRUM, "piscina exhausta");
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        CarmenNota* n = &frustum->notae[i];

        n->pulsus       = notae[i].pulsus - ab;
        n->longitudo    = notae[i].longitudo;
        n->nota         = notae[i].nota;
        n->lucrum       = notae[i].lucrum;
        n->littera      = notae[i].littera;
        n->praeparatum  = _vacua();
        n->offset       = notae[i].offset;
        n->illapsus     = ZEPHYRUM;
        n->continuatur  = FALSUM;
    }
    frustum->pulsus     = ab;
    frustum->textus     = figura->textus;
    frustum->numerus    = numerus;
    frustum->longitudo  = ad - ab;
    frustum->repetitio  = -I;
    frustum->scriptum   = -I;
    redde VERUM;
}

/* aliquis <comitatus> partium carminis figuram cum voce 'B' adhibet */
interior b32
_bassus_comitatus (
                  StmlNodus* nodus,
    constans FiguraNominata* figurae,
                        i32  numerus_figurarum)
{
    i32 i;
    i32 j;
    i32 f;

    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* pars = stml_liberum_ad_indicem(nodus, i);

        si (!_elementum_est(pars, "pars"))
        {
            perge;
        }
        per (j = ZEPHYRUM; j < stml_numerus_liberorum(pars); j++)
        {
            StmlNodus* l = stml_liberum_ad_indicem(pars, j);
               chorda* a;

            si (!_elementum_est(l, "comitatus"))
            {
                perge;
            }
            a = stml_attributum_capere(l, "figura");
            per (f = ZEPHYRUM; a != NIHIL && f < numerus_figurarum; f++)
            {
                si (   chorda_aequalis(figurae[f].titulus, *a)
                    && figurae[f].figura.bassus)
                {
                    redde VERUM;
                }
            }
        }
    }
    redde FALSUM;
}

interior b32
_partem_legere (
                  StmlNodus* nodus,
            constans Carmen* carmen,
    constans FiguraNominata* figurae,
                        i32  numerus_figurarum,
                        b32  bassus_sonat,
                    Piscina* piscina,
                 CarmenPars* pars,
             CarminisVitium* vitium)
{
    chorda* a;
       f64  lucrum;
       i32  numerus    = ZEPHYRUM;
       i32  scripta    = ZEPHYRUM;
       i32  comitatus  = ZEPHYRUM;
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
        si (   !_elementum_est(l, "frustum")
            && !_elementum_est(l, "comitatus"))
        {
            redde _vitium(vitium, carmen->titulus, pars->titulus, -I,
                ZEPHYRUM, "in parte solum frusta et comitatus");
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
        si (_elementum_est(l, "comitatus"))
        {
            si (!_comitatum_legere(l, carmen, figurae,
                numerus_figurarum, bassus_sonat,
                                   pars->titulus, (s32)j, piscina,
                                   &pars->frusta[j], vitium))
            {
                redde FALSUM;
            }
            pars->frusta[j].comitatus = (s32)comitatus++;
        }
        alioquin
        {
            si (!_frustum_legere(l, carmen, pars->titulus, (s32)j,
                                 piscina, &pars->frusta[j], vitium))
            {
                redde FALSUM;
            }
            pars->frusta[j].repetitio  = -I;
            pars->frusta[j].scriptum   = (s32)scripta++;
            pars->frusta[j].comitatus  = -I;
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

/* <figura titulus textus/> liberi nodi (libri aut carminis): tituli
 * [a-z0-9_]+ non iterati, figura lecta (comitatus_figuram_legere) */
interior b32
_figuras_legere (
          StmlNodus*  nodus,
             chorda   carmen,
            Piscina*  piscina,
     FiguraNominata** figurae,
                i32*  numerus,
     CarminisVitium*  vitium)
{
    FiguraNominata* f;
               i32  n = ZEPHYRUM;
               i32  i;
               i32  j;

    f = (FiguraNominata*)piscina_allocare(piscina,
        (memoriae_index)(_numerare(nodus, "figura") + I)
        * magnitudo(FiguraNominata));
    si (f == NIHIL)
    {
        redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
            "piscina exhausta");
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
           StmlNodus* l = stml_liberum_ad_indicem(nodus, i);
              chorda* a;
              chorda* t;
     ComitatusVitium  cv;

        si (!_elementum_est(l, "figura"))
        {
            perge;
        }
        a = stml_attributum_capere(l, "titulus");
        si (a == NIHIL || !_signum_validum(*a))
        {
            redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                "figura sine titulo [a-z0-9_]+");
        }
        per (j = ZEPHYRUM; j < n; j++)
        {
            si (chorda_aequalis(f[j].titulus, *a))
            {
                redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                    "figura: titulus iteratus");
            }
        }
        t = stml_attributum_capere(l, "textus");
        si (t == NIHIL)
        {
            redde _vitium(vitium, carmen, _vacua(), -I, ZEPHYRUM,
                "figura sine textu");
        }
        f[n].titulus  = *a;
        f[n].textus   = *t;
        si (!comitatus_figuram_legere(*t, piscina, &f[n].figura, &cv))
        {
            redde _vitium(vitium, carmen, _vacua(), -I, cv.offset,
                cv.causa);
        }
        n++;
    }
    *figurae = f;
    *numerus = n;
    redde VERUM;
}

/* <acorda> omnia (mappa metri plena): positio, textus in mensuras;
 * Carmen.acorda ordine pulsus, non se tegentia */
interior b32
_acorda_legere (
          StmlNodus* nodus,
             Carmen* k,
            Piscina* piscina,
     CarminisVitium* vitium)
{
                 i32   elementa = _numerare(nodus, "acorda");
    ComitatusAcordum** lecta;
                 i32*  numeri;
                 i32   e      = ZEPHYRUM;
                 i32   summa  = ZEPHYRUM;
                 i32   i;
                 i32   j;

    k->acorda             = NIHIL;
    k->numerus_acordorum  = ZEPHYRUM;
    si (elementa == ZEPHYRUM)
    {
        redde VERUM;
    }
    lecta = (ComitatusAcordum**)piscina_allocare(piscina,
        (memoriae_index)elementa * magnitudo(ComitatusAcordum*));
    numeri = (i32*)piscina_allocare(piscina,
        (memoriae_index)elementa * magnitudo(i32));
    si (lecta == NIHIL || numeri == NIHIL)
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            "piscina exhausta");
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
                 StmlNodus* n = stml_liberum_ad_indicem(nodus, i);
                    chorda* t;
                       s64  positio;
                       s64* initia;
                       i32  mensurae;
                       i32  reliquae;
        constans character* causa = NIHIL;
           ComitatusVitium  cv;

        si (!_elementum_est(n, "acorda"))
        {
            perge;
        }
        si (!_positionem_legere(n, k, VERUM, &positio, &causa))
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                causa);
        }
        t = stml_attributum_capere(n, "textus");
        si (t == NIHIL)
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                "acorda sine textu");
        }
        mensurae = comitatus_mensurae(*t);
        si (mensurae == ZEPHYRUM)
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                "acorda vacua");
        }
        initia = _initia_mensurarum(k, positio, ZEPHYRUM, mensurae,
                                    piscina, &reliquae);
        si (initia == NIHIL)
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                "piscina exhausta");
        }
        si (!comitatus_acorda_legere(*t, initia, mensurae, piscina,
                                     &lecta[e], &numeri[e], &cv))
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, cv.offset,
                cv.causa);
        }
        summa += numeri[e];
        e++;
    }
    k->acorda = (ComitatusAcordum*)piscina_allocare(piscina,
        (memoriae_index)(summa + I) * magnitudo(ComitatusAcordum));
    si (k->acorda == NIHIL)
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            "piscina exhausta");
    }
    /* insertio ordine pulsus (stabilis) */
    per (e = ZEPHYRUM; e < elementa; e++)
    {
        per (j = ZEPHYRUM; j < numeri[e]; j++)
        {
            ComitatusAcordum x = lecta[e][j];
                         i32 l = k->numerus_acordorum;

            dum (l > ZEPHYRUM && k->acorda[l - I].pulsus > x.pulsus)
            {
                k->acorda[l] = k->acorda[l - I];
                l--;
            }
            k->acorda[l] = x;
            k->numerus_acordorum++;
        }
    }
    per (i = I; i < k->numerus_acordorum; i++)
    {
        si (   k->acorda[i].pulsus
            < k->acorda[i - I].pulsus + k->acorda[i - I].longitudo)
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I,
                k->acorda[i].offset, "acorda se tegunt");
        }
    }
    redde VERUM;
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

/* <fermata>: positio (mensura [ictus] aut pulsus) et factor 1..4
 * (ordinarium II). Finis et ordo postea (_coronas_finire), cum notae
 * lectae sunt */
interior b32
_coronam_legere (
             StmlNodus*  n,
                Carmen*  k,
    constans character** causa)
{
     CarmenCorona* f       = &k->coronae[k->numerus_coronarum];
           chorda* a       = stml_attributum_capere(n, "factor");
              f64  factor  = 2.0;

    si (!_positionem_legere(n, k, VERUM, &f->pulsus, causa))
    {
        redde FALSUM;
    }
    si (   a != NIHIL && (   !chorda_ut_f64(*a, &factor) || factor < 1.0
                       || factor > 4.0))
    {
        *causa = "fermata: factor 1..4";
        redde FALSUM;
    }
    f->factor  = (f32)factor;
    f->finis   = ZEPHYRUM;
    k->numerus_coronarum++;
    redde VERUM;
}

/* finis fermatae: initium notae proximae post eam (pars qualibet),
 * aut finis carminis. Fermata post finem aut ante finem prioris
 * (intra eam aut ordine inverso - prior finis > prior initium):
 * vitium */
interior b32
_coronas_finire (
                Carmen*  k,
    constans character** causa)
{
    i32 i;

    per (i = ZEPHYRUM; i < k->numerus_coronarum; i++)
    {
         CarmenCorona* f = &k->coronae[i];
                  i32  p;

        si (f->pulsus >= k->pulsus)
        {
            *causa = "fermata post finem carminis";
            redde FALSUM;
        }
        si (i > ZEPHYRUM && f->pulsus < k->coronae[i - I].finis)
        {
            *causa = "fermata: ordine positionis, non intra priorem";
            redde FALSUM;
        }
        f->finis = k->pulsus;
        per (p = ZEPHYRUM; p < k->numerus_partium; p++)
        {
            constans CarmenPars* pars = &k->partes[p];
                            i32  r;

            per (r = ZEPHYRUM; r < pars->numerus_frustorum; r++)
            {
                constans CarmenFrustum* fr = &pars->frusta[r];
                                   i32  q;

                per (q = ZEPHYRUM; q < fr->numerus; q++)
                {
                    s64 initium = fr->pulsus + fr->notae[q].pulsus;

                    si (initium > f->pulsus && initium < f->finis)
                    {
                        f->finis = initium;
                    }
                }
            }
        }
    }
    redde VERUM;
}

/* signa dynamicae et lucra eorum */
nomen structura {
    constans character* titulus;
                   f32  valor;
} IntensioNominata;

hic_manens constans IntensioNominata INTENSIONES_NOMINATAE[] = {
    { "pp", 0.2f }, { "p", 0.32f }, { "mp", 0.45f },
    { "mf", 0.6f }, { "f", 0.8f },  { "ff", 1.0f }
};

/* valor dynamicae: signum (pp..ff) aut numerus 0..1 */
interior b32
_intensionem_valere (
    chorda  textus,
       f32* valor)
{
    i32 nomina = (i32)(magnitudo(INTENSIONES_NOMINATAE)
               / magnitudo(INTENSIONES_NOMINATAE[ZEPHYRUM]));
    f64 x;
    i32 j;

    per (j = ZEPHYRUM; j < nomina; j++)
    {
        si (chorda_aequalis_literis(textus,
                                    INTENSIONES_NOMINATAE[j].titulus))
        {
            *valor = INTENSIONES_NOMINATAE[j].valor;
            redde VERUM;
        }
    }
    si (!chorda_ut_f64(textus, &x) || x < 0.0 || x > 1.0)
    {
        redde FALSUM;
    }
    *valor = (f32)x;
    redde VERUM;
}

/* pars in indice per titulum; -1 si ignota */
interior s32
_partis_index (
    constans Carmen* k,
             chorda  titulus)
{
    s32 j;

    per (j = ZEPHYRUM; j < (s32)k->numerus_partium; j++)
    {
        si (chorda_aequalis(k->partes[j].titulus, titulus))
        {
            redde j;
        }
    }
    redde -I;
}

/* pars in 'partes' (verba spatio separata) nominatur; partes vacua =
 * omnes */
interior b32
_pars_nominata (
    constans Carmen* k,
             chorda* partes,
                s32  pars)
{
    i32 i = ZEPHYRUM;

    si (partes == NIHIL)
    {
        redde VERUM;
    }
    dum (i < partes->mensura)
    {
        i32 initium;

        dum (i < partes->mensura && partes->datum[i] == ' ')
        {
            i++;
        }
        initium = i;
        dum (i < partes->mensura && partes->datum[i] != ' ')
        {
            i++;
        }
        si (   i > initium
            && chorda_aequalis(chorda_sectio(*partes, initium, i),
                               k->partes[pars].titulus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* nomina 'partes' omnia nota; FALSUM si quod ignotum */
interior b32
_partes_notae (
    constans Carmen* k,
             chorda  partes)
{
    i32 i = ZEPHYRUM;

    dum (i < partes.mensura)
    {
        i32 initium;

        dum (i < partes.mensura && partes.datum[i] == ' ')
        {
            i++;
        }
        initium = i;
        dum (i < partes.mensura && partes.datum[i] != ' ')
        {
            i++;
        }
        si (   i > initium
            && _partis_index(k, chorda_sectio(partes, initium, i)) < 0)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* frustum iteratum partis ex sectione ad positionem: notae scriptae
 * quae in [s->pulsus, s->usque) incipiunt, transpositae, litterae ex
 * clave; ordine pulsus inter frusta insertum. Nullae notae: nihil.
 * FALSUM: causa (meta non vacua, MIDI, piscina) */
interior b32
_frustum_iterare (
                   Carmen*  k,
                      s32   pars,
    constans CarmenSectio*  s,
                      s32   repetitio,
                  Piscina*  piscina,
       constans character** causa)
{
    constans CarmenRepetitio* r        = &k->repetitiones[repetitio];
                  CarmenPars* p        = &k->partes[pars];
                         s64  spatium  = s->usque - s->pulsus;
               CarmenFrustum  novum;
               CarmenFrustum* frusta;
                         i32  numerus = ZEPHYRUM;
                         i32  f;
                         i32  j;
                         i32  locus;

    per (f = ZEPHYRUM; f < p->numerus_frustorum; f++)
    {
        constans CarmenFrustum* fr = &p->frusta[f];

        per (j = ZEPHYRUM; fr->repetitio < ZEPHYRUM && j < fr->numerus;
             j++)
        {
            s64 a = fr->pulsus + fr->notae[j].pulsus;

            si (a >= s->pulsus && a < s->usque)
            {
                numerus++;
            }
        }
    }
    si (numerus == ZEPHYRUM)
    {
        redde VERUM;
    }
    per (f = ZEPHYRUM; f < p->numerus_frustorum; f++)
    {
        constans CarmenFrustum* fr = &p->frusta[f];

        si (   fr->pulsus < r->pulsus + spatium
            && r->pulsus < fr->pulsus + fr->longitudo)
        {
            *causa = "repetitio: pars ibi non vacua (frusta se tegunt)";
            redde FALSUM;
        }
    }
    memset(&novum, ZEPHYRUM, magnitudo(novum));
    novum.pulsus     = r->pulsus;
    novum.longitudo  = spatium;
    novum.repetitio  = repetitio;
    novum.scriptum   = -I;
    novum.comitatus  = -I;
    novum.notae      = (CarmenNota*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(CarmenNota));
    frusta = (CarmenFrustum*)piscina_allocare(piscina,
        (memoriae_index)(p->numerus_frustorum + I)
        * magnitudo(CarmenFrustum));
    si (!novum.notae || !frusta)
    {
        *causa = "piscina exhausta";
        redde FALSUM;
    }
    per (f = ZEPHYRUM; f < p->numerus_frustorum; f++)
    {
        constans CarmenFrustum* fr = &p->frusta[f];

        per (j = ZEPHYRUM; fr->repetitio < ZEPHYRUM && j < fr->numerus;
             j++)
        {
            CarmenNota x = fr->notae[j];
                   s64 a = fr->pulsus + x.pulsus;
                   s32 nota;

            si (a < s->pulsus || a >= s->usque)
            {
                perge;
            }
            nota = (s32)x.nota + r->semitonia;
            si (nota < ZEPHYRUM || nota > CXXVII)
            {
                *causa = "repetitio: nota transposita extra 0..127";
                redde FALSUM;
            }
            x.nota    = (i32)nota;
            x.pulsus  = a - s->pulsus;
            /* octavae litteras servant; aliter ex clave */
            x.littera = r->semitonia % XII != ZEPHYRUM ? '\0'
                                                       : x.littera;
            /* nota finem sectionis transiens (ligatura) truncatur */
            si (a + x.longitudo > s->usque)
            {
                x.longitudo = s->usque - a;
            }
            novum.notae[novum.numerus++] = x;
        }
    }
    /* portamentum sectione sectum: par (continuatur, portamentum)
     * manet solum si ambae iteratae et contiguae */
    per (j = ZEPHYRUM; j < novum.numerus; j++)
    {
        CarmenNota* x = &novum.notae[j];

        si (   x->continuatur
            && (   j + I >= novum.numerus
                || novum.notae[j + I].illapsus == ZEPHYRUM
                || novum.notae[j + I].pulsus != x->pulsus
                    + x->longitudo))
        {
            x->continuatur = FALSUM;
        }
    }
    per (j = ZEPHYRUM; j < novum.numerus; j++)
    {
        si (   novum.notae[j].illapsus > ZEPHYRUM
            && (j == ZEPHYRUM || !novum.notae[j - I].continuatur))
        {
            novum.notae[j].illapsus = ZEPHYRUM;
        }
    }
    /* insertio ordine pulsus */
    locus = ZEPHYRUM;
    dum (   locus < p->numerus_frustorum && p->frusta[locus].pulsus
                                         < novum.pulsus)
    {
        locus++;
    }
    per (f = ZEPHYRUM; f < locus; f++)
    {
        frusta[f] = p->frusta[f];
    }
    frusta[locus] = novum;
    per (f = locus; f < p->numerus_frustorum; f++)
    {
        frusta[f + I] = p->frusta[f];
    }
    p->frusta = frusta;
    p->numerus_frustorum++;
    redde VERUM;
}

/* <sectio> et <repetitio> (post partes). Sectio: titulus unus,
 * positio, usque post initium. Repetitio: sectio nota, positio,
 * semitonia -24..24, partes notae; expansio ordine libri; sectio
 * frusta iterata tegere non potest (fons = scripta solum) */
interior b32
_repetitiones_legere (
             StmlNodus*  nodus,
                Carmen*  k,
               Piscina*  piscina,
    constans character** causa)
{
    i32 i;
    i32 r;

    k->sectiones = (CarmenSectio*)piscina_allocare(piscina,
        (memoriae_index)(_numerare(nodus, "sectio") + I)
        * magnitudo(CarmenSectio));
    k->repetitiones = (CarmenRepetitio*)piscina_allocare(piscina,
        (memoriae_index)(_numerare(nodus, "repetitio") + I)
        * magnitudo(CarmenRepetitio));
    si (!k->sectiones || !k->repetitiones)
    {
        *causa = "piscina exhausta";
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
            StmlNodus* n = stml_liberum_ad_indicem(nodus, i);
         CarmenSectio* s = &k->sectiones[k->numerus_sectionum];
               chorda* a;
                  s64  u;
                  i32  j;

        si (!_elementum_est(n, "sectio"))
        {
            perge;
        }
        a = stml_attributum_capere(n, "titulus");
        si (a == NIHIL || !_signum_validum(*a))
        {
            *causa = "sectio sine titulo [a-z0-9_]+";
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < k->numerus_sectionum; j++)
        {
            si (chorda_aequalis(k->sectiones[j].titulus, *a))
            {
                *causa = "sectio: titulus iteratus";
                redde FALSUM;
            }
        }
        s->titulus = *a;
        si (!_positionem_legere(n, k, VERUM, &s->pulsus, causa))
        {
            redde FALSUM;
        }
        si (   !_s64_legere(n, "usque", &u) || u < I
            || u > CMXCIX * M)
        {
            *causa = "sectio: usque numerus mensurae";
            redde FALSUM;
        }
        s->usque = _pulsus_mensurae(k->metra, k->numerus_metrorum,
                                    (i32)u);
        si (s->usque <= s->pulsus)
        {
            *causa = "sectio: usque post initium";
            redde FALSUM;
        }
        k->numerus_sectionum++;
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
              StmlNodus* n = stml_liberum_ad_indicem(nodus, i);
        CarmenRepetitio* x = &k->repetitiones[k->numerus_repetitionum];
                 chorda* a;
                 chorda* partes;
                    s32  semitonia = ZEPHYRUM;
                    s32  j;

        si (!_elementum_est(n, "repetitio"))
        {
            perge;
        }
        a          = stml_attributum_capere(n, "sectio");
        x->sectio  = -I;
        per (j = ZEPHYRUM; a != NIHIL && j < (s32)k->numerus_sectionum;
             j++)
        {
            si (chorda_aequalis(k->sectiones[j].titulus, *a))
            {
                x->sectio = j;
            }
        }
        si (x->sectio < ZEPHYRUM)
        {
            *causa = "repetitio: sectio ignota";
            redde FALSUM;
        }
        si (!_positionem_legere(n, k, VERUM, &x->pulsus, causa))
        {
            redde FALSUM;
        }
        a = stml_attributum_capere(n, "semitonia");
        si (   a != NIHIL
            && (!chorda_ut_s32(*a, &semitonia) || semitonia < -XXIV
                || semitonia > XXIV))
        {
            *causa = "repetitio: semitonia -24..24";
            redde FALSUM;
        }
        x->semitonia  = semitonia;
        partes        = stml_attributum_capere(n, "partes");
        si (partes != NIHIL && !_partes_notae(k, *partes))
        {
            *causa = "repetitio: pars ignota";
            redde FALSUM;
        }
        k->numerus_repetitionum++;
        per (j = ZEPHYRUM; j < (s32)k->numerus_partium; j++)
        {
            si (   _pars_nominata(k, partes, j)
                && !_frustum_iterare(k, j, &k->sectiones[x->sectio],
                       (s32)(k->numerus_repetitionum - I), piscina,
                       causa))
            {
                redde FALSUM;
            }
        }
    }
    /* sectio nulla frusta iterata tegit (repetitio repetitionis) */
    per (r = ZEPHYRUM; r < k->numerus_repetitionum; r++)
    {
        constans CarmenSectio* s = &k->sectiones[
            k->repetitiones[r].sectio];

        per (i = ZEPHYRUM; i < k->numerus_partium; i++)
        {
            constans CarmenPars* p = &k->partes[i];
                            i32  f;

            per (f = ZEPHYRUM; f < p->numerus_frustorum; f++)
            {
                constans CarmenFrustum* fr = &p->frusta[f];

                si (   fr->repetitio >= ZEPHYRUM
                    && fr->pulsus < s->usque
                    && s->pulsus < fr->pulsus + fr->longitudo)
                {
                    *causa = "sectio frusta iterata tegit (repetitio "
                             "repetitionis non)";
                    redde FALSUM;
                }
            }
        }
    }
    redde VERUM;
}

/* <dynamica> omnes (post partes: pars per titulum). Ordine positionis
 * per partem; nulla mutatio intra clivum prioris; eadem positione
 * solum clivus post gradum */
interior b32
_intensiones_legere (
             StmlNodus*  nodus,
                Carmen*  k,
               Piscina*  piscina,
    constans character** causa)
{
    i32 i;

    k->intensiones = (CarmenIntensio*)piscina_allocare(piscina,
        (memoriae_index)(_numerare(nodus, "dynamica") + I)
        * magnitudo(CarmenIntensio));
    si (!k->intensiones)
    {
        *causa = "piscina exhausta";
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
             StmlNodus* n = stml_liberum_ad_indicem(nodus, i);
        CarmenIntensio* x = &k->intensiones[k->numerus_intensionum];
                chorda* a;
                   s64  u;
                   s32  j;

        si (!_elementum_est(n, "dynamica"))
        {
            perge;
        }
        x->pars  = -I;
        a        = stml_attributum_capere(n, "pars");
        si (a != NIHIL)
        {
            per (j = ZEPHYRUM; j < (s32)k->numerus_partium; j++)
            {
                si (chorda_aequalis(k->partes[j].titulus, *a))
                {
                    x->pars = j;
                }
            }
            si (x->pars < ZEPHYRUM)
            {
                *causa = "dynamica: pars ignota";
                redde FALSUM;
            }
        }
        si (!_positionem_legere(n, k, VERUM, &x->pulsus, causa))
        {
            redde FALSUM;
        }
        a = stml_attributum_capere(n, "valor");
        si (a == NIHIL || !_intensionem_valere(*a, &x->valor))
        {
            *causa = "dynamica: valor pp p mp mf f ff aut 0..1";
            redde FALSUM;
        }
        x->textus  = *a;
        x->usque   = ZEPHYRUM;
        si (stml_attributum_habet(n, "usque"))
        {
            si (   !_s64_legere(n, "usque", &u) || u < I
                || u > CMXCIX * M)
            {
                *causa = "dynamica: usque non numerus mensurae";
                redde FALSUM;
            }
            x->usque = _pulsus_mensurae(k->metra, k->numerus_metrorum,
                                        (i32)u);
            si (x->usque <= x->pulsus)
            {
                *causa = "dynamica: usque post initium clivi";
                redde FALSUM;
            }
        }
        /* prior eiusdem partis */
        per (j = (s32)k->numerus_intensionum - I; j >= ZEPHYRUM; j--)
        {
            constans CarmenIntensio* p = &k->intensiones[j];

            si (p->pars != x->pars)
            {
                perge;
            }
            si (p->usque != ZEPHYRUM && x->pulsus < p->usque)
            {
                *causa = "dynamica: mutatio intra clivum";
                redde FALSUM;
            }
            si (   x->pulsus < p->pulsus
                || (   x->pulsus == p->pulsus
                    && (x->usque == ZEPHYRUM || p->usque != ZEPHYRUM)))
            {
                *causa = "dynamica: ordine positionis per partem";
                redde FALSUM;
            }
            frange;
        }
        k->numerus_intensionum++;
    }
    redde VERUM;
}

/* valor dynamicae partis p (-1 = carminis) ad pulsum: 1 ante primam */
interior f64
_intensio_partis (
    constans Carmen* k,
                s32  pars,
                s64  pulsus)
{
    f64 prior = 1.0;
    i32 i;

    per (i = ZEPHYRUM; i < k->numerus_intensionum; i++)
    {
        constans CarmenIntensio* e = &k->intensiones[i];

        si (e->pars != pars)
        {
            perge;
        }
        si (e->pulsus > pulsus)
        {
            frange;
        }
        si (e->usque != ZEPHYRUM && pulsus < e->usque)
        {
            f64 t = (f64)(pulsus - e->pulsus)
                  / (f64)(e->usque - e->pulsus);

            si (prior > 0.0 && e->valor > 0.0f)
            {
                redde prior * pow((f64)e->valor / prior, t);
            }
            redde prior + ((f64)e->valor - prior) * t;
        }
        prior = (f64)e->valor;
    }
    redde prior;
}

/* gradum mappae sonandi addit; eadem positione ultimum substituit */
interior vacuum
_gradum_addere (
    CantusTempus* tempora,
             i32* numerus,
             s64  pulsus,
             f64  tempus)
{
    CantusTempus* t;

    si (*numerus > ZEPHYRUM && tempora[*numerus - I].pulsus == pulsus)
    {
        t = &tempora[*numerus - I];
    }
    alioquin
    {
        t = &tempora[*numerus];
        (*numerus)++;
    }
    t->pulsus      = pulsus;
    t->tempus      = tempus < 1.5 ? I : (i32)(tempus + 0.5);
    t->millesimae  = tempus < 0.0015 ? I : (i32)(tempus * 1000.0 + 0.5);
    t->quadrum     = ZEPHYRUM;
}

/* longitudo mensurae metri et finis spatii eius in carmine */
interior s64
_finis_metri (
    constans Carmen* k,
                i32  i)
{
    redde i + I < k->numerus_metrorum ? k->metra[i + I].pulsus
                                      : k->pulsus;
}

/* mappa sonandi MODULATA: lineae divisionis = gradus mappae, termini
 * coronarum, partes mensurarum sub agogica. Tempus ad lineam = tempus
 * mappae / (factor coronae x pondus partis) (millesimae, >= I) */
interior b32
_mappam_modulare (
     Carmen* k,
    Piscina* piscina)
{
    CantusTempus* nova;
             s64* lineae;
             i32  capacitas = k->numerus_temporum
                            + II * k->numerus_coronarum;
    i32 n        = ZEPHYRUM;
    i32 numerus  = ZEPHYRUM;
    i32 g        = ZEPHYRUM;   /* gradus mappae veteris */
    i32 f        = ZEPHYRUM;   /* corona */
    i32 r        = ZEPHYRUM;   /* metrum */
    i32 i;

    per (i = ZEPHYRUM; i < k->numerus_metrorum; i++)
    {
        constans CarmenMetrum* m = &k->metra[i];
                          s64  l = _longitudo_mensurae(m);
                          s64  spatium = _finis_metri(k, i) - m->pulsus;

        /* lineae partium spatii, et finis spatii semper (etiam spatio
         * nullo: carmen vacuum, metrum ad finem) */
        si (m->numerus_ponderum > ZEPHYRUM)
        {
            capacitas += I;
        }
        si (m->numerus_ponderum > ZEPHYRUM && spatium > ZEPHYRUM)
        {
            capacitas += (i32)((spatium + l - I) / l)
                       * m->numerus_ponderum;
        }
    }
    lineae = (s64*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(s64));
    nova   = (CantusTempus*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(CantusTempus));
    si (!lineae || !nova)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < k->numerus_temporum; i++)
    {
        lineae[n++] = k->tempora[i].pulsus;
    }
    per (i = ZEPHYRUM; i < k->numerus_coronarum; i++)
    {
        lineae[n++] = k->coronae[i].pulsus;
        lineae[n++] = k->coronae[i].finis;
    }
    per (i = ZEPHYRUM; i < k->numerus_metrorum; i++)
    {
        constans CarmenMetrum* m = &k->metra[i];
                          s64  pars;
                          s64  p;

        si (m->numerus_ponderum == ZEPHYRUM)
        {
            perge;
        }
        pars = _longitudo_mensurae(m) / (s64)m->numerus_ponderum;
        per (p = m->pulsus; p < _finis_metri(k, i); p += pars)
        {
            si (n >= capacitas)
            {
                redde FALSUM;
            }
            lineae[n++] = p;
        }
        /* finis spatii: metrum proximum (aequale fortasse) redit */
        si (n >= capacitas)
        {
            redde FALSUM;
        }
        lineae[n++] = _finis_metri(k, i);
    }
    /* ordo insertionis: catenae singulae iam ordinatae */
    per (i = I; i < n; i++)
    {
        s64 x = lineae[i];
        i32 j = i;

        dum (j > ZEPHYRUM && lineae[j - I] > x)
        {
            lineae[j] = lineae[j - I];
            j--;
        }
        lineae[j] = x;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        s64 x = lineae[i];
        f64 t;

        dum (   g + I < k->numerus_temporum
             && k->tempora[g + I].pulsus <= x)
        {
            g++;
        }
        dum (f < k->numerus_coronarum && k->coronae[f].finis <= x)
        {
            f++;
        }
        dum (   r + I < k->numerus_metrorum
             && k->metra[r + I].pulsus <= x)
        {
            r++;
        }
        t = (f64)k->tempora[g].millesimae / 1000.0;
        si (f < k->numerus_coronarum && k->coronae[f].pulsus <= x)
        {
            t = t / (f64)k->coronae[f].factor;
        }
        si (k->metra[r].numerus_ponderum > ZEPHYRUM)
        {
            constans CarmenMetrum* m = &k->metra[r];
                              s64  l = _longitudo_mensurae(m);
                              s64  q = ((x - m->pulsus) % l)
                                     / (l / (s64)m->numerus_ponderum);

            t = t / (f64)m->pondera[q];
        }
        _gradum_addere(nova, &numerus, x, t);
    }
    k->tempora           = nova;
    k->numerus_temporum  = numerus;
    redde VERUM;
}

/* mappa sonandi ex scriptis: gradus ut sunt; clivus = gradus per
 * CARMEN_GRADUS_TEMPORIS, tempus medii gradus (linearis in pulsibus,
 * millesimae exactae), deinde valor ad usque. FALSUM: piscina
 * exhausta */
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
            _gradum_addere(k->tempora, &numerus, t->pulsus,
                           (f64)t->tempus);
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

            _gradum_addere(k->tempora, &numerus, p, valor);
        }
        _gradum_addere(k->tempora, &numerus, t->usque, (f64)t->tempus);
        prius = t->tempus;
    }
    k->numerus_temporum = numerus;
    si (!_mappam_modulare(k, piscina))
    {
        redde FALSUM;
    }
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

#define CAMPI_NUMERUS XXIII

hic_manens constans CampusInstrumenti CAMPI[CAMPI_NUMERUS] = {
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
    { "motus",     -48.0f, 48.0f    },
    { "unisonus",    1.0f, 7.0f     },
    { "dispersio",   0.0f, 100.0f   },
    { "mora",        0.0f, 10.0f    }   /* vibrati */
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
    f32* campi[CAMPI_NUMERUS];

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
    campi[20]  = &p->unisonus;
    campi[21]  = &p->dispersio;
    campi[22]  = &p->mora_vibrati;
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
        per (k = ZEPHYRUM; k < CAMPI_NUMERUS; k++)
        {
            si (chorda_aequalis_literis(*t->titulus, CAMPI[k].titulus))
            {
                frange;
            }
        }
        si (k == CAMPI_NUMERUS)
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
            constans FiguraNominata* figurae_libri,
                                i32  numerus_figurarum_libri,
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
        FiguraNominata* figurae;
                   i32  numerus_figurarum;
                   b32  bassus_sonat;

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
    k->coronae = (CarmenCorona*)piscina_allocare(piscina,
        (memoriae_index)(_numerare(nodus, "fermata") + I)
        * magnitudo(CarmenCorona));
    si (!k->tempora_scripta || !k->metra || !k->claves || !k->coronae)
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
    si (!_inaequalitatem_attributi(nodus, &k->metra[ZEPHYRUM], &causa))
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            causa);
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
            && !_elementum_est(n, "fermata")
            && !_elementum_est(n, "dynamica")
            && !_elementum_est(n, "sectio")
            && !_elementum_est(n, "repetitio")
            && !_elementum_est(n, "pars")
            && !_elementum_est(n, "instrumentum")
            && !_elementum_est(n, "reverberatio")
            && !_elementum_est(n, "acorda")
            && !_elementum_est(n, "figura"))
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                "in carmine solum tempus, metrum, clavis, fermata, "
                "dynamica, sectio, repetitio, pars, instrumentum, "
                "reverberatio, acorda, figura");
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
        si (!_inaequalitatem_attributi(n, &k->metra[l], &causa))
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                causa);
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

        si (_elementum_est(n, "fermata"))
        {
            si (!_coronam_legere(n, k, &causa))
            {
                redde _vitium(vitium, k->titulus, _vacua(), -I,
                    ZEPHYRUM, causa);
            }
            perge;
        }
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
    /* figurae: carminis propriae primum (quaerendo vincunt), deinde
     * libri; acorda (mappa metri plena) ante partes */
    si (!_figuras_legere(nodus, k->titulus, piscina, &figurae,
                         &numerus_figurarum, vitium))
    {
        redde FALSUM;
    }
    {
        FiguraNominata* omnes = (FiguraNominata*)piscina_allocare(
            piscina, (memoriae_index)(numerus_figurarum
                                      + numerus_figurarum_libri + I)
            * magnitudo(FiguraNominata));

        si (omnes == NIHIL)
        {
            redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
                "piscina exhausta");
        }
        per (i = ZEPHYRUM; i < numerus_figurarum; i++)
        {
            omnes[i] = figurae[i];
        }
        per (i = ZEPHYRUM; i < numerus_figurarum_libri; i++)
        {
            omnes[numerus_figurarum + i] = figurae_libri[i];
        }
        figurae            = omnes;
        numerus_figurarum  += numerus_figurarum_libri;
    }
    si (!_acorda_legere(nodus, k, piscina, vitium))
    {
        redde FALSUM;
    }
    /* bassus comitatus: figura alicuius <comitatus> carminis vocem 'B'
     * habet - radicem in superioribus omittere licet (lilia:
     * harpa_bassus) */
    bassus_sonat = _bassus_comitatus(nodus, figurae, numerus_figurarum);
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
        si (!_partem_legere(n, k, figurae, numerus_figurarum,
                            bassus_sonat, piscina, &k->partes[j],
                            vitium))
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
    /* sectiones et repetitiones: frusta iterata inter scripta */
    si (!_repetitiones_legere(nodus, k, piscina, &causa))
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            causa);
    }
    /* longitudo: finis frusti ultimi (ordine pulsus) */
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
    /* dynamica (partes iam lectae: nomina), fermatae (notae), deinde
     * mappa sonandi */
    si (!_intensiones_legere(nodus, k, piscina, &causa))
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            causa);
    }
    si (!_coronas_finire(k, &causa))
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            causa);
    }
    si (!_mappam_sonandi(k, piscina))
    {
        redde _vitium(vitium, k->titulus, _vacua(), -I, ZEPHYRUM,
            "piscina exhausta");
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
            FiguraNominata* figurae;
                       i32  numerus_figurarum;

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
        si (   _elementum_est(n, "instrumentum")
            || _elementum_est(n, "figura"))
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
                "in carminibus solum carmina, instrumenta, figurae, "
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
    si (!_figuras_legere(radix, _vacua(), piscina, &figurae,
                         &numerus_figurarum, vitium))
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
            || _elementum_est(n, "figura")
            || _elementum_est(n, "reverberatio"))
        {
            perge;
        }
        si (!_carmen_legere(n, visibilia,
                            numerus_libri + numerus_instrumentarii,
                            figurae, numerus_figurarum,
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

f32
carmen_intensio (
    constans Carmen* carmen,
                s32  pars,
                s64  pulsus)
{
    f64 v = _intensio_partis(carmen, -I, pulsus);

    si (pars >= ZEPHYRUM && pars < (s32)carmen->numerus_partium)
    {
        v = v * _intensio_partis(carmen, pars, pulsus);
    }
    redde (f32)v;
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
