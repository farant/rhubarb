/* url.h - URL Percent Encoding/Decoding
 *
 * Bibliotheca pro URL encoding secundum RFC 3986.
 * Utilis pro query parameters, form data, path segments.
 *
 * USUS:
 *   chorda codificata = url_codificare(originalis, piscina);
 *   chorda decodificata = url_decodificare(codificata, piscina);
 *   chorda query = url_params_construere(claves, valores, numerus, piscina);
 */

#ifndef URL_H
#define URL_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* Fructus parsing query string */
nomen structura {
    chorda* claves;
    chorda* valores;
    i32     numerus;
} UrlParams;


/* ========================================================================
 * FUNCTIONES - CODIFICATIO
 * ======================================================================== */

/* Percent-encode chorda pro URLs
 *
 * Characteres unreserved (A-Z, a-z, 0-9, -, _, ., ~) non mutantur.
 * Alii characteres convertuntur ad %XX.
 *
 * originalis: Chorda ad codificandum
 * piscina:    Arena pro allocatio
 *
 * Redde: chorda percent-encoded
 */
chorda
url_codificare(
    chorda   originalis,
    Piscina* piscina);


/* ========================================================================
 * FUNCTIONES - DECODIFICATIO
 * ======================================================================== */

/* Decode percent-encoded chorda
 *
 * codificata: Chorda percent-encoded
 * piscina:    Arena pro allocatio
 *
 * Redde: chorda decodificata
 */
chorda
url_decodificare(
    chorda   codificata,
    Piscina* piscina);


/* ========================================================================
 * FUNCTIONES - QUERY STRING
 * ======================================================================== */

/* Construere query string ex key-value pairs
 *
 * Format: "key1=value1&key2=value2&..."
 * Claves et valores sunt percent-encoded.
 *
 * claves:   Array chordae clavium
 * valores:  Array chordae valorum
 * numerus:  Numerus par
 * piscina:  Arena pro allocatio
 *
 * Redde: Query string
 */
chorda
url_params_construere(
    chorda*  claves,
    chorda*  valores,
    i32      numerus,
    Piscina* piscina);

/* Parse query string ad key-value pairs
 *
 * query_string: Chorda "key1=value1&key2=value2"
 * piscina:      Arena pro allocatio
 *
 * Redde: UrlParams cum arrays clavium et valorum
 */
UrlParams
url_params_parse(
    chorda   query_string,
    Piscina* piscina);


#endif /* URL_H */
