/* norma_gignere.c - valores JSON ficti ex schemate (norma-spec par. IV;
 * norma-plan-2 N4). Semina PER VIAM: clavis = VIII octeti primi
 * sigilli (SHA-256) textus viae. Viae = eaedem ac iudicis (copia
 * auxiliorum norma.c; oraculum aequalitatem probat). */
#include "norma_gignere.h"
#include "fictio.h"
#include "sigillum.h"
#include "utf8.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

nomen structura {
               Piscina* piscina;
                  Sors  radix;
    NormaModusGignendi  modus;   /* TYPICA aut FINES */
} Gignitor;

/* ---- viae (copia norma.c) ---- */

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

interior chorda
_via_suffixa (
                chorda  via,
    constans character* suffixum,
               Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(via.mensura + XVI));

    chorda_aedificator_appendere_chorda(aed, via);
    chorda_aedificator_appendere_literis(aed, suffixum);
    redde chorda_aedificator_finire(aed);
}

interior Sors
_sors_viae (
    constans Gignitor* g,
               chorda  via)
{
    Sigillum s = sigillum_computare(via.datum,
        (memoriae_index)via.mensura);
         i64 clavis = 0;
         i32 i;

    per (i = 0; i < VIII; i++)
    {
        clavis = (clavis << VIII) | (i64)s.octeti[i];
    }
    redde sors_derivare(&g->radix, clavis);
}

/* textus ex runis 'fons' cyclice usque ad 'longitudo' runas */
interior chorda
_runis_implere (
     chorda  fons,
        i32  longitudo,
    Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(longitudo * IV + I));
    constans i8* p;
    constans i8* finis;
            i32  n = 0;

    si (fons.mensura == 0)
    {
        fons = chorda_ex_literis("a", piscina);
    }
    p      = fons.datum;
    finis  = fons.datum + fons.mensura;
    dum (n < longitudo)
    {
        constans i8* q = utf8_proxima_runa(p, finis);

        si (q <= p)
        {
            q = p + I;
        }
        dum (p < q)
        {
            chorda_aedificator_appendere_character(aed, (character)*p);
            p++;
        }
        n++;
        si (p >= finis)
        {
            p = fons.datum;
        }
    }
    redde chorda_aedificator_finire(aed);
}

interior JsonValor*
_gignere (
    constans Gignitor* g,
       constans Norma* n,
               chorda  via);

interior JsonValor*
_textum_gignere (
    constans Gignitor* g,
           NormaVisus  v,
               chorda  via)
{
       Sors  s = _sors_viae(g, via);
    Piscina* p = g->piscina;
        i32  minimum =
            v.habet_longitudinem ? v.longitudo_minima : III;
        i32 maximum =
            v.habet_longitudinem ? v.longitudo_maxima : XII;
        i32 longitudo;
     chorda fons;

    si (v.licita && xar_numerus(v.licita) > 0)
    {
        i32 numerus = xar_numerus(v.licita);
        i32 k = g->modus == NORMA_FINES ? (sors_casu(&s, I,
            II) ? 0 : numerus - I)
                                        : sors_intra(&s, numerus);

        redde json_chorda_creare(p, *(chorda*)xar_obtinere(v.licita,
            k));
    }
    si (chorda_aequalis_literis(v.forma, "date-time"))
    {
        redde json_chorda_creare(p, fictio_tempus(&s, 0,
            (s64)4102444800, p));
    }
    si (chorda_aequalis_literis(v.forma, "uuid"))
    {
        redde json_chorda_creare(p, fictio_uuid(&s, p));
    }
    si (chorda_aequalis_literis(v.forma, "email"))
    {
        redde json_chorda_creare(p, fictio_email(&s, p));
    }
    si (chorda_aequalis_literis(v.forma, "uri"))
    {
        ChordaAedificator* aed = chorda_aedificator_creare(p, LXIV);

        chorda_aedificator_appendere_literis(aed,
            "https://example.org/");
        chorda_aedificator_appendere_chorda(aed, fictio_textus(&s, III,
            VIII, NIHIL, p));
        redde json_chorda_creare(p, chorda_aedificator_finire(aed));
    }
    longitudo = g->modus == NORMA_FINES
              ? (sors_casu(&s, I, II) ? minimum : maximum)
              : (i32)fictio_integer(&s, (s64)minimum, (s64)maximum);
    fons = g->modus == NORMA_FINES ? fictio_textus_difficilis(&s, p)
                                   : fictio_textus_latinus(&s, I, p);
    redde json_chorda_creare(p, _runis_implere(fons, longitudo, p));
}

