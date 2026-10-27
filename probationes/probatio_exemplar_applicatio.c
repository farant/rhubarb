/* probatio_exemplar_applicatio.c - hospitium v3 T8: applicatio
 * probans finis ad finem (cliens in processu, data in /tmp). Iter
 * usoris (inscriptio, intrare, nota cum filo II MB, index evasus,
 * download, exire),
 * invitatio, CSRF, Origin, frenum, filum alienum = 404, restitutio ex
 * volumine (sessio superstat), salus, turba (salus ante intrationes
 * IV), secreta (acta signum crustuli et tesseram numquam continent);
 * sonda MCP/OAuth (mcp-remotus T1): 401 cum WWW-Authenticate octetim,
 * metadata RFC 9728/8414, acta sondae redacta, inscriptio 503.
 *
 * Cliens et servus in filo uno: cliens mora I ms recipit et servum XX
 * gressibus inter receptiones pumpat - labor argon2id gradum unum per
 * gyrum agit (~MCC gradus per hash), mora longior probationem
 * minutis extenderet (675 s cum XX ms). */
#include "postulata_posix.h"
#define EXEMPLAR_PROBATIO
#define ADMISSIO_PROBATIO
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "moneta.h"
#include "tcp.h"
#include "reactor.h"
#include "tempus.h"
#include "hospitium.h"
#include "volumen.h"
#include "exemplar_nucleus.h"
#include "custodia.h"
#include "communio_servus.h"
#include "cives.h"
#include "admissio.h"
#include "scrinium.h"
#include "json.h"
#include "url.h"
#include "xar.h"
#include "canalis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LIMES        "LimesExemplaris"
#define CAPITA_N     (VIII * MXXIV)
#define MAGNUM       (II * MXXIV * MXXIV)

/* ---- cliens: lector per connexionem ---- */

nomen structura {
    TcpConnexio* c;
             i8* data;
            s64  n;
            s64  capacitas;
            s64  finis_capitum;   /* -I = nondum */
            s64  corporis;        /* Content-Length */
            b32  finitum;
            i32  status;
} Lector;

interior vacuum
_pumpare (
     Exemplar* e,
          i32  vices)
{
    i32 i;

    per (i = ZEPHYRUM; i < vices; i++)
    {
        (vacuum)hospitium_gressus(exemplar_hospitium(e), I);
    }
}

interior b32
_aperire (
    Exemplar* e,
      Lector* l,
     Piscina* p,
         s64  capacitas)
{
    TcpOptiones o = tcp_optiones_default();
    TcpResultus r;

    memset(l, ZEPHYRUM, magnitudo(*l));
    o.timeout_ms = I;
    r = tcp_connectere_cum_optionibus("127.0.0.1",
        hospitium_portus(exemplar_hospitium(e)), &o, p);
    si (!r.successus)
    {
        redde FALSUM;
    }
    l->c              = r.connexio;
    l->capacitas      = capacitas;
    l->data           = (i8*)piscina_allocare(p, (i64)capacitas + I);
    l->finis_capitum  = -I;
    l->corporis       = -I;
    redde VERUM;
}

interior b32
_mittere (
       Exemplar* e,
         Lector* l,
    constans i8* d,
            s64  n)
{
    s64 missa   = ZEPHYRUM;
    i32 inanes  = ZEPHYRUM;

    dum (missa < n)
    {
        s64 pars = n - missa;
        s32 k;

        si (pars > LXIV * MXXIV)
        {
            pars = LXIV * MXXIV;
        }
        k = tcp_mittere(l->c, d + missa, (i32)pars);
        si (k < ZEPHYRUM || (k == ZEPHYRUM && ++inanes > MM))
        {
            redde FALSUM;
        }
        si (k > ZEPHYRUM)
        {
            inanes  = ZEPHYRUM;
            missa   += (s64)k;
        }
        _pumpare(e, I);
    }
    redde VERUM;
}

/* recipere semel; VERUM si responsum totum */
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

interior b32
_exspectare (
    Exemplar* e,
      Lector* l)
{
    i32 k;

    per (k = ZEPHYRUM; k < C * M; k++)
    {
        _pumpare(e, XX);
        si (_progredi(l))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_claudere (
    Lector* l)
{
    si (l->c)
    {
        tcp_claudere(l->c);
        l->c = NIHIL;
    }
}

/* corpus responsi (NUL terminatum) */
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

/* petitio una; capita_extra lineas CRLF terminatas habent */
interior b32
_rogare (
                Exemplar* e,
                 Piscina* p,
     constans character* linea,
     constans character* capita_extra,
            constans i8* corpus,
                     s64  n,
                  Lector* l,
                     s64  capacitas)
{
    character* capita = (character*)piscina_allocare(p, CAPITA_N);

    si (!_aperire(e, l, p, capacitas))
    {
        redde FALSUM;
    }
    sprintf(capita, "%s HTTP/1.1\r\nHost: 127.0.0.1:%u\r\n"
        "Connection: close\r\n%sContent-Length: %ld\r\n\r\n", linea,
        (insignatus integer)hospitium_portus(exemplar_hospitium(e)),
        capita_extra ? capita_extra : "", (longus)n);
    si (   !_mittere(e, l, (constans i8*)capita, (s64)strlen(capita))
        || (n > ZEPHYRUM && !_mittere(e, l, corpus, n)))
    {
        _claudere(l);
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_rogare_et_legere (
                Exemplar* e,
                 Piscina* p,
      constans character* linea,
      constans character* capita_extra,
      constans character* corpus,
                  Lector* l)
{
    b32 bene = _rogare(e, p, linea, capita_extra, (constans i8*)corpus,
                   corpus ? (s64)strlen(corpus) : ZEPHYRUM, l,
                   IV * MXXIV * MXXIV)
        && _exspectare(e, l);

    _claudere(l);
    redde bene;
}

/* valor post praefixum usque ad terminum (in piscina) */
interior constans character*
_extrahere (
                 Lector* l,
     constans character* praefixum,
              character  terminus,
                Piscina* p)
{
    constans character* a = strstr((constans character*)l->data,
                                praefixum);
     constans character* b;
              character* v;

    si (!a)
    {
        redde "";
    }
    a += strlen(praefixum);
    b = strchr(a, terminus);
    si (!b)
    {
        redde "";
    }
    v = (character*)piscina_allocare(p, (i64)(b - a) + I);
    memcpy(v, a, (size_t)(b - a));
    v[b - a] = '\0';
    redde v;
}

/* ---- applicatio ---- */

nomen structura {
    character via[CXXVIII];
    character origo[LXIV];      /* "Origin: http://127.0.0.1:N\r\n" */
} Locus;

interior Exemplar*
_exemplar (
    CredoContextus* c,
             Locus* locus,
               i32  frenum,
               b32  novus)
{
    ExemplarConfiguratio cfg;
                Exemplar* e;

    si (novus)
    {
        character n[MONETA_NANOID_PLAGULAE_MENSURA];

        (vacuum)moneta_nanoid_plagulae(n);
        sprintf(locus->via, "/tmp/exemplar_%s", n);
    }
    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.via_datorum     = locus->via;
    cfg.frenum_conatus  = frenum;
    e                   = exemplar_creare(&cfg, c->piscina);
    si (e)
    {
        sprintf(locus->origo, "Origin: http://127.0.0.1:%u\r\n",
            (insignatus integer)hospitium_portus(
                exemplar_hospitium(e)));
    }
    redde e;
}

interior vacuum
_finire (
    Exemplar* e,
       Locus* locus)
{
    exemplar_claudere(e);
    (vacuum)filum_arborem_delere(locus->via);
}

/* invitatio nova per uncum probationis (codex SEMEL) */
interior constans character*
_invitatio (
    Exemplar* e,
     Piscina* p)
{
    chorda codex;

    si (!exemplar_invitationem_probationis(e, &codex, p))
    {
        redde "nulla";
    }
    redde chorda_ut_cstr(codex, p);
}

/* capita cum Origin, Cookie et typo */
interior constans character*
_capita (
               Piscina* p,
                 Locus* locus,
    constans character* sid,
    constans character* typus)
{
    character* s = (character*)piscina_allocare(p, CCCLXXXIV);

    sprintf(s, "%s%s%s%s%s%s", locus->origo,
        sid ? "Cookie: sid=" : "", sid ? sid : "", sid ? "\r\n" : "",
        typus ? typus : "", typus ? "\r\n" : "");
    redde s;
}

#define FORMA_U "Content-Type: application/x-www-form-urlencoded"
#define FORMA_M "Content-Type: multipart/form-data; boundary=" LIMES

interior b32
_inscribere (
              Exemplar* e,
               Piscina* p,
                 Locus* locus,
    constans character* nomen_usoris,
    constans character* tessera,
    constans character* invitatio,
                   i32* status)
{
       Lector l;
    character corpus[CCLVI];

    sprintf(corpus, "nomen=%s&tessera=%s&invitatio=%s", nomen_usoris,
        tessera, invitatio);
    si (!_rogare_et_legere(e, p, "POST /inscribere", _capita(p, locus,
        NIHIL, FORMA_U), corpus, &l))
    {
        redde FALSUM;
    }
    *status = l.status;
    redde VERUM;
}

/* signum sessionis (crustulum sid), aut "" */
interior constans character*
_intrare (
              Exemplar* e,
               Piscina* p,
                 Locus* locus,
    constans character* nomen_usoris,
    constans character* tessera,
                   i32* status)
{
       Lector l;
    character corpus[CCLVI];

    sprintf(corpus, "nomen=%s&tessera=%s", nomen_usoris, tessera);
    si (!_rogare_et_legere(e, p, "POST /intrare", _capita(p, locus,
        NIHIL, FORMA_U), corpus, &l))
    {
        *status = ZEPHYRUM;
        redde "";
    }
    *status = l.status;
    redde _extrahere(&l, "Set-Cookie: sid=", ';', p);
}

/* GET /notae: signum CSRF paginae */
interior constans character*
_csrf_paginae (
              Exemplar* e,
               Piscina* p,
                 Locus* locus,
    constans character* sid)
{
    Lector l;

    si (   !_rogare_et_legere(e, p, "GET /notae", _capita(p, locus, sid,
        NIHIL), NIHIL, &l) || l.status != CC)
    {
        redde "";
    }
    redde _extrahere(&l, "name=\"csrf\" value=\"", '"', p);
}

/* POST /notae multipartita; status */
interior i32
_notam_mittere (
            Exemplar* e,
             Piscina* p,
               Locus* locus,
    constans character* sid,
    constans character* csrf,
    constans character* textus,
          constans i8* filum,
                  s64  filum_n,
    constans character* capita_alia)
{
    ChordaAedificator* a = chorda_aedificator_creare(p, MXXIV);
               chorda  corpus;
               Lector  l;

    si (csrf)
    {
        chorda_aedificator_appendere_literis(a, "--" LIMES "\r\n"
            "Content-Disposition: form-data; name=\"csrf\"\r\n\r\n");
        chorda_aedificator_appendere_literis(a, csrf);
        chorda_aedificator_appendere_literis(a, "\r\n");
    }
    chorda_aedificator_appendere_literis(a, "--" LIMES "\r\n"
        "Content-Disposition: form-data; name=\"textus\"\r\n\r\n");
    chorda_aedificator_appendere_literis(a, textus);
    chorda_aedificator_appendere_literis(a, "\r\n");
    si (filum)
    {
        s64 i;

        chorda_aedificator_appendere_literis(a, "--" LIMES "\r\n"
            "Content-Disposition: form-data; name=\"filum\"; "
            "filename=\"magnum \\\"x\\\".bin\"\r\n"
            "Content-Type: application/octet-stream\r\n\r\n");
        per (i = ZEPHYRUM; i < filum_n; i++)
        {
            chorda_aedificator_appendere_character(a,
                (character)filum[i]);
        }
        chorda_aedificator_appendere_literis(a, "\r\n");
    }
    chorda_aedificator_appendere_literis(a, "--" LIMES "--\r\n");
    corpus = chorda_aedificator_finire(a);
    si (   !_rogare(e, p, "POST /notae", capita_alia ? capita_alia
        : _capita(p, locus, sid, FORMA_M), corpus.datum,
        (s64)corpus.mensura, &l, CCLVI * MXXIV) || !_exspectare(e, &l))
    {
        _claudere(&l);
        redde ZEPHYRUM;
    }
    _claudere(&l);
    redde l.status;
}

interior i8*
_octeti (
    Piscina* p,
        s64  n)
{
     i8* d = (i8*)piscina_allocare(p, (i64)n);
    i32  x = XLII;
    s64  i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        x     = x * 1103515245U + 12345U;
        d[i]  = (i8)(x >> XVI);
    }
    redde d;
}

/* usor ana: inscriptus et intratus; signum reddit */
interior constans character*
_ana (
    Exemplar* e,
     Piscina* p,
       Locus* locus)
{
    i32 status;

    si (   !_inscribere(e, p, locus, "ana", "tessera-anae-bona",
        _invitatio(e, p), &status) || status != CCCIII)
    {
        redde "";
    }
    redde _intrare(e, p, locus, "ana", "tessera-anae-bona", &status);
}

/* ---- sectiones ---- */

interior vacuum
_sectio_iter (CredoContextus* c)
{
                 Locus  locus;
              Exemplar* e     = _exemplar(c, &locus, ZEPHYRUM, VERUM);
                    i8* data  = _octeti(c->piscina, MAGNUM);
    constans character* sid;
    constans character* csrf;
    constans character* via_fili;
                Lector  l;
                   i32  status;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(_inscribere(e, c->piscina, &locus, "ana",
        "tessera-anae-bona", _invitatio(e, c->piscina), &status));
    CREDO_AEQUALIS_I32(status, CCCIII);
    sid = _intrare(e, c->piscina, &locus, "ana", "tessera-anae-bona",
        &status);
    CREDO_AEQUALIS_I32(status, CCCIII);
    CREDO_AEQUALIS_I32((i32)strlen(sid), XLIII);
    csrf = _csrf_paginae(e, c->piscina, &locus, sid);
    CREDO_AEQUALIS_I32((i32)strlen(csrf), XLIII);
    CREDO_AEQUALIS_I32(_notam_mittere(e, c->piscina, &locus, sid, csrf,
        "<script>alert(1)</script> nota & prima", data, MAGNUM, NIHIL),
        CCCIII);

    /* index: textus evasus, nexus fili */
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, "GET /notae", _capita(
        c->piscina, &locus, sid, NIHIL), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l,
        "&lt;script&gt;alert(1)&lt;/script&gt; nota &amp; prima"));
    CREDO_FALSUM(_continet(&l, "<script>alert"));
    via_fili = _extrahere(&l, "href=\"/fila/", '"', c->piscina);
    CREDO_AEQUALIS_I32((i32)strlen(via_fili), LXIV);

    /* download: idem, attachment, nosniff */
    {
        character linea[CXXVIII];

        sprintf(linea, "GET /fila/%s", via_fili);
        CREDO_VERUM(_rogare(e, c->piscina, linea, _capita(c->piscina,
            &locus, sid, NIHIL), NIHIL, ZEPHYRUM, &l,
            IV * MXXIV * MXXIV)
            && _exspectare(e, &l));
        _claudere(&l);
        CREDO_AEQUALIS_I32(l.status, CC);
        CREDO_VERUM(l.corporis == MAGNUM);
        CREDO_VERUM(memcmp(l.data + l.finis_capitum, data, MAGNUM)
            == ZEPHYRUM);
        CREDO_VERUM(_continet(&l, "Content-Disposition: attachment; "
            "filename=\"magnum _x_.bin\""));
        CREDO_VERUM(_continet(&l, "X-Content-Type-Options: nosniff"));
    }

    /* exire: sessio deleta, /notae -> /intrare */
    {
        character corpus[CXXVIII];

        sprintf(corpus, "csrf=%s", csrf);
        CREDO_VERUM(_rogare_et_legere(e, c->piscina, "POST /exire",
            _capita(c->piscina, &locus, sid, FORMA_U), corpus, &l));
        CREDO_AEQUALIS_I32(l.status, CCCIII);
        CREDO_VERUM(_continet(&l, "Set-Cookie: sid=; Max-Age=0"));
    }
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, "GET /notae", _capita(
        c->piscina, &locus, sid, NIHIL), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CCCIII);
    CREDO_VERUM(_continet(&l, "Location: /intrare"));
    _finire(e, &locus);
}

