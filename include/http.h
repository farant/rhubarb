/* http.h - Bibliotheca HTTP Client
 *
 * HTTP/1.1 client pro petitionibus HTTPS.
 * Pure C89, utitur tls.h pro connexionibus securis.
 *
 * USUS:
 *   HttpPetitio* pet = http_petitio_creare(piscina, HTTP_GET, "https://api.example.com/data");
 *   http_petitio_caput_addere(pet, "Authorization", "Bearer token123");
 *   HttpResultus res = http_exsequi(pet, piscina);
 *   si (res.successus) {
 *       printf("Status: %d\n", res.responsum->status);
 *       printf("Corpus: %.*s\n", res.responsum->corpus.mensura, res.responsum->corpus.datum);
 *   }
 */

#ifndef HTTP_H
#define HTTP_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* ========================================================================
 * TYPI
 * ======================================================================== */

/* HTTP methodi */
nomen enumeratio {
    HTTP_GET = 0,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_PATCH,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_METHODUS_IGNOTUS
} HttpMethodus;

/* HTTP error codes */
nomen enumeratio {
    HTTP_OK = 0,
    HTTP_ERROR_URL,          /* URL invalida */
    HTTP_ERROR_CONNEXIO,     /* Connexio fallita */
    HTTP_ERROR_TIMEOUT,      /* Timeout */
    HTTP_ERROR_PARSE,        /* Parse error */
    HTTP_ERROR_IO,           /* I/O error */
    HTTP_ERROR_REDIRECTIO    /* Nimis redirectiones */
} HttpError;

/* Caput (header) */
nomen structura {
    chorda titulus;
    chorda valor;
} HttpCaput;

/* Petitio (request) - opaque */
nomen structura HttpPetitio HttpPetitio;

/* Responsum (response) */
nomen structura {
    i32    status;               /* 200, 404, etc. */
    chorda status_descriptio;    /* "OK", "Not Found", etc. */
    chorda corpus;               /* Response body */

    /* Capita (headers) - pro accessu interno */
    HttpCaput* capita;
    i32        capita_numerus;
} HttpResponsum;

/* Resultus */
nomen structura {
    b32           successus;
    HttpResponsum* responsum;
    HttpError     error;
    chorda        error_descriptio;
} HttpResultus;


/* ========================================================================
 * TYPI - SERVER (REQUEST PARSING)
 * ======================================================================== */

/* Status parsing */
nomen enumeratio {
    HTTP_PARSE_LINEA_PETITIONIS = 0,  /* Parsing request line */
    HTTP_PARSE_CAPITA,                 /* Parsing headers */
    HTTP_PARSE_CORPUS,                 /* Parsing body */
    HTTP_PARSE_COMPLETA,               /* Parsing complete */
    HTTP_PARSE_ERROR                   /* Parse error */
} HttpParseStatus;

/* Petitio serveri (parsed incoming request) */
nomen structura {
    HttpMethodus methodus;
    chorda       uri;              /* Full URI "/path?query" */
    chorda       via;              /* Path only "/path" */
    chorda       quaestio;         /* Query string (after ?) */
    i32          versio;           /* 10 = HTTP/1.0, 11 = HTTP/1.1 */

    HttpCaput*   capita;           /* Array of headers */
    i32          capita_numerus;

    chorda       corpus;           /* Request body */
    i32          corpus_longitudo; /* Body length */

    b32          keep_alive;       /* Connection: keep-alive */
    b32          chunked;          /* Transfer-Encoding: chunked */
    i32          content_length;   /* Content-Length header value */
} HttpPetitioServeri;

/* Parser incrementalis (opaque) */
nomen structura HttpParser HttpParser;

/* Resultus parse */
nomen structura {
    b32                 successus;
    b32                 completa;      /* FALSUM si necesse plus data */
    HttpPetitioServeri* petitio;
    HttpError           error;
    chorda              error_descriptio;
} HttpParseResultus;


/* ========================================================================
 * FUNCTIONES - PETITIO
 * ======================================================================== */

