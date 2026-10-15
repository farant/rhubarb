/* tls_bearssl.c - VECTURA BearSSL (tls-bearssl-spec II.6)
 *
 * Machina BearSSL (vendor/bearssl_amalgama.c) est machina status pura:
 * quattuor capsae (sendrec, recvrec, sendapp, recvapp). _antlia eam per
 * br_ssl_engine_current_state agit super tcp_mittere/tcp_recipere -
 * EADEM pro modo gradatim (fossa non-blocans: 'exspecta' = LEGERE/
 * SCRIBERE) et obstruente (fossa cum moris tcp: 'exspecta' = tempus
 * excessum, regula R9).
 *
 * Fiducia: iudex br_x509_class proprius catenam DER colligit,
 * clavem folii per br_x509_decoder legit (get_pkey), in end_chain
 * fiducia_iudicare vocat (platforma iudicat; decisio A).
 * verificare_certificatum FALSUM: clavis legitur, fiducia omittitur.
 *
 * Limen: TLS 1.2 solum; sex cifrae (ECDHE-ECDSA/RSA x AES-GCM /
 * ChaCha20-Poly1305) - idem ac SecureTransport (_cifra_admissa). Nihil
 * extra piscinam dimittendum (classis R5 hic nequit). */
#include "postulata_posix.h"
#include "bearssl_amalgama.h"   /* ANTE latina.h: numeri Romani nomina
                                 * parametrorum vendoris frangerent */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "tcp.h"
#include "tls.h"
#include "fiducia.h"
#include "tls_bearssl.h"
#include <stdio.h>
#include <string.h>

#define CATENA_MAXIMA         XVI
#define CERTIFICATUM_MAXIMUM  (XVI * MXXIV)

/* iudex: vtable PRIMUM (BearSSL &iudex.vtable tradit) */
nomen structura {
    constans br_x509_class* vtable;
                TlsBearssl* connexio;
} Iudex;

nomen enumeratio {
    ANTLIA_PARATA = 0,   /* status quaesitus adest */
    ANTLIA_LEGERE,       /* fossa legibilis exspectanda (obstruens:
                          * tempus excessum) */
    ANTLIA_SCRIBERE,     /* fossa scribibilis exspectanda */
    ANTLIA_CLAUSA,       /* clausura munda (close_notify, aut EOF post
                          * pactionem - ut SecureTransport) */
    ANTLIA_ERROR         /* connexio->erratum nominat */
} Antlia;

structura TlsBearssl {
      br_ssl_client_context  cc;
                      Iudex  iudex;
    br_x509_decoder_context  decoder;     /* clavis folii (get_pkey) */
       insignatus character* capsa;       /* BR_SSL_BUFSIZE_BIDI */
                TcpConnexio* tcp;
                    Piscina* piscina;
         constans character* hospes;
         constans character* ancora_via;  /* NIHIL = repositorium */
                        b32  verificare;
                        b32  pacta;       /* pactio perfecta */
                        b32  clausa;
                        b32  reiecta;     /* fiducia catenam reiecit */
        FiduciaCertificatum  catena[CATENA_MAXIMA];
                        i32  numerus;     /* certificata perfecta */
                         i8* scribendum;  /* certificatum currens */
                        i32  exspectata;  /* longitudo eius */
                        i32  scripta;
                        b32  nimis;       /* catena aut certificatum
                                           * nimium */
                     size_t pendens;     /* mittere: octeti accepti,
                                           * recorda nondum missa */
                     chorda erratum;
};

/* ---- iudex (br_x509_class) ----
 * (parametrum 'server_name' BearSSL -> titulus_servi: numquam 'nomen*',
 * nomen = macrum typedef) */

interior vacuum
_v_initium_catenae (
    constans br_x509_class** ctx,
        constans character*  titulus_servi)
{
    TlsBearssl* c = ((Iudex*)(vacuum*)ctx)->connexio;

    (vacuum)titulus_servi;
    c->numerus  = ZEPHYRUM;
    c->nimis    = FALSUM;
    c->reiecta  = FALSUM;
}

interior vacuum
_v_initium_certificati (
    constans br_x509_class** ctx,
                   uint32_t  longitudo)
{
    TlsBearssl* c = ((Iudex*)(vacuum*)ctx)->connexio;

    c->scribendum = NIHIL;
    si (   c->nimis || c->numerus >= CATENA_MAXIMA
        || longitudo > CERTIFICATUM_MAXIMUM || longitudo == ZEPHYRUM)
    {
        c->nimis = VERUM;
        redde;
    }
    c->scribendum  = (i8*)piscina_allocare(c->piscina, (i64)longitudo);
    c->exspectata  = (i32)longitudo;
    c->scripta     = ZEPHYRUM;
    si (c->scribendum == NIHIL)
    {
        c->nimis = VERUM;
        redde;
    }
    si (c->numerus == ZEPHYRUM)
    {
        br_x509_decoder_init(&c->decoder, NIHIL, NIHIL);
    }
}

