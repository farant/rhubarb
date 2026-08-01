/* probatio_vitrea_servus.c - transportus HTTP applicationis vitreae
 *
 * Exemplar idem quo probatio_hospitium: cliens obstruens (timeout
 * II s - probatio numquam pendet) + pumpatio manualis gressuum in
 * eodem filo. Capsula fixturae = capsula_templates (probationes/
 * vitrea_assets), quae unum index.html cum <head> fert.
 *
 * Nihil hic reticulum externum tangit neque daemonem: praebitor
 * probationis methodos suas proprias fert.
 */

#include "vitrea_servus.h"
#include "tcp.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "capsula_templates.h"

#include <stdio.h>
#include <string.h>

/* ========================================================================
 * PRAEBITOR PROBATIONIS
 * ======================================================================== */

hic_manens i32 g_praebitor_vocatus = ZEPHYRUM;
hic_manens i32 g_modus_publicus_visus = ZEPHYRUM;

interior JsonValor*
_salve (JsonValor* argumenta, Piscina* piscina, vacuum* datum,
    chorda* culpa)
{
    JsonValor* fructus = json_objectum_creare(piscina);

    (vacuum)argumenta;
    (vacuum)datum;
    (vacuum)culpa;
    json_objectum_ponere(fructus, "salutatio",
        json_chorda_creare_literis(piscina, "salve munde"));
    redde fructus;
}

/* methodus harnesii: fenestrae SOLIUS - super socket abesse debet */
interior JsonValor*
_harnesium (JsonValor* argumenta, Piscina* piscina, vacuum* datum,
    chorda* culpa)
{
    JsonValor* fructus = json_objectum_creare(piscina);

    (vacuum)argumenta;
    (vacuum)datum;
    (vacuum)culpa;
    json_objectum_ponere(fructus, "arcanum",
        json_chorda_creare_literis(piscina, "NON DIVULGANDUM"));
    redde fructus;
}

interior vacuum
_praebitor (Internuntius* inx, InternuntiusModus modus,
    vacuum* datum)
{
    g_praebitor_vocatus++;
    si (modus == INTERNUNTIUS_MODUS_PUBLICUS)
    {
        g_modus_publicus_visus++;
    }
    (vacuum)internuntius_praebere(inx, "salve", _salve, datum);
    si (modus == INTERNUNTIUS_MODUS_LOCALIS)
    {
        (vacuum)internuntius_praebere(inx, "harnesium", _harnesium,
            datum);
    }
}

/* ========================================================================
 * AUXILIA
 * ======================================================================== */

interior vacuum
_pumpare (VitreaServus* s, i32 vices)
{
    i32 i;

    per (i = ZEPHYRUM; i < vices; i++)
    {
        (vacuum)vitrea_servus_gressus(s, X);   /* 10ms */
    }
}

interior TcpConnexio*
_cliens_connectere (Piscina* piscina, i32 portus)
{
    TcpOptiones opt = tcp_optiones_default();
    TcpResultus res;

    opt.timeout_ms = MM;   /* 2s - recv fallit pro pendendo */
    res = tcp_connectere_cum_optionibus("127.0.0.1", portus, &opt,
        piscina);
    redde res.successus ? res.connexio : NIHIL;
}

interior s32
_commercium (VitreaServus* s, TcpConnexio* cliens,
    constans character* petitio, character* buffer, i32 capacitas)
{
    s32 n;

    si (!tcp_mittere_omnia(cliens, (constans i8*)petitio,
            (i32)strlen(petitio)))
    {
        redde -I;
    }
    _pumpare(s, X);
    n = tcp_recipere(cliens, (i8*)buffer, capacitas - I);
    si (n > ZEPHYRUM)
    {
        buffer[n] = '\0';
    }
    redde n;
}

/* petitionem pontis aedificare et mittere: Content-Length
 * COMPUTATUM, numquam manu numeratum. (Primo conatu tres ex
 * quattuor numeri manu scripti falsi erant - servus recte bytes
 * absentes exspectabat et probatio in silentium cadebat. Mensura
 * quam machina computat mentiri non potest.) */
