/* oceanus.c - cliens DNS DigitalOcean (vide oceanus.h)
 *
 * Omnis petitio piscinam propriam brevem habet; res servandae
 * (registra) in piscinam oceani copiantur. Clavis in structura sola
 * vivit (capsa fixa), in claudere deletur; caput Authorization in
 * piscina petitionis aedificatur et cum ea perit. Error: nuntius API
 * (campus 'message') aut causa conexionis - clavis numquam. */
#include "postulata_posix.h"
#include "oceanus.h"
#include "http.h"
#include "json.h"
#include "chorda_aedificator.h"
#include "lectiones.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define OCEANUS_BASIS            "https://api.digitalocean.com"
#define OCEANUS_CLAVIS_MAXIMA    CCLVI
#define OCEANUS_PAGINAE_MAXIMAE  L
#define OCEANUS_MORA_MS          (XV * M)
#define OCEANUS_VIA_MAXIMA       DXII

structura Oceanus {
      Piscina* piscina;
    character  clavis[OCEANUS_CLAVIS_MAXIMA];
    character  basis[CCLVI];
    character  error[CCLVI];
};


/* ==================================================
 * CLAVIS
 * ================================================== */

/* clavis CRUDA legitur (numquam lectiones: fabrica contentum
 * sigillaret) - exemplar anthropic.clavis, custodia secretum */
interior b32
_clavem_legere (
     constans character* via,
              character* exitus)
{
       integer fd;
    structura stat st;
       ssize_t n;
           i32 a = 0;
           i32 b;

    /* <tolera codex="CANALIS_PRAETERITUS" (>clavis secreta DO */
    fd = open(via, O_RDONLY | O_NOFOLLOW);
    si (fd < 0)
    {
        fprintf(stderr, "oceanus: clavis legi nequit: %s\n", via);
        redde FALSUM;
    }
    si (fstat(fd, &st) != 0 || !S_ISREG(st.st_mode))
    {
        close(fd);
        fprintf(stderr, "oceanus: clavis non filum regulare: %s\n",
            via);
        redde FALSUM;
    }
    si ((st.st_mode & 077) != 0)
    {
        close(fd);
        fprintf(stderr, "oceanus: clavis modus latior quam 0600 (chmod "
            "600 %s)\n", via);
        redde FALSUM;
    }
    n = read(fd, exitus, OCEANUS_CLAVIS_MAXIMA - I);
    close(fd);
    si (n <= 0)
    {
        fprintf(stderr, "oceanus: clavis vacua: %s\n", via);
        redde FALSUM;
    }
    b          = (i32)n;
    exitus[b]  = '\0';
    dum (   b > 0 && (exitus[b - I] == '\n' || exitus[b - I] == '\r'
         || exitus[b - I] == ' ' || exitus[b - I] == '\t'))
    {
        exitus[--b] = '\0';
    }
    dum (a < b && (exitus[a] == ' ' || exitus[a] == '\t'))
    {
        a++;
    }
    si (a > 0)
    {
        memmove(exitus, exitus + a, (size_t)(b - a + I));
    }
    si (   strncmp(exitus, "dop_v1_", VII) != 0 || strchr(exitus, ' ')
        || strchr(exitus, '\n'))
    {
        memset(exitus, 0, OCEANUS_CLAVIS_MAXIMA);
        fprintf(stderr, "oceanus: clavis non 'dop_v1_...' (signum "
            "personale DigitalOcean): %s\n", via);
        redde FALSUM;
    }
    redde VERUM;
}

Oceanus*
oceanus_creare (
    constans character* via_clavis,
    constans character* basis,
               Piscina* piscina)
{
      Oceanus* o;
    character  via[OCEANUS_VIA_MAXIMA];

    si (!piscina)
    {
        redde NIHIL;
    }
    si (!via_clavis)
    {
        constans character* domus = lectiones_ambitus("HOME");

        si (!domus || strlen(domus) > CD)
        {
            fprintf(stderr, "oceanus: HOME deest\n");
            redde NIHIL;
        }
        sprintf(via, "%s/.rhubarb/digitalocean.clavis", domus);
        via_clavis = via;
    }
    basis = basis ? basis : OCEANUS_BASIS;
    si (strlen(basis) >= CCLVI)
    {
        redde NIHIL;
    }
    o = (Oceanus*)piscina_allocare(piscina, (i64)magnitudo(Oceanus));
    si (!o)
    {
        redde NIHIL;
    }
    memset(o, 0, magnitudo(*o));
    o->piscina = piscina;
    strcpy(o->basis, basis);
    si (!_clavem_legere(via_clavis, o->clavis))
    {
        memset(o->clavis, 0, magnitudo(o->clavis));
        redde NIHIL;
    }
    redde o;
}

