/* rete_fumus.c - fumus retis: TLS, refusationes certificatorum,
 * tempora longa et brevia, API Anthropic (vates-plan-1 T1).
 *
 * NON PORTA: rete (et API) poscit - manu curritur, numquam in suite.
 *
 * Curre per: ./tools/rete_fumus.sh [-tls] [-badssl] [-mora] [-anthropic]
 * Sine optionibus omnia. Exitus 0 = omnes exspectationes impletae.
 *
 * Clavis: ~/.rhubarb/anthropic.clavis (0600, a Frano scripta). In
 * processu legitur, in caput x-api-key ponitur, NUMQUAM imprimitur.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "http.h"
#include "json.h"
#include "filum.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens i32 _fracti = 0;

interior vacuum
_nuntiare (
    constans character* titulus,
                   b32  bene,
    constans character* nota)
{
    imprimere("%s  %-46s %s\n", bene ? "BENE " : "FRACT", titulus,
              nota ? nota : "");
    si (!bene)
    {
        _fracti++;
    }
}

interior HttpResultus
_petere (
               Piscina* piscina,
    constans character* url,
           MoraAngusta  tempus_ms)
{
     HttpPetitio* petitio;
    HttpResultus  res;

    petitio = http_petitio_creare(piscina, HTTP_GET, url);
    si (!petitio)
    {
        memset(&res, 0, magnitudo(res));
        res.error = HTTP_ERROR_URL;
        redde res;
    }
    si (tempus_ms > 0)
    {
        http_petitio_tempus_ponere(petitio, tempus_ms);
    }
    redde http_exsequi(petitio, piscina);
}

/* ---- I. quam versionem TLS tractamus? ---- */
interior vacuum
_probare_tls (
    Piscina* piscina)
{
    HttpResultus res;
    JsonResultus json;
       character nota[CCLVI];
          chorda versio;
          chorda gradus;

    res = _petere(piscina, "https://www.howsmyssl.com/a/check", 0);
    si (!res.successus || res.responsum->status != CC)
    {
        _nuntiare("howsmyssl: responsum CC", FALSUM,
                  http_error_descriptio(res.error));
        redde;
    }
    json = json_legere(res.responsum->corpus, piscina);
    si (!json.successus)
    {
        _nuntiare("howsmyssl: JSON", FALSUM, "non legibile");
        redde;
    }
    versio = json_capere_chorda(json.radix, "tls_version",
                                chorda_ex_literis("?", piscina));
    gradus = json_capere_chorda(json.radix, "rating",
                                chorda_ex_literis("?", piscina));
    sprintf(nota, "%.*s, gradus %.*s",
            (integer)versio.mensura, (constans character*)versio.datum,
            (integer)gradus.mensura, (constans character*)gradus.datum);
    _nuntiare("howsmyssl: versio TLS tractata", VERUM, nota);

    /* causae gradus: quae howsmyssl ut insecura notat */
    {
        JsonValor* infirmae = json_objectum_capere(json.radix,
                                                    "insecure_cipher_suites");
                      chorda  clavis;
                   JsonValor* valor;
        JsonObjectumIterator  it;
                         i32  numerus_infirmarum = 0;

        si (infirmae && json_est_objectum(infirmae))
        {
            it = json_objectum_iterator(infirmae);
            dum (json_objectum_iterator_proxima(&it, &clavis, &valor))
            {
                imprimere("        insecura: %.*s\n",
                    (integer)clavis.mensura,
                          (constans character*)clavis.datum);
                numerus_infirmarum++;
            }
        }
        /* asseritur, non solum imprimitur (T1b): sine hoc planta quae
         * indicem cifrarum exstinguit viridis manebat */
        _nuntiare("howsmyssl: nullae cifrae insecurae oblatae",
                  infirmae != NIHIL && numerus_infirmarum == 0,
                  infirmae
                      == NIHIL ? "campus insecure_cipher_suites abest"
                                    : "");
        imprimere("        compressio TLS: %s, beast: %s, tesserae sessionis: %s\n",
                  json_capere_boolean(json.radix,
                  "tls_compression_supported", FALSUM) ? "ita" : "non",
                  json_capere_boolean(json.radix, "beast_vuln",
                  FALSUM) ? "ita" : "non",
                  json_capere_boolean(json.radix,
                  "session_ticket_supported", FALSUM) ? "ita" : "non");
    }
}

/* ---- II. quod recusare debemus, recusamusne? ---- */
interior vacuum
_probare_recusationem (
               Piscina* piscina,
    constans character* url,
                   b32  recusandum)
{
    HttpResultus res;
       character titulus[CCLVI];

    res = _petere(piscina, url, XV * M);
    sprintf(titulus, "%s %s", recusandum ? "recusat" : "accipit", url
        + VIII);
    si (recusandum)
    {
        _nuntiare(titulus, !res.successus,
                  res.successus ? "ACCEPTUM - exspectabatur recusatio" : "");
    }
    alioquin
    {
        _nuntiare(titulus, res.successus,
                  res.successus ? "" : http_error_descriptio(res.error));
    }
}

interior vacuum
_probare_badssl (
    Piscina* piscina)
{
    _probare_recusationem(piscina, "https://expired.badssl.com/",
        VERUM);
    _probare_recusationem(piscina, "https://wrong.host.badssl.com/",
        VERUM);
    _probare_recusationem(piscina, "https://self-signed.badssl.com/",
        VERUM);
    _probare_recusationem(piscina, "https://untrusted-root.badssl.com/",
        VERUM);
    _probare_recusationem(piscina, "https://tls-v1-0.badssl.com:1010/",
        VERUM);
    _probare_recusationem(piscina, "https://tls-v1-1.badssl.com:1011/",
        VERUM);
    _probare_recusationem(piscina, "https://tls-v1-2.badssl.com:1012/",
        FALSUM);
}