interior JsonValor*
_objectum_gignere (
        constans Gignitor* g,
           constans Norma* n,
                   chorda  via,
                   chorda  tag,
                   chorda  valor_tag)
{
    Piscina*   p      = g->piscina;
    NormaVisus v      = norma_visus(n);
    JsonValor* o      = json_objectum_creare(p);
    Sors       s      = _sors_viae(g, via);
    b32        omnes  = sors_casu(&s, I, II);   /* FINES: omnes aut nulli */
    i32        i;

    si (tag.mensura > 0)
    {
        json_objectum_ponere_chorda(o, tag, json_chorda_creare(p,
            valor_tag));
    }
    per (i = 0; v.campi && i < xar_numerus(v.campi); i++)
    {
        NormaCampus* c      = (NormaCampus*)xar_obtinere(v.campi, i);
             chorda  via_c  = _via_clavis(via, c->titulus, p);
                b32  adest  = c->requiritur;

        si (!adest)
        {
            Sors sp = _sors_viae(g, _via_suffixa(via_c, "?", p));

            adest = g->modus == NORMA_FINES ? omnes : sors_casu(&sp, I,
                II);
        }
        si (adest)
        {
            json_objectum_ponere_chorda(o, c->titulus, _gignere(g,
                c->valor, via_c));
        }
    }
    redde o;
}

interior JsonValor*
_gignere (
    constans Gignitor* g,
       constans Norma* n,
               chorda  via)
{
       Piscina* p = g->piscina;
    NormaVisus  v = norma_visus(n);
          Sors  s = _sors_viae(g, via);

    si (v.gignens)
    {
        redde v.gignens(&s, p, v.gignens_datum);
    }
    si (   v.aut_nullum && v.genus != NORMA_NULLUM
        && (g->modus == NORMA_FINES ? sors_casu(&s, I,
        II) : sors_casu(&s, I, X)))
    {
        redde json_nullum_creare(p);
    }
    commutatio (v.genus)
    {
        casus NORMA_LIBERUM:
            si (g->modus == NORMA_FINES)
            {
                redde json_nullum_creare(p);
            }
            redde sors_casu(&s, I, II)
                ? json_integer_creare(p, fictio_integer(&s, 0, M))
                : json_chorda_creare(p, fictio_textus_latinus(&s, I,
                p));
        casus NORMA_NULLUM:
            redde json_nullum_creare(p);
        casus NORMA_BOOLEAN:
            redde json_boolean_creare(p, sors_casu(&s, I, II));
        casus NORMA_INTEGER:
        {
            s64 minimum = v.habet_intra ? v.minimum : 0;
            s64 maximum = v.habet_intra ? v.maximum : M;

            redde json_integer_creare(p, g->modus == NORMA_FINES
                ? (sors_casu(&s, I, II) ? minimum : maximum)
                : fictio_integer(&s, minimum, maximum));
        }
        casus NORMA_NUMERUS:
        {
            f64 minimum =
                v.habet_intra_fluitans ? v.minimum_fluitans : 0.0;
            f64 maximum =
                v.habet_intra_fluitans ? v.maximum_fluitans : 1000.0;

            redde json_fluitans_creare(p, g->modus == NORMA_FINES
                ? (sors_casu(&s, I, II) ? minimum : maximum)
                : fictio_numerus(&s, minimum, maximum));
        }
        casus NORMA_TEXTUS:
            redde _textum_gignere(g, v, via);
        casus NORMA_TABULATUM:
        {
            JsonValor* arr = json_tabulatum_creare(p);
                  i32  numerus;
                  i32  i;

            si (v.habet_longitudinem)
            {
                i32 summum = v.longitudo_maxima < v.longitudo_minima
                    + II
                           ? v.longitudo_maxima : v.longitudo_minima
                               + II;

                numerus = g->modus == NORMA_FINES
                        ? (sors_casu(&s, I,
                        II) ? v.longitudo_minima : v.longitudo_maxima)
                        : (i32)fictio_integer(&s,
                        (s64)v.longitudo_minima, (s64)summum);
            }
            alioquin
            {
                numerus = g->modus == NORMA_FINES ? (sors_casu(&s, I,
                    II) ? 0 : III)
                                                  : (i32)fictio_integer(&s,
                                                  I, III);
            }
            per (i = 0; i < numerus; i++)
            {
                json_tabulatum_addere(arr, _gignere(g, v.elementum,
                    _via_index(via, i, p)));
            }
            redde arr;
        }
        casus NORMA_OBJECTUM:
            redde _objectum_gignere(g, n, via, chorda_ex_literis("", p),
                                    chorda_ex_literis("", p));
        casus NORMA_DISCRIMEN:
        {
            NormaVariatio* var;

            si (!v.variationes || xar_numerus(v.variationes) == 0)
            {
                redde json_objectum_creare(p);
            }
            var = (NormaVariatio*)xar_obtinere(v.variationes,
                      sors_intra(&s, xar_numerus(v.variationes)));
            redde _objectum_gignere(g, var->objectum, via,
                v.clavis_discriminis,
                                    var->valor);
        }
        ordinarius:
            redde json_nullum_creare(p);
    }
}