interior s32
_pons_petere_cum (VitreaServus* s, Piscina* piscina,
    constans character* corpus, constans character* capita_extra,
    character* buffer, i32 capacitas)
{
    TcpConnexio* c;
    character    petitio[4096];

    c = _cliens_connectere(piscina, vitrea_servus_portus(s));
    si (c == NIHIL)
    {
        redde -I;
    }
    sprintf(petitio,
        "POST /internuntius HTTP/1.1\r\nHost: x\r\n"
        "Content-Type: application/json\r\n%s"
        "Content-Length: %d\r\n\r\n%s",
        capita_extra ? capita_extra : "",
        (int)strlen(corpus), corpus);
    redde _commercium(s, c, petitio, buffer, capacitas);
}

interior s32
_pons_petere (VitreaServus* s, Piscina* piscina,
    constans character* corpus, character* buffer, i32 capacitas)
{
    redde _pons_petere_cum(s, piscina, corpus, NIHIL, buffer,
        capacitas);
}

interior VitreaServus*
_servum_creare (Piscina* piscina)
{
    VitreaServusConfiguratio figura;
    Capsula* capsula = capsula_aperire(&capsula_templates, piscina);

    si (capsula == NIHIL)
    {
        redde NIHIL;
    }
    memset(&figura, ZEPHYRUM, magnitudo(figura));
    figura.capsula         = capsula;
    figura.via_initialis   = "index.html";
    figura.praebitor       = _praebitor;
    figura.praebitor_datum = NIHIL;
    figura.portus          = ZEPHYRUM;   /* auto */
    redde vitrea_servus_creare(piscina, &figura);
}

/* ========================================================================
 * PROBATIONES
 * ======================================================================== */

interior vacuum
probatio_creare (Piscina* piscina)
{
    VitreaServus* s;

    printf("--- creare: portus auto + praebitor modo PUBLICO ---\n");
    g_praebitor_vocatus = ZEPHYRUM;
    g_modus_publicus_visus = ZEPHYRUM;
    s = _servum_creare(piscina);
    CREDO_NON_NIHIL(s);
    CREDO_VERUM(vitrea_servus_portus(s) > ZEPHYRUM);
    CREDO_NON_NIHIL(vitrea_servus_internuntius(s));
    /* praebitor SEMEL vocatus, et modo publico: applicatio
     * methodos harnesii excludere potuit */
    CREDO_AEQUALIS_I32(g_praebitor_vocatus, I);
    CREDO_AEQUALIS_I32(g_modus_publicus_visus, I);
    vitrea_servus_destruere(s);
}

interior vacuum
probatio_pagina_scriptum_fert (Piscina* piscina)
{
    VitreaServus* s = _servum_creare(piscina);
    TcpConnexio*  c;
    character     buffer[16384];

    printf("--- pagina: scriptum pontis post <head> insertum ---\n");
    CREDO_NON_NIHIL(s);
    c = _cliens_connectere(piscina, vitrea_servus_portus(s));
    CREDO_NON_NIHIL(c);
    CREDO_VERUM(_commercium(s, c,
        "GET / HTTP/1.1\r\nHost: x\r\n\r\n", buffer,
        (i32)magnitudo(buffer)) > ZEPHYRUM);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 200 OK") != NIHIL);
    CREDO_VERUM(strstr(buffer, "text/html") != NIHIL);
    /* scriptum iniectum adest... */
    CREDO_VERUM(strstr(buffer, "window.internuntius") != NIHIL);
    CREDO_VERUM(strstr(buffer, "fetch('/internuntius'") != NIHIL);
    /* ...ET pagina originalis intacta manet */
    CREDO_VERUM(strstr(buffer, "salve vitrea") != NIHIL);
    /* insertio POST <head>, non ante <html> */
    CREDO_VERUM(strstr(buffer, "<head><script>") != NIHIL);
    vitrea_servus_destruere(s);
}

