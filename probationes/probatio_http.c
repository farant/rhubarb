/* probatio_http.c - Probationes pro HTTP bibliotheca (cliens)
 *
 * OFFLINE: fixtura localis (hospitium in processu filiato, portus 0
 * auto, portus per fistulam relatus) pro httpbin.org substituitur.
 * Probationes TLS vivae in probatio_tls manent (-reticularis).
 */

#include "http.h"
#include "hospitium.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


/* ========================================================================
 * FIXTURA - SERVUS FILIATUS
 * ======================================================================== */

interior vacuum
_fixtura_get(HospitiumColloquium* colloquium)
{
    chorda corpus = chorda_ex_literis(
        "{\"url\": \"/get\", \"origo\": \"fixtura localis\"}",
        colloquium_piscina(colloquium));
    colloquium_respondere(colloquium, CC, "application/json", corpus);
}

interior vacuum
_fixtura_post(HospitiumColloquium* colloquium)
{
    /* echo corporis petitionis */
    constans HttpPetitioServeri* petitio = colloquium_petitio(colloquium);
    colloquium_respondere(colloquium, CC, "application/json",
                          petitio->corpus);
}

interior vacuum
_fixtura_capita(HospitiumColloquium* colloquium)
{
    chorda corpus = chorda_ex_literis("{\"capita\": \"visa\"}",
                                      colloquium_piscina(colloquium));
    colloquium_respondere(colloquium, CC, "application/json", corpus);
}

interior vacuum
_fixtura_status(HospitiumColloquium* colloquium)
{
    chorda codex_ch = colloquium_param(colloquium, "codex");
    i32 codex = 0;
    i32 i;
    chorda vacua;

    per (i = 0; i < codex_ch.mensura; i++)
    {
        si (codex_ch.datum[i] >= '0' && codex_ch.datum[i] <= '9')
        {
            codex = codex * X + (i32)(codex_ch.datum[i] - '0');
        }
    }
    si (codex < C || codex > D + XCIX)
    {
        codex = CC;
    }

    vacua.datum = NIHIL;
    vacua.mensura = 0;
    colloquium_respondere(colloquium, codex, NIHIL, vacua);
}

/* Incipere fixturam in processu filiato.
 * Redde portum (parens) vel 0 si defectus. */
interior i32
_fixturam_incipere(pid_t* pid_exitus)
{
    integer fistula[II];
    pid_t pid;
    i32 portus = 0;

    si (pipe(fistula) != 0)
    {
        redde 0;
    }

    pid = fork();
    si (pid < 0)
    {
        close(fistula[0]);
        close(fistula[I]);
        redde 0;
    }

    si (pid == 0)
    {
        /* infans = servus; numquam ad probationes redit */
        Piscina* p = piscina_generare_dynamicum("fixtura_http", M * M);
        HospitiumConfiguratio cfg;
        Hospitium* h;
        i32 portus_infantis;

        close(fistula[0]);
        memset(&cfg, 0, magnitudo(cfg));
        h = hospitium_creare(p, &cfg);
        si (h == NIHIL)
        {
            _exit(I);
        }

        hospitium_praebere(h, HTTP_GET, "/get", _fixtura_get, NIHIL);
        hospitium_praebere(h, HTTP_POST, "/post", _fixtura_post, NIHIL);
        hospitium_praebere(h, HTTP_GET, "/headers", _fixtura_capita, NIHIL);
        hospitium_praebere(h, HTTP_GET, "/status/:codex", _fixtura_status,
                           NIHIL);

        portus_infantis = hospitium_portus(h);
        si (write(fistula[I], &portus_infantis,
                  magnitudo(portus_infantis))
            != (ssize_t)magnitudo(portus_infantis))
        {
            _exit(I);
        }
        close(fistula[I]);

        hospitium_currere(h);  /* usque ad SIGTERM ex parente */
        _exit(0);
    }

    /* parens */
    close(fistula[I]);
    si (read(fistula[0], &portus, magnitudo(portus))
        != (ssize_t)magnitudo(portus))
    {
        portus = 0;
    }
    close(fistula[0]);

    *pid_exitus = pid;
    redde portus;
}

interior vacuum
_fixturam_terminare(pid_t pid)
{
    si (pid > 0)
    {
        kill(pid, SIGTERM);
        waitpid(pid, NIHIL, 0);
    }
}

interior constans character*
_url_fixturae(character* buffer, i32 portus, constans character* via)
{
    sprintf(buffer, "http://127.0.0.1:%u%s", portus, via);
    redde buffer;
}


