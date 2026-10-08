/* herbarium.c - specimina pressa responsorum API: captura et redditio
 * (herbarium-spec; vates-plan-2 T2). Decisiones intra caput: vide
 * lib/herbarium.worklog.md. */
#include "postulata_posix.h"
#include "herbarium.h"
#include "json.h"
#include "filum.h"
#include "fasti.h"
#include "sigillum.h"
#include "iter_directoria.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HERBARIUM_VIA_MAXIMA  MXXIV

structura Herbarium {
               Piscina* piscina;
     HerbariumOptiones  optiones;
             character  specimina_dir[HERBARIUM_VIA_MAXIMA];
             character  index_via[HERBARIUM_VIA_MAXIMA];
                   b32  defectus_nuntiatus;
};

nomen structura {
      Herbarium* herbarium;
    HttpVectura  involuta;
} HerbariumInvolucrum;

nomen structura {
    Xar* specimina;
    i32  index;
} HerbariumRedditor;

hic_manens constans character* _capita_ordinaria[] = {
    "content-type", "retry-after", "request-id", "x-request-id", NIHIL
};


/* ======================================================================
 * AUXILIA
 * ====================================================================== */

interior vacuum
_defectum_nuntiare (
              Herbarium* h,
     constans character* nuntius)
{
    si (!h->defectus_nuntiatus)
    {
        fprintf(stderr, "herbarium: %s (captura pergit sine nuntiis "
                        "ulterioribus; petitiones intactae)\n",
                        nuntius);
        h->defectus_nuntiatus = VERUM;
    }
}

interior b32
_est_cifra (
    i8 c)
{
    redde c >= '0' && c <= '9';
}

interior b32
_est_vocis (
    i8 c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || _est_cifra(c) || c == '_' || c == '-';
}

/* Larvare: series [A-Za-z0-9_-] >= XII cum cifra -> '@'; ceterae
 * cifrarum series -> '#'. Variantes per textum larvatum different. */
interior chorda
_larvare (
      chorda  s,
     Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)s.mensura + I);
    i32 i = 0;

    dum (i < s.mensura)
    {
        si (_est_vocis(s.datum[i]))
        {
            i32 initium  = i;
            b32 cifra    = FALSUM;

            dum (i < s.mensura && _est_vocis(s.datum[i]))
            {
                si (_est_cifra(s.datum[i]))
                {
                    cifra = VERUM;
                }
                i++;
            }
            si (cifra && i - initium >= XII)
            {
                chorda_aedificator_appendere_character(aed, '@');
            }
            alioquin
            {
                i32 k;
                b32 in_numeris = FALSUM;

                per (k = initium; k < i; k++)
                {
                    si (_est_cifra(s.datum[k]))
                    {
                        si (!in_numeris)
                        {
                            chorda_aedificator_appendere_character(aed,
                                '#');
                        }
                        in_numeris = VERUM;
                    }
                    alioquin
                    {
                        chorda_aedificator_appendere_character(aed,
                            (character)s.datum[k]);
                        in_numeris = FALSUM;
                    }
                }
            }
        }
        alioquin
        {
            chorda_aedificator_appendere_character(aed,
                (character)s.datum[i]);
            i++;
        }
    }
    redde chorda_aedificator_finire(aed);
}

