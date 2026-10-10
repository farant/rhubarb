/* norma_inferre.c - schema ex exemplis inferre (norma-spec-3; B1.1:
 * figurae, additio, coniunctio, regulae). Adumbratio, non veritas:
 * requisitum = in omni instantia parentis; claves ordine octetorum
 * (ordo exemplorum adumbrationem non mutat - lex coniunctionis). */
#include "norma_inferre.h"
#include "utf8.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

/* formae probatae, ordine praeferentiae; bit i = forma i adhuc viva */
#define INFERENTIA_FORMAE IV
hic_manens constans character* constans _formae[INFERENTIA_FORMAE] = {
    "date-time", "uuid", "email", "uri"
};

nomen structura Figura Figura;

/* discrimen (norma-spec-3 §IV): valor tag et partitio eius */
nomen structura {
     chorda  valor;
        i32  instantiae;
     Figura* figura;    /* partitio: instantiae objecti cum hoc valore */
} ValorDiscriminis;

nomen structura {
    chorda  clavis;
       b32  vivus;      /* in omni instantia, semper textus, valores pauci */
       Xar* valores;    /* ValorDiscriminis */
} CandidatusDiscriminis;

nomen structura {
               chorda  titulus;
                  i32  praesentia;   /* instantiae objecti cum clave */
               Figura* figura;
} CampusFigurae;

structura Figura {
                  i32 instantiae;          /* valores visi hic */
                  i32 genera[VII];         /* per JsonGenus */
    /* textus */
                  b32  habet_longitudinem;
                  i32  longitudo_minima;    /* runae */
                  i32  longitudo_maxima;
                  Xar* distincti;           /* chorda, usque ad limitem */
                  b32  distincti_superati;
                  i32  formae_vivae;        /* bit per _formae */
    /* numeri */
                  b32 habet_integrum;
                  s64 integrum_minimum;
                  s64 integrum_maximum;
                  b32 habet_fluitans;
                  f64 fluitans_minimum;
                  f64 fluitans_maximum;
    /* tabulatum */
               Figura* elementum;
                  b32  habet_tabulatum;
                  i32  tabulatum_minimum;
                  i32  tabulatum_maximum;
    /* objectum */
                  Xar* campi;               /* CampusFigurae */
                  Xar* candidati;           /* CandidatusDiscriminis */
                  b32  partitio;            /* VERUM: figura partitionis -
                                             * candidatos SUOS non servat
                                             * (aliter se ipsam in
                                             * infinitum partiretur) */
};

structura Inferentia {
               Piscina* piscina;
    InferentiaOptiones  optiones;
                   i32  numerus;
                Figura* radix;
                 Norma* formae[INFERENTIA_FORMAE];   /* iudices formarum */
};


/* ====================================================================
 * A. FIGURAE
 * ==================================================================== */

interior Figura*
_figura_nova (
    Piscina* p)
{
    Figura* f = (Figura*)piscina_allocare(p,
        (memoriae_index)magnitudo(Figura));

    memset(f, 0, magnitudo(*f));
    f->distincti  = xar_creare(p, (i32)magnitudo(chorda));
    f->campi      = xar_creare(p, (i32)magnitudo(CampusFigurae));
    f->candidati = xar_creare(p,
        (i32)magnitudo(CandidatusDiscriminis));
    f->formae_vivae = ((i32)I << INFERENTIA_FORMAE) - I;
    redde f;
}

/* limes distinctorum: maximum electionis et discriminis, plus unus */
interior i32
_limes_distinctorum (
    constans InferentiaOptiones* o)
{
    redde (o->electio_maxima > o->discrimen_maximum ? o->electio_maxima
           : o->discrimen_maximum) + I;
}

interior vacuum
_distinctum_addere (
                        Inferentia* inf,
                            Figura* f,
                            chorda  valor)
{
    i32 i;

    si (f->distincti_superati)
    {
        redde;
    }
    per (i = 0; i < xar_numerus(f->distincti); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(f->distincti, i),
            valor))
        {
            redde;
        }
    }
    si (xar_numerus(f->distincti)
        >= _limes_distinctorum(&inf->optiones))
    {
        f->distincti_superati = VERUM;
        redde;
    }
    *(chorda*)xar_addere(f->distincti) = chorda_transcribere(valor,
        inf->piscina);
}

interior CampusFigurae*
_campum_invenire (
           Inferentia* inf,
               Figura* f,
               chorda  titulus,
                  b32  creare)
{
        CampusFigurae* c;
                  i32  i;

    per (i = 0; i < xar_numerus(f->campi); i++)
    {
        c = (CampusFigurae*)xar_obtinere(f->campi, i);
        si (chorda_aequalis(c->titulus, titulus))
        {
            redde c;
        }
    }
    si (!creare)
    {
        redde NIHIL;
    }
    c              = (CampusFigurae*)xar_addere(f->campi);
    c->titulus     = chorda_transcribere(titulus, inf->piscina);
    c->praesentia  = 0;
    c->figura      = _figura_nova(inf->piscina);
    redde c;
}

interior vacuum
_figuram_addere (
    Inferentia* inf,
        Figura* f,
     JsonValor* v);

interior Figura*
_partitio_nova (
    Piscina* p)
{
    Figura* f = _figura_nova(p);

    f->partitio = VERUM;
    redde f;
}

interior ValorDiscriminis*
_valorem_invenire (
    CandidatusDiscriminis* c,
                   chorda  valor)
{
    i32 i;

    per (i = 0; i < xar_numerus(c->valores); i++)
    {
        ValorDiscriminis* vd =
            (ValorDiscriminis*)xar_obtinere(c->valores,
            i);

        si (chorda_aequalis(vd->valor, valor))
        {
            redde vd;
        }
    }
    redde NIHIL;
}

interior JsonValor*
_valor_clavis (
     JsonValor* objectum,
        chorda  clavis)
{
    JsonObjectumIterator  it = json_objectum_iterator(objectum);
                  chorda  k;
               JsonValor* valor;

    dum (json_objectum_iterator_proxima(&it, &k, &valor))
    {
        si (chorda_aequalis(k, clavis))
        {
            redde valor;
        }
    }
    redde NIHIL;
}

/* candidati ex instantia PRIMA nascuntur (claves textus); deinde quisque
 * moritur si clavis deest, non textus est, aut valores > maximum */
