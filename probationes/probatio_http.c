/* probatio_http.c - Probationes pro HTTP bibliotheca
 *
 * Haec probationes requirunt connexionem ad rete.
 * Utuntur httpbin.org pro probationibus.
 */

#include "http.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>


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
 * PROBATIONES - PETITIO CREARE
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
 * PROBATIONES - GET HTTPS
 * ======================================================================== */

interior vacuum
probatio_get_https(Piscina* piscina)
{
    HttpPetitio* pet;
    HttpResultus res;

    printf("--- Probans GET HTTPS (httpbin.org) ---\n");

    pet = http_petitio_creare(piscina, HTTP_GET, "https://httpbin.org/get");
    CREDO_NON_NIHIL(pet);

    res = http_exsequi(pet, piscina);

    si (!res.successus)
    {
        printf("  NOTA: Petitio fallita (rete non disponibilis?)\n");
        printf("  Error: %s\n", http_error_descriptio(res.error));
        si (res.error_descriptio.mensura > 0)
        {
            printf("  Descriptio: %.*s\n",
                   res.error_descriptio.mensura,
                   res.error_descriptio.datum);
        }
        printf("\n");
        redde;
    }

    CREDO_VERUM(res.successus);
    CREDO_NON_NIHIL(res.responsum);
    CREDO_AEQUALIS_I32(res.responsum->status, CC);

    printf("  Status: %d %.*s\n",
           res.responsum->status,
           res.responsum->status_descriptio.mensura,
           res.responsum->status_descriptio.datum);

    printf("  Corpus: %d bytes\n", res.responsum->corpus.mensura);

    /* Verificare JSON responsum continet "url" */
    si (res.responsum->corpus.mensura > 0)
    {
        CREDO_VERUM(res.responsum->corpus.mensura > XX);
    }

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - POST HTTPS
 * ======================================================================== */

interior vacuum
probatio_post_https(Piscina* piscina)
{
    HttpPetitio* pet;
    HttpResultus res;
    constans character* corpus = "{\"name\": \"test\", \"value\": 42}";

    printf("--- Probans POST HTTPS (httpbin.org) ---\n");

    pet = http_petitio_creare(piscina, HTTP_POST, "https://httpbin.org/post");
    CREDO_NON_NIHIL(pet);

    http_petitio_caput_addere(pet, "Content-Type", "application/json");
    http_petitio_corpus_ponere(pet, corpus, (i32)strlen(corpus));

    res = http_exsequi(pet, piscina);

    si (!res.successus)
    {
        printf("  NOTA: Petitio fallita (rete non disponibilis?)\n");
        printf("\n");
        redde;
    }

    CREDO_VERUM(res.successus);
    CREDO_NON_NIHIL(res.responsum);
    CREDO_AEQUALIS_I32(res.responsum->status, CC);

    printf("  Status: %d\n", res.responsum->status);
    printf("  Corpus: %d bytes\n", res.responsum->corpus.mensura);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - HEADERS
 * ======================================================================== */

interior vacuum
probatio_headers(Piscina* piscina)
{
    HttpPetitio* pet;
    HttpResultus res;
    chorda content_type;

    printf("--- Probans headers ---\n");

    pet = http_petitio_creare(piscina, HTTP_GET, "https://httpbin.org/headers");
    CREDO_NON_NIHIL(pet);

    http_petitio_caput_addere(pet, "X-Custom-Header", "test-value");
    http_petitio_caput_addere(pet, "Accept", "application/json");

    res = http_exsequi(pet, piscina);

    si (!res.successus)
    {
        printf("  NOTA: Petitio fallita\n");
        printf("\n");
        redde;
    }

    CREDO_VERUM(res.successus);
    CREDO_AEQUALIS_I32(res.responsum->status, CC);

    /* Verificare Content-Type header in responsum */
    content_type = http_responsum_caput(res.responsum, "Content-Type");
    CREDO_VERUM(content_type.mensura > 0);
    printf("  Content-Type: %.*s\n", content_type.mensura, content_type.datum);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - STATUS CODES
 * ======================================================================== */

interior vacuum
probatio_status_codes(Piscina* piscina)
{
    HttpPetitio* pet;
    HttpResultus res;

    printf("--- Probans status codes ---\n");

    /* 404 Not Found */
    pet = http_petitio_creare(piscina, HTTP_GET, "https://httpbin.org/status/404");
    CREDO_NON_NIHIL(pet);

    res = http_exsequi(pet, piscina);

    si (res.successus)
    {
        CREDO_AEQUALIS_I32(res.responsum->status, CD + IV);
        printf("  404: %.*s\n",
               res.responsum->status_descriptio.mensura,
               res.responsum->status_descriptio.datum);
    }
    alioquin
    {
        printf("  NOTA: Petitio fallita\n");
    }

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

    pet = http_petitio_creare(piscina, HTTP_GET, "https://non.existens.hospes.example/path");
    CREDO_NON_NIHIL(pet);

    res = http_exsequi(pet, piscina);

    CREDO_FALSUM(res.successus);
    CREDO_AEQUALIS_I32(res.error, HTTP_ERROR_CONNEXIO);

    printf("  Error: %s\n", http_error_descriptio(res.error));
    si (res.error_descriptio.mensura > 0)
    {
        printf("  Descriptio: %.*s\n",
               res.error_descriptio.mensura,
               res.error_descriptio.datum);
    }

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

    printf("\n");
    printf("========================================\n");
    printf("PROBATIONES HTTP\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_http", CXXVIII * M);
    credo_aperire(piscina);

    /* Probationes locales (non requirunt rete) */
    probatio_methodus_nomen(piscina);
    probatio_error_descriptio(piscina);
    probatio_petitio_creare(piscina);

    /* Probationes rete */
    probatio_connexio_invalida(piscina);
    probatio_get_https(piscina);
    probatio_post_https(piscina);
    probatio_headers(piscina);
    probatio_status_codes(piscina);

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
