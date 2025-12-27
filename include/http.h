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
    HTTP_HEAD
} HttpMethodus;

/* HTTP error codes */
nomen enumeratio {
    HTTP_OK = 0,
    HTTP_ERROR_URL,          /* URL invalida */
    HTTP_ERROR_CONNEXIO,     /* Connexio fallita */
    HTTP_ERROR_TIMEOUT,      /* Timeout */
    HTTP_ERROR_PARSE,        /* Parse error */
    HTTP_ERROR_IO            /* I/O error */
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


#endif /* HTTP_H */
