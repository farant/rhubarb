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
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;
        Uri  uri;
        b32  successus;

    piscina = c->piscina;

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
}


/* ========================================================================
 * PROBATIONES - PARSING CUM PORTUS
 * ======================================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
        Uri  uri;
        b32  successus;

    piscina = c->piscina;

    /* Portus explicitus */
    successus = uri_parse("https://example.com:8080/path", &uri,
        piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.hospes, "example.com"));
    CREDO_VERUM(uri.portus == 8080);
    CREDO_VERUM(chorda_aequalis_literis(uri.via, "/path"));

    /* Portus 443 */
    successus = uri_parse("https://secure.example.com:443", &uri,
        piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(uri.portus == 443);
}


/* ========================================================================
 * PROBATIONES - PARSING CUM USERINFO
 * ======================================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
        Uri  uri;
        b32  successus;

    piscina = c->piscina;

    /* User simplex */
    successus = uri_parse("https://user@example.com/path", &uri,
        piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.userinfo, "user"));
    CREDO_VERUM(chorda_aequalis_literis(uri.hospes, "example.com"));

    /* User et password */
    successus = uri_parse("https://user:pass@example.com:8080/path",
        &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.userinfo, "user:pass"));
    CREDO_VERUM(chorda_aequalis_literis(uri.hospes, "example.com"));
    CREDO_VERUM(uri.portus == 8080);
}


/* ========================================================================
 * PROBATIONES - PARSING CUM QUAESTIO ET FRAGMENTUM
 * ======================================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
        Uri  uri;
        b32  successus;

    piscina = c->piscina;

    /* Quaestio */
    successus = uri_parse("https://example.com/search?q=test&lang=en",
        &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.via, "/search"));
    CREDO_VERUM(chorda_aequalis_literis(uri.quaestio,
        "q=test&lang=en"));

    /* Fragmentum */
    successus = uri_parse("https://example.com/page#section", &uri,
        piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.via, "/page"));
    CREDO_VERUM(chorda_aequalis_literis(uri.fragmentum, "section"));

    /* Quaestio et fragmentum */
    successus = uri_parse("https://example.com/page?key=val#anchor",
        &uri, piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.quaestio, "key=val"));
    CREDO_VERUM(chorda_aequalis_literis(uri.fragmentum, "anchor"));
}


/* ========================================================================
 * PROBATIONES - PARSING COMPLETA
 * ======================================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
        Uri  uri;
        b32  successus;

    piscina = c->piscina;

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
}


/* ========================================================================
 * PROBATIONES - PARSING VIA RELATIVUM
 * ======================================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
        Uri  uri;
        b32  successus;

    piscina = c->piscina;

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
}


/* ========================================================================
 * PROBATIONES - URI CREARE
 * ======================================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;
        Uri* uri;

    piscina = c->piscina;

    uri = uri_creare("https://example.com:443/path", piscina);
    CREDO_NON_NIHIL(uri);
    CREDO_VERUM(chorda_aequalis_literis(uri->schema, "https"));
    CREDO_VERUM(chorda_aequalis_literis(uri->hospes, "example.com"));
    CREDO_VERUM(uri->portus == 443);

    /* Cum NIHIL */
    uri = uri_creare(NIHIL, piscina);
    CREDO_NIHIL(uri);
}


/* ========================================================================
 * PROBATIONES - URI CONSTRUERE
 * ======================================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;
        Uri* uri;
     chorda  constructa;

    piscina = c->piscina;

    /* Simplex */
    uri = uri_creare("https://example.com/path", piscina);
    CREDO_NON_NIHIL(uri);
    constructa = uri_construere(uri, piscina);
    CREDO_VERUM(chorda_aequalis_literis(constructa,
        "https://example.com/path"));

    /* Cum portus */
    uri = uri_creare("https://example.com:8080/api", piscina);
    CREDO_NON_NIHIL(uri);
    constructa = uri_construere(uri, piscina);
    CREDO_VERUM(chorda_aequalis_literis(constructa,
        "https://example.com:8080/api"));

    /* Cum quaestio */
    uri = uri_creare("https://example.com/search?q=test", piscina);
    CREDO_NON_NIHIL(uri);
    constructa = uri_construere(uri, piscina);
    CREDO_VERUM(chorda_aequalis_literis(constructa,
        "https://example.com/search?q=test"));
}


