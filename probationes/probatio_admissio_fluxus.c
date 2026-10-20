/* probatio_admissio_fluxus.c - admissio II (mcp-remotus T6): fluxus
 * OAuth totus in processu. Hospitium + cliens TCP crudus (P8),
 * intratus probationis (crustulum "sessio=valida" -> fran, CSRF
 * fixum), horologium iniectum, PKCE ex RFC 7636 appendice B.
 * Inscriptio -> authorize (consensus) -> code -> token -> refresh;
 * insidiae (code iteratum, PKCE falsum, resource alienum,
 * redirectiones falsae, renovatio iterata, plain, cliens ignotus);
 * expiratio; consensus malus; scopi electi; forma corporis.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "tcp.h"
#include "hospitium.h"
#include "json.h"
#include "url.h"
#include "tempus.h"
#include "volumen.h"
#include "mcp_servus.h"
#include "admissio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUCTORITAS  "https://exemplar.example"
#define AUDIENTIA   "https://exemplar.example/mcp"
#define CALLBACK    "https://claude.ai/api/mcp/auth_callback"
#define VERIFICATOR "dBjftJeZ4CVP-mB92K27uhbUJU1p1r_wW1gFWFOEjXk"
#define PROVOCATIO  "E9Melhoa2OwvFrEMTJguCHaoeK1t8URWbuGJSstw-cM"
#define CSRF        "csrf-fixum"
#define SESSIO      "Cookie: sessio=valida\r\n"
#define ORIGO       "Origin: https://exemplar.example\r\n"
#define FORMA \
    "Content-Type: application/x-www-form-urlencoded\r\n"
#define CAPITA_N    (VIII * MXXIV)
#define DIES_MS     ((s64)XXIV * LX * LX * M)

hic_manens s64 g_nunc = (s64)1760000000 * M;

interior s64
_nunc (vacuum)
{
    redde g_nunc;
}

interior vacuum
_horologium_parare (
    CredoContextus* c)
{
    (vacuum)c;
    g_nunc = (s64)1760000000 * M;
}

hic_manens constans character* constans SCOPI[] = {
    "notae:legere", "notae:scribere", NIHIL
};


/* ==================================================
 * cliens TCP (auxilia probatio_mcp_servus_http.c)
 * ================================================== */

nomen structura {
    TcpConnexio* c;
             i8* data;
            s64  n;
            s64  capacitas;
            s64  finis_capitum;
            s64  corporis;
            b32  finitum;
            i32  status;
} Lector;

interior vacuum
_pumpare (
    Hospitium* h,
          i32  vices)
{
    i32 i;

    per (i = ZEPHYRUM; i < vices; i++)
    {
        (vacuum)hospitium_gressus(h, I);
    }
}

interior b32
_aperire (
    Hospitium* h,
       Lector* l,
      Piscina* p)
{
    TcpOptiones o = tcp_optiones_default();
    TcpResultus r;

    memset(l, ZEPHYRUM, magnitudo(*l));
    o.timeout_ms = I;
    r = tcp_connectere_cum_optionibus("127.0.0.1", hospitium_portus(h),
        &o, p);
    si (!r.successus)
    {
        redde FALSUM;
    }
    l->c          = r.connexio;
    l->capacitas  = LXIV * MXXIV;
    l->data           = (i8*)piscina_allocare(p,
        (memoriae_index)l->capacitas + I);
    l->finis_capitum  = -I;
    l->corporis       = -I;
    redde VERUM;
}

interior b32
_progredi (Lector* l)
{
     i8 capsa[XVI * MXXIV];
    s32 k;

    si (l->finitum)
    {
        redde VERUM;
    }
    k = tcp_recipere(l->c, capsa, (i32)magnitudo(capsa));
    si (k == TCP_ITERUM)
    {
        redde FALSUM;
    }
    si (k <= ZEPHYRUM)
    {
        l->finitum = l->finis_capitum >= ZEPHYRUM;
        redde l->finitum;
    }
    si (l->n + k <= l->capacitas)
    {
        memcpy(l->data + l->n, capsa, (size_t)k);
    }
    l->n                                                 += (s64)k;
    l->data[l->n <= l->capacitas ? l->n : l->capacitas]  = '\0';
    si (l->finis_capitum < ZEPHYRUM)
    {
        character* f = strstr((character*)l->data, "\r\n\r\n");

        si (f != NIHIL)
        {
            character* cl = strstr((character*)l->data,
                "Content-Length: ");

            l->finis_capitum  = (s64)(f - (character*)l->data) + IV;
            l->corporis       = cl ? (s64)atol(cl + XVI) : ZEPHYRUM;
            l->status         = (i32)atoi((character*)l->data + IX);
        }
    }
    si (   l->finis_capitum >= ZEPHYRUM
        && l->n             >= l->finis_capitum + l->corporis)
    {
        l->finitum = VERUM;
    }
    redde l->finitum;
}