interior vacuum
probatio_pons_vocatio (Piscina* piscina)
{
    VitreaServus* s = _servum_creare(piscina);
    character     buffer[8192];

    printf("--- pons: vocatio -> fructus methodi ---\n");
    CREDO_NON_NIHIL(s);
    CREDO_VERUM(_pons_petere(s, piscina,
        "{\"id\":7,\"methodus\":\"salve\",\"argumenta\":{\"quem\":\"x\"}}",
        buffer, (i32)magnitudo(buffer)) > ZEPHYRUM);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 200 OK") != NIHIL);
    CREDO_VERUM(strstr(buffer, "application/json") != NIHIL);
    CREDO_VERUM(strstr(buffer, "\"salutatio\":\"salve munde\"")
        != NIHIL);
    CREDO_VERUM(strstr(buffer, "\"id\":7") != NIHIL);
    vitrea_servus_destruere(s);
}

/* PORTA: methodus modo LOCALI registrata super socket abest.
 * Haec probatio est causa cur modus PARAMETRUM praebitoris est. */
interior vacuum
probatio_porta_harnesii (Piscina* piscina)
{
    VitreaServus* s = _servum_creare(piscina);
    character     buffer[8192];

    printf("--- porta: methodus harnesii super socket ABEST ---\n");
    CREDO_NON_NIHIL(s);
    CREDO_VERUM(_pons_petere(s, piscina,
        "{\"id\":8,\"methodus\":\"harnesium\",\"argumenta\":{}}",
        buffer, (i32)magnitudo(buffer)) > ZEPHYRUM);
    CREDO_VERUM(strstr(buffer, "methodus ignota") != NIHIL);
    /* arcanum harnesii per fistulam NUMQUAM transit */
    CREDO_VERUM(strstr(buffer, "NON DIVULGANDUM") == NIHIL);
    vitrea_servus_destruere(s);
}

interior vacuum
probatio_culpae_non_frangunt (Piscina* piscina)
{
    VitreaServus* s = _servum_creare(piscina);
    character     buffer[8192];

    printf("--- culpae: JSON fractum -> culpa, servus vivit ---\n");
    CREDO_NON_NIHIL(s);
    CREDO_VERUM(_pons_petere(s, piscina, "{\"ruptum\"", buffer,
        (i32)magnitudo(buffer)) > ZEPHYRUM);
    CREDO_VERUM(strstr(buffer, "culpa") != NIHIL);

    /* servus vivit: vocatio sana post fracturam adhuc respondet */
    CREDO_VERUM(_pons_petere(s, piscina,
        "{\"id\":9,\"methodus\":\"salve\",\"argumenta\":{}}",
        buffer, (i32)magnitudo(buffer)) > ZEPHYRUM);
    CREDO_VERUM(strstr(buffer, "salve munde") != NIHIL);
    vitrea_servus_destruere(s);
}

interior vacuum
probatio_assetum_absens (Piscina* piscina)
{
    VitreaServus* s = _servum_creare(piscina);
    TcpConnexio*  c;
    character     buffer[8192];

    printf("--- asseta: via extra capsulam -> CDIV ---\n");
    CREDO_NON_NIHIL(s);
    c = _cliens_connectere(piscina, vitrea_servus_portus(s));
    CREDO_NON_NIHIL(c);
    CREDO_VERUM(_commercium(s, c,
        "GET /nusquam.html HTTP/1.1\r\nHost: x\r\n\r\n", buffer,
        (i32)magnitudo(buffer)) > ZEPHYRUM);
    CREDO_VERUM(strstr(buffer, "404") != NIHIL);

    /* traversalis non est res: viae ex capsula veniunt, non ex
     * petitione - ergo haec quoque simpliciter absens est */
    c = _cliens_connectere(piscina, vitrea_servus_portus(s));
    CREDO_NON_NIHIL(c);
    CREDO_VERUM(_commercium(s, c,
        "GET /../../etc/passwd HTTP/1.1\r\nHost: x\r\n\r\n", buffer,
        (i32)magnitudo(buffer)) > ZEPHYRUM);
    CREDO_VERUM(strstr(buffer, "root:") == NIHIL);
    vitrea_servus_destruere(s);
}

/* ========================================================================
 * CUSTODIA
 * ======================================================================== */

#define TESSERA_PROBATIONIS \
    "abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789"

