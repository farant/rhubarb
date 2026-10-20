/* probatio_hospitium.c - Probationes pro servo HTTP domestico
 *
 * Exemplar: cliens obstruens (timeout II s - probatio numquam pendet)
 * + pumpatio manualis gressuum in eodem filo. Ordo: cliens mittit
 * (in buffer socket cadit), gressus pumpantur (accipere + legere +
 * expedire + scribere), cliens recipit.
 */

#include "hospitium.h"
#include "tcp.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"

#include <stdio.h>
#include <string.h>

#define STATICA_RADIX "/tmp/probatio_hospitium_static"


/* ========================================================================
 * AUXILIA
 * ======================================================================== */

interior vacuum
_pumpare (
    Hospitium* h,
          i32  vices)
{
    i32 i;
    per (i = 0; i < vices; i++)
    {
        hospitium_gressus(h, X);  /* 10ms */
    }
}

interior TcpConnexio*
_cliens_connectere (
    Piscina* piscina,
        i32  portus)
{
    TcpOptiones opt = tcp_optiones_default();
    TcpResultus res;

    opt.timeout_ms = MM;  /* 2s - recv fallit pro pendendo */
    res = tcp_connectere_cum_optionibus("127.0.0.1", portus, &opt,
        piscina);
    redde res.successus ? res.connexio : NIHIL;
}

/* Mittere petitionem, pumpare, recipere responsum (NUL-terminatum) */
interior s32
_commercium (
             Hospitium* h,
           TcpConnexio* cliens,
    constans character* petitio,
             character* buffer,
                   i32  capacitas)
{
    s32 n;

    si (!tcp_mittere_omnia(cliens, (constans i8*)petitio,
                           (i32)strlen(petitio)))
    {
        redde -1;
    }
    _pumpare(h, X);

    n = tcp_recipere(cliens, (i8*)buffer, capacitas - I);
    si (n > 0)
    {
        buffer[n] = '\0';
    }
    redde n;
}


/* ========================================================================
 * TRACTATORES
 * ======================================================================== */

interior vacuum
_tractator_salve (
    HospitiumColloquium* colloquium)
{
    chorda corpus = chorda_ex_literis("salve munde",
                                      colloquium_piscina(colloquium));
    colloquium_respondere(colloquium, CC, "text/plain", corpus);
}

interior vacuum
_tractator_resonans (
    HospitiumColloquium* colloquium)
{
    /* redde valorem parametri :id */
    chorda id = colloquium_param(colloquium, "id");
    colloquium_respondere(colloquium, CC, "text/plain", id);
}

interior vacuum
_tractator_datum (
    HospitiumColloquium* colloquium)
{
    /* redde datum registratum ut corpus */
    constans character* datum =
        (constans character*)colloquium_datum(colloquium);
    chorda corpus = chorda_ex_literis(datum,
                                      colloquium_piscina(colloquium));
    colloquium_caput_addere(colloquium, "X-Probatio", "verum");
    colloquium_respondere(colloquium, CC, NIHIL, corpus);
}


/* ========================================================================
 * PROBATIONES - CYCLUS VITAE
 * ======================================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                  Piscina* piscina;
    HospitiumConfiguratio  cfg;
                Hospitium* h;
         HospitiumFructus  fr;

    piscina = c->piscina;

    memset(&cfg, 0, magnitudo(cfg));
    h = hospitium_creare(piscina, &cfg);
    CREDO_NON_NIHIL(h);
    CREDO_VERUM(hospitium_portus(h) > 0);  /* portus 0 = auto-selectus */

    fr = hospitium_fructus(h);
    CREDO_VERUM(fr.connexiones_acceptae == 0);
    CREDO_VERUM(fr.responsa_missa == 0);

    hospitium_destruere(h);
    printf("  Portus auto-selectus, fructus vacui\n");
}


