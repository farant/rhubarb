/* uri_credo_drafts.c - sectiones credo pro inventis uri (omnes hodie fractae) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "uri.h"
#include <stdio.h>
#include <string.h>

interior chorda
_ch (constans character* s)
{
    chorda c;
    c.datum   = (i8*)(vacuum*)s;
    c.mensura = (i32)strlen(s);
    redde c;
}

interior chorda
_resolvere_textum (
    constans character* basis_textus,
    constans character* relativum,
               Piscina* piscina)
{
    Uri*   basis;
    Uri*   r;
    chorda vacua;

    vacua.datum   = NIHIL;
    vacua.mensura = 0;
    basis = uri_creare(basis_textus, piscina);
    si (basis == NIHIL)
    {
        redde vacua;
    }
    r = uri_resolvere(basis, _ch(relativum), piscina);
    si (r == NIHIL)
    {
        redde vacua;
    }
    redde uri_construere(r, piscina);
}

/* I: via relativa cum '://' interiore manet via */
interior vacuum
probare_via_cum_separatore (CredoContextus* c)
{
    Uri uri;

    CREDO_VERUM(uri_parse("/proxy/http://evil.com/x", &uri, c->piscina));
    CREDO_FALSUM(uri.est_absoluta);
    CREDO_AEQUALIS_I32(uri.schema.mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(uri.hospes.mensura, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(uri.via, "/proxy/http://evil.com/x");

    CREDO_VERUM(uri_parse("//h/x://y", &uri, c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(uri.hospes, "h");
    CREDO_CHORDA_AEQUALIS_LITERIS(uri.via, "/x://y");
}

/* II: schema vacuum vel illicitum */
interior vacuum
probare_schema_illicitum (CredoContextus* c)
{
    Uri uri;

    si (uri_parse("://h", &uri, c->piscina))
    {
        CREDO_FALSUM(uri.est_absoluta);
    }
    CREDO_FALSUM(uri_parse("1http://h", &uri, c->piscina) &&
                 uri.est_absoluta);
    CREDO_FALSUM(uri_parse("ht tp://h", &uri, c->piscina) &&
                 uri.est_absoluta);
}

/* III: schema sine '//' */
interior vacuum
probare_schema_sine_auctoritate (CredoContextus* c)
{
    Uri uri;

    CREDO_VERUM(uri_parse("mailto:a@b.com", &uri, c->piscina));
    CREDO_VERUM(uri.est_absoluta);
    CREDO_CHORDA_AEQUALIS_LITERIS(uri.schema, "mailto");
    CREDO_CHORDA_AEQUALIS_LITERIS(uri.via, "a@b.com");

    CREDO_VERUM(uri_parse("urn:isbn:0451450523", &uri, c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(uri.schema, "urn");
    CREDO_CHORDA_AEQUALIS_LITERIS(uri.via, "isbn:0451450523");
}

/* IV + V: portus extra ambitum vel malformatus */
interior vacuum
probare_portus_malus (CredoContextus* c)
{
    Uri uri;

    CREDO_FALSUM(uri_parse("http://h:4294967376/", &uri, c->piscina));
    CREDO_FALSUM(uri_parse("http://h:4294967296/", &uri, c->piscina));
    CREDO_FALSUM(uri_parse("http://h:65536/", &uri, c->piscina));
    CREDO_FALSUM(uri_parse("http://h:8a/", &uri, c->piscina));
    CREDO_FALSUM(uri_parse("http://h:-1/", &uri, c->piscina));
    CREDO_FALSUM(uri_parse("http://[::1]:8a/", &uri, c->piscina));
    /* custodia: licita manent */
    CREDO_VERUM(uri_parse("http://h:65535/", &uri, c->piscina));
    CREDO_AEQUALIS_I32(uri.portus, 65535);
}

/* VI: post ']' solum ':' vel finis */
interior vacuum
probare_ipv6_cauda (CredoContextus* c)
{
    Uri uri;

    CREDO_FALSUM(uri_parse("http://[::1]x/", &uri, c->piscina));
}

/* VII: hospes numquam '@' continet */
interior vacuum
probare_userinfo_duplex_at (CredoContextus* c)
{
    Uri uri;

    si (uri_parse("http://a@b@c/", &uri, c->piscina))
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(uri.hospes, "c");
        CREDO_CHORDA_AEQUALIS_LITERIS(uri.userinfo, "a@b");
    }
}

/* VIII: post FALSUM structura vacua */
interior vacuum
probare_status_post_errorem (CredoContextus* c)
{
    Uri uri;

    CREDO_FALSUM(uri_parse("http://[::1/", &uri, c->piscina));
    CREDO_FALSUM(uri.est_absoluta);
    CREDO_AEQUALIS_I32(uri.schema.mensura, ZEPHYRUM);
}

/* X: relativum cum auctoritate */
interior vacuum
probare_resolvere_auctoritas (CredoContextus* c)
{
    CREDO_CHORDA_AEQUALIS_LITERIS(
        _resolvere_textum("http://a/b/c/d;p?q", "//g", c->piscina),
        "http://g");
    CREDO_CHORDA_AEQUALIS_LITERIS(
        _resolvere_textum("http://a/b/c/d;p?q", "//other:81/x", c->piscina),
        "http://other:81/x");
    CREDO_CHORDA_AEQUALIS_LITERIS(
        _resolvere_textum("http://u@a:81/b/c", "//g/x", c->piscina),
        "http://g/x");
}

/* XI: via vacua - basis via et quaestio servantur */
interior vacuum
probare_resolvere_via_vacua (CredoContextus* c)
{
    CREDO_CHORDA_AEQUALIS_LITERIS(
        _resolvere_textum("http://a/b/c/d;p?q", "?y", c->piscina),
        "http://a/b/c/d;p?y");
    CREDO_CHORDA_AEQUALIS_LITERIS(
        _resolvere_textum("http://a/b/c/d;p?q", "#s", c->piscina),
        "http://a/b/c/d;p?q#s");
}

/* XII: relativum "" -> basis */
interior vacuum
probare_resolvere_vacuum (CredoContextus* c)
{
    Uri*   basis;
    Uri*   r;
    chorda vacuum_rel;

    basis = uri_creare("http://a/b/c/d;p?q", c->piscina);
    vacuum_rel.datum   = (i8*)(vacuum*)"";
    vacuum_rel.mensura = ZEPHYRUM;
    r = uri_resolvere(basis, vacuum_rel, c->piscina);
    CREDO_NON_NIHIL(r);
    si (r != NIHIL)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(uri_construere(r, c->piscina),
            "http://a/b/c/d;p?q");
    }
}

/* XIII: virgula terminalis servatur */
interior vacuum
probare_resolvere_virgula_terminalis (CredoContextus* c)
{
    constans character* b = "http://a/b/c/d;p?q";

    CREDO_CHORDA_AEQUALIS_LITERIS(_resolvere_textum(b, "g/", c->piscina),
        "http://a/b/c/g/");
    CREDO_CHORDA_AEQUALIS_LITERIS(_resolvere_textum(b, ".", c->piscina),
        "http://a/b/c/");
    CREDO_CHORDA_AEQUALIS_LITERIS(_resolvere_textum(b, "..", c->piscina),
        "http://a/b/");
    CREDO_CHORDA_AEQUALIS_LITERIS(_resolvere_textum(b, "../", c->piscina),
        "http://a/b/");
    CREDO_CHORDA_AEQUALIS_LITERIS(_resolvere_textum(b, "./g/.", c->piscina),
        "http://a/b/c/g/");
}

/* XIV: relativum cum schemate sine '//' */
interior vacuum
probare_resolvere_schema_opacum (CredoContextus* c)
{
    CREDO_CHORDA_AEQUALIS_LITERIS(
        _resolvere_textum("http://a/b/c/d;p?q", "g:h", c->piscina), "g:h");
}

/* XV: segmenta vacua servantur */
interior vacuum
probare_resolvere_segmenta_vacua (CredoContextus* c)
{
    CREDO_CHORDA_AEQUALIS_LITERIS(
        _resolvere_textum("http://a/b/c/d;p?q", "g//h", c->piscina),
        "http://a/b/c/g//h");
    CREDO_CHORDA_AEQUALIS_LITERIS(
        _resolvere_textum("http://a/b/c/d;p?q", "/a//b/", c->piscina),
        "http://a/a//b/");
}

/* XVII: NUL interior */
interior vacuum
probare_resolvere_nul_interior (CredoContextus* c)
{
    Uri*   basis;
    Uri*   r;
    chorda rel;

    basis = uri_creare("http://a/b/c/d;p?q", c->piscina);
    rel.datum   = (i8*)(vacuum*)"a\0b";
    rel.mensura = III;
    r = uri_resolvere(basis, rel, c->piscina);
    CREDO_NIHIL(r);
}

/* XVIII + XIX: uri_via_normalizare */
interior vacuum
probare_via_normalizare_rfc (CredoContextus* c)
{
    Uri uri;

    memset(&uri, 0, magnitudo(uri));
    uri.via = _ch("/a/b/");
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_via_normalizare(&uri, c->piscina),
        "/a/b/");
    uri.via = _ch("/a/.");
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_via_normalizare(&uri, c->piscina),
        "/a/");
    uri.via = _ch("/a//b");
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_via_normalizare(&uri, c->piscina),
        "/a//b");
}

/* XXI: referentia retis */
interior vacuum
probare_construere_referentia_retis (CredoContextus* c)
{
    Uri* u;

    u = uri_creare("//host/p", c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_construere(u, c->piscina), "//host/p");
    u = uri_creare("//host", c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_construere(u, c->piscina), "//host");
}

/* XXII + XXIII: schema opacum, via sine '/' */
interior vacuum
probare_construere_formae (CredoContextus* c)
{
    Uri u;

    memset(&u, 0, magnitudo(u));
    u.schema = _ch("mailto");
    u.via    = _ch("a@b.com");
    u.est_absoluta = VERUM;
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_construere(&u, c->piscina),
        "mailto:a@b.com");

    memset(&u, 0, magnitudo(u));
    u.schema = _ch("http");
    u.hospes = _ch("h");
    u.via    = _ch("a");
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_construere(&u, c->piscina),
        "http://h/a");
}

