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
    b32            tempus_excessum; /* callback lectionis TCP_ITERUM vidit */
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
    s32 n;
    size_t petitum;

    memcpy(&conn, &connection, magnitudo(conn));

    si (!conn || !conn->tcp || !tcp_est_valida(conn->tcp))
    {
        *dataLength = 0;
        redde errSSLClosedAbort;
    }

    petitum = *dataLength;
    n = tcp_recipere(conn->tcp, (i8*)data, (i32)petitum);

    /* TCP_ITERUM ante n < 0 - sentinella ipsa negativa est */
    si (n == TCP_ITERUM)
    {
        conn->tempus_excessum = VERUM;
        *dataLength = 0;
        redde errSSLWouldBlock;
    }

    si (n < 0)
    {
        *dataLength = 0;
        redde errSSLClosedAbort;
    }

    si (n == 0)
    {
        *dataLength = 0;
        redde errSSLClosedGraceful;
    }

    *dataLength = (size_t)n;

    si ((size_t)n < petitum)
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
    s32 n;

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

/* Cifrae admissae (vates-plan-1 T1b): INDEX ADMISSUS, non vetitus -
 * cifra quam Apple postea addit exclusa manet donec hic nominetur.
 * ECDHE solum (secretum anterius), AEAD solum (AES-GCM, ChaCha20-
 * Poly1305). Fumus 2026-10-07: SecureTransport sine indice 3DES
 * offerebat (howsmyssl 'Bad', SWEET32). */
interior b32
_cifra_admissa(SSLCipherSuite cifra)
{
    commutatio (cifra)
    {
        casus TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:
        casus TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:
        casus TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:
        casus TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384:
        casus TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256:
        casus TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* Contextum durare: TLS 1.2 minimum + cifrae admissae solae. Redde
 * NIHIL si bene, aliter causam nominatam (connexio recusanda). Fumus
 * 2026-10-07: tls-v1-0/1-1.badssl.com ACCEPTA erant. */
interior constans character*
_durare(SSLContextRef ssl_context, Piscina* piscina)
{
          OSStatus  status;
            size_t  numerus  = 0;
            size_t  admissae = 0;
            size_t  i;
    SSLCipherSuite* omnes;
    SSLCipherSuite* electae;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    status = SSLSetProtocolVersionMin(ssl_context, kTLSProtocol12);
    si (status != noErr)
    {
        redde "SSLSetProtocolVersionMin fallita";
    }

    status = SSLGetNumberSupportedCiphers(ssl_context, &numerus);
    si (status != noErr || numerus == 0)
    {
        redde "SSLGetNumberSupportedCiphers fallita";
    }

    omnes   = (SSLCipherSuite*)piscina_allocare(piscina,
                  (i64)(numerus * magnitudo(SSLCipherSuite)));
    electae = (SSLCipherSuite*)piscina_allocare(piscina,
                  (i64)(numerus * magnitudo(SSLCipherSuite)));
    si (!omnes || !electae)
    {
        redde "Allocatio cifrarum fallita";
    }

    status = SSLGetSupportedCiphers(ssl_context, omnes, &numerus);
    si (status != noErr)
    {
        redde "SSLGetSupportedCiphers fallita";
    }

    per (i = 0; i < numerus; i++)
    {
        si (_cifra_admissa(omnes[i]))
        {
            electae[admissae] = omnes[i];
            admissae++;
        }
    }
    si (admissae == 0)
    {
        redde "Nulla cifra admissa a systemate sustentata";
    }

    status = SSLSetEnabledCiphers(ssl_context, electae, admissae);
    si (status != noErr)
    {
        redde "SSLSetEnabledCiphers fallita";
    }
#pragma clang diagnostic pop

    redde NIHIL;
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
    conn->tempus_excessum = FALSUM;
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

    /* Durare: TLS 1.2 minimum, cifrae admissae solae (T1b) */
    {
        constans character* causa = _durare(ssl_context, piscina);

        si (causa)
        {
            CFRelease(ssl_context);
            tcp_claudere(conn->tcp);
            redde _creare_error(TLS_ERROR_HANDSHAKE, causa, piscina);
        }
    }

    /* Disactivare verificatio certificati si petitum */
    si (optiones && !optiones->verificare_certificatum)
    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        SSLSetSessionOption(ssl_context, kSSLSessionOptionBreakOnServerAuth, true);
#pragma clang diagnostic pop
    }

    /* Facere handshake. errSSLWouldBlock sine tempore excesso =
     * recordum partiale, iterare; cum tempore excesso = hospes
     * tacet - olim ansa in aeternum iterabat. */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    fac
    {
        status = SSLHandshake(ssl_context);
    } dum (status == errSSLWouldBlock && !conn->tempus_excessum);
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
        } dum (status == errSSLWouldBlock && !conn->tempus_excessum);
#pragma clang diagnostic pop
    }

    si (status == errSSLWouldBlock)
    {
        CFRelease(ssl_context);
        tcp_claudere(conn->tcp);
        redde _creare_error(TLS_ERROR_TCP, "Tempus handshake excessum",
                            piscina);
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

s32
tls_mittere(
    TlsConnexio*   connexio,
    constans i8*   data,
    i32            mensura)
{
    size_t processed = 0;
    OSStatus status;

    si (!connexio || connexio->clausa || !connexio->ssl_context)
    {
        redde -1;
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
        redde (s32)processed;
    }

    si (status != noErr)
    {
        redde -1;
    }

    redde (s32)processed;
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
        s32 n = tls_mittere(connexio, data + totalis, restans);
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
        totalis += (i32)n;
        restans -= (i32)n;
    }

    redde VERUM;
}