interior vacuum
_v_appendere (
           constans br_x509_class** ctx,
    constans insignatus character*  octeti,
                           size_t   mensura)
{
    TlsBearssl* c = ((Iudex*)(vacuum*)ctx)->connexio;
        size_t  capax;

    si (c->nimis || c->scribendum == NIHIL)
    {
        redde;
    }
    capax = (size_t)(c->exspectata - c->scripta);
    si (mensura > capax)
    {
        c->nimis = VERUM;
        redde;
    }
    memcpy(c->scribendum + c->scripta, octeti, mensura);
    c->scripta += (i32)mensura;
    si (c->numerus == ZEPHYRUM)
    {
        br_x509_decoder_push(&c->decoder, octeti, mensura);
    }
}

interior vacuum
_v_finis_certificati (
    constans br_x509_class** ctx)
{
    TlsBearssl* c = ((Iudex*)(vacuum*)ctx)->connexio;

    si (   c->nimis || c->scribendum == NIHIL
        || c->scripta != c->exspectata)
    {
        c->nimis = VERUM;
        redde;
    }
    c->catena[c->numerus].octeti   = c->scribendum;
    c->catena[c->numerus].mensura  = c->scripta;
    c->numerus++;
    c->scribendum = NIHIL;
}

interior insignatus
_v_finis_catenae (
    constans br_x509_class** ctx)
{
    TlsBearssl* c = ((Iudex*)(vacuum*)ctx)->connexio;

    si (c->nimis || c->numerus == ZEPHYRUM)
    {
        c->erratum = chorda_ex_literis(
            "catena certificatorum nimis magna aut vacua", c->piscina);
        c->reiecta = VERUM;
        redde BR_ERR_X509_LIMIT_EXCEEDED;
    }
    si (br_x509_decoder_get_pkey(&c->decoder) == NIHIL)
    {
        c->erratum = chorda_ex_literis("clavis folii legi non potest",
            c->piscina);
        c->reiecta = VERUM;
        redde BR_ERR_X509_UNSUPPORTED;
    }
    si (   c->verificare && !fiducia_iudicare(c->catena, c->numerus,
            c->hospes, c->ancora_via, &c->erratum, c->piscina))
    {
        c->reiecta = VERUM;
        redde BR_ERR_X509_NOT_TRUSTED;
    }
    redde ZEPHYRUM;
}

interior constans br_x509_pkey*
_v_clavis (
    constans br_x509_class* constans* ctx,
                       insignatus* usus)
{
    TlsBearssl* c =
        ((constans Iudex*)(constans vacuum*)ctx)->connexio;

    si (usus != NIHIL)
    {
        *usus = BR_KEYTYPE_KEYX | BR_KEYTYPE_SIGN;
    }
    redde br_x509_decoder_get_pkey(&c->decoder);
}

hic_manens constans br_x509_class IUDICIS_CLASSIS = {
    magnitudo(Iudex),
    _v_initium_catenae,
    _v_initium_certificati,
    _v_appendere,
    _v_finis_certificati,
    _v_finis_catenae,
    _v_clavis
};

/* ---- nomina errorum ---- */

/* capsa hic_manens: vocans STATIM copiat (chorda_ex_literis) */
interior constans character*
_erratum_machinae (
    integer codex)
{
    hic_manens character capsa[XCVI];

    commutatio (codex)
    {
        casus BR_ERR_BAD_VERSION:
        casus BR_ERR_UNSUPPORTED_VERSION:
            redde "versio TLS non admissa (solum TLS 1.2)";
        casus BR_ERR_BAD_CIPHER_SUITE:
            redde "cifra non admissa";
        casus BR_ERR_BAD_SIGNATURE:
            redde "subscriptio hospitis falsa";
        casus BR_ERR_BAD_MAC:
            redde "MAC recordi falsum";
        casus BR_ERR_NO_RANDOM:
            redde "entropia systematis deest";
        casus BR_ERR_IO:
            redde "I/O fossae fallita";
        ordinarius:
            frange;
    }
    si (codex >= BR_ERR_SEND_FATAL_ALERT)
    {
        sprintf(capsa, "nuntius fatalis missus: %d",
            codex - BR_ERR_SEND_FATAL_ALERT);
    }
    alioquin si (codex >= BR_ERR_RECV_FATAL_ALERT)
    {
        sprintf(capsa, "nuntius fatalis a pare: %d",
            codex - BR_ERR_RECV_FATAL_ALERT);
    }
    alioquin
    {
        sprintf(capsa, "BearSSL error %d", codex);
    }
    redde capsa;
}

