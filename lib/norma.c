/* norma.c - schema valoris JSON: aedificatores, visus, iudicium,
 * exportatio (norma-spec; norma-plan-2 N2/N3). Regulae viarum et
 * mutationum: project-specs/norma-plan-2.md, lib/norma.worklog.md. */
#include "norma.h"
#include "fasti.h"
#include "utf8.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

#define NORMA_VITIA_MAXIMA CCLVI

structura Norma {
          Piscina* piscina;
       NormaGenus  genus;
              b32  aut_nullum;
       NormaModus  modus;
            Norma* elementum;
              Xar* campi;
           chorda  clavis_discriminis;
              Xar* variationes;
              b32  habet_intra;
              s64  minimum;
              s64  maximum;
              b32  habet_intra_fluitans;
              f64  minimum_fluitans;
              f64  maximum_fluitans;
              b32  habet_longitudinem;
              i32  longitudo_minima;
              i32  longitudo_maxima;
              Xar* licita;
           chorda  forma;
           chorda  descriptio;
     NormaGignens  gignens;
           vacuum* gignens_datum;
           chorda  error_schematis;
};


/* ====================================================================
 * A. AEDIFICATORES ET VISUS
 * ==================================================================== */

interior chorda
_vacua (vacuum)
{
    chorda v;

    v.datum    = NIHIL;
    v.mensura  = 0;
    redde v;
}

interior Norma*
_nodus (
       Piscina* piscina,
    NormaGenus  genus)
{
    Norma* n;

    si (!piscina)
    {
        redde NIHIL;
    }
    n = (Norma*)piscina_allocare(piscina, (i64)magnitudo(Norma));
    memset(n, 0, magnitudo(*n));
    n->piscina  = piscina;
    n->genus    = genus;
    redde n;
}

interior Norma*
_pravus (
                 Norma* n,
    constans character* nuntius)
{
    si (n && n->error_schematis.mensura == 0)
    {
        n->error_schematis = chorda_ex_literis(nuntius, n->piscina);
    }
    redde n;
}

Norma*
norma_liberum (
    Piscina* piscina)
{ redde _nodus(piscina, NORMA_LIBERUM);
}

Norma*
norma_nullum (
    Piscina* piscina)
{ redde _nodus(piscina, NORMA_NULLUM);
}

Norma*
norma_boolean (
    Piscina* piscina)
{ redde _nodus(piscina, NORMA_BOOLEAN);
}

Norma*
norma_integer (
    Piscina* piscina)
{ redde _nodus(piscina, NORMA_INTEGER);
}

Norma*
norma_numerus (
    Piscina* piscina)
{ redde _nodus(piscina, NORMA_NUMERUS);
}

Norma*
norma_textus (
    Piscina* piscina)
{ redde _nodus(piscina, NORMA_TEXTUS);
}

Norma*
norma_tabulatum (
    Piscina* piscina,
      Norma* elementum)
{
    Norma* n = _nodus(piscina, NORMA_TABULATUM);

    si (!n)
    {
        redde NIHIL;
    }
    n->elementum = elementum;
    redde elementum ? n : _pravus(n, "tabulatum sine elemento");
}

Norma*
norma_objectum (
    Piscina* piscina)
{
    Norma* n = _nodus(piscina, NORMA_OBJECTUM);

    si (n)
    {
        n->campi = xar_creare(piscina, (i32)magnitudo(NormaCampus));
    }
    redde n;
}

Norma*
norma_campus (
                  Norma* objectum,
     constans character* titulus,
                  Norma* valor,
                    b32  requiritur)
{
    NormaCampus* c;
            i32  i;

    si (!objectum)
    {
        redde NIHIL;
    }
    si (objectum->genus != NORMA_OBJECTUM)
    {
        redde _pravus(objectum, "campus in nodo non objecto");
    }
    si (!titulus || !valor)
    {
        redde _pravus(objectum, "campus sine titulo aut valore");
    }
    per (i = 0; i < xar_numerus(objectum->campi); i++)
    {
        si (chorda_aequalis_literis(
                ((NormaCampus*)xar_obtinere(objectum->campi,
                i))->titulus, titulus))
        {
            redde _pravus(objectum, "campus duplicatus");
        }
    }
    c              = (NormaCampus*)xar_addere(objectum->campi);
    c->titulus     = chorda_ex_literis(titulus, objectum->piscina);
    c->valor       = valor;
    c->requiritur  = requiritur;
    redde objectum;
}

