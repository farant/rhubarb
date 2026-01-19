/* tls_linux.c - Implementatio TLS pro Linux (OpenSSL)
 *
 * Utitur OpenSSL pro TLS.
 *
 * NOTA: Includimus headers OpenSSL ANTE latina.h quia
 * macros Latinae (si, integer, L, etc.) conflictum habent cum OpenSSL.
 */

#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 200112L

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>
#include <unistd.h>

#include "tls.h"
#include "tcp.h"


/* ========================================================================
 * STRUCTURA INTERNA
 * ======================================================================== */

structura TlsConnexio {
    Piscina*     piscina;
    TcpConnexio* tcp;           /* Connexio TCP subiacens */
    SSL_CTX*     ssl_ctx;       /* OpenSSL context */
    SSL*         ssl;           /* OpenSSL connexio */
    b32          clausa;
};


/* ========================================================================
 * VARIABILES GLOBALES
 * ======================================================================== */

hic_manens b32 _openssl_initiatus = FALSUM;


/* ========================================================================
 * FUNCTIONES INTERNAE
 * ======================================================================== */

interior vacuum
_openssl_initiare(vacuum)
{
    si (!_openssl_initiatus)
    {
        SSL_library_init();
        SSL_load_error_strings();
        OpenSSL_add_all_algorithms();
        _openssl_initiatus = VERUM;
    }
}

interior TlsResultus
_creare_error(TlsError error, constans character* msg, Piscina* piscina)
{
    TlsResultus res;
    res.successus = FALSUM;
    res.connexio = NIHIL;
    res.error = error;
    si (msg && piscina)
    {
        res.error_descriptio = chorda_ex_literis(msg, piscina);
    }
    alioquin
    {
        res.error_descriptio.datum = NIHIL;
        res.error_descriptio.mensura = 0;
    }
    redde res;
}

interior constans character*
_openssl_error_string(vacuum)
{
    unsigned long err;
    err = ERR_get_error();
    si (err != 0)
    {
        redde ERR_error_string(err, NIHIL);
    }
    redde "Unknown OpenSSL error";
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - CONNEXIO
 * ======================================================================== */

TlsOptiones
tls_optiones_default(vacuum)
{
    TlsOptiones opt;
    opt.timeout_ms = XXX * M;  /* 30 seconds */
    opt.verificare_certificatum = VERUM;
    redde opt;
}

TlsResultus
tls_connectere(
    constans character* hospes,
    i32                 portus,
    Piscina*            piscina)
{
    TlsOptiones opt = tls_optiones_default();
    redde tls_connectere_cum_optionibus(hospes, portus, &opt, piscina);
}

TlsResultus
tls_connectere_cum_optionibus(
    constans character*   hospes,
    i32                   portus,
    constans TlsOptiones* optiones,
    Piscina*              piscina)
{
    TlsResultus   res;
    TlsConnexio*  conn;
    TcpResultus   tcp_res;
    TcpOptiones   tcp_opt;
    SSL_CTX*      ssl_ctx;
    SSL*          ssl;
    integer       fd;
    integer       ssl_result;

    /* Initiare OpenSSL si necesse */
    _openssl_initiare();

    /* Validare argumenta */
    si (!hospes || !piscina)
    {
        redde _creare_error(TLS_ERROR_TCP, "Argumenta invalida", piscina);
    }

    /* Connectere TCP */
    tcp_opt = tcp_optiones_default();
    si (optiones && optiones->timeout_ms > 0)
    {
        tcp_opt.timeout_ms = optiones->timeout_ms;
    }

    tcp_res = tcp_connectere_cum_optionibus(hospes, portus, &tcp_opt, piscina);
    si (!tcp_res.successus)
    {
        res.successus = FALSUM;
        res.connexio = NIHIL;
        res.error = TLS_ERROR_TCP;
        res.error_descriptio = tcp_res.error_descriptio;
        redde res;
    }

    /* Creare SSL context */
    ssl_ctx = SSL_CTX_new(TLS_client_method());
    si (!ssl_ctx)
    {
        tcp_claudere(tcp_res.connexio);
        redde _creare_error(TLS_ERROR_HANDSHAKE, _openssl_error_string(), piscina);
    }

    /* Configurare verificatio certificati */
    si (optiones && optiones->verificare_certificatum)
    {
        SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER, NIHIL);
        SSL_CTX_set_default_verify_paths(ssl_ctx);
    }
    alioquin
    {
        SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_NONE, NIHIL);
    }

    /* Creare SSL connexio */
    ssl = SSL_new(ssl_ctx);
    si (!ssl)
    {
        SSL_CTX_free(ssl_ctx);
        tcp_claudere(tcp_res.connexio);
        redde _creare_error(TLS_ERROR_HANDSHAKE, _openssl_error_string(), piscina);
    }

    /* Ponere hostname pro SNI */
    {
        character* hospes_copy;
        memcpy(&hospes_copy, &hospes, magnitudo(hospes_copy));
        SSL_set_tlsext_host_name(ssl, hospes_copy);
    }

    /* Obtinere file descriptor de TCP connexio */
    fd = (integer)tcp_obtinere_fd(tcp_res.connexio);
    si (fd < 0)
    {
        SSL_free(ssl);
        SSL_CTX_free(ssl_ctx);
        tcp_claudere(tcp_res.connexio);
        redde _creare_error(TLS_ERROR_TCP, "File descriptor invalidus", piscina);
    }

    /* Associare SSL cum socket */
    si (SSL_set_fd(ssl, fd) != I)
    {
        SSL_free(ssl);
        SSL_CTX_free(ssl_ctx);
        tcp_claudere(tcp_res.connexio);
        redde _creare_error(TLS_ERROR_HANDSHAKE, _openssl_error_string(), piscina);
    }

    /* Facere handshake */
    ssl_result = SSL_connect(ssl);
    si (ssl_result != I)
    {
        integer ssl_error;
        ssl_error = SSL_get_error(ssl, ssl_result);

        SSL_free(ssl);
        SSL_CTX_free(ssl_ctx);
        tcp_claudere(tcp_res.connexio);

        si (ssl_error == SSL_ERROR_SSL)
        {
            /* Verificare si error certificati */
            long verify_result;
            verify_result = SSL_get_verify_result(ssl);
            si (verify_result != X509_V_OK)
            {
                redde _creare_error(TLS_ERROR_CERTIFICATUM,
                    X509_verify_cert_error_string(verify_result), piscina);
            }
        }

        redde _creare_error(TLS_ERROR_HANDSHAKE, _openssl_error_string(), piscina);
    }

    /* Allocare connexio */
    conn = (TlsConnexio*)piscina_allocare(piscina, (memoriae_index)magnitudo(TlsConnexio));
    si (!conn)
    {
        SSL_free(ssl);
        SSL_CTX_free(ssl_ctx);
        tcp_claudere(tcp_res.connexio);
        redde _creare_error(TLS_ERROR_TCP, "Allocatio fallita", piscina);
    }

    conn->piscina = piscina;
    conn->tcp = tcp_res.connexio;
    conn->ssl_ctx = ssl_ctx;
    conn->ssl = ssl;
    conn->clausa = FALSUM;

    /* Successus! */
    res.successus = VERUM;
    res.connexio = conn;
    res.error = TLS_OK;
    res.error_descriptio.datum = NIHIL;
    res.error_descriptio.mensura = 0;

    redde res;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - I/O
 * ======================================================================== */

