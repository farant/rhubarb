/* tls_macos.m - Implementatio TLS pro macOS (Security.framework)
 *
 * Utitur SecureTransport pro TLS.
 *
 * NOTA: Includimus headers systema ANTE latina.h quia
 * "character" macro conflictum habet cum CoreFoundation.
 */

#import <Security/Security.h>
#import <Security/SecureTransport.h>
#include <string.h>
#include <unistd.h>

#include "tls.h"
#include "tcp.h"


/* ========================================================================
 * STRUCTURA INTERNA
 * ======================================================================== */

structura TlsConnexio {
    Piscina*       piscina;
    TcpConnexio*   tcp;           /* Connexio TCP subiacens */
    SSLContextRef  ssl_context;   /* Security.framework context */
    b32            clausa;
};


/* ========================================================================
 * FUNCTIONES I/O PRO SECURETRANSPORT
 * ======================================================================== */

/* Callback pro legere data de TCP */
interior OSStatus
_tls_read_callback(
    SSLConnectionRef connection,
    void*            data,
    size_t*          dataLength)
{
    /* SSLConnectionRef est const void* - debemus removere const */
    TlsConnexio* conn;
    i32 n;

    memcpy(&conn, &connection, magnitudo(conn));

    si (!conn || !conn->tcp || !tcp_est_valida(conn->tcp))
    {
        *dataLength = 0;
        redde errSSLClosedAbort;
    }

    n = tcp_recipere(conn->tcp, (i8*)data, (i32)*dataLength);

    si (n < 0)
    {
        *dataLength = 0;
        redde errSSLClosedAbort;
    }

    si (n == 0)
    {
        *dataLength = 0;
        redde errSSLWouldBlock;
    }

    *dataLength = (size_t)n;

    si ((size_t)n < *dataLength)
    {
        redde errSSLWouldBlock;
    }

    redde noErr;
}

/* Callback pro scribere data ad TCP */
interior OSStatus
_tls_write_callback(
    SSLConnectionRef connection,
    constans void*   data,
    size_t*          dataLength)
{
    /* SSLConnectionRef est const void* - debemus removere const */
    TlsConnexio* conn;
    i32 n;

    memcpy(&conn, &connection, magnitudo(conn));

    si (!conn || !conn->tcp || !tcp_est_valida(conn->tcp))
    {
        *dataLength = 0;
        redde errSSLClosedAbort;
    }

    n = tcp_mittere(conn->tcp, (constans i8*)data, (i32)*dataLength);

    si (n < 0)
    {
        *dataLength = 0;
        redde errSSLClosedAbort;
    }

    si (n == 0)
    {
        *dataLength = 0;
        redde errSSLWouldBlock;
    }

    *dataLength = (size_t)n;
    redde noErr;
}