/* ========================================================================
 * PROBATIONES - URI RESOLVERE
 * ======================================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;
        Uri* base;
        Uri* resultatus;
     chorda  relativum;

    piscina = c->piscina;

    base = uri_creare("https://example.com/foo/bar", piscina);
    CREDO_NON_NIHIL(base);

    /* Via absolutum */
    relativum   = chorda_ex_literis("/absolute/path", piscina);
    resultatus  = uri_resolvere(base, relativum, piscina);
    CREDO_NON_NIHIL(resultatus);
    CREDO_VERUM(chorda_aequalis_literis(resultatus->via,
        "/absolute/path"));
    CREDO_VERUM(chorda_aequalis_literis(resultatus->hospes,
        "example.com"));

    /* Via relativum */
    relativum   = chorda_ex_literis("sibling", piscina);
    resultatus  = uri_resolvere(base, relativum, piscina);
    CREDO_NON_NIHIL(resultatus);
    CREDO_VERUM(chorda_aequalis_literis(resultatus->via,
        "/foo/sibling"));

    /* Via cum .. */
    relativum   = chorda_ex_literis("../other", piscina);
    resultatus  = uri_resolvere(base, relativum, piscina);
    CREDO_NON_NIHIL(resultatus);
    CREDO_VERUM(chorda_aequalis_literis(resultatus->via, "/other"));

    /* Via retis (RFC 3986 5.2.2: "//hospes/via" - auctoritas ex
     * relativo, schema ex base; navigator N1: <img src="//thumb...">) */
    relativum   = chorda_ex_literis("//cdn.example.org/a/b.jpg?x=1",
        piscina);
    resultatus  = uri_resolvere(base, relativum, piscina);
    CREDO_NON_NIHIL(resultatus);
    CREDO_VERUM(chorda_aequalis_literis(resultatus->schema, "https"));
    CREDO_VERUM(chorda_aequalis_literis(resultatus->hospes,
        "cdn.example.org"));
    CREDO_VERUM(chorda_aequalis_literis(resultatus->via, "/a/b.jpg"));
    CREDO_VERUM(chorda_aequalis_literis(resultatus->quaestio, "x=1"));
}


/* RFC 3986 5.4.1 + 5.4.2 (navigator N2.3a: Location relativa
 * redirectionum per uri_resolvere solvitur). Basis
 * "http://a/b/c/d;p?q".
 * OMISSI consulto: "g:h" et "http:g" (schema sine "//" - Uri campum
 * auctoritatis non habet, uri_construere "://" semper scribit) et "//g"
 * (redditur "http://g/" - pagina eadem; uri_construere viam vacuam ut
 * "/" scribit). */
hic_manens constans character* tabula_rfc[][II] = {
    { "g",             "http://a/b/c/g" },
    { "./g",           "http://a/b/c/g" },
    { "g/",            "http://a/b/c/g/" },
    { "/g",            "http://a/g" },
    { "?y",            "http://a/b/c/d;p?y" },
    { "g?y",           "http://a/b/c/g?y" },
    { "#s",            "http://a/b/c/d;p?q#s" },
    { "g#s",           "http://a/b/c/g#s" },
    { "g?y#s",         "http://a/b/c/g?y#s" },
    { ";x",            "http://a/b/c/;x" },
    { "g;x",           "http://a/b/c/g;x" },
    { "g;x?y#s",       "http://a/b/c/g;x?y#s" },
    { "",              "http://a/b/c/d;p?q" },
    { ".",             "http://a/b/c/" },
    { "./",            "http://a/b/c/" },
    { "..",            "http://a/b/" },
    { "../",           "http://a/b/" },
    { "../g",          "http://a/b/g" },
    { "../..",         "http://a/" },
    { "../../",        "http://a/" },
    { "../../g",       "http://a/g" },
    { "../../../g",    "http://a/g" },
    { "../../../../g", "http://a/g" },
    { "/./g",          "http://a/g" },
    { "/../g",         "http://a/g" },
    { "g.",            "http://a/b/c/g." },
    { ".g",            "http://a/b/c/.g" },
    { "g..",           "http://a/b/c/g.." },
    { "..g",           "http://a/b/c/..g" },
    { "./../g",        "http://a/b/g" },
    { "./g/.",         "http://a/b/c/g/" },
    { "g/./h",         "http://a/b/c/g/h" },
    { "g/../h",        "http://a/b/c/h" },
    { "g;x=1/./y",     "http://a/b/c/g;x=1/y" },
    { "g;x=1/../y",    "http://a/b/c/y" },
    { "g?y/./x",       "http://a/b/c/g?y/./x" },
    { "g?y/../x",      "http://a/b/c/g?y/../x" },
    { "g#s/./x",       "http://a/b/c/g#s/./x" },
    { "g#s/../x",      "http://a/b/c/g#s/../x" },
    { NIHIL,           NIHIL }
};