Norma*
norma_modus (
         Norma* n,
    NormaModus  modus)
{
    si (!n)
    {
        redde NIHIL;
    }
    si (n->genus != NORMA_OBJECTUM && n->genus != NORMA_DISCRIMEN)
    {
        redde _pravus(n, "modus solum in objecto aut discrimine");
    }
    n->modus = modus;
    redde n;
}

Norma*
norma_discrimen (
               Piscina* piscina,
    constans character* clavis)
{
    Norma* n = _nodus(piscina, NORMA_DISCRIMEN);

    si (!n)
    {
        redde NIHIL;
    }
    n->variationes = xar_creare(piscina, (i32)magnitudo(NormaVariatio));
    si (!clavis)
    {
        redde _pravus(n, "discrimen sine clave");
    }
    n->clavis_discriminis = chorda_ex_literis(clavis, piscina);
    redde n;
}

Norma*
norma_variatio (
                 Norma* discrimen,
    constans character* valor,
                 Norma* objectum)
{
    NormaVariatio* v;
              i32  i;

    si (!discrimen)
    {
        redde NIHIL;
    }
    si (discrimen->genus != NORMA_DISCRIMEN)
    {
        redde _pravus(discrimen, "variatio in nodo non discrimine");
    }
    si (!valor || !objectum || objectum->genus != NORMA_OBJECTUM)
    {
        redde _pravus(discrimen, "variatio sine valore aut objecto");
    }
    per (i = 0; i < xar_numerus(discrimen->variationes); i++)
    {
        si (chorda_aequalis_literis(
                ((NormaVariatio*)xar_obtinere(discrimen->variationes,
                i))->valor, valor))
        {
            redde _pravus(discrimen, "variatio duplicata");
        }
    }
    v            = (NormaVariatio*)xar_addere(discrimen->variationes);
    v->valor     = chorda_ex_literis(valor, discrimen->piscina);
    v->objectum  = objectum;
    redde discrimen;
}

Norma*
norma_aut_nullum (
    Norma* n)
{
    si (n)
    {
        n->aut_nullum = VERUM;
    }
    redde n;
}

Norma*
norma_intra (
    Norma* n,
      s64  minimum,
      s64  maximum)
{
    si (!n)
    {
        redde NIHIL;
    }
    si (n->genus != NORMA_INTEGER)
    {
        redde _pravus(n,
            "intra solum in integro (numerus: intra_fluitans)");
    }
    si (minimum > maximum)
    {
        redde _pravus(n, "intra: minimum > maximum");
    }
    n->habet_intra  = VERUM;
    n->minimum      = minimum;
    n->maximum      = maximum;
    redde n;
}

Norma*
norma_intra_fluitans (
    Norma* n,
      f64  minimum,
      f64  maximum)
{
    si (!n)
    {
        redde NIHIL;
    }
    si (n->genus != NORMA_NUMERUS)
    {
        redde _pravus(n, "intra_fluitans solum in numero");
    }
    si (minimum > maximum)
    {
        redde _pravus(n, "intra_fluitans: minimum > maximum");
    }
    n->habet_intra_fluitans  = VERUM;
    n->minimum_fluitans      = minimum;
    n->maximum_fluitans      = maximum;
    redde n;
}

Norma*
norma_longitudo (
    Norma* n,
      i32  minimum,
      i32  maximum)
{
    si (!n)
    {
        redde NIHIL;
    }
    si (n->genus != NORMA_TEXTUS && n->genus != NORMA_TABULATUM)
    {
        redde _pravus(n, "longitudo solum in textu aut tabulato");
    }
    si (minimum > maximum)
    {
        redde _pravus(n, "longitudo: minimum > maximum");
    }
    n->habet_longitudinem  = VERUM;
    n->longitudo_minima    = minimum;
    n->longitudo_maxima    = maximum;
    redde n;
}

