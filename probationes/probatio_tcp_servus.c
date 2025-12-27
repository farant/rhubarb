/* probatio_tcp_servus.c - Probationes pro TCP Server
 *
 * Probationes functionum server: creare, auscultare, accipere.
 */

#include "tcp.h"
#include "http.h"
#include "router.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>


/* ========================================================================
 * PROBATIONES - OPTIONES DEFAULT
 * ======================================================================== */

interior vacuum
probatio_servus_optiones_default(vacuum)
{
    TcpServusOptiones opt;

    printf("--- Probans servus optiones default ---\n");

    opt = tcp_servus_optiones_default();
    CREDO_VERUM(opt.tergum == CXXVIII);
    CREDO_VERUM(opt.reuti_inscriptio == VERUM);
    CREDO_VERUM(opt.non_blocans == VERUM);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CREARE SERVER
 * ======================================================================== */

interior vacuum
probatio_creare_simplex(Piscina* piscina)
{
    TcpServusResultus res;
    i32 portus;
    integer fd;

    printf("--- Probans creare simplex ---\n");

    /* Creare server in portus 0 (auto-select) */
    res = tcp_servus_creare(0, piscina);
    CREDO_VERUM(res.successus);
    CREDO_NON_NIHIL(res.servus);

    /* Verificare portus assignatus */
    portus = tcp_servus_obtinere_portum(res.servus);
    printf("  Portus assignatus: %u\n", portus);
    CREDO_VERUM(portus > 0);

    /* Verificare fd */
    fd = tcp_servus_obtinere_fd(res.servus);
    CREDO_VERUM(fd >= 0);

    tcp_servus_claudere(res.servus);
    printf("\n");
}


interior vacuum
probatio_creare_cum_optionibus(Piscina* piscina)
{
    TcpServusResultus res;
    TcpServusOptiones opt;
    i32 portus;

    printf("--- Probans creare cum optionibus ---\n");

    opt = tcp_servus_optiones_default();
    opt.non_blocans = FALSUM;  /* Blocking mode */

    res = tcp_servus_creare_cum_optionibus(NIHIL, 0, &opt, piscina);
    CREDO_VERUM(res.successus);
    CREDO_NON_NIHIL(res.servus);

    portus = tcp_servus_obtinere_portum(res.servus);
    printf("  Portus assignatus: %u\n", portus);
    CREDO_VERUM(portus > 0);

    tcp_servus_claudere(res.servus);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - AUSCULTARE
 * ======================================================================== */

interior vacuum
probatio_auscultare(Piscina* piscina)
{
    TcpServusResultus res;
    TcpError err;

    printf("--- Probans auscultare ---\n");

    res = tcp_servus_creare(0, piscina);
    CREDO_VERUM(res.successus);

    /* Auscultare */
    err = tcp_servus_auscultare(res.servus, X);
    CREDO_VERUM(err == TCP_OK);

    tcp_servus_claudere(res.servus);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - ADDRESS
 * ======================================================================== */

interior vacuum
probatio_obtinere_address(Piscina* piscina)
{
    TcpServusResultus res;
    TcpAddress addr;

    printf("--- Probans obtinere address ---\n");

    res = tcp_servus_creare(0, piscina);
    CREDO_VERUM(res.successus);

    addr = tcp_servus_obtinere_address(res.servus);
    printf("  Address: %s:%d\n", addr.hospes, addr.portus);
    CREDO_VERUM(addr.portus > 0);

    tcp_servus_claudere(res.servus);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CONNEXIO CLIENT-SERVER
 * ======================================================================== */

interior vacuum
probatio_accipere_connexio(Piscina* piscina)
{
    TcpServusResultus servus_res;
    TcpResultus client_res;
    TcpResultus accept_res;
    TcpServusOptiones opt;
    TcpError err;
    i32 portus;
    TcpAddress peer;
    constans character* msg;
    constans character* resp;
    i32 sent;
    i32 recv_len;
    character buffer[CXXVIII];

    printf("--- Probans accipere connexio ---\n");

    /* Creare server in BLOCKING mode pro reliabile accept */
    opt = tcp_servus_optiones_default();
    opt.non_blocans = FALSUM;  /* Blocking mode evitat race condition */

    servus_res = tcp_servus_creare_cum_optionibus(NIHIL, 0, &opt, piscina);
    CREDO_VERUM(servus_res.successus);

    portus = tcp_servus_obtinere_portum(servus_res.servus);
    printf("  Server in portus: %u\n", portus);

    /* Auscultare */
    err = tcp_servus_auscultare(servus_res.servus, V);
    CREDO_VERUM(err == TCP_OK);

    /* Connectere client ad server */
    client_res = tcp_connectere("127.0.0.1", (i32)portus, piscina);
    CREDO_VERUM(client_res.successus);
    printf("  Client connexus\n");

    /* Accipere connexionem in server (blocking - expectat) */
    accept_res = tcp_servus_accipere(servus_res.servus, piscina);
    CREDO_VERUM(accept_res.successus);
    CREDO_NON_NIHIL(accept_res.connexio);
    printf("  Server acceptavit connexionem\n");

    /* Verificare peer address */
    peer = tcp_obtinere_peer_address(accept_res.connexio);
    printf("  Peer: %s:%d\n", peer.hospes, peer.portus);
    CREDO_VERUM(peer.portus > 0);

    /* Mittere data client -> server */
    msg = "Salve Server!";
    sent = tcp_mittere(client_res.connexio, (constans i8*)msg, (i32)strlen(msg));
    CREDO_VERUM(sent == (i32)strlen(msg));

    /* Recipere in server */
    recv_len = tcp_recipere(accept_res.connexio, (i8*)buffer, CXXVIII - I);
    CREDO_VERUM(recv_len > 0);
    buffer[recv_len] = '\0';
    printf("  Server recepit: %s\n", buffer);
    CREDO_VERUM(strcmp(buffer, msg) == 0);

    /* Mittere responsum server -> client */
    resp = "Salve Client!";
    sent = tcp_mittere(accept_res.connexio, (constans i8*)resp, (i32)strlen(resp));
    CREDO_VERUM(sent == (i32)strlen(resp));

    /* Recipere in client */
    recv_len = tcp_recipere(client_res.connexio, (i8*)buffer, CXXVIII - I);
    CREDO_VERUM(recv_len > 0);
    buffer[recv_len] = '\0';
    printf("  Client recepit: %s\n", buffer);
    CREDO_VERUM(strcmp(buffer, resp) == 0);

    tcp_claudere(client_res.connexio);
    tcp_claudere(accept_res.connexio);
    tcp_servus_claudere(servus_res.servus);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - NON-BLOCKING
 * ======================================================================== */

interior vacuum
probatio_non_blocans(Piscina* piscina)
{
    TcpServusResultus res;
    TcpResultus accept_res;
    TcpError err;

    printf("--- Probans non-blocking mode ---\n");

    /* Creare server (default non-blocking) */
    res = tcp_servus_creare(0, piscina);
    CREDO_VERUM(res.successus);

    err = tcp_servus_auscultare(res.servus, V);
    CREDO_VERUM(err == TCP_OK);

    /* Accipere sine connexiones (debet fallere immediate) */
    accept_res = tcp_servus_accipere(res.servus, piscina);
    CREDO_FALSUM(accept_res.successus);  /* Nulla connexio */
    printf("  Non-blocking accept: nullae connexiones (expectatum)\n");

    tcp_servus_claudere(res.servus);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PONERE NON-BLOCANS
 * ======================================================================== */

interior vacuum
probatio_ponere_non_blocans(Piscina* piscina)
{
    TcpResultus res;
    TcpError err;

    printf("--- Probans ponere non-blocans ---\n");

    /* Connectere ad httpbin.org */
    res = tcp_connectere("httpbin.org", LXXX, piscina);
    si (!res.successus)
    {
        printf("  (Saltans - non connexus ad rete)\n\n");
        redde;
    }

    /* Ponere non-blocking */
    err = tcp_ponere_non_blocans(res.connexio, VERUM);
    CREDO_VERUM(err == TCP_OK);
    printf("  Non-blocking mode activatus\n");

    /* Ponere blocking */
    err = tcp_ponere_non_blocans(res.connexio, FALSUM);
    CREDO_VERUM(err == TCP_OK);
    printf("  Blocking mode activatus\n");

    tcp_claudere(res.connexio);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - NULLUM ARGUMENTA
 * ======================================================================== */

interior vacuum
probatio_nullum_argumenta(Piscina* piscina)
{
    TcpServusResultus res;
    TcpResultus accept_res;
    TcpError err;

    printf("--- Probans nullum argumenta ---\n");

    /* Creare cum NIHIL piscina */
    res = tcp_servus_creare(0, NIHIL);
    CREDO_FALSUM(res.successus);

    /* Auscultare cum NIHIL servus */
    err = tcp_servus_auscultare(NIHIL, X);
    CREDO_VERUM(err != TCP_OK);

    /* Accipere cum NIHIL */
    accept_res = tcp_servus_accipere(NIHIL, piscina);
    CREDO_FALSUM(accept_res.successus);

    /* Ponere non-blocans cum NIHIL */
    err = tcp_ponere_non_blocans(NIHIL, VERUM);
    CREDO_VERUM(err != TCP_OK);

    printf("\n");
}


interior vacuum
probatio_client_nullum_argumenta(Piscina* piscina)
{
    TcpResultus res;
    i32 result;
    character buffer[XXXII];

    printf("--- Probans client nullum argumenta ---\n");

    /* Connectere cum NIHIL hospes */
    res = tcp_connectere(NIHIL, LXXX, piscina);
    CREDO_FALSUM(res.successus);

    /* Connectere cum NIHIL piscina */
    res = tcp_connectere("localhost", LXXX, NIHIL);
    CREDO_FALSUM(res.successus);

    /* Mittere cum NIHIL connexio - returns (i32)-1 which is 0xFFFFFFFF */
    result = tcp_mittere(NIHIL, (constans i8*)"test", IV);
    CREDO_VERUM(result == (i32)-1);

    /* Recipere cum NIHIL connexio */
    result = tcp_recipere(NIHIL, (i8*)buffer, XXXII);
    CREDO_VERUM(result == (i32)-1);

    /* Obtinere fd cum NIHIL */
    result = tcp_obtinere_fd(NIHIL);
    CREDO_VERUM(result == (i32)-1);

    /* Est valida cum NIHIL */
    CREDO_FALSUM(tcp_est_valida(NIHIL));

    /* Claudere cum NIHIL - non debet crash */
    tcp_claudere(NIHIL);

    printf("  Client nullum handled\n");
    printf("\n");
}


interior vacuum
probatio_error_descriptio(vacuum)
{
    constans character* desc;

    printf("--- Probans error descriptio ---\n");

    desc = tcp_error_descriptio(TCP_OK);
    CREDO_NON_NIHIL(desc);
    printf("  TCP_OK: %s\n", desc);

    desc = tcp_error_descriptio(TCP_ERROR_DNS);
    CREDO_NON_NIHIL(desc);
    printf("  TCP_ERROR_DNS: %s\n", desc);

    desc = tcp_error_descriptio(TCP_ERROR_CONNEXIO);
    CREDO_NON_NIHIL(desc);
    printf("  TCP_ERROR_CONNEXIO: %s\n", desc);

    desc = tcp_error_descriptio(TCP_ERROR_TIMEOUT);
    CREDO_NON_NIHIL(desc);
    printf("  TCP_ERROR_TIMEOUT: %s\n", desc);

    desc = tcp_error_descriptio(TCP_ERROR_CLAUSUM);
    CREDO_NON_NIHIL(desc);
    printf("  TCP_ERROR_CLAUSUM: %s\n", desc);

    desc = tcp_error_descriptio(TCP_ERROR_IO);
    CREDO_NON_NIHIL(desc);
    printf("  TCP_ERROR_IO: %s\n", desc);

    printf("\n");
}


interior vacuum
probatio_optiones_client(vacuum)
{
    TcpOptiones opt;

    printf("--- Probans client optiones default ---\n");

    opt = tcp_optiones_default();
    printf("  timeout_ms: %d\n", opt.timeout_ms);
    printf("  nodelay: %d\n", opt.nodelay);
    printf("  keepalive: %d\n", opt.keepalive);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - MULTI CONNEXIONES
 * ======================================================================== */

interior vacuum
probatio_multi_connexiones(Piscina* piscina)
{
    TcpServusResultus servus_res;
    TcpError err;
    i32 portus;
    TcpConnexio* clients[V];
    TcpConnexio* server_conns[V];
    i32 i;
    character hospes[XXXII];
    character buffer[LXIV];
    i32 n;

    printf("--- Probans multi connexiones ---\n");

    /* Creare server */
    servus_res = tcp_servus_creare(0, piscina);
    CREDO_VERUM(servus_res.successus);
    CREDO_NON_NIHIL(servus_res.servus);

    err = tcp_servus_auscultare(servus_res.servus, X);
    CREDO_VERUM(err == TCP_OK);

    portus = tcp_servus_obtinere_portum(servus_res.servus);
    sprintf(hospes, "127.0.0.1");
    printf("  Server in portus: %d\n", portus);

    /* Connectere V clients */
    per (i = 0; i < V; i++)
    {
        TcpResultus res = tcp_connectere(hospes, portus, piscina);
        si (!res.successus)
        {
            printf("  Client %d fallitus\n", i);
            clients[i] = NIHIL;
        }
        alioquin
        {
            clients[i] = res.connexio;
        }
    }

    /* Acceptare V connexiones */
    per (i = 0; i < V; i++)
    {
        TcpResultus accept_res = tcp_servus_accipere(servus_res.servus, piscina);
        si (!accept_res.successus)
        {
            printf("  Accept %d fallitus\n", i);
            server_conns[i] = NIHIL;
        }
        alioquin
        {
            server_conns[i] = accept_res.connexio;
        }
    }

    /* Verificare V connexiones acceptatae */
    per (i = 0; i < V; i++)
    {
        CREDO_NON_NIHIL(clients[i]);
        CREDO_NON_NIHIL(server_conns[i]);
    }
    printf("  %d connexiones acceptatae\n", V);

    /* Mittere datum ex omni client */
    per (i = 0; i < V; i++)
    {
        character msg[XVI];
        sprintf(msg, "Client%d", i);
        n = tcp_mittere(clients[i], (constans i8*)msg, (i32)strlen(msg));
        CREDO_VERUM(n > 0);
    }

    /* Breve mora ut data arrivat */
    usleep(X * M);  /* 10ms */

    /* Recipere in server */
    per (i = 0; i < V; i++)
    {
        n = tcp_recipere(server_conns[i], (i8*)buffer, LXIV - I);
        si (n > 0)
        {
            buffer[n] = '\0';
            printf("  Server recepit: %s\n", buffer);
        }
        alioquin
        {
            printf("  Server: nullum datum (n=%d)\n", n);
        }
    }

    /* Claudere omnes */
    per (i = 0; i < V; i++)
    {
        tcp_claudere(clients[i]);
        tcp_claudere(server_conns[i]);
    }

    tcp_servus_claudere(servus_res.servus);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - INTEGRATIO HTTP
 * ======================================================================== */

/* Handler pro test route */
interior vacuum
handler_integratio(HttpPetitioServeri* petitio, HttpResponsum* responsum, RoutaParams* params)
{
    (vacuum)petitio;
    (vacuum)params;
    responsum->status = CC;
}

interior vacuum
probatio_integratio_http(Piscina* piscina)
{
    TcpServusResultus servus_res;
    TcpError err;
    i32 portus;
    TcpResultus client_res;
    TcpResultus accept_res;
    Router* router;
    character hospes[XXXII];
    character buffer[M];
    i32 n;

    /* HTTP request et response */
    constans character* http_request =
        "GET /api/test HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "\r\n";
    HttpParseResultus parse_res;
    RoutaResultus routa_res;
    HttpResponsum responsum;
    chorda serialized;

    printf("--- Probans integratio HTTP ---\n");

    /* 1. Creare server */
    servus_res = tcp_servus_creare(0, piscina);
    CREDO_VERUM(servus_res.successus);

    err = tcp_servus_auscultare(servus_res.servus, V);
    CREDO_VERUM(err == TCP_OK);

    portus = tcp_servus_obtinere_portum(servus_res.servus);
    sprintf(hospes, "127.0.0.1");
    printf("  Server in portus: %d\n", portus);

    /* 2. Creare router */
    router = router_creare(piscina);
    CREDO_NON_NIHIL(router);
    router_get(router, "/api/test", handler_integratio);

    /* 3. Connectere client */
    client_res = tcp_connectere(hospes, portus, piscina);
    CREDO_VERUM(client_res.successus);
    printf("  Client connexus\n");

    /* 4. Acceptare */
    accept_res = tcp_servus_accipere(servus_res.servus, piscina);
    CREDO_VERUM(accept_res.successus);
    printf("  Server acceptavit\n");

    /* 5. Client mittit HTTP request */
    n = tcp_mittere(client_res.connexio, (constans i8*)http_request,
                    (i32)strlen(http_request));
    CREDO_VERUM(n > 0);
    printf("  Client misit: GET /api/test\n");

    usleep(X * M);

    /* 6. Server recipit et parse */
    n = tcp_recipere(accept_res.connexio, (i8*)buffer, M - I);
    CREDO_VERUM(n > 0);
    buffer[n] = '\0';

    parse_res = http_petitio_parse(buffer, n, piscina);
    CREDO_VERUM(parse_res.successus);
    CREDO_VERUM(parse_res.completa);
    CREDO_VERUM(parse_res.petitio->methodus == HTTP_GET);
    printf("  Server parsevit: %s %.*s\n",
           http_methodus_nomen(parse_res.petitio->methodus),
           parse_res.petitio->via.mensura,
           parse_res.petitio->via.datum);

    /* 7. Router matching */
    routa_res = router_matching(router, parse_res.petitio->methodus,
                                parse_res.petitio->via, piscina);
    CREDO_VERUM(routa_res.invenit);
    printf("  Router invenit handler\n");

    /* 8. Vocare handler */
    memset(&responsum, 0, magnitudo(responsum));
    routa_res.handler(parse_res.petitio, &responsum, &routa_res.params);
    CREDO_VERUM(responsum.status == CC);

    /* 9. Serialize responsum */
    responsum.corpus = chorda_ex_literis("{\"status\":\"ok\"}", piscina);
    serialized = http_responsum_serialize(&responsum, piscina);
    CREDO_VERUM(serialized.mensura > 0);
    printf("  Serialized %d bytes\n", serialized.mensura);

    /* 10. Mittere responsum */
    n = tcp_mittere(accept_res.connexio, (constans i8*)serialized.datum,
                    serialized.mensura);
    CREDO_VERUM(n == serialized.mensura);

    usleep(X * M);

    /* 11. Client recipit responsum */
    n = tcp_recipere(client_res.connexio, (i8*)buffer, M - I);
    CREDO_VERUM(n > 0);
    buffer[n] = '\0';

    CREDO_VERUM(strstr(buffer, "HTTP/1.1 200 OK") != NIHIL);
    CREDO_VERUM(strstr(buffer, "{\"status\":\"ok\"}") != NIHIL);
    printf("  Client recepit: 200 OK\n");

    tcp_claudere(client_res.connexio);
    tcp_claudere(accept_res.connexio);
    tcp_servus_claudere(servus_res.servus);

    printf("  INTEGRATIO COMPLETA!\n");
    printf("\n");
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale(vacuum)
{
    Piscina* piscina;
    b32 successus;

    printf("\n");
    printf("========================================\n");
    printf("PROBATIONES TCP SERVUS\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_tcp_servus", CXXVIII * M);
    credo_aperire(piscina);

    probatio_servus_optiones_default();
    probatio_creare_simplex(piscina);
    probatio_creare_cum_optionibus(piscina);
    probatio_auscultare(piscina);
    probatio_obtinere_address(piscina);
    probatio_accipere_connexio(piscina);
    probatio_non_blocans(piscina);
    probatio_ponere_non_blocans(piscina);
    probatio_nullum_argumenta(piscina);
    probatio_client_nullum_argumenta(piscina);
    probatio_error_descriptio();
    probatio_optiones_client();
    probatio_multi_connexiones(piscina);
    probatio_integratio_http(piscina);

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
