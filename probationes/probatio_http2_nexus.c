/* probatio_http2_nexus.c - connexio HTTP/2 diuturna super reactorem
 * (http2-plan T8) contra oraculum Go (tools/oracula/http2_servus.go,
 * bibliotheca norma sola, processus filius) et openssl s_server (non
 * h2). Sine 'go' suita II reddit (NIHIL CURSUM, spec D5).
 *
 * HTTP2_TABULAE=<dir>: casus salve, eco, multae, magnum (CCLVI KiB),
 * abire octetos utriusque partis (tap nexus) et tabulam formarum Go
 * (GODEBUG=http2debug=2) in <dir> scribunt - tools/http2_tabulare.sh
 * eas in probationes/fixa/http2 ponit (http2-plan C5). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "reactor.h"
#include "tcp.h"
#include "tls.h"
#include "filum.h"
#include "processus.h"
#include "lectiones.h"
#include "hpack.h"
#include "http2_nexus.h"
#include "servus_tls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MAGNUM_VIVUM     (IV * 0x100000)
#define MAGNUM_TABULAE   (CCLVI * MXXIV)
#define MENSURA_REDDENDA      (C * MXXIV)
#define MULTAE           XX

nomen structura {
    character dir[DXII];
    character fixa[DXII];
    character ancora[MXXIV];
    character servus_oraculi[MXXIV];
} Locus;

/* exitus unius petitionis */
nomen structura {
     i32 facta;          /* quoties finis vocatus */
     b32 successus;
     i32 status;
  chorda corpus;
  chorda causa;
     b32 retentabilis;
} Exitus;

nomen structura {
     i32 vivi;
     i32 mortui;
  chorda causa;
} Observatio;

nomen structura {
     Processus* processus;
           i32  portus;
} ServusOraculi;

hic_manens constans character* LOCI_ORACULI[IV] = {
    "/usr/local/go/bin/go", "/opt/homebrew/bin/go", "/usr/bin/go", NIHIL
};

interior constans character*
_compilator_oraculi (vacuum)
{
    i32 k;

    per (k = ZEPHYRUM; LOCI_ORACULI[k]; k++)
    {
        si (filum_existit(LOCI_ORACULI[k]))
        {
            redde LOCI_ORACULI[k];
        }
    }
    redde NIHIL;
}

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
                  Locus* l = (Locus*)piscina_allocare(c->piscina,
                      magnitudo(Locus));
     constans character* area = credo_area(c);
     constans character* argumenta[VI];
              character  cella[MXXIV];
      ProcessusResultus  pr;

    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_NECESSE_NON_NIHIL(area);
    strcpy(l->dir, area);
    sprintf(l->fixa, "%s/certa", area);
    sprintf(l->ancora, "%s/ca.der", l->fixa);
    sprintf(l->servus_oraculi, "%s/http2_servus", area);
    argumenta[ZEPHYRUM] = "probationes/fixa/tls/generare.sh";
    argumenta[I] = l->fixa;
    argumenta[II] = NIHIL;
    pr = processus_exsequi(argumenta, XXX * M, c->piscina);
    CREDO_NECESSE(pr.successus && pr.codex_exitus == ZEPHYRUM);
    sprintf(cella, "%s/gocache", area);
    (vacuum)setenv("GOCACHE", cella, I);
    (vacuum)setenv("GOTELEMETRY", "off", I);
    argumenta[ZEPHYRUM] = _compilator_oraculi();
    argumenta[I] = "build";
    argumenta[II] = "-o";
    argumenta[III] = l->servus_oraculi;
    argumenta[IV] = "tools/oracula/http2_servus.go";
    argumenta[V] = NIHIL;
    pr = processus_exsequi(argumenta, CXX * M, c->piscina);
    CREDO_NECESSE(pr.successus && pr.codex_exitus == ZEPHYRUM);
    c->datum = l;
}

interior vacuum
_suitam_purgare (
    CredoContextus* c)
{
    (vacuum)c;
}