interior vacuum
_sectio_sine_invitatione (CredoContextus* c)
{
       Locus  locus;
    Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
         i32  status;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(_inscribere(e, c->piscina, &locus, "ana",
        "tessera-anae-bona", "", &status));
    CREDO_AEQUALIS_I32(status, CDIII);
    CREDO_VERUM(_inscribere(e, c->piscina, &locus, "ana",
        "tessera-anae-bona", "falsa", &status));
    CREDO_AEQUALIS_I32(status, CDIII);
    /* tessera brevis: 400 */
    CREDO_VERUM(_inscribere(e, c->piscina, &locus, "ana", "brevis",
        _invitatio(e, c->piscina), &status));
    CREDO_AEQUALIS_I32(status, CD);
    _finire(e, &locus);
}

interior vacuum
_sectio_csrf (CredoContextus* c)
{
                 Locus  locus;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
    constans character* sid_a;
    constans character* sid_b;
    constans character* csrf_b;
                   i32  status;

    CREDO_NECESSE_NON_NIHIL(e);
    sid_a = _ana(e, c->piscina, &locus);
    CREDO_VERUM(_inscribere(e, c->piscina, &locus, "bea",
        "tessera-beae-bona", _invitatio(e, c->piscina), &status));
    sid_b = _intrare(e, c->piscina, &locus, "bea", "tessera-beae-bona",
        &status);
    csrf_b = _csrf_paginae(e, c->piscina, &locus, sid_b);
    CREDO_AEQUALIS_I32((i32)strlen(sid_a), XLIII);
    CREDO_AEQUALIS_I32(_notam_mittere(e, c->piscina, &locus, sid_a,
        NIHIL, "sine signo", NIHIL, ZEPHYRUM, NIHIL), CDIII);
    CREDO_AEQUALIS_I32(_notam_mittere(e, c->piscina, &locus, sid_a,
        csrf_b, "signo alieno", NIHIL, ZEPHYRUM, NIHIL), CDIII);
    _finire(e, &locus);
}

interior vacuum
_sectio_origo (CredoContextus* c)
{
                 Locus  locus;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
    constans character* sid;
    constans character* csrf;
             character  capita[CCLVI];

    CREDO_NECESSE_NON_NIHIL(e);
    sid   = _ana(e, c->piscina, &locus);
    csrf  = _csrf_paginae(e, c->piscina, &locus, sid);
    sprintf(capita, "Origin: http://malum.example\r\nCookie: sid=%s\r\n"
        "%s\r\n", sid, FORMA_M);
    CREDO_AEQUALIS_I32(_notam_mittere(e, c->piscina, &locus, sid, csrf,
        "origo aliena", NIHIL, ZEPHYRUM, capita), CDIII);
    sprintf(capita, "Cookie: sid=%s\r\n%s\r\n", sid, FORMA_M);
    CREDO_AEQUALIS_I32(_notam_mittere(e, c->piscina, &locus, sid, csrf,
        "sine origine", NIHIL, ZEPHYRUM, capita), CDIII);
    CREDO_AEQUALIS_I32(_notam_mittere(e, c->piscina, &locus, sid, csrf,
        "origo propria", NIHIL, ZEPHYRUM, NIHIL), CCCIII);
    _finire(e, &locus);
}

/* frenum III: tres falsae celeres, deinde morae, deinde 429 */
interior vacuum
_sectio_frenum (CredoContextus* c)
{
       Locus  locus;
    Exemplar* e = _exemplar(c, &locus, III, VERUM);
         i32  status;
         i32  k;
         f64  a;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(_inscribere(e, c->piscina, &locus, "ana",
        "tessera-anae-bona", _invitatio(e, c->piscina), &status));
    per (k = ZEPHYRUM; k < VI; k++)
    {
        a = tempus_nunc();
        (vacuum)_intrare(e, c->piscina, &locus, "ana", "tessera-falsa",
            &status);
        CREDO_AEQUALIS_I32(status, CDIII);
        si (k >= III)
        {
            /* morae CCL, D, M ms */
            CREDO_VERUM(tempus_nunc() - a >= 0.2);
        }
    }
    (vacuum)_intrare(e, c->piscina, &locus, "ana", "tessera-anae-bona",
        &status);
    CREDO_AEQUALIS_I32(status, CDXXIX);
    _finire(e, &locus);
}

interior vacuum
_sectio_alienus (CredoContextus* c)
{
                 Locus  locus;
              Exemplar* e     = _exemplar(c, &locus, ZEPHYRUM, VERUM);
                    i8* data  = _octeti(c->piscina, C);
    constans character* sid_a;
    constans character* sid_b;
    constans character* via_fili;
                Lector  l;
             character  linea[CXXVIII];
                   i32  status;

    CREDO_NECESSE_NON_NIHIL(e);
    sid_a = _ana(e, c->piscina, &locus);
    CREDO_AEQUALIS_I32(_notam_mittere(e, c->piscina, &locus, sid_a,
        _csrf_paginae(e, c->piscina, &locus, sid_a), "anae", data, C,
        NIHIL), CCCIII);
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, "GET /notae", _capita(
        c->piscina, &locus, sid_a, NIHIL), NIHIL, &l));
    via_fili = _extrahere(&l, "href=\"/fila/", '"', c->piscina);
    CREDO_AEQUALIS_I32((i32)strlen(via_fili), LXIV);
    CREDO_VERUM(_inscribere(e, c->piscina, &locus, "bea",
        "tessera-beae-bona", _invitatio(e, c->piscina), &status));
    sid_b = _intrare(e, c->piscina, &locus, "bea", "tessera-beae-bona",
        &status);
    sprintf(linea, "GET /fila/%s", via_fili);
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, linea, _capita(
        c->piscina, &locus, sid_b, NIHIL), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CDIV);
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, linea, _capita(
        c->piscina, &locus, NIHIL, NIHIL), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CDIV);
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, linea, _capita(
        c->piscina, &locus, sid_a, NIHIL), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    _finire(e, &locus);
}

interior vacuum
_sectio_restitutio (CredoContextus* c)
{
                 Locus  locus;
              Exemplar* e     = _exemplar(c, &locus, ZEPHYRUM, VERUM);
                    i8* data  = _octeti(c->piscina, CCLVI * MXXIV);
    constans character* sid;
    constans character* via_fili;
                Lector  l;
             character  linea[CXXVIII];
                   i32  status;

    CREDO_NECESSE_NON_NIHIL(e);
    sid = _ana(e, c->piscina, &locus);
    CREDO_AEQUALIS_I32(_notam_mittere(e, c->piscina, &locus, sid,
        _csrf_paginae(e, c->piscina, &locus, sid), "manens", data,
        CCLVI * MXXIV, NIHIL), CCCIII);
    exemplar_claudere(e);

    e = _exemplar(c, &locus, ZEPHYRUM, FALSUM);
    CREDO_NECESSE_NON_NIHIL(e);
    /* sessio vetus valet */
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, "GET /notae", _capita(
        c->piscina, &locus, sid, NIHIL), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "manens"));
    via_fili = _extrahere(&l, "href=\"/fila/", '"', c->piscina);
    sprintf(linea, "GET /fila/%s", via_fili);
    CREDO_VERUM(_rogare(e, c->piscina, linea, _capita(c->piscina,
        &locus, sid, NIHIL), NIHIL, ZEPHYRUM, &l, MXXIV * MXXIV)
        && _exspectare(e, &l));
    _claudere(&l);
    CREDO_VERUM(l.corporis == CCLVI * MXXIV
        && memcmp(l.data + l.finis_capitum, data, CCLVI * MXXIV)
           == ZEPHYRUM);
    /* usor manet: intrare iterum */
    CREDO_AEQUALIS_I32((i32)strlen(_intrare(e, c->piscina, &locus,
        "ana",
        "tessera-anae-bona", &status)), XLIII);
    _finire(e, &locus);
}

interior vacuum
_sectio_salus (CredoContextus* c)
{
       Locus  locus;
    Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
      Lector  l;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, "GET /salus", NIHIL,
        NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(strncmp(_corpus(&l), "{\"status\":\"ok\",\"acta\":",
        XXII)
        == ZEPHYRUM);
    CREDO_VERUM(atoi(_corpus(&l) + XXII) >= I);
    _finire(e, &locus);
}

/* IV intrationes simul + salus: salus ante intrationem ultimam */
interior vacuum
_sectio_turba (CredoContextus* c)
{
       Locus  locus;
    Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
      Lector  l[V];
         s32  ordo[V];
         i32  facti = ZEPHYRUM;
         i32  k;
         i32  i;
         i32  status;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(_inscribere(e, c->piscina, &locus, "ana",
        "tessera-anae-bona", _invitatio(e, c->piscina), &status));
    per (i = ZEPHYRUM; i < IV; i++)
    {
        CREDO_VERUM(_rogare(e, c->piscina, "POST /intrare", _capita(
            c->piscina, &locus, NIHIL, FORMA_U),
            (constans i8*)"nomen=ana&tessera=tessera-anae-bona", XXXV,
            &l[i], LXIV * MXXIV));
        ordo[i] = -I;
    }
    CREDO_VERUM(_rogare(e, c->piscina, "GET /salus", NIHIL, NIHIL,
        ZEPHYRUM, &l[IV], LXIV * MXXIV));
    ordo[IV] = -I;
    per (k = ZEPHYRUM; k < C * M && facti < V; k++)
    {
        _pumpare(e, XX);
        per (i = ZEPHYRUM; i < V; i++)
        {
            si (ordo[i] < ZEPHYRUM && _progredi(&l[i]))
            {
                ordo[i] = (s32)facti++;
            }
        }
    }
    CREDO_AEQUALIS_I32(facti, V);
    CREDO_AEQUALIS_I32(l[IV].status, CC);
    CREDO_VERUM(ordo[IV] < IV);   /* non ultima */
    per (i = ZEPHYRUM; i < V; i++)
    {
        _claudere(&l[i]);
    }
    _finire(e, &locus);
}

/* acta numquam signum crustuli aut tesseram continent */
interior vacuum
_sectio_secreta (CredoContextus* c)
{
                 Locus  locus;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
    constans character* sid;
             character  via[CCLVI];
               Volumen* v;
                   Xar* acta;
                   i32  i;
                   i32  sessiones = ZEPHYRUM;

    CREDO_NECESSE_NON_NIHIL(e);
    sid = _ana(e, c->piscina, &locus);
    CREDO_AEQUALIS_I32((i32)strlen(sid), XLIII);
    exemplar_claudere(e);
    sprintf(via, "%s/exemplar.volumen", locus.via);
    v = volumen_aperire(c->piscina, via);
    CREDO_NECESSE_NON_NIHIL(v);
    acta = volumen_acta_legere(v, ZEPHYRUM, c->piscina);
    CREDO_NECESSE_NON_NIHIL(acta);
    per (i = ZEPHYRUM; i < xar_numerus(acta); i++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, i);
           character* t = chorda_ut_cstr(a->datum, c->piscina);

        CREDO_VERUM(strstr(t, sid) == NIHIL);
        CREDO_VERUM(strstr(t, "tessera-anae-bona") == NIHIL);
        si (chorda_aequalis_literis(a->genus, "sessio.creata"))
        {
            sessiones++;
        }
    }
    CREDO_AEQUALIS_I32(sessiones, I);
    volumen_claudere(v);
    (vacuum)filum_arborem_delere(locus.via);
}

