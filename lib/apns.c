/* apns.c - Apple Push Notification service, provisor. Vide
 * include/apns.h. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "lectiones.h"
#include "internamentum.h"
#include "stml.h"
#include "json.h"
#include "reactor.h"
#include "sors.h"
#include "hpack.h"
#include "http2_nexus.h"
#include "apns.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define APNS_PORTUS           CDXLIII
#define APNS_MORA_MS          (XV * M)
#define APNS_TERMINUS_MS      (XXX * M)


/* ==================================================
 * Configuratio
 * ================================================== */

interior b32
_attributum (
             StmlNodus* nodus,
    constans character* titulus,
                chorda* valor,
                chorda* causa,
               Piscina* p)
{
    chorda* a;

    a = stml_attributum_capere(nodus, titulus);
    si (!a || a->mensura == ZEPHYRUM)
    {
        *causa = chorda_concatenare(chorda_ex_literis(
            "apns: attributum absens: ", p), chorda_ex_literis(titulus,
            p), p);
        redde FALSUM;
    }
    *valor = *a;
    redde VERUM;
}

b32
apns_configurationem_legere (
     constans character* via,
       ApnsConfiguratio* c,
                 chorda* causa,
                Piscina* p)
{
                chorda  textus;
          StmlResultus  r;
                chorda* portus;
                chorda* ancora;
                chorda* sandbox;
    constans character* domus;

    memset(c, ZEPHYRUM, magnitudo(ApnsConfiguratio));
    textus = filum_legere_totum(via, p);
    si (textus.mensura == ZEPHYRUM)
    {
        *causa = chorda_concatenare(chorda_ex_literis(
            "apns: configuratio legi nequit: ", p),
            chorda_ex_literis(via, p), p);
        redde FALSUM;
    }
    r = stml_legere(textus, p, internamentum_creare(p));
    si (!r.successus || !r.elementum_radix)
    {
        *causa = chorda_ex_literis("apns: configuratio non STML", p);
        redde FALSUM;
    }
    si (   !_attributum(r.elementum_radix, "clavis_via", &c->clavis_via,
               causa, p)
        || !_attributum(r.elementum_radix, "clavis_id", &c->clavis_id,
               causa, p)
        || !_attributum(r.elementum_radix, "turma", &c->turma, causa, p)
        || !_attributum(r.elementum_radix, "thema", &c->thema, causa, p)
        || !_attributum(r.elementum_radix, "hospes", &c->hospes, causa,
               p))
    {
        redde FALSUM;
    }
    c->portus  = APNS_PORTUS;
    portus     = stml_attributum_capere(r.elementum_radix, "portus");
    si (portus && portus->mensura > ZEPHYRUM)
    {
        i32 i;

        c->portus = ZEPHYRUM;
        per (i = ZEPHYRUM; i < portus->mensura; i++)
        {
            c->portus = c->portus * X + (i32)(portus->datum[i] - '0');
        }
    }
    ancora = stml_attributum_capere(r.elementum_radix, "ancora");
    si (ancora)
    {
        c->ancora = *ancora;
    }
    sandbox = stml_attributum_capere(r.elementum_radix,
        "hospes_sandbox");
    si (sandbox)
    {
        c->hospes_sandbox = *sandbox;
    }
    domus = lectiones_ambitus("HOME");
    si (   domus && c->clavis_via.mensura >= II
        && c->clavis_via.datum[ZEPHYRUM] == '~'
        && c->clavis_via.datum[I]        == '/')
    {
        c->clavis_via = chorda_concatenare(chorda_ex_literis(domus, p),
            chorda_sectio(c->clavis_via, I, c->clavis_via.mensura), p);
    }
    redde VERUM;
}


/* ==================================================
 * Petitio (obstruens)
 * ================================================== */

nomen structura {
    ApnsResponsum* r;
          Piscina* p;
              i32  factum;
} ApnsContextus;

interior vacuum
_finis (
                     vacuum* ctx,
    constans Http2Responsum* resp)
{
    ApnsContextus* k = (ApnsContextus*)ctx;
     JsonResultus  j;
              i32  i;

    k->factum       = I;
    k->r->transita  = resp->successus;
    k->r->status    = resp->status;
    k->r->capita    = resp->capita;
    k->r->causa     = resp->causa;
    si (resp->successus && resp->corpus.mensura > ZEPHYRUM)
    {
        j = json_legere(resp->corpus, k->p);
        si (j.successus)
        {
            k->r->ratio = json_capere_chorda(j.radix, "reason",
                chorda_ex_literis("", k->p));
        }
    }
    per (i = ZEPHYRUM; resp->capita
        && i < xar_numerus(resp->capita); i++)
    {
        HpackCaput* h = (HpackCaput*)xar_obtinere(resp->capita, i);

        si (chorda_aequalis_literis(h->titulus, "apns-id"))
        {
            k->r->apns_id = h->valor;
        }
    }
}