/* petitio una et responsum totum */
interior b32
_rogare (
              Hospitium* h,
                Piscina* p,
    constans character* linea,
    constans character* capita_extra,
    constans character* corpus,
                 Lector* l)
{
    character* capita  = (character*)piscina_allocare(p, CAPITA_N);
          s64  n       = corpus ? (s64)strlen(corpus) : ZEPHYRUM;
          i32  k;
          b32  bene;

    si (!_aperire(h, l, p))
    {
        redde FALSUM;
    }
    sprintf(capita, "%s HTTP/1.1\r\nHost: 127.0.0.1:%u\r\n"
        "Connection: close\r\n%sContent-Length: %ld\r\n\r\n", linea,
        (insignatus integer)hospitium_portus(h),
        capita_extra ? capita_extra : "", (longus)n);
    bene = tcp_mittere_omnia(l->c, (constans i8*)capita,
        (i32)strlen(capita));
    si (bene && n > ZEPHYRUM)
    {
        bene = tcp_mittere_omnia(l->c, (constans i8*)corpus, (i32)n);
    }
    per (k = ZEPHYRUM; bene && k < C * X; k++)
    {
        _pumpare(h, X);
        si (_progredi(l))
        {
            frange;
        }
    }
    tcp_claudere(l->c);
    l->c = NIHIL;
    redde bene && l->finitum;
}

interior constans character*
_corpus (
    Lector* l)
{
    redde (constans character*)l->data + l->finis_capitum;
}

interior b32
_continet (
                Lector* l,
    constans character* s)
{
    redde l->finitum
        && strstr((constans character*)l->data, s) != NIHIL;
}

/* valor capitis Location (NUL); NIHIL si absens */
interior character*
_locus (
     Lector* l,
    Piscina* p)
{
    constans character* a = strstr((constans character*)l->data,
        "\r\nLocation: ");
    constans character* b;
             character* t;

    si (   !a || (l->finis_capitum >= ZEPHYRUM
            && a > (constans character*)l->data + l->finis_capitum))
    {
        redde NIHIL;
    }
    a += XII;
    b = strstr(a, "\r\n");
    t = (character*)piscina_allocare(p, (memoriae_index)(b - a) + I);
    memcpy(t, a, (size_t)(b - a));
    t[b - a] = '\0';
    redde t;
}

/* parametrum quaestionis in URL (decodificatum); vacua si absens */
interior chorda
_param (
     constans character* url,
     constans character* clavis,
                Piscina* p)
{
     constans character* q = url ? strchr(url, '?') : NIHIL;
              UrlParams  u;
                    i32  k;
                 chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (!q)
    {
        redde vacua;
    }
    u = url_params_parse(chorda_ex_literis(q + I, p), p);
    per (k = ZEPHYRUM; k < u.numerus; k++)
    {
        si (chorda_aequalis_literis(u.claves[k], clavis))
        {
            redde u.valores[k];
        }
    }
    redde vacua;
}

interior constans character*
_codificare (
    constans character* s,
               Piscina* p)
{
    redde chorda_ut_cstr(url_codificare(chorda_ex_literis(s, p), p), p);
}

/* valor campi JSON corporis (NUL); "" si absens */
interior constans character*
_json (
                Lector* l,
    constans character* clavis,
               Piscina* p)
{
    JsonResultus r = json_legere_literis(_corpus(l), p);
          chorda v;

    si (!r.successus)
    {
        redde "";
    }
    /* campus absens -> "" (numquam NIHIL: strcmp vocantium) */
    v = json_ad_chorda(json_objectum_capere(r.radix, clavis));
    si (!v.datum || v.mensura == ZEPHYRUM)
    {
        redde "";
    }
    redde chorda_ut_cstr(v, p);
}


/* ==================================================
 * fixura: hospitium + admissio
 * ================================================== */