/* ---- sonda MCP/OAuth (mcp-remotus T1) ---- */

#define SONDA_CAPSA (XVI * MXXIV)

nomen structura {
    character textus[SONDA_CAPSA];
          s32 n;
          s32 lineae;
} Capta;

interior vacuum
_capere (
    vacuum* datum,
    chorda  linea)
{
    Capta* c = (Capta*)datum;

    si (c->n + (s32)linea.mensura + II < SONDA_CAPSA)
    {
        memcpy(c->textus + c->n, linea.datum, linea.mensura);
        c->n               += (s32)linea.mensura;
        c->textus[c->n++]  = '\n';
        c->textus[c->n]    = '\0';
    }
    c->lineae++;
}

interior Exemplar*
_exemplar_cum_sonda (
    CredoContextus* c,
             Locus* locus,
               b32  sonda,
             Capta* capta)
{
    ExemplarConfiguratio cfg;
                Exemplar* e;
               character  n[MONETA_NANOID_PLAGULAE_MENSURA];

    (vacuum)moneta_nanoid_plagulae(n);
    sprintf(locus->via, "/tmp/exemplar_%s", n);
    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    memset(capta, ZEPHYRUM, magnitudo(*capta));
    cfg.via_datorum     = locus->via;
    cfg.sonda           = sonda;
    cfg.sonda_scriptor  = _capere;
    cfg.sonda_datum     = capta;
    e                   = exemplar_creare(&cfg, c->piscina);
    si (e)
    {
        sprintf(locus->origo, "Origin: http://127.0.0.1:%u\r\n",
            (insignatus integer)hospitium_portus(
                exemplar_hospitium(e)));
    }
    redde e;
}

/* basis ordinaria sine -basis: http://127.0.0.1:<portus> */
interior constans character*
_basis (
    Exemplar* e,
     Piscina* p)
{
    character* b = (character*)piscina_allocare(p, LXIV);

    sprintf(b, "http://127.0.0.1:%u",
        (insignatus integer)hospitium_portus(exemplar_hospitium(e)));
    redde b;
}

#define INITIUM_RPC \
    "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\"," \
    "\"params\":{\"protocolVersion\":\"2025-06-18\"," \
    "\"capabilities\":{},\"clientInfo\":{\"name\":\"probatio\"," \
    "\"version\":\"0\"}}}"

interior vacuum
_sectio_sonda_401 (CredoContextus* c)
{
       Locus  locus;
       Capta  capta;
    Exemplar* e = _exemplar_cum_sonda(c, &locus, FALSUM, &capta);
      Lector  l;
   character  caput[CCLVI];

    CREDO_NECESSE_NON_NIHIL(e);
    sprintf(caput, "WWW-Authenticate: Bearer resource_metadata=\"%s"
        "/.well-known/oauth-protected-resource\", scope=\"notae:legere "
        "notae:scribere\"\r\n", _basis(e, c->piscina));
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, "POST /mcp",
        "Content-Type: application/json\r\n", INITIUM_RPC, &l));
    CREDO_AEQUALIS_I32(l.status, CDI);
    CREDO_VERUM(_continet(&l, caput));
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, "POST /mcp",
        "Content-Type: application/json\r\n"
        "Authorization: Basic eDp5\r\n",
        INITIUM_RPC, &l));
    CREDO_AEQUALIS_I32(l.status, CDI);
    CREDO_VERUM(_continet(&l, caput));
    _finire(e, &locus);
}

interior b32
_tabulatum_est (
                    JsonValor* t,
    constans character* constans* exspectata,
                           i32  n)
{
    i32 i;

    si (t == NIHIL || json_tabulatum_numerus(t) != n)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (!chorda_aequalis_literis(json_ad_chorda(
            json_tabulatum_obtinere(t, i)), exspectata[i]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior JsonValor*
_json_corporis (
     Lector* l,
    Piscina* p)
{
    JsonResultus r = json_legere(chorda_ex_literis(_corpus(l), p), p);

    redde r.successus ? r.radix : NIHIL;
}

interior vacuum
_sectio_sonda_documenta (CredoContextus* c)
{
                 Locus  locus;
                 Capta  capta;
              Exemplar* e = _exemplar_cum_sonda(c, &locus, FALSUM,
                  &capta);
                Lector  l;
                Lector  m;
             JsonValor* j;
    constans character* basis;
             character  s[CCLVI];
    constans character* scopi[II];
    constans character* scopi_auctoritatis[III];
    constans character* uni[I];

    CREDO_NECESSE_NON_NIHIL(e);
    basis = _basis(e, c->piscina);
    scopi[ZEPHYRUM] = scopi_auctoritatis[ZEPHYRUM] = "notae:legere";
    scopi[I] = scopi_auctoritatis[I] = "notae:scribere";
    scopi_auctoritatis[II] = "offline_access";

    CREDO_VERUM(_rogare_et_legere(e, c->piscina,
        "GET /.well-known/oauth-protected-resource", NIHIL, NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    j = _json_corporis(&l, c->piscina);
    CREDO_NECESSE_NON_NIHIL(j);
    sprintf(s, "%s/mcp", basis);
    CREDO_VERUM(chorda_aequalis_literis(json_capere_chorda(j,
        "resource",
        chorda_ex_literis("", c->piscina)), s));
    uni[ZEPHYRUM] = basis;
    CREDO_VERUM(_tabulatum_est(json_objectum_capere(j,
        "authorization_servers"), uni, I));
    CREDO_VERUM(_tabulatum_est(json_objectum_capere(j,
        "scopes_supported"),
        scopi, II));
    uni[ZEPHYRUM] = "header";
    CREDO_VERUM(_tabulatum_est(json_objectum_capere(j,
        "bearer_methods_supported"), uni, I));
    /* via cum suffixo /mcp: idem documentum */
    CREDO_VERUM(_rogare_et_legere(e, c->piscina,
        "GET /.well-known/oauth-protected-resource/mcp", NIHIL, NIHIL,
        &m));
    CREDO_AEQUALIS_I32(m.status, CC);
    CREDO_VERUM(strcmp(_corpus(&l), _corpus(&m)) == ZEPHYRUM);

    CREDO_VERUM(_rogare_et_legere(e, c->piscina,
        "GET /.well-known/oauth-authorization-server", NIHIL, NIHIL,
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    j = _json_corporis(&l, c->piscina);
    CREDO_NECESSE_NON_NIHIL(j);
    CREDO_VERUM(chorda_aequalis_literis(json_capere_chorda(j, "issuer",
        chorda_ex_literis("", c->piscina)), basis));
    sprintf(s, "%s/oauth/authorize", basis);
    CREDO_VERUM(chorda_aequalis_literis(json_capere_chorda(j,
        "authorization_endpoint", chorda_ex_literis("", c->piscina)),
        s));
    sprintf(s, "%s/oauth/token", basis);
    CREDO_VERUM(chorda_aequalis_literis(json_capere_chorda(j,
        "token_endpoint", chorda_ex_literis("", c->piscina)), s));
    sprintf(s, "%s/oauth/register", basis);
    CREDO_VERUM(chorda_aequalis_literis(json_capere_chorda(j,
        "registration_endpoint", chorda_ex_literis("", c->piscina)),
        s));
    uni[ZEPHYRUM] = "S256";
    CREDO_VERUM(_tabulatum_est(json_objectum_capere(j,
        "code_challenge_methods_supported"), uni, I));
    uni[ZEPHYRUM] = "none";
    CREDO_VERUM(_tabulatum_est(json_objectum_capere(j,
        "token_endpoint_auth_methods_supported"), uni, I));
    uni[ZEPHYRUM] = "code";
    CREDO_VERUM(_tabulatum_est(json_objectum_capere(j,
        "response_types_supported"), uni, I));
    {
        constans character* g[II];

        g[ZEPHYRUM]  = "authorization_code";
        g[I]         = "refresh_token";
        CREDO_VERUM(_tabulatum_est(json_objectum_capere(j,
            "grant_types_supported"), g, II));
    }
    CREDO_VERUM(_tabulatum_est(json_objectum_capere(j,
        "scopes_supported"),
        scopi_auctoritatis, III));
    CREDO_VERUM(json_ad_boolean(json_objectum_capere(j,
        "authorization_response_iss_parameter_supported")));
    _finire(e, &locus);
}

interior vacuum
_sectio_sonda_acta (CredoContextus* c)
{
       Locus  locus;
       Capta  capta;
    Exemplar* e = _exemplar_cum_sonda(c, &locus, VERUM, &capta);
      Lector  l;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, "POST /mcp",
        "Content-Type: application/json\r\n"
        "Authorization: Bearer SECRETUM-TESSERAE\r\n"
        "Cookie: sid=SECRETUM-CRUSTULI\r\n"
        "MCP-Protocol-Version: 2025-06-18\r\n"
        "User-Agent: probatio-sondae/1\r\n", INITIUM_RPC, &l));
    CREDO_AEQUALIS_I32(l.status, CDI);   /* tessera ignota */
    CREDO_AEQUALIS_S32(capta.lineae, I);
    CREDO_VERUM(strstr(capta.textus, "POST /mcp") != NIHIL);
    CREDO_VERUM(strstr(capta.textus, "2025-06-18") != NIHIL);
    CREDO_VERUM(strstr(capta.textus, "probatio-sondae/1") != NIHIL);
    /* corpus /mcp ut compendium: methodus, versio, cliens - numquam
     * argumenta (textus notarum) */
    CREDO_VERUM(strstr(capta.textus, "methodus=initialize") != NIHIL);
    CREDO_VERUM(strstr(capta.textus, "cliens=probatio/0") != NIHIL);
    CREDO_VERUM(strstr(capta.textus, "SECRETUM") == NIHIL);
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, "POST /mcp",
        "Content-Type: application/json\r\n"
        "Authorization: Bearer SECRETUM-TESSERAE\r\n",
        "{\"jsonrpc\":\"2.0\",\"id\":9,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"notam_addere\",\"arguments\":"
        "{\"textus\":\"TEXTUS-PRIVATUS\"}}}", &l));
    CREDO_AEQUALIS_S32(capta.lineae, II);
    CREDO_VERUM(strstr(capta.textus, "instrumentum=notam_addere")
        != NIHIL);
    CREDO_VERUM(strstr(capta.textus, "TEXTUS-PRIVATUS") == NIHIL);
    /* via non MCP: nihil scriptum */
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, "GET /salus", NIHIL,
        NIHIL, &l));
    CREDO_AEQUALIS_S32(capta.lineae, II);
    _finire(e, &locus);

    /* sonda clausa: nihil omnino */
    e = _exemplar_cum_sonda(c, &locus, FALSUM, &capta);
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, "POST /mcp",
        "Content-Type: application/json\r\n", INITIUM_RPC, &l));
    CREDO_AEQUALIS_S32(capta.lineae, ZEPHYRUM);
    _finire(e, &locus);
}

interior vacuum
_sectio_sonda_register (CredoContextus* c)
{
       Locus  locus;
       Capta  capta;
    Exemplar* e = _exemplar_cum_sonda(c, &locus, VERUM, &capta);
      Lector  l;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, "POST /oauth/register",
        "Content-Type: application/json\r\n",
        "{\"client_name\":\"Claude\",\"redirect_uris\":"
        "[\"https://claude.ai/api/mcp/auth_callback\"]}", &l));
    CREDO_AEQUALIS_I32(l.status, CCI);
    CREDO_VERUM(_continet(&l, "\"client_id\""));
    CREDO_AEQUALIS_S32(capta.lineae, I);
    CREDO_VERUM(strstr(capta.textus, "auth_callback") != NIHIL);
    _finire(e, &locus);
}


/* ==================================================
 * SERVUS MCP TERTIUS (mcp-remotus T7)
 * ================================================== */

#define CALLBACK    "https://claude.ai/api/mcp/auth_callback"
#define VERIFICATOR "dBjftJeZ4CVP-mB92K27uhbUJU1p1r_wW1gFWFOEjXk"
#define PROVOCATIO  "E9Melhoa2OwvFrEMTJguCHaoeK1t8URWbuGJSstw-cM"
#define TESSERA     "tessera-bona-longa"

interior constans character*
_codificare (
    constans character* s,
               Piscina* p)
{
    redde chorda_ut_cstr(url_codificare(chorda_ex_literis(s, p), p), p);
}

/* usor inscriptus et intratus: signum sid aut "" */
interior constans character*
_usor_novus (
              Exemplar* e,
               Piscina* p,
                 Locus* locus,
    constans character* nomen_usoris)
{
    i32 status;

    si (   !_inscribere(e, p, locus, nomen_usoris, TESSERA,
               _invitatio(e, p), &status) || status != CCCIII)
    {
        redde "";
    }
    redde _intrare(e, p, locus, nomen_usoris, TESSERA, &status);
}

/* parametra authorize pro cliente */
interior character*
_quaestio_auctorizandi (
              Exemplar* e,
               Piscina* p,
    constans character* cliens)
{
    character* q = (character*)piscina_allocare(p, MMXLVIII);
    character  audientia[CXXVIII];

    sprintf(audientia, "%s/mcp", _basis(e, p));
    sprintf(q, "response_type=code&client_id=%s&redirect_uri=%s"
        "&code_challenge=%s&code_challenge_method=S256&scope=%s"
        "&state=st&resource=%s", cliens, _codificare(CALLBACK, p),
        PROVOCATIO,
        _codificare("notae:legere notae:scribere offline_access", p),
        _codificare(audientia, p));
    redde q;
}

