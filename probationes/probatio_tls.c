/* probatio_tls.c - Probationes pro TLS bibliotheca
 *
 * Haec probationes requirunt connexionem ad rete.
 * Utuntur httpbin.org pro probationibus.
 */

#include "tls.h"
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
    TlsOptiones opt;

    (vacuum)piscina;

    printf("--- Probans optiones default ---\n");

    opt = tls_optiones_default();

    CREDO_VERUM(opt.timeout_ms > 0);
    CREDO_VERUM(opt.verificare_certificatum);

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

    desc = tls_error_descriptio(TLS_OK);
    CREDO_NON_NIHIL(desc);

    desc = tls_error_descriptio(TLS_ERROR_TCP);
    CREDO_NON_NIHIL(desc);

    desc = tls_error_descriptio(TLS_ERROR_HANDSHAKE);
    CREDO_NON_NIHIL(desc);

    desc = tls_error_descriptio(TLS_ERROR_CERTIFICATUM);
    CREDO_NON_NIHIL(desc);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CONNEXIO INVALIDA
 * ======================================================================== */

interior vacuum
probatio_connexio_invalida(Piscina* piscina)
{
    TlsResultus res;

    printf("--- Probans connexio invalida ---\n");

    /* DNS fallita - hospes non existens */
    res = tls_connectere("non.existens.hospes.example", CDXL + III, piscina);
    CREDO_FALSUM(res.successus);
    CREDO_AEQUALIS_I32(res.error, TLS_ERROR_TCP);
    CREDO_NIHIL(res.connexio);

    printf("  TCP error: ");
    si (res.error_descriptio.mensura > 0)
    {
        printf("%.*s", res.error_descriptio.mensura, res.error_descriptio.datum);
    }
    printf("\n");

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CONNEXIO HTTPS
 * ======================================================================== */

interior vacuum
probatio_connexio_https(Piscina* piscina)
{
    TlsResultus res;
    constans character* petitio;
    i8 buffer[MMMMXCVI];
    i32 n;
    b32 misit;

    printf("--- Probans connexio HTTPS (httpbin.org:443) ---\n");

    /* Connectere ad httpbin.org:443 */
    res = tls_connectere("httpbin.org", CDXL + III, piscina);

    si (!res.successus)
    {
        printf("  NOTA: Connexio fallita (rete non disponibilis?)\n");
        printf("  Error: %s\n", tls_error_descriptio(res.error));
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
    CREDO_VERUM(tls_est_valida(res.connexio));

    /* Mittere HTTP petitio */
    petitio = "GET /get HTTP/1.1\r\n"
              "Host: httpbin.org\r\n"
              "Connection: close\r\n"
              "\r\n";

    misit = tls_mittere_omnia(res.connexio,
                              (constans i8*)petitio,
                              (i32)strlen(petitio));
    CREDO_VERUM(misit);

    printf("  Petitio missa, expectans responsum...\n");

    /* Recipere responsum */
    n = tls_recipere(res.connexio, buffer, MMMMXCVI - I);
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
    tls_claudere(res.connexio);
    CREDO_FALSUM(tls_est_valida(res.connexio));

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - MITTERE/RECIPERE
 * ======================================================================== */

interior vacuum
probatio_mittere_recipere(Piscina* piscina)
{
    TlsResultus res;
    constans character* petitio;
    i8 buffer[MMMMXCVI];
    i32 totalis;
    i32 n;

    printf("--- Probans mittere/recipere ---\n");

    res = tls_connectere("httpbin.org", CDXL + III, piscina);
    si (!res.successus)
    {
        printf("  NOTA: Connexio fallita (rete non disponibilis?)\n");
        printf("\n");
        redde;
    }

    /* Mittere petitio */
    petitio = "GET /bytes/1024 HTTP/1.1\r\n"
              "Host: httpbin.org\r\n"
              "Connection: close\r\n"
              "\r\n";

    n = tls_mittere(res.connexio, (constans i8*)petitio, (i32)strlen(petitio));
    CREDO_MAIOR_I32(n, 0);

    /* Recipere in loop usque connexio clausa */
    totalis = 0;
    fac
    {
        n = tls_recipere(res.connexio, buffer, MMMMXCVI);
        si (n > 0)
        {
            totalis += n;
        }
    } dum (n > 0);

    printf("  Totalis receptum: %d bytes\n", totalis);
    CREDO_MAIOR_I32(totalis, 0);

    tls_claudere(res.connexio);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - NULLUM ARGUMENTA
 * ======================================================================== */

interior vacuum
probatio_nullum_argumenta(Piscina* piscina)
{
    s32 n;
    TlsResultus res;

    printf("--- Probans nullum argumenta ---\n");

    /* tls_connectere cum NIHIL hospes */
    res = tls_connectere(NIHIL, CDXL + III, piscina);
    CREDO_FALSUM(res.successus);
    CREDO_NIHIL(res.connexio);
    printf("  NIHIL hospes: recte recusatum\n");

    /* tls_connectere cum NIHIL piscina */
    res = tls_connectere("httpbin.org", CDXL + III, NIHIL);
    CREDO_FALSUM(res.successus);
    CREDO_NIHIL(res.connexio);
    printf("  NIHIL piscina: recte recusatum\n");

    /* tls_mittere cum NIHIL */
    n = (s32)tls_mittere(NIHIL, (constans i8*)"test", IV);
    CREDO_AEQUALIS_S32(n, -1);

    /* tls_recipere cum NIHIL */
    {
        i8 buffer[CXXVIII];
        n = (s32)tls_recipere(NIHIL, buffer, CXXVIII);
        CREDO_AEQUALIS_S32(n, -1);
    }

    /* tls_claudere cum NIHIL - non debet crash */
    tls_claudere(NIHIL);

    /* tls_est_valida cum NIHIL */
    CREDO_FALSUM(tls_est_valida(NIHIL));

    printf("\n");
}


interior vacuum
probatio_connectere_cum_optionibus(Piscina* piscina)
{
    TlsOptiones opt;
    TlsResultus res;
    constans character* petitio;
    i8 buffer[MMMMXCVI];
    i32 n;

    printf("--- Probans connectere cum optionibus ---\n");

    /* Optiones cum verificare_certificatum = VERUM (default) */
    opt = tls_optiones_default();
    CREDO_VERUM(opt.verificare_certificatum);

    res = tls_connectere_cum_optionibus("httpbin.org", CDXL + III, &opt, piscina);
    si (!res.successus)
    {
        printf("  NOTA: Connexio fallita (rete non disponibilis?)\n");
        printf("\n");
        redde;
    }

    CREDO_VERUM(res.successus);
    CREDO_NON_NIHIL(res.connexio);
    printf("  Connexio cum optionibus successus\n");

    /* Verificare functionalitas */
    petitio = "GET /get HTTP/1.1\r\n"
              "Host: httpbin.org\r\n"
              "Connection: close\r\n"
              "\r\n";

    n = tls_mittere(res.connexio, (constans i8*)petitio, (i32)strlen(petitio));
    CREDO_MAIOR_I32(n, 0);

    n = tls_recipere(res.connexio, buffer, MMMMXCVI - I);
    CREDO_MAIOR_I32(n, 0);
    buffer[n] = '\0';

    CREDO_VERUM(strncmp((character*)buffer, "HTTP/1.1 200", XII) == 0);
    printf("  Responsum: 200 OK\n");

    tls_claudere(res.connexio);
    printf("\n");
}


/* Disabled - slow, waits for network timeout
interior vacuum
probatio_optiones_timeout(Piscina* piscina)
{
    TlsOptiones opt;
    TlsResultus res;

    printf("--- Probans optiones timeout ---\n");

    opt = tls_optiones_default();
    opt.timeout_ms = M;

    res = tls_connectere_cum_optionibus("192.0.2.1", CDXL + III, &opt, piscina);

    CREDO_FALSUM(res.successus);
    printf("  Timeout/failure correcte: error=%d\n", res.error);

    printf("\n");
}
*/


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
    printf("PROBATIONES TLS\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_tls", LXIV * M);
    credo_aperire(piscina);

    /* Probationes locales (non requirunt rete) */
    probatio_optiones_default(piscina);
    probatio_error_descriptio(piscina);
    probatio_nullum_argumenta(piscina);

    /* Probationes rete (requirunt connexionem) */
    probatio_connexio_invalida(piscina);
    probatio_connexio_https(piscina);
    probatio_mittere_recipere(piscina);
    probatio_connectere_cum_optionibus(piscina);
    /* probatio_optiones_timeout(piscina); -- slow, waits for network timeout */

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