interior vacuum
_candidatos_addere (
    Inferentia* inf,
        Figura* f,
     JsonValor* v)
{
    i32 i;

    si (f->genera[JSON_OBJECTUM] == I)
    {
        JsonObjectumIterator  it = json_objectum_iterator(v);
                      chorda  k;
                   JsonValor* valor;

        dum (json_objectum_iterator_proxima(&it, &k, &valor))
        {
            si (json_genus(valor) == JSON_CHORDA)
            {
                CandidatusDiscriminis* c = (CandidatusDiscriminis*)
                    xar_addere(f->candidati);

                c->clavis  = chorda_transcribere(k, inf->piscina);
                c->vivus   = VERUM;
                c->valores  = xar_creare(inf->piscina,
                    (i32)magnitudo(ValorDiscriminis));
            }
        }
    }
    per (i = 0; i < xar_numerus(f->candidati); i++)
    {
        CandidatusDiscriminis* c = (CandidatusDiscriminis*)xar_obtinere(
            f->candidati, i);
                   JsonValor* t;
            ValorDiscriminis* vd;

        si (!c->vivus)
        {
            perge;
        }
        t = _valor_clavis(v, c->clavis);
        si (!t || json_genus(t) != JSON_CHORDA)
        {
            c->vivus = FALSUM;
            perge;
        }
        vd = _valorem_invenire(c, json_ad_chorda(t));
        si (!vd)
        {
            si (xar_numerus(c->valores)
                >= inf->optiones.discrimen_maximum)
            {
                c->vivus = FALSUM;
                perge;
            }
            vd = (ValorDiscriminis*)xar_addere(c->valores);
            vd->valor       = chorda_transcribere(json_ad_chorda(t),
                inf->piscina);
            vd->instantiae  = 0;
            vd->figura      = _partitio_nova(inf->piscina);
        }
        vd->instantiae++;
        _figuram_addere(inf, vd->figura, v);
    }
}

interior vacuum
_figuram_addere (
           Inferentia* inf,
               Figura* f,
            JsonValor* v)
{
    JsonGenus g = json_genus(v);
          i32 i;

    f->instantiae++;
    f->genera[g]++;
    commutatio (g)
    {
        casus JSON_CHORDA:
        {
            chorda s = json_ad_chorda(v);
               i32 lr = (i32)utf8_numerare_runas(s.datum,
                   (s32)s.mensura);

            si (!f->habet_longitudinem || lr < f->longitudo_minima)
            {
                f->longitudo_minima = lr;
            }
            si (!f->habet_longitudinem || lr > f->longitudo_maxima)
            {
                f->longitudo_maxima = lr;
            }
            f->habet_longitudinem = VERUM;
            _distinctum_addere(inf, f, s);
            per (i = 0; i < INFERENTIA_FORMAE; i++)
            {
                si (   (f->formae_vivae & ((i32)I << i))
                    && !norma_iudicare(inf->formae[i], v,
                        inf->piscina).validum)
                {
                    f->formae_vivae &= ~((i32)I << i);
                }
            }
            frange;
        }
        casus JSON_INTEGER:
        {
            s64 n = json_ad_integer(v);

            si (!f->habet_integrum || n < f->integrum_minimum)
            {
                f->integrum_minimum = n;
            }
            si (!f->habet_integrum || n > f->integrum_maximum)
            {
                f->integrum_maximum = n;
            }
            f->habet_integrum = VERUM;
            frange;
        }
        casus JSON_FLUITANS:
        {
            f64 x = json_ad_fluitans(v);

            si (!f->habet_fluitans || x < f->fluitans_minimum)
            {
                f->fluitans_minimum = x;
            }
            si (!f->habet_fluitans || x > f->fluitans_maximum)
            {
                f->fluitans_maximum = x;
            }
            f->habet_fluitans = VERUM;
            frange;
        }
        casus JSON_TABULATUM:
        {
            i32 n = json_tabulatum_numerus(v);

            si (!f->habet_tabulatum || n < f->tabulatum_minimum)
            {
                f->tabulatum_minimum = n;
            }
            si (!f->habet_tabulatum || n > f->tabulatum_maximum)
            {
                f->tabulatum_maximum = n;
            }
            f->habet_tabulatum = VERUM;
            per (i = 0; i < n; i++)
            {
                si (!f->elementum)
                {
                    f->elementum = _figura_nova(inf->piscina);
                }
                _figuram_addere(inf, f->elementum,
                    json_tabulatum_obtinere(v, i));
            }
            frange;
        }
        casus JSON_OBJECTUM:
        {
            JsonObjectumIterator  it = json_objectum_iterator(v);
                          chorda  clavis;
                       JsonValor* valor;

            dum (json_objectum_iterator_proxima(&it, &clavis, &valor))
            {
                CampusFigurae* c = _campum_invenire(inf, f, clavis,
                    VERUM);

                c->praesentia++;
                _figuram_addere(inf, c->figura, valor);
            }
            si (!f->partitio)
            {
                _candidatos_addere(inf, f, v);
            }
            frange;
        }
        ordinarius:
            frange;
    }
}

interior vacuum
_figuras_miscere (
         Inferentia* inf,
             Figura* dest,
    constans Figura* fons);

/* candidatus vivit si in utraque parte vivit (lex: idem ac additio
 * singula); dest vacua -> candidati fontis copiantur */
interior vacuum
_candidatos_miscere (
         Inferentia* inf,
             Figura* dest,
    constans Figura* fons,
                b32  dest_vacua)
{
    i32 i;
    i32 k;

    si (dest_vacua)
    {
        per (i = 0; i < xar_numerus(fons->candidati); i++)
        {
            CandidatusDiscriminis* fc = (CandidatusDiscriminis*)
                xar_obtinere(fons->candidati, i);
            CandidatusDiscriminis* dc = (CandidatusDiscriminis*)
                xar_addere(dest->candidati);

            dc->clavis  = chorda_transcribere(fc->clavis,
                inf->piscina);
            dc->vivus   = fc->vivus;
            dc->valores  = xar_creare(inf->piscina,
                (i32)magnitudo(ValorDiscriminis));
            per (k = 0; k < xar_numerus(fc->valores); k++)
            {
                ValorDiscriminis* fv = (ValorDiscriminis*)xar_obtinere(
                    fc->valores, k);
                ValorDiscriminis* dv = (ValorDiscriminis*)xar_addere(
                    dc->valores);

                dv->valor       = chorda_transcribere(fv->valor,
                    inf->piscina);
                dv->instantiae  = fv->instantiae;
                dv->figura      = _partitio_nova(inf->piscina);
                _figuras_miscere(inf, dv->figura, fv->figura);
            }
        }
        redde;
    }
    per (i = 0; i < xar_numerus(dest->candidati); i++)
    {
        CandidatusDiscriminis* dc =
            (CandidatusDiscriminis*)xar_obtinere(
            dest->candidati, i);
        CandidatusDiscriminis* fc = NIHIL;

        si (!dc->vivus)
        {
            perge;
        }
        per (k = 0; k < xar_numerus(fons->candidati); k++)
        {
            CandidatusDiscriminis* x = (CandidatusDiscriminis*)
                xar_obtinere(fons->candidati, k);

            si (chorda_aequalis(x->clavis, dc->clavis))
            {
                fc = x;
            }
        }
        si (!fc || !fc->vivus)
        {
            dc->vivus = FALSUM;
            perge;
        }
        per (k = 0; k < xar_numerus(fc->valores) && dc->vivus; k++)
        {
            ValorDiscriminis* fv = (ValorDiscriminis*)xar_obtinere(
                fc->valores, k);
            ValorDiscriminis* dv = _valorem_invenire(dc, fv->valor);

            si (!dv)
            {
                si (xar_numerus(dc->valores)
                    >= inf->optiones.discrimen_maximum)
                {
                    dc->vivus = FALSUM;
                    frange;
                }
                dv = (ValorDiscriminis*)xar_addere(dc->valores);
                dv->valor       = chorda_transcribere(fv->valor,
                    inf->piscina);
                dv->instantiae  = 0;
                dv->figura      = _partitio_nova(inf->piscina);
            }
            dv->instantiae += fv->instantiae;
            _figuras_miscere(inf, dv->figura, fv->figura);
        }
    }
}