interior VitreaServus*
_servum_custoditum (Piscina* piscina, constans character* hospes,
    constans character* tessera)
{
    VitreaServusConfiguratio figura;
    Capsula* capsula = capsula_aperire(&capsula_templates, piscina);

    si (capsula == NIHIL)
    {
        redde NIHIL;
    }
    memset(&figura, ZEPHYRUM, magnitudo(figura));
    figura.capsula         = capsula;
    figura.via_initialis   = "index.html";
    figura.praebitor       = _praebitor;
    figura.praebitor_datum = NIHIL;
    figura.portus          = ZEPHYRUM;
    figura.hospes          = hospes;
    figura.tessera         = tessera;
    redde vitrea_servus_creare(piscina, &figura);
}

/* PORTA NATIVITATIS: expositio sine tessera = REFUSIO.
 * Haec probatio est cur porta in strato sedet, non in app. */
interior vacuum
probatio_porta_nativitatis (Piscina* piscina)
{
    printf("--- porta nativitatis: expositio sine tessera ---\n");

    /* hospes datus, tessera nulla -> NIHIL */
    CREDO_NIHIL(_servum_custoditum(piscina, "127.0.0.1", NIHIL));
    /* tessera nimis brevis -> NIHIL (secretum debile = nullum) */
    CREDO_NIHIL(_servum_custoditum(piscina, "127.0.0.1", "brevis"));
    /* hospes datus + tessera valida -> licet */
    {
        VitreaServus* s = _servum_custoditum(piscina, "127.0.0.1",
            TESSERA_PROBATIONIS);

        CREDO_NON_NIHIL(s);
        vitrea_servus_destruere(s);
    }
    /* sine hospite (loopback) tessera non poscitur */
    {
        VitreaServus* s = _servum_custoditum(piscina, NIHIL, NIHIL);

        CREDO_NON_NIHIL(s);
        vitrea_servus_destruere(s);
    }
}

