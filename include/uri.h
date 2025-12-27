/* uri.h - Bibliotheca URI Parsing (RFC 3986)
 *
 * Parsing et constructio URIs secundum RFC 3986.
 * Utitur url.h pro codificatione et via.h pro normalizatione.
 *
 * USUS:
 *   Uri uri;
 *   si (uri_parse("https://user@example.com:8080/via?q=1#frag", &uri, piscina)) {
 *       printf("Schema: %.*s\n", uri.schema.mensura, uri.schema.datum);
 *       printf("Hospes: %.*s\n", uri.hospes.mensura, uri.hospes.datum);
 *       printf("Portus: %d\n", uri.portus);
 *   }
 */

#ifndef URI_H
#define URI_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* ========================================================================
 * CONSTANTES
 * ======================================================================== */

#define URI_PORTUS_NULLUS 0


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* URI structura secundum RFC 3986
 *
 * Format: schema://[userinfo@]hospes[:portus]/via[?quaestio][#fragmentum]
 *
 * Exempla:
 *   https://user:pass@example.com:8080/path?query=1#section
 *   http://localhost/api/v1
 *   /relative/path
 *   ../parent/file
 */
nomen structura {
    chorda schema;       /* "https", "http", "ftp", etc. */
    chorda userinfo;     /* "user:pass" (ante @, optionalis) */
    chorda hospes;       /* "example.com" vel "[::1]" pro IPv6 */
    i32    portus;       /* Numerus portus vel URI_PORTUS_NULLUS */
    chorda via;          /* "/path/to/resource" */
    chorda quaestio;     /* "key=value&foo=bar" (post ?) */
    chorda fragmentum;   /* "section" (post #) */
    b32    est_absoluta; /* VERUM si habet schema */
} Uri;


/* ========================================================================
 * FUNCTIONES - PARSING
 * ======================================================================== */

/* Parse URI chorda in structuram
 *
 * url:     Chorda URI
 * uri:     Pointer ad Uri structuram (replendum)
 * piscina: Arena pro allocationibus
 *
 * Redde: VERUM si parsing successit, FALSUM si error
 */
b32
uri_parse(
    constans character* url,
    Uri*                uri,
    Piscina*            piscina);

/* Parse et creare Uri in piscina
 *
 * Commodior versio quae allocat Uri in piscina.
 *
 * Redde: Pointer ad Uri vel NIHIL si error
 */
Uri*
uri_creare(
    constans character* url,
    Piscina*            piscina);


/* ========================================================================
 * FUNCTIONES - PATH RESOLUTION
 * ======================================================================== */

/* Normalizare viam URI (resolvere . et ..)
 *
 * Utitur via_normalizare() interne.
 *
 * Redde: Chorda cum via normalizata
 */
chorda
uri_via_normalizare(
    Uri*     uri,
    Piscina* piscina);

/* Resolvere URI relativum contra base
 *
 * base:      Uri absolutum (cum schema et hospes)
 * relativum: Chorda URI relativum (e.g., "../other", "/abs", "file")
 * piscina:   Arena pro allocationibus
 *
 * Redde: Novum Uri absolutum vel NIHIL si error
 */
Uri*
uri_resolvere(
    Uri*     base,
    chorda   relativum,
    Piscina* piscina);


/* ========================================================================
 * FUNCTIONES - BUILDING
 * ======================================================================== */

/* Construere URI chorda ex structura
 *
 * uri:     Uri structura
 * piscina: Arena pro allocationibus
 *
 * Redde: Chorda cum URI completa
 */
chorda
uri_construere(
    Uri*     uri,
    Piscina* piscina);


/* ========================================================================
 * FUNCTIONES - UTILITAS
 * ======================================================================== */

/* Obtinere portus default pro schema
 *
 * "https" -> 443
 * "http"  -> 80
 * "ftp"   -> 21
 * aliud   -> URI_PORTUS_NULLUS
 */
i32
uri_portus_default(chorda schema);

/* Verificare si URI est absoluta (habet schema) */
b32
uri_est_absoluta(Uri* uri);

/* Comparare duo URIs
 *
 * Redde: VERUM si aequales
 */
b32
uri_aequalis(
    Uri* a,
    Uri* b);


#endif /* URI_H */