/* ========================================================================
 * PROBATIONES - GYRUS SIMPLEX
 * ======================================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                  Piscina* piscina;
    HospitiumConfiguratio  cfg;
                Hospitium* h;
              TcpConnexio* cliens;
                character  buffer[IV * MXXIV];
                      s32  n;
         HospitiumFructus  fr;

    piscina = c->piscina;

    memset(&cfg, 0, magnitudo(cfg));
    h = hospitium_creare(piscina, &cfg);
    CREDO_NON_NIHIL(h);
    CREDO_VERUM(hospitium_praebere(h, HTTP_GET, "/salve",
                                   _tractator_salve, NIHIL));

    cliens = _cliens_connectere(piscina, hospitium_portus(h));
    CREDO_NON_NIHIL(cliens);
    _pumpare(h, V);

    n = _commercium(h, cliens,
        "GET /salve HTTP/1.1\r\nHost: probatio\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 200 OK") != NIHIL);
    CREDO_VERUM(strstr(buffer, "Content-Type: text/plain") != NIHIL);
    CREDO_VERUM(strstr(buffer, "Content-Length: 11") != NIHIL);
    CREDO_VERUM(strstr(buffer, "Connection: keep-alive") != NIHIL);
    CREDO_VERUM(strstr(buffer, "salve munde") != NIHIL);

    fr = hospitium_fructus(h);
    CREDO_VERUM(fr.connexiones_acceptae == I);
    CREDO_VERUM(fr.petitiones_tractatae == I);
    CREDO_VERUM(fr.responsa_missa == I);

    tcp_claudere(cliens);
    hospitium_destruere(h);
    printf("  Gyrus completus: 200 + corpus + framing\n");
}


/* ========================================================================
 * PROBATIONES - KEEP-ALIVE
 * ======================================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
                  Piscina* piscina;
    HospitiumConfiguratio  cfg;
                Hospitium* h;
              TcpConnexio* cliens;
                character  buffer[IV * MXXIV];
                      s32  n;
         HospitiumFructus  fr;

    piscina = c->piscina;

    memset(&cfg, 0, magnitudo(cfg));
    h = hospitium_creare(piscina, &cfg);
    CREDO_NON_NIHIL(h);
    hospitium_praebere(h, HTTP_GET, "/salve", _tractator_salve, NIHIL);
    hospitium_praebere(h, HTTP_GET, "/res/:id", _tractator_resonans,
        NIHIL);

    cliens = _cliens_connectere(piscina, hospitium_portus(h));
    CREDO_NON_NIHIL(cliens);
    _pumpare(h, V);

    /* petitio prima */
    n = _commercium(h, cliens,
        "GET /salve HTTP/1.1\r\nHost: probatio\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "salve munde") != NIHIL);

    /* petitio secunda in EADEM connexione - refectio + parser novus */
    n = _commercium(h, cliens,
        "GET /res/42 HTTP/1.1\r\nHost: probatio\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 200 OK") != NIHIL);
    CREDO_VERUM(strstr(buffer, "Content-Length: 2") != NIHIL);
    CREDO_VERUM(strstr(buffer, "42") != NIHIL);

    fr = hospitium_fructus(h);
    CREDO_VERUM(fr.connexiones_acceptae == I);  /* UNA connexio */
    CREDO_VERUM(fr.responsa_missa == II);

    tcp_claudere(cliens);
    hospitium_destruere(h);
    printf("  II petitiones per I connexionem\n");
}


/* ========================================================================
 * PROBATIONES - CDIV / CDV + ALLOW
 * ======================================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
                  Piscina* piscina;
    HospitiumConfiguratio  cfg;
                Hospitium* h;
              TcpConnexio* cliens;
                character  buffer[IV * MXXIV];
                      s32  n;

    piscina = c->piscina;

    memset(&cfg, 0, magnitudo(cfg));
    h = hospitium_creare(piscina, &cfg);
    CREDO_NON_NIHIL(h);
    hospitium_praebere(h, HTTP_GET, "/salve", _tractator_salve, NIHIL);

    /* via ignota -> CDIV */
    cliens = _cliens_connectere(piscina, hospitium_portus(h));
    CREDO_NON_NIHIL(cliens);
    _pumpare(h, V);
    n = _commercium(h, cliens,
        "GET /nusquam HTTP/1.1\r\nHost: probatio\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 404 Not Found") != NIHIL);
    CREDO_VERUM(strstr(buffer, "Content-Length: 0") != NIHIL);
    tcp_claudere(cliens);

    /* methodus falsa -> CDV cum Allow */
    cliens = _cliens_connectere(piscina, hospitium_portus(h));
    CREDO_NON_NIHIL(cliens);
    _pumpare(h, V);
    n = _commercium(h, cliens,
        "POST /salve HTTP/1.1\r\nHost: probatio\r\nContent-Length: 0\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 405 Method Not Allowed")
        != NIHIL);
    CREDO_VERUM(strstr(buffer, "Allow: GET, HEAD, OPTIONS") != NIHIL);
    tcp_claudere(cliens);

    hospitium_destruere(h);
    printf("  CDIV + CDV cum Allow recte\n");
}


