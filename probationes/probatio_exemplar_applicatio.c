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
#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVITATIO    "clavis-invitationis"
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
    cfg.invitatio       = INVITATIO;
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

    si (!_inscribere(e, p, locus, "ana", "tessera-anae-bona", INVITATIO,
        &status) || status != CCCIII)
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
        "tessera-anae-bona", INVITATIO, &status));
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
        INVITATIO, &status));
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
        "tessera-beae-bona", INVITATIO, &status));
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
        "tessera-anae-bona", INVITATIO, &status));
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
        "tessera-beae-bona", INVITATIO, &status));
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
        "tessera-anae-bona", INVITATIO, &status));
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
    cfg.invitatio       = INVITATIO;
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
    CREDO_VERUM(strstr(capta.textus, "clientInfo") != NIHIL);
    CREDO_VERUM(strstr(capta.textus, "SECRETUM") == NIHIL);
    /* via non MCP: nihil scriptum */
    CREDO_VERUM(_rogare_et_legere(e, c->piscina, "GET /salus", NIHIL,
        NIHIL, &l));
    CREDO_AEQUALIS_S32(capta.lineae, I);
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
    CREDO_AEQUALIS_I32(l.status, DIII);
    CREDO_VERUM(_continet(&l, "\"temporarily_unavailable\""));
    CREDO_AEQUALIS_S32(capta.lineae, I);
    CREDO_VERUM(strstr(capta.textus, "auth_callback") != NIHIL);
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
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("exemplar_applicatio", SECTIONES);
}