interior vacuum
_sceletum_scribere (
    ChordaAedificator* aed,
            JsonValor* v)
{
    commutatio (json_genus(v))
    {
        casus JSON_OBJECTUM:
        {
            JsonObjectumIterator  it = json_objectum_iterator(v);
                          chorda  clavis;
                       JsonValor* filius;
                             b32  primus = VERUM;

            chorda_aedificator_appendere_character(aed, '{');
            dum (json_objectum_iterator_proxima(&it, &clavis, &filius))
            {
                si (!primus)
                {
                    chorda_aedificator_appendere_character(aed, ',');
                }
                primus = FALSUM;
                chorda_aedificator_appendere_chorda(aed, clavis);
                chorda_aedificator_appendere_character(aed, ':');
                _sceletum_scribere(aed, filius);
            }
            chorda_aedificator_appendere_character(aed, '}');
            frange;
        }
        casus JSON_TABULATUM:
            chorda_aedificator_appendere_character(aed, '[');
            si (json_tabulatum_numerus(v) > 0)
            {
                _sceletum_scribere(aed, json_tabulatum_obtinere(v, 0));
            }
            chorda_aedificator_appendere_character(aed, ']');
            frange;
        casus JSON_CHORDA:
            chorda_aedificator_appendere_character(aed, 's');
            frange;
        casus JSON_INTEGER:
        casus JSON_FLUITANS:
            chorda_aedificator_appendere_character(aed, 'n');
            frange;
        casus JSON_BOOLEAN:
            chorda_aedificator_appendere_character(aed, 'b');
            frange;
        ordinarius:
            chorda_aedificator_appendere_character(aed, 'z');
            frange;
    }
}

