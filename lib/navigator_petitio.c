/* navigator_petitio.c - articulum petere et servare (navigator N2.5;
 * vide caput). Situs albi in tabula SITUUM (v1: Vicipaedia sola).
 * Finis semper per _finire: semel, et numquam intra navigator_petere
 * (defectus immediati per timer 0). Oblatio in bibliothecam (extractio
 * + volumen) intra finem vocationis HTTP currit - synchrona, ~X-XXX ms
 * pro articulo magno. */
#include "navigator_petitio.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "uri.h"
#include "vicipaedia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALVEUS_PETITIONIS  ((memoriae_index)(LXIV * MXXIV))
#define REDIRECTIONES      V
#define ACCEPT_FONTIS \
    "text/html; charset=utf-8; " \
    "profile=\"https://www.mediawiki.org/wiki/Specs/HTML/2.8.0\""
#define VIA_AGENTIS        "/.rhubarb/navigator/agens"
#define ACCEPT_QUAESTIONIS "application/json"
#define LIMES_QUAESTIONIS  XX

structura NavigatorPetitio {
                  Piscina* piscina;       /* propria */
                  Reactor* reactor;
     NavigatorBibliotheca* bibliotheca;
    NavigatorPetitioFinis  finis;
                   vacuum* datum;
              HttpVocatio* vocatio;
          VicipaediaSedes  sedes;
 NavigatorPetitioResultus  resultus;
    /* N3.3b: quaestio (finis_quaestionis non NIHIL) - eadem machina */
    NavigatorQuaestioFinis finis_quaestionis;
 NavigatorQuaestioResultus resultus_quaestionis;
            ReactorTimerId mora;
                       b32 mora_posita;
                       b32 abrumpens;
                       b32 finita;
};

interior chorda
vacua (vacuum)
{
    chorda v;

    v.datum    = NIHIL;
    v.mensura  = ZEPHYRUM;
    redde v;
}

chorda
navigator_agens_legere (
    constans character* via,
               Piscina* piscina)
{
     constans character* domus;
              character  plena[DXII];
                 chorda  t;
                    i32  n;

    si (!via)
    {
        domus = getenv("HOME");
        si (!domus || strlen(domus) + strlen(VIA_AGENTIS) >= DXII)
        {
            redde chorda_ex_literis(NAVIGATOR_AGENS_ORDINARIUS,
                piscina);
        }
        sprintf(plena, "%s%s", domus, VIA_AGENTIS);
        via = plena;
    }
    t = filum_existit(via) ? filum_legere_totum(via, piscina) : vacua();
    n = ZEPHYRUM;
    dum (n < t.mensura && t.datum[n] != '\n' && t.datum[n] != '\r')
    {
        n++;
    }
    dum (   n > ZEPHYRUM && (t.datum[n - I] == ' '
        || t.datum[n - I] == '\t'))
    {
        n--;
    }
    si (n == ZEPHYRUM)
    {
        redde chorda_ex_literis(NAVIGATOR_AGENS_ORDINARIUS, piscina);
    }
    redde chorda_sectio(t, ZEPHYRUM, n);
}

interior vacuum
_finire (
    NavigatorPetitio* np)
{
    si (np->finita)
    {
        redde;
    }
    np->finita = VERUM;
    si (np->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(np->reactor, np->mora);
        np->mora_posita = FALSUM;
    }
    si (np->finis_quaestionis)
    {
        np->resultus_quaestionis.status      = np->resultus.status;
        np->resultus_quaestionis.descriptio  = np->resultus.descriptio;
        np->finis_quaestionis(np->datum, &np->resultus_quaestionis);
        redde;
    }
    np->finis(np->datum, &np->resultus);
}

interior vacuum
_mora_expiravit (
    vacuum* datum)
{
    NavigatorPetitio* np = (NavigatorPetitio*)datum;

    np->mora_posita = FALSUM;
    _finire(np);
}

/* defectus immediatus: finis in pulsu proximo */
interior vacuum
_differre (
          NavigatorPetitio* np,
    NavigatorPetitioStatus  status,
        constans character* causa)
{
    np->resultus.status      = status;
    np->resultus.descriptio  = chorda_ex_literis(causa, np->piscina);
    np->mora = reactor_timer_adicere(np->reactor, ZEPHYRUM, FALSUM,
        _mora_expiravit, np);
    np->mora_posita = (np->mora != REACTOR_TIMER_ERROR);
    si (!np->mora_posita)
    {
        _finire(np);    /* tabula timerorum plena: nihil melius */
    }
}

/* URL canonica ex URL ULTIMA: via eius sub hospite sedis (radix
 * probationis hospitem alium habere potest); aliter sedes data */