/* ---- III. tempus vocantis in via HTTPS ---- */
interior vacuum
_probare_moram (
    Piscina* piscina)
{
    HttpResultus res;

    /* longa (custodia): responsum post V s, tempus XX s - succedere
     * debet ante et post emendationem. httpbin /delay/n ad X s
     * limitatur: casus > XXX s hic fingi non potest. */
    res = _petere(piscina, "https://httpbin.org/delay/5", XX * M);
    _nuntiare("mora longa: /delay/5 cum tempore XX s",
              res.successus && res.responsum->status == CC,
              res.successus ? "" : http_error_descriptio(res.error));

    /* brevis: responsum post X s, tempus III s - debet deficere TEMPUS */
    res = _petere(piscina, "https://httpbin.org/delay/10", III * M);
    _nuntiare("mora brevis: /delay/10 cum tempore III s",
              !res.successus && res.error == HTTP_ERROR_TIMEOUT,
              res.successus ? "SUCCESSIT - tempus neglectum"
                            : http_error_descriptio(res.error));
}

/* ---- IV. API Anthropic: GET /v1/models (gratis) ---- */
interior vacuum
_probare_anthropic (
    Piscina* piscina)
{
    constans character* domus;
             character  via[MXXIV];
             character  clavis[DXII];
                chorda  crudum;
           HttpPetitio* petitio;
          HttpResultus  res;
          JsonResultus  json;
             JsonValor* data;
             character  nota[DXII];
                   i32  numerus;
                   i32  i;

    domus = getenv("HOME");
    si (!domus)
    {
        _nuntiare("anthropic: HOME", FALSUM, "HOME non positum");
        redde;
    }
    sprintf(via, "%s/.rhubarb/anthropic.clavis", domus);
    si (!filum_existit(via))
    {
        _nuntiare("anthropic: clavis", FALSUM,
                  "~/.rhubarb/anthropic.clavis abest (T1 gradus 1.1)");
        redde;
    }
    crudum = chorda_praecidere(filum_legere_totum(via, piscina));
    si (crudum.mensura == 0 || crudum.mensura >= (i32)magnitudo(clavis))
    {
        _nuntiare("anthropic: clavis", FALSUM, "vacua aut nimis longa");
        redde;
    }
    memcpy(clavis, crudum.datum, (size_t)crudum.mensura);
    clavis[crudum.mensura] = '\0';

    petitio = http_petitio_creare(piscina, HTTP_GET,
                                  "https://api.anthropic.com/v1/models");
    http_petitio_caput_addere(petitio, "x-api-key", clavis);
    http_petitio_caput_addere(petitio, "anthropic-version",
        "2023-06-01");
    res = http_exsequi(petitio, piscina);
    memset(clavis, 0, magnitudo(clavis));

    si (!res.successus)
    {
        _nuntiare("anthropic: /v1/models", FALSUM,
                  http_error_descriptio(res.error));
        redde;
    }
    si (res.responsum->status != CC)
    {
        sprintf(nota, "status %d", (integer)res.responsum->status);
        _nuntiare("anthropic: /v1/models", FALSUM, nota);
        redde;
    }
    json = json_legere(res.responsum->corpus, piscina);
    data = json.successus ? json_objectum_capere(json.radix,
        "data") : NIHIL;
    numerus = data ? json_tabulatum_numerus(data) : 0;
    sprintf(nota, "%u exemplaria", numerus);
    _nuntiare("anthropic: /v1/models", numerus > 0, nota);
    per (i = 0; i < numerus && i < VIII; i++)
    {
        chorda id = json_capere_chorda(json_tabulatum_obtinere(data, i),
                                       "id", chorda_ex_literis("?",
                                       piscina));
        imprimere("        %.*s\n", (integer)id.mensura,
                  (constans character*)id.datum);
    }
}

s32
principale (
      integer   argc,
    character** argv)
{
     Piscina* piscina;
         b32  omnia;
         b32  tls_       = FALSUM;
         b32  badssl     = FALSUM;
         b32  mora       = FALSUM;
         b32  anthropic  = FALSUM;
     integer  i;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-tls") == 0)       tls_      = VERUM;
        alioquin si (strcmp(argv[i], "-badssl") == 0)    badssl    =
                                                             VERUM;
        alioquin si (strcmp(argv[i], "-mora") == 0)      mora      =
                                                             VERUM;
        alioquin si (strcmp(argv[i], "-anthropic") == 0) anthropic =
                                                             VERUM;
        alioquin
        {
            fprintf(stderr, "optio ignota: %s\n", argv[i]);
            redde II;
        }
    }
    omnia = !tls_ && !badssl && !mora && !anthropic;

    piscina = piscina_generare_dynamicum("rete_fumus", M * M);
    si (!piscina)
    {
        redde II;
    }
    si (omnia || tls_)      _probare_tls(piscina);
    si (omnia || badssl)    _probare_badssl(piscina);
    si (omnia || mora)      _probare_moram(piscina);
    si (omnia || anthropic) _probare_anthropic(piscina);

    imprimere("\nfumus retis: %s (%u fracti)\n",
              _fracti == 0 ? "sanum" : "FRACTUM", _fracti);
    piscina_destruere(piscina);
    redde _fracti == 0 ? 0 : I;
}
