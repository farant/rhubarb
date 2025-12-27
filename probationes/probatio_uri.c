/* probatio_uri.c - Probationes pro URI bibliotheca
 *
 * Probationes parsing et constructionis URIs (RFC 3986).
 */

#include "uri.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>


/* ========================================================================
 * PROBATIONES - PARSING SIMPLEX
 * ======================================================================== */

interior vacuum
probatio_parse_simplex(Piscina* piscina)
{
    Uri uri;
    b32 successus;

    printf("--- Probans parse simplex ---\n");

    /* URL simplex */
    successus = uri_parse("https://example.com/path", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(uri.est_absoluta);
    CREDO_VERUM(chorda_aequalis_literis(uri.schema, "https"));
    CREDO_VERUM(chorda_aequalis_literis(uri.hospes, "example.com"));
    CREDO_VERUM(chorda_aequalis_literis(uri.via, "/path"));
    CREDO_VERUM(uri.portus == 0);

    /* HTTP */
    successus = uri_parse("http://localhost/api/v1", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.schema, "http"));
    CREDO_VERUM(chorda_aequalis_literis(uri.hospes, "localhost"));
    CREDO_VERUM(chorda_aequalis_literis(uri.via, "/api/v1"));

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PARSING CUM PORTUS
 * ======================================================================== */

interior vacuum
probatio_parse_portus(Piscina* piscina)
{
    Uri uri;
    b32 successus;

    printf("--- Probans parse cum portus ---\n");

    /* Portus explicitus */
    successus = uri_parse("https://example.com:8080/path", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.hospes, "example.com"));
    CREDO_VERUM(uri.portus == 8080);
    CREDO_VERUM(chorda_aequalis_literis(uri.via, "/path"));

    /* Portus 443 */
    successus = uri_parse("https://secure.example.com:443", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(uri.portus == 443);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PARSING CUM USERINFO
 * ======================================================================== */

interior vacuum
probatio_parse_userinfo(Piscina* piscina)
{
    Uri uri;
    b32 successus;

    printf("--- Probans parse cum userinfo ---\n");

    /* User simplex */
    successus = uri_parse("https://user@example.com/path", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.userinfo, "user"));
    CREDO_VERUM(chorda_aequalis_literis(uri.hospes, "example.com"));

    /* User et password */
    successus = uri_parse("https://user:pass@example.com:8080/path", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.userinfo, "user:pass"));
    CREDO_VERUM(chorda_aequalis_literis(uri.hospes, "example.com"));
    CREDO_VERUM(uri.portus == 8080);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PARSING CUM QUAESTIO ET FRAGMENTUM
 * ======================================================================== */

interior vacuum
probatio_parse_quaestio_fragmentum(Piscina* piscina)
{
    Uri uri;
    b32 successus;

    printf("--- Probans parse cum quaestio et fragmentum ---\n");

    /* Quaestio */
    successus = uri_parse("https://example.com/search?q=test&lang=en", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.via, "/search"));
    CREDO_VERUM(chorda_aequalis_literis(uri.quaestio, "q=test&lang=en"));

    /* Fragmentum */
    successus = uri_parse("https://example.com/page#section", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.via, "/page"));
    CREDO_VERUM(chorda_aequalis_literis(uri.fragmentum, "section"));

    /* Quaestio et fragmentum */
    successus = uri_parse("https://example.com/page?key=val#anchor", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.quaestio, "key=val"));
    CREDO_VERUM(chorda_aequalis_literis(uri.fragmentum, "anchor"));

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PARSING COMPLETA
 * ======================================================================== */