interior constans character*
_clientem_inscribere (
     Exemplar* e,
      Piscina* p)
{
    Lector l;

    si (   !_rogare_et_legere(e, p, "POST /oauth/register",
               "Content-Type: application/json\r\n",
               "{\"redirect_uris\":[\"" CALLBACK "\"],\"client_name\":"
               "\"Claude\",\"token_endpoint_auth_method\":\"none\"}",
               &l)
        || l.status != CCI)
    {
        redde "";
    }
    redde _extrahere(&l, "\"client_id\":\"", '"', p);
}

/* signum intrandi totum OAuth: cliens, accessus, renovatio */
interior b32
_connectere (
              Exemplar*  e,
               Piscina*  p,
                 Locus*  locus,
    constans character*  sid,
    constans character** cliens,
    constans character** accessus,
    constans character** renovatio)
{
                Lector  l;
             character* q;
             character* linea;
             character* corpus;
    constans character* csrf;
    constans character* codex;
             character  audientia[CXXVIII];

    *cliens = _clientem_inscribere(e, p);
    si (!**cliens)
    {
        redde FALSUM;
    }
    q       = _quaestio_auctorizandi(e, p, *cliens);
    linea   = (character*)piscina_allocare(p, IV * MXXIV);
    corpus  = (character*)piscina_allocare(p, IV * MXXIV);
    sprintf(linea, "GET /oauth/authorize?%s", q);
    si (   !_rogare_et_legere(e, p, linea, _capita(p, locus, sid,
        NIHIL),
               NIHIL, &l) || l.status != CC)
    {
        redde FALSUM;
    }
    csrf = _extrahere(&l, "name=\"csrf\" value=\"", '"', p);
    sprintf(corpus, "%s&s0=1&s1=1&csrf=%s&decisio=permittere", q, csrf);
    si (   !_rogare_et_legere(e, p, "POST /oauth/authorize",
               _capita(p, locus, sid, FORMA_U), corpus, &l)
        || l.status != CCCII)
    {
        redde FALSUM;
    }
    codex = _extrahere(&l, "code=", '&', p);
    sprintf(audientia, "%s/mcp", _basis(e, p));
    sprintf(corpus,
        "grant_type=authorization_code&code=%s&redirect_uri=%s"
        "&client_id=%s&code_verifier=%s&resource=%s", codex,
        _codificare(CALLBACK, p), *cliens, VERIFICATOR,
        _codificare(audientia, p));
    si (   !_rogare_et_legere(e, p, "POST /oauth/token",
               FORMA_U "\r\n", corpus, &l) || l.status != CC)
    {
        redde FALSUM;
    }
    *accessus   = _extrahere(&l, "\"access_token\":\"", '"', p);
    *renovatio  = _extrahere(&l, "\"refresh_token\":\"", '"', p);
    redde **accessus && **renovatio;
}

/* tools/call per POST /mcp */
interior b32
_instrumentum (
               Exemplar* e,
                Piscina* p,
     constans character* accessus,
     constans character* titulus,
     constans character* argumenta,
                 Lector* l)
{
    character  capita[CCLVI];
    character* corpus = (character*)piscina_allocare(p,
        (memoriae_index)strlen(argumenta) + CCLVI);

    sprintf(capita, "Content-Type: application/json\r\n"
        "Authorization: Bearer %s\r\n", accessus);
    sprintf(corpus, "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":"
        "\"tools/call\",\"params\":{\"name\":\"%s\",\"arguments\":%s}}",
        titulus, argumenta);
    redde _rogare_et_legere(e, p, "POST /mcp", capita, corpus, l);
}

interior JsonValor*
_resultatum (
     Lector* l,
    Piscina* p)
{
    redde json_via(_json_corporis(l, p), "result");
}

interior b32
_vitium (
     Lector* l,
    Piscina* p)
{
    redde json_ad_boolean(json_via(_json_corporis(l, p),
        "result.isError"));
}

interior s64
_id_novum (
              Exemplar* e,
               Piscina* p,
    constans character* accessus,
    constans character* textus_json)
{
         Lector  l;
      character* arg = (character*)piscina_allocare(p,
          (memoriae_index)strlen(textus_json) + XXXII);

    sprintf(arg, "{\"textus\":%s}", textus_json);
    si (   !_instrumentum(e, p, accessus, "notam_addere", arg, &l)
        || l.status != CC || _vitium(&l, p))
    {
        redde ZEPHYRUM;
    }
    redde json_ad_integer(json_via(_resultatum(&l, p),
        "structuredContent.id"));
}

/* actum generis cum fragmentis (volumen lectum per connexionem
 * alteram - WAL lectores tolerat) */
interior b32
_actum_est (
                 Locus* locus,
               Piscina* p,
    constans character* genus,
    constans character* fragmentum_a,
    constans character* fragmentum_b)
{
    character  via[CCLVI];
      Volumen* v;
          Xar* acta;
          i32  k;
          b32  inventum = FALSUM;

    sprintf(via, "%s/exemplar.volumen", locus->via);
    v = volumen_aperire(p, via);
    si (!v)
    {
        redde FALSUM;
    }
    acta = volumen_acta_legere(v, ZEPHYRUM, p);
    per (k = ZEPHYRUM; acta && k < xar_numerus(acta); k++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, k);

        si (   chorda_aequalis_literis(a->genus, genus)
            && (!fragmentum_a || chorda_continet(a->datum,
                   chorda_ex_literis(fragmentum_a, p)))
            && (!fragmentum_b || chorda_continet(a->datum,
                   chorda_ex_literis(fragmentum_b, p))))
        {
            inventum = VERUM;
        }
    }
    volumen_claudere(v);
    redde inventum;
}

/* fragmentum "usor":"<ULID civis>" (ianua T5: concessiones per ULID) */
interior constans character*
_usor_civis (
                 Locus* locus,
               Piscina* p,
    constans character* nomen_usoris)
{
               character  via[CCLVI];
                 Volumen* v;
                   Cives* cv;
                   Civis  x;
               character* f = (character*)piscina_allocare(p, LXIV);

    sprintf(via, "%s/exemplar.volumen", locus->via);
    v   = volumen_aperire(p, via);
    cv  = v ? cives_aperire(v, NIHIL, p) : NIHIL;
    strcpy(f, "\"usor\":\"?\"");
    si (   cv
        && cives_per_nomen(cv, chorda_ex_literis(nomen_usoris, p), &x,
        p))
    {
        sprintf(f, "\"usor\":\"%.*s\"", (integer)x.id.mensura,
            (constans character*)x.id.datum);
    }
    si (v)
    {
        volumen_claudere(v);
    }
    redde f;
}

interior vacuum
_sectio_instrumenta (CredoContextus* c)
{
                 Locus  locus;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
               Piscina* p = c->piscina;
    constans character* sid;
    constans character* cliens;
    constans character* accessus;
    constans character* renovatio;
                Lector  l;
             JsonValor* r;
                   s64  id;
                   s64  id_fili;
             character  arg[CXXVIII];
             character  frag[CXXVIII];

    CREDO_NECESSE_NON_NIHIL(e);
    sid = _usor_novus(e, p, &locus, "fran");
    CREDO_NECESSE(strlen(sid) > ZEPHYRUM);
    CREDO_NECESSE(_connectere(e, p, &locus, sid, &cliens, &accessus,
        &renovatio));
    CREDO_VERUM(_actum_est(&locus, p, "concessio.creata",
        "\"titulus_clientis\":\"Claude\"",
        "\"scopi\":\"notae:legere notae:scribere offline_access\""));
    CREDO_VERUM(_actum_est(&locus, p, "concessio.creata",
        _usor_civis(&locus, p, "fran"), NIHIL));
    /* notam_addere: actum via claude cum cliente */
    id = _id_novum(e, p, accessus, "\"prima nota\"");
    CREDO_MAIOR_S32((s32)id, ZEPHYRUM);
    sprintf(frag, "\"cliens\":\"%s\"", cliens);
    CREDO_VERUM(_actum_est(&locus, p, "nota.scripta",
        "\"via\":\"claude\"",
        frag));
    /* nota per paginam cum filo */
    CREDO_AEQUALIS_I32(_notam_mittere(e, p, &locus, sid,
        _csrf_paginae(e, p,
        &locus, sid), "cum filo", (constans i8*)"octeti", VI, NIHIL),
        CCCIII);
    /* notas_enumerare: recentissima prima */
    CREDO_VERUM(_instrumentum(e, p, accessus, "notas_enumerare", "{}",
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    r = _resultatum(&l, p);
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_via(r,
        "structuredContent.notae.0.textus")), "cum filo");
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_via(r,
        "structuredContent.notae.1.textus")), "prima nota");
    CREDO_AEQUALIS_S32((s32)json_ad_integer(json_via(r,
        "structuredContent.pagina")), I);
    id_fili = json_ad_integer(json_via(r,
        "structuredContent.notae.0.id"));
    /* notam_legere: textus + nomen et mensura fili */
    sprintf(arg, "{\"id\":%ld}", (longus)id_fili);
    CREDO_VERUM(_instrumentum(e, p, accessus, "notam_legere", arg, &l));
    r = _resultatum(&l, p);
    CREDO_FALSUM(_vitium(&l, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_via(r,
        "structuredContent.textus")), "cum filo");
    CREDO_AEQUALIS_S32((s32)json_ad_integer(json_via(r,
        "structuredContent.filum.mensura")), VI);
    CREDO_VERUM(json_ad_chorda(json_via(r,
        "structuredContent.filum.titulus")).mensura > ZEPHYRUM);
    /* notam_delere */
    sprintf(arg, "{\"id\":%ld}", (longus)id);
    CREDO_VERUM(_instrumentum(e, p, accessus, "notam_delere", arg, &l));
    CREDO_FALSUM(_vitium(&l, p));
    CREDO_VERUM(_instrumentum(e, p, accessus, "notam_legere", arg, &l));
    CREDO_VERUM(_vitium(&l, p));
    CREDO_VERUM(_continet(&l, "nota non inventa"));
    _finire(e, &locus);
}

interior vacuum
_sectio_alieni (CredoContextus* c)
{
                 Locus  locus;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
               Piscina* p = c->piscina;
    constans character* cliens;
    constans character* accessus_f;
    constans character* accessus_a;
    constans character* renovatio;
                Lector  l;
                   s64  id;
             character  arg[CXXVIII];

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_NECESSE(_connectere(e, p, &locus, _usor_novus(e, p, &locus,
        "fran"), &cliens, &accessus_f, &renovatio));
    CREDO_NECESSE(_connectere(e, p, &locus, _usor_novus(e, p, &locus,
        "amanda"), &cliens, &accessus_a, &renovatio));
    id = _id_novum(e, p, accessus_f, "\"secretum franisci\"");
    CREDO_MAIOR_S32((s32)id, ZEPHYRUM);
    sprintf(arg, "{\"id\":%ld}", (longus)id);
    CREDO_VERUM(_instrumentum(e, p, accessus_a, "notam_legere", arg,
        &l));
    CREDO_VERUM(_vitium(&l, p));
    CREDO_VERUM(_continet(&l, "nota non inventa"));
    CREDO_FALSUM(_continet(&l, "secretum franisci"));
    CREDO_VERUM(_instrumentum(e, p, accessus_a, "notas_enumerare", "{}",
        &l));
    CREDO_FALSUM(_continet(&l, "secretum franisci"));
    CREDO_AEQUALIS_I32(json_tabulatum_numerus(json_via(_resultatum(&l,
        p),
        "structuredContent.notae")), ZEPHYRUM);
    CREDO_VERUM(_instrumentum(e, p, accessus_a, "notam_delere", arg,
        &l));
    CREDO_VERUM(_vitium(&l, p));
    CREDO_VERUM(_instrumentum(e, p, accessus_f, "notam_legere", arg,
        &l));
    CREDO_FALSUM(_vitium(&l, p));
    CREDO_VERUM(_continet(&l, "secretum franisci"));
    _finire(e, &locus);
}

