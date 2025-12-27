/* uri.c - Bibliotheca URI Parsing (RFC 3986)
 *
 * Implementatio parsing et constructionis URIs.
 */

#include "uri.h"
#include "via.h"
#include "chorda.h"
#include "chorda_aedificator.h"

#include <string.h>


/* ========================================================================
 * FUNCTIONES INTERNAE
 * ======================================================================== */

/* Invenire characterem in chorda, redde index vel i32 max si non invenitur */
interior i32
_invenire_character(constans character* s, i32 longitudo, character c)
{
    i32 i;
    per (i = 0; i < longitudo; i++)
    {
        si (s[i] == c)
        {
            redde i;
        }
    }
    redde longitudo; /* Non invenitur */
}

/* Invenire sequentia ":/" in chorda, redde index vel longitudo si non invenitur */
interior i32
_invenire_schema_separator(constans character* s, i32 longitudo)
{
    i32 i;
    si (longitudo < 3)
    {
        redde longitudo;
    }
    per (i = 0; i < longitudo - 2; i++)
    {
        si (s[i] == ':' && s[i + 1] == '/' && s[i + 2] == '/')
        {
            redde i;
        }
    }
    redde longitudo;
}

/* Copiare substring in piscinam */
interior chorda
_copiare_substring(
    constans character* s,
    i32                 initium,
    i32                 longitudo,
    Piscina*            piscina)
{
    chorda resultatus;
    i8*    buffer;

    si (longitudo == 0)
    {
        resultatus.datum = NIHIL;
        resultatus.mensura = 0;
        redde resultatus;
    }

    buffer = piscina_allocare(piscina, longitudo);
    memcpy(buffer, s + initium, (memoriae_index)longitudo);

    resultatus.datum = buffer;
    resultatus.mensura = longitudo;
    redde resultatus;
}

/* Parse numerum ex chorda, redde 0 si error */
interior i32
_parse_numerum(constans character* s, i32 longitudo)
{
    i32 resultatus;
    i32 i;

    si (longitudo == 0)
    {
        redde 0;
    }

    resultatus = 0;
    per (i = 0; i < longitudo; i++)
    {
        si (s[i] >= '0' && s[i] <= '9')
        {
            resultatus = resultatus * X + (i32)(s[i] - '0');
        }
        alioquin
        {
            redde 0;
        }
    }
    redde resultatus;
}

/* Initializare Uri ad valores vacuos */
interior vacuum
_uri_initializare(Uri* uri)
{
    uri->schema.datum = NIHIL;
    uri->schema.mensura = 0;
    uri->userinfo.datum = NIHIL;
    uri->userinfo.mensura = 0;
    uri->hospes.datum = NIHIL;
    uri->hospes.mensura = 0;
    uri->portus = 0;
    uri->via.datum = NIHIL;
    uri->via.mensura = 0;
    uri->quaestio.datum = NIHIL;
    uri->quaestio.mensura = 0;
    uri->fragmentum.datum = NIHIL;
    uri->fragmentum.mensura = 0;
    uri->est_absoluta = FALSUM;
}


/* ========================================================================
 * FUNCTIONES - PARSING
 * ======================================================================== */