/* ====================================================================
 * INVALIDA - mutationes, vitium UNUM quaeque
 * ==================================================================== */

nomen enumeratio {
    MUTATIO_GENUS = 0,
    MUTATIO_DEEST,
    MUTATIO_EXTRA,
    MUTATIO_MINIMUM,
    MUTATIO_MAXIMUM,
    MUTATIO_BREVIUS,
    MUTATIO_LONGIUS,
    MUTATIO_ELECTIO,
    MUTATIO_FORMA,
    MUTATIO_VARIATIO,
    MUTATIO_SINE_TAG
} MutatioGenus;

nomen structura {
     MutatioGenus genus;
    constans Norma* n;
        JsonValor* valor;     /* valor mutandus (aut parens pro DEEST/SINE_TAG) */
           chorda  clavis;    /* DEEST/SINE_TAG: clavis removenda */
           chorda  via;       /* via vitii a iudice reddenda */
} Mutatio;

interior b32
_forma_nota (
    chorda forma)
{
    redde chorda_aequalis_literis(forma, "date-time")
        || chorda_aequalis_literis(forma, "uuid")
        || chorda_aequalis_literis(forma, "email")
        || chorda_aequalis_literis(forma, "uri");
}

interior vacuum
_addere (
               Xar* m,
      MutatioGenus  genus,
    constans Norma* n,
         JsonValor* valor,
            chorda  clavis,
            chorda  via)
{
    Mutatio* x = (Mutatio*)xar_addere(m);

    x->genus   = genus;
    x->n       = n;
    x->valor   = valor;
    x->clavis  = clavis;
    x->via     = via;
}

/* textus 'quid' (runae) intra fines nodi? */
interior b32
_intra_longitudinem (
    NormaVisus v,
           i32 runae)
{
    redde !v.habet_longitudinem
        || (runae >= v.longitudo_minima && runae <= v.longitudo_maxima);
}

interior vacuum
_colligere (
                                Xar* m,
                     constans Norma* n,
                          JsonValor* valor,
                             chorda  via,
                            Piscina* p);

interior vacuum
_colligere_objectum (
                        Xar* m,
             constans Norma* n,
                  JsonValor* valor,
                     chorda  via,
                     chorda  tag,
                    Piscina* p)
{
    NormaVisus v = norma_visus(n);
           i32 i;

    si (v.modus == NORMA_CLAUSUM)
    {
        chorda extranea = chorda_ex_literis("clavis_extranea", p);

        _addere(m, MUTATIO_EXTRA, n, valor, extranea, _via_clavis(via,
            extranea, p));
    }
    per (i = 0; v.campi && i < xar_numerus(v.campi); i++)
    {
        NormaCampus* c = (NormaCampus*)xar_obtinere(v.campi, i);
          JsonValor* f = json_objectum_capere_chorda(valor,
              c->titulus);
             chorda via_c = _via_clavis(via, c->titulus, p);

        si (!f)
        {
            perge;
        }
        si (c->requiritur)
        {
            _addere(m, MUTATIO_DEEST, c->valor, valor, c->titulus,
                via_c);
        }
        _colligere(m, c->valor, f, via_c, p);
    }
    (vacuum)tag;
}