interior b32
_intratus (
                 vacuum* datum,
    HospitiumColloquium* col,
                 chorda* usor,
                 chorda* csrf)
{
    chorda s = colloquium_crustulum(col, "sessio");

    (vacuum)datum;
    si (!s.datum || !chorda_aequalis_literis(s, "valida"))
    {
        redde FALSUM;
    }
    *usor = chorda_ex_literis("fran", colloquium_piscina(col));
    *csrf = chorda_ex_literis(CSRF, colloquium_piscina(col));
    redde VERUM;
}

nomen structura {
    Hospitium* h;
     Admissio* a;
      Volumen* v;
          i32  speculata;
} Fixura;

interior vacuum
_speculator (
                 vacuum* datum,
    HospitiumColloquium* colloquium)
{
    (vacuum)colloquium;
    ((Fixura*)datum)->speculata++;
}

interior b32
_fixura (
    CredoContextus* c,
            Fixura* f)
{
    HospitiumConfiguratio hospitii;
         AdmissioOptiones o;

    memset(f, ZEPHYRUM, magnitudo(*f));
    memset(&hospitii, ZEPHYRUM, magnitudo(hospitii));
    f->h = hospitium_creare(c->piscina, &hospitii);
    f->v = volumen_temporarium(c->piscina, "admissio_fluxus");
    si (!f->h || !f->v)
    {
        redde FALSUM;
    }
    memset(&o, ZEPHYRUM, magnitudo(o));
    o.auctoritas  = AUCTORITAS;
    o.audientia   = AUDIENTIA;
    o.scopi       = SCOPI;
    o.nunc_ms     = _nunc;
    f->a          = admissio_creare(f->v, &o, c->piscina);
    si (!f->a)
    {
        redde FALSUM;
    }
    admissio_speculatorem_ponere(f->a, _speculator, f);
    redde admissio_montare(f->a, f->h, _intratus, NIHIL, "/intrare");
}

interior vacuum
_fixuram_solvere (
    Fixura* f)
{
    si (f->h)
    {
        hospitium_destruere(f->h);
    }
    si (f->v)
    {
        volumen_claudere(f->v);
    }
}

/* client_id; "" si fractum */
interior constans character*
_inscribere (
          CredoContextus* c,
                  Fixura* f,
      constans character* redirectio)
{
       Lector l;
    character corpus[DXII];

    sprintf(corpus, "{\"redirect_uris\":[\"%s\"],\"client_name\":"
        "\"Claude\",\"token_endpoint_auth_method\":\"none\"}",
        redirectio);
    si (   !_rogare(f->h, c->piscina, "POST /oauth/register",
               "Content-Type: application/json\r\n", corpus, &l)
        || l.status != CCI)
    {
        redde "";
    }
    redde _json(&l, "client_id", c->piscina);
}

/* parametra authorize (sine '?') */
interior character*
_parametra (
          CredoContextus* c,
      constans character* cliens,
      constans character* redirectio,
      constans character* scopus,
      constans character* methodus,
      constans character* audientia)
{
    character* t = (character*)piscina_allocare(c->piscina, MMXLVIII);

    sprintf(t, "response_type=code&client_id=%s&redirect_uri=%s"
        "&code_challenge=%s&code_challenge_method=%s&scope=%s"
        "&state=status-xyz&resource=%s", _codificare(cliens,
        c->piscina),
        _codificare(redirectio, c->piscina), PROVOCATIO, methodus,
        _codificare(scopus, c->piscina), _codificare(audientia,
        c->piscina));
    redde t;
}

interior b32
_get (
          CredoContextus* c,
                  Fixura* f,
      constans character* parametra,
      constans character* capita,
                  Lector* l)
{
    character* linea = (character*)piscina_allocare(c->piscina,
        (memoriae_index)strlen(parametra) + XXXII);

    sprintf(linea, "GET /oauth/authorize?%s", parametra);
    redde _rogare(f->h, c->piscina, linea, capita, NIHIL, l);
}

/* POST consensus: parametra + csrf + scopi electi + decisio */
interior b32
_post (
          CredoContextus* c,
                  Fixura* f,
      constans character* parametra,
      constans character* electi,     /* e.g. "&s0=1&s1=1" */
      constans character* csrf,       /* NIHIL = omissum */
      constans character* decisio,
      constans character* capita,
                  Lector* l)
{
    character* corpus = (character*)piscina_allocare(c->piscina,
        (memoriae_index)strlen(parametra) + CCLVI);

    sprintf(corpus, "%s%s%s%s&decisio=%s", parametra, electi,
        csrf ? "&csrf=" : "", csrf ? csrf : "", decisio);
    redde _rogare(f->h, c->piscina, "POST /oauth/authorize", capita,
        corpus, l);
}