Norma*
norma_electio (
    Norma* n,
    constans character* constans* licita)
{
    i32 i;

    si (!n)
    {
        redde NIHIL;
    }
    si (n->genus != NORMA_TEXTUS || !licita)
    {
        redde _pravus(n, "electio solum in textu, licita non NIHIL");
    }
    n->licita = xar_creare(n->piscina, (i32)magnitudo(chorda));
    per (i = 0; licita[i]; i++)
    {
        *(chorda*)xar_addere(n->licita) = chorda_ex_literis(licita[i],
            n->piscina);
    }
    redde n;
}

Norma*
norma_forma (
                 Norma* n,
    constans character* forma)
{
    si (!n)
    {
        redde NIHIL;
    }
    si (n->genus != NORMA_TEXTUS || !forma)
    {
        redde _pravus(n, "forma solum in textu");
    }
    n->forma = chorda_ex_literis(forma, n->piscina);
    redde n;
}

Norma*
norma_descriptio (
                 Norma* n,
    constans character* textus)
{
    si (n && textus)
    {
        n->descriptio = chorda_ex_literis(textus, n->piscina);
    }
    redde n;
}

Norma*
norma_gignens (
           Norma* n,
    NormaGignens  functio,
          vacuum* datum)
{
    si (n)
    {
        n->gignens        = functio;
        n->gignens_datum  = datum;
    }
    redde n;
}

NormaVisus
norma_visus (
    constans Norma* n)
{
    NormaVisus v;

    memset(&v, 0, magnitudo(v));
    si (!n)
    {
        v.error_schematis = _vacua();
        redde v;
    }
    v.genus                 = n->genus;
    v.aut_nullum            = n->aut_nullum;
    v.modus                 = n->modus;
    v.elementum             = n->elementum;
    v.campi                 = n->campi;
    v.clavis_discriminis    = n->clavis_discriminis;
    v.variationes           = n->variationes;
    v.habet_intra           = n->habet_intra;
    v.minimum               = n->minimum;
    v.maximum               = n->maximum;
    v.habet_intra_fluitans  = n->habet_intra_fluitans;
    v.minimum_fluitans      = n->minimum_fluitans;
    v.maximum_fluitans      = n->maximum_fluitans;
    v.habet_longitudinem    = n->habet_longitudinem;
    v.longitudo_minima      = n->longitudo_minima;
    v.longitudo_maxima      = n->longitudo_maxima;
    v.licita                = n->licita;
    v.forma                 = n->forma;
    v.descriptio            = n->descriptio;
    v.gignens               = n->gignens;
    v.gignens_datum         = n->gignens_datum;
    v.error_schematis       = n->error_schematis;
    redde v;
}


/* ====================================================================
 * B. IUDICIUM
 * ==================================================================== */

nomen structura {
     Piscina* piscina;
         Xar* vitia;
         Xar* notae;
         b32  cessatum;
} NormaContextus;

constans character*
norma_causa_descriptio (
    NormaCausa causa)
{
    commutatio (causa)
    {
        casus NORMA_CAUSA_GENUS:        redde "genus falsum";
        casus NORMA_CAUSA_DEEST:        redde "campus requisitus deest";
        casus NORMA_CAUSA_EXTRA:        redde "clavis non declarata";
        casus NORMA_CAUSA_MINIMUM:      redde "infra minimum";
        casus NORMA_CAUSA_MAXIMUM:      redde "supra maximum";
        casus NORMA_CAUSA_LONGITUDO:    redde "longitudo extra fines";
        casus NORMA_CAUSA_ELECTIO:      redde "extra electionem";
        casus NORMA_CAUSA_FORMA:        redde "forma fracta";
        casus NORMA_CAUSA_VARIATIO:     redde "variatio ignota";
        casus NORMA_CAUSA_DISCRIMEN:    redde "campus discriminis deest aut non textus";
        casus NORMA_CAUSA_LIMES:        redde "limes vitiorum: iudicium cessavit";
        casus NORMA_CAUSA_SCHEMA_PRAVA: redde "schema pravum";
        ordinarius:                     redde "causa ignota";
    }
}