/* ---- antlia ---- */

interior vacuum
_errorem_ponere (
             TlsBearssl* c,
     constans character* nuntius)
{
    si (c->erratum.mensura == ZEPHYRUM)
    {
        c->erratum = chorda_ex_literis(nuntius, c->piscina);
    }
}

/* machinam agere donec status 'sitis' adest, aut exspectandum, aut
 * clausa/error. Recorda mittenda SEMPER primum (BearSSL doctrina). */
interior Antlia
_antlia (
    TlsBearssl* c,
    insignatus  sitis)
{
    br_ssl_engine_context* e = &c->cc.eng;

    dum (VERUM)
    {
        insignatus status = br_ssl_engine_current_state(e);

        si (status & BR_SSL_CLOSED)
        {
            integer err = br_ssl_engine_last_error(e);

            si (err == BR_ERR_OK)
            {
                redde ANTLIA_CLAUSA;
            }
            si (!c->reiecta)
            {
                _errorem_ponere(c, _erratum_machinae(err));
            }
            redde ANTLIA_ERROR;
        }
        si (status & BR_SSL_SENDREC)
        {
                          size_t  n;
            insignatus character* b = br_ssl_engine_sendrec_buf(e, &n);
                             s32  m = tcp_mittere(c->tcp,
                                 (constans i8*)b,
                                 (i32)n);

            si (m < ZEPHYRUM)
            {
                _errorem_ponere(c, "scriptio fossae fallita");
                redde ANTLIA_ERROR;
            }
            si (m == ZEPHYRUM)
            {
                redde ANTLIA_SCRIBERE;
            }
            br_ssl_engine_sendrec_ack(e, (size_t)m);
            perge;
        }
        si (status & sitis)
        {
            redde ANTLIA_PARATA;
        }
        si (status & BR_SSL_RECVREC)
        {
                          size_t  n;
            insignatus character* b = br_ssl_engine_recvrec_buf(e, &n);
                             s32  m = tcp_recipere(c->tcp, (i8*)b,
                                 (i32)n);

            si (m == TCP_ITERUM)
            {
                redde ANTLIA_LEGERE;
            }
            si (m == ZEPHYRUM)
            {
                /* EOF sine close_notify: post pactionem finis (ut
                 * errSSLClosedNoNotify SecureTransport); in pactione
                 * error */
                si (c->pacta)
                {
                    redde ANTLIA_CLAUSA;
                }
                _errorem_ponere(c,
                    "connexio a pare clausa in pactione");
                redde ANTLIA_ERROR;
            }
            si (m < ZEPHYRUM)
            {
                _errorem_ponere(c, "lectio fossae fallita");
                redde ANTLIA_ERROR;
            }
            br_ssl_engine_recvrec_ack(e, (size_t)m);
            perge;
        }
        _errorem_ponere(c, "machina BearSSL immota");
        redde ANTLIA_ERROR;
    }
}

/* ---- praeparatio ---- */

interior constans character*
_copiare (
    constans character* s,
               Piscina* piscina)
{
        size_t  n = strlen(s) + I;
     character* d = (character*)piscina_allocare(piscina, (i64)n);

    si (d != NIHIL)
    {
        memcpy(d, s, n);
    }
    redde d;
}