/* Review focus 3: revocatio in /applicationes dum Claude laborat */
interior vacuum
_sectio_revocare (CredoContextus* c)
{
                 Locus  locus;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
               Piscina* p = c->piscina;
    constans character* sid;
    constans character* cliens_f;
    constans character* cliens_a;
    constans character* accessus_f;
    constans character* accessus_a;
    constans character* renovatio_f;
    constans character* renovatio_a;
    constans character* concessio;
    constans character* csrf;
                Lector  l;
             character  linea[CCLVI];
             character  corpus[CCCLXXXIV];

    CREDO_NECESSE_NON_NIHIL(e);
    sid = _usor_novus(e, p, &locus, "fran");
    CREDO_NECESSE(_connectere(e, p, &locus, sid, &cliens_f, &accessus_f,
        &renovatio_f));
    CREDO_NECESSE(_connectere(e, p, &locus, _usor_novus(e, p, &locus,
        "amanda"), &cliens_a, &accessus_a, &renovatio_a));
    CREDO_VERUM(_rogare_et_legere(e, p, "GET /applicationes", _capita(p,
        &locus, sid, NIHIL), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "Claude"));
    concessio  = _extrahere(&l, "action=\"/applicationes/", '/', p);
    csrf       = _extrahere(&l, "name=\"csrf\" value=\"", '"', p);
    CREDO_NECESSE(strlen(concessio) > ZEPHYRUM);
    /* CSRF absens: 403, concessio viva */
    sprintf(linea, "POST /applicationes/%s/revocare", concessio);
    CREDO_VERUM(_rogare_et_legere(e, p, linea, _capita(p, &locus, sid,
        FORMA_U), "csrf=vetus", &l));
    CREDO_AEQUALIS_I32(l.status, CDIII);
    CREDO_VERUM(_instrumentum(e, p, accessus_f, "notas_enumerare", "{}",
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    sprintf(corpus, "csrf=%s", csrf);
    CREDO_VERUM(_rogare_et_legere(e, p, linea, _capita(p, &locus, sid,
        FORMA_U), corpus, &l));
    CREDO_AEQUALIS_I32(l.status, CCCIII);
    CREDO_VERUM(_actum_est(&locus, p, "concessio.revocata",
        _usor_civis(&locus, p, "fran"), NIHIL));
    /* petitio proxima Franisci: 401; renovatio: invalid_grant */
    CREDO_VERUM(_instrumentum(e, p, accessus_f, "notas_enumerare", "{}",
        &l));
    CREDO_AEQUALIS_I32(l.status, CDI);
    sprintf(corpus, "grant_type=refresh_token&refresh_token=%s"
        "&client_id=%s", renovatio_f, cliens_f);
    CREDO_VERUM(_rogare_et_legere(e, p, "POST /oauth/token",
        FORMA_U "\r\n",
        corpus, &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    CREDO_VERUM(_continet(&l, "invalid_grant"));
    /* Amanda intacta */
    CREDO_VERUM(_instrumentum(e, p, accessus_a, "notas_enumerare", "{}",
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_FALSUM(_vitium(&l, p));
    _finire(e, &locus);
}

/* POST /intrare cum redire; Location */
interior constans character*
_intrare_redire (
              Exemplar* e,
               Piscina* p,
                 Locus* locus,
    constans character* redire)
{
       Lector l;
    character corpus[MMXLVIII];

    sprintf(corpus, "nomen=fran&tessera=" TESSERA "&redire=%s",
        _codificare(redire, p));
    si (   !_rogare_et_legere(e, p, "POST /intrare", _capita(p, locus,
               NIHIL, FORMA_U), corpus, &l) || l.status != CCCIII)
    {
        redde "";
    }
    redde _extrahere(&l, "Location: ", '\r', p);
}

interior vacuum
_sectio_redire (CredoContextus* c)
{
                 Locus  locus;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
               Piscina* p = c->piscina;
                Lector  l;
                   i32  status;
    constans character* cliens;
             character* reditus;
             character* linea;
    constans character* locus_novus;
    constans character* sid;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(_inscribere(e, p, &locus, "fran", TESSERA, _invitatio(e,
        c->piscina),
        &status));
    cliens   = _clientem_inscribere(e, p);
    reditus  = (character*)piscina_allocare(p, MMXLVIII);
    linea    = (character*)piscina_allocare(p, IV * MXXIV);
    sprintf(reditus, "/oauth/authorize?%s", _quaestio_auctorizandi(e, p,
        cliens));
    /* pagina intrandi campum redire servat */
    sprintf(linea, "GET /intrare?redire=%s", _codificare(reditus, p));
    CREDO_VERUM(_rogare_et_legere(e, p, linea, NIHIL, NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l,
        "name=\"redire\" value=\"/oauth/authorize?"));
    locus_novus = _intrare_redire(e, p, &locus, reditus);
    CREDO_VERUM(strcmp(locus_novus, reditus) == ZEPHYRUM);
    /* ad paginam consensus pervenit */
    sid = _intrare(e, p, &locus, "fran", TESSERA, &status);
    sprintf(linea, "GET %s", locus_novus);
    CREDO_VERUM(_rogare_et_legere(e, p, linea, _capita(p, &locus, sid,
        NIHIL), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "Permissio"));
    /* redirectiones alienae: /notae */
    CREDO_VERUM(strcmp(_intrare_redire(e, p, &locus,
        "https://evil.example/"), "/notae") == ZEPHYRUM);
    CREDO_VERUM(strcmp(_intrare_redire(e, p, &locus, "//evil.example"),
        "/notae") == ZEPHYRUM);
    CREDO_VERUM(strcmp(_intrare_redire(e, p, &locus, "/\\evil.example"),
        "/notae") == ZEPHYRUM);
    _finire(e, &locus);
}

interior vacuum
_sectio_textus_varius (CredoContextus* c)
{
                 Locus  locus;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
               Piscina* p = c->piscina;
    constans character* cliens;
    constans character* accessus;
    constans character* renovatio;
    constans character* textus = "Gr\xc3\xbc\xc3\x9f" "e \xe2\x80\x93 "
        "\xe6\xb5\x8b\xe8\xaf\x95";
             character json[CXXVIII];
             character arg[LXIV];
                Lector l;
                   s64 id;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_NECESSE(_connectere(e, p, &locus, _usor_novus(e, p, &locus,
        "fran"), &cliens, &accessus, &renovatio));
    sprintf(json, "\"%s\"", textus);
    id = _id_novum(e, p, accessus, json);
    CREDO_MAIOR_S32((s32)id, ZEPHYRUM);
    sprintf(arg, "{\"id\":%ld}", (longus)id);
    CREDO_VERUM(_instrumentum(e, p, accessus, "notam_legere", arg, &l));
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_via(
        _resultatum(&l, p), "structuredContent.textus")), textus);
    _finire(e, &locus);
}

interior vacuum
_sectio_paginae (CredoContextus* c)
{
                 Locus  locus;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
               Piscina* p = c->piscina;
    constans character* cliens;
    constans character* accessus;
    constans character* renovatio;
             character  json[XXXII];
             character  arg[XXXII];
                Lector  l;
                   i32  k;
                   i32  numeri[IV];

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_NECESSE(_connectere(e, p, &locus, _usor_novus(e, p, &locus,
        "fran"), &cliens, &accessus, &renovatio));
    per (k = I; k <= XLV; k++)
    {
        sprintf(json, "\"nota %u\"", (insignatus integer)k);
        CREDO_NECESSE(_id_novum(e, p, accessus, json) > ZEPHYRUM);
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        sprintf(arg, "{\"pagina\":%u}", (insignatus integer)(k + I));
        CREDO_VERUM(_instrumentum(e, p, accessus, "notas_enumerare",
            arg,
            &l));
        numeri[k] = json_tabulatum_numerus(json_via(_resultatum(&l, p),
            "structuredContent.notae"));
        si (k == ZEPHYRUM)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_via(
                _resultatum(&l, p),
                "structuredContent.notae.0.textus")),
                "nota 45");
        }
    }
    CREDO_AEQUALIS_I32(numeri[ZEPHYRUM], XX);
    CREDO_AEQUALIS_I32(numeri[I], XX);
    CREDO_AEQUALIS_I32(numeri[II], V);
    CREDO_AEQUALIS_I32(numeri[III], ZEPHYRUM);
    _finire(e, &locus);
}

/* ---- migratio civium (ianua T5) ---- */

nomen structura {
          i32  venit;
       chorda  phc;
      Piscina* p;
} HashCaptum;

interior vacuum
_hash_captum (
    vacuum* datum,
    chorda  phc)
{
    HashCaptum* h = (HashCaptum*)datum;

    h->phc.mensura  = phc.mensura;
    h->phc.datum    = (i8*)piscina_allocare(h->p, (i64)phc.mensura + I);
    memcpy(h->phc.datum, phc.datum, (size_t)phc.mensura);
    h->venit++;
}

/* phc argon2id (parametri ordinarii, ut exemplar) */
interior chorda
_phc (
        CredoContextus* c,
    constans character* tessera)
{
     CustodiaConfiguratio  cfg;
                  Reactor* r = reactor_creare(c->piscina);
                 Custodia* custos;
               HashCaptum  h;
                      i32  k;

    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    memset(cfg.secretum, V, CUSTODIA_SECRETUM);
    memset(&h, ZEPHYRUM, magnitudo(h));
    h.p     = c->piscina;
    custos  = custodia_creare(&cfg, r, c->piscina);
    si (   custos
        && custodia_tesseram_digerere(custos, chorda_ex_literis(tessera,
        c->piscina), _hash_captum, &h))
    {
        per (k = ZEPHYRUM; k < C * M && h.venit == ZEPHYRUM; k++)
        {
            reactor_poll(r, I);
        }
    }
    reactor_destruere(r);
    redde h.phc;
}

interior vacuum
_actum_vetus (
               Volumen* v,
    constans character* genus,
    constans character* forma,
    constans character* a,
    constans character* b,
               Piscina* p)
{
    character* d = (character*)piscina_allocare(p, (i64)(strlen(forma)
        + strlen(a) + strlen(b) + XVI));

    sprintf(d, forma, a, b);
    (vacuum)volumen_actum_appendere(v, genus, chorda_ex_literis(d, p));
}

hic_manens constans character* constans SCOPI_VETERES[] = {
    "notae:legere", "notae:scribere", NIHIL
};

/* volumen ANTE ianuam: usores per acta cum phc, notae per nomen,
 * concessio Claudii pro "fran"; conflictus: civis "amanda" iam est */
interior b32
_volumen_vetus (
    CredoContextus* c,
             Locus* locus,
               b32  conflictus)
{
             Piscina* p = c->piscina;
           character  n[MONETA_NANOID_PLAGULAE_MENSURA];
           character  via[CCLVI];
             Volumen* v;
    AdmissioOptiones  o;
            Admissio* a;
              chorda  responsum;
              chorda  cliens;
              chorda  accessus;
              chorda  renovatio;
        JsonResultus  r;

    (vacuum)moneta_nanoid_plagulae(n);
    sprintf(locus->via, "/tmp/exemplar_%s", n);
    sprintf(via, "%s/exemplar.volumen", locus->via);
    si (   !filum_directorium_creare_cum_modo(locus->via, 0700)
        || !(v = volumen_aperire_aut_creare(p, via)))
    {
        redde FALSUM;
    }
    _actum_vetus(v, "usor.creatus", "{\"usor\":\"%s\",\"phc\":\"%s\"}",
        "fran", chorda_ut_cstr(_phc(c, "tessera-vetus"), p), p);
    _actum_vetus(v, "tessera.mutata",
        "{\"usor\":\"%s\",\"phc\":\"%s\"}",
        "fran", chorda_ut_cstr(_phc(c, TESSERA), p), p);
    _actum_vetus(v, "usor.creatus", "{\"usor\":\"%s\",\"phc\":\"%s\"}",
        "amanda", chorda_ut_cstr(_phc(c, "tessera-amandae"), p), p);
    _actum_vetus(v, "nota.scripta",
        "{\"usor\":\"%s\",\"textus\":\"%s\","
        "\"via\":\"retis\",\"sigillum\":\"\"}", "fran",
        "nota franisci vetus", p);
    _actum_vetus(v, "nota.scripta",
        "{\"usor\":\"%s\",\"textus\":\"%s\","
        "\"via\":\"retis\",\"sigillum\":\"\"}", "amanda",
        "nota amandae vetus", p);
    _actum_vetus(v, "sessio.creata",
        "{\"sigillum\":\"%s\",\"usor\":\"%s\","
        "\"expirat\":9999999999999}", "00ff", "fran", p);
    memset(&o, ZEPHYRUM, magnitudo(o));
    o.auctoritas  = "http://127.0.0.1";
    o.audientia   = "http://127.0.0.1/mcp";
    o.scopi       = SCOPI_VETERES;
    a             = admissio_creare(v, &o, p);
    si (   !a || admissio_inscribere(a, chorda_ex_literis(
            "{\"redirect_uris\":[\"https://claude.ai/api/mcp/auth_callback\"],"
            "\"client_name\":\"Claude\",\"token_endpoint_auth_method\":"
            "\"none\"}", p), chorda_ex_literis("127.0.0.1", p),
            &responsum,
            p) != CCI)
    {
        volumen_claudere(v);
        redde FALSUM;
    }
    r       = json_legere(responsum, p);
    cliens  = json_capere_chorda(r.radix, "client_id",
        chorda_ex_literis("",
        p));
    si (!admissio_concessionem_creare_probationis(a, chorda_ex_literis(
        "fran", p), cliens,
        chorda_ex_literis("notae:legere notae:scribere",
        p), &accessus, &renovatio, p))
    {
        volumen_claudere(v);
        redde FALSUM;
    }
    si (conflictus)
    {
         Cives* cv = cives_aperire(v, NIHIL, p);
        chorda  id;

        si (   !cv || !volumen_transactionem_incipere(v)
            || !cives_creare(cv, chorda_ex_literis("amanda", p),
                   chorda_ex_literis("Alia", p), &id, p)
            || !volumen_transactionem_committere(v))
        {
            volumen_claudere(v);
            redde FALSUM;
        }
    }
    (vacuum)scrinium_exsequi(volumen_scrinium(v),
        "PRAGMA wal_checkpoint(TRUNCATE)");
    volumen_claudere(v);
    redde VERUM;
}

interior i32
_acta_generis (
               Volumen* v,
    constans character* genus,
               Piscina* p)
{
    Xar* acta  = volumen_acta_legere(v, ZEPHYRUM, p);
    i32  n     = ZEPHYRUM;
    i32  k;

    per (k = ZEPHYRUM; acta && k < xar_numerus(acta); k++)
    {
        si (chorda_aequalis_literis(((VolumenActum*)xar_obtinere(acta,
            k))->genus, genus))
        {
            n++;
        }
    }
    redde n;
}

interior Volumen*
_volumen_loci (
    CredoContextus* c,
             Locus* locus)
{
    character via[CCLVI];

    sprintf(via, "%s/exemplar.volumen", locus->via);
    redde volumen_aperire(c->piscina, via);
}

interior vacuum
_sectio_migratio (CredoContextus* c)
{
                 Locus  locus;
               Piscina* p = c->piscina;
              Exemplar* e;
    constans character* sid;
                Lector  l;
                   i32  status;

    CREDO_NECESSE(_volumen_vetus(c, &locus, FALSUM));
    e = _exemplar(c, &locus, ZEPHYRUM, FALSUM);
    CREDO_NECESSE_NON_NIHIL(e);
    /* tessera ULTIMA (tessera.mutata) valet, vetus non */
    sid = _intrare(e, p, &locus, "fran", TESSERA, &status);
    CREDO_AEQUALIS_I32(status, CCCIII);
    (vacuum)_intrare(e, p, &locus, "fran", "tessera-vetus", &status);
    CREDO_AEQUALIS_I32(status, CDIII);
    (vacuum)_intrare(e, p, &locus, "amanda", "tessera-amandae",
        &status);
    CREDO_AEQUALIS_I32(status, CCCIII);
    /* notae per mappam nominum veterum */
    CREDO_VERUM(_rogare_et_legere(e, p, "GET /notae", _capita(p, &locus,
        sid, NIHIL), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "nota franisci vetus"));
    CREDO_FALSUM(_continet(&l, "nota amandae vetus"));
    /* concessio Claudii re-clavata */
    CREDO_VERUM(_rogare_et_legere(e, p, "GET /applicationes", _capita(p,
        &locus, sid, NIHIL), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "Claude"));
    _finire(e, &locus);
}

interior vacuum
_sectio_migratio_iterata (CredoContextus* c)
{
                 Locus  locus;
               Piscina* p = c->piscina;
              Exemplar* e;
               Volumen* v;
    constans character* sid;
                Lector  l;
                   i32  status;

    CREDO_NECESSE(_volumen_vetus(c, &locus, FALSUM));
    e = _exemplar(c, &locus, ZEPHYRUM, FALSUM);
    CREDO_NECESSE_NON_NIHIL(e);
    exemplar_claudere(e);
    e = _exemplar(c, &locus, ZEPHYRUM, FALSUM);
    CREDO_NECESSE_NON_NIHIL(e);
    sid = _intrare(e, p, &locus, "fran", TESSERA, &status);
    CREDO_AEQUALIS_I32(status, CCCIII);
    CREDO_VERUM(_rogare_et_legere(e, p, "GET /notae", _capita(p, &locus,
        sid, NIHIL), NIHIL, &l));
    CREDO_VERUM(_continet(&l, "nota franisci vetus"));
    exemplar_claudere(e);
    v = _volumen_loci(c, &locus);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_AEQUALIS_I32(_acta_generis(v, "exemplar.migratio", p), I);
    CREDO_AEQUALIS_I32(_acta_generis(v, "civis.creatus", p), II);
    volumen_claudere(v);
    (vacuum)filum_arborem_delere(locus.via);
}

interior vacuum
_sectio_migratio_fracta (CredoContextus* c)
{
                 Locus  locus;
               Piscina* p = c->piscina;
              Exemplar* e;
               Volumen* v;
                 Cives* cv;
                 Civis  x;
                   i32  status;

    CREDO_NECESSE(_volumen_vetus(c, &locus, VERUM));
    e = _exemplar(c, &locus, ZEPHYRUM, FALSUM);
    CREDO_NIHIL(e);
    /* nihil mutatum: fran non civis, nulla migratio, nullum actum */
    v = _volumen_loci(c, &locus);
    CREDO_NECESSE_NON_NIHIL(v);
    cv = cives_aperire(v, NIHIL, p);
    CREDO_NECESSE_NON_NIHIL(cv);
    CREDO_FALSUM(cives_per_nomen(cv, chorda_ex_literis("fran", p), &x,
        p));
    CREDO_AEQUALIS_I32(_acta_generis(v, "exemplar.migratio", p),
        ZEPHYRUM);
    CREDO_NECESSE(scrinium_exsequi(volumen_scrinium(v),
        "DELETE FROM cives WHERE nomen_usoris = 'amanda'"));
    CREDO_NECESSE(scrinium_exsequi(volumen_scrinium(v),
        "DELETE FROM cives_nomina WHERE nomen_usoris = 'amanda'"));
    volumen_claudere(v);
    /* conflictu sublato: migratio procedit (sera liberata erat) */
    e = _exemplar(c, &locus, ZEPHYRUM, FALSUM);
    CREDO_NECESSE_NON_NIHIL(e);
    (vacuum)_intrare(e, p, &locus, "amanda", "tessera-amandae",
        &status);
    CREDO_AEQUALIS_I32(status, CCCIII);
    _finire(e, &locus);
}

interior vacuum
_sectio_sera (CredoContextus* c)
{
       Locus  locus;
     Piscina* p = c->piscina;
    Exemplar* e;
    Exemplar* alter;
      chorda  codex;
         i32  status;
   character  via_canalis[CCLVI];

    e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
    CREDO_NECESSE_NON_NIHIL(e);
    alter = _exemplar(c, &locus, ZEPHYRUM, FALSUM);
    CREDO_NIHIL(alter);
    /* servus currens per canalem respondet (invitare_currens); hic
     * sera tenetur sed nemo in canali audit (socket remotum) -> II
     * (canalis P9). Servus non pumpatur: vocatio obstruens alioquin
     * XXX s exspectaret */
    sprintf(via_canalis, "%s/canalis/servus", locus.via);
    CREDO_NECESSE(filum_delere(via_canalis));
    CREDO_AEQUALIS_S32(exemplar_invitare(locus.via, "prima", &codex, p),
        II);
    exemplar_claudere(e);
    CREDO_AEQUALIS_S32(exemplar_invitare(locus.via, "prima", &codex, p),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(codex.mensura, XXII);
    e = _exemplar(c, &locus, ZEPHYRUM, FALSUM);
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(_inscribere(e, p, &locus, "fran", TESSERA,
        chorda_ut_cstr(codex, p), &status));
    CREDO_AEQUALIS_I32(status, CCCIII);
    _finire(e, &locus);
}

interior vacuum
_sectio_mcp_nota_civis (CredoContextus* c)
{
                 Locus  locus;
               Piscina* p = c->piscina;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
    constans character* cliens;
    constans character* accessus;
    constans character* renovatio;
               Volumen* v;
                   Xar* acta;
                   i32  k;
                   i32  inventae = ZEPHYRUM;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_NECESSE(_connectere(e, p, &locus, _usor_novus(e, p, &locus,
        "fran"), &cliens, &accessus, &renovatio));
    CREDO_NECESSE(_id_novum(e, p, accessus, "\"per claudium\"")
        > ZEPHYRUM);
    exemplar_claudere(e);
    v = _volumen_loci(c, &locus);
    CREDO_NECESSE_NON_NIHIL(v);
    acta = volumen_acta_legere(v, ZEPHYRUM, p);
    per (k = ZEPHYRUM; acta && k < xar_numerus(acta); k++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, k);
        JsonResultus  r;

        si (!chorda_aequalis_literis(a->genus, "nota.scripta"))
        {
            perge;
        }
        r = json_legere(a->datum, p);
        CREDO_NECESSE(r.successus);
        CREDO_AEQUALIS_I32(json_capere_chorda(r.radix, "civis",
            chorda_ex_literis("", p)).mensura, XXVI);
        CREDO_FALSUM(json_objectum_habet(r.radix, "usor"));
        CREDO_CHORDA_AEQUALIS_LITERIS(json_capere_chorda(r.radix, "via",
            chorda_ex_literis("", p)), "claude");
        inventae++;
    }
    CREDO_AEQUALIS_I32(inventae, I);
    volumen_claudere(v);
    (vacuum)filum_arborem_delere(locus.via);
}

/* ---- canalis administrationis (canalis T4) ---- */

#define INITIUM_ADMINISTRATIONIS \
    "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":" \
    "\"initialize\",\"params\":{\"protocolVersion\":\"2025-11-25\"," \
    "\"capabilities\":{},\"clientInfo\":{\"name\":\"probatio\"," \
    "\"version\":\"1\"}}}\n{\"jsonrpc\":\"2.0\",\"method\":" \
    "\"notifications/initialized\"}\n"
#define DIES_PROBATIONIS_MS ((s64)XXIV * LX * LX * M)

interior s64
_nunc_verum_ms (vacuum)
{
    structura timeval t;

    gettimeofday(&t, NIHIL);
    redde (s64)t.tv_sec * M + (s64)(t.tv_usec / M);
}

/* vocatio administrationis per canalem, servo pumpato (canalis P5):
 * radix responsi id II, aut NIHIL */
interior JsonValor*
_administrare (
              Exemplar* e,
                 Locus* locus,
    constans character* instrumentum,
    constans character* argumenta,
               Piscina* p)
{
       character  via[CCLVI];
       character* lineae;
       character* alveus;
     TcpResultus  r;
             s64  n      = ZEPHYRUM;
             s64  missa  = ZEPHYRUM;
             s64  longitudo;
             i32  k;

    sprintf(via, "%s/canalis/servus", locus->via);
    r = canalis_connectere(via, p);
    si (!r.successus)
    {
        redde NIHIL;
    }
    (vacuum)tcp_ponere_non_blocans(r.connexio, VERUM);
    lineae = (character*)piscina_allocare(p,
        (memoriae_index)strlen(argumenta) + MXXIV);
    sprintf(lineae, INITIUM_ADMINISTRATIONIS
        "{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"%s\",\"arguments\":%s}}\n",
        instrumentum, argumenta);
    longitudo = (s64)strlen(lineae);
    alveus = (character*)piscina_allocare(p, (memoriae_index)MAGNUM);
    per (k = ZEPHYRUM; k < C * M; k++)
    {
        s32 m;
        s64 initium = ZEPHYRUM;
        s64 j;

        si (missa < longitudo)
        {
            m = tcp_mittere(r.connexio, (constans i8*)lineae + missa,
                (i32)(longitudo - missa));
            si (m > ZEPHYRUM)
            {
                missa += (s64)m;
            }
        }
        _pumpare(e, I);
        m = tcp_recipere(r.connexio, (i8*)alveus + n,
            (i32)(MAGNUM - I - n));
        si (m == TCP_ITERUM)
        {
            perge;
        }
        si (m <= ZEPHYRUM)
        {
            frange;
        }
        n += (s64)m;
        per (j = ZEPHYRUM; j < n; j++)
        {
            si (alveus[j] == '\n')
            {
                      chorda linea;
                JsonResultus res;

                linea.datum    = (i8*)alveus + initium;
                linea.mensura  = (i32)(j - initium);
                initium        = j + I;
                res            = json_legere(linea, p);
                si (   res.successus
                    && json_capere_integer(res.radix, "id", -I) == II)
                {
                    tcp_claudere(r.connexio);
                    redde res.radix;
                }
            }
        }
    }
    tcp_claudere(r.connexio);
    redde NIHIL;
}

interior JsonValor*
_structura (
    JsonValor* radix)
{
    redde json_via(radix, "result.structuredContent");
}

interior b32
_vitium_administrationis (
    JsonValor* radix)
{
    redde radix && json_ad_boolean(json_via(radix, "result.isError"));
}

interior chorda
_textus_administrationis (
    JsonValor* radix)
{
    redde json_ad_chorda(json_via(radix, "result.content.0.text"));
}

/* status: salus et numeri */
interior vacuum
_sectio_administratio_status (CredoContextus* c)
{
     Piscina* p = c->piscina;
       Locus  locus;
    Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
   JsonValor* r;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_NECESSE(_usor_novus(e, p, &locus, "fran")[ZEPHYRUM] != '\0');
    r = _administrare(e, &locus, "status", "{}", p);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_FALSUM(_vitium_administrationis(r));
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_via(_structura(r),
        "salus")), "ok");
    CREDO_AEQUALIS_S64(json_ad_integer(json_via(_structura(r),
        "cives.activi")), (s64)I);
    CREDO_AEQUALIS_S64(json_ad_integer(json_via(_structura(r),
        "cives.suspensi")), (s64)ZEPHYRUM);
    CREDO_MAIOR_S64(json_ad_integer(json_via(_structura(r), "acta")),
        (s64)ZEPHYRUM);
    _finire(e, &locus);
}