interior vacuum
_notare (
             NormaContextus* cx,
                        b32  nota,
                     chorda  via,
                 NormaCausa  causa,
         constans character* nuntius)
{
            Xar* x = nota ? cx->notae : cx->vitia;
    NormaVitium* v;

    si (cx->cessatum)
    {
        redde;
    }
    si (xar_numerus(x) >= NORMA_VITIA_MAXIMA)
    {
        si (nota)
        {
            redde;
        }
        v         = (NormaVitium*)xar_addere(cx->vitia);
        v->via    = chorda_ex_literis("$", cx->piscina);
        v->causa  = NORMA_CAUSA_LIMES;
        v->nuntius =
            chorda_ex_literis(norma_causa_descriptio(NORMA_CAUSA_LIMES),
                                       cx->piscina);
        cx->cessatum = VERUM;
        redde;
    }
    v           = (NormaVitium*)xar_addere(x);
    v->via      = via;
    v->causa    = causa;
    v->nuntius  = chorda_ex_literis(nuntius, cx->piscina);
}

interior b32
_clavis_simplex (
    chorda k)
{
    i32 i;

    si (k.mensura == 0)
    {
        redde FALSUM;
    }
    per (i = 0; i < k.mensura; i++)
    {
        i8 c = k.datum[i];

        si (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            || c == '_'
              || (i > 0 && c >= '0' && c <= '9')))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior chorda
_via_clavis (
     chorda  via,
     chorda  clavis,
    Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(via.mensura + clavis.mensura + VIII));

    chorda_aedificator_appendere_chorda(aed, via);
    si (_clavis_simplex(clavis))
    {
        chorda_aedificator_appendere_character(aed, '.');
        chorda_aedificator_appendere_chorda(aed, clavis);
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(aed, "[\"");
        chorda_aedificator_appendere_evasus_json(aed, clavis);
        chorda_aedificator_appendere_literis(aed, "\"]");
    }
    redde chorda_aedificator_finire(aed);
}

interior chorda
_via_index (
     chorda  via,
        i32  index,
    Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(via.mensura + XVI));
    character numerus[XVI];

    chorda_aedificator_appendere_chorda(aed, via);
    sprintf(numerus, "[%u]", index);
    chorda_aedificator_appendere_literis(aed, numerus);
    redde chorda_aedificator_finire(aed);
}

interior constans character*
_genus_normae (
    NormaGenus g)
{
    commutatio (g)
    {
        casus NORMA_NULLUM:    redde "null";
        casus NORMA_BOOLEAN:   redde "boolean";
        casus NORMA_INTEGER:   redde "integer";
        casus NORMA_NUMERUS:   redde "numerus";
        casus NORMA_TEXTUS:    redde "textus";
        casus NORMA_TABULATUM: redde "tabulatum";
        casus NORMA_OBJECTUM:  redde "objectum";
        casus NORMA_DISCRIMEN: redde "objectum (discrimen)";
        ordinarius:            redde "quidvis";
    }
}

interior constans character*
_genus_valoris (
    JsonValor* v)
{
    commutatio (json_genus(v))
    {
        casus JSON_NULLUM:    redde "null";
        casus JSON_BOOLEAN:   redde "boolean";
        casus JSON_INTEGER:   redde "integer";
        casus JSON_FLUITANS:  redde "fluitans";
        casus JSON_CHORDA:    redde "textus";
        casus JSON_TABULATUM: redde "tabulatum";
        ordinarius:           redde "objectum";
    }
}

interior vacuum
_genus_falsum (
    NormaContextus* cx,
    constans Norma* n,
         JsonValor* v,
            chorda  via)
{
    character nuntius[CXXVIII];

    sprintf(nuntius, "exspectatur %s, inventum %s",
        _genus_normae(n->genus),
            v ? _genus_valoris(v) : "null");
    _notare(cx, FALSUM, via, NORMA_CAUSA_GENUS, nuntius);
}