/* Creare novam petitio
 *
 * methodus: HTTP_GET, HTTP_POST, etc.
 * url:      URL completa (e.g., "https://api.example.com/path")
 *
 * Redde: HttpPetitio* vel NIHIL si error
 */
HttpPetitio*
http_petitio_creare(
    Piscina*            piscina,
    HttpMethodus        methodus,
    constans character* url);

/* Addere caput (header) ad petitio */
vacuum
http_petitio_caput_addere(
    HttpPetitio*        petitio,
    constans character* titulus,
    constans character* valor);

/* Ponere corpus (body) petitionis */
vacuum
http_petitio_corpus_ponere(
    HttpPetitio*        petitio,
    constans character* corpus,
    i32                 mensura);

/* Ponere corpus ex chorda */
vacuum
http_petitio_corpus_ponere_chorda(
    HttpPetitio* petitio,
    chorda       corpus);


/* ========================================================================
 * FUNCTIONES - EXSEQUI
 * ======================================================================== */

/* Exsequi petitio et recipere responsum
 *
 * petitio: Petitio praeparata
 * piscina: Arena pro responsum
 *
 * Redde: Resultus cum responsum vel error
 */
HttpResultus
http_exsequi(
    HttpPetitio* petitio,
    Piscina*     piscina);

/* Exsequi petitio cum sequendo redirectiones (301, 302, 307, 308)
 *
 * petitio:           Petitio praeparata
 * piscina:           Arena pro responsum
 * max_redirectiones: Maximum redirectiones sequendae (typice V vel X)
 *
 * Redde: Resultus cum responsum finale vel error
 */
HttpResultus
http_exsequi_cum_redirectionibus(
    HttpPetitio* petitio,
    Piscina*     piscina,
    i32          max_redirectiones);


/* ========================================================================
 * FUNCTIONES - RESPONSUM
 * ======================================================================== */

/* Obtinere caput (header) ex responsum
 *
 * Redde: Valor capitis vel chorda vacua si non invenitur
 */
chorda
http_responsum_caput(
    HttpResponsum*      responsum,
    constans character* titulus);


/* ========================================================================
 * FUNCTIONES - UTILITAS
 * ======================================================================== */

/* Obtinere nomen methodi */
constans character*
http_methodus_nomen(HttpMethodus methodus);

/* Obtinere error descriptio */
constans character*
http_error_descriptio(HttpError error);


/* ========================================================================
 * FUNCTIONES - SERVER (REQUEST PARSING)
 * ======================================================================== */

/* Parse petitio uno ictu (si habes totam petitionem)
 *
 * datum:     Data petitionis
 * longitudo: Longitudo datorum
 * piscina:   Arena pro allocationibus
 *
 * Redde: Resultus cum petitio vel error
 */
HttpParseResultus
http_petitio_parse(
    constans character* datum,
    i32                 longitudo,
    Piscina*            piscina);

/* Creare parser incrementalis
 *
 * piscina: Arena pro allocationibus
 *
 * Redde: Parser vel NIHIL si error
 */
HttpParser*
http_parser_creare(Piscina* piscina);

/* Adicere data ad parser
 *
 * parser:    Parser
 * datum:     Nova data
 * longitudo: Longitudo datorum
 *
 * Redde: Resultus (successus = parse valida, completa = petitio completa)
 */
HttpParseResultus
http_parser_adicere(
    HttpParser*         parser,
    constans character* datum,
    i32                 longitudo);

/* Verificare si parsing completa */
b32
http_parser_est_completa(HttpParser* parser);

/* Obtinere petitio parsata (post completa) */
HttpPetitioServeri*
http_parser_obtinere_petitio(HttpParser* parser);

/* Reset parser pro nova petitio */
vacuum
http_parser_reset(HttpParser* parser);

/* Convertere methodus string ad enum */
HttpMethodus
http_methodus_ex_literis(constans character* methodus);


#endif /* HTTP_H */
