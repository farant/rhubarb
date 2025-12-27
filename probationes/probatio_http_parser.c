/* probatio_http_parser.c - Probationes pro HTTP Request Parser
 *
 * Probationes parsing petitionum HTTP serveri.
 */

#include "http.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>


/* ========================================================================
 * PROBATIONES - METHODUS EX LITERIS
 * ======================================================================== */

interior vacuum
probatio_methodus_ex_literis(vacuum)
{
    printf("--- Probans methodus_ex_literis ---\n");

    CREDO_VERUM(http_methodus_ex_literis("GET") == HTTP_GET);
    CREDO_VERUM(http_methodus_ex_literis("POST") == HTTP_POST);
    CREDO_VERUM(http_methodus_ex_literis("PUT") == HTTP_PUT);
    CREDO_VERUM(http_methodus_ex_literis("DELETE") == HTTP_DELETE);
    CREDO_VERUM(http_methodus_ex_literis("PATCH") == HTTP_PATCH);
    CREDO_VERUM(http_methodus_ex_literis("HEAD") == HTTP_HEAD);
    CREDO_VERUM(http_methodus_ex_literis("OPTIONS") == HTTP_OPTIONS);
    CREDO_VERUM(http_methodus_ex_literis("UNKNOWN") == HTTP_METHODUS_IGNOTUS);
    CREDO_VERUM(http_methodus_ex_literis(NIHIL) == HTTP_METHODUS_IGNOTUS);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PARSE SIMPLEX
 * ======================================================================== */

interior vacuum
probatio_parse_get_simplex(Piscina* piscina)
{
    constans character* petitio =
        "GET /path HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "\r\n";
    HttpParseResultus res;

    printf("--- Probans parse GET simplex ---\n");

    res = http_petitio_parse(petitio, (i32)strlen(petitio), piscina);

    CREDO_VERUM(res.successus);
    CREDO_VERUM(res.completa);
    CREDO_NON_NIHIL(res.petitio);

    CREDO_VERUM(res.petitio->methodus == HTTP_GET);
    CREDO_VERUM(chorda_aequalis_literis(res.petitio->uri, "/path"));
    CREDO_VERUM(chorda_aequalis_literis(res.petitio->via, "/path"));
    CREDO_VERUM(res.petitio->versio == XI);
    CREDO_VERUM(res.petitio->keep_alive == VERUM);

    printf("  Methodus: %s\n", http_methodus_nomen(res.petitio->methodus));
    printf("  Via: %.*s\n", res.petitio->via.mensura, res.petitio->via.datum);
    printf("\n");
}


interior vacuum
probatio_parse_post_cum_corpus(Piscina* piscina)
{
    constans character* petitio =
        "POST /api/users HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 28\r\n"
        "\r\n"
        "{\"name\":\"Claudius\",\"age\":42}";
    HttpParseResultus res;

    printf("--- Probans parse POST cum corpus ---\n");

    res = http_petitio_parse(petitio, (i32)strlen(petitio), piscina);

    CREDO_VERUM(res.successus);
    CREDO_VERUM(res.completa);
    CREDO_NON_NIHIL(res.petitio);

    CREDO_VERUM(res.petitio->methodus == HTTP_POST);
    CREDO_VERUM(chorda_aequalis_literis(res.petitio->via, "/api/users"));
    CREDO_VERUM(res.petitio->content_length == XXVIII);
    CREDO_VERUM(res.petitio->corpus.mensura == XXVIII);
    CREDO_VERUM(chorda_aequalis_literis(res.petitio->corpus,
                                         "{\"name\":\"Claudius\",\"age\":42}"));

    printf("  Methodus: %s\n", http_methodus_nomen(res.petitio->methodus));
    printf("  Corpus: %.*s\n", res.petitio->corpus.mensura, res.petitio->corpus.datum);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PARSE CUM QUAESTIO
 * ======================================================================== */

interior vacuum
probatio_parse_cum_quaestio(Piscina* piscina)
{
    constans character* petitio =
        "GET /search?q=test&lang=la HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "\r\n";
    HttpParseResultus res;

    printf("--- Probans parse cum quaestio ---\n");

    res = http_petitio_parse(petitio, (i32)strlen(petitio), piscina);

    CREDO_VERUM(res.successus);
    CREDO_VERUM(res.completa);
    CREDO_NON_NIHIL(res.petitio);

    CREDO_VERUM(chorda_aequalis_literis(res.petitio->uri, "/search?q=test&lang=la"));
    CREDO_VERUM(chorda_aequalis_literis(res.petitio->via, "/search"));
    CREDO_VERUM(chorda_aequalis_literis(res.petitio->quaestio, "q=test&lang=la"));

    printf("  URI: %.*s\n", res.petitio->uri.mensura, res.petitio->uri.datum);
    printf("  Via: %.*s\n", res.petitio->via.mensura, res.petitio->via.datum);
    printf("  Quaestio: %.*s\n", res.petitio->quaestio.mensura, res.petitio->quaestio.datum);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PARSE HEADERS
 * ======================================================================== */

interior vacuum
probatio_parse_headers(Piscina* piscina)
{
    constans character* petitio =
        "GET /api HTTP/1.1\r\n"
        "Host: api.example.com\r\n"
        "Authorization: Bearer token123\r\n"
        "Accept: application/json\r\n"
        "User-Agent: Rhubarb/1.0\r\n"
        "\r\n";
    HttpParseResultus res;
    i32 i;

    printf("--- Probans parse headers ---\n");

    res = http_petitio_parse(petitio, (i32)strlen(petitio), piscina);

    CREDO_VERUM(res.successus);
    CREDO_VERUM(res.completa);
    CREDO_NON_NIHIL(res.petitio);

    CREDO_VERUM(res.petitio->capita_numerus == IV);

    printf("  Headers (%d):\n", res.petitio->capita_numerus);
    per (i = 0; i < res.petitio->capita_numerus; i++)
    {
        printf("    %.*s: %.*s\n",
               res.petitio->capita[i].titulus.mensura,
               res.petitio->capita[i].titulus.datum,
               res.petitio->capita[i].valor.mensura,
               res.petitio->capita[i].valor.datum);
    }
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - HTTP VERSIO
 * ======================================================================== */

interior vacuum
probatio_http_versio(Piscina* piscina)
{
    constans character* petitio_10 =
        "GET / HTTP/1.0\r\n"
        "Host: example.com\r\n"
        "\r\n";
    constans character* petitio_11 =
        "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "\r\n";
    HttpParseResultus res;

    printf("--- Probans HTTP versio ---\n");

    /* HTTP/1.0 */
    res = http_petitio_parse(petitio_10, (i32)strlen(petitio_10), piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(res.petitio->versio == X);
    CREDO_VERUM(res.petitio->keep_alive == FALSUM);  /* Default pro 1.0 */
    printf("  HTTP/1.0: versio=%d, keep_alive=%d\n",
           res.petitio->versio, res.petitio->keep_alive);

    /* HTTP/1.1 */
    res = http_petitio_parse(petitio_11, (i32)strlen(petitio_11), piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(res.petitio->versio == XI);
    CREDO_VERUM(res.petitio->keep_alive == VERUM);  /* Default pro 1.1 */
    printf("  HTTP/1.1: versio=%d, keep_alive=%d\n",
           res.petitio->versio, res.petitio->keep_alive);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CONNECTION HEADER
 * ======================================================================== */

interior vacuum
probatio_connection_header(Piscina* piscina)
{
    constans character* petitio_close =
        "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Connection: close\r\n"
        "\r\n";
    constans character* petitio_keepalive =
        "GET / HTTP/1.0\r\n"
        "Host: example.com\r\n"
        "Connection: keep-alive\r\n"
        "\r\n";
    HttpParseResultus res;

    printf("--- Probans Connection header ---\n");

    /* HTTP/1.1 cum Connection: close */
    res = http_petitio_parse(petitio_close, (i32)strlen(petitio_close), piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(res.petitio->keep_alive == FALSUM);
    printf("  1.1 + close: keep_alive=%d\n", res.petitio->keep_alive);

    /* HTTP/1.0 cum Connection: keep-alive */
    res = http_petitio_parse(petitio_keepalive, (i32)strlen(petitio_keepalive), piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(res.petitio->keep_alive == VERUM);
    printf("  1.0 + keep-alive: keep_alive=%d\n", res.petitio->keep_alive);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PARSER INCREMENTALIS
 * ======================================================================== */

interior vacuum
probatio_parser_incrementalis(Piscina* piscina)
{
    HttpParser* parser;
    HttpParseResultus res;
    constans character* pars1 = "GET /path HTTP/1.1\r\n";
    constans character* pars2 = "Host: example.com\r\n";
    constans character* pars3 = "\r\n";

    printf("--- Probans parser incrementalis ---\n");

    parser = http_parser_creare(piscina);
    CREDO_NON_NIHIL(parser);

    /* Prima pars - request line */
    res = http_parser_adicere(parser, pars1, (i32)strlen(pars1));
    CREDO_VERUM(res.successus);
    CREDO_FALSUM(res.completa);
    printf("  Post pars1: completa=%d\n", res.completa);

    /* Secunda pars - header */
    res = http_parser_adicere(parser, pars2, (i32)strlen(pars2));
    CREDO_VERUM(res.successus);
    CREDO_FALSUM(res.completa);
    printf("  Post pars2: completa=%d\n", res.completa);

    /* Tertia pars - finis */
    res = http_parser_adicere(parser, pars3, (i32)strlen(pars3));
    CREDO_VERUM(res.successus);
    CREDO_VERUM(res.completa);
    printf("  Post pars3: completa=%d\n", res.completa);

    CREDO_VERUM(http_parser_est_completa(parser));

    /* Verificare petitio */
    CREDO_NON_NIHIL(http_parser_obtinere_petitio(parser));
    CREDO_VERUM(http_parser_obtinere_petitio(parser)->methodus == HTTP_GET);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - OMNES METHODI
 * ======================================================================== */

interior vacuum
probatio_omnes_methodi(Piscina* piscina)
{
    HttpMethodus methodi[] = {HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_DELETE,
                              HTTP_PATCH, HTTP_HEAD, HTTP_OPTIONS};
    constans character* nomina[] = {"GET", "POST", "PUT", "DELETE",
                                     "PATCH", "HEAD", "OPTIONS"};
    i32 i;
    character petitio[CXXVIII];
    HttpParseResultus res;

    printf("--- Probans omnes methodi ---\n");

    per (i = 0; i < VII; i++)
    {
        /* Construere petitio */
        sprintf(petitio, "%s / HTTP/1.1\r\nHost: test.com\r\n\r\n", nomina[i]);

        res = http_petitio_parse(petitio, (i32)strlen(petitio), piscina);
        CREDO_VERUM(res.successus);
        CREDO_VERUM(res.petitio->methodus == methodi[i]);
        printf("  %s: OK\n", nomina[i]);
    }

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - NULLUM ARGUMENTA
 * ======================================================================== */

interior vacuum
probatio_nullum_argumenta(Piscina* piscina)
{
    HttpParseResultus res;
    HttpParser* parser;

    printf("--- Probans nullum argumenta ---\n");

    /* Parse cum NIHIL */
    res = http_petitio_parse(NIHIL, X, piscina);
    CREDO_FALSUM(res.successus);

    res = http_petitio_parse("GET / HTTP/1.1\r\n\r\n", 0, piscina);
    CREDO_FALSUM(res.successus);

    res = http_petitio_parse("GET / HTTP/1.1\r\n\r\n", X, NIHIL);
    CREDO_FALSUM(res.successus);

    /* Parser cum NIHIL */
    parser = http_parser_creare(NIHIL);
    CREDO_NIHIL(parser);

    CREDO_FALSUM(http_parser_est_completa(NIHIL));
    CREDO_NIHIL(http_parser_obtinere_petitio(NIHIL));

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
    printf("PROBATIONES HTTP PARSER\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_http_parser", CXXVIII * M);
    credo_aperire(piscina);

    probatio_methodus_ex_literis();
    probatio_parse_get_simplex(piscina);
    probatio_parse_post_cum_corpus(piscina);
    probatio_parse_cum_quaestio(piscina);
    probatio_parse_headers(piscina);
    probatio_http_versio(piscina);
    probatio_connection_header(piscina);
    probatio_parser_incrementalis(piscina);
    probatio_omnes_methodi(piscina);
    probatio_nullum_argumenta(piscina);

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