/* ========================================================================
 * PROBATIONES - CUSTOS TRANS FILUM
 * ======================================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
                  Piscina* piscina;
    HospitiumConfiguratio  cfg;
                Hospitium* h;
              TcpConnexio* cliens;
                character  buffer[IV * MXXIV];
                      s32  n;
         HospitiumFructus  fr;

    piscina = c->piscina;

    memset(&cfg, 0, magnitudo(cfg));
    h = hospitium_creare(piscina, &cfg);
    CREDO_NON_NIHIL(h);

    cliens = _cliens_connectere(piscina, hospitium_portus(h));
    CREDO_NON_NIHIL(cliens);
    _pumpare(h, V);

    n = _commercium(h, cliens, "GET / HTTP/9.9\r\n\r\n",
                    buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 400 Bad Request") != NIHIL);
    CREDO_VERUM(strstr(buffer, "Connection: close") != NIHIL);

    /* post responsum custodis: clausura honesta -> EOF */
    _pumpare(h, V);
    n = tcp_recipere(cliens, (i8*)buffer, IV * MXXIV - I);
    CREDO_VERUM(n == 0);

    fr = hospitium_fructus(h);
    CREDO_VERUM(fr.petitiones_reiectae == I);

    tcp_claudere(cliens);
    hospitium_destruere(h);
    printf("  Custos: 400 + close + EOF\n");
}


/* ========================================================================
 * PROBATIONES - CAPUT (HEAD)
 * ======================================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
                  Piscina* piscina;
    HospitiumConfiguratio  cfg;
                Hospitium* h;
              TcpConnexio* cliens;
                character  buffer[IV * MXXIV];
                      s32  n;

    piscina = c->piscina;

    memset(&cfg, 0, magnitudo(cfg));
    h = hospitium_creare(piscina, &cfg);
    CREDO_NON_NIHIL(h);
    hospitium_praebere(h, HTTP_GET, "/salve", _tractator_salve, NIHIL);

    cliens = _cliens_connectere(piscina, hospitium_portus(h));
    CREDO_NON_NIHIL(cliens);
    _pumpare(h, V);

    n = _commercium(h, cliens,
        "HEAD /salve HTTP/1.1\r\nHost: probatio\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 200 OK") != NIHIL);
    /* Content-Length verum corporis GET... */
    CREDO_VERUM(strstr(buffer, "Content-Length: 11") != NIHIL);
    /* ...sed corpus ipsum omissum */
    CREDO_VERUM(strstr(buffer, "salve munde") == NIHIL);
    /* responsum in fine capitum terminatur */
    CREDO_VERUM(memcmp(buffer + n - IV, "\r\n\r\n", IV) == 0);

    tcp_claudere(cliens);
    hospitium_destruere(h);
    printf("  HEAD: capita cum CL vero, sine corpore\n");
}


/* ========================================================================
 * PROBATIONES - RELIQUIAE (CLAUSURA HONESTA)
 * ======================================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
                  Piscina* piscina;
    HospitiumConfiguratio  cfg;
                Hospitium* h;
              TcpConnexio* cliens;
                character  buffer[IV * MXXIV];
                      s32  n;
         HospitiumFructus  fr;

    piscina = c->piscina;

    memset(&cfg, 0, magnitudo(cfg));
    h = hospitium_creare(piscina, &cfg);
    CREDO_NON_NIHIL(h);
    hospitium_praebere(h, HTTP_GET, "/salve", _tractator_salve, NIHIL);

    cliens = _cliens_connectere(piscina, hospitium_portus(h));
    CREDO_NON_NIHIL(cliens);
    _pumpare(h, V);

    /* duae petitiones UNO ictu = pipelining = reliquiae */
    n = _commercium(h, cliens,
        "GET /salve HTTP/1.1\r\nHost: a\r\n\r\n"
        "GET /salve HTTP/1.1\r\nHost: b\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 200 OK") != NIHIL);
    CREDO_VERUM(strstr(buffer, "Connection: close") != NIHIL);

    /* petitio secunda NON servitur - clausura honesta */
    _pumpare(h, V);
    n = tcp_recipere(cliens, (i8*)buffer, IV * MXXIV - I);
    CREDO_VERUM(n == 0);

    fr = hospitium_fructus(h);
    CREDO_VERUM(fr.clausae_reliquiis == I);
    CREDO_VERUM(fr.responsa_missa == I);

    tcp_claudere(cliens);
    hospitium_destruere(h);
    printf("  Reliquiae: responsum I + close + EOF\n");
}


