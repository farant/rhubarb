/* probatio_crustula.c - crustula RFC 6265 (hospitium v3 T4): caput
 * Cookie legere (paria multa, spatia, valor citatus, titulus absens),
 * Set-Cookie scribere (ordo attributorum, recusationes signorum,
 * SameSite=None sine Secure, praefixa __Secure-/__Host-, Max-Age 0). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "crustula.h"
#include <string.h>

interior b32
_aequalis (
                chorda  a,
    constans character* b)
{
    redde a.datum != NIHIL && a.mensura == (i32)strlen(b)
        && memcmp(a.datum, b, (size_t)a.mensura) == ZEPHYRUM;
}

interior chorda
_c (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

interior CrustulumOptiones
_optiones (vacuum)
{
    CrustulumOptiones o;

    memset(&o, ZEPHYRUM, magnitudo(o));
    o.max_aetas_s = -I;
    redde o;
}

interior vacuum
_sectio_legere (CredoContextus* c)
{
    Piscina* p = c->piscina;
     chorda  caput = _c("sid=abc123; theme=\"dark\";lang=la ;  x=1",
                 p);

    CREDO_VERUM(_aequalis(crustulum_valor(caput, "sid"), "abc123"));
    CREDO_VERUM(_aequalis(crustulum_valor(caput, "theme"), "dark"));
    CREDO_VERUM(_aequalis(crustulum_valor(caput, "lang"), "la"));
    CREDO_VERUM(_aequalis(crustulum_valor(caput, "x"), "1"));
    CREDO_NIHIL(crustulum_valor(caput, "absens").datum);
    CREDO_NIHIL(crustulum_valor(caput, "si").datum);   /* praefixum */
    CREDO_NIHIL(crustulum_valor(caput, "SID").datum);  /* casus */

    /* titulus absens, par sine '=', valor vacuus, primum vincit */
    caput = _c("=orphanum; solum; vacuum=; a=1; a=2", p);
    CREDO_NIHIL(crustulum_valor(caput, "").datum);
    CREDO_NIHIL(crustulum_valor(caput, "solum").datum);
    CREDO_VERUM(crustulum_valor(caput, "vacuum").datum != NIHIL);
    CREDO_AEQUALIS_I32(crustulum_valor(caput, "vacuum").mensura,
        ZEPHYRUM);
    CREDO_VERUM(_aequalis(crustulum_valor(caput, "a"), "1"));

    CREDO_NIHIL(crustulum_valor(_c("", p), "a").datum);
}

interior vacuum
_sectio_scribere (CredoContextus* c)
{
              Piscina* p = c->piscina;
    CrustulumOptiones  o = _optiones();

    CREDO_VERUM(_aequalis(crustulum_scribere("sid", _c("abc", p), NIHIL,
        p), "sid=abc; Path=/; SameSite=Lax"));

    o.max_aetas_s  = MMMDC;
    o.via          = "/app";
    o.dominium     = "exemplar.golden.earth";
    o.http_solum   = VERUM;
    o.securum      = VERUM;
    o.eadem        = CRUSTULUM_EADEM_STRICTA;
    CREDO_VERUM(_aequalis(crustulum_scribere("sid", _c("abc", p), &o,
        p),
        "sid=abc; Max-Age=3600; Path=/app; "
        "Domain=exemplar.golden.earth; "
        "Secure; HttpOnly; SameSite=Strict"));

    /* deletio: Max-Age=0, valor vacuus licet */
    o              = _optiones();
    o.max_aetas_s  = ZEPHYRUM;
    CREDO_VERUM(_aequalis(crustulum_scribere("sid", _c("", p), &o, p),
        "sid=; Max-Age=0; Path=/; SameSite=Lax"));

    /* SameSite=None cum Secure licet */
    o          = _optiones();
    o.eadem    = CRUSTULUM_EADEM_NULLA;
    o.securum  = VERUM;
    CREDO_VERUM(_aequalis(crustulum_scribere("t", _c("1", p), &o, p),
        "t=1; Path=/; Secure; SameSite=None"));
}

interior vacuum
_sectio_recusare (CredoContextus* c)
{
              Piscina* p = c->piscina;
    CrustulumOptiones  o = _optiones();

    /* tituli mali */
    CREDO_AEQUALIS_I32(crustulum_scribere("", _c("v", p), NIHIL,
        p).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(crustulum_scribere("a b", _c("v", p), NIHIL,
        p).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(crustulum_scribere("a;b", _c("v", p), NIHIL,
        p).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(crustulum_scribere("a=b", _c("v", p), NIHIL,
        p).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(crustulum_scribere("a\tb", _c("v", p), NIHIL,
        p).mensura, ZEPHYRUM);

    /* valores mali */
    CREDO_AEQUALIS_I32(crustulum_scribere("a", _c("x,y", p), NIHIL,
        p).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(crustulum_scribere("a", _c("x;y", p), NIHIL,
        p).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(crustulum_scribere("a", _c("x y", p), NIHIL,
        p).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(crustulum_scribere("a", _c("x\"y", p), NIHIL,
        p).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(crustulum_scribere("a", _c("x\\y", p), NIHIL,
        p).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(crustulum_scribere("a",
        _c("x\r\nSet-Cookie: b=1",
        p), NIHIL, p).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(crustulum_scribere("a", _c("x\001", p), NIHIL,
        p).mensura, ZEPHYRUM);

    /* attributa mala (injectio per Path / Domain) */
    o.via = "/; Domain=malum.example";
    CREDO_AEQUALIS_I32(crustulum_scribere("a", _c("v", p), &o,
        p).mensura, ZEPHYRUM);
    o           = _optiones();
    o.dominium  = "a.example\r\nX-Malum: 1";
    CREDO_AEQUALIS_I32(crustulum_scribere("a", _c("v", p), &o,
        p).mensura, ZEPHYRUM);

    /* SameSite=None sine Secure */
    o        = _optiones();
    o.eadem  = CRUSTULUM_EADEM_NULLA;
    CREDO_AEQUALIS_I32(crustulum_scribere("a", _c("v", p), &o,
        p).mensura, ZEPHYRUM);

    /* praefixa */
    o = _optiones();
    CREDO_AEQUALIS_I32(crustulum_scribere("__Secure-a", _c("v", p), &o,
        p).mensura, ZEPHYRUM);
    o.securum = VERUM;
    CREDO_VERUM(crustulum_scribere("__Secure-a", _c("v", p), &o,
        p).mensura > ZEPHYRUM);
    o.dominium = "a.example";
    CREDO_AEQUALIS_I32(crustulum_scribere("__Host-a", _c("v", p), &o,
        p).mensura, ZEPHYRUM);
    o.dominium  = NIHIL;
    o.via       = "/sub";
    CREDO_AEQUALIS_I32(crustulum_scribere("__Host-a", _c("v", p), &o,
        p).mensura, ZEPHYRUM);
    o.via = NIHIL;
    CREDO_VERUM(_aequalis(crustulum_scribere("__Host-a", _c("v", p), &o,
        p), "__Host-a=v; Path=/; Secure; SameSite=Lax"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "legere", _sectio_legere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "scribere", _sectio_scribere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusare", _sectio_recusare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("crustula", SECTIONES);
}