interior vacuum
_colligere (
               Xar* m,
    constans Norma* n,
         JsonValor* valor,
            chorda  via,
           Piscina* p)
{
    NormaVisus v = norma_visus(n);
           i32 i;

    si (   !valor || json_est_nullum(valor) || v.genus == NORMA_LIBERUM
        || v.gignens)
    {
        redde;   /* null licitum, liberum, functio vocantis: non mutantur */
    }
    _addere(m, MUTATIO_GENUS, n, valor, chorda_ex_literis("", p), via);
    commutatio (v.genus)
    {
        casus NORMA_INTEGER:
            si (v.habet_intra && v.minimum > (s64)((i64)I << LXIII))
            {
                _addere(m, MUTATIO_MINIMUM, n, valor,
                    chorda_ex_literis("", p), via);
            }
            si (   v.habet_intra
                && v.maximum < (s64)(((i64)I << LXIII) - I))
            {
                _addere(m, MUTATIO_MAXIMUM, n, valor,
                    chorda_ex_literis("", p), via);
            }
            frange;
        casus NORMA_NUMERUS:
            si (v.habet_intra_fluitans)
            {
                _addere(m, MUTATIO_MINIMUM, n, valor,
                    chorda_ex_literis("", p), via);
                _addere(m, MUTATIO_MAXIMUM, n, valor,
                    chorda_ex_literis("", p), via);
            }
            frange;
        casus NORMA_TEXTUS:
            si (v.licita)
            {
                si (_intra_longitudinem(v, XVI))
                {
                    _addere(m, MUTATIO_ELECTIO, n, valor,
                        chorda_ex_literis("", p), via);
                }
            }
            alioquin si (_forma_nota(v.forma))
            {
                si (_intra_longitudinem(v, IX))
                {
                    _addere(m, MUTATIO_FORMA, n, valor,
                        chorda_ex_literis("", p), via);
                }
            }
            alioquin si (v.habet_longitudinem)
            {
                si (v.longitudo_minima > 0)
                {
                    _addere(m, MUTATIO_BREVIUS, n, valor,
                        chorda_ex_literis("", p), via);
                }
                _addere(m, MUTATIO_LONGIUS, n, valor,
                    chorda_ex_literis("", p), via);
            }
            frange;
        casus NORMA_TABULATUM:
            si (v.habet_longitudinem)
            {
                si (v.longitudo_minima > 0)
                {
                    _addere(m, MUTATIO_BREVIUS, n, valor,
                        chorda_ex_literis("", p), via);
                }
                _addere(m, MUTATIO_LONGIUS, n, valor,
                    chorda_ex_literis("", p), via);
            }
            per (i = 0; i < json_tabulatum_numerus(valor); i++)
            {
                _colligere(m, v.elementum,
                    json_tabulatum_obtinere(valor, i),
                           _via_index(via, i, p), p);
            }
            frange;
        casus NORMA_OBJECTUM:
            _colligere_objectum(m, n, valor, via, chorda_ex_literis("",
                p), p);
            frange;
        casus NORMA_DISCRIMEN:
        {
            JsonValor* tag = json_objectum_capere_chorda(valor,
                v.clavis_discriminis);
               chorda via_tag = _via_clavis(via,
                   v.clavis_discriminis,
                   p);

            _addere(m, MUTATIO_SINE_TAG, n, valor, v.clavis_discriminis,
                via_tag);
            si (v.modus == NORMA_CLAUSUM)
            {
                _addere(m, MUTATIO_VARIATIO, n, valor,
                    v.clavis_discriminis, via_tag);
            }
            per (i = 0; tag && i < xar_numerus(v.variationes); i++)
            {
                NormaVariatio* var =
                    (NormaVariatio*)xar_obtinere(v.variationes, i);

                si (chorda_aequalis(var->valor, json_ad_chorda(tag)))
                {
                    _colligere_objectum(m, var->objectum, valor, via,
                                        v.clavis_discriminis, p);
                }
            }
            frange;
        }
        ordinarius:
            frange;
    }
}

/* objectum sine clave (API json removendi caret: aedificatur novum) */
interior vacuum
_removere (
    JsonValor* objectum,
       chorda  clavis,
      Piscina* p)
{
               JsonValor* novum  = json_objectum_creare(p);
    JsonObjectumIterator  it     = json_objectum_iterator(objectum);
                  chorda  k;
               JsonValor* f;

    dum (json_objectum_iterator_proxima(&it, &k, &f))
    {
        si (!chorda_aequalis(k, clavis))
        {
            json_objectum_ponere_chorda(novum, k, f);
        }
    }
    *objectum = *novum;
}