b32
uri_parse(
    constans character* url,
    Uri*                uri,
    Piscina*            piscina)
{
    constans character* cursor;
    i32                 longitudo;
    i32                 schema_end;
    i32                 authority_start;
    i32                 authority_end;
    i32                 path_start;
    i32                 query_start;
    i32                 fragment_start;
    i32                 at_pos;
    i32                 colon_pos;
    i32                 bracket_end;

    si (url == NIHIL || uri == NIHIL || piscina == NIHIL)
    {
        redde FALSUM;
    }

    _uri_initializare(uri);

    longitudo = (i32)strlen(url);
    si (longitudo == 0)
    {
        redde VERUM;
    }

    cursor = url;

    /* ----------------------------------------------------------------
     * Fragmentum (post #)
     * ---------------------------------------------------------------- */
    fragment_start = _invenire_character(cursor, longitudo, '#');
    si (fragment_start < longitudo)
    {
        uri->fragmentum = _copiare_substring(cursor, fragment_start + 1,
                                              longitudo - fragment_start - 1,
                                              piscina);
        longitudo = fragment_start;
    }

    /* ----------------------------------------------------------------
     * Quaestio (post ?)
     * ---------------------------------------------------------------- */
    query_start = _invenire_character(cursor, longitudo, '?');
    si (query_start < longitudo)
    {
        uri->quaestio = _copiare_substring(cursor, query_start + 1,
                                            longitudo - query_start - 1,
                                            piscina);
        longitudo = query_start;
    }

    /* ----------------------------------------------------------------
     * Schema (ante ://)
     * ---------------------------------------------------------------- */
    schema_end = _invenire_schema_separator(cursor, longitudo);
    si (schema_end < longitudo)
    {
        uri->schema = _copiare_substring(cursor, 0, schema_end, piscina);
        uri->est_absoluta = VERUM;
        authority_start = schema_end + 3; /* Post "://" */
    }
    alioquin si (longitudo >= 2 && cursor[0] == '/' && cursor[1] == '/')
    {
        /* Protocol-relative: //example.com/path */
        uri->est_absoluta = FALSUM;
        authority_start = 2;
    }
    alioquin
    {
        /* Via relativum vel absolutum */
        uri->via = _copiare_substring(cursor, 0, longitudo, piscina);
        uri->est_absoluta = FALSUM;
        redde VERUM;
    }

    /* ----------------------------------------------------------------
     * Authority (hospes, userinfo, portus)
     * ---------------------------------------------------------------- */
    /* Invenire finem authority (initium viae) */
    authority_end = _invenire_character(cursor + authority_start,
                                         longitudo - authority_start, '/');
    si (authority_end < longitudo - authority_start)
    {
        path_start = authority_start + authority_end;
        authority_end = path_start;
    }
    alioquin
    {
        authority_end = longitudo;
        path_start = longitudo;
    }

    /* Parse authority: [userinfo@]hospes[:portus] */
    {
        constans character* auth;
        i32                 auth_len;
        i32                 host_start;
        i32                 host_end;

        auth = cursor + authority_start;
        auth_len = authority_end - authority_start;

        /* Userinfo (ante @) */
        at_pos = _invenire_character(auth, auth_len, '@');
        si (at_pos < auth_len)
        {
            uri->userinfo = _copiare_substring(auth, 0, at_pos, piscina);
            host_start = at_pos + 1;
        }
        alioquin
        {
            host_start = 0;
        }

        /* Verificare IPv6 (in brackets) */
        si (host_start < auth_len && auth[host_start] == '[')
        {
            bracket_end = _invenire_character(auth + host_start,
                                               auth_len - host_start, ']');
            si (bracket_end < auth_len - host_start)
            {
                /* Hospes cum brackets */
                host_end = host_start + bracket_end + 1;
                uri->hospes = _copiare_substring(auth, host_start,
                                                  host_end - host_start, piscina);
                /* Portus post ] */
                si (host_end < auth_len && auth[host_end] == ':')
                {
                    uri->portus = _parse_numerum(auth + host_end + 1,
                                                  auth_len - host_end - 1);
                }
            }
            alioquin
            {
                /* Bracket non clausum - error */
                redde FALSUM;
            }
        }
        alioquin
        {
            /* Hospes normale - invenire : pro portus */
            colon_pos = _invenire_character(auth + host_start,
                                             auth_len - host_start, ':');
            si (colon_pos < auth_len - host_start)
            {
                host_end = host_start + colon_pos;
                uri->hospes = _copiare_substring(auth, host_start,
                                                  host_end - host_start, piscina);
                uri->portus = _parse_numerum(auth + host_end + 1,
                                              auth_len - host_end - 1);
            }
            alioquin
            {
                host_end = auth_len;
                uri->hospes = _copiare_substring(auth, host_start,
                                                  host_end - host_start, piscina);
            }
        }
    }

    /* ----------------------------------------------------------------
     * Via
     * ---------------------------------------------------------------- */
    si (path_start < longitudo)
    {
        uri->via = _copiare_substring(cursor, path_start,
                                       longitudo - path_start, piscina);
    }

    redde VERUM;
}