/* XXIV: delimitatores in campis */
interior vacuum
probare_construere_delimitatores (CredoContextus* c)
{
    Uri    u;
    Uri*   u2;
    chorda o;
    character buf[CCLVI];

    memset(&u, 0, magnitudo(u));
    u.schema   = _ch("http");
    u.userinfo = _ch("a@b/c");
    u.hospes   = _ch("h");
    u.via      = _ch("/a?b#c");
    u.quaestio = _ch("x#y");
    o = uri_construere(&u, c->piscina);
    memcpy(buf, o.datum, (size_t)o.mensura);
    buf[o.mensura] = '\0';
    u2 = uri_creare(buf, c->piscina);
    CREDO_NON_NIHIL(u2);
    si (u2 != NIHIL)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(u2->hospes, "h");
        CREDO_FALSUM(u2->fragmentum.mensura > 0);
    }
}

/* XXV: iter rotundum */
interior vacuum
probare_construere_iter_rotundum (CredoContextus* c)
{
    Uri*   u;
    Uri*   u2;
    chorda o;
    character buf[LXIV];

    u = uri_creare("http://h", c->piscina);
    o = uri_construere(u, c->piscina);
    memcpy(buf, o.datum, (size_t)o.mensura);
    buf[o.mensura] = '\0';
    u2 = uri_creare(buf, c->piscina);
    CREDO_VERUM(uri_aequalis(u, u2));
}