interior NormaCausa
_mutare (
    constans Gignitor* g,
              Mutatio* x)
{
       Piscina* p = g->piscina;
    NormaVisus  v = norma_visus(x->n);

    commutatio (x->genus)
    {
        casus MUTATIO_GENUS:
            *x->valor = v.genus
                == NORMA_TEXTUS ? *json_integer_creare(p, VII)
                                                : *json_chorda_creare_literis(p,
                                                "x");
            redde NORMA_CAUSA_GENUS;
        casus MUTATIO_DEEST:
            _removere(x->valor, x->clavis, p);
            redde NORMA_CAUSA_DEEST;
        casus MUTATIO_EXTRA:
            json_objectum_ponere_chorda(x->valor, x->clavis,
                json_integer_creare(p, I));
            redde NORMA_CAUSA_EXTRA;
        casus MUTATIO_MINIMUM:
            *x->valor = v.genus == NORMA_INTEGER
                ? *json_integer_creare(p, v.minimum - I)
                : *json_fluitans_creare(p, v.minimum_fluitans - 1.0);
            redde NORMA_CAUSA_MINIMUM;
        casus MUTATIO_MAXIMUM:
            *x->valor = v.genus == NORMA_INTEGER
                ? *json_integer_creare(p, v.maximum + I)
                : *json_fluitans_creare(p, v.maximum_fluitans + 1.0);
            redde NORMA_CAUSA_MAXIMUM;
        casus MUTATIO_BREVIUS:
        casus MUTATIO_LONGIUS:
        {
            i32 meta = x->genus == MUTATIO_BREVIUS ? v.longitudo_minima
                - I
                                                   : v.longitudo_maxima
                                                       + I;

            si (v.genus == NORMA_TEXTUS)
            {
                *x->valor = *json_chorda_creare(p,
                    _runis_implere(json_ad_chorda(x->valor), meta, p));
            }
            alioquin
            {
                JsonValor* arr = json_tabulatum_creare(p);
                      i32  i;

                per (i = 0; i < meta; i++)
                {
                    json_tabulatum_addere(arr, i
                        < json_tabulatum_numerus(x->valor)
                        ? json_tabulatum_obtinere(x->valor, i)
                        : _gignere(g, v.elementum, _via_index(x->via, i,
                        p)));
                }
                *x->valor = *arr;
            }
            redde NORMA_CAUSA_LONGITUDO;
        }
        casus MUTATIO_ELECTIO:
            *x->valor = *json_chorda_creare_literis(p,
                "extra_electionem");
            redde NORMA_CAUSA_ELECTIO;
        casus MUTATIO_FORMA:
            *x->valor = *json_chorda_creare_literis(p, "non forma");
            redde NORMA_CAUSA_FORMA;
        casus MUTATIO_VARIATIO:
            json_objectum_ponere_chorda(x->valor, x->clavis,
                                        json_chorda_creare_literis(p,
                                        "variatio_ignota"));
            redde NORMA_CAUSA_VARIATIO;
        casus MUTATIO_SINE_TAG:
            _removere(x->valor, x->clavis, p);
            redde NORMA_CAUSA_DISCRIMEN;
        ordinarius:
            redde NORMA_CAUSA_GENUS;
    }
}

NormaGenitum
norma_gignere (
        constans Norma* n,
    NormaModusGignendi  modus,
                   s64  semen,
               Piscina* piscina)
{
    NormaGenitum r;
        Gignitor g;
          chorda radix_via = chorda_ex_literis("$", piscina);

    memset(&r, 0, magnitudo(r));
    r.via_fracta  = chorda_ex_literis("", piscina);
    g.piscina     = piscina;
    g.modus       = modus == NORMA_FINES ? NORMA_FINES : NORMA_TYPICA;
    sors_seminare(&g.radix, (i64)semen, 0);
    r.valor = _gignere(&g, n, radix_via);
    si (modus == NORMA_INVALIDA)
    {
            Xar* m = xar_creare(piscina, (i32)magnitudo(Mutatio));
           Sors  s = _sors_viae(&g, chorda_ex_literis("INVALIDA",
               piscina));
        Mutatio* x;

        _colligere(m, n, r.valor, radix_via, piscina);
        si (xar_numerus(m) == 0)
        {
            r.valor = NIHIL;
            redde r;
        }
        x = (Mutatio*)xar_obtinere(m, sors_intra(&s, xar_numerus(m)));
        r.causa_fracta = _mutare(&g, x);
        r.via_fracta = x->via;
    }
    redde r;
}