/* ========================================================================
 * PROBATIONES - METHODUS NOMEN
 * ======================================================================== */

interior vacuum
probatio_methodus_nomen(Piscina* piscina)
{
    (vacuum)piscina;

    printf("--- Probans methodus nomen ---\n");

    CREDO_VERUM(strcmp(http_methodus_nomen(HTTP_GET), "GET") == 0);
    CREDO_VERUM(strcmp(http_methodus_nomen(HTTP_POST), "POST") == 0);
    CREDO_VERUM(strcmp(http_methodus_nomen(HTTP_PUT), "PUT") == 0);
    CREDO_VERUM(strcmp(http_methodus_nomen(HTTP_DELETE), "DELETE") == 0);
    CREDO_VERUM(strcmp(http_methodus_nomen(HTTP_PATCH), "PATCH") == 0);
    CREDO_VERUM(strcmp(http_methodus_nomen(HTTP_HEAD), "HEAD") == 0);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - ERROR DESCRIPTIO
 * ======================================================================== */

interior vacuum
probatio_error_descriptio(Piscina* piscina)
{
    constans character* desc;

    (vacuum)piscina;

    printf("--- Probans error descriptio ---\n");

    desc = http_error_descriptio(HTTP_OK);
    CREDO_NON_NIHIL(desc);

    desc = http_error_descriptio(HTTP_ERROR_URL);
    CREDO_NON_NIHIL(desc);

    desc = http_error_descriptio(HTTP_ERROR_CONNEXIO);
    CREDO_NON_NIHIL(desc);

    desc = http_error_descriptio(HTTP_ERROR_PARSE);
    CREDO_NON_NIHIL(desc);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PETITIO CREARE (parse solum, nulla connexio)
 * ======================================================================== */

interior vacuum
probatio_petitio_creare(Piscina* piscina)
{
    HttpPetitio* pet;

    printf("--- Probans petitio creare ---\n");

    /* HTTPS URL */
    pet = http_petitio_creare(piscina, HTTP_GET, "https://httpbin.org/get");
    CREDO_NON_NIHIL(pet);

    /* HTTP URL */
    pet = http_petitio_creare(piscina, HTTP_POST, "http://httpbin.org/post");
    CREDO_NON_NIHIL(pet);

    /* URL cum portus */
    pet = http_petitio_creare(piscina, HTTP_GET, "https://httpbin.org:443/get");
    CREDO_NON_NIHIL(pet);

    /* URL invalida */
    pet = http_petitio_creare(piscina, HTTP_GET, "invalid-url");
    CREDO_NIHIL(pet);

    /* NIHIL argumenta */
    pet = http_petitio_creare(NIHIL, HTTP_GET, "https://httpbin.org/get");
    CREDO_NIHIL(pet);

    pet = http_petitio_creare(piscina, HTTP_GET, NIHIL);
    CREDO_NIHIL(pet);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - GET (FIXTURA LOCALIS)
 * ======================================================================== */

interior vacuum
probatio_get_localis(Piscina* piscina, i32 portus)
{
    HttpPetitio* pet;
    HttpResultus res;
    character url[CXXVIII];

    printf("--- Probans GET (fixtura localis) ---\n");

    pet = http_petitio_creare(piscina, HTTP_GET,
                              _url_fixturae(url, portus, "/get"));
    CREDO_NON_NIHIL(pet);

    res = http_exsequi(pet, piscina);

    CREDO_VERUM(res.successus);
    CREDO_NON_NIHIL(res.responsum);
    CREDO_AEQUALIS_I32(res.responsum->status, CC);
    CREDO_VERUM(res.responsum->corpus.mensura > XX);

    printf("  Status: %d, corpus: %d bytes\n",
           res.responsum->status, res.responsum->corpus.mensura);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - POST (FIXTURA LOCALIS)
 * ======================================================================== */

interior vacuum
probatio_post_localis(Piscina* piscina, i32 portus)
{
    HttpPetitio* pet;
    HttpResultus res;
    character url[CXXVIII];
    constans character* corpus = "{\"name\": \"test\", \"value\": 42}";

    printf("--- Probans POST echo (fixtura localis) ---\n");

    pet = http_petitio_creare(piscina, HTTP_POST,
                              _url_fixturae(url, portus, "/post"));
    CREDO_NON_NIHIL(pet);

    http_petitio_caput_addere(pet, "Content-Type", "application/json");
    http_petitio_corpus_ponere(pet, corpus, (i32)strlen(corpus));

    res = http_exsequi(pet, piscina);

    CREDO_VERUM(res.successus);
    CREDO_NON_NIHIL(res.responsum);
    CREDO_AEQUALIS_I32(res.responsum->status, CC);

    /* fixtura corpus resonat - echo probat corpus trans filum */
    CREDO_VERUM(chorda_aequalis_literis(res.responsum->corpus, corpus));

    printf("  Status: %d, echo: %d bytes\n",
           res.responsum->status, res.responsum->corpus.mensura);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - HEADERS (FIXTURA LOCALIS)
 * ======================================================================== */

interior vacuum
probatio_headers(Piscina* piscina, i32 portus)
{
    HttpPetitio* pet;
    HttpResultus res;
    character url[CXXVIII];
    chorda content_type;

    printf("--- Probans headers (fixtura localis) ---\n");

    pet = http_petitio_creare(piscina, HTTP_GET,
                              _url_fixturae(url, portus, "/headers"));
    CREDO_NON_NIHIL(pet);

    http_petitio_caput_addere(pet, "X-Custom-Header", "test-value");
    http_petitio_caput_addere(pet, "Accept", "application/json");

    res = http_exsequi(pet, piscina);

    CREDO_VERUM(res.successus);
    CREDO_AEQUALIS_I32(res.responsum->status, CC);

    content_type = http_responsum_caput(res.responsum, "Content-Type");
    CREDO_VERUM(content_type.mensura > 0);
    CREDO_VERUM(chorda_aequalis_literis(content_type, "application/json"));
    printf("  Content-Type: %.*s\n", content_type.mensura, content_type.datum);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - STATUS CODES (FIXTURA LOCALIS)
 * ======================================================================== */

interior vacuum
probatio_status_codes(Piscina* piscina, i32 portus)
{
    HttpPetitio* pet;
    HttpResultus res;
    character url[CXXVIII];

    printf("--- Probans status codes (fixtura localis) ---\n");

    /* 404 Not Found per /status/:codex */
    pet = http_petitio_creare(piscina, HTTP_GET,
                              _url_fixturae(url, portus, "/status/404"));
    CREDO_NON_NIHIL(pet);

    res = http_exsequi(pet, piscina);
    CREDO_VERUM(res.successus);
    CREDO_AEQUALIS_I32(res.responsum->status, CDIV);
    printf("  404: %.*s\n",
           res.responsum->status_descriptio.mensura,
           res.responsum->status_descriptio.datum);

    /* 503 quoque */
    pet = http_petitio_creare(piscina, HTTP_GET,
                              _url_fixturae(url, portus, "/status/503"));
    CREDO_NON_NIHIL(pet);

    res = http_exsequi(pet, piscina);
    CREDO_VERUM(res.successus);
    CREDO_AEQUALIS_I32(res.responsum->status, DIII);
    printf("  503: %.*s\n",
           res.responsum->status_descriptio.mensura,
           res.responsum->status_descriptio.datum);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CONNEXIO INVALIDA
 * ======================================================================== */

interior vacuum
probatio_connexio_invalida(Piscina* piscina)
{
    HttpPetitio* pet;
    HttpResultus res;

    printf("--- Probans connexio invalida ---\n");

    pet = http_petitio_creare(piscina, HTTP_GET,
                              "https://non.existens.hospes.example/path");
    CREDO_NON_NIHIL(pet);

    res = http_exsequi(pet, piscina);

    CREDO_FALSUM(res.successus);
    CREDO_AEQUALIS_I32(res.error, HTTP_ERROR_CONNEXIO);

    printf("  Error: %s\n", http_error_descriptio(res.error));
    printf("\n");
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale(vacuum)
{
    Piscina* piscina;
    b32 successus;
    pid_t fixtura_pid = 0;
    i32 portus;

    printf("\n");
    printf("========================================\n");
    printf("PROBATIONES HTTP (fixtura localis)\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_http", CXXVIII * M);
    credo_aperire(piscina);

    /* Probationes locales (nulla connexio) */
    probatio_methodus_nomen(piscina);
    probatio_error_descriptio(piscina);
    probatio_petitio_creare(piscina);
    probatio_connexio_invalida(piscina);

    /* Fixtura: hospitium filiatum, portus auto */
    portus = _fixturam_incipere(&fixtura_pid);
    CREDO_VERUM(portus > 0);
    printf("[fixtura] hospitium filiatum in portu %u (pid %ld)\n\n",
           portus, (longus)fixtura_pid);

    si (portus > 0)
    {
        probatio_get_localis(piscina, portus);
        probatio_post_localis(piscina, portus);
        probatio_headers(piscina, portus);
        probatio_status_codes(piscina, portus);
    }

    _fixturam_terminare(fixtura_pid);

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