/* invitare: codex validus, terminus dierum, limites dierum */
interior vacuum
_sectio_administratio_invitare (CredoContextus* c)
{
               Piscina* p = c->piscina;
                 Locus  locus;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
             JsonValor* r;
                chorda  codex;
                   s64  ante;
                   s64  expirat;
                   i32  status;
             character  nexus[CCLVI];

    CREDO_NECESSE_NON_NIHIL(e);
    ante = _nunc_verum_ms();
    r = _administrare(e, &locus, "invitare",
        "{\"nota\":\"pro Amanda\",\"dies\":3}", p);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_FALSUM(_vitium_administrationis(r));
    codex = json_ad_chorda(json_via(_structura(r), "codex"));
    CREDO_AEQUALIS_I32(codex.mensura, XXII);
    expirat = json_ad_integer(json_via(_structura(r), "expirat_ms"));
    CREDO_MAIOR_AUT_AEQUALIS_S64(expirat, ante + III *
        DIES_PROBATIONIS_MS);
    CREDO_MINOR_AUT_AEQUALIS_S64(expirat, _nunc_verum_ms() + III *
        DIES_PROBATIONIS_MS);
    sprintf(nexus, "/inscribere?invitatio=%.*s", (integer)codex.mensura,
        (constans character*)codex.datum);
    CREDO_CHORDA_CONTINET(json_ad_chorda(json_via(_structura(r),
        "nexus")),
        chorda_ex_literis(nexus, p));
    CREDO_VERUM(_inscribere(e, p, &locus, "amanda", TESSERA,
        chorda_ut_cstr(codex, p), &status));
    CREDO_AEQUALIS_I32(status, CCCIII);
    CREDO_VERUM(_vitium_administrationis(_administrare(e, &locus,
        "invitare",
        "{\"nota\":\"x\",\"dies\":0}", p)));
    CREDO_VERUM(_vitium_administrationis(_administrare(e, &locus,
        "invitare",
        "{\"nota\":\"x\",\"dies\":31}", p)));
    r = _administrare(e, &locus, "invitationes_enumerare", "{}", p);
    CREDO_AEQUALIS_I32(json_tabulatum_numerus(json_via(_structura(r),
        "invitationes")), I);
    _finire(e, &locus);
}

