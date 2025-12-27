/* probatio_tcp.c - Probationes pro TCP bibliotheca
 *
 * Haec probationes requirunt connexionem ad rete.
 * Utuntur httpbin.org pro probationibus.
 */

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
probatio_optiones_default(Piscina* piscina)
{
    TcpOptiones opt;

    (vacuum)piscina;

    printf("--- Probans optiones default ---\n");

    opt = tcp_optiones_default();

    CREDO_VERUM(opt.timeout_ms > 0);
    CREDO_VERUM(opt.nodelay);
    CREDO_FALSUM(opt.keepalive);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - ERROR DESCRIPTIO
 * ======================================================================== */

interior vacuum
probatio_error_descriptio(Piscina* piscina)
{
    constans character* desc;

    (vacuum)piscina;

    printf("--- Probans error descriptio ---\n");

    desc = tcp_error_descriptio(TCP_OK);
    CREDO_NON_NIHIL(desc);

    desc = tcp_error_descriptio(TCP_ERROR_DNS);
    CREDO_NON_NIHIL(desc);

    desc = tcp_error_descriptio(TCP_ERROR_CONNEXIO);
    CREDO_NON_NIHIL(desc);

    desc = tcp_error_descriptio(TCP_ERROR_TIMEOUT);
    CREDO_NON_NIHIL(desc);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CONNEXIO INVALIDA
 * ======================================================================== */

interior vacuum
probatio_connexio_invalida(Piscina* piscina)
{
    TcpResultus res;

    printf("--- Probans connexio invalida ---\n");

    /* DNS fallita - hospes non existens */
    res = tcp_connectere("non.existens.hospes.example", LXXX, piscina);
    CREDO_FALSUM(res.successus);
    CREDO_AEQUALIS_I32(res.error, TCP_ERROR_DNS);
    CREDO_NIHIL(res.connexio);

    printf("  DNS error: ");
    si (res.error_descriptio.mensura > 0)
    {
        printf("%.*s", res.error_descriptio.mensura, res.error_descriptio.datum);
    }
    printf("\n");

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CONNEXIO VALIDA (HTTP)
 * ======================================================================== */

interior vacuum
probatio_connexio_http(Piscina* piscina)
{
    TcpResultus res;
    constans character* petitio;
    i8 buffer[MMMMXCVI];
    i32 n;
    b32 misit;

    printf("--- Probans connexio HTTP (httpbin.org:80) ---\n");

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
    CREDO_MAIOR_S32((s32)tcp_obtinere_fd(res.connexio), -1);

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
    n = tcp_recipere(res.connexio, buffer, MMMMXCVI - I);
    CREDO_MAIOR_I32(n, 0);

    si (n > 0)
    {
        buffer[n] = '\0';
        printf("  Receptum %d bytes\n", n);

        /* Verificare HTTP responsum */
        CREDO_VERUM(strncmp((character*)buffer, "HTTP/1.1 200", XII) == 0);
        printf("  Status: HTTP/1.1 200 OK\n");
    }

    /* Claudere */
    tcp_claudere(res.connexio);
    CREDO_FALSUM(tcp_est_valida(res.connexio));

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - MITTERE/RECIPERE
 * ======================================================================== */

interior vacuum
probatio_mittere_recipere(Piscina* piscina)
{
    TcpResultus res;
    constans character* petitio;
    i8 buffer[MMMMXCVI];
    i32 totalis;
    i32 n;

    printf("--- Probans mittere/recipere ---\n");

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

    n = tcp_mittere(res.connexio, (constans i8*)petitio, (i32)strlen(petitio));
    CREDO_MAIOR_I32(n, 0);

    /* Recipere in loop usque connexio clausa */
    totalis = 0;
    fac
    {
        n = tcp_recipere(res.connexio, buffer, MMMMXCVI);
        si (n > 0)
        {
            totalis += n;
        }
    } dum (n > 0);

    printf("  Totalis receptum: %d bytes\n", totalis);
    CREDO_MAIOR_I32(totalis, 0);

    tcp_claudere(res.connexio);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - NULLUM ARGUMENTA
 * ======================================================================== */

interior vacuum
probatio_nullum_argumenta(Piscina* piscina)
{
    s32 n;

    (vacuum)piscina;

    printf("--- Probans nullum argumenta ---\n");

    /* tcp_mittere cum NIHIL */
    n = (s32)tcp_mittere(NIHIL, (constans i8*)"test", IV);
    CREDO_AEQUALIS_S32(n, -1);

    /* tcp_recipere cum NIHIL */
    {
        i8 buffer[CXXVIII];
        n = (s32)tcp_recipere(NIHIL, buffer, CXXVIII);
        CREDO_AEQUALIS_S32(n, -1);
    }

    /* tcp_claudere cum NIHIL - non debet crash */
    tcp_claudere(NIHIL);

    /* tcp_est_valida cum NIHIL */
    CREDO_FALSUM(tcp_est_valida(NIHIL));

    /* tcp_obtinere_fd cum NIHIL */
    CREDO_AEQUALIS_S32((s32)tcp_obtinere_fd(NIHIL), -1);

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
    printf("PROBATIONES TCP\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_tcp", LXIV * M);
    credo_aperire(piscina);

    /* Probationes locales (non requirunt rete) */
    probatio_optiones_default(piscina);
    probatio_error_descriptio(piscina);
    probatio_nullum_argumenta(piscina);

    /* Probationes rete (requirunt connexionem) */
    probatio_connexio_invalida(piscina);
    probatio_connexio_http(piscina);
    probatio_mittere_recipere(piscina);

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