/* code ex consensu permisso; "" si fractum */
interior constans character*
_codicem (
          CredoContextus* c,
                  Fixura* f,
      constans character* cliens,
      constans character* scopus,
      constans character* electi)
{
       Lector  l;
    character* parametra = _parametra(c, cliens, CALLBACK, scopus,
        "S256",
        AUDIENTIA);

    si (   !_post(c, f, parametra, electi, CSRF, "permittere",
               FORMA SESSIO ORIGO, &l)
        || l.status != CCCII)
    {
        redde "";
    }
    redde chorda_ut_cstr(_param(_locus(&l, c->piscina), "code",
        c->piscina), c->piscina);
}

interior b32
_token (
          CredoContextus* c,
                  Fixura* f,
      constans character* corpus,
                  Lector* l)
{
    redde _rogare(f->h, c->piscina, "POST /oauth/token", FORMA, corpus,
        l);
}

interior b32
_permutare (
          CredoContextus* c,
                  Fixura* f,
      constans character* cliens,
      constans character* codex,
      constans character* verificator,
      constans character* audientia,
                  Lector* l)
{
    character corpus[MMXLVIII];

    sprintf(corpus, "grant_type=authorization_code&code=%s"
        "&redirect_uri=%s&client_id=%s&code_verifier=%s&resource=%s",
        codex, _codificare(CALLBACK, c->piscina), cliens, verificator,
        _codificare(audientia, c->piscina));
    redde _token(c, f, corpus, l);
}

interior b32
_renovare (
          CredoContextus* c,
                  Fixura* f,
      constans character* cliens,
      constans character* renovatio,
                  Lector* l)
{
    character corpus[DXII];

    sprintf(corpus, "grant_type=refresh_token&refresh_token=%s"
        "&client_id=%s", renovatio, cliens);
    redde _token(c, f, corpus, l);
}

interior b32
_valet (
          CredoContextus* c,
                  Fixura* f,
      constans character* accessus)
{
    McpPersona q;

    memset(&q, ZEPHYRUM, magnitudo(q));
    redde admissio_verificator(f->a, chorda_ex_literis(accessus,
        c->piscina), chorda_ex_literis(AUDIENTIA, c->piscina), &q,
        c->piscina);
}

#define OMNES "notae:legere notae:scribere offline_access"
#define AMBO  "&s0=1&s1=1"


/* ==================================================
 * sectiones
 * ================================================== */