interior b32
_hex (
    i8 c)
{
    redde (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F');
}

/* -1 forma ignota (non probatur), 0 fracta, 1 bona */
interior s32
_forma_probare (
    chorda forma,
    chorda t)
{
    i32 i;

    si (chorda_aequalis_literis(forma, "date-time"))
    {
        DiesHora dh;

        redde fasti_ex_iso(t, &dh) ? I : 0;
    }
    si (chorda_aequalis_literis(forma, "uuid"))
    {
        si (t.mensura != XXXVI)
        {
            redde 0;
        }
        per (i = 0; i < XXXVI; i++)
        {
            b32 linea = (i == VIII || i == XIII || i == XVIII
                || i == XXIII);

            si (linea ? t.datum[i] != '-' : !_hex(t.datum[i]))
            {
                redde 0;
            }
        }
        redde I;
    }
    si (chorda_aequalis_literis(forma, "email"))
    {
        s32 ad       = -I;
        s32 punctum  = -I;

        per (i = 0; i < t.mensura; i++)
        {
            si (t.datum[i] == '@')
            {
                si (ad >= 0)
                {
                    redde 0;
                }
                ad = (s32)i;
            }
            alioquin si (t.datum[i] == '.' && ad >= 0)
            {
                punctum = (s32)i;
            }
        }
        redde (ad > 0 && punctum > ad + I
            && punctum < (s32)t.mensura - I) ? I : 0;
    }
    si (chorda_aequalis_literis(forma, "uri"))
    {
        si (t.mensura == 0 || !((t.datum[0] >= 'a' && t.datum[0] <= 'z')
                                || (t.datum[0] >= 'A'
                                    && t.datum[0] <= 'Z')))
        {
            redde 0;
        }
        per (i = I; i < t.mensura; i++)
        {
            i8 c = t.datum[i];

            si (c == ':')
            {
                redde i + I < t.mensura ? I : 0;
            }
            si (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                  || (c >= '0' && c <= '9') || c == '+' || c == '.'
                  || c == '-'))
            {
                redde 0;
            }
        }
        redde 0;
    }
    redde -I;
}

interior vacuum
_iudicare (
                           constans Norma* n,
                                JsonValor* v,
                                   chorda  via,
                           NormaContextus* cx);