interior vacuum
probatio_parse_completa(Piscina* piscina)
{
    Uri uri;
    b32 successus;

    printf("--- Probans parse completa ---\n");

    /* Omnia componentia */
    successus = uri_parse(
        "https://user:pass@example.com:8080/path/to/resource?query=1#section",
        &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(uri.est_absoluta);
    CREDO_VERUM(chorda_aequalis_literis(uri.schema, "https"));
    CREDO_VERUM(chorda_aequalis_literis(uri.userinfo, "user:pass"));
    CREDO_VERUM(chorda_aequalis_literis(uri.hospes, "example.com"));
    CREDO_VERUM(uri.portus == 8080);
    CREDO_VERUM(chorda_aequalis_literis(uri.via, "/path/to/resource"));
    CREDO_VERUM(chorda_aequalis_literis(uri.quaestio, "query=1"));
    CREDO_VERUM(chorda_aequalis_literis(uri.fragmentum, "section"));

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PARSING VIA RELATIVUM
 * ======================================================================== */

interior vacuum
probatio_parse_relativum(Piscina* piscina)
{
    Uri uri;
    b32 successus;

    printf("--- Probans parse via relativum ---\n");

    /* Via absolutum (incipit cum /) */
    successus = uri_parse("/api/v1/users", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_FALSUM(uri.est_absoluta);
    CREDO_VERUM(chorda_aequalis_literis(uri.via, "/api/v1/users"));
    CREDO_VERUM(uri.schema.mensura == 0);
    CREDO_VERUM(uri.hospes.mensura == 0);

    /* Via relativum */
    successus = uri_parse("../parent/file.txt", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_FALSUM(uri.est_absoluta);
    CREDO_VERUM(chorda_aequalis_literis(uri.via, "../parent/file.txt"));

    /* Via relativum simplex */
    successus = uri_parse("file.txt", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.via, "file.txt"));

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - URI CREARE
 * ======================================================================== */

interior vacuum
probatio_uri_creare(Piscina* piscina)
{
    Uri* uri;

    printf("--- Probans uri_creare ---\n");

    uri = uri_creare("https://example.com:443/path", piscina);
    CREDO_NON_NIHIL(uri);
    CREDO_VERUM(chorda_aequalis_literis(uri->schema, "https"));
    CREDO_VERUM(chorda_aequalis_literis(uri->hospes, "example.com"));
    CREDO_VERUM(uri->portus == 443);

    /* Cum NIHIL */
    uri = uri_creare(NIHIL, piscina);
    CREDO_NIHIL(uri);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - URI CONSTRUERE
 * ======================================================================== */

interior vacuum
probatio_construere(Piscina* piscina)
{
    Uri*   uri;
    chorda constructa;

    printf("--- Probans uri_construere ---\n");

    /* Simplex */
    uri = uri_creare("https://example.com/path", piscina);
    CREDO_NON_NIHIL(uri);
    constructa = uri_construere(uri, piscina);
    CREDO_VERUM(chorda_aequalis_literis(constructa, "https://example.com/path"));

    /* Cum portus */
    uri = uri_creare("https://example.com:8080/api", piscina);
    CREDO_NON_NIHIL(uri);
    constructa = uri_construere(uri, piscina);
    CREDO_VERUM(chorda_aequalis_literis(constructa, "https://example.com:8080/api"));

    /* Cum quaestio */
    uri = uri_creare("https://example.com/search?q=test", piscina);
    CREDO_NON_NIHIL(uri);
    constructa = uri_construere(uri, piscina);
    CREDO_VERUM(chorda_aequalis_literis(constructa, "https://example.com/search?q=test"));

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - URI RESOLVERE
 * ======================================================================== */

interior vacuum
probatio_resolvere(Piscina* piscina)
{
    Uri*   base;
    Uri*   resultatus;
    chorda relativum;

    printf("--- Probans uri_resolvere ---\n");

    base = uri_creare("https://example.com/foo/bar", piscina);
    CREDO_NON_NIHIL(base);

    /* Via absolutum */
    relativum = chorda_ex_literis("/absolute/path", piscina);
    resultatus = uri_resolvere(base, relativum, piscina);
    CREDO_NON_NIHIL(resultatus);
    CREDO_VERUM(chorda_aequalis_literis(resultatus->via, "/absolute/path"));
    CREDO_VERUM(chorda_aequalis_literis(resultatus->hospes, "example.com"));

    /* Via relativum */
    relativum = chorda_ex_literis("sibling", piscina);
    resultatus = uri_resolvere(base, relativum, piscina);
    CREDO_NON_NIHIL(resultatus);
    CREDO_VERUM(chorda_aequalis_literis(resultatus->via, "/foo/sibling"));

    /* Via cum .. */
    relativum = chorda_ex_literis("../other", piscina);
    resultatus = uri_resolvere(base, relativum, piscina);
    CREDO_NON_NIHIL(resultatus);
    CREDO_VERUM(chorda_aequalis_literis(resultatus->via, "/other"));

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PORTUS DEFAULT
 * ======================================================================== */

interior vacuum
probatio_portus_default(Piscina* piscina)
{
    printf("--- Probans uri_portus_default ---\n");

    CREDO_VERUM(uri_portus_default(chorda_ex_literis("https", piscina)) == 443);
    CREDO_VERUM(uri_portus_default(chorda_ex_literis("http", piscina)) == 80);
    CREDO_VERUM(uri_portus_default(chorda_ex_literis("ftp", piscina)) == 21);
    CREDO_VERUM(uri_portus_default(chorda_ex_literis("ssh", piscina)) == 22);
    CREDO_VERUM(uri_portus_default(chorda_ex_literis("unknown", piscina)) == 0);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - NULLUM ARGUMENTA
 * ======================================================================== */

interior vacuum
probatio_nullum_argumenta(Piscina* piscina)
{
    Uri  uri;
    b32  successus;
    Uri* ptr;

    printf("--- Probans nullum argumenta ---\n");

    /* Parse cum NIHIL */
    successus = uri_parse(NIHIL, &uri, piscina);
    CREDO_FALSUM(successus);

    successus = uri_parse("https://test.com", NIHIL, piscina);
    CREDO_FALSUM(successus);

    successus = uri_parse("https://test.com", &uri, NIHIL);
    CREDO_FALSUM(successus);

    /* Creare cum NIHIL */
    ptr = uri_creare(NIHIL, piscina);
    CREDO_NIHIL(ptr);

    ptr = uri_creare("https://test.com", NIHIL);
    CREDO_NIHIL(ptr);

    /* URL vacua */
    successus = uri_parse("", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(uri.schema.mensura == 0);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - IPv6
 * ======================================================================== */

interior vacuum
probatio_ipv6(Piscina* piscina)
{
    Uri uri;
    b32 successus;

    printf("--- Probans IPv6 ---\n");

    /* IPv6 simplex */
    successus = uri_parse("http://[::1]/path", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.hospes, "[::1]"));
    CREDO_VERUM(chorda_aequalis_literis(uri.via, "/path"));

    /* IPv6 cum portus */
    successus = uri_parse("http://[::1]:8080/path", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.hospes, "[::1]"));
    CREDO_VERUM(uri.portus == 8080);

    /* IPv6 completa */
    successus = uri_parse("http://[2001:db8::1]:8080/path", &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.hospes, "[2001:db8::1]"));
    CREDO_VERUM(uri.portus == 8080);

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
    printf("PROBATIONES URI\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_uri", CXXVIII * M);
    credo_aperire(piscina);

    probatio_parse_simplex(piscina);
    probatio_parse_portus(piscina);
    probatio_parse_userinfo(piscina);
    probatio_parse_quaestio_fragmentum(piscina);
    probatio_parse_completa(piscina);
    probatio_parse_relativum(piscina);
    probatio_uri_creare(piscina);
    probatio_construere(piscina);
    probatio_resolvere(piscina);
    probatio_portus_default(piscina);
    probatio_nullum_argumenta(piscina);
    probatio_ipv6(piscina);

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