interior vacuum
probatio_custodia (Piscina* piscina)
{
    VitreaServus* s = _servum_custoditum(piscina, NIHIL,
        TESSERA_PROBATIONIS);
    TcpConnexio*  c;
    character     buffer[16384];
    character     petitio[2048];

    printf("--- custodia: crustulum, redirectio, CSRF ---\n");
    CREDO_NON_NIHIL(s);

    /* sine tessera: CDI */
    c = _cliens_connectere(piscina, vitrea_servus_portus(s));
    CREDO_NON_NIHIL(c);
    CREDO_VERUM(_commercium(s, c, "GET / HTTP/1.1\r\nHost: x\r\n\r\n",
        buffer, (i32)magnitudo(buffer)) > ZEPHYRUM);
    CREDO_VERUM(strstr(buffer, "401") != NIHIL);
    /* pagina NON transit */
    CREDO_VERUM(strstr(buffer, "salve vitrea") == NIHIL);

    /* tessera falsa: CDI */
    c = _cliens_connectere(piscina, vitrea_servus_portus(s));
    CREDO_NON_NIHIL(c);
    CREDO_VERUM(_commercium(s, c,
        "GET /?clavis=0000000000000000000000000000000000000000"
        "000000000000000000000000 HTTP/1.1\r\nHost: x\r\n\r\n",
        buffer, (i32)magnitudo(buffer)) > ZEPHYRUM);
    CREDO_VERUM(strstr(buffer, "401") != NIHIL);

    /* tessera vera in URL: CCCII + crustulum + Location */
    sprintf(petitio, "GET /?clavis=%s HTTP/1.1\r\nHost: x\r\n\r\n",
        TESSERA_PROBATIONIS);
    c = _cliens_connectere(piscina, vitrea_servus_portus(s));
    CREDO_NON_NIHIL(c);
    CREDO_VERUM(_commercium(s, c, petitio, buffer,
        (i32)magnitudo(buffer)) > ZEPHYRUM);
    CREDO_VERUM(strstr(buffer, "302") != NIHIL);
    CREDO_VERUM(strstr(buffer, "HttpOnly") != NIHIL);
    CREDO_VERUM(strstr(buffer, "SameSite=Strict") != NIHIL);
    CREDO_VERUM(strstr(buffer, "Location: /") != NIHIL);
    /* pagina in responso redirectionis NON venit */
    CREDO_VERUM(strstr(buffer, "salve vitrea") == NIHIL);

    /* crustulum validum: pagina transit */
    sprintf(petitio, "GET / HTTP/1.1\r\nHost: x\r\n"
        "Cookie: vitrea=%s\r\n\r\n", TESSERA_PROBATIONIS);
    c = _cliens_connectere(piscina, vitrea_servus_portus(s));
    CREDO_NON_NIHIL(c);
    CREDO_VERUM(_commercium(s, c, petitio, buffer,
        (i32)magnitudo(buffer)) > ZEPHYRUM);
    CREDO_VERUM(strstr(buffer, "200 OK") != NIHIL);
    CREDO_VERUM(strstr(buffer, "salve vitrea") != NIHIL);

    /* CSRF: crustulum validum SINE capite X-Vitrea -> recusatio.
     * Forma aliena crustulum ferre POTEST, caput NON potest.
     * (Capita per _pons_petere_cum, ne Content-Length manu iterum
     * numeretur - eadem decipula bis in eadem sessione momordit.) */
    sprintf(petitio, "Cookie: vitrea=%s\r\n", TESSERA_PROBATIONIS);
    CREDO_VERUM(_pons_petere_cum(s, piscina,
        "{\"id\":9,\"methodus\":\"salve\",\"argumenta\":{}}",
        petitio, buffer, (i32)magnitudo(buffer)) > ZEPHYRUM);
    CREDO_VERUM(strstr(buffer, "X-Vitrea") != NIHIL);
    CREDO_VERUM(strstr(buffer, "salve munde") == NIHIL);

    /* NEXUS: scriptum SERVITUM caput mittit quod porta POSCIT.
     *
     * Haec probatio ex defectu vivo nata est (2026-08-01): scriptum
     * caput non mittebat, porta id poscebat, ergo omnis pagina in
     * telephono "defectus lectionis" monstrabat. Utraque pars sola
     * VIRIDIS erat - probationes custodiae capita manu ponebant,
     * probationes scripti sine custodia currebant. Nemo NEXUM
     * probavit. Hic eum probamus. */
    sprintf(petitio, "GET / HTTP/1.1\r\nHost: x\r\n"
        "Cookie: vitrea=%s\r\n\r\n", TESSERA_PROBATIONIS);
    c = _cliens_connectere(piscina, vitrea_servus_portus(s));
    CREDO_NON_NIHIL(c);
    CREDO_VERUM(_commercium(s, c, petitio, buffer,
        (i32)magnitudo(buffer)) > ZEPHYRUM);
    CREDO_VERUM(strstr(buffer, "X-Vitrea") != NIHIL);

    /* crustulum + caput: transit */
    sprintf(petitio, "Cookie: vitrea=%s\r\nX-Vitrea: 1\r\n",
        TESSERA_PROBATIONIS);
    CREDO_VERUM(_pons_petere_cum(s, piscina,
        "{\"id\":9,\"methodus\":\"salve\",\"argumenta\":{}}",
        petitio, buffer, (i32)magnitudo(buffer)) > ZEPHYRUM);
    CREDO_VERUM(strstr(buffer, "salve munde") != NIHIL);

    vitrea_servus_destruere(s);
}

/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

s32
principale (vacuum)
{
    Piscina* piscina;
    b32      successus;

    printf("\n");
    printf("========================================\n");
    printf("PROBATIONES VITREA_SERVUS\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_vitrea_servus",
        M * M * IV);
    credo_aperire(piscina);

    probatio_creare(piscina);
    probatio_pagina_scriptum_fert(piscina);
    probatio_pons_vocatio(piscina);
    probatio_porta_harnesii(piscina);
    probatio_culpae_non_frangunt(piscina);
    probatio_assetum_absens(piscina);
    probatio_porta_nativitatis(piscina);
    probatio_custodia(piscina);

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? ZEPHYRUM : I;
}