interior b32
_campus_declaratus (
    constans Norma* n,
            chorda  clavis)
{
    i32 i;

    per (i = 0; i < xar_numerus(n->campi); i++)
    {
        si (chorda_aequalis(((NormaCampus*)xar_obtinere(n->campi,
            i))->titulus, clavis))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_iudicare_objectum (
            constans Norma* n,
                 JsonValor* v,
                    chorda  via,
                    chorda  clavis_implicita,
            NormaContextus* cx)
{
    i32 i;

    per (i = 0; i < xar_numerus(n->campi); i++)
    {
        NormaCampus* c = (NormaCampus*)xar_obtinere(n->campi, i);
          JsonValor* f = json_objectum_capere_chorda(v, c->titulus);

        si (!f)
        {
            si (c->requiritur)
            {
                _notare(cx, FALSUM, _via_clavis(via, c->titulus,
                    cx->piscina),
                        NORMA_CAUSA_DEEST, "campus requisitus deest");
            }
            perge;
        }
        _iudicare(c->valor, f, _via_clavis(via, c->titulus,
            cx->piscina), cx);
    }
    si (n->modus != NORMA_APERTUM)
    {
        JsonObjectumIterator  it = json_objectum_iterator(v);
                      chorda  k;
                   JsonValor* f;

        dum (json_objectum_iterator_proxima(&it, &k, &f))
        {
            si (   clavis_implicita.mensura > 0
                && chorda_aequalis(k, clavis_implicita))
            {
                perge;
            }
            si (!_campus_declaratus(n, k))
            {
                _notare(cx, n->modus == NORMA_NOTANDUM,
                        _via_clavis(via, k, cx->piscina),
                        NORMA_CAUSA_EXTRA,
                        "clavis non declarata");
            }
        }
    }
}

interior vacuum
_iudicare_discrimen (
                     constans Norma* n,
                          JsonValor* v,
                             chorda  via,
                     NormaContextus* cx)
{
       chorda via_tag = _via_clavis(via, n->clavis_discriminis,
           cx->piscina);
    JsonValor* tag = json_objectum_capere_chorda(v,
        n->clavis_discriminis);
          i32 i;

    si (!tag || !json_est_chorda(tag))
    {
        _notare(cx, FALSUM, via_tag, NORMA_CAUSA_DISCRIMEN,
                norma_causa_descriptio(NORMA_CAUSA_DISCRIMEN));
        redde;
    }
    per (i = 0; i < xar_numerus(n->variationes); i++)
    {
        NormaVariatio* var =
            (NormaVariatio*)xar_obtinere(n->variationes, i);

        si (chorda_aequalis(var->valor, json_ad_chorda(tag)))
        {
            _iudicare_objectum(var->objectum, v, via,
                n->clavis_discriminis, cx);
            redde;
        }
    }
    si (n->modus != NORMA_APERTUM)
    {
        _notare(cx, n->modus == NORMA_NOTANDUM, via_tag,
            NORMA_CAUSA_VARIATIO,
                "variatio ignota");
    }
}

interior vacuum
_iudicare_textum (
    constans Norma* n,
         JsonValor* v,
            chorda  via,
    NormaContextus* cx)
{
    chorda t      = json_ad_chorda(v);
       s32 runae  = utf8_numerare_runas(t.datum, (s32)t.mensura);
       i32 i;

    si (   n->habet_longitudinem
        && (runae < (s32)n->longitudo_minima
        || runae > (s32)n->longitudo_maxima))
    {
        _notare(cx, FALSUM, via, NORMA_CAUSA_LONGITUDO,
            "longitudo textus extra fines");
    }
    si (n->licita)
    {
        b32 inventum = FALSUM;

        per (i = 0; i < xar_numerus(n->licita); i++)
        {
            si (chorda_aequalis(*(chorda*)xar_obtinere(n->licita, i),
                t))
            {
                inventum = VERUM;
            }
        }
        si (!inventum)
        {
            _notare(cx, FALSUM, via, NORMA_CAUSA_ELECTIO,
                "valor extra electionem");
        }
    }
    si (n->forma.mensura > 0 && _forma_probare(n->forma, t) == 0)
    {
        _notare(cx, FALSUM, via, NORMA_CAUSA_FORMA, "forma fracta");
    }
}

interior vacuum
_iudicare (
    constans Norma* n,
         JsonValor* v,
            chorda  via,
    NormaContextus* cx)
{
    si (cx->cessatum || n->genus == NORMA_LIBERUM)
    {
        redde;
    }
    si (!v || json_est_nullum(v))
    {
        si (n->genus != NORMA_NULLUM && !n->aut_nullum)
        {
            _genus_falsum(cx, n, v, via);
        }
        redde;
    }
    commutatio (n->genus)
    {
        casus NORMA_NULLUM:
            _genus_falsum(cx, n, v, via);
            frange;
        casus NORMA_BOOLEAN:
            si (!json_est_boolean(v))
            {
                _genus_falsum(cx, n, v, via);
            }
            frange;
        casus NORMA_INTEGER:
            si (!json_est_integer(v))
            {
                _genus_falsum(cx, n, v, via);
                frange;
            }
            si (n->habet_intra && json_ad_integer(v) < n->minimum)
            {
                _notare(cx, FALSUM, via, NORMA_CAUSA_MINIMUM,
                    "infra minimum");
            }
            si (n->habet_intra && json_ad_integer(v) > n->maximum)
            {
                _notare(cx, FALSUM, via, NORMA_CAUSA_MAXIMUM,
                    "supra maximum");
            }
            frange;
        casus NORMA_NUMERUS:
        {
            f64 x;

            si (!json_est_integer(v) && !json_est_fluitans(v))
            {
                _genus_falsum(cx, n, v, via);
                frange;
            }
            x =
                json_est_integer(v) ? (f64)json_ad_integer(v) : json_ad_fluitans(v);
            si (n->habet_intra_fluitans && x < n->minimum_fluitans)
            {
                _notare(cx, FALSUM, via, NORMA_CAUSA_MINIMUM,
                    "infra minimum");
            }
            si (n->habet_intra_fluitans && x > n->maximum_fluitans)
            {
                _notare(cx, FALSUM, via, NORMA_CAUSA_MAXIMUM,
                    "supra maximum");
            }
            frange;
        }
        casus NORMA_TEXTUS:
            si (!json_est_chorda(v))
            {
                _genus_falsum(cx, n, v, via);
                frange;
            }
            _iudicare_textum(n, v, via, cx);
            frange;
        casus NORMA_TABULATUM:
        {
            i32 i;
            i32 numerus;

            si (!json_est_tabulatum(v))
            {
                _genus_falsum(cx, n, v, via);
                frange;
            }
            numerus = json_tabulatum_numerus(v);
            si (   n->habet_longitudinem
                && (numerus < n->longitudo_minima
                || numerus > n->longitudo_maxima))
            {
                _notare(cx, FALSUM, via, NORMA_CAUSA_LONGITUDO,
                        "numerus elementorum extra fines");
            }
            per (i = 0; i < numerus && !cx->cessatum; i++)
            {
                _iudicare(n->elementum, json_tabulatum_obtinere(v, i),
                          _via_index(via, i, cx->piscina), cx);
            }
            frange;
        }
        casus NORMA_OBJECTUM:
            si (!json_est_objectum(v))
            {
                _genus_falsum(cx, n, v, via);
                frange;
            }
            _iudicare_objectum(n, v, via, _vacua(), cx);
            frange;
        casus NORMA_DISCRIMEN:
            si (!json_est_objectum(v))
            {
                _genus_falsum(cx, n, v, via);
                frange;
            }
            _iudicare_discrimen(n, v, via, cx);
            frange;
        ordinarius:
            frange;
    }
}

interior chorda
_pravitas (
    constans Norma* n)
{
       i32 i;
    chorda e;

    si (!n)
    {
        redde _vacua();
    }
    si (n->error_schematis.mensura > 0)
    {
        redde n->error_schematis;
    }
    si (n->genus == NORMA_TABULATUM)
    {
        redde _pravitas(n->elementum);
    }
    per (i = 0; n->campi && i < xar_numerus(n->campi); i++)
    {
        e = _pravitas(((NormaCampus*)xar_obtinere(n->campi, i))->valor);
        si (e.mensura > 0)
        {
            redde e;
        }
    }
    per (i = 0; n->variationes && i < xar_numerus(n->variationes); i++)
    {
        e = _pravitas(((NormaVariatio*)xar_obtinere(n->variationes,
            i))->objectum);
        si (e.mensura > 0)
        {
            redde e;
        }
    }
    redde _vacua();
}

NormaIudicium
norma_iudicare (
    constans Norma* n,
         JsonValor* valor,
           Piscina* piscina)
{
     NormaIudicium j;
    NormaContextus cx;
            chorda pravitas;

    memset(&j, 0, magnitudo(j));
    j.vitia      = xar_creare(piscina, (i32)magnitudo(NormaVitium));
    j.notae      = xar_creare(piscina, (i32)magnitudo(NormaVitium));
    cx.piscina   = piscina;
    cx.vitia     = j.vitia;
    cx.notae     = j.notae;
    cx.cessatum  = FALSUM;
    pravitas = n ? _pravitas(n) : chorda_ex_literis("schema NIHIL",
        piscina);
    si (pravitas.mensura > 0)
    {
        NormaVitium* v = (NormaVitium*)xar_addere(j.vitia);

        v->via      = chorda_ex_literis("$", piscina);
        v->causa    = NORMA_CAUSA_SCHEMA_PRAVA;
        v->nuntius  = pravitas;
        j.validum   = FALSUM;
        redde j;
    }
    _iudicare(n, valor, chorda_ex_literis("$", piscina), &cx);
    j.validum = xar_numerus(j.vitia) == 0;
    redde j;
}