/* servus Go cum tabula formarum in <dir>/<casus>.go.log */
interior b32
servum_oraculi_incipere (
             CredoContextus* c,
              ServusOraculi* s,
         constans character* exemplum)
{
     constans Locus* l = (constans Locus*)c->suita;
          character  mandatum[MMXLVIII];
          character  portus_c[XXIV];
 constans character* argumenta[IV];
                i32  k;

    s->portus = _servi_tls_portus(c->piscina);
    sprintf(portus_c, "%u", (insignatus integer)s->portus);
    sprintf(mandatum, "GODEBUG=http2debug=2 exec '%s' -portus %s"
        " -cert '%s/localhost.pem' -clavis '%s/localhost.key'"
        " 2> '%s/%s.go.log'", l->servus_oraculi, portus_c, l->fixa,
        l->fixa,
        (lectiones_ambitus("HTTP2_TABULAE")
         && lectiones_ambitus("HTTP2_TABULAE")[ZEPHYRUM])
        ? lectiones_ambitus("HTTP2_TABULAE") : l->dir, exemplum);
    argumenta[ZEPHYRUM] = "/bin/sh";
    argumenta[I] = "-c";
    argumenta[II] = mandatum;
    argumenta[III] = NIHIL;
    s->processus = processus_incipere(argumenta, CXX * M, c->piscina);
    si (!s->processus)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < LX; k++)
    {
        si (_servi_tls_auscultat(s->portus, c->piscina))
        {
            redde VERUM;
        }
        (vacuum)processus_pulsare(s->processus);
        (vacuum)usleep(L * M);
    }
    redde FALSUM;
}

interior vacuum
servum_oraculi_claudere (
    ServusOraculi* s)
{
    si (s->processus)
    {
        processus_abrumpere(s->processus);
        s->processus = NIHIL;
    }
}

interior vacuum
_finis (
                     vacuum* ctx,
    constans Http2Responsum* r)
{
    Exitus* e = (Exitus*)ctx;

    e->facta++;
    e->successus     = r->successus;
    e->status        = r->status;
    e->corpus        = r->corpus;
    e->causa         = r->causa;
    e->retentabilis  = r->retentabilis;
}

interior vacuum
_status (
    vacuum* ctx,
       b32  vivus,
    chorda  causa)
{
    Observatio* o = (Observatio*)ctx;

    si (vivus)
    {
        o->vivi++;
    }
    alioquin
    {
        o->mortui++;
        o->causa = causa;
    }
}

/* reactor pulsatur donec *factum aut ms praeterierunt */
interior b32
donec (
         Reactor* reactor,
    constans i32* factum,
             i32  ms)
{
    s64 finis = _ms_nunc() + (s64)ms;

    dum (*factum == ZEPHYRUM && _ms_nunc() < finis)
    {
        (vacuum)reactor_poll(reactor, L);
    }
    redde *factum != ZEPHYRUM;
}

interior vacuum
morari (
    Reactor* reactor,
        i32  ms)
{
    s64 finis = _ms_nunc() + (s64)ms;

    dum (_ms_nunc() < finis)
    {
        (vacuum)reactor_poll(reactor, X);
    }
}

/* tabulae (HTTP2_TABULAE): via casus aut NIHIL */
interior constans character*
tabula (
        CredoContextus* c,
    constans character* exemplum)
{
    constans character* dir = lectiones_ambitus("HTTP2_TABULAE");
             character* via;

    si (!dir || !dir[ZEPHYRUM])
    {
        redde NIHIL;
    }
    via = (character*)piscina_allocare(c->piscina, MXXIV);
    sprintf(via, "%s/%s", dir, exemplum);
    redde via;
}

interior Http2Nexus*
aperire (
        CredoContextus* c,
               Reactor* reactor,
                   i32  portus,
            Observatio* o,
                   i32  mora_otii_ms,
    constans character* tabula_via)
{
        constans Locus* l = (constans Locus*)c->suita;
    Http2NexusOptiones  opt;

    memset(&opt, ZEPHYRUM, magnitudo(opt));
    opt.tls_ancora_via  = l->ancora;
    opt.mora_otii_ms    = mora_otii_ms;
    opt.tabula_via      = tabula_via;
    memset(o, ZEPHYRUM, magnitudo(*o));
    redde http2_nexum_aperire(reactor, "localhost", portus, &opt,
        _status,
        o, c->piscina);
}

interior b32
get (
        CredoContextus* c,
            Http2Nexus* x,
    constans character* via,
                Exitus* e)
{
    chorda vacuum_corpus;

    memset(e, ZEPHYRUM, magnitudo(*e));
    vacuum_corpus.datum    = NIHIL;
    vacuum_corpus.mensura  = ZEPHYRUM;
    redde http2_nexus_petere(x, "GET", via, NIHIL, ZEPHYRUM,
        vacuum_corpus,
        _finis, e, c->piscina);
}