interior chorda
_url_canonica (
    NavigatorPetitio* np,
              chorda  ultima)
{
                Uri u;
             chorda r;
    VicipaediaSedes s;

    si (   ultima.mensura > ZEPHYRUM
        && uri_parse(chorda_ut_cstr(ultima, np->piscina), &u,
            np->piscina))
    {
        r = chorda_concatenare(chorda_ex_literis("https://",
            np->piscina), np->sedes.hospes, np->piscina);
        r = chorda_concatenare(r, u.via, np->piscina);
        si (vicipaedia_sedem_legere(r, &s, np->piscina))
        {
            redde vicipaedia_url_articuli(&s, np->piscina);
        }
    }
    redde vicipaedia_url_articuli(&np->sedes, np->piscina);
}

interior vacuum
_vocatio_finis (
         vacuum* datum,
    HttpVocatio* vocatio)
{
              NavigatorPetitio* np = (NavigatorPetitio*)datum;
                  HttpResultus  res;
                        chorda  canonica;
                        chorda  ultima;
      constans NavigatorVersio* versio = NIHIL;
                     character  causa[LXIV];

    np->vocatio = NIHIL;
    si (np->abrumpens)
    {
        np->resultus.status     = NAVIGATOR_PETITIO_ABRUPTA;
        np->resultus.descriptio = chorda_ex_literis("abrupta",
            np->piscina);
        _finire(np);
        redde;
    }
    res = http_vocatio_resultus(vocatio);
    si (!res.successus || !res.responsum)
    {
        np->resultus.status     = NAVIGATOR_PETITIO_RETE;
        np->resultus.descriptio = res.error_descriptio.mensura
            > ZEPHYRUM
            ? res.error_descriptio : chorda_ex_literis("rete defecit",
            np->piscina);
        _finire(np);
        redde;
    }
    si (res.responsum->status == CDIV)
    {
        np->resultus.status     = NAVIGATOR_PETITIO_NON_INVENTA;
        np->resultus.descriptio =
            chorda_ex_literis("pagina non inventa",
            np->piscina);
        _finire(np);
        redde;
    }
    si (res.responsum->status != CC)
    {
        sprintf(causa, "status HTTP %u",
            (insignatus integer)res.responsum->status);
        np->resultus.status = NAVIGATOR_PETITIO_RETE;
        np->resultus.descriptio = chorda_ex_literis(causa, np->piscina);
        _finire(np);
        redde;
    }
    si (np->finis_quaestionis)
    {
        /* N3.3b: eventus in piscina petitionis */
        np->resultus_quaestionis.eventus = vicipaedia_eventus_legere(
            res.responsum->corpus, np->piscina);
        np->resultus.status     = np->resultus_quaestionis.eventus
            ? NAVIGATOR_PETITIO_SERVATA : NAVIGATOR_PETITIO_RETE;
        np->resultus.descriptio = chorda_ex_literis(
            np->resultus_quaestionis.eventus ? ""
            : "responsum quaestionis non JSON", np->piscina);
        _finire(np);
        redde;
    }
    ultima    = http_vocatio_url(vocatio);
    canonica  = _url_canonica(np, ultima);
    np->resultus.exitus = navigator_versionem_offerre(np->bibliotheca,
        canonica, ultima, "vicipaedia", res.responsum->corpus,
        vicipaedia_articulus, &versio);
    si (np->resultus.exitus == NAVIGATOR_VITIUM)
    {
        np->resultus.status     = NAVIGATOR_PETITIO_RETE;
        np->resultus.descriptio = chorda_ex_literis(
            "extractio aut volumen defecit", np->piscina);
        _finire(np);
        redde;
    }
    np->resultus.status = NAVIGATOR_PETITIO_SERVATA;
    np->resultus.versio = versio;
    np->resultus.articulus = navigator_articulum_invenire(
        np->bibliotheca, canonica);
    _finire(np);
}

/* petitio nova in piscina propria; NIHIL si memoria deficit */
interior NavigatorPetitio*
_petitionem_creare (
    Reactor* reactor,
     vacuum* datum)
{
             Piscina* p;
    NavigatorPetitio* np;

    p = piscina_generare_dynamicum("navigator_petitio",
        ALVEUS_PETITIONIS);
    si (!p)
    {
        redde NIHIL;
    }
    np = (NavigatorPetitio*)piscina_allocare(p,
        magnitudo(NavigatorPetitio));
    si (!np)
    {
        piscina_destruere(p);
        redde NIHIL;
    }
    memset(np, ZEPHYRUM, magnitudo(NavigatorPetitio));
    np->piscina  = p;
    np->reactor  = reactor;
    np->datum    = datum;
    redde np;
}