interior b32
_caput_admissum (
    constans Herbarium* h,
                chorda  titulus,
               Piscina* piscina)
{
    chorda minuscula = chorda_minuscula(titulus, piscina);
       i32 i;

    per (i = 0; _capita_ordinaria[i]; i++)
    {
        si (chorda_aequalis_literis(minuscula, _capita_ordinaria[i]))
        {
            redde VERUM;
        }
    }
    si (h->optiones.capita_admissa)
    {
        per (i = 0; h->optiones.capita_admissa[i]; i++)
        {
            si (chorda_aequalis_literis(minuscula,
                    h->optiones.capita_admissa[i]))
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

interior JsonValor*
_specimen_json (
             Herbarium* h,
           HttpPetitio* petitio,
         HttpResponsum* resp,
                chorda  clavis,
                chorda  causa,
               Piscina* piscina)
{
    JsonValor* obj     = json_objectum_creare(piscina);
    JsonValor* capita  = json_tabulatum_creare(piscina);
    JsonValor* summarium_petitionis =
        json_objectum_creare(piscina);
    HttpPetitioVisus visus = http_petitio_visus(petitio);
                 i32 i;

    json_objectum_ponere(obj, "clavis", json_chorda_creare(piscina,
        clavis));
    json_objectum_ponere(obj, "causa", json_chorda_creare(piscina,
        causa));
    json_objectum_ponere(obj, "status",
        json_integer_creare(piscina, (s64)resp->status));
    per (i = 0; i < resp->capita_numerus; i++)
    {
        si (_caput_admissum(h, resp->capita[i].titulus, piscina))
        {
            JsonValor* c = json_objectum_creare(piscina);

            json_objectum_ponere(c, "titulus",
                json_chorda_creare(piscina, resp->capita[i].titulus));
            json_objectum_ponere(c, "valor",
                json_chorda_creare(piscina, resp->capita[i].valor));
            json_tabulatum_addere(capita, c);
        }
    }
    json_objectum_ponere(obj, "capita", capita);
    json_objectum_ponere(obj, "corpus", json_chorda_creare(piscina,
        resp->corpus));
    json_objectum_ponere(obj, "primum_visum",
        json_chorda_creare(piscina, fasti_ad_iso(fasti_nunc(),
        piscina)));
    json_objectum_ponere(obj, "methodus",
        json_chorda_creare_literis(piscina,
        petitio ? http_methodus_nomen(visus.methodus) : ""));
    json_objectum_ponere(obj, "hospes", json_chorda_creare(piscina,
        visus.hospes));
    json_objectum_ponere(obj, "via", json_chorda_creare(piscina,
        visus.via));
    si (h->optiones.campi_petitionis && visus.corpus.mensura > 0)
    {
        JsonResultus j = json_legere(visus.corpus, piscina);

        si (j.successus && json_est_objectum(j.radix))
        {
            per (i = 0; h->optiones.campi_petitionis[i]; i++)
            {
                JsonValor* v = json_objectum_capere(j.radix,
                    h->optiones.campi_petitionis[i]);

                si (v)
                {
                    json_objectum_ponere(summarium_petitionis,
                        h->optiones.campi_petitionis[i], v);
                }
            }
        }
    }
    json_objectum_ponere(obj, "summarium", summarium_petitionis);
    redde obj;
}

interior vacuum
_premere (
        Herbarium* h,
      HttpPetitio* petitio,
    HttpResponsum* resp,
           chorda  causa)
{
    Piscina* p = piscina_generare_dynamicum("herbarium_premere",
        LXIV * M);
       chorda clavis;
       chorda larvatum;
     Sigillum sig;
    character hex[SIGILLUM_HEX_MENSURA];
    character via[HERBARIUM_VIA_MAXIMA + C];
          s32 novus = -I;
          i32 n;

    si (!p)
    {
        _defectum_nuntiare(h, "piscina creari non potuit");
        redde;
    }
    clavis = h->optiones.clavis
        ? h->optiones.clavis(petitio, resp, p, h->optiones.clavis_datum)
        : herbarium_clavis_sceleti(petitio, resp, p, NIHIL);
    sig = sigillum_computare(clavis.datum,
        (memoriae_index)clavis.mensura);
    sigillum_hex(&sig, hex);
    larvatum = _larvare(resp->corpus, p);

    per (n = I; n <= h->optiones.variantes_maximae; n++)
    {
        sprintf(via, "%s/%s-%u.json", h->specimina_dir, hex, n);
        si (!filum_existit(via))
        {
            novus = (s32)n;
            frange;
        }
        {
            JsonResultus j = json_legere(filum_legere_totum(via, p), p);
                  chorda prius = j.successus
                      ? json_capere_chorda(j.radix, "corpus",
                      chorda_ex_literis("", p))
                      : chorda_ex_literis("", p);

            si (chorda_aequalis(_larvare(prius, p), larvatum))
            {
                novus = 0;
                frange;
            }
        }
    }
    si (novus > 0)
    {
        JsonValor* spec = _specimen_json(h, petitio, resp, clavis,
            causa, p);

        si (!filum_scribere(via, json_scribere_pulchrum(spec, p)))
        {
            _defectum_nuntiare(h, "specimen scribi non potuit");
        }
    }
    {
        JsonValor* linea = json_objectum_creare(p);
           chorda  textus;
           chorda  cum_fine;

        json_objectum_ponere(linea, "tempus",
            json_chorda_creare(p, fasti_ad_iso(fasti_nunc(), p)));
        json_objectum_ponere(linea, "sigillum",
            json_chorda_creare_literis(p, hex));
        json_objectum_ponere(linea, "status", json_integer_creare(p,
            (s64)resp->status));
        json_objectum_ponere(linea, "causa", json_chorda_creare(p,
            causa));
        json_objectum_ponere(linea, "variantes_index",
            json_integer_creare(p, novus > 0 ? (s64)novus : 0));
        textus            = json_scribere(linea, p);
        cum_fine.mensura  = textus.mensura + I;
        cum_fine.datum = (i8*)piscina_allocare(p,
            (i64)cum_fine.mensura);
        memcpy(cum_fine.datum, textus.datum, (size_t)textus.mensura);
        cum_fine.datum[textus.mensura] = '\n';
        si (!filum_appendere_firmiter(h->index_via, cum_fine))
        {
            _defectum_nuntiare(h, "index.jsonl scribi non potuit");
        }
    }
    piscina_destruere(p);
}


/* ======================================================================
 * CAPTURA
 * ====================================================================== */

HerbariumOptiones
herbarium_optiones_ordinariae (vacuum)
{
    HerbariumOptiones o;

    memset(&o, 0, magnitudo(o));
    redde o;
}

Herbarium*
herbarium_aperire (
                       Piscina* piscina,
    constans HerbariumOptiones* optiones)
{
    Herbarium* h;

    si (   !piscina || !optiones || !optiones->directorium
        || strlen(optiones->directorium) + XXXII
            >= HERBARIUM_VIA_MAXIMA)
    {
        redde NIHIL;
    }
    h = (Herbarium*)piscina_allocare(piscina,
        (i64)magnitudo(Herbarium));
    si (!h)
    {
        redde NIHIL;
    }
    memset(h, 0, magnitudo(*h));
    h->piscina   = piscina;
    h->optiones  = *optiones;
    si (h->optiones.status_minimus == 0)
    {
        h->optiones.status_minimus = CD;
    }
    si (h->optiones.variantes_maximae == 0)
    {
        h->optiones.variantes_maximae = III;
    }
    sprintf(h->specimina_dir, "%s/specimina", optiones->directorium);
    sprintf(h->index_via, "%s/index.jsonl", optiones->directorium);
    si (!filum_directorium_creare_cum_parentibus(h->specimina_dir))
    {
        fprintf(stderr, "herbarium: directorium creari non potest: %s"
                        " (sine captura pergitur)\n", h->specimina_dir);
        redde NIHIL;
    }
    redde h;
}

interior HttpResultus
_exsequi_capiens (
    HttpPetitio* petitio,
        Piscina* piscina,
         vacuum* datum)
{
    HerbariumInvolucrum* inv = (HerbariumInvolucrum*)datum;
           HttpResultus  res;

    res = http_vectura_exsequi(inv->involuta, petitio, piscina);
    si (   res.successus && res.responsum
        && res.responsum->status
            >= inv->herbarium->optiones.status_minimus)
    {
        _premere(inv->herbarium, petitio, res.responsum,
                 chorda_ex_literis("status", piscina));
    }
    redde res;
}

HttpVectura
herbarium_vectura (
      Herbarium* herbarium,
    HttpVectura  involuta)
{
    HerbariumInvolucrum* inv;
            HttpVectura  v;

    si (!herbarium)
    {
        redde involuta;
    }
    inv = (HerbariumInvolucrum*)piscina_allocare(herbarium->piscina,
        (i64)magnitudo(HerbariumInvolucrum));
    inv->herbarium  = herbarium;
    inv->involuta   = involuta;
    v.exsequi       = _exsequi_capiens;
    v.datum         = inv;
    redde v;
}

vacuum
herbarium_premere (
        Herbarium* herbarium,
      HttpPetitio* petitio,
    HttpResponsum* responsum,
           chorda  causa)
{
    si (!herbarium || !responsum)
    {
        redde;
    }
    _premere(herbarium, petitio, responsum, causa);
}

chorda
herbarium_clavis_sceleti (
      HttpPetitio* petitio,
    HttpResponsum* responsum,
          Piscina* piscina,
           vacuum* datum)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina, CCLVI);
            character  numerus[XXXII];
         JsonResultus  j;

    (vacuum)petitio;
    (vacuum)datum;
    sprintf(numerus, "%u:", responsum ? responsum->status : 0);
    chorda_aedificator_appendere_literis(aed, numerus);
    si (!responsum)
    {
        redde chorda_aedificator_finire(aed);
    }
    j = json_legere(responsum->corpus, piscina);
    si (j.successus)
    {
        _sceletum_scribere(aed, j.radix);
    }
    alioquin
    {
        i32 m = responsum->corpus.mensura;

        chorda_aedificator_appendere_literis(aed, "crudum:");
        chorda_aedificator_appendere_chorda(aed,
            http_responsum_caput(responsum, "content-type"));
        chorda_aedificator_appendere_literis(aed,
            m == 0 ? ":0" : m < M ? ":<1K" : m
                < LXIV * M ? ":<64K" : ":>=64K");
    }
    redde chorda_aedificator_finire(aed);
}


/* ======================================================================
 * REDDITIO
 * ====================================================================== */

interior b32
_specimen_legere (
               chorda  nomen_fili,
               chorda  textus,
    HerbariumSpecimen* s,
              Piscina* piscina)
{
    JsonResultus  j = json_legere(textus, piscina);
       JsonValor* capita;
          chorda  vacua = chorda_ex_literis("", piscina);
             s32  linea = -I;
             i32  i;

    si (!j.successus || !json_est_objectum(j.radix))
    {
        redde FALSUM;
    }
    memset(s, 0, magnitudo(*s));
    s->clavis  = json_capere_chorda(j.radix, "clavis", vacua);
    s->causa   = json_capere_chorda(j.radix, "causa", vacua);
    s->status  = (i32)json_capere_integer(j.radix, "status", 0);
    s->corpus  = json_capere_chorda(j.radix, "corpus", vacua);
    s->primum_visum = json_capere_chorda(j.radix, "primum_visum",
        vacua);
    s->methodus  = json_capere_chorda(j.radix, "methodus", vacua);
    s->hospes    = json_capere_chorda(j.radix, "hospes", vacua);
    s->via       = json_capere_chorda(j.radix, "via", vacua);
    s->summarium    = json_objectum_capere(j.radix, "summarium")
        ? json_scribere(json_objectum_capere(j.radix, "summarium"),
        piscina)
        : vacua;
    capita             = json_objectum_capere(j.radix, "capita");
    s->capita_numerus  = capita ? json_tabulatum_numerus(capita) : 0;
    s->capita = (HttpCaput*)piscina_allocare(piscina,
        (i64)((s->capita_numerus + I) * magnitudo(HttpCaput)));
    per (i = 0; i < s->capita_numerus; i++)
    {
        JsonValor* c = json_tabulatum_obtinere(capita, i);

        s->capita[i].titulus  = json_capere_chorda(c, "titulus", vacua);
        s->capita[i].valor    = json_capere_chorda(c, "valor", vacua);
    }
    /* nomen: <sigillum>-<n>.json */
    per (i = 0; i < nomen_fili.mensura; i++)
    {
        si (nomen_fili.datum[i] == '-')
        {
            linea = (s32)i;
        }
    }
    s->sigillum.datum    = nomen_fili.datum;
    s->sigillum.mensura  = linea > 0 ? (i32)linea : nomen_fili.mensura;
    s->variantes_index   = 0;
    si (linea > 0)
    {
        per (i = (i32)linea + I; i < nomen_fili.mensura
             && _est_cifra(nomen_fili.datum[i]); i++)
        {
            s->variantes_index = s->variantes_index * X
                + (i32)(nomen_fili.datum[i] - '0');
        }
    }
    redde VERUM;
}

interior integer
_specimina_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans HerbariumSpecimen* x =
        *(constans HerbariumSpecimen* constans*)a;
    constans HerbariumSpecimen* y =
        *(constans HerbariumSpecimen* constans*)b;
    i32 brevior = x->sigillum.mensura < y->sigillum.mensura
                    ? x->sigillum.mensura : y->sigillum.mensura;
    integer c = memcmp(x->sigillum.datum, y->sigillum.datum,
        (size_t)brevior);

    si (c != 0)
    {
        redde c;
    }
    si (x->sigillum.mensura != y->sigillum.mensura)
    {
        redde x->sigillum.mensura < y->sigillum.mensura ? -I : I;
    }
    si (x->variantes_index != y->variantes_index)
    {
        redde x->variantes_index < y->variantes_index ? -I : I;
    }
    redde 0;
}