s32
tls_recipere(
    TlsConnexio* connexio,
    i8*          buffer,
    i32          capacitas)
{
    size_t processed = 0;
    OSStatus status;

    si (!connexio || connexio->clausa || !connexio->ssl_context)
    {
        redde -1;
    }

    si (!buffer || capacitas <= 0)
    {
        redde 0;
    }

    /* SSLRead errSSLWouldBlock cum 0 octetis reddit quotiens callback
     * lectionem TCP partialem nuntiat (recordum TLS nondum totum) -
     * id NON est finis. Olim 0 hinc reddebatur, quod contractus
     * 'connexio clausa' vocat: http EOF credebat et corpus in medio
     * truncabat cum successu (lapide briar-feedback bugs/015, 016).
     * Nunc iteramus; socket blocans est, ergo callback exspectat -
     * nulla rotatio. Solum tempus excessum ansam frangit. */
    connexio->tempus_excessum = FALSUM;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    fac
    {
        processed = 0;
        status = SSLRead(connexio->ssl_context, buffer, (size_t)capacitas, &processed);
    } dum (   status == errSSLWouldBlock
           && processed == 0
           && !connexio->tempus_excessum);
#pragma clang diagnostic pop

    si (status == errSSLWouldBlock)
    {
        si (processed == 0)
        {
            redde TCP_ITERUM;   /* tempus receptionis excessum */
        }
        redde (s32)processed;
    }

    si (status == errSSLClosedGraceful || status == errSSLClosedNoNotify)
    {
        connexio->clausa = VERUM;
        redde (s32)processed;
    }

    si (status != noErr && processed == 0)
    {
        redde -1;
    }

    redde (s32)processed;
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
 * FUNCTIONES PUBLICAE - CERTIFICATUM HOSPITIS
 * ======================================================================== */

/* CFAbsoluteTime (secunda ab 2001-01-01) -> epocha unix.
 * Valores validitatis ex SecCertificateCopyValues ut CFNumber
 * (absolutum) plerumque veniunt, ut CFDate interdum - ambo
 * tractantur ne versio systematis nos frangat. */
interior s64
_tempus_ex_valore(CFTypeRef valor)
{
    si (valor == NULL)
    {
        redde 0;
    }
    si (CFGetTypeID(valor) == CFNumberGetTypeID())
    {
        double absolutum = 0.0;

        si (CFNumberGetValue((CFNumberRef)valor, kCFNumberDoubleType,
                &absolutum))
        {
            redde (s64)(absolutum + kCFAbsoluteTimeIntervalSince1970);
        }
        redde 0;
    }
    si (CFGetTypeID(valor) == CFDateGetTypeID())
    {
        redde (s64)(CFDateGetAbsoluteTime((CFDateRef)valor)
            + kCFAbsoluteTimeIntervalSince1970);
    }
    redde 0;
}

/* valorem ex dictionario valorum certificati:
 * { OID: { kSecPropertyKeyValue: <valor> } } */
interior CFTypeRef
_proprietatem_capere(CFDictionaryRef valores, CFTypeRef oid)
{
    CFDictionaryRef proprietas;

    si (valores == NULL)
    {
        redde NULL;
    }
    proprietas = (CFDictionaryRef)CFDictionaryGetValue(valores, oid);
    si (proprietas == NULL
        || CFGetTypeID(proprietas) != CFDictionaryGetTypeID())
    {
        redde NULL;
    }
    redde CFDictionaryGetValue(proprietas, kSecPropertyKeyValue);
}

b32
tls_certificatum_obtinere(TlsConnexio* connexio,
    TlsCertificatum* exitus)
{
    SecTrustRef fiducia = NULL;
    SecCertificateRef folium = NULL;
    CFDictionaryRef valores = NULL;
    CFArrayRef claves = NULL;
    CFTypeRef oids[II];
    CFStringRef summarium = NULL;
    OSStatus status;

    si (exitus == NIHIL)
    {
        redde FALSUM;
    }
    exitus->valida = FALSUM;
    exitus->non_ante = 0;
    exitus->non_post = 0;
    exitus->subiectum.datum = NIHIL;
    exitus->subiectum.mensura = 0;

    si (connexio == NIHIL || connexio->clausa
        || connexio->ssl_context == NULL)
    {
        redde FALSUM;
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    status = SSLCopyPeerTrust(connexio->ssl_context, &fiducia);
#pragma clang diagnostic pop
    si (status != noErr || fiducia == NULL)
    {
        redde FALSUM;
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    folium = SecTrustGetCertificateAtIndex(fiducia, 0);
#pragma clang diagnostic pop
    si (folium == NULL)
    {
        CFRelease(fiducia);
        redde FALSUM;
    }

    oids[0] = kSecOIDX509V1ValidityNotBefore;
    oids[I] = kSecOIDX509V1ValidityNotAfter;
    claves = CFArrayCreate(NULL, oids, II, &kCFTypeArrayCallBacks);
    si (claves != NULL)
    {
        valores = SecCertificateCopyValues(folium, claves, NULL);
        CFRelease(claves);
    }
    si (valores != NULL)
    {
        exitus->non_ante = _tempus_ex_valore(_proprietatem_capere(
            valores, kSecOIDX509V1ValidityNotBefore));
        exitus->non_post = _tempus_ex_valore(_proprietatem_capere(
            valores, kSecOIDX509V1ValidityNotAfter));
        CFRelease(valores);
    }

    summarium = SecCertificateCopySubjectSummary(folium);
    si (summarium != NULL)
    {
        character buffer[CCLVI];

        si (CFStringGetCString(summarium, buffer, (CFIndex)CCLVI,
                kCFStringEncodingUTF8))
        {
            exitus->subiectum = chorda_ex_literis(buffer,
                connexio->piscina);
        }
        CFRelease(summarium);
    }

    CFRelease(fiducia);
    exitus->valida = (exitus->non_post > 0);
    redde exitus->valida;
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