/* ========================================================================
 * PROBATIONES - DATUM ET CAPUT PROPRIUM
 * ======================================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
                          Piscina* piscina;
            HospitiumConfiguratio  cfg;
                        Hospitium* h;
                      TcpConnexio* cliens;
                        character  buffer[IV * MXXIV];
                              s32  n;
    hic_manens constans character* datum_probationis = "datum-arcanum";

    piscina = c->piscina;

    memset(&cfg, 0, magnitudo(cfg));
    h = hospitium_creare(piscina, &cfg);
    CREDO_NON_NIHIL(h);
    hospitium_praebere(h, HTTP_GET, "/datum", _tractator_datum,
                       (vacuum*)(size_t)datum_probationis);

    cliens = _cliens_connectere(piscina, hospitium_portus(h));
    CREDO_NON_NIHIL(cliens);
    _pumpare(h, V);

    n = _commercium(h, cliens,
        "GET /datum HTTP/1.1\r\nHost: probatio\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "X-Probatio: verum") != NIHIL);
    CREDO_VERUM(strstr(buffer, "datum-arcanum") != NIHIL);

    tcp_claudere(cliens);
    hospitium_destruere(h);
    printf("  Datum + caput proprium per filum\n");
}


/* ========================================================================
 * PROBATIONES - DIRECTORIUM STATICUM
 * ======================================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
                  Piscina* piscina;
    HospitiumConfiguratio  cfg;
                Hospitium* h;
              TcpConnexio* cliens;
                character  buffer[IV * MXXIV];
                      s32  n;

    piscina = c->piscina;

    /* figere directorium probationis */
    CREDO_VERUM(filum_directorium_creare_si_necesse(STATICA_RADIX));
    CREDO_VERUM(filum_directorium_creare_si_necesse(STATICA_RADIX "/sub"));
    CREDO_VERUM(filum_scribere_literis(STATICA_RADIX "/index.html",
                                       "<h1>salve statica</h1>"));
    CREDO_VERUM(filum_scribere_literis(STATICA_RADIX "/stilus.css",
                                       "body{color:red}"));
    CREDO_VERUM(filum_scribere_literis(STATICA_RADIX "/sub/res.txt",
                                       "contentum subdirectorii"));

    memset(&cfg, 0, magnitudo(cfg));
    h = hospitium_creare(piscina, &cfg);
    CREDO_NON_NIHIL(h);
    CREDO_VERUM(hospitium_directorium_servire(h, "/static",
        STATICA_RADIX));

    cliens = _cliens_connectere(piscina, hospitium_portus(h));
    CREDO_NON_NIHIL(cliens);
    _pumpare(h, V);

    /* filum cum typo MIME recto */
    n = _commercium(h, cliens,
        "GET /static/stilus.css HTTP/1.1\r\nHost: probatio\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 200 OK") != NIHIL);
    CREDO_VERUM(strstr(buffer, "Content-Type: text/css") != NIHIL);
    CREDO_VERUM(strstr(buffer, "body{color:red}") != NIHIL);

    /* radix directorii -> index.html */
    n = _commercium(h, cliens,
        "GET /static HTTP/1.1\r\nHost: probatio\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "Content-Type: text/html") != NIHIL);
    CREDO_VERUM(strstr(buffer, "salve statica") != NIHIL);

    /* subdirectorium sine index -> pagina indicis generata */
    n = _commercium(h, cliens,
        "GET /static/sub HTTP/1.1\r\nHost: probatio\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 200 OK") != NIHIL);
    CREDO_VERUM(strstr(buffer, "res.txt") != NIHIL);

    /* traversalis -> CDIII */
    n = _commercium(h, cliens,
        "GET /static/../../etc/passwd HTTP/1.1\r\nHost: probatio\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 403 Forbidden") != NIHIL);

    /* traversalis codificata -> CDIII (decodificatio prima) */
    n = _commercium(h, cliens,
        "GET /static/%2e%2e/%2e%2e/etc/passwd HTTP/1.1\r\nHost: p\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 403 Forbidden") != NIHIL);

    /* filum absens -> CDIV */
    n = _commercium(h, cliens,
        "GET /static/nusquam.txt HTTP/1.1\r\nHost: probatio\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 404 Not Found") != NIHIL);

    tcp_claudere(cliens);
    hospitium_destruere(h);
    printf("  Statica: MIME + index + pagina indicis + traversalis recusata\n");
}