/* invitationes omnium: auctores, sine codicibus; revocatio quaelibet */
interior vacuum
_sectio_administratio_invitationes (CredoContextus* c)
{
               Piscina* p = c->piscina;
                 Locus  locus;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
                Lector  l;
             JsonValor* r;
             JsonValor* lista;
    constans character* sid;
    constans character* csrf;
    constans character* codex_civis;
                chorda  codex_administrationis;
                chorda  id_civis;
                chorda  textus;
             character  corpus[CCLVI];
             character  linea[CCLVI];
             character  arg[CXXVIII];
                   i32  k;

    CREDO_NECESSE_NON_NIHIL(e);
    sid = _usor_novus(e, p, &locus, "fran");
    CREDO_NECESSE(sid[ZEPHYRUM] != '\0');
    CREDO_VERUM(_rogare_et_legere(e, p, "GET /invitationes",
        _capita(p, &locus, sid, NIHIL), NIHIL, &l));
    csrf = _extrahere(&l, "name=\"csrf\" value=\"", '"', p);
    sprintf(corpus, "csrf=%s&nota=amici&dies=7", csrf);
    CREDO_VERUM(_rogare_et_legere(e, p, "POST /invitationes",
        _capita(p, &locus, sid, FORMA_U), corpus, &l));
    codex_civis = _extrahere(&l, "codex: <code>", '<', p);
    CREDO_NECESSE(codex_civis[ZEPHYRUM] != '\0');
    r = _administrare(e, &locus, "invitare", "{\"nota\":\"adm\"}", p);
    codex_administrationis = json_ad_chorda(json_via(_structura(r),
        "codex"));
    r = _administrare(e, &locus, "invitationes_enumerare", "{}", p);
    CREDO_NECESSE_NON_NIHIL(r);
    textus = _textus_administrationis(r);
    CREDO_FALSUM(chorda_continet(textus, chorda_ex_literis(codex_civis,
        p)));
    CREDO_FALSUM(chorda_continet(textus, codex_administrationis));
    lista = json_via(_structura(r), "invitationes");
    CREDO_AEQUALIS_I32(json_tabulatum_numerus(lista), III);
    id_civis.datum    = NIHIL;
    id_civis.mensura  = ZEPHYRUM;
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(lista); k++)
    {
        JsonValor* x = json_tabulatum_obtinere(lista, k);

        si (chorda_aequalis_literis(json_capere_chorda(x, "auctor",
                chorda_ex_literis("", p)), "fran"))
        {
            id_civis = json_capere_chorda(x, "id", chorda_ex_literis("",
                p));
        }
    }
    CREDO_NECESSE(id_civis.datum != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(json_capere_chorda(
        json_tabulatum_obtinere(lista, ZEPHYRUM), "auctor",
        chorda_ex_literis("", p)), "administratio");
    sprintf(arg, "{\"id\":\"%.*s\"}", (integer)id_civis.mensura,
        (constans character*)id_civis.datum);
    CREDO_FALSUM(_vitium_administrationis(_administrare(e, &locus,
        "invitationem_revocare", arg, p)));
    sprintf(linea, "GET /inscribere?invitatio=%s", codex_civis);
    CREDO_VERUM(_rogare_et_legere(e, p, linea, _capita(p, &locus, NIHIL,
        NIHIL), NIHIL, &l));
    CREDO_VERUM(_continet(&l, "invitatio non valet"));
    CREDO_VERUM(_vitium_administrationis(_administrare(e, &locus,
        "invitationem_revocare", arg, p)));
    _finire(e, &locus);
}