/* vocatio GET url ("https://<hospes>..."; radix probationis loco
 * "https://<hospes>"), User-Agent, Accept, redirectiones */
interior vacuum
_vocare (
                     NavigatorPetitio* np,
                               chorda  url,
                               chorda  hospes,
                   constans character* accept,
    constans NavigatorPetitioOptiones* optiones)
{
                Piscina* p = np->piscina;
                 chorda  agens;
            HttpPetitio* petitio;
    HttpVocatioOptiones  optiones_http;

    si (optiones && optiones->radix_fontis)
    {
        url = chorda_concatenare(chorda_ex_literis(
            optiones->radix_fontis, p), chorda_sectio(url,
            (i32)strlen("https://") + hospes.mensura, url.mensura), p);
    }
    petitio = http_petitio_creare(p, HTTP_GET, chorda_ut_cstr(url, p));
    si (!petitio)
    {
        _differre(np, NAVIGATOR_PETITIO_RETE, "URL invalida");
        redde;
    }
    agens = (optiones && optiones->agens.mensura > ZEPHYRUM)
        ? optiones->agens
        : chorda_ex_literis(NAVIGATOR_AGENS_ORDINARIUS, p);
    http_petitio_caput_addere(petitio, "User-Agent",
        chorda_ut_cstr(agens, p));
    http_petitio_caput_addere(petitio, "Accept", accept);
    memset(&optiones_http, ZEPHYRUM, magnitudo(optiones_http));
    si (optiones && optiones->http)
    {
        optiones_http = *optiones->http;
    }
    si (optiones_http.redirectiones_maximae == ZEPHYRUM)
    {
        optiones_http.redirectiones_maximae = REDIRECTIONES;
    }
    np->vocatio = http_vocare(np->reactor, petitio, NIHIL, NIHIL,
        _vocatio_finis, np, &optiones_http, p);
    si (!np->vocatio)
    {
        _differre(np, NAVIGATOR_PETITIO_RETE, "vocatio non incepit");
    }
}

NavigatorPetitio*
navigator_petere (
                          Reactor* reactor,
             NavigatorBibliotheca* bibliotheca,
                           chorda  url,
            NavigatorPetitioFinis  finis,
                           vacuum* datum,
    constans NavigatorPetitioOptiones* optiones)
{
    NavigatorPetitio* np;

    si (!reactor || !bibliotheca || !finis)
    {
        redde NIHIL;
    }
    np = _petitionem_creare(reactor, datum);
    si (!np)
    {
        redde NIHIL;
    }
    np->bibliotheca  = bibliotheca;
    np->finis        = finis;
    si (!vicipaedia_sedem_legere(url, &np->sedes, np->piscina))
    {
        _differre(np, NAVIGATOR_PETITIO_IGNOTA,
            "situs non sustentus (v1: Vicipaedia sola)");
        redde np;
    }
    _vocare(np, vicipaedia_url_fontis(&np->sedes, np->piscina),
        np->sedes.hospes, ACCEPT_FONTIS, optiones);
    redde np;
}

NavigatorPetitio*
navigator_quaerere (
                              Reactor* reactor,
                               chorda  hospes,
                               chorda  verba,
               NavigatorQuaestioFinis  finis,
                               vacuum* datum,
    constans NavigatorPetitioOptiones* optiones)
{
    NavigatorPetitio* np;

    si (!reactor || !finis || hospes.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    np = _petitionem_creare(reactor, datum);
    si (!np)
    {
        redde NIHIL;
    }
    np->finis_quaestionis = finis;
    _vocare(np, vicipaedia_url_quaestionis(hospes, verba,
        LIMES_QUAESTIONIS, np->piscina), hospes, ACCEPT_QUAESTIONIS,
        optiones);
    redde np;
}

vacuum
navigator_petitionem_abrumpere (
    NavigatorPetitio* np)
{
    si (!np || np->finita)
    {
        redde;
    }
    np->abrumpens = VERUM;
    si (np->vocatio)
    {
        /* finis vocationis synchrone: _vocatio_finis -> ABRUPTA */
        http_vocationem_abrumpere(np->vocatio);
        redde;
    }
    np->resultus.status = NAVIGATOR_PETITIO_ABRUPTA;
    np->resultus.descriptio = chorda_ex_literis("abrupta", np->piscina);
    _finire(np);
}

vacuum
navigator_petitionem_dimittere (
    NavigatorPetitio* np)
{
    si (!np)
    {
        redde;
    }
    navigator_petitionem_abrumpere(np);
    si (np->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(np->reactor, np->mora);
        np->mora_posita = FALSUM;
    }
    piscina_destruere(np->piscina);
}