vacuum
oceanus_claudere (
    Oceanus* oceanus)
{
    si (oceanus)
    {
        memset(oceanus->clavis, 0, magnitudo(oceanus->clavis));
    }
}

constans character*
oceanus_error (
    Oceanus* oceanus)
{
    redde oceanus ? oceanus->error : "";
}


/* ==================================================
 * PETITIONES
 * ================================================== */

interior b32
_errare (
               Oceanus* o,
    constans character* nuntius)
{
    strncpy(o->error, nuntius, magnitudo(o->error) - I);
    o->error[magnitudo(o->error) - I] = '\0';
    redde FALSUM;
}

/* nomen dominii: [a-z0-9.-] - URL non inicitur */
interior b32
_dominium_validum (
               Oceanus* o,
    constans character* d)
{
    constans character* s;

    si (!d || !d[0] || strlen(d) > CXXVIII)
    {
        redde _errare(o, "dominium malum");
    }
    per (s = d; *s; s++)
    {
        si (!((*s >= 'a' && *s <= 'z') || (*s >= '0' && *s <= '9')
              || *s == '.' || *s == '-'))
        {
            redde _errare(o, "dominium malum (a-z 0-9 . -)");
        }
    }
    redde VERUM;
}

/* petitio una; responsum in *exitus (piscina p). VERUM si status ==
 * status_expectatus; aliter error ex nuntio API */
interior b32
_petere (
               Oceanus* o,
          HttpMethodus  methodus,
    constans character* url,
                chorda  corpus,
                   i32  status_expectatus,
               Piscina* p,
        HttpResponsum** exitus)
{
     HttpPetitio* q;
    HttpResultus  r;
       character* auctoritas;

    q = http_petitio_creare(p, methodus, url);
    si (!q)
    {
        redde _errare(o, "petitio creari nequit (url mala?)");
    }
    auctoritas = (character*)piscina_allocare(p,
        (i64)(strlen(o->clavis) + VIII));
    sprintf(auctoritas, "Bearer %s", o->clavis);
    http_petitio_caput_addere(q, "Authorization", auctoritas);
    memset(auctoritas, 0, strlen(auctoritas));
    http_petitio_caput_addere(q, "Accept", "application/json");
    si (corpus.mensura > 0)
    {
        http_petitio_caput_addere(q, "Content-Type",
            "application/json");
        http_petitio_corpus_ponere_chorda(q, corpus);
    }
    http_petitio_tempus_ponere(q, OCEANUS_MORA_MS);
    r = http_exsequi(q, p);
    si (!r.successus || !r.responsum)
    {
        character m[CCLVI];

        sprintf(m, "conexio fracta: %.*s",
            (int)(r.error_descriptio.mensura > CC ? CC
                  : r.error_descriptio.mensura),
            r.error_descriptio.datum
                ? (constans character*)r.error_descriptio.datum : "");
        redde _errare(o, m);
    }
    si ((i32)r.responsum->status != status_expectatus)
    {
        JsonResultus j        = json_legere(r.responsum->corpus, p);
              chorda nuntius  = chorda_ex_literis("", p);
           character m[CCLVI];

        si (j.successus)
        {
            nuntius = json_capere_chorda(j.radix, "message", nuntius);
        }
        sprintf(m, "DigitalOcean %u: %.*s",
            (insignatus integer)r.responsum->status,
            (int)(nuntius.mensura > CC ? CC : nuntius.mensura),
            nuntius.datum ? (constans character*)nuntius.datum : "");
        redde _errare(o, m);
    }
    *exitus      = r.responsum;
    o->error[0]  = '\0';
    redde VERUM;
}

interior chorda
_copia (
      chorda  s,
     Piscina* p)
{
    chorda c;

    c.mensura  = s.mensura;
    c.datum    = (i8*)piscina_allocare(p, (i64)s.mensura + I);
    si (s.mensura > 0)
    {
        memcpy(c.datum, s.datum, (size_t)s.mensura);
    }
    c.datum[s.mensura] = '\0';
    redde c;
}