/* cives: nomina et status, nihil secretum */
interior vacuum
_sectio_administratio_cives (CredoContextus* c)
{
     Piscina* p = c->piscina;
       Locus  locus;
    Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
   JsonValor* r;
   JsonValor* x;
      chorda  textus;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_NECESSE(_usor_novus(e, p, &locus, "fran")[ZEPHYRUM] != '\0');
    r = _administrare(e, &locus, "cives_enumerare", "{}", p);
    CREDO_NECESSE_NON_NIHIL(r);
    x = json_tabulatum_obtinere(json_via(_structura(r), "cives"),
        ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(json_capere_chorda(x, "nomen_usoris",
        chorda_ex_literis("", p)), "fran");
    CREDO_CHORDA_AEQUALIS_LITERIS(json_capere_chorda(x, "status",
        chorda_ex_literis("", p)), "activus");
    textus = _textus_administrationis(r);
    CREDO_FALSUM(chorda_continet(textus, chorda_ex_literis("argon2",
        p)));
    CREDO_FALSUM(chorda_continet(textus, chorda_ex_literis("phc", p)));
    CREDO_FALSUM(chorda_continet(textus, chorda_ex_literis("sigillum",
        p)));
    CREDO_FALSUM(chorda_continet(textus, chorda_ex_literis(TESSERA,
        p)));
    _finire(e, &locus);
}

/* suspensio: sessio et concessio statim mortuae, intratio recusata;
 * restitutio eas non resuscitat */
interior vacuum
_sectio_suspensio (CredoContextus* c)
{
               Piscina* p = c->piscina;
                 Locus  locus;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
                Lector  l;
             JsonValor* r;
    constans character* sid;
    constans character* cliens;
    constans character* accessus;
    constans character* renovatio;
                   i32  status;

    CREDO_NECESSE_NON_NIHIL(e);
    sid = _usor_novus(e, p, &locus, "fran");
    CREDO_NECESSE(sid[ZEPHYRUM] != '\0');
    CREDO_NECESSE(_connectere(e, p, &locus, sid, &cliens, &accessus,
        &renovatio));
    CREDO_VERUM(_instrumentum(e, p, accessus, "notas_enumerare", "{}",
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    r = _administrare(e, &locus, "civem_suspendere",
        "{\"nomen\":\"Fran\"}",
        p);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_FALSUM(_vitium_administrationis(r));
    CREDO_AEQUALIS_S64(json_ad_integer(json_via(_structura(r),
        "concessiones")), (s64)I);
    CREDO_VERUM(_rogare_et_legere(e, p, "GET /notae", _capita(p, &locus,
        sid, NIHIL), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CCCIII);
    CREDO_VERUM(_instrumentum(e, p, accessus, "notas_enumerare", "{}",
        &l));
    CREDO_AEQUALIS_I32(l.status, CDI);
    (vacuum)_intrare(e, p, &locus, "fran", TESSERA, &status);
    CREDO_INAEQUALITAS_I32(status, CCCIII);
    CREDO_FALSUM(_vitium_administrationis(_administrare(e, &locus,
        "civem_restituere",
        "{\"nomen\":\"fran\"}", p)));
    CREDO_VERUM(_rogare_et_legere(e, p, "GET /notae", _capita(p, &locus,
        sid, NIHIL), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CCCIII);
    CREDO_VERUM(_instrumentum(e, p, accessus, "notas_enumerare", "{}",
        &l));
    CREDO_AEQUALIS_I32(l.status, CDI);
    (vacuum)_intrare(e, p, &locus, "fran", TESSERA, &status);
    CREDO_AEQUALIS_I32(status, CCCIII);
    _finire(e, &locus);
}

/* limites: ignotus, deletus (sepulcrum non mutatur), bis suspensus ->
 * actum unum (Review focus 4) */
interior vacuum
_sectio_suspensio_limites (CredoContextus* c)
{
     Piscina* p = c->piscina;
       Locus  locus;
    Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
   JsonValor* r;
     Volumen* v;
       Cives* cv;
       Civis  x;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_NECESSE(_usor_novus(e, p, &locus, "fran")[ZEPHYRUM] != '\0');
    CREDO_NECESSE(_usor_novus(e, p, &locus, "ana")[ZEPHYRUM] != '\0');
    r = _administrare(e, &locus, "civem_suspendere",
        "{\"nomen\":\"nemo\"}", p);
    CREDO_VERUM(_vitium_administrationis(r));
    CREDO_CHORDA_CONTINET(_textus_administrationis(r),
        chorda_ex_literis("nemo", p));
    /* 'ana' deletur per connexionem alteram (sepulcrum) */
    v = _volumen_loci(c, &locus);
    CREDO_NECESSE_NON_NIHIL(v);
    cv = cives_aperire(v, NIHIL, p);
    CREDO_NECESSE(cives_per_nomen(cv, chorda_ex_literis("ana", p), &x,
        p));
    CREDO_NECESSE(volumen_transactionem_incipere(v));
    CREDO_NECESSE(cives_delere(cv, x.id));
    CREDO_NECESSE(volumen_transactionem_committere(v));
    volumen_claudere(v);
    r = _administrare(e, &locus, "civem_suspendere",
        "{\"nomen\":\"ana\"}",
        p);
    CREDO_VERUM(_vitium_administrationis(r));
    CREDO_CHORDA_CONTINET(_textus_administrationis(r),
        chorda_ex_literis("deletus",
        p));
    CREDO_VERUM(_vitium_administrationis(_administrare(e, &locus,
        "civem_restituere",
        "{\"nomen\":\"ana\"}", p)));
    CREDO_FALSUM(_vitium_administrationis(_administrare(e, &locus,
        "civem_suspendere",
        "{\"nomen\":\"fran\"}", p)));
    CREDO_FALSUM(_vitium_administrationis(_administrare(e, &locus,
        "civem_suspendere",
        "{\"nomen\":\"fran\"}", p)));
    v = _volumen_loci(c, &locus);
    CREDO_NECESSE_NON_NIHIL(v);
    /* civis.status: deletio ana (I) + suspensio fran semel (I) */
    CREDO_AEQUALIS_I32(_acta_generis(v, "civis.status", p), I);
    CREDO_AEQUALIS_I32(_acta_generis(v, "civis.deletus", p), I);
    volumen_claudere(v);
    _finire(e, &locus);
}

/* tabula administrationis numquam in /mcp (canalis P6) */
interior vacuum
_sectio_mcp_separatum (CredoContextus* c)
{
               Piscina* p = c->piscina;
                 Locus  locus;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
                Lector  l;
    constans character* sid;
    constans character* cliens;
    constans character* accessus;
    constans character* renovatio;
             character  capita[CCLVI];

    CREDO_NECESSE_NON_NIHIL(e);
    sid = _usor_novus(e, p, &locus, "fran");
    CREDO_NECESSE(_connectere(e, p, &locus, sid, &cliens, &accessus,
        &renovatio));
    sprintf(capita, "Content-Type: application/json\r\n"
        "Authorization: Bearer %s\r\n", accessus);
    CREDO_VERUM(_rogare_et_legere(e, p, "POST /mcp", capita,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"tools/list\"}",
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "notas_enumerare"));
    CREDO_FALSUM(_continet(&l, "invitare"));
    CREDO_FALSUM(_continet(&l, "civem_suspendere"));
    CREDO_VERUM(_instrumentum(e, p, accessus, "invitare",
        "{\"nota\":\"x\"}", &l));
    CREDO_FALSUM(_continet(&l, "codex"));
    CREDO_AEQUALIS_I32(json_tabulatum_numerus(json_via(_structura(
        _administrare(e, &locus, "invitationes_enumerare", "{}", p)),
        "invitationes")), I);
    _finire(e, &locus);
}

/* filius: exemplar_invitare (obstruens) dum parens pumpat; codex per
 * fistulam */
interior s32
_invitator (
    constans character* via_datorum,
               integer  scriptor)
{
    s32 pid;

    fflush(NIHIL);
    pid = (s32)fork();
    si (pid == ZEPHYRUM)
    {
         Piscina* q = piscina_generare_dynamicum("invitator",
             LXIV * MXXIV);
          chorda codex;
             s32 exitus;

        exitus = q ? exemplar_invitare(via_datorum, "per canalem",
            &codex,
            q) : I;
        si (exitus == ZEPHYRUM)
        {
            si (write(scriptor, codex.datum, (size_t)codex.mensura) < 0)
            {
                _exit(IV);
            }
        }
        _exit((integer)exitus);
    }
    redde pid;
}

/* parens pumpat donec filius exit; codex ex fistula */
interior b32
_invitator_exspectare (
        Exemplar* e,
             s32  pid,
         integer  lector,
       character* codex,
         integer* status)
{
    i32 k;

    per (k = ZEPHYRUM; k < C * M; k++)
    {
        _pumpare(e, I);
        si (waitpid((pid_t)pid, status, WNOHANG) == (pid_t)pid)
        {
            ssize_t n = read(lector, codex, XXII);

            codex[n > ZEPHYRUM ? n : ZEPHYRUM] = '\0';
            redde VERUM;
        }
    }
    (vacuum)kill((pid_t)pid, SIGKILL);
    (vacuum)waitpid((pid_t)pid, status, ZEPHYRUM);
    redde FALSUM;
}

/* -invitare dum servus currit: per canalem, codex validus */
interior vacuum
_sectio_invitare_currens (CredoContextus* c)
{
     Piscina* p = c->piscina;
       Locus  locus;
    Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
     integer  f[II];
     integer  status = ZEPHYRUM;
   character  codex[XXXII];
         s32  pid;
         i32  st;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_NECESSE(pipe(f) == ZEPHYRUM);
    pid = _invitator(locus.via, f[I]);
    close(f[I]);   /* aliter read() EOF numquam videt */
    CREDO_NECESSE(pid > ZEPHYRUM);
    CREDO_VERUM(_invitator_exspectare(e, pid, f[ZEPHYRUM], codex,
        &status));
    CREDO_VERUM(WIFEXITED(status));
    CREDO_AEQUALIS_S32((s32)WEXITSTATUS(status), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)strlen(codex), XXII);
    CREDO_VERUM(_inscribere(e, p, &locus, "amanda", TESSERA, codex,
        &st));
    CREDO_AEQUALIS_I32(st, CCCIII);
    close(f[ZEPHYRUM]);
    _finire(e, &locus);
}

/* duo simul (Review focus 3): codices distincti, ambo validi */
interior vacuum
_sectio_invitare_simul (CredoContextus* c)
{
     Piscina* p = c->piscina;
       Locus  locus;
    Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
     integer  fa[II];
     integer  fb[II];
     integer  sa = ZEPHYRUM;
     integer  sb = ZEPHYRUM;
   character  ca[XXXII];
   character  cb[XXXII];
         s32  pa;
         s32  pb;
         i32  st;

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_NECESSE(pipe(fa) == ZEPHYRUM && pipe(fb) == ZEPHYRUM);
    pa = _invitator(locus.via, fa[I]);
    close(fa[I]);
    pb = _invitator(locus.via, fb[I]);
    close(fb[I]);
    CREDO_NECESSE(pa > ZEPHYRUM && pb > ZEPHYRUM);
    CREDO_VERUM(_invitator_exspectare(e, pa, fa[ZEPHYRUM], ca, &sa));
    CREDO_VERUM(_invitator_exspectare(e, pb, fb[ZEPHYRUM], cb, &sb));
    CREDO_AEQUALIS_S32((s32)WEXITSTATUS(sa), ZEPHYRUM);
    CREDO_AEQUALIS_S32((s32)WEXITSTATUS(sb), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)strlen(ca), XXII);
    CREDO_AEQUALIS_I32((i32)strlen(cb), XXII);
    CREDO_VERUM(strcmp(ca, cb) != ZEPHYRUM);
    CREDO_VERUM(_inscribere(e, p, &locus, "amanda", TESSERA, ca, &st));
    CREDO_AEQUALIS_I32(st, CCCIII);
    CREDO_VERUM(_inscribere(e, p, &locus, "ana", TESSERA, cb, &st));
    CREDO_AEQUALIS_I32(st, CCCIII);
    close(fa[ZEPHYRUM]);
    close(fb[ZEPHYRUM]);
    _finire(e, &locus);
}

/* servo sistente: via serae (P1); socket nullum relictum */
interior vacuum
_sectio_invitare_sistens (CredoContextus* c)
{
        Piscina* p = c->piscina;
          Locus  locus;
       Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
         chorda  codex;
      character  via[CCLVI];
    FilumStatus  st;

    CREDO_NECESSE_NON_NIHIL(e);
    exemplar_claudere(e);
    sprintf(via, "%s/canalis/servus", locus.via);
    CREDO_FALSUM(filum_status(via, &st));
    CREDO_AEQUALIS_S32(exemplar_invitare(locus.via, "sistens", &codex,
        p),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(codex.mensura, XXII);
    CREDO_FALSUM(filum_status(via, &st));
    (vacuum)filum_arborem_delere(locus.via);
}

/* filum regulare in via canalis: exemplar non incipit, sera libera;
 * remoto filo incipit */
interior vacuum
_sectio_canalis_fractus (CredoContextus* c)
{
       Piscina* p = c->piscina;
         Locus  locus;
      Exemplar* e;
     character  directorium[CCLVI];
     character  via[CCCLXXXIV];   /* directorium + "/servus" (gcc
                                    * format-overflow) */
     character n[MONETA_NANOID_PLAGULAE_MENSURA];

    (vacuum)moneta_nanoid_plagulae(n);
    sprintf(locus.via, "/tmp/exemplar_%s", n);
    sprintf(directorium, "%s/canalis", locus.via);
    sprintf(via, "%s/servus", directorium);
    CREDO_NECESSE(filum_directorium_creare_cum_modo(locus.via, 0700));
    CREDO_NECESSE(filum_directorium_creare_cum_modo(directorium, 0700));
    CREDO_NECESSE(filum_scribere_literis(via, "non socket"));
    e = _exemplar(c, &locus, ZEPHYRUM, FALSUM);
    CREDO_NIHIL(e);
    CREDO_CHORDA_AEQUALIS_LITERIS(filum_legere_totum(via, p),
        "non socket");
    CREDO_NECESSE(filum_delere(via));
    e = _exemplar(c, &locus, ZEPHYRUM, FALSUM);
    CREDO_NON_NIHIL(e);
    si (e)
    {
        _finire(e, &locus);
    }
}

/* via canalis longior sun_path (Review focus 5): non incipit, sera
 * libera */
interior vacuum
_sectio_canalis_longus (CredoContextus* c)
{
         Locus locus;
      Exemplar* e;
     character  n[MONETA_NANOID_PLAGULAE_MENSURA];
     character  via[CCLVI];
     FilumSera* sera;

    (vacuum)moneta_nanoid_plagulae(n);
    sprintf(locus.via, "/tmp/exemplar_%s_"
        "longissimum_directorium_datorum_ultra_limitem_sun_path_"
        "abcdefghijklmnopqrstuvwxyz", n);
    CREDO_NECESSE(strlen(locus.via) + XV > CVIII);
    e = _exemplar(c, &locus, ZEPHYRUM, FALSUM);
    CREDO_NIHIL(e);
    sprintf(via, "%s/sera", locus.via);
    sera = filum_seram_capere(via, ZEPHYRUM, c->piscina);
    CREDO_NON_NIHIL(sera);
    filum_seram_liberare(sera);
    (vacuum)filum_arborem_delere(locus.via);
}

/* /mcp statum civis consulit (inventum recensionis): civis suspensus
 * SINE revocatione concessionum (codex OAuth serus, revocatio omissa)
 * tessera bearer nihil potest; restitutus iterum potest */
interior vacuum
_sectio_mcp_status_civis (CredoContextus* c)
{
               Piscina* p = c->piscina;
                 Locus  locus;
              Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
                Lector  l;
    constans character* sid;
    constans character* cliens;
    constans character* accessus;
    constans character* renovatio;
               Volumen* v;
                 Cives* cv;
                 Civis  x;

    CREDO_NECESSE_NON_NIHIL(e);
    sid = _usor_novus(e, p, &locus, "fran");
    CREDO_NECESSE(_connectere(e, p, &locus, sid, &cliens, &accessus,
        &renovatio));
    CREDO_VERUM(_instrumentum(e, p, accessus, "notas_enumerare", "{}",
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    /* status per connexionem alteram: concessiones intactae */
    v = _volumen_loci(c, &locus);
    CREDO_NECESSE_NON_NIHIL(v);
    cv = cives_aperire(v, NIHIL, p);
    CREDO_NECESSE(cives_per_nomen(cv, chorda_ex_literis("fran", p), &x,
        p));
    CREDO_NECESSE(volumen_transactionem_incipere(v));
    CREDO_NECESSE(cives_statum_ponere(cv, x.id, CIVIS_SUSPENSUS));
    CREDO_NECESSE(volumen_transactionem_committere(v));
    CREDO_VERUM(_instrumentum(e, p, accessus, "notas_enumerare", "{}",
        &l));
    CREDO_AEQUALIS_I32(l.status, CDI);
    CREDO_NECESSE(volumen_transactionem_incipere(v));
    CREDO_NECESSE(cives_statum_ponere(cv, x.id, CIVIS_ACTIVUS));
    CREDO_NECESSE(volumen_transactionem_committere(v));
    volumen_claudere(v);
    CREDO_VERUM(_instrumentum(e, p, accessus, "notas_enumerare", "{}",
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    _finire(e, &locus);
}

/* communio plan-2 T7: servus communionis in exemplare montatus;
 * apparatus per canalem administrationis (digestum solum, numquam
 * tessera); tessera vera admittit, revocata non */
interior vacuum
_sectio_communio (CredoContextus* c)
{
     Piscina* p = c->piscina;
       Locus  locus;
    Exemplar* e = _exemplar(c, &locus, ZEPHYRUM, VERUM);
      Lector  l;
   JsonValor* r;
      chorda  id;
      chorda  textus;
   character  tessera[XLIV];
   character  digestum[LXV];
   character  argumenta[CCCLXXXIV];
   character  caput[CXXVIII];

    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_NECESSE(_usor_novus(e, p, &locus, "fran")[ZEPHYRUM] != '\0');
    r = _administrare(e, &locus, "communio_membrum_addere",
        "{\"spatium\":\"familia\",\"nomen\":\"fran\"}", p);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_FALSUM(_vitium_administrationis(r));
    CREDO_NECESSE(communio_tesseram_facere(tessera, digestum));
    sprintf(argumenta, "{\"nomen\":\"fran\",\"spatium\":\"familia\","
        "\"titulus\":\"Macintosh\",\"digestum\":\"%s\"}", digestum);
    r = _administrare(e, &locus, "communio_apparatum_addere", argumenta,
        p);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_FALSUM(_vitium_administrationis(r));
    id = json_ad_chorda(json_via(_structura(r), "id"));
    CREDO_AEQUALIS_I32(id.mensura, XXVI);

    CREDO_GRADUS("tessera admittit; sine tessera 401");
    sprintf(caput, "Authorization: Bearer %s\r\n", tessera);
    CREDO_VERUM(_rogare_et_legere(e, p, "GET /communio/acta?post=0",
        caput, NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_rogare_et_legere(e, p, "GET /communio/acta?post=0", "",
        NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CDI);

    CREDO_GRADUS("enumeratio sine digestis");
    r = _administrare(e, &locus, "communio_apparatus_enumerare", "{}",
        p);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_FALSUM(_vitium_administrationis(r));
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_via(_structura(r),
        "apparatus.0.titulus")), "Macintosh");
    textus = _textus_administrationis(r);
    CREDO_FALSUM(chorda_continet(textus, chorda_ex_literis(digestum,
        p)));
    CREDO_FALSUM(chorda_continet(textus, chorda_ex_literis(tessera,
        p)));

    CREDO_GRADUS("errores nominati");
    CREDO_VERUM(_vitium_administrationis(_administrare(e, &locus,
        "communio_membrum_addere",
        "{\"spatium\":\"familia\",\"nomen\":\"nemo\"}", p)));
    CREDO_VERUM(_vitium_administrationis(_administrare(e, &locus,
        "communio_apparatum_addere", argumenta, p)));
    CREDO_VERUM(_vitium_administrationis(_administrare(e, &locus,
        "communio_apparatum_revocare",
        "{\"id\":\"01ZZZZZZZZZZZZZZZZZZZZZZZZ\"}", p)));

    CREDO_GRADUS("revocatus: 401");
    sprintf(argumenta, "{\"id\":\"%.*s\"}", (integer)id.mensura,
        (constans character*)id.datum);
    CREDO_FALSUM(_vitium_administrationis(_administrare(e, &locus,
        "communio_apparatum_revocare", argumenta, p)));
    CREDO_VERUM(_rogare_et_legere(e, p, "GET /communio/acta?post=0",
        caput, NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CDI);
    _finire(e, &locus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "iter", _sectio_iter, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine_invitatione", _sectio_sine_invitatione, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "csrf", _sectio_csrf, NIHIL, NIHIL, NIHIL, NIHIL },
    { "origo", _sectio_origo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "frenum", _sectio_frenum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "alienus", _sectio_alienus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "restitutio", _sectio_restitutio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "salus", _sectio_salus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "turba", _sectio_turba, NIHIL, NIHIL, NIHIL, NIHIL },
    { "secreta", _sectio_secreta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sonda_401", _sectio_sonda_401, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sonda_documenta", _sectio_sonda_documenta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "sonda_acta", _sectio_sonda_acta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sonda_register", _sectio_sonda_register, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "instrumenta", _sectio_instrumenta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "alieni", _sectio_alieni, NIHIL, NIHIL, NIHIL, NIHIL },
    { "revocare", _sectio_revocare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "redire", _sectio_redire, NIHIL, NIHIL, NIHIL, NIHIL },
    { "textus_varius", _sectio_textus_varius, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "paginae", _sectio_paginae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "migratio", _sectio_migratio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "migratio_iterata", _sectio_migratio_iterata, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "migratio_fracta", _sectio_migratio_fracta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "sera", _sectio_sera, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mcp_nota_civis", _sectio_mcp_nota_civis, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "adm_status", _sectio_administratio_status, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "communio", _sectio_communio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "adm_invitare", _sectio_administratio_invitare, NIHIL, NIHIL,
        NIHIL,
        NIHIL },
    { "adm_invitationes", _sectio_administratio_invitationes, NIHIL,
        NIHIL, NIHIL,
        NIHIL },
    { "adm_cives", _sectio_administratio_cives, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "suspensio", _sectio_suspensio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "suspensio_limites", _sectio_suspensio_limites, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "mcp_separatum", _sectio_mcp_separatum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "invitare_currens", _sectio_invitare_currens, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "invitare_simul", _sectio_invitare_simul, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "invitare_sistens", _sectio_invitare_sistens, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "canalis_fractus", _sectio_canalis_fractus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "canalis_longus", _sectio_canalis_longus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "mcp_status_civis", _sectio_mcp_status_civis, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("exemplar_applicatio", SECTIONES);
}
