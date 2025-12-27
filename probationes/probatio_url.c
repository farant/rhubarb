/* probatio_url.c - Probationes pro URL bibliotheca
 *
 * Probationes encoding/decoding URL secundum RFC 3986.
 */

#include "url.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>


/* ========================================================================
 * PROBATIONES - CODIFICATIO SIMPLEX
 * ======================================================================== */

interior vacuum
probatio_codificatio_simplex(Piscina* piscina)
{
    chorda originalis;
    chorda codificata;

    printf("--- Probans codificatio simplex ---\n");

    /* Textus simplex sine characteres speciales */
    originalis = chorda_ex_literis("hello", piscina);
    codificata = url_codificare(originalis, piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, V);
    CREDO_VERUM(memcmp(codificata.datum, "hello", V) == 0);

    /* Unreserved characteres */
    originalis = chorda_ex_literis("test-_.~", piscina);
    codificata = url_codificare(originalis, piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, VIII);
    CREDO_VERUM(memcmp(codificata.datum, "test-_.~", VIII) == 0);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CODIFICATIO SPECIALES
 * ======================================================================== */

interior vacuum
probatio_codificatio_speciales(Piscina* piscina)
{
    chorda originalis;
    chorda codificata;

    printf("--- Probans codificatio speciales ---\n");

    /* Spatium -> %20 */
    originalis = chorda_ex_literis("hello world", piscina);
    codificata = url_codificare(originalis, piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, XIII);
    CREDO_VERUM(memcmp(codificata.datum, "hello%20world", XIII) == 0);

    /* Ampersand -> %26 */
    originalis = chorda_ex_literis("a&b", piscina);
    codificata = url_codificare(originalis, piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, V);
    CREDO_VERUM(memcmp(codificata.datum, "a%26b", V) == 0);

    /* Equals -> %3D */
    originalis = chorda_ex_literis("a=b", piscina);
    codificata = url_codificare(originalis, piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, V);
    CREDO_VERUM(memcmp(codificata.datum, "a%3Db", V) == 0);

    /* Question mark -> %3F */
    originalis = chorda_ex_literis("what?", piscina);
    codificata = url_codificare(originalis, piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, VII);
    CREDO_VERUM(memcmp(codificata.datum, "what%3F", VII) == 0);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - DECODIFICATIO
 * ======================================================================== */

interior vacuum
probatio_decodificatio(Piscina* piscina)
{
    chorda codificata;
    chorda decodificata;

    printf("--- Probans decodificatio ---\n");

    /* Simplex */
    codificata = chorda_ex_literis("hello", piscina);
    decodificata = url_decodificare(codificata, piscina);
    CREDO_AEQUALIS_I32(decodificata.mensura, V);
    CREDO_VERUM(memcmp(decodificata.datum, "hello", V) == 0);

    /* %20 -> spatium */
    codificata = chorda_ex_literis("hello%20world", piscina);
    decodificata = url_decodificare(codificata, piscina);
    CREDO_AEQUALIS_I32(decodificata.mensura, XI);
    CREDO_VERUM(memcmp(decodificata.datum, "hello world", XI) == 0);

    /* + -> spatium (form encoding) */
    codificata = chorda_ex_literis("hello+world", piscina);
    decodificata = url_decodificare(codificata, piscina);
    CREDO_AEQUALIS_I32(decodificata.mensura, XI);
    CREDO_VERUM(memcmp(decodificata.datum, "hello world", XI) == 0);

    /* %26 -> & */
    codificata = chorda_ex_literis("a%26b", piscina);
    decodificata = url_decodificare(codificata, piscina);
    CREDO_AEQUALIS_I32(decodificata.mensura, III);
    CREDO_VERUM(memcmp(decodificata.datum, "a&b", III) == 0);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - ROUND TRIP
 * ======================================================================== */

interior vacuum
probatio_round_trip(Piscina* piscina)
{
    chorda originalis;
    chorda codificata;
    chorda decodificata;

    printf("--- Probans round trip ---\n");

    originalis = chorda_ex_literis("Hello World! How are you?", piscina);
    codificata = url_codificare(originalis, piscina);
    decodificata = url_decodificare(codificata, piscina);

    CREDO_AEQUALIS_I32(decodificata.mensura, originalis.mensura);
    CREDO_VERUM(memcmp(decodificata.datum, originalis.datum,
                       (size_t)originalis.mensura) == 0);

    printf("  '%.*s' -> '%.*s' -> '%.*s'\n",
           originalis.mensura, originalis.datum,
           codificata.mensura, codificata.datum,
           decodificata.mensura, decodificata.datum);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PARAMS CONSTRUERE
 * ======================================================================== */

interior vacuum
probatio_params_construere(Piscina* piscina)
{
    chorda claves[III];
    chorda valores[III];
    chorda query;

    printf("--- Probans params construere ---\n");

    /* Una par */
    claves[0] = chorda_ex_literis("name", piscina);
    valores[0] = chorda_ex_literis("John", piscina);
    query = url_params_construere(claves, valores, I, piscina);
    CREDO_AEQUALIS_I32(query.mensura, IX);
    CREDO_VERUM(memcmp(query.datum, "name=John", IX) == 0);

    /* Duae pares */
    claves[I] = chorda_ex_literis("age", piscina);
    valores[I] = chorda_ex_literis("30", piscina);
    query = url_params_construere(claves, valores, II, piscina);
    CREDO_AEQUALIS_I32(query.mensura, XVI);
    CREDO_VERUM(memcmp(query.datum, "name=John&age=30", XVI) == 0);

    printf("  Query: '%.*s'\n", query.mensura, query.datum);

    /* Cum characteres speciales */
    claves[0] = chorda_ex_literis("q", piscina);
    valores[0] = chorda_ex_literis("hello world", piscina);
    query = url_params_construere(claves, valores, I, piscina);
    CREDO_VERUM(query.mensura > 0);
    printf("  Query cum spatio: '%.*s'\n", query.mensura, query.datum);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PARAMS PARSE
 * ======================================================================== */

interior vacuum
probatio_params_parse(Piscina* piscina)
{
    chorda    query;
    UrlParams params;

    printf("--- Probans params parse ---\n");

    /* Una par */
    query = chorda_ex_literis("name=John", piscina);
    params = url_params_parse(query, piscina);
    CREDO_AEQUALIS_I32(params.numerus, I);
    CREDO_AEQUALIS_I32(params.claves[0].mensura, IV);
    CREDO_VERUM(memcmp(params.claves[0].datum, "name", IV) == 0);
    CREDO_AEQUALIS_I32(params.valores[0].mensura, IV);
    CREDO_VERUM(memcmp(params.valores[0].datum, "John", IV) == 0);

    /* Duae pares */
    query = chorda_ex_literis("name=John&age=30", piscina);
    params = url_params_parse(query, piscina);
    CREDO_AEQUALIS_I32(params.numerus, II);

    /* Cum percent encoding */
    query = chorda_ex_literis("q=hello%20world", piscina);
    params = url_params_parse(query, piscina);
    CREDO_AEQUALIS_I32(params.numerus, I);
    CREDO_AEQUALIS_I32(params.valores[0].mensura, XI);
    CREDO_VERUM(memcmp(params.valores[0].datum, "hello world", XI) == 0);

    printf("  Parsed: q='%.*s'\n",
           params.valores[0].mensura, params.valores[0].datum);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - NULLUM ARGUMENTA
 * ======================================================================== */

interior vacuum
probatio_nullum_argumenta(Piscina* piscina)
{
    chorda    input;
    chorda    result;
    UrlParams params;

    printf("--- Probans nullum argumenta ---\n");

    /* Codificare cum NIHIL */
    input.datum = NIHIL;
    input.mensura = 0;
    result = url_codificare(input, piscina);
    CREDO_NIHIL(result.datum);

    /* Decodificare cum NIHIL */
    result = url_decodificare(input, piscina);
    CREDO_NIHIL(result.datum);

    /* Params construere cum NIHIL */
    result = url_params_construere(NIHIL, NIHIL, 0, piscina);
    CREDO_NIHIL(result.datum);

    /* Params parse cum NIHIL */
    params = url_params_parse(input, piscina);
    CREDO_AEQUALIS_I32(params.numerus, 0);

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
    printf("PROBATIONES URL\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_url", LXIV * M);
    credo_aperire(piscina);

    probatio_codificatio_simplex(piscina);
    probatio_codificatio_speciales(piscina);
    probatio_decodificatio(piscina);
    probatio_round_trip(piscina);
    probatio_params_construere(piscina);
    probatio_params_parse(piscina);
    probatio_nullum_argumenta(piscina);

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