interior vacuum
_scheda_legere (
             Oceanus* o,
           JsonValor* v,
    OceanusRegistrum* r,
             Piscina* p)
{
    chorda vacua = chorda_ex_literis("", p);

    r->id = (i64)json_capere_integer(v, "id", 0);
    r->genus = _copia(json_capere_chorda(v, "type", vacua),
        o->piscina);
    r->titulus = _copia(json_capere_chorda(v, "name", vacua),
        o->piscina);
    r->data = _copia(json_capere_chorda(v, "data", vacua),
        o->piscina);
    r->ttl = (i32)json_capere_integer(v, "ttl", 0);
}

interior chorda
_corpus_schedae (
    constans OceanusRegistrum* s,
                      Piscina* p)
{
    JsonValor* o = json_objectum_creare(p);

    json_objectum_ponere(o, "type", json_chorda_creare(p, s->genus));
    json_objectum_ponere(o, "name", json_chorda_creare(p, s->titulus));
    json_objectum_ponere(o, "data", json_chorda_creare(p, s->data));
    json_objectum_ponere(o, "ttl", json_integer_creare(p, (s64)s->ttl));
    redde json_scribere(o, p);
}


/* ==================================================
 * API
 * ================================================== */

b32
oceanus_registra (
               Oceanus* oceanus,
    constans character* dominium,
                   Xar* exitus)
{
    Piscina* p;
  character  url[OCEANUS_VIA_MAXIMA];
        i32  pagina;
        b32  bene  = FALSUM;
        i32  n     = (i32)strlen(oceanus ? oceanus->basis : "");

    si (!oceanus || !exitus || !_dominium_validum(oceanus, dominium))
    {
        redde FALSUM;
    }
    sprintf(url, "%s/v2/domains/%s/records?per_page=200",
        oceanus->basis,
        dominium);
    p = piscina_generare_dynamicum("oceanus_registra", XVI * MXXIV);
    per (pagina = 0; pagina < OCEANUS_PAGINAE_MAXIMAE; pagina++)
    {
        HttpResponsum* r = NIHIL;
         JsonResultus  j;
            JsonValor* tab;
                  i32  k;
               chorda  proxima;

        si (!_petere(oceanus, HTTP_GET, url, chorda_ex_literis("", p),
            CC,
            p, &r))
        {
            frange;
        }
        j = json_legere(r->corpus, p);
        tab = j.successus ? json_objectum_capere(j.radix,
            "domain_records")
            : NIHIL;
        si (!tab || !json_est_tabulatum(tab))
        {
            (vacuum)_errare(oceanus, "responsum sine domain_records");
            frange;
        }
        per (k = 0; k < (i32)json_tabulatum_numerus(tab); k++)
        {
            OceanusRegistrum* s = (OceanusRegistrum*)xar_addere(exitus);

            _scheda_legere(oceanus, json_tabulatum_obtinere(tab, k), s,
                p);
        }
        proxima = json_capere_chorda(json_via(j.radix, "links.pages"),
            "next", chorda_ex_literis("", p));
        si (proxima.mensura == 0)
        {
            bene = VERUM;
            frange;
        }
        /* pagina proxima SOLUM sub basi: clavis ad hospitem alium
         * numquam mittitur */
        si (   proxima.mensura >= OCEANUS_VIA_MAXIMA
            || proxima.mensura <= n
            || memcmp(proxima.datum, oceanus->basis, (size_t)n) != 0
            || proxima.datum[n] != '/')
        {
            (vacuum)_errare(oceanus, "pagina proxima extra basim - non "
                "secuta");
            frange;
        }
        memcpy(url, proxima.datum, (size_t)proxima.mensura);
        url[proxima.mensura] = '\0';
    }
    si (pagina == OCEANUS_PAGINAE_MAXIMAE)
    {
        (vacuum)_errare(oceanus, "paginae nimis multae");
    }
    piscina_destruere(p);
    redde bene;
}

b32
oceanus_registrum_creare (
                      Oceanus* oceanus,
           constans character* dominium,
    constans OceanusRegistrum* scheda,
                          i64* id)
{
        Piscina* p;
      character  url[OCEANUS_VIA_MAXIMA];
  HttpResponsum* r = NIHIL;
            b32  bene;

    si (!oceanus || !scheda || !_dominium_validum(oceanus, dominium))
    {
        redde FALSUM;
    }
    sprintf(url, "%s/v2/domains/%s/records", oceanus->basis, dominium);
    p = piscina_generare_dynamicum("oceanus_creare", IV * MXXIV);
    bene = _petere(oceanus, HTTP_POST, url, _corpus_schedae(scheda, p),
        CCI,
        p, &r);
    si (bene && id)
    {
        JsonResultus j = json_legere(r->corpus, p);

        *id =
            j.successus ? (i64)json_capere_integer(json_objectum_capere(
            j.radix, "domain_record"), "id", 0) : 0;
    }
    piscina_destruere(p);
    redde bene;
}