Uri*
uri_creare(
    constans character* url,
    Piscina*            piscina)
{
    Uri* uri;

    si (url == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }

    uri = piscina_allocare(piscina, (i32)magnitudo(Uri));
    si (!uri_parse(url, uri, piscina))
    {
        redde NIHIL;
    }

    redde uri;
}


/* ========================================================================
 * FUNCTIONES - PATH RESOLUTION
 * ======================================================================== */

chorda
uri_via_normalizare(
    Uri*     uri,
    Piscina* piscina)
{
    si (uri == NIHIL || piscina == NIHIL)
    {
        chorda vacua;
        vacua.datum = NIHIL;
        vacua.mensura = 0;
        redde vacua;
    }

    si (uri->via.datum == NIHIL || uri->via.mensura == 0)
    {
        chorda vacua;
        vacua.datum = NIHIL;
        vacua.mensura = 0;
        redde vacua;
    }

    redde via_normalizare(uri->via, piscina);
}

Uri*
uri_resolvere(
    Uri*     base,
    chorda   relativum,
    Piscina* piscina)
{
    Uri*   resultatus;
    Uri    rel_uri;
    chorda nova_via;

    si (base == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }

    si (relativum.datum == NIHIL || relativum.mensura == 0)
    {
        redde NIHIL;
    }

    resultatus = piscina_allocare(piscina, (i32)magnitudo(Uri));
    _uri_initializare(resultatus);

    /* Parse relativum */
    {
        i8* temp;
        temp = piscina_allocare(piscina, relativum.mensura + 1);
        memcpy(temp, relativum.datum, (memoriae_index)relativum.mensura);
        temp[relativum.mensura] = '\0';

        si (!uri_parse((constans character*)temp, &rel_uri, piscina))
        {
            redde NIHIL;
        }
    }

    /* Si relativum est absolutum, redde illud */
    si (rel_uri.est_absoluta)
    {
        *resultatus = rel_uri;
        resultatus->via = via_normalizare(rel_uri.via, piscina);
        redde resultatus;
    }

    /* Copiare schema et hospes ex base */
    resultatus->schema = base->schema;
    resultatus->hospes = base->hospes;
    resultatus->portus = base->portus;
    resultatus->userinfo = base->userinfo;
    resultatus->est_absoluta = VERUM;

    /* Resolvere via */
    si (rel_uri.via.mensura > 0 && rel_uri.via.datum[0] == '/')
    {
        /* Via absolutum */
        resultatus->via = via_normalizare(rel_uri.via, piscina);
    }
    alioquin
    {
        /* Via relativum - combinare cum base */
        ChordaAedificator* aed;
        i32                last_slash;
        i32                i;

        aed = chorda_aedificator_creare(piscina, CXXVIII);

        /* Invenire ultimum '/' in base via */
        last_slash = 0;
        per (i = 0; i < base->via.mensura; i++)
        {
            si (base->via.datum[i] == '/')
            {
                last_slash = i + 1;
            }
        }

        si (last_slash > 0)
        {
            /* Addere directory ex base */
            per (i = 0; i < last_slash; i++)
            {
                chorda_aedificator_appendere_character(aed,
                    (character)base->via.datum[i]);
            }
        }
        alioquin
        {
            chorda_aedificator_appendere_character(aed, '/');
        }

        /* Addere relativum */
        chorda_aedificator_appendere_chorda(aed, rel_uri.via);

        nova_via = chorda_aedificator_finire(aed);
        resultatus->via = via_normalizare(nova_via, piscina);
    }

    /* Copiare quaestio et fragmentum ex relativum */
    resultatus->quaestio = rel_uri.quaestio;
    resultatus->fragmentum = rel_uri.fragmentum;

    redde resultatus;
}


/* ========================================================================
 * FUNCTIONES - BUILDING
 * ======================================================================== */