Xar*
herbarium_enumerare (
               Piscina* piscina,
    constans character* directorium)
{
    Xar* fructus = xar_creare(piscina,
                                        (i32)magnitudo(HerbariumSpecimen));
    Xar* lecta   = xar_creare(piscina,
                                        (i32)magnitudo(HerbariumSpecimen));
               character   dir[HERBARIUM_VIA_MAXIMA];
               character   via[HERBARIUM_VIA_MAXIMA + CCLVI];
     DirectoriumIterator*  it;
    DirectoriumIntroitus*  introitus;
       HerbariumSpecimen** ordo;
                     i32   numerus;
                     i32   i;

    si (   !directorium
        || strlen(directorium) + XVI >= HERBARIUM_VIA_MAXIMA)
    {
        redde fructus;
    }
    sprintf(dir, "%s/specimina", directorium);
    it = directorium_iterator_aperire(dir, piscina);
    si (!it)
    {
        redde fructus;
    }
    dum ((introitus = directorium_iterator_proximum(it)) != NIHIL)
    {
        HerbariumSpecimen* s;
                   chorda  titulus;

        si (   introitus->genus           != INTROITUS_FILUM
            || introitus->titulus.mensura <= V
            || memcmp(introitus->titulus.datum
                + introitus->titulus.mensura - V,
                      ".json", V) != 0)
        {
            perge;
        }
        sprintf(via, "%s/%.*s", dir,
            (integer)introitus->titulus.mensura,
                (constans character*)introitus->titulus.datum);
        titulus  = chorda_ex_literis(via + strlen(dir) + I, piscina);
        s        = (HerbariumSpecimen*)xar_addere(lecta);
        si (!_specimen_legere(titulus, filum_legere_totum(via, piscina),
                              s, piscina))
        {
            xar_truncare(lecta, xar_numerus(lecta) - I);
        }
    }
    directorium_iterator_claudere(it);

    numerus = xar_numerus(lecta);
    si (numerus == 0)
    {
        redde fructus;
    }
    ordo = (HerbariumSpecimen**)piscina_allocare(piscina,
        (i64)(numerus * magnitudo(HerbariumSpecimen*)));
    per (i = 0; i < numerus; i++)
    {
        ordo[i] = (HerbariumSpecimen*)xar_obtinere(lecta, i);
    }
    qsort(ordo, (size_t)numerus, magnitudo(HerbariumSpecimen*),
          _specimina_comparare);
    per (i = 0; i < numerus; i++)
    {
        *(HerbariumSpecimen*)xar_addere(fructus) = *ordo[i];
    }
    redde fructus;
}