interior vacuum
_status (
    vacuum* ctx,
       b32  vivus,
    chorda  causa)
{
    (vacuum)ctx;
    (vacuum)vivus;
    (vacuum)causa;
}

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior constans character*
_cstr (
     chorda  s,
    Piscina* p)
{
    redde s.mensura > ZEPHYRUM ? chorda_ut_cstr(s, p) : NIHIL;
}

b32
apns_mittere (
    constans ApnsConfiguratio* c,
                       chorda  tessera,
           constans character* signum,
                ApnsResponsum* r,
                      Piscina* p)
{
               Reactor* reactor;
            Http2Nexus* x;
    Http2NexusOptiones  opt;
         ApnsContextus  k;
            HpackCaput  h[III];
             character  via[CXXVIII];
                   s64  terminus;

    si (!c || !signum || !r || !p || strlen(signum) > LXIV)
    {
        redde FALSUM;
    }
    memset(r, ZEPHYRUM, magnitudo(ApnsResponsum));
    reactor = reactor_creare(p);
    si (!reactor)
    {
        redde FALSUM;
    }
    memset(&opt, ZEPHYRUM, magnitudo(opt));
    opt.mora_connexionis_ms  = APNS_MORA_MS;
    opt.mora_otii_ms         = APNS_MORA_MS;
    opt.tls_ancora_via       = _cstr(c->ancora, p);
    x = http2_nexum_aperire(reactor, chorda_ut_cstr(c->hospes, p),
        c->portus, &opt, _status, NIHIL, p);
    si (!x)
    {
        reactor_destruere(reactor);
        redde FALSUM;
    }
    h[ZEPHYRUM].titulus  = chorda_ex_literis("apns-topic", p);
    h[ZEPHYRUM].valor    = c->thema;
    h[ZEPHYRUM].numquam  = FALSUM;
    h[I].titulus         = chorda_ex_literis("apns-push-type", p);
    h[I].valor           = chorda_ex_literis("alert", p);
    h[I].numquam         = FALSUM;
    h[II].titulus        = chorda_ex_literis("authorization", p);
    h[II].valor          =
        chorda_concatenare(chorda_ex_literis("bearer ",
        p), tessera, p);
    h[II].numquam        = VERUM;
    sprintf(via, "/3/device/%s", signum);
    memset(&k, ZEPHYRUM, magnitudo(k));
    k.r = r;
    k.p = p;
    (vacuum)http2_nexus_petere(x, "POST", via, h, III,
        chorda_ex_literis(
        "{\"aps\":{\"alert\":\"probatio\"}}", p), _finis, &k, p);
    terminus = _ms_nunc() + (s64)APNS_TERMINUS_MS;
    dum (!k.factum && _ms_nunc() < terminus)
    {
        (vacuum)reactor_poll(reactor, L);
    }
    http2_nexum_claudere(x);
    reactor_destruere(reactor);
    si (!k.factum)
    {
        r->transita  = FALSUM;
        r->causa     =
            chorda_ex_literis("apns: nullum responsum intra XXX s",
            p);
    }
    redde VERUM;
}


/* ==================================================
 * Regula (spec III.4) et signum fortuitum
 * ================================================== */

ApnsIudicium
apns_iudicare (
    constans ApnsResponsum* r,
                       b32  signum_nullum)
{
    hic_manens constans character* constans MALAE[V] = {
        "InvalidProviderToken", "ExpiredProviderToken",
        "MissingProviderToken", "TopicDisallowed", "BadTopic"
    };
    i32 i;

    si (!r || !r->transita)
    {
        redde APNS_DEFECTUS;
    }
    si (r->status == CC)
    {
        redde APNS_TRANSITUS;
    }
    per (i = ZEPHYRUM; i < V; i++)
    {
        si (chorda_aequalis_literis(r->ratio, MALAE[i]))
        {
            redde APNS_DEFECTUS;
        }
    }
    si (chorda_aequalis_literis(r->ratio, "BadDeviceToken"))
    {
        redde APNS_TRANSITUS;
    }
    redde signum_nullum ? APNS_ITERANDUM : APNS_TRANSITUS;
}

vacuum
apns_signum_fortuitum (
    character* exitus,
          s64  semen)
{
    hic_manens constans character HEX[] = "0123456789abcdef";
                             Sors s;
                              i32 i;

    sors_seminare(&s, (i64)semen, ZEPHYRUM);
    per (i = ZEPHYRUM; i < LXIV; i++)
    {
        exitus[i] = HEX[sors_proximum(&s) & 0xf];
    }
    exitus[LXIV] = '\0';
}