chorda
uri_construere(
    Uri*     uri,
    Piscina* piscina)
{
    ChordaAedificator* aed;
    chorda             resultatus;

    si (uri == NIHIL || piscina == NIHIL)
    {
        resultatus.datum = NIHIL;
        resultatus.mensura = 0;
        redde resultatus;
    }

    aed = chorda_aedificator_creare(piscina, CCLVI);

    /* Schema */
    si (uri->schema.mensura > 0)
    {
        chorda_aedificator_appendere_chorda(aed, uri->schema);
        chorda_aedificator_appendere_literis(aed, "://");
    }

    /* Userinfo */
    si (uri->userinfo.mensura > 0)
    {
        chorda_aedificator_appendere_chorda(aed, uri->userinfo);
        chorda_aedificator_appendere_character(aed, '@');
    }

    /* Hospes */
    si (uri->hospes.mensura > 0)
    {
        chorda_aedificator_appendere_chorda(aed, uri->hospes);
    }

    /* Portus */
    si (uri->portus > 0)
    {
        i32 portus;
        i32 divisor;
        b32 started;

        chorda_aedificator_appendere_character(aed, ':');

        portus = uri->portus;

        /* Invenire divisor maximum */
        divisor = 1;
        dum (portus / divisor >= X)
        {
            divisor *= X;
        }

        started = FALSUM;
        dum (divisor > 0)
        {
            i32 digit;
            digit = portus / divisor;
            si (digit > 0 || started || divisor == 1)
            {
                chorda_aedificator_appendere_character(aed,
                    (character)('0' + (character)digit));
                started = VERUM;
            }
            portus = portus % divisor;
            divisor /= X;
        }
    }

    /* Via */
    si (uri->via.mensura > 0)
    {
        chorda_aedificator_appendere_chorda(aed, uri->via);
    }
    alioquin si (uri->hospes.mensura > 0)
    {
        /* Si habemus hospes sed non via, addere / */
        chorda_aedificator_appendere_character(aed, '/');
    }

    /* Quaestio */
    si (uri->quaestio.mensura > 0)
    {
        chorda_aedificator_appendere_character(aed, '?');
        chorda_aedificator_appendere_chorda(aed, uri->quaestio);
    }

    /* Fragmentum */
    si (uri->fragmentum.mensura > 0)
    {
        chorda_aedificator_appendere_character(aed, '#');
        chorda_aedificator_appendere_chorda(aed, uri->fragmentum);
    }

    redde chorda_aedificator_finire(aed);
}


/* ========================================================================
 * FUNCTIONES - UTILITAS
 * ======================================================================== */

i32
uri_portus_default(chorda schema)
{
    si (schema.datum == NIHIL || schema.mensura == 0)
    {
        redde 0;
    }

    si (schema.mensura == 5)
    {
        si (memcmp(schema.datum, "https", 5) == 0)
        {
            redde 443;
        }
    }

    si (schema.mensura == 4)
    {
        si (memcmp(schema.datum, "http", 4) == 0)
        {
            redde 80;
        }
    }

    si (schema.mensura == 3)
    {
        si (memcmp(schema.datum, "ftp", 3) == 0)
        {
            redde 21;
        }
        si (memcmp(schema.datum, "ssh", 3) == 0)
        {
            redde 22;
        }
        si (memcmp(schema.datum, "wss", 3) == 0)
        {
            redde 443;
        }
    }

    si (schema.mensura == 2)
    {
        si (memcmp(schema.datum, "ws", 2) == 0)
        {
            redde 80;
        }
    }

    redde 0;
}

b32
uri_est_absoluta(Uri* uri)
{
    si (uri == NIHIL)
    {
        redde FALSUM;
    }

    redde uri->est_absoluta;
}

b32
uri_aequalis(
    Uri* a,
    Uri* b)
{
    si (a == NIHIL || b == NIHIL)
    {
        redde a == b;
    }

    /* Comparare schema */
    si (!chorda_aequalis(a->schema, b->schema))
    {
        redde FALSUM;
    }

    /* Comparare hospes */
    si (!chorda_aequalis(a->hospes, b->hospes))
    {
        redde FALSUM;
    }

    /* Comparare portus */
    si (a->portus != b->portus)
    {
        redde FALSUM;
    }

    /* Comparare via */
    si (!chorda_aequalis(a->via, b->via))
    {
        redde FALSUM;
    }

    /* Comparare quaestio */
    si (!chorda_aequalis(a->quaestio, b->quaestio))
    {
        redde FALSUM;
    }

    /* Comparare fragmentum */
    si (!chorda_aequalis(a->fragmentum, b->fragmentum))
    {
        redde FALSUM;
    }

    redde VERUM;
}