interior HttpResultus
_exsequi_reddens (
    HttpPetitio* petitio,
        Piscina* piscina,
         vacuum* datum)
{
    HerbariumRedditor* r = (HerbariumRedditor*)datum;
         HttpResultus  res;
        HttpResponsum* resp;
    HerbariumSpecimen* s;

    (vacuum)petitio;
    memset(&res, 0, magnitudo(res));
    si (r->index >= xar_numerus(r->specimina))
    {
        res.error = HTTP_ERROR_CONNEXIO;
        res.error_descriptio = chorda_ex_literis("herbarium exhaustum",
            piscina);
        redde res;
    }
    s = (HerbariumSpecimen*)xar_obtinere(r->specimina, r->index);
    r->index++;
    resp = (HttpResponsum*)piscina_allocare(piscina,
        (i64)magnitudo(HttpResponsum));
    memset(resp, 0, magnitudo(*resp));
    resp->status             = s->status;
    resp->status_descriptio  =
        chorda_ex_literis(http_status_descriptio(s->status),
                                                 piscina);
    resp->corpus          = s->corpus;
    resp->capita          = s->capita;
    resp->capita_numerus  = s->capita_numerus;
    res.successus         = VERUM;
    res.responsum         = resp;
    redde res;
}

HttpVectura
herbarium_reddens (
    Piscina* piscina,
        Xar* specimina)
{
    HerbariumRedditor* r = (HerbariumRedditor*)piscina_allocare(piscina,
        (i64)magnitudo(HerbariumRedditor));
    HttpVectura v;

    r->specimina  = specimina;
    r->index      = 0;
    v.exsequi     = _exsequi_reddens;
    v.datum       = r;
    redde v;
}