/* ========================================================================
 * PROBATIONES - ACAO (postura dev)
 * ======================================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
                  Piscina* piscina;
    HospitiumConfiguratio  cfg;
                Hospitium* h;
              TcpConnexio* cliens;
                character  buffer[IV * MXXIV];
                      s32  n;

    piscina = c->piscina;

    memset(&cfg, 0, magnitudo(cfg));
    cfg.acao  = VERUM;
    h         = hospitium_creare(piscina, &cfg);
    CREDO_NON_NIHIL(h);
    hospitium_praebere(h, HTTP_GET, "/salve", _tractator_salve, NIHIL);

    cliens = _cliens_connectere(piscina, hospitium_portus(h));
    CREDO_NON_NIHIL(cliens);
    _pumpare(h, V);

    /* responsum tractatoris */
    n = _commercium(h, cliens,
        "GET /salve HTTP/1.1\r\nHost: probatio\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "Access-Control-Allow-Origin: *")
        != NIHIL);

    /* etiam CDIV - uniformis in OMNI responso */
    n = _commercium(h, cliens,
        "GET /nusquam HTTP/1.1\r\nHost: probatio\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 404 Not Found") != NIHIL);
    CREDO_VERUM(strstr(buffer, "Access-Control-Allow-Origin: *")
        != NIHIL);

    tcp_claudere(cliens);
    hospitium_destruere(h);
    printf("  ACAO in responso tractatoris et in CDIV\n");
}


/* ========================================================================
 * PROBATIONES - PLENITUDO (DIII)
 * ======================================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
                  Piscina* piscina;
    HospitiumConfiguratio  cfg;
                Hospitium* h;
              TcpConnexio* cliens_a;
              TcpConnexio* cliens_b;
                character  buffer[IV * MXXIV];
                      s32  n;
         HospitiumFructus  fr;

    piscina = c->piscina;

    memset(&cfg, 0, magnitudo(cfg));
    cfg.connexiones_maximae  = I;
    h                        = hospitium_creare(piscina, &cfg);
    CREDO_NON_NIHIL(h);
    hospitium_praebere(h, HTTP_GET, "/salve", _tractator_salve, NIHIL);

    /* cliens A sedem unicam occupat */
    cliens_a = _cliens_connectere(piscina, hospitium_portus(h));
    CREDO_NON_NIHIL(cliens_a);
    _pumpare(h, V);

    /* cliens B recusatur visibiliter */
    cliens_b = _cliens_connectere(piscina, hospitium_portus(h));
    CREDO_NON_NIHIL(cliens_b);
    _pumpare(h, V);

    n = tcp_recipere(cliens_b, (i8*)buffer, IV * MXXIV - I);
    CREDO_VERUM(n > 0);
    buffer[n] = '\0';
    CREDO_VERUM(strstr(buffer, "HTTP/1.1 503 Service Unavailable")
        != NIHIL);

    fr = hospitium_fructus(h);
    CREDO_VERUM(fr.connexiones_plenae_recusatae == I);
    CREDO_VERUM(fr.connexiones_acceptae == I);

    /* sedes A adhuc laborat */
    n = _commercium(h, cliens_a,
        "GET /salve HTTP/1.1\r\nHost: probatio\r\n\r\n",
        buffer, IV * MXXIV);
    CREDO_VERUM(n > 0);
    CREDO_VERUM(strstr(buffer, "salve munde") != NIHIL);

    tcp_claudere(cliens_a);
    tcp_claudere(cliens_b);
    hospitium_destruere(h);
    printf("  Plenitudo: DIII visibile, sedes prima intacta\n");
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

hic_manens constans CredoSectio SECTIONES[] = {
    { "creare et destruere", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "gyrum simplicem", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "keep-alive (II petitiones, I connexio)", _sectio_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "CDIV et CDV + Allow", _sectio_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "custodem trans filum (versio mala)", _sectio_quinta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "HEAD = GET sine corpore", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "reliquias -> clausuram honestam", _sectio_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "datum registratum + caput proprium", _sectio_octava, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "directorium staticum", _sectio_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ACAO in omni responso", _sectio_decima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "plenitudinem -> DIII", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("hospitium", SECTIONES);
}