interior vacuum
_sectio_fluxus (CredoContextus* c)
{
                Fixura  f;
                Lector  l;
    constans character* cliens;
             character* parametra;
    constans character* codex;
    constans character* accessus;
    constans character* renovatio;
    constans character* nova;

    CREDO_NECESSE(_fixura(c, &f));
    CREDO_VERUM(_rogare(f.h, c->piscina,
        "GET /.well-known/oauth-authorization-server", NIHIL, NIHIL,
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "\"issuer\":\"" AUCTORITAS "\""));
    cliens = _inscribere(c, &f, CALLBACK);
    CREDO_NECESSE(strlen(cliens) > X);
    CREDO_AEQUALIS_I32(f.speculata, II);   /* documentum + register */
    /* pagina consensus */
    parametra = _parametra(c, cliens, CALLBACK, OMNES, "S256",
        AUDIENTIA);
    CREDO_VERUM(_get(c, &f, parametra, SESSIO, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "Claude"));
    CREDO_VERUM(_continet(&l, "<strong>claude.ai</strong>"));
    CREDO_VERUM(_continet(&l, "name=\"s0\" value=\"1\" checked"));
    CREDO_VERUM(_continet(&l, "name=\"s1\" value=\"1\" checked"));
    CREDO_VERUM(_continet(&l, "name=\"csrf\" value=\"" CSRF "\""));
    CREDO_FALSUM(_continet(&l, "programma locale"));
    /* permittere -> 302 cum code, state, iss */
    CREDO_VERUM(_post(c, &f, parametra, AMBO, CSRF, "permittere",
        FORMA SESSIO ORIGO, &l));
    CREDO_AEQUALIS_I32(l.status, CCCII);
    CREDO_NECESSE_NON_NIHIL(_locus(&l, c->piscina));
    CREDO_VERUM(strncmp(_locus(&l, c->piscina), CALLBACK "?",
        strlen(CALLBACK) + I) == ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(_param(_locus(&l, c->piscina),
        "state",
        c->piscina), "status-xyz");
    CREDO_CHORDA_AEQUALIS_LITERIS(_param(_locus(&l, c->piscina), "iss",
        c->piscina), AUCTORITAS);
    codex = chorda_ut_cstr(_param(_locus(&l, c->piscina), "code",
        c->piscina), c->piscina);
    CREDO_AEQUALIS_I32((i32)strlen(codex), XLIII);
    /* token */
    CREDO_VERUM(_permutare(c, &f, cliens, codex, VERIFICATOR, AUDIENTIA,
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "Cache-Control: no-store"));
    CREDO_VERUM(_continet(&l, "\"token_type\":\"Bearer\""));
    CREDO_VERUM(_continet(&l, "\"expires_in\":3600"));
    CREDO_VERUM(strcmp(_json(&l, "scope", c->piscina), OMNES)
        == ZEPHYRUM);
    accessus   = _json(&l, "access_token", c->piscina);
    renovatio  = _json(&l, "refresh_token", c->piscina);
    CREDO_AEQUALIS_I32((i32)strlen(accessus), XLIII);
    CREDO_AEQUALIS_I32((i32)strlen(renovatio), XLIII);
    CREDO_VERUM(_valet(c, &f, accessus));
    /* renovatio: par novum, vetus mortuum */
    CREDO_VERUM(_renovare(c, &f, cliens, renovatio, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    nova = _json(&l, "access_token", c->piscina);
    CREDO_VERUM(_valet(c, &f, nova));
    CREDO_VERUM(strcmp(_json(&l, "refresh_token", c->piscina),
        renovatio)
        != ZEPHYRUM);
    CREDO_VERUM(_renovare(c, &f, cliens, _json(&l, "refresh_token",
        c->piscina), &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    /* cliens solum loopback: monitio programmatis localis */
    cliens = _inscribere(c, &f, "http://127.0.0.1:3000/callback");
    CREDO_VERUM(_get(c, &f, _parametra(c, cliens,
        "http://127.0.0.1:3000/callback", OMNES, "S256", AUDIENTIA),
        SESSIO,
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "programma locale"));
    _fixuram_solvere(&f);
}

interior vacuum
_sectio_insidiae (CredoContextus* c)
{
                Fixura  f;
                Lector  l;
    constans character* cliens;
    constans character* codex;
    constans character* accessus;
    constans character* r1;
    constans character* r2;
    constans character* a2;
    constans character* loopback;

    CREDO_NECESSE(_fixura(c, &f));
    cliens = _inscribere(c, &f, CALLBACK);
    /* code iteratum: invalid_grant ET tesserae priores revocatae */
    codex = _codicem(c, &f, cliens, OMNES, AMBO);
    CREDO_VERUM(_permutare(c, &f, cliens, codex, VERIFICATOR, AUDIENTIA,
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    accessus = _json(&l, "access_token", c->piscina);
    CREDO_VERUM(_valet(c, &f, accessus));
    CREDO_VERUM(_permutare(c, &f, cliens, codex, VERIFICATOR, AUDIENTIA,
        &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    CREDO_VERUM(strcmp(_json(&l, "error", c->piscina), "invalid_grant")
        == ZEPHYRUM);
    CREDO_FALSUM(_valet(c, &f, accessus));
    /* PKCE falsum */
    codex = _codicem(c, &f, cliens, OMNES, AMBO);
    CREDO_VERUM(_permutare(c, &f, cliens, codex,
        "dBjftJeZ4CVP-mB92K27uhbUJU1p1r_wW1gFWFOEjXX", AUDIENTIA, &l));
    CREDO_VERUM(strcmp(_json(&l, "error", c->piscina), "invalid_grant")
        == ZEPHYRUM);
    /* resource alienum */
    codex = _codicem(c, &f, cliens, OMNES, AMBO);
    CREDO_VERUM(_permutare(c, &f, cliens, codex, VERIFICATOR,
        "https://alius.example/mcp", &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    CREDO_VERUM(strcmp(_json(&l, "error", c->piscina), "invalid_target")
        == ZEPHYRUM);
    CREDO_VERUM(_get(c, &f, _parametra(c, cliens, CALLBACK, OMNES,
        "S256",
        "https://alius.example/mcp"), SESSIO, &l));
    CREDO_AEQUALIS_I32(l.status, CCCII);
    CREDO_CHORDA_AEQUALIS_LITERIS(_param(_locus(&l, c->piscina),
        "error",
        c->piscina), "invalid_target");
    /* redirectiones falsae: pagina erroris, nulla Location */
    CREDO_VERUM(_get(c, &f, _parametra(c, cliens, CALLBACK "/plus",
        OMNES,
        "S256", AUDIENTIA), SESSIO, &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    CREDO_NIHIL(_locus(&l, c->piscina));
    CREDO_VERUM(_get(c, &f, _parametra(c, cliens,
        "https://evil.example/api/mcp/auth_callback", OMNES, "S256",
        AUDIENTIA), SESSIO, &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    CREDO_NIHIL(_locus(&l, c->piscina));
    CREDO_VERUM(_get(c, &f, _parametra(c, cliens,
        "https://claude.ai:8443/api/mcp/auth_callback", OMNES, "S256",
        AUDIENTIA), SESSIO, &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    CREDO_NIHIL(_locus(&l, c->piscina));
    /* loopback: portus alius licet */
    loopback = _inscribere(c, &f, "http://127.0.0.1:3000/callback");
    CREDO_VERUM(_get(c, &f, _parametra(c, loopback,
        "http://127.0.0.1:5555/callback", OMNES, "S256", AUDIENTIA),
        SESSIO,
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_get(c, &f, _parametra(c, loopback,
        "http://127.0.0.1:5555/aliud", OMNES, "S256", AUDIENTIA),
        SESSIO,
        &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    /* renovatio iterata: invalid_grant ET familia tota revocata */
    codex = _codicem(c, &f, cliens, OMNES, AMBO);
    CREDO_VERUM(_permutare(c, &f, cliens, codex, VERIFICATOR, AUDIENTIA,
        &l));
    r1 = _json(&l, "refresh_token", c->piscina);
    CREDO_VERUM(_renovare(c, &f, cliens, r1, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    a2 = _json(&l, "access_token", c->piscina);
    r2 = _json(&l, "refresh_token", c->piscina);
    CREDO_VERUM(_valet(c, &f, a2));
    CREDO_VERUM(_renovare(c, &f, cliens, r1, &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    CREDO_VERUM(strcmp(_json(&l, "error", c->piscina), "invalid_grant")
        == ZEPHYRUM);
    CREDO_FALSUM(_valet(c, &f, a2));
    CREDO_VERUM(_renovare(c, &f, cliens, r2, &l));
    CREDO_VERUM(strcmp(_json(&l, "error", c->piscina), "invalid_grant")
        == ZEPHYRUM);
    /* plain refutatum */
    CREDO_VERUM(_get(c, &f, _parametra(c, cliens, CALLBACK, OMNES,
        "plain", AUDIENTIA), SESSIO, &l));
    CREDO_AEQUALIS_I32(l.status, CCCII);
    CREDO_CHORDA_AEQUALIS_LITERIS(_param(_locus(&l, c->piscina),
        "error",
        c->piscina), "invalid_request");
    /* cliens ignotus: pagina erroris */
    CREDO_VERUM(_get(c, &f, _parametra(c, "ignotus", CALLBACK, OMNES,
        "S256", AUDIENTIA), SESSIO, &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    CREDO_NIHIL(_locus(&l, c->piscina));
    _fixuram_solvere(&f);
}

/* Review focus 2: tessera mediam per colloquium expirat */
interior vacuum
_sectio_terminus (CredoContextus* c)
{
                Fixura  f;
                Lector  l;
    constans character* cliens;
    constans character* codex;
    constans character* accessus;
    constans character* renovatio;
                   s64  initium;

    CREDO_NECESSE(_fixura(c, &f));
    cliens = _inscribere(c, &f, CALLBACK);
    /* code post LXI s */
    codex   = _codicem(c, &f, cliens, OMNES, AMBO);
    g_nunc  += (s64)LXI * M;
    CREDO_VERUM(_permutare(c, &f, cliens, codex, VERIFICATOR, AUDIENTIA,
        &l));
    CREDO_VERUM(strcmp(_json(&l, "error", c->piscina), "invalid_grant")
        == ZEPHYRUM);
    /* accessus ad expirationem + I ms; renovatio dat accessum novum */
    initium  = g_nunc;
    codex    = _codicem(c, &f, cliens, OMNES, AMBO);
    CREDO_VERUM(_permutare(c, &f, cliens, codex, VERIFICATOR, AUDIENTIA,
        &l));
    accessus   = _json(&l, "access_token", c->piscina);
    renovatio  = _json(&l, "refresh_token", c->piscina);
    g_nunc     = initium + (s64)LX * LX * M + I;
    CREDO_FALSUM(_valet(c, &f, accessus));
    CREDO_VERUM(_renovare(c, &f, cliens, renovatio, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_valet(c, &f, _json(&l, "access_token", c->piscina)));
    CREDO_VERUM(_renovare(c, &f, cliens, renovatio, &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    /* renovatio ad secundum expirationis exactum: refutata */
    initium  = g_nunc;
    codex    = _codicem(c, &f, cliens, OMNES, AMBO);
    CREDO_VERUM(_permutare(c, &f, cliens, codex, VERIFICATOR, AUDIENTIA,
        &l));
    renovatio  = _json(&l, "refresh_token", c->piscina);
    g_nunc     = initium + (s64)XXX * DIES_MS;
    CREDO_VERUM(_renovare(c, &f, cliens, renovatio, &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    CREDO_VERUM(strcmp(_json(&l, "error", c->piscina), "invalid_grant")
        == ZEPHYRUM);
    _fixuram_solvere(&f);
}

/* Review focus 5: pagina consensus via mala adita */
interior vacuum
_sectio_consensus_malus (CredoContextus* c)
{
                Fixura  f;
                Lector  l;
    constans character* cliens;
             character* parametra;
             character* locus;

    CREDO_NECESSE(_fixura(c, &f));
    cliens = _inscribere(c, &f, CALLBACK);
    parametra = _parametra(c, cliens, CALLBACK, OMNES, "S256",
        AUDIENTIA);
    CREDO_VERUM(_rogare(f.h, c->piscina, "GET /oauth/authorize", SESSIO,
        NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    CREDO_NIHIL(_locus(&l, c->piscina));
    /* non intratus: ad paginam intrandi */
    CREDO_VERUM(_get(c, &f, parametra, NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CCCIII);
    locus = _locus(&l, c->piscina);
    CREDO_NECESSE_NON_NIHIL(locus);
    CREDO_VERUM(strncmp(locus, "/intrare?redire=", XVI) == ZEPHYRUM);
    /* CSRF absens, vetus; Origo aliena */
    CREDO_VERUM(_post(c, &f, parametra, AMBO, NIHIL, "permittere",
        FORMA SESSIO ORIGO, &l));
    CREDO_AEQUALIS_I32(l.status, CDIII);
    CREDO_NIHIL(_locus(&l, c->piscina));
    CREDO_VERUM(_post(c, &f, parametra, AMBO, "vetus", "permittere",
        FORMA SESSIO ORIGO, &l));
    CREDO_AEQUALIS_I32(l.status, CDIII);
    CREDO_VERUM(_post(c, &f, parametra, AMBO, CSRF, "permittere",
        FORMA SESSIO "Origin: https://evil.example\r\n", &l));
    CREDO_AEQUALIS_I32(l.status, CDIII);
    /* sessio finita inter GET et POST */
    CREDO_VERUM(_post(c, &f, parametra, AMBO, CSRF, "permittere",
        FORMA ORIGO, &l));
    CREDO_AEQUALIS_I32(l.status, CCCIII);
    CREDO_FALSUM(_continet(&l, "code="));
    /* negare */
    CREDO_VERUM(_post(c, &f, parametra, AMBO, CSRF, "negare",
        FORMA SESSIO ORIGO, &l));
    CREDO_AEQUALIS_I32(l.status, CCCII);
    locus = _locus(&l, c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(_param(locus, "error", c->piscina),
        "access_denied");
    CREDO_CHORDA_AEQUALIS_LITERIS(_param(locus, "state", c->piscina),
        "status-xyz");
    CREDO_CHORDA_AEQUALIS_LITERIS(_param(locus, "iss", c->piscina),
        AUCTORITAS);
    CREDO_FALSUM(_param(locus, "code", c->piscina).datum != NIHIL);
    _fixuram_solvere(&f);
}

interior vacuum
_sectio_scopi_electi (CredoContextus* c)
{
                Fixura  f;
                Lector  l;
    constans character* cliens;
    constans character* codex;

    CREDO_NECESSE(_fixura(c, &f));
    cliens = _inscribere(c, &f, CALLBACK);
    /* legere solum notatum, sine offline_access: sine renovatione */
    codex = _codicem(c, &f, cliens, "notae:legere notae:scribere",
        "&s0=1");
    CREDO_VERUM(_permutare(c, &f, cliens, codex, VERIFICATOR, AUDIENTIA,
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(strcmp(_json(&l, "scope", c->piscina), "notae:legere")
        == ZEPHYRUM);
    CREDO_FALSUM(_continet(&l, "refresh_token"));
    /* offline_access: renovatio data */
    codex = _codicem(c, &f, cliens, "notae:scribere offline_access",
        "&s0=1");
    CREDO_VERUM(_permutare(c, &f, cliens, codex, VERIFICATOR, AUDIENTIA,
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(strcmp(_json(&l, "scope", c->piscina),
        "notae:scribere offline_access") == ZEPHYRUM);
    CREDO_VERUM(_continet(&l, "refresh_token"));
    /* nullus notatus: access_denied */
    CREDO_VERUM(_post(c, &f, _parametra(c, cliens, CALLBACK, OMNES,
        "S256",
        AUDIENTIA), "", CSRF, "permittere", FORMA SESSIO ORIGO, &l));
    CREDO_AEQUALIS_I32(l.status, CCCII);
    CREDO_CHORDA_AEQUALIS_LITERIS(_param(_locus(&l, c->piscina),
        "error",
        c->piscina), "access_denied");
    /* scopus ignotus */
    CREDO_VERUM(_get(c, &f, _parametra(c, cliens, CALLBACK,
        "notae:delere",
        "S256", AUDIENTIA), SESSIO, &l));
    CREDO_AEQUALIS_I32(l.status, CCCII);
    CREDO_CHORDA_AEQUALIS_LITERIS(_param(_locus(&l, c->piscina),
        "error",
        c->piscina), "invalid_scope");
    _fixuram_solvere(&f);
}

interior vacuum
_sectio_forma (CredoContextus* c)
{
                Fixura  f;
                Lector  l;
    constans character* cliens;
    constans character* codex;
                   f64  t0;

    CREDO_NECESSE(_fixura(c, &f));
    cliens = _inscribere(c, &f, CALLBACK);
    CREDO_VERUM(_rogare(f.h, c->piscina, "POST /oauth/token",
        "Content-Type: application/json\r\n",
        "{\"grant_type\":\"refresh_token\",\"refresh_token\":\"x\"}",
        &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    CREDO_VERUM(strcmp(_json(&l, "error", c->piscina),
        "invalid_request")
        == ZEPHYRUM);
    CREDO_VERUM(_rogare(f.h, c->piscina, "POST /oauth/token", FORMA,
        "grant_type=password&username=a&password=b", &l));
    CREDO_VERUM(strcmp(_json(&l, "error", c->piscina),
        "unsupported_grant_type") == ZEPHYRUM);
    /* responsum cito (custos contra argon2 aut rete fortuitum) */
    codex  = _codicem(c, &f, cliens, OMNES, AMBO);
    t0     = tempus_nunc();
    CREDO_VERUM(_permutare(c, &f, cliens, codex, VERIFICATOR, AUDIENTIA,
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(tempus_nunc() - t0 < 0.1);
    _fixuram_solvere(&f);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "fluxus", _sectio_fluxus, _horologium_parare, NIHIL, NIHIL,
        NIHIL },
    { "insidiae", _sectio_insidiae, _horologium_parare, NIHIL, NIHIL,
        NIHIL },
    { "terminus", _sectio_terminus, _horologium_parare, NIHIL,
        NIHIL,
        NIHIL },
    { "consensus_malus", _sectio_consensus_malus, _horologium_parare,
        NIHIL, NIHIL, NIHIL },
    { "scopi_electi", _sectio_scopi_electi, _horologium_parare, NIHIL,
        NIHIL, NIHIL },
    { "forma", _sectio_forma, _horologium_parare, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("admissio_fluxus", SECTIONES);
}