/* XXVI: userinfo comparatur */
interior vacuum
probare_aequalis_userinfo (CredoContextus* c)
{
    CREDO_FALSUM(uri_aequalis(uri_creare("http://alice@h/p", c->piscina),
                              uri_creare("http://bob@h/p", c->piscina)));
}

/* XXVIII: portus default */
interior vacuum
probare_portus_default_casus (CredoContextus* c)
{
    Uri uri;

    CREDO_VERUM(uri_parse("HTTPS://h/", &uri, c->piscina));
    CREDO_AEQUALIS_I32(uri_portus_default(uri.schema), 443);
    CREDO_AEQUALIS_I32(uri_portus_default(_ch("Http")), 80);
}

interior constans CredoSectio sectiones[] = {
    { "via_cum_separatore",        probare_via_cum_separatore,        NIHIL, NIHIL },
    { "schema_illicitum",          probare_schema_illicitum,          NIHIL, NIHIL },
    { "schema_sine_auctoritate",   probare_schema_sine_auctoritate,   NIHIL, NIHIL },
    { "portus_malus",              probare_portus_malus,              NIHIL, NIHIL },
    { "ipv6_cauda",                probare_ipv6_cauda,                NIHIL, NIHIL },
    { "userinfo_duplex_at",        probare_userinfo_duplex_at,        NIHIL, NIHIL },
    { "status_post_errorem",       probare_status_post_errorem,       NIHIL, NIHIL },
    { "resolvere_auctoritas",      probare_resolvere_auctoritas,      NIHIL, NIHIL },
    { "resolvere_via_vacua",       probare_resolvere_via_vacua,       NIHIL, NIHIL },
    { "resolvere_vacuum",          probare_resolvere_vacuum,          NIHIL, NIHIL },
    { "resolvere_virgula",         probare_resolvere_virgula_terminalis, NIHIL, NIHIL },
    { "resolvere_schema_opacum",   probare_resolvere_schema_opacum,   NIHIL, NIHIL },
    { "resolvere_segmenta_vacua",  probare_resolvere_segmenta_vacua,  NIHIL, NIHIL },
    { "resolvere_nul_interior",    probare_resolvere_nul_interior,    NIHIL, NIHIL },
    { "via_normalizare_rfc",       probare_via_normalizare_rfc,       NIHIL, NIHIL },
    { "construere_retis",          probare_construere_referentia_retis, NIHIL, NIHIL },
    { "construere_formae",         probare_construere_formae,         NIHIL, NIHIL },
    { "construere_delimitatores",  probare_construere_delimitatores,  NIHIL, NIHIL },
    { "construere_iter_rotundum",  probare_construere_iter_rotundum,  NIHIL, NIHIL },
    { "aequalis_userinfo",         probare_aequalis_userinfo,         NIHIL, NIHIL },
    { "portus_default_casus",      probare_portus_default_casus,      NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("uri_inventa", sectiones);
}
