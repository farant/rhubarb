/* norma_inferre.c - schema ex exemplis inferre (norma-spec-3; B1.1:
 * figurae, additio, coniunctio, regulae). Adumbratio, non veritas:
 * requisitum = in omni instantia parentis; claves ordine octetorum
 * (ordo exemplorum adumbrationem non mutat - lex coniunctionis). */
#include "norma_inferre.h"
#include "utf8.h"

#include <string.h>

/* formae probatae, ordine praeferentiae; bit i = forma i adhuc viva */
#define INFERENTIA_FORMAE IV
hic_manens constans character* constans _formae[INFERENTIA_FORMAE] = {
    "date-time", "uuid", "email", "uri"
};

nomen structura Figura Figura;

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
    f->distincti     = xar_creare(p, (i32)magnitudo(chorda));
    f->campi         = xar_creare(p, (i32)magnitudo(CampusFigurae));
    f->formae_vivae  = ((i32)I << INFERENTIA_FORMAE) - I;
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
            frange;
        }
        ordinarius:
            frange;
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

interior Norma*
_figuram_normam (
    constans Inferentia* inf,
        constans Figura* f,
                Piscina* p);

interior Norma*
_objectum_normam (
    constans Inferentia* inf,
        constans Figura* f,
                Piscina* p)
{
                Norma* o = norma_modus(norma_objectum(p),
                               inf->optiones.modus);
                  i32   n = xar_numerus(f->campi);
        CampusFigurae** ordo;
                  i32   i;
                  i32   k;

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

        norma_campus(o, titulus, _figuram_normam(inf, ordo[i]->figura,
            p),
            ordo[i]->praesentia == f->genera[JSON_OBJECTUM]);
    }
    redde o;
}

interior Norma*
_figuram_normam (
    constans Inferentia* inf,
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
            ? _figuram_normam(inf, f->elementum, p) : norma_liberum(p));
    }
    alioquin
    {
        n = _objectum_normam(inf, f, p);
    }
    si (f->genera[JSON_NULLUM] > 0)
    {
        norma_aut_nullum(n);
    }
    redde n;
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
    redde _figuram_normam(inferentia, inferentia->radix, piscina);
}