i32
tls_mittere(
    TlsConnexio*   connexio,
    constans i8*   data,
    i32            mensura)
{
    integer n;

    si (!connexio || connexio->clausa || !connexio->ssl)
    {
        redde (i32)-1;
    }

    si (!data || mensura <= 0)
    {
        redde 0;
    }

    n = SSL_write(connexio->ssl, data, (integer)mensura);

    si (n <= 0)
    {
        integer ssl_error;
        ssl_error = SSL_get_error(connexio->ssl, n);

        si (ssl_error == SSL_ERROR_WANT_WRITE || ssl_error == SSL_ERROR_WANT_READ)
        {
            redde 0;  /* Would block */
        }

        redde (i32)-1;
    }

    redde (i32)n;
}

b32
tls_mittere_omnia(
    TlsConnexio*   connexio,
    constans i8*   data,
    i32            mensura)
{
    i32 totalis = 0;
    i32 restans = mensura;

    si (!connexio || connexio->clausa || !data || mensura <= 0)
    {
        redde FALSUM;
    }

    dum (restans > 0)
    {
        i32 n = tls_mittere(connexio, data + totalis, restans);
        si (n < 0)
        {
            redde FALSUM;
        }
        si (n == 0)
        {
            /* Breve mora et retry */
            usleep(M);  /* 1ms */
            perge;
        }
        totalis += n;
        restans -= n;
    }

    redde VERUM;
}

i32
tls_recipere(
    TlsConnexio* connexio,
    i8*          buffer,
    i32          capacitas)
{
    integer n;

    si (!connexio || connexio->clausa || !connexio->ssl)
    {
        redde (i32)-1;
    }

    si (!buffer || capacitas <= 0)
    {
        redde 0;
    }

    n = SSL_read(connexio->ssl, buffer, (integer)capacitas);

    si (n <= 0)
    {
        integer ssl_error;
        ssl_error = SSL_get_error(connexio->ssl, n);

        si (ssl_error == SSL_ERROR_ZERO_RETURN)
        {
            /* Connexio clausa gracefully */
            connexio->clausa = VERUM;
            redde 0;
        }

        si (ssl_error == SSL_ERROR_WANT_READ || ssl_error == SSL_ERROR_WANT_WRITE)
        {
            redde 0;  /* Would block */
        }

        redde (i32)-1;
    }

    redde (i32)n;
}

vacuum
tls_claudere(TlsConnexio* connexio)
{
    si (!connexio)
    {
        redde;
    }

    si (connexio->ssl && !connexio->clausa)
    {
        SSL_shutdown(connexio->ssl);
        SSL_free(connexio->ssl);
        connexio->ssl = NIHIL;
    }

    si (connexio->ssl_ctx)
    {
        SSL_CTX_free(connexio->ssl_ctx);
        connexio->ssl_ctx = NIHIL;
    }

    si (connexio->tcp)
    {
        tcp_claudere(connexio->tcp);
        connexio->tcp = NIHIL;
    }

    connexio->clausa = VERUM;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - UTILITAS
 * ======================================================================== */

b32
tls_est_valida(TlsConnexio* connexio)
{
    redde connexio != NIHIL &&
           !connexio->clausa &&
           connexio->ssl != NIHIL &&
           connexio->tcp != NIHIL &&
           tcp_est_valida(connexio->tcp);
}

constans character*
tls_error_descriptio(TlsError error)
{
    commutatio (error)
    {
        casus TLS_OK:                  redde "OK";
        casus TLS_ERROR_TCP:           redde "Connexio TCP fallita";
        casus TLS_ERROR_HANDSHAKE:     redde "TLS handshake fallita";
        casus TLS_ERROR_CERTIFICATUM:  redde "Certificatum invalidum";
        casus TLS_ERROR_IO:            redde "I/O error";
        casus TLS_ERROR_CLAUSUM:       redde "Connexio clausa";
        ordinarius:                    redde "Error ignotus";
    }
}
