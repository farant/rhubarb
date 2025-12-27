/* probatio_tcp_servus.c - Probationes pro TCP Server
 *
 * Probationes functionum server: creare, auscultare, accipere.
 */

#include "tcp.h"
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

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