interior vacuum
_sectio_salve (
    CredoContextus* c)
{
            Reactor* r;
      ServusOraculi  s;
         Http2Nexus* x;
         Observatio  o;
             Exitus  e;

    CREDO_GRADUS("GET /salve -> 200 salve, vivus semel");
    r = reactor_creare(c->piscina);
    CREDO_NECESSE(r && servum_oraculi_incipere(c, &s, "salve"));
    x = aperire(c, r, s.portus, &o, ZEPHYRUM, tabula(c, "salve"));
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_VERUM(get(c, x, "/salve", &e));
    CREDO_AEQUALIS_I32(e.facta, ZEPHYRUM);
    CREDO_VERUM(donec(r, &e.facta, X * M));
    CREDO_VERUM(e.successus);
    CREDO_AEQUALIS_I32(e.status, CC);
    CREDO_CHORDA_AEQUALIS_LITERIS(e.corpus, "salve");
    CREDO_AEQUALIS_I32(o.vivi, I);
    http2_nexum_claudere(x);
    servum_oraculi_claudere(&s);
    reactor_destruere(r);
}

interior vacuum
_sectio_reddere (
    CredoContextus* c)
{
            Reactor* r;
      ServusOraculi  s;
         Http2Nexus* x;
         Observatio  o;
             Exitus  e;
             chorda  corpus;
         HpackCaput  h[I];
                i32  i;

    CREDO_GRADUS("POST C KiB -> idem redditum");
    r = reactor_creare(c->piscina);
    CREDO_NECESSE(r && servum_oraculi_incipere(c, &s, "eco"));
    x = aperire(c, r, s.portus, &o, ZEPHYRUM, tabula(c, "eco"));
    CREDO_NECESSE_NON_NIHIL(x);
    corpus.mensura = MENSURA_REDDENDA;
    corpus.datum = (i8*)piscina_allocare(c->piscina,
        MENSURA_REDDENDA);
    per (i = ZEPHYRUM; i < MENSURA_REDDENDA; i++)
    {
        corpus.datum[i] = (i8)('A' + i % XXVI);
    }
    h[ZEPHYRUM].titulus  = chorda_ex_literis("content-type",
        c->piscina);
    h[ZEPHYRUM].valor    = chorda_ex_literis("application/octet-stream",
        c->piscina);
    h[ZEPHYRUM].numquam  = FALSUM;
    memset(&e, ZEPHYRUM, magnitudo(e));
    CREDO_VERUM(http2_nexus_petere(x, "POST", "/eco", h, I, corpus,
        _finis,
        &e, c->piscina));
    CREDO_VERUM(donec(r, &e.facta, XX * M));
    CREDO_VERUM(e.successus);
    CREDO_AEQUALIS_I32(e.corpus.mensura, MENSURA_REDDENDA);
    CREDO_VERUM(e.corpus.mensura == MENSURA_REDDENDA
        && memcmp(e.corpus.datum, corpus.datum, MENSURA_REDDENDA)
            == ZEPHYRUM);
    http2_nexum_claudere(x);
    servum_oraculi_claudere(&s);
    reactor_destruere(r);
}