/* NIHIL si bene; aliter causa */
interior constans character*
_parare (
              TlsBearssl* c,
      constans character* hospes,
    constans TlsOptiones* optiones)
{
    hic_manens constans uint16_t CIFRAE[] = {
        BR_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
        BR_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
        BR_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,
        BR_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,
        BR_TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256,
        BR_TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256
    };
    br_ssl_engine_context* e = &c->cc.eng;

    c->hospes     = _copiare(hospes, c->piscina);
    c->ancora_via = (optiones && optiones->ancora_via)
        ? _copiare(optiones->ancora_via, c->piscina) : NIHIL;
    c->verificare =
        optiones ? optiones->verificare_certificatum : VERUM;
    c->capsa      = (insignatus character*)piscina_allocare(c->piscina,
        (i64)BR_SSL_BUFSIZE_BIDI);
    si (   c->hospes == NIHIL || c->capsa == NIHIL
        || (optiones && optiones->ancora_via && c->ancora_via == NIHIL))
    {
        redde "Allocatio fallita";
    }
    si (c->ancora_via != NIHIL && !filum_existit(c->ancora_via))
    {
        redde "ancora legi non potest";
    }
    br_ssl_client_zero(&c->cc);
    br_ssl_engine_set_versions(e, BR_TLS12, BR_TLS12);
    br_ssl_engine_set_suites(e, CIFRAE,
        magnitudo(CIFRAE) / magnitudo(CIFRAE[ZEPHYRUM]));
    br_ssl_engine_set_hash(e, br_sha256_ID, &br_sha256_vtable);
    br_ssl_engine_set_hash(e, br_sha384_ID, &br_sha384_vtable);
    br_ssl_engine_set_prf_sha256(e, &br_tls12_sha256_prf);
    br_ssl_engine_set_prf_sha384(e, &br_tls12_sha384_prf);
    br_ssl_engine_set_default_ec(e);
    br_ssl_engine_set_default_ecdsa(e);
    br_ssl_engine_set_default_rsavrfy(e);
    br_ssl_engine_set_default_aes_gcm(e);
    br_ssl_engine_set_default_chapol(e);
    c->iudex.vtable    = &IUDICIS_CLASSIS;
    c->iudex.connexio  = c;
    br_ssl_engine_set_x509(e, &c->iudex.vtable);
    br_ssl_engine_set_buffer(e, c->capsa, BR_SSL_BUFSIZE_BIDI, I);
    si (!br_ssl_client_reset(&c->cc, c->hospes, ZEPHYRUM))
    {
        redde _erratum_machinae(br_ssl_engine_last_error(e));
    }
    redde NIHIL;
}

interior TlsBearsslResultus
_error (
              TlsError  error,
    constans character* nuntius,
               Piscina* piscina)
{
    TlsBearsslResultus r;

    r.successus         = FALSUM;
    r.connexio          = NIHIL;
    r.error             = error;
    r.error_descriptio  = chorda_ex_literis(nuntius, piscina);
    redde r;
}

/* connexio nova super tcp (possessa: clauditur si defectus) */
interior TlsBearsslResultus
_creare (
            TcpConnexio* tcp,
     constans character* hospes,
   constans TlsOptiones* optiones,
                Piscina* piscina)
{
     TlsBearsslResultus  r;
             TlsBearssl* c;
     constans character* causa;

    si (tcp == NIHIL || hospes == NIHIL || piscina == NIHIL)
    {
        si (tcp)
        {
            tcp_claudere(tcp);
        }
        redde _error(TLS_ERROR_TCP, "Argumenta invalida", piscina);
    }
    c = (TlsBearssl*)piscina_allocare(piscina,
        (i64)magnitudo(TlsBearssl));
    si (c == NIHIL)
    {
        tcp_claudere(tcp);
        redde _error(TLS_ERROR_TCP, "Allocatio fallita", piscina);
    }
    memset(c, ZEPHYRUM, magnitudo(TlsBearssl));
    c->tcp      = tcp;
    c->piscina  = piscina;
    causa       = _parare(c, hospes, optiones);
    si (causa != NIHIL)
    {
        chorda descriptio = chorda_ex_literis(causa, piscina);

        tcp_claudere(tcp);
        r.successus         = FALSUM;
        r.connexio          = NIHIL;
        r.error             = TLS_ERROR_HANDSHAKE;
        r.error_descriptio  = descriptio;
        redde r;
    }
    r.successus                 = VERUM;
    r.connexio                  = c;
    r.error                     = TLS_OK;
    r.error_descriptio.datum    = NIHIL;
    r.error_descriptio.mensura  = ZEPHYRUM;
    redde r;
}

/* ---- publica (tls_bearssl.h) ---- */

TlsBearsslResultus
tls_bearssl_incipere (
             TcpConnexio* tcp,
      constans character* hospes,
    constans TlsOptiones* optiones,
                 Piscina* piscina)
{
    redde _creare(tcp, hospes, optiones, piscina);
}

TlsGradus
tls_bearssl_gradus (
    TlsBearssl* c)
{
    si (c == NIHIL || c->clausa)
    {
        redde TLS_GRADUS_ERROR;
    }
    commutatio (_antlia(c, BR_SSL_SENDAPP))
    {
        casus ANTLIA_PARATA:
            c->pacta = VERUM;
            redde TLS_GRADUS_PERFECTUS;
        casus ANTLIA_LEGERE:
            redde TLS_GRADUS_LEGERE;
        casus ANTLIA_SCRIBERE:
            redde TLS_GRADUS_SCRIBERE;
        casus ANTLIA_CLAUSA:
            _errorem_ponere(c, "connexio clausa in pactione");
            redde TLS_GRADUS_ERROR;
        ordinarius:
            redde TLS_GRADUS_ERROR;
    }
}