/* fons in destinationem miscetur (coniunctio); fons non mutatur */
interior vacuum
_figuras_miscere (
           Inferentia* inf,
               Figura* dest,
      constans Figura* fons)
{
    i32 i;
    b32 dest_vacua = dest->genera[JSON_OBJECTUM] == 0;
    b32 fons_vacua = fons->genera[JSON_OBJECTUM] == 0;

    /* candidati: ante summas generum (vacuitas ex numeris veteribus) */
    si (!dest->partitio && !fons_vacua)
    {
        _candidatos_miscere(inf, dest, fons, dest_vacua);
    }
    dest->instantiae += fons->instantiae;
    per (i = 0; i < VII; i++)
    {
        dest->genera[i] += fons->genera[i];
    }
    si (fons->habet_longitudinem)
    {
        si (   !dest->habet_longitudinem
            || fons->longitudo_minima < dest->longitudo_minima)
        {
            dest->longitudo_minima = fons->longitudo_minima;
        }
        si (   !dest->habet_longitudinem
            || fons->longitudo_maxima > dest->longitudo_maxima)
        {
            dest->longitudo_maxima = fons->longitudo_maxima;
        }
        dest->habet_longitudinem = VERUM;
    }
    si (fons->distincti_superati)
    {
        dest->distincti_superati = VERUM;
    }
    per (i = 0; i < xar_numerus(fons->distincti); i++)
    {
        _distinctum_addere(inf, dest, *(chorda*)xar_obtinere(
            fons->distincti, i));
    }
    dest->formae_vivae &= fons->formae_vivae;
    si (fons->habet_integrum)
    {
        si (   !dest->habet_integrum
            || fons->integrum_minimum < dest->integrum_minimum)
        {
            dest->integrum_minimum = fons->integrum_minimum;
        }
        si (   !dest->habet_integrum
            || fons->integrum_maximum > dest->integrum_maximum)
        {
            dest->integrum_maximum = fons->integrum_maximum;
        }
        dest->habet_integrum = VERUM;
    }
    si (fons->habet_fluitans)
    {
        si (   !dest->habet_fluitans
            || fons->fluitans_minimum < dest->fluitans_minimum)
        {
            dest->fluitans_minimum = fons->fluitans_minimum;
        }
        si (   !dest->habet_fluitans
            || fons->fluitans_maximum > dest->fluitans_maximum)
        {
            dest->fluitans_maximum = fons->fluitans_maximum;
        }
        dest->habet_fluitans = VERUM;
    }
    si (fons->habet_tabulatum)
    {
        si (   !dest->habet_tabulatum
            || fons->tabulatum_minimum < dest->tabulatum_minimum)
        {
            dest->tabulatum_minimum = fons->tabulatum_minimum;
        }
        si (   !dest->habet_tabulatum
            || fons->tabulatum_maximum > dest->tabulatum_maximum)
        {
            dest->tabulatum_maximum = fons->tabulatum_maximum;
        }
        dest->habet_tabulatum = VERUM;
    }
    si (fons->elementum)
    {
        si (!dest->elementum)
        {
            dest->elementum = _figura_nova(inf->piscina);
        }
        _figuras_miscere(inf, dest->elementum, fons->elementum);
    }
    per (i = 0; i < xar_numerus(fons->campi); i++)
    {
        CampusFigurae* cf = (CampusFigurae*)xar_obtinere(
            fons->campi, i);
        CampusFigurae* cd = _campum_invenire(inf, dest, cf->titulus,
            VERUM);

        cd->praesentia += cf->praesentia;
        _figuras_miscere(inf, cd->figura, cf->figura);
    }
}


/* ====================================================================
 * B. INFERENTIA
 * ==================================================================== */

InferentiaOptiones
inferentia_optiones_ordinariae (vacuum)
{
    InferentiaOptiones o;

    memset(&o, 0, magnitudo(o));
    o.modus              = NORMA_NOTANDUM;
    o.formae             = VERUM;
    o.electio            = FALSUM;
    o.fines              = FALSUM;
    o.exempla_minima     = V;
    o.electio_maxima     = VIII;
    o.discrimen_maximum  = XVI;
    redde o;
}

Inferentia*
inferentia_creare (
                        Piscina* piscina,
    constans InferentiaOptiones* optiones)
{
    Inferentia* inf;
           i32  i;

    si (!piscina)
    {
        redde NIHIL;
    }
    inf = (Inferentia*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(Inferentia));
    memset(inf, 0, magnitudo(*inf));
    inf->piscina = piscina;
    inf->optiones =
        optiones ? *optiones : inferentia_optiones_ordinariae();
    si (inf->optiones.exempla_minima == 0)
    {
        inf->optiones.exempla_minima = V;
    }
    si (inf->optiones.electio_maxima == 0)
    {
        inf->optiones.electio_maxima = VIII;
    }
    si (inf->optiones.discrimen_maximum == 0)
    {
        inf->optiones.discrimen_maximum = XVI;
    }
    inf->radix = _figura_nova(piscina);
    per (i = 0; i < INFERENTIA_FORMAE; i++)
    {
        inf->formae[i] = norma_forma(norma_textus(piscina), _formae[i]);
    }
    redde inf;
}

vacuum
inferentia_addere (
    Inferentia* inferentia,
     JsonValor* exemplum)
{
    si (!inferentia || !exemplum)
    {
        redde;
    }
    inferentia->numerus++;
    _figuram_addere(inferentia, inferentia->radix, exemplum);
}

Inferentia*
inferentia_coniungere (
    constans Inferentia* a,
    constans Inferentia* b,
                Piscina* piscina)
{
    Inferentia* c;

    si (!a || !b)
    {
        redde NIHIL;
    }
    c = inferentia_creare(piscina, &a->optiones);
    si (!c)
    {
        redde NIHIL;
    }
    c->numerus = a->numerus + b->numerus;
    _figuras_miscere(c, c->radix, a->radix);
    _figuras_miscere(c, c->radix, b->radix);
    redde c;
}

i32
inferentia_numerus (
    constans Inferentia* inferentia)
{
    redde inferentia ? inferentia->numerus : 0;
}


/* ====================================================================
 * C. ADUMBRATIO
 * ==================================================================== */

/* ordo octetorum (memcmp, deinde longitudo) */
interior s32
_comparare_chordas (
    chorda a,
    chorda b)
{
    i32 n = a.mensura < b.mensura ? a.mensura : b.mensura;
    s32 r = n > 0 ? (s32)memcmp(a.datum, b.datum, (size_t)n) : 0;

    si (r != 0)
    {
        redde r;
    }
    redde a.mensura < b.mensura ? -I : (a.mensura > b.mensura ? I : 0);
}


/* ====================================================================
 * D. TESTIMONIA (norma-spec-4 §III)
 * ==================================================================== */

nomen structura {
     constans Norma* nodus;
             chorda  textus;
} TestimoniumNodi;

structura InferentiaTestimonia {
    Piscina* piscina;
        Xar* paria;     /* TestimoniumNodi */
};