interior vacuum
_sectio_multae (
    CredoContextus* c)
{
            Reactor* r;
      ServusOraculi  s;
         Http2Nexus* x;
         Observatio  o;
             Exitus  e[MULTAE];
                i32  i;
                i32  omnes;
                s64  finis;

    CREDO_GRADUS("XX petitiones simul, connexio una");
    r = reactor_creare(c->piscina);
    CREDO_NECESSE(r && servum_oraculi_incipere(c, &s, "multae"));
    x = aperire(c, r, s.portus, &o, ZEPHYRUM, tabula(c, "multae"));
    CREDO_NECESSE_NON_NIHIL(x);
    per (i = ZEPHYRUM; i < MULTAE; i++)
    {
        CREDO_VERUM(get(c, x, "/salve", &e[i]));
    }
    finis = _ms_nunc() + (s64)(XX * M);
    fac
    {
        (vacuum)reactor_poll(r, L);
        omnes = ZEPHYRUM;
        per (i = ZEPHYRUM; i < MULTAE; i++)
        {
            omnes += e[i].facta;
        }
    } dum (omnes < MULTAE && _ms_nunc() < finis);
    per (i = ZEPHYRUM; i < MULTAE; i++)
    {
        CREDO_AEQUALIS_I32(e[i].facta, I);
        CREDO_VERUM(e[i].successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(e[i].corpus, "salve");
    }
    CREDO_AEQUALIS_I32(o.vivi, I);
    CREDO_AEQUALIS_I32(o.mortui, ZEPHYRUM);
    http2_nexum_claudere(x);
    servum_oraculi_claudere(&s);
    reactor_destruere(r);
}

interior vacuum
_sectio_magnum (
    CredoContextus* c)
{
            Reactor* r;
      ServusOraculi  s;
         Http2Nexus* x;
         Observatio  o;
             Exitus  e;
          character  via[LXIV];
                i32  n;

    n = tabula(c, "magnum") ? MAGNUM_TABULAE : MAGNUM_VIVUM;
    CREDO_GRADUS("magnum: corpus totum");
    r = reactor_creare(c->piscina);
    CREDO_NECESSE(r && servum_oraculi_incipere(c, &s, "magnum"));
    x = aperire(c, r, s.portus, &o, ZEPHYRUM, tabula(c, "magnum"));
    CREDO_NECESSE_NON_NIHIL(x);
    sprintf(via, "/magnum?n=%u", (insignatus integer)n);
    CREDO_VERUM(get(c, x, via, &e));
    CREDO_VERUM(donec(r, &e.facta, LX * M));
    CREDO_VERUM(e.successus);
    CREDO_AEQUALIS_I32(e.corpus.mensura, n);
    CREDO_VERUM(e.corpus.mensura >= XXVI && memcmp(e.corpus.datum,
        "abcdefghijklmnopqrstuvwxyz", XXVI) == ZEPHYRUM);
    http2_nexum_claudere(x);
    servum_oraculi_claudere(&s);
    reactor_destruere(r);
}

interior vacuum
_sectio_abire (
    CredoContextus* c)
{
            Reactor* r;
      ServusOraculi  s;
         Http2Nexus* x;
         Observatio  o;
             Exitus  e;

    CREDO_GRADUS("GOAWAY servi: status semel, petitio postea retentabilis");
    r = reactor_creare(c->piscina);
    CREDO_NECESSE(r && servum_oraculi_incipere(c, &s, "abire"));
    x = aperire(c, r, s.portus, &o, ZEPHYRUM, tabula(c, "abire"));
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_VERUM(get(c, x, "/abire", &e));
    CREDO_VERUM(donec(r, &e.facta, X * M));
    CREDO_VERUM(e.successus);
    CREDO_CHORDA_AEQUALIS_LITERIS(e.corpus, "vale");
    CREDO_VERUM(donec(r, &o.mortui, X * M));
    CREDO_VERUM(get(c, x, "/salve", &e));
    CREDO_AEQUALIS_I32(e.facta, ZEPHYRUM);
    CREDO_VERUM(donec(r, &e.facta, X * M));
    CREDO_FALSUM(e.successus);
    CREDO_VERUM(e.retentabilis);
    morari(r, CC);
    CREDO_AEQUALIS_I32(o.mortui, I);
    CREDO_AEQUALIS_I32(e.facta, I);
    http2_nexum_claudere(x);
    servum_oraculi_claudere(&s);
    reactor_destruere(r);
}

interior vacuum
_sectio_abruptio (
    CredoContextus* c)
{
            Reactor* r;
      ServusOraculi  s;
         Http2Nexus* x;
         Observatio  o;
             Exitus  e[II];

    CREDO_GRADUS("servus necatus medio (Review Focus 4): semel, semel");
    r = reactor_creare(c->piscina);
    CREDO_NECESSE(r && servum_oraculi_incipere(c, &s, "abruptio"));
    x = aperire(c, r, s.portus, &o, ZEPHYRUM, NIHIL);
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_VERUM(get(c, x, "/tarde?ms=5000", &e[ZEPHYRUM]));
    CREDO_VERUM(get(c, x, "/tarde?ms=5000", &e[I]));
    CREDO_VERUM(donec(r, &o.vivi, X * M));
    morari(r, CCC);
    servum_oraculi_claudere(&s);
    CREDO_VERUM(donec(r, &e[I].facta, X * M));
    morari(r, CC);
    CREDO_AEQUALIS_I32(e[ZEPHYRUM].facta, I);
    CREDO_AEQUALIS_I32(e[I].facta, I);
    CREDO_FALSUM(e[ZEPHYRUM].successus);
    CREDO_AEQUALIS_I32(o.mortui, I);
    http2_nexum_claudere(x);
    CREDO_AEQUALIS_I32(e[ZEPHYRUM].facta, I);
    reactor_destruere(r);
}

interior vacuum
_sectio_non_h2 (
    CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
           Reactor* r;
         ServusTls  s;
        Http2Nexus* x;
        Observatio  o;
            Exitus  e;

    CREDO_GRADUS("servus sine h2 (openssl): status FALSUM, h2 nominatum");
    r = reactor_creare(c->piscina);
    CREDO_NECESSE(r && servum_tls_incipere(&s, l->dir, l->fixa,
        "localhost", c->piscina));
    x = aperire(c, r, (i32)s.portus, &o, ZEPHYRUM, NIHIL);
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_VERUM(get(c, x, "/", &e));
    CREDO_VERUM(donec(r, &o.mortui, X * M));
    CREDO_VERUM(chorda_continet(o.causa, chorda_ex_literis("h2",
        c->piscina)));
    CREDO_VERUM(donec(r, &e.facta, X * M));
    CREDO_FALSUM(e.successus);
    CREDO_AEQUALIS_I32(o.vivi, ZEPHYRUM);
    http2_nexum_claudere(x);
    servum_tls_claudere(&s);
    reactor_destruere(r);
}

interior vacuum
_sectio_tempus (
    CredoContextus* c)
{
            Reactor* r;
      ServusOraculi  s;
         Http2Nexus* x;
         Observatio  o;
             Exitus  e;

    CREDO_GRADUS("otium M ms, servus V s: tempus nominatum");
    r = reactor_creare(c->piscina);
    CREDO_NECESSE(r && servum_oraculi_incipere(c, &s, "tempus"));
    x = aperire(c, r, s.portus, &o, M, NIHIL);
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_VERUM(get(c, x, "/tarde?ms=5000", &e));
    CREDO_VERUM(donec(r, &e.facta, IV * M));
    CREDO_FALSUM(e.successus);
    CREDO_VERUM(chorda_continet(e.causa, chorda_ex_literis("tempus",
        c->piscina)));
    http2_nexum_claudere(x);
    servum_oraculi_claudere(&s);
    reactor_destruere(r);
}

/* RHUBARB_RETE=1 solum: situs publici per h2 (fiducia systematis) */
interior vacuum
_sectio_rete (
    CredoContextus* c)
{
    hic_manens constans character* constans HOSPITES[II] = {
        "nghttp2.org", "www.google.com"
    };
    i32 k;

    per (k = ZEPHYRUM; k < II; k++)
    {
                   Reactor* r;
                Http2Nexus* x;
        Http2NexusOptiones  opt;
                Observatio  o;
                    Exitus  e;

        CREDO_GRADUS(HOSPITES[k]);
        r = reactor_creare(c->piscina);
        CREDO_NECESSE_NON_NIHIL(r);
        memset(&opt, ZEPHYRUM, magnitudo(opt));
        memset(&o, ZEPHYRUM, magnitudo(o));
        x = http2_nexum_aperire(r, HOSPITES[k], CDXLIII, &opt, _status,
            &o,
            c->piscina);
        CREDO_NECESSE_NON_NIHIL(x);
        CREDO_VERUM(get(c, x, "/", &e));
        CREDO_VERUM(donec(r, &e.facta, XX * M));
        CREDO_VERUM(e.successus && e.status >= CC && e.status < CD);
        http2_nexum_claudere(x);
        reactor_destruere(r);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "salve", _sectio_salve, NIHIL, NIHIL, NIHIL, NIHIL },
    { "eco", _sectio_reddere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "multae", _sectio_multae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "magnum", _sectio_magnum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "abire", _sectio_abire, NIHIL, NIHIL, NIHIL, NIHIL },
    { "abruptio", _sectio_abruptio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "non h2", _sectio_non_h2, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tempus", _sectio_tempus, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

/* RHUBARB_RETE=1: situs publici quoque (ut probatio_http_vocatio_rete) */
hic_manens constans CredoSectio SECTIONES_RETIS[] = {
    { "rete: situs publici", _sectio_rete, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    constans character* rete = lectiones_ambitus("RHUBARB_RETE");

    si (_compilator_oraculi() == NIHIL)
    {
        imprimere("NIHIL CURSUM: go deest (oraculum http2_servus)\n");
        redde II;
    }
    si (rete && strcmp(rete, "1") == ZEPHYRUM)
    {
        redde credo_suitam_currere_cum("http2_nexus_rete",
            SECTIONES_RETIS,
            _suitam_parare, _suitam_purgare);
    }
    redde credo_suitam_currere_cum("http2_nexus", SECTIONES,
        _suitam_parare, _suitam_purgare);
}