chorda
tls_bearssl_gradus_error (
    TlsBearssl* c)
{
    chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde c ? c->erratum : vacua;
}

/* mittere: octeti in sendapp, flush, recorda effunduntur. Contractus
 * tls.h: TLS_ITERUM = idem iterum vocandum; octeti iam accepti
 * (pendens) tunc redduntur cum recorda eorum missa sunt (ut
 * scriptum_pendens SecureTransport) */
s32
tls_bearssl_mittere (
     TlsBearssl* c,
    constans i8* data,
            i32  mensura)
{
    br_ssl_engine_context* e;
                   Antlia  a;

    si (c == NIHIL || c->clausa)
    {
        redde -I;
    }
    si (data == NIHIL || mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    e = &c->cc.eng;
    si (c->pendens == ZEPHYRUM)
    {
        a = _antlia(c, BR_SSL_SENDAPP);
        si (a == ANTLIA_LEGERE || a == ANTLIA_SCRIBERE)
        {
            redde TLS_ITERUM;
        }
        si (a != ANTLIA_PARATA)
        {
            redde -I;
        }
        {
                          size_t  capax;
            insignatus character* b = br_ssl_engine_sendapp_buf(e,
                &capax);
                         size_t n = (size_t)mensura < capax
                             ? (size_t)mensura : capax;

            memcpy(b, data, n);
            br_ssl_engine_sendapp_ack(e, n);
            br_ssl_engine_flush(e, ZEPHYRUM);
            c->pendens = n;
        }
    }
    a = _antlia(c, BR_SSL_SENDAPP);   /* recorda effundere */
    si (a == ANTLIA_SCRIBERE || a == ANTLIA_LEGERE)
    {
        redde TLS_ITERUM;
    }
    si (a != ANTLIA_PARATA)
    {
        redde -I;
    }
    {
        size_t n = c->pendens;

        c->pendens = ZEPHYRUM;
        redde (s32)n;
    }
}

s32
tls_bearssl_recipere (
    TlsBearssl* c,
            i8* buffer,
           i32  capacitas)
{
    br_ssl_engine_context* e;

    si (c == NIHIL || c->clausa)
    {
        redde -I;
    }
    si (buffer == NIHIL || capacitas == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    e = &c->cc.eng;
    commutatio (_antlia(c, BR_SSL_RECVAPP))
    {
        casus ANTLIA_PARATA:
        {
                          size_t  n;
            insignatus character* b = br_ssl_engine_recvapp_buf(e, &n);

            si (n > (size_t)capacitas)
            {
                n = (size_t)capacitas;
            }
            memcpy(buffer, b, n);
            br_ssl_engine_recvapp_ack(e, n);
            redde (s32)n;
        }
        casus ANTLIA_LEGERE:
        casus ANTLIA_SCRIBERE:
            redde TLS_ITERUM;   /* obstruens: tempus excessum
                                 * (TCP_ITERUM) */
        casus ANTLIA_CLAUSA:
            c->clausa = VERUM;
            redde ZEPHYRUM;
        ordinarius:
            redde -I;
    }
}

vacuum
tls_bearssl_claudere (
    TlsBearssl* c)
{
    si (c == NIHIL)
    {
        redde;
    }
    si (!c->clausa && c->capsa != NIHIL && c->tcp != NIHIL)
    {
        br_ssl_engine_context* e = &c->cc.eng;
                          i32  k;

        /* close_notify, conatu uno: recorda pendentia semel
         * mittuntur */
        br_ssl_engine_close(e);
        per (k = ZEPHYRUM; k < VIII
             && (br_ssl_engine_current_state(e) & BR_SSL_SENDREC); k++)
        {
                          size_t  n;
            insignatus character* b = br_ssl_engine_sendrec_buf(e, &n);
                             s32  m = tcp_mittere(c->tcp,
                                 (constans i8*)b,
                                 (i32)n);

            si (m <= ZEPHYRUM)
            {
                frange;
            }
            br_ssl_engine_sendrec_ack(e, (size_t)m);
        }
    }
    si (c->tcp != NIHIL)
    {
        tcp_claudere(c->tcp);
        c->tcp = NIHIL;
    }
    c->clausa = VERUM;
}

b32
tls_bearssl_est_valida (
    TlsBearssl* c)
{
    redde c != NIHIL && !c->clausa && c->tcp != NIHIL
        && tcp_est_valida(c->tcp);
}