interior TestimoniumNodi*
_testimonium_invenire (
    InferentiaTestimonia* t,
          constans Norma* nodus)
{
    i32 i;

    per (i = 0; i < xar_numerus(t->paria); i++)
    {
        TestimoniumNodi* tn = (TestimoniumNodi*)xar_obtinere(t->paria,
            i);

        si (tn->nodus == nodus)
        {
            redde tn;
        }
    }
    redde NIHIL;
}

interior vacuum
_testimonium_ponere (
    InferentiaTestimonia* t,
          constans Norma* nodus,
                  chorda  textus)
{
    TestimoniumNodi* tn;

    si (!t || !nodus)
    {
        redde;
    }
    tn = _testimonium_invenire(t, nodus);
    si (!tn)
    {
        tn         = (TestimoniumNodi*)xar_addere(t->paria);
        tn->nodus  = nodus;
    }
    tn->textus = textus;
}

/* praefixum ante testimonium nodi ('visum 3/3; ...') */
interior vacuum
_testimonium_praeponere (
    InferentiaTestimonia* t,
          constans Norma* nodus,
      constans character* praefixum)
{
      TestimoniumNodi* tn;
    ChordaAedificator* a;

    si (!t || !nodus)
    {
        redde;
    }
    tn  = _testimonium_invenire(t, nodus);
    a   = chorda_aedificator_creare(t->piscina, CXXVIII);
    chorda_aedificator_appendere_literis(a, praefixum);
    si (tn && tn->textus.mensura > 0)
    {
        chorda_aedificator_appendere_literis(a, "; ");
        chorda_aedificator_appendere_chorda(a, tn->textus);
    }
    _testimonium_ponere(t, nodus, chorda_aedificator_finire(a));
}

hic_manens constans character* constans _genera_nomina[VII] = {
    "nullum", "boolean", "integer", "fluitans", "textus", "tabulatum",
    "objectum"
};

interior vacuum
_pars_addere (
     ChordaAedificator* a,
    constans character* pars)
{
    si (chorda_aedificator_longitudo(a) > 0)
    {
        chorda_aedificator_appendere_literis(a, "; ");
    }
    chorda_aedificator_appendere_literis(a, pars);
}

interior Norma*
_figuram_normam (
     constans Inferentia* inf,
    InferentiaTestimonia* t,
         constans Figura* f,
                 Piscina* p);

interior i32
_requisita (
    constans Figura* f)
{
    i32 i;
    i32 n = 0;

    per (i = 0; i < xar_numerus(f->campi); i++)
    {
        si (((CampusFigurae*)xar_obtinere(f->campi, i))->praesentia
            == f->genera[JSON_OBJECTUM])
        {
            n++;
        }
    }
    redde n;
}

