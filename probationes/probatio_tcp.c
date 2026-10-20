/* probatio_tcp.c - Probationes pro TCP bibliotheca
 *
 * Haec probationes requirunt connexionem ad rete.
 * Utuntur httpbin.org pro probationibus.
 */

/* <aedilis facultas="rete"/> */
#include "tcp.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>


/* ========================================================================
 * PROBATIONES - OPTIONES
 * ======================================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    TcpOptiones opt;

    (vacuum)c;

    opt = tcp_optiones_default();

    CREDO_VERUM(opt.timeout_ms > 0);
    CREDO_VERUM(opt.nodelay);
    CREDO_FALSUM(opt.keepalive);
}


/* ========================================================================
 * PROBATIONES - ERROR DESCRIPTIO
 * ======================================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    constans character* desc;

    (vacuum)c;

    desc = tcp_error_descriptio(TCP_OK);
    CREDO_NON_NIHIL(desc);

    desc = tcp_error_descriptio(TCP_ERROR_DNS);
    CREDO_NON_NIHIL(desc);

    desc = tcp_error_descriptio(TCP_ERROR_CONNEXIO);
    CREDO_NON_NIHIL(desc);

    desc = tcp_error_descriptio(TCP_ERROR_TIMEOUT);
    CREDO_NON_NIHIL(desc);
}


/* ========================================================================
 * PROBATIONES - NULLUM ARGUMENTA
 * ======================================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    s32 n;

    (vacuum)c;

    /* tcp_mittere cum NIHIL */
    n = tcp_mittere(NIHIL, (constans i8*)"test", IV);
    CREDO_AEQUALIS_S32(n, -1);

    /* tcp_recipere cum NIHIL */
    {
        i8 buffer[CXXVIII];
        n = tcp_recipere(NIHIL, buffer, CXXVIII);
        CREDO_AEQUALIS_S32(n, -1);
    }

    /* tcp_claudere cum NIHIL - non debet crash */
    tcp_claudere(NIHIL);

    /* tcp_est_valida cum NIHIL */
    CREDO_FALSUM(tcp_est_valida(NIHIL));

    /* tcp_obtinere_fd cum NIHIL */
    CREDO_AEQUALIS_S32(tcp_obtinere_fd(NIHIL), -1);
}


/* ========================================================================
 * PROBATIONES - CONNEXIO INVALIDA
 * ======================================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
           TcpResultus  res;
               Piscina* piscina;

    piscina = c->piscina;

    /* DNS fallita - hospes non existens */
    res = tcp_connectere("non.existens.hospes.example", LXXX, piscina);
    CREDO_FALSUM(res.successus);
    CREDO_AEQUALIS_I32(res.error, TCP_ERROR_DNS);
    CREDO_NIHIL(res.connexio);

    printf("  DNS error: ");
    si (res.error_descriptio.mensura > 0)
    {
        printf("%.*s", res.error_descriptio.mensura,
            res.error_descriptio.datum);
    }
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CONNEXIO VALIDA (HTTP)
 * ======================================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
           TcpResultus  res;
    constans character* petitio;
                    i8  buffer[IV * MXXIV];
                   s32  n;
                   b32  misit;
               Piscina* piscina;

    piscina = c->piscina;

    /* Connectere ad httpbin.org:80 */
    res = tcp_connectere("httpbin.org", LXXX, piscina);

    si (!res.successus)
    {
        printf("  NOTA: Connexio fallita (rete non disponibilis?)\n");
        printf("  Error: %s\n", tcp_error_descriptio(res.error));
        si (res.error_descriptio.mensura > 0)
        {
            printf("  Descriptio: %.*s\n",
                   res.error_descriptio.mensura,
                   res.error_descriptio.datum);
        }
        printf("\n");
        redde;
    }

    CREDO_VERUM(res.successus);
    CREDO_NON_NIHIL(res.connexio);
    CREDO_VERUM(tcp_est_valida(res.connexio));
    CREDO_MAIOR_S32(tcp_obtinere_fd(res.connexio), -1);

    /* Mittere HTTP petitio */
    petitio = "GET /get HTTP/1.1\r\n"
              "Host: httpbin.org\r\n"
              "Connection: close\r\n"
              "\r\n";

    misit = tcp_mittere_omnia(res.connexio,
                              (constans i8*)petitio,
                              (i32)strlen(petitio));
    CREDO_VERUM(misit);

    printf("  Petitio missa, expectans responsum...\n");

    /* Recipere responsum */
    n = tcp_recipere(res.connexio, buffer, IV * MXXIV - I);
    CREDO_MAIOR_S32(n, 0);

    si (n > 0)
    {
        buffer[n] = '\0';
        printf("  Receptum %d bytes\n", n);

        /* Verificare HTTP responsum */
        CREDO_VERUM(strncmp((character*)buffer, "HTTP/1.1 200", XII)
            == 0);
        printf("  Status: HTTP/1.1 200 OK\n");
    }

    /* Claudere */
    tcp_claudere(res.connexio);
    CREDO_FALSUM(tcp_est_valida(res.connexio));
}


/* ========================================================================
 * PROBATIONES - MITTERE/RECIPERE
 * ======================================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
           TcpResultus  res;
    constans character* petitio;
                    i8  buffer[IV * MXXIV];
                   s32  totalis;
                   s32  n;
               Piscina* piscina;

    piscina = c->piscina;

    res = tcp_connectere("httpbin.org", LXXX, piscina);
    si (!res.successus)
    {
        printf("  NOTA: Connexio fallita (rete non disponibilis?)\n");
        printf("\n");
        redde;
    }

    /* Mittere petitio in partibus */
    petitio = "GET /bytes/1024 HTTP/1.1\r\n"
              "Host: httpbin.org\r\n"
              "Connection: close\r\n"
              "\r\n";

    n = tcp_mittere(res.connexio, (constans i8*)petitio,
        (i32)strlen(petitio));
    CREDO_MAIOR_S32(n, 0);

    /* Recipere in loop usque connexio clausa */
    totalis = 0;
    fac
    {
        n = tcp_recipere(res.connexio, buffer, IV * MXXIV);
        si (n > 0)
        {
            totalis += n;
        }
    } dum (n > 0);

    printf("  Totalis receptum: %d bytes\n", totalis);
    CREDO_MAIOR_S32(totalis, 0);

    tcp_claudere(res.connexio);
}


hic_manens constans CredoSectio SECTIONES[] = {
    /* Probationes locales (non requirunt rete) */
    { "optiones default", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "error descriptio", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nullum argumenta", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    /* Probationes rete (requirunt connexionem) */
    { "connexio invalida", _sectio_quarta, NIHIL, NIHIL, "rete",
        NIHIL },
    { "connexio HTTP (httpbin.org:80)", _sectio_quinta, NIHIL, NIHIL,
        "rete", NIHIL },
    { "mittere/recipere", _sectio_sexta, NIHIL, NIHIL, "rete", NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale (vacuum)
{
    redde credo_suitam_currere("tcp", SECTIONES);
}