/* ========================================================================
 * FUNCTIONES INTERNAE
 * ======================================================================== */

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
_osstatus_descriptio(OSStatus status)
{
    commutatio (status)
    {
        casus noErr:                    redde "OK";
        casus errSSLProtocol:           redde "SSL protocol error";
        casus errSSLNegotiation:        redde "SSL negotiation failed";
        casus errSSLFatalAlert:         redde "SSL fatal alert";
        casus errSSLWouldBlock:         redde "Would block";
        casus errSSLSessionNotFound:    redde "Session not found";
        casus errSSLClosedGraceful:     redde "Connection closed gracefully";
        casus errSSLClosedAbort:        redde "Connection aborted";
        casus errSSLXCertChainInvalid:  redde "Invalid certificate chain";
        casus errSSLBadCert:            redde "Bad certificate";
        casus errSSLCrypto:             redde "Cryptographic error";
        casus errSSLInternal:           redde "Internal error";
        casus errSSLCertExpired:        redde "Certificate expired";
        casus errSSLCertNotYetValid:    redde "Certificate not yet valid";
        casus errSSLClosedNoNotify:     redde "Connection closed without notify";
        casus errSSLBufferOverflow:     redde "Buffer overflow";
        casus errSSLBadRecordMac:       redde "Bad record MAC";
        casus errSSLDecryptionFail:     redde "Decryption failed";
        casus errSSLHostNameMismatch:   redde "Hostname mismatch";
        casus errSSLPeerHandshakeFail:  redde "Peer handshake failed";
        casus errSSLPeerProtocolVersion: redde "Protocol version mismatch";
        casus errSSLPeerInsufficientSecurity: redde "Insufficient security";
        casus errSSLConnectionRefused:  redde "Connection refused";
        ordinarius:                     redde "Unknown SSL error";
    }
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
    TlsResultus res;
    TlsConnexio* conn;
    TcpResultus tcp_res;
    TcpOptiones tcp_opt;
    SSLContextRef ssl_context;
    OSStatus status;

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

    /* Allocare connexio */
    conn = (TlsConnexio*)piscina_allocare(piscina, (i64)magnitudo(TlsConnexio));
    si (!conn)
    {
        tcp_claudere(tcp_res.connexio);
        redde _creare_error(TLS_ERROR_TCP, "Allocatio fallita", piscina);
    }

    conn->piscina = piscina;
    conn->tcp = tcp_res.connexio;
    conn->clausa = FALSUM;
    conn->ssl_context = NIHIL;

    /* Creare SSL context */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    ssl_context = SSLCreateContext(NIHIL, kSSLClientSide, kSSLStreamType);
#pragma clang diagnostic pop

    si (!ssl_context)
    {
        tcp_claudere(conn->tcp);
        redde _creare_error(TLS_ERROR_HANDSHAKE, "SSLCreateContext fallita", piscina);
    }

    conn->ssl_context = ssl_context;

    /* Configurare I/O callbacks */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    status = SSLSetIOFuncs(ssl_context, _tls_read_callback, _tls_write_callback);
#pragma clang diagnostic pop

    si (status != noErr)
    {
        CFRelease(ssl_context);
        tcp_claudere(conn->tcp);
        redde _creare_error(TLS_ERROR_HANDSHAKE, "SSLSetIOFuncs fallita", piscina);
    }

    /* Ponere connection reference (pro callbacks) */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    status = SSLSetConnection(ssl_context, conn);
#pragma clang diagnostic pop

    si (status != noErr)
    {
        CFRelease(ssl_context);
        tcp_claudere(conn->tcp);
        redde _creare_error(TLS_ERROR_HANDSHAKE, "SSLSetConnection fallita", piscina);
    }

    /* Ponere hostname pro SNI et verificatio certificati */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    status = SSLSetPeerDomainName(ssl_context, hospes, strlen(hospes));
#pragma clang diagnostic pop

    si (status != noErr)
    {
        CFRelease(ssl_context);
        tcp_claudere(conn->tcp);
        redde _creare_error(TLS_ERROR_HANDSHAKE, "SSLSetPeerDomainName fallita", piscina);
    }

    /* Disactivare verificatio certificati si petitum */
    si (optiones && !optiones->verificare_certificatum)
    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        SSLSetSessionOption(ssl_context, kSSLSessionOptionBreakOnServerAuth, true);
#pragma clang diagnostic pop
    }

    /* Facere handshake */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    fac
    {
        status = SSLHandshake(ssl_context);
    } dum (status == errSSLWouldBlock);
#pragma clang diagnostic pop

    /* Tractare kSSLSessionOptionBreakOnServerAuth */
    si (status == errSSLServerAuthCompleted)
    {
        /* Continuare handshake post server auth */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        fac
        {
            status = SSLHandshake(ssl_context);
        } dum (status == errSSLWouldBlock);
#pragma clang diagnostic pop
    }

    si (status != noErr)
    {
        CFRelease(ssl_context);
        tcp_claudere(conn->tcp);

        si (status == errSSLXCertChainInvalid ||
            status == errSSLBadCert ||
            status == errSSLCertExpired ||
            status == errSSLCertNotYetValid ||
            status == errSSLHostNameMismatch)
        {
            redde _creare_error(TLS_ERROR_CERTIFICATUM,
                               _osstatus_descriptio(status), piscina);
        }

        redde _creare_error(TLS_ERROR_HANDSHAKE,
                           _osstatus_descriptio(status), piscina);
    }

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
    size_t processed = 0;
    OSStatus status;

    si (!connexio || connexio->clausa || !connexio->ssl_context)
    {
        redde (i32)-1;
    }

    si (!data || mensura <= 0)
    {
        redde 0;
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    status = SSLWrite(connexio->ssl_context, data, (size_t)mensura, &processed);
#pragma clang diagnostic pop

    si (status == errSSLWouldBlock)
    {
        redde (i32)processed;
    }

    si (status != noErr)
    {
        redde (i32)-1;
    }

    redde (i32)processed;
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
    size_t processed = 0;
    OSStatus status;

    si (!connexio || connexio->clausa || !connexio->ssl_context)
    {
        redde (i32)-1;
    }

    si (!buffer || capacitas <= 0)
    {
        redde 0;
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    status = SSLRead(connexio->ssl_context, buffer, (size_t)capacitas, &processed);
#pragma clang diagnostic pop

    si (status == errSSLWouldBlock)
    {
        redde (i32)processed;
    }

    si (status == errSSLClosedGraceful || status == errSSLClosedNoNotify)
    {
        connexio->clausa = VERUM;
        redde (i32)processed;
    }

    si (status != noErr && processed == 0)
    {
        redde (i32)-1;
    }

    redde (i32)processed;
}

vacuum
tls_claudere(TlsConnexio* connexio)
{
    si (!connexio)
    {
        redde;
    }

    si (connexio->ssl_context && !connexio->clausa)
    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        SSLClose(connexio->ssl_context);
#pragma clang diagnostic pop
        CFRelease(connexio->ssl_context);
        connexio->ssl_context = NIHIL;
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
           connexio->ssl_context != NIHIL &&
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