interior vacuum
_sectio_rfc (
    CredoContextus* c)
{
       Uri* basis;
       Uri* r;
    chorda  s;
       i32  i;

    basis = uri_creare("http://a/b/c/d;p?q", c->piscina);
    CREDO_NECESSE_NON_NIHIL(basis);
    per (i = ZEPHYRUM; tabula_rfc[i][ZEPHYRUM]; i++)
    {
        r = uri_resolvere(basis,
            chorda_ex_literis(tabula_rfc[i][ZEPHYRUM],
            c->piscina), c->piscina);
        s = r ? uri_construere(r, c->piscina)
              : chorda_ex_literis("(NIHIL)", c->piscina);
        si (!chorda_aequalis_literis(s, tabula_rfc[i][I]))
        {
            imprimere("  RFC: \"%s\" -> %.*s (exspectatum %s)\n",
                tabula_rfc[i][ZEPHYRUM], (integer)s.mensura,
                (constans character*)s.datum, tabula_rfc[i][I]);
        }
        CREDO_CHORDA_AEQUALIS_LITERIS(s, tabula_rfc[i][I]);
    }
}

/* schema = praefixum (ALPHA *(ALPHA/DIGIT/+/-/.)) ante "://"; "://"
 * alibi (in via, quaestione) schema non facit */
interior vacuum
_sectio_schema_praefixum (
    CredoContextus* c)
{
    Uri u;

    CREDO_GRADUS("'://' in via relativa");
    CREDO_VERUM(uri_parse("/proxy/http://evil.com/x", &u, c->piscina));
    CREDO_FALSUM(u.est_absoluta);
    CREDO_AEQUALIS_I32((i32)u.hospes.mensura, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(u.via, "/proxy/http://evil.com/x");

    CREDO_GRADUS("'://' in via relativa sine solido initiali");
    CREDO_VERUM(uri_parse("a/b://c", &u, c->piscina));
    CREDO_FALSUM(u.est_absoluta);
    CREDO_CHORDA_AEQUALIS_LITERIS(u.via, "a/b://c");

    CREDO_GRADUS("'://' in quaestione");
    CREDO_VERUM(uri_parse("/r?u=http://evil.com", &u, c->piscina));
    CREDO_FALSUM(u.est_absoluta);
    CREDO_CHORDA_AEQUALIS_LITERIS(u.via, "/r");
    CREDO_CHORDA_AEQUALIS_LITERIS(u.quaestio, "u=http://evil.com");

    CREDO_GRADUS("schema verum manet");
    CREDO_VERUM(uri_parse("svn+ssh://h.example/x", &u, c->piscina));
    CREDO_VERUM(u.est_absoluta);
    CREDO_CHORDA_AEQUALIS_LITERIS(u.schema, "svn+ssh");
    CREDO_CHORDA_AEQUALIS_LITERIS(u.hospes, "h.example");
}


/* ========================================================================
 * PROBATIONES - PORTUS DEFAULT
 * ======================================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;

    CREDO_VERUM(uri_portus_default(chorda_ex_literis("https", piscina))
        == 443);
    CREDO_VERUM(uri_portus_default(chorda_ex_literis("http", piscina))
        == 80);
    CREDO_VERUM(uri_portus_default(chorda_ex_literis("ftp", piscina))
        == 21);
    CREDO_VERUM(uri_portus_default(chorda_ex_literis("ssh", piscina))
        == 22);
    CREDO_VERUM(uri_portus_default(chorda_ex_literis("unknown",
        piscina)) == 0);
}


/* ========================================================================
 * PROBATIONES - NULLUM ARGUMENTA
 * ======================================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
        Uri  uri;
        b32  successus;
        Uri* ptr;

    piscina = c->piscina;

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
}


/* ========================================================================
 * PROBATIONES - IPv6
 * ======================================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
        Uri  uri;
        b32  successus;

    piscina = c->piscina;

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
    successus = uri_parse("http://[2001:db8::1]:8080/path", &uri,
        piscina);
    CREDO_VERUM(successus);
    CREDO_VERUM(chorda_aequalis_literis(uri.hospes, "[2001:db8::1]"));
    CREDO_VERUM(uri.portus == 8080);
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

hic_manens constans CredoSectio SECTIONES[] = {
    { "parse simplex", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "parse cum portus", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "parse cum userinfo", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "parse cum quaestio et fragmentum", _sectio_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "parse completa", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "parse via relativum", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "uri_creare", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "uri_construere", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "uri_resolvere", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "uri_portus_default", _sectio_decima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "nullum argumenta", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "IPv6", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "RFC 3986 5.4", _sectio_rfc, NIHIL, NIHIL, NIHIL, NIHIL },
    { "schema praefixum", _sectio_schema_praefixum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("uri", SECTIONES);
}