b32
oceanus_registrum_mutare (
                      Oceanus* oceanus,
           constans character* dominium,
                          i64  id,
    constans OceanusRegistrum* scheda)
{
        Piscina* p;
      character  url[OCEANUS_VIA_MAXIMA];
  HttpResponsum* r = NIHIL;
            b32  bene;

    si (!oceanus || !scheda || !_dominium_validum(oceanus, dominium))
    {
        redde FALSUM;
    }
    sprintf(url, "%s/v2/domains/%s/records/%lu", oceanus->basis,
        dominium,
        (insignatus longus)id);
    p = piscina_generare_dynamicum("oceanus_mutare", IV * MXXIV);
    bene = _petere(oceanus, HTTP_PUT, url, _corpus_schedae(scheda, p),
        CC,
        p, &r);
    piscina_destruere(p);
    redde bene;
}

b32
oceanus_registrum_delere (
               Oceanus* oceanus,
    constans character* dominium,
                   i64  id)
{
        Piscina* p;
      character  url[OCEANUS_VIA_MAXIMA];
  HttpResponsum* r = NIHIL;
            b32  bene;

    si (!oceanus || !_dominium_validum(oceanus, dominium))
    {
        redde FALSUM;
    }
    sprintf(url, "%s/v2/domains/%s/records/%lu", oceanus->basis,
        dominium,
        (insignatus longus)id);
    p = piscina_generare_dynamicum("oceanus_delere", MXXIV);
    bene = _petere(oceanus, HTTP_DELETE, url, chorda_ex_literis("", p),
        CCIV, p, &r);
    piscina_destruere(p);
    redde bene;
}

OceanusExitus
oceanus_assecurare (
               Oceanus* oceanus,
    constans character* dominium,
    constans character* genus,
    constans character* titulus,
    constans character* data,
                   i32  ttl)
{
             Piscina* p;
                 Xar* x;
    OceanusRegistrum* inventum = NIHIL;
    OceanusRegistrum  novum;
                 i32  k;
                 i32  congruentia = 0;
       OceanusExitus  fructus;

    si (!oceanus || !genus || !titulus || !data)
    {
        redde OCEANUS_ERROR;
    }
    p = piscina_generare_dynamicum("oceanus_assecurare", IV * MXXIV);
    x = xar_creare(p, (i32)magnitudo(OceanusRegistrum));
    si (!oceanus_registra(oceanus, dominium, x))
    {
        piscina_destruere(p);
        redde OCEANUS_ERROR;
    }
    per (k = 0; k < (i32)xar_numerus(x); k++)
    {
        OceanusRegistrum* r = (OceanusRegistrum*)xar_obtinere(x, k);

        si (   chorda_aequalis_literis(r->genus, genus)
            && chorda_aequalis_literis(r->titulus, titulus))
        {
            congruentia++;
            inventum = r;
        }
    }
    novum.id       = 0;
    novum.genus    = chorda_ex_literis(genus, p);
    novum.titulus  = chorda_ex_literis(titulus, p);
    novum.data     = chorda_ex_literis(data, p);
    novum.ttl      = ttl;
    si (congruentia > I)
    {
        (vacuum)_errare(oceanus, "plura registra eiusdem generis et "
            "tituli - ambiguum, nihil tactum");
        fructus = OCEANUS_ERROR;
    }
    alioquin si (!inventum)
    {
        fructus = oceanus_registrum_creare(oceanus, dominium, &novum,
            NIHIL) ? OCEANUS_CREATUM : OCEANUS_ERROR;
    }
    alioquin si (   chorda_aequalis_literis(inventum->data, data)
                 && inventum->ttl == ttl)
    {
        fructus = OCEANUS_IDEM;   /* nulla scriptio */
    }
    alioquin
    {
        fructus = oceanus_registrum_mutare(oceanus, dominium,
            inventum->id,
            &novum) ? OCEANUS_MUTATUM : OCEANUS_ERROR;
    }
    piscina_destruere(p);
    redde fructus;
}