interior b32
_claves_aequales (
    constans Figura* a,
    constans Figura* b)
{
    i32 i;
    i32 k;

    si (xar_numerus(a->campi) != xar_numerus(b->campi))
    {
        redde FALSUM;
    }
    per (i = 0; i < xar_numerus(a->campi); i++)
    {
        chorda t = ((CampusFigurae*)xar_obtinere(a->campi, i))->titulus;
           b32 inventa = FALSUM;

        per (k = 0; k < xar_numerus(b->campi) && !inventa; k++)
        {
            inventa = chorda_aequalis(t, ((CampusFigurae*)xar_obtinere(
                b->campi, k))->titulus);
        }
        si (!inventa)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* §IV: lucrum = summa requisitorum per valorem - valores x requisita
 * communia; candidatus aptus si lucrum > 0 et claves partitionum
 * differunt; maximum lucrum vincit; aequalitas: 'type', deinde ordo.
 * TESTIMONIUM (B1.2, mensuratum): solum valores bis saltem visi
 * numerantur, et duo tales requiruntur - partitio unius exempli omnes
 * claves 'requisitas' facit, unde campus quasi-identificator (valor
 * novus per exemplum) discrimen falsum fiebat. Variatio semel visa in
 * adumbratione tamen manet. */
/* lucrum candidati; d = valores bis saltem visi; VERUM si aptus */
interior b32
_candidatum_aestimare (
                   constans Figura* f,
    constans CandidatusDiscriminis* c,
                               s64* lucrum,
                               i32* d,
                               b32* differunt)
{
    i32 k;
    i32 m;

    *d          = 0;
    *lucrum     = 0;
    *differunt  = FALSUM;
    per (k = 0; k < xar_numerus(c->valores); k++)
    {
        constans ValorDiscriminis* prior = (ValorDiscriminis*)
            xar_obtinere(c->valores, k);

        si (prior->instantiae < II)
        {
            perge;
        }
        (*d)++;
        *lucrum += (s64)_requisita(prior->figura);
        per (m = k + I; m < xar_numerus(c->valores) && !*differunt; m++)
        {
            constans ValorDiscriminis* posterior = (ValorDiscriminis*)
                xar_obtinere(c->valores, m);

            *differunt = posterior->instantiae >= II
                && !_claves_aequales(prior->figura, posterior->figura);
        }
    }
    *lucrum -= (s64)*d * (s64)_requisita(f);
    redde *d >= II && *lucrum > 0 && *differunt;
}

interior constans CandidatusDiscriminis*
_discrimen_eligere (
    constans Figura* f)
{
    constans CandidatusDiscriminis* optimus         = NIHIL;
                               s64  lucrum_optimum  = 0;
                               i32  i;

    per (i = 0; i < xar_numerus(f->candidati); i++)
    {
        constans CandidatusDiscriminis* c = (CandidatusDiscriminis*)
            xar_obtinere(f->candidati, i);
                                    s64 lucrum;
                                    b32 differunt;
                                    i32 d;
                                    b32 melior;

        si (!c->vivus || xar_numerus(c->valores) < II)
        {
            perge;
        }
        si (!_candidatum_aestimare(f, c, &lucrum, &d, &differunt))
        {
            perge;
        }
        si (!optimus || lucrum > lucrum_optimum)
        {
            melior = VERUM;
        }
        alioquin si (lucrum < lucrum_optimum)
        {
            melior = FALSUM;
        }
        alioquin si (chorda_aequalis_literis(optimus->clavis, "type"))
        {
            melior = FALSUM;
        }
        alioquin si (chorda_aequalis_literis(c->clavis, "type"))
        {
            melior = VERUM;
        }
        alioquin
        {
            melior = _comparare_chordas(c->clavis, optimus->clavis) < 0;
        }
        si (melior)
        {
            optimus         = c;
            lucrum_optimum  = lucrum;
        }
    }
    redde optimus;
}

interior Norma*
_objectum_normam (
     constans Inferentia* inf,
    InferentiaTestimonia* t,
         constans Figura* f,
                 Piscina* p,
         constans chorda* omittere);

interior Norma*
_discrimen_normam (
                  constans Inferentia* inf,
                 InferentiaTestimonia* t,
       constans CandidatusDiscriminis* c,
                              Piscina* p)
{
                       Norma* d = norma_modus(norma_discrimen(p,
                                      chorda_ut_cstr(c->clavis, p)),
                                      inf->optiones.modus);
                          i32   n = xar_numerus(c->valores);
    constans ValorDiscriminis** ordo;
                          i32   i;
                          i32   k;

    ordo = (constans ValorDiscriminis**)piscina_allocare(p,
        (memoriae_index)n * magnitudo(ValorDiscriminis*));
    per (i = 0; i < n; i++)
    {
        ordo[i] = (ValorDiscriminis*)xar_obtinere(c->valores, i);
    }
    per (i = I; i < n; i++)
    {
        constans ValorDiscriminis* x = ordo[i];

        k = i;
        dum (   k > 0
             && _comparare_chordas(ordo[k - I]->valor, x->valor) > 0)
        {
            ordo[k] = ordo[k - I];
            k--;
        }
        ordo[k] = x;
    }
    per (i = 0; i < n; i++)
    {
        /* tag in variatione implicite declaratur: campus omittitur */
        Norma* variatio = _objectum_normam(inf, t, ordo[i]->figura,
            p,
            &c->clavis);
        character instantiae[XLVIII];

        sprintf(instantiae, "instantiae %u",
            (insignatus integer)ordo[i]->instantiae);
        si (t)
        {
            _testimonium_ponere(t, variatio,
                chorda_ex_literis(instantiae,
                t->piscina));
        }
        norma_variatio(d, chorda_ut_cstr(ordo[i]->valor, p), variatio);
    }
    redde d;
}

interior Norma*
_objectum_normam (
     constans Inferentia* inf,
    InferentiaTestimonia* t,
         constans Figura* f,
                 Piscina* p,
         constans chorda* omittere)
{
                Norma*  o;
                  i32   n = xar_numerus(f->campi);
        CampusFigurae** ordo;
                  i32   i;
                  i32   k;

    /* positio tabulae similis: claves multae, nulla in plus quam
     * dimidio instantiarum - objectum apertum cum nota (typus tabulae,
     * 'ceteri', fasciculus posterior) */
    si (n > XXXII)
    {
        b32 rarae = VERUM;

        per (i = 0; i < n && rarae; i++)
        {
            rarae = (s64)((CampusFigurae*)xar_obtinere(f->campi,
                i))->praesentia * II <= (s64)f->genera[JSON_OBJECTUM];
        }
        si (rarae)
        {
            character nuntius[CXXVIII];

            sprintf(nuntius, "inferentia: positio tabulae similis (%u"
                " claves distinctae)", (insignatus integer)n);
            redde norma_descriptio(norma_modus(norma_objectum(p),
                NORMA_APERTUM), nuntius);
        }
    }
    si (!omittere && !f->partitio)
    {
        constans CandidatusDiscriminis* c = _discrimen_eligere(f);

        si (c)
        {
            redde _discrimen_normam(inf, t, c, p);
        }
    }
    o = norma_modus(norma_objectum(p), inf->optiones.modus);
    si (n == 0)
    {
        redde o;
    }
    ordo = (CampusFigurae**)piscina_allocare(p,
        (memoriae_index)n * magnitudo(CampusFigurae*));
    per (i = 0; i < n; i++)
    {
        ordo[i] = (CampusFigurae*)xar_obtinere(f->campi, i);
    }
    /* insertio: claves paucae, ordo stabilis */
    per (i = I; i < n; i++)
    {
        CampusFigurae* x = ordo[i];

        k = i;
        dum (   k > 0 && _comparare_chordas(ordo[k - I]->titulus,
                x->titulus) > 0)
        {
            ordo[k] = ordo[k - I];
            k--;
        }
        ordo[k] = x;
    }
    per (i = 0; i < n; i++)
    {
        character* titulus = chorda_ut_cstr(ordo[i]->titulus, p);

        si (omittere && chorda_aequalis(ordo[i]->titulus, *omittere))
        {
            perge;
        }

        {
            Norma* filius = _figuram_normam(inf, t, ordo[i]->figura,
                p);
            character visum[LXIV];

            sprintf(visum, "visum %u/%u",
                (insignatus integer)ordo[i]->praesentia,
                (insignatus integer)f->genera[JSON_OBJECTUM]);
            _testimonium_praeponere(t, filius, visum);
            norma_campus(o, titulus, filius,
                ordo[i]->praesentia == f->genera[JSON_OBJECTUM]);
        }
    }
    redde o;
}

/* electio et fines ROGATI (§III): ex observatis, solum si exempla
 * >= exempla_minima; electio solum si distincti <= electio_maxima */
interior vacuum
_fines_et_electio (
    constans Inferentia* inf,
        constans Figura* f,
                  Norma* n,
                Piscina* p)
{
    NormaVisus v       = norma_visus(n);
           i32 minima  = inf->optiones.exempla_minima;

    si (inf->optiones.fines)
    {
        si (   v.genus                 == NORMA_INTEGER
            && f->genera[JSON_INTEGER] >= minima)
        {
            norma_intra(n, f->integrum_minimum, f->integrum_maximum);
        }
        alioquin si (   v.genus == NORMA_NUMERUS
                     && f->genera[JSON_INTEGER]
                         + f->genera[JSON_FLUITANS]
                        >= minima)
        {
            f64 infimum = f->habet_fluitans ? f->fluitans_minimum
                                         : (f64)f->integrum_minimum;
            f64 summum  = f->habet_fluitans ? f->fluitans_maximum
                                         : (f64)f->integrum_maximum;

            si (f->habet_integrum && (f64)f->integrum_minimum < infimum)
            {
                infimum = (f64)f->integrum_minimum;
            }
            si (f->habet_integrum && (f64)f->integrum_maximum > summum)
            {
                summum = (f64)f->integrum_maximum;
            }
            norma_intra_fluitans(n, infimum, summum);
        }
        alioquin si (   v.genus                == NORMA_TEXTUS
                     && f->genera[JSON_CHORDA] >= minima)
        {
            norma_longitudo(n, f->longitudo_minima,
                f->longitudo_maxima);
        }
        alioquin si (   v.genus                   == NORMA_TABULATUM
                     && f->genera[JSON_TABULATUM] >= minima)
        {
            norma_longitudo(n, f->tabulatum_minimum,
                f->tabulatum_maximum);
        }
    }
    si (   inf->optiones.electio && v.genus == NORMA_TEXTUS
        && f->genera[JSON_CHORDA]    >= minima && !f->distincti_superati
        && xar_numerus(f->distincti) > 0
        && xar_numerus(f->distincti) <= inf->optiones.electio_maxima)
    {
                       i32   d = xar_numerus(f->distincti);
        constans character** licita = (constans character**)
            piscina_allocare(p, (memoriae_index)(d + I)
                                * magnitudo(character*));
                    i32 i;
                    i32 k;

        per (i = 0; i < d; i++)
        {
            licita[i] =
                chorda_ut_cstr(*(chorda*)xar_obtinere(f->distincti,
                i), p);
        }
        per (i = I; i < d; i++)
        {
            constans character* x = licita[i];

            k = i;
            dum (k > 0 && strcmp(licita[k - I], x) > 0)
            {
                licita[k] = licita[k - I];
                k--;
            }
            licita[k] = x;
        }
        licita[d] = NIHIL;
        norma_electio(n, licita);
    }
}

interior Norma*
_figuram_normam_nuda (
     constans Inferentia* inf,
    InferentiaTestimonia* t,
         constans Figura* f,
                 Piscina* p)
{
      i32  non_nulla = 0;
      i32  g;
      i32  solum = VII;   /* nullum genus */
    Norma* n;

    per (g = JSON_BOOLEAN; g <= JSON_OBJECTUM; g++)
    {
        si (f->genera[g] > 0)
        {
            non_nulla++;
            solum = g;
        }
    }
    si (non_nulla == 0)
    {
        redde f->genera[JSON_NULLUM] > 0 ? norma_nullum(p)
                                         : norma_liberum(p);
    }
    si (   non_nulla == II && f->genera[JSON_INTEGER] > 0
        && f->genera[JSON_FLUITANS] > 0)
    {
        n = norma_numerus(p);
    }
    alioquin si (non_nulla > I)
    {
        redde norma_liberum(p);   /* genera discordia */
    }
    alioquin si (solum == JSON_BOOLEAN)
    {
        n = norma_boolean(p);
    }
    alioquin si (solum == JSON_INTEGER)
    {
        n = norma_integer(p);
    }
    alioquin si (solum == JSON_FLUITANS)
    {
        n = norma_numerus(p);
    }
    alioquin si (solum == JSON_CHORDA)
    {
        n = norma_textus(p);
        si (   inf->optiones.formae
            && f->genera[JSON_CHORDA] >= inf->optiones.exempla_minima)
        {
            per (g = 0; g < INFERENTIA_FORMAE; g++)
            {
                si (f->formae_vivae & ((i32)I << g))
                {
                    norma_forma(n, _formae[g]);
                    frange;
                }
            }
        }
    }
    alioquin si (solum == JSON_TABULATUM)
    {
        n = norma_tabulatum(p, f->elementum
            ? _figuram_normam(inf, t, f->elementum,
            p) : norma_liberum(p));
    }
    alioquin
    {
        n = _objectum_normam(inf, t, f, p, NIHIL);
    }
    _fines_et_electio(inf, f, n, p);
    si (f->genera[JSON_NULLUM] > 0)
    {
        norma_aut_nullum(n);
    }
    redde n;
}

/* testimonium figurae pro nodo n (§III): genera, distincti, forma,
 * longitudo, decisiones discriminis - numeri, numquam valores */
interior chorda
_testimonium_figurae (
     constans Inferentia* inf,
         constans Figura* f,
          constans Norma* n,
                 Piscina* p)
{
    ChordaAedificator* a = chorda_aedificator_creare(p, CXXVIII);
           NormaVisus  v = norma_visus(n);
            character  pars[CCLVI];
                  i32  genera = 0;
                  i32  g;

    per (g = 0; g < VII; g++)
    {
        si (f->genera[g] > 0)
        {
            genera++;
        }
    }
    si (genera > I)
    {
        ChordaAedificator* b = chorda_aedificator_creare(p, LXIV);

        chorda_aedificator_appendere_literis(b, "genera: ");
        genera = 0;
        per (g = 0; g < VII; g++)
        {
            si (f->genera[g] > 0)
            {
                sprintf(pars, "%s%s %u", genera > 0 ? ", " : "",
                    _genera_nomina[g],
                    (insignatus integer)f->genera[g]);
                chorda_aedificator_appendere_literis(b, pars);
                genera++;
            }
        }
        _pars_addere(a, chorda_ut_cstr(chorda_aedificator_finire(b),
            p));
    }
    si (f->genera[JSON_CHORDA] > 0)
    {
        si (f->distincti_superati)
        {
            sprintf(pars, "distincti > %u", (insignatus integer)
                (_limes_distinctorum(&inf->optiones) - I));
        }
        alioquin
        {
            sprintf(pars, "distincti %u",
                (insignatus integer)xar_numerus(
                f->distincti));
        }
        _pars_addere(a, pars);
    }
    si (v.forma.mensura > 0)
    {
        sprintf(pars, "forma %.*s: %u/%u", (integer)v.forma.mensura,
            (constans character*)v.forma.datum,
            (insignatus integer)f->genera[JSON_CHORDA],
            (insignatus integer)f->genera[JSON_CHORDA]);
        _pars_addere(a, pars);
    }
    si (f->habet_tabulatum)
    {
        sprintf(pars, "longitudo %u..%u",
            (insignatus integer)f->tabulatum_minimum,
            (insignatus integer)f->tabulatum_maximum);
        _pars_addere(a, pars);
    }
    si (!f->partitio && f->genera[JSON_OBJECTUM] > 0)
    {
        constans CandidatusDiscriminis* electus = _discrimen_eligere(f);
                                    i32 i;

        per (i = 0; i < xar_numerus(f->candidati); i++)
        {
            constans CandidatusDiscriminis* c = (CandidatusDiscriminis*)
                xar_obtinere(f->candidati, i);
                                    s64 lucrum;
                                    i32 d;
                                    b32 differunt;
                                    b32 aptus;

            si (!c->vivus || xar_numerus(c->valores) < II)
            {
                perge;
            }
            aptus = _candidatum_aestimare(f, c, &lucrum, &d,
                &differunt);
            si (c == electus && v.genus == NORMA_DISCRIMEN)
            {
                sprintf(pars, "discrimen '%.*s': lucrum %ld",
                    (integer)c->clavis.mensura,
                    (constans character*)c->clavis.datum,
                    (longus)lucrum);
            }
            alioquin si (aptus)
            {
                sprintf(pars, "alter '%.*s' lucrum %ld",
                    (integer)c->clavis.mensura,
                    (constans character*)c->clavis.datum,
                    (longus)lucrum);
            }
            alioquin si (d < II)
            {
                sprintf(pars,
                    "discrimen '%.*s' reiectum: valores bis visi"
                    " %u (2 postulati)", (integer)c->clavis.mensura,
                    (constans character*)c->clavis.datum,
                    (insignatus integer)d);
            }
            alioquin si (lucrum <= 0)
            {
                sprintf(pars, "discrimen '%.*s' reiectum: lucrum %ld",
                    (integer)c->clavis.mensura,
                    (constans character*)c->clavis.datum,
                    (longus)lucrum);
            }
            alioquin
            {
                sprintf(pars, "discrimen '%.*s' reiectum: claves"
                    " partitionum aequales", (integer)c->clavis.mensura,
                    (constans character*)c->clavis.datum);
            }
            _pars_addere(a, pars);
        }
    }
    redde chorda_aedificator_finire(a);
}

interior Norma*
_figuram_normam (
     constans Inferentia* inf,
    InferentiaTestimonia* t,
         constans Figura* f,
                 Piscina* p)
{
    Norma* n = _figuram_normam_nuda(inf, t, f, p);

    si (t)
    {
        _testimonium_ponere(t, n, _testimonium_figurae(inf, f, n,
            t->piscina));
    }
    redde n;
}

Norma*
inferentia_normam_testatam (
     constans Inferentia*  inferentia,
                 Piscina*  piscina,
    InferentiaTestimonia** testimonia)
{
    InferentiaTestimonia* t;
                   Norma* n;
               character  exempla[XLVIII];

    si (testimonia)
    {
        *testimonia = NIHIL;
    }
    si (!inferentia || inferentia->numerus == 0 || !piscina)
    {
        redde NIHIL;
    }
    t = (InferentiaTestimonia*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(InferentiaTestimonia));
    t->piscina = piscina;
    t->paria = xar_creare(piscina, (i32)magnitudo(TestimoniumNodi));
    n = _figuram_normam(inferentia, t, inferentia->radix, piscina);
    sprintf(exempla, "%u exempla",
        (insignatus integer)inferentia->numerus);
    _testimonium_praeponere(t, n, exempla);
    si (testimonia)
    {
        *testimonia = t;
    }
    redde n;
}

chorda
inferentia_commentarius (
    constans Norma* nodus,
           Piscina* piscina,
            vacuum* testimonia)
{
    InferentiaTestimonia* t = (InferentiaTestimonia*)testimonia;
         TestimoniumNodi* tn;

    si (!t || !nodus)
    {
        redde chorda_ex_literis("", piscina);
    }
    tn = _testimonium_invenire(t, nodus);
    redde tn ? tn->textus : chorda_ex_literis("", piscina);
}

Norma*
inferentia_normam (
    constans Inferentia* inferentia,
                Piscina* piscina)
{
    si (!inferentia || inferentia->numerus == 0 || !piscina)
    {
        redde NIHIL;
    }
    redde _figuram_normam(inferentia, NIHIL, inferentia->radix,
        piscina);
}


/* ====================================================================
 * E. DISCREPANTIA (norma-spec-4 §IV): schema declaratum ambulat iuxta
 * figuras exemplorum - numeri, numquam valores
 * ==================================================================== */

nomen structura {
    Piscina* p;
        Xar* inventa;   /* NormaDiscrepantia */
} Comparator;

/* via clavis: forma iudicis (lib/norma.c _via_clavis) - '.k' si
 * simplex, aliter '["k"]' cum effugio JSON */
interior chorda
_via_clavis_comparatoris (
     chorda  via,
     chorda  clavis,
    Piscina* p)
{
    ChordaAedificator* a = chorda_aedificator_creare(p,
        (memoriae_index)(via.mensura + clavis.mensura + VIII));
                   b32 simplex = clavis.mensura > 0;
                   i32 i;

    per (i = 0; i < clavis.mensura && simplex; i++)
    {
        character k = (character)clavis.datum[i];

        simplex = (k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z')
            || k == '_' || (i > 0 && k >= '0' && k <= '9');
    }
    chorda_aedificator_appendere_chorda(a, via);
    si (simplex)
    {
        chorda_aedificator_appendere_character(a, '.');
        chorda_aedificator_appendere_chorda(a, clavis);
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a, "[\"");
        chorda_aedificator_appendere_evasus_json(a, clavis);
        chorda_aedificator_appendere_literis(a, "\"]");
    }
    redde chorda_aedificator_finire(a);
}

interior chorda
_via_addere (
                  chorda  via,
      constans character* cauda,
                 Piscina* p)
{
    ChordaAedificator* a = chorda_aedificator_creare(p,
        (memoriae_index)via.mensura + VIII);

    chorda_aedificator_appendere_chorda(a, via);
    chorda_aedificator_appendere_literis(a, cauda);
    redde chorda_aedificator_finire(a);
}

interior vacuum
_discrepantiam_addere (
                Comparator* c,
                    chorda  via,
    NormaDiscrepantiaGenus  genus,
        constans character* nuntius)
{
    NormaDiscrepantia* d = (NormaDiscrepantia*)xar_addere(c->inventa);

    d->via      = via;
    d->genus    = genus;
    d->nuntius  = chorda_ex_literis(nuntius, c->p);
}

interior b32
_genus_licitum (
    NormaGenus declaratum,
           i32 visum)
{
    commutatio (declaratum)
    {
        casus NORMA_LIBERUM:    redde VERUM;
        casus NORMA_NULLUM:     redde visum == JSON_NULLUM;
        casus NORMA_BOOLEAN:    redde visum == JSON_BOOLEAN;
        casus NORMA_INTEGER:    redde visum == JSON_INTEGER;
        casus NORMA_NUMERUS:    redde visum == JSON_INTEGER
                                      || visum == JSON_FLUITANS;
        casus NORMA_TEXTUS:     redde visum == JSON_CHORDA;
        casus NORMA_TABULATUM:  redde visum == JSON_TABULATUM;
        casus NORMA_OBJECTUM:
        casus NORMA_DISCRIMEN:  redde visum == JSON_OBJECTUM;
    }
    redde FALSUM;
}

interior vacuum
_comparare (
         Comparator* c,
     constans Norma* d,
    constans Figura* f,
             chorda  via,
    constans chorda* omittere);

interior vacuum
_objecta_comparare (
          Comparator* c,
      constans Norma* d,
     constans Figura* f,
              chorda  via,
     constans chorda* omittere)
{
    NormaVisus v        = norma_visus(d);
           i32 objecta  = f->genera[JSON_OBJECTUM];
           i32 i;
           i32 k;
     character nuntius[CXXVIII];

    per (i = 0; v.campi && i < xar_numerus(v.campi); i++)
    {
          NormaCampus* dc = (NormaCampus*)xar_obtinere(v.campi, i);
        CampusFigurae* fc = NIHIL;
                  i32  visum;
               chorda  via_campi = _via_clavis_comparatoris(via,
                   dc->titulus,
                   c->p);

        per (k = 0; k < xar_numerus(f->campi) && !fc; k++)
        {
            CampusFigurae* x = (CampusFigurae*)xar_obtinere(f->campi,
                k);

            si (chorda_aequalis(x->titulus, dc->titulus))
            {
                fc = x;
            }
        }
        visum = fc ? fc->praesentia : 0;
        sprintf(nuntius, "visum %u/%u", (insignatus integer)visum,
            (insignatus integer)objecta);
        si (visum == 0)
        {
            _discrepantiam_addere(c, via_campi,
                NORMA_DISCREPANTIA_NUMQUAM_VISUM, nuntius);
            perge;
        }
        si (dc->requiritur && visum < objecta)
        {
            _discrepantiam_addere(c, via_campi,
                NORMA_DISCREPANTIA_REQUISITUM_ABSENS, nuntius);
        }
        si (!dc->requiritur && visum == objecta)
        {
            _discrepantiam_addere(c, via_campi,
                NORMA_DISCREPANTIA_FORTASSE_REQUISITUM, nuntius);
        }
        _comparare(c, dc->valor, fc->figura, via_campi, NIHIL);
    }
    per (k = 0; k < xar_numerus(f->campi); k++)
    {
        CampusFigurae* x = (CampusFigurae*)xar_obtinere(f->campi, k);
                  b32  declaratum = FALSUM;

        si (omittere && chorda_aequalis(x->titulus, *omittere))
        {
            perge;   /* tag variationis implicite declaratur */
        }
        per (i = 0; v.campi && i < xar_numerus(v.campi)
            && !declaratum; i++)
        {
            declaratum = chorda_aequalis(x->titulus,
                ((NormaCampus*)xar_obtinere(v.campi, i))->titulus);
        }
        si (!declaratum && x->praesentia > 0)
        {
            sprintf(nuntius, "visum %u/%u",
                (insignatus integer)x->praesentia,
                (insignatus integer)objecta);
            _discrepantiam_addere(c, _via_clavis_comparatoris(via,
                x->titulus, c->p), NORMA_DISCREPANTIA_NON_DECLARATUM,
                nuntius);
        }
    }
}

interior vacuum
_discrimen_comparare (
          Comparator* c,
      constans Norma* d,
     constans Figura* f,
              chorda  via)
{
                        NormaVisus  v     = norma_visus(d);
    constans CandidatusDiscriminis* cand  = NIHIL;
                               i32  i;
                               i32  k;
                         character  nuntius[CXXVIII];

    per (i = 0; i < xar_numerus(f->candidati) && !cand; i++)
    {
        constans CandidatusDiscriminis* x = (CandidatusDiscriminis*)
            xar_obtinere(f->candidati, i);

        si (   x->vivus
            && chorda_aequalis(x->clavis, v.clavis_discriminis))
        {
            cand = x;
        }
    }
    si (!cand)
    {
        sprintf(nuntius,
            "clavis '%.*s' non candidatus (deest, non textus,"
            " aut valores plures quam %u)",
            (integer)(v.clavis_discriminis.mensura > LX ? LX
                      : v.clavis_discriminis.mensura),
            (constans character*)v.clavis_discriminis.datum, XVI);
        _discrepantiam_addere(c, via,
            NORMA_DISCREPANTIA_NON_COMPARABILE,
            nuntius);
        redde;
    }
    per (i = 0; v.variationes && i < xar_numerus(v.variationes); i++)
    {
        NormaVariatio* va = (NormaVariatio*)xar_obtinere(v.variationes,
            i);
        constans ValorDiscriminis* vd = NIHIL;

        per (k = 0; k < xar_numerus(cand->valores) && !vd; k++)
        {
            constans ValorDiscriminis* x =
                (ValorDiscriminis*)xar_obtinere(
                cand->valores, k);

            si (chorda_aequalis(x->valor, va->valor))
            {
                vd = x;
            }
        }
        si (!vd)
        {
            sprintf(nuntius, "variatio '%.*s': visa 0",
                (integer)(va->valor.mensura
                    > LX ? LX : va->valor.mensura),
                (constans character*)va->valor.datum);
            _discrepantiam_addere(c, via,
                NORMA_DISCREPANTIA_VARIATIO_NUMQUAM_VISA, nuntius);
            perge;
        }
        /* variatio contra partitionem SUAM, tag omisso */
        _objecta_comparare(c, va->objectum, vd->figura, via,
            &v.clavis_discriminis);
    }
    per (k = 0; k < xar_numerus(cand->valores); k++)
    {
        constans ValorDiscriminis* x = (ValorDiscriminis*)xar_obtinere(
            cand->valores, k);
                               b32 declarata = FALSUM;

        per (i = 0; v.variationes && i < xar_numerus(v.variationes)
            && !declarata; i++)
        {
            declarata = chorda_aequalis(x->valor, ((NormaVariatio*)
                xar_obtinere(v.variationes, i))->valor);
        }
        si (!declarata)
        {
            sprintf(nuntius, "variatio '%.*s': visa %u",
                (integer)(x->valor.mensura
                    > LX ? LX : x->valor.mensura),
                (constans character*)x->valor.datum,
                (insignatus integer)x->instantiae);
            _discrepantiam_addere(c, via,
                NORMA_DISCREPANTIA_VARIATIO_NON_DECLARATA, nuntius);
        }
    }
}

interior vacuum
_comparare (
          Comparator* c,
      constans Norma* d,
     constans Figura* f,
              chorda  via,
     constans chorda* omittere)
{
    NormaVisus v = norma_visus(d);
           i32 g;
     character nuntius[CXXVIII];

    si (!d || !f || f->instantiae == 0)
    {
        redde;
    }
    per (g = JSON_BOOLEAN; g <= JSON_OBJECTUM; g++)
    {
        si (f->genera[g] > 0 && !_genus_licitum(v.genus, g))
        {
            sprintf(nuntius, "genus visum %s %u/%u", _genera_nomina[g],
                (insignatus integer)f->genera[g],
                (insignatus integer)f->instantiae);
            _discrepantiam_addere(c, via,
                NORMA_DISCREPANTIA_GENUS_LATIUS,
                nuntius);
        }
    }
    si (   f->genera[JSON_NULLUM] > 0 && !v.aut_nullum
        && v.genus != NORMA_NULLUM && v.genus != NORMA_LIBERUM)
    {
        sprintf(nuntius, "null visum %u/%u",
            (insignatus integer)f->genera[JSON_NULLUM],
            (insignatus integer)f->instantiae);
        _discrepantiam_addere(c, via, NORMA_DISCREPANTIA_NULLUM_NOVUM,
            nuntius);
    }
    si (v.genus == NORMA_OBJECTUM && f->genera[JSON_OBJECTUM] > 0)
    {
        _objecta_comparare(c, d, f, via, omittere);
    }
    alioquin si (   v.genus == NORMA_DISCRIMEN
                 && f->genera[JSON_OBJECTUM] > 0)
    {
        _discrimen_comparare(c, d, f, via);
    }
    alioquin si (v.genus == NORMA_TABULATUM && f->elementum)
    {
        _comparare(c, v.elementum, f->elementum, _via_addere(via, "[]",
            c->p), NIHIL);
    }
}

Xar*
norma_comparare (
         constans Norma* declarata,
    constans Inferentia* inferentia,
                Piscina* piscina)
{
    Comparator c;

    si (!declarata || !inferentia || !piscina)
    {
        redde NIHIL;
    }
    c.p        = piscina;
    c.inventa  = xar_creare(piscina, (i32)magnitudo(NormaDiscrepantia));
    _comparare(&c, declarata, inferentia->radix, chorda_ex_literis("$",
        piscina), NIHIL);
    redde c.inventa;
}

constans character*
norma_discrepantia_descriptio (
    NormaDiscrepantiaGenus genus)
{
    commutatio (genus)
    {
        casus NORMA_DISCREPANTIA_FORTASSE_REQUISITUM:
            redde "FORTASSE_REQUISITUM";
        casus NORMA_DISCREPANTIA_REQUISITUM_ABSENS:
            redde "REQUISITUM_ABSENS";
        casus NORMA_DISCREPANTIA_NUMQUAM_VISUM:
            redde "NUMQUAM_VISUM";
        casus NORMA_DISCREPANTIA_NON_DECLARATUM:
            redde "NON_DECLARATUM";
        casus NORMA_DISCREPANTIA_GENUS_LATIUS:
            redde "GENUS_LATIUS";
        casus NORMA_DISCREPANTIA_NULLUM_NOVUM:
            redde "NULLUM_NOVUM";
        casus NORMA_DISCREPANTIA_VARIATIO_NUMQUAM_VISA:
            redde "VARIATIO_NUMQUAM_VISA";
        casus NORMA_DISCREPANTIA_VARIATIO_NON_DECLARATA:
            redde "VARIATIO_NON_DECLARATA";
        casus NORMA_DISCREPANTIA_NON_COMPARABILE:
            redde "NON_COMPARABILE";
    }
    redde "IGNOTUM";
}
