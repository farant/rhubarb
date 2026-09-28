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
#include <time.h>


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
    res = tls_connectere("non.existens.hospes.example", CDXL + III,
        piscina);
    CREDO_FALSUM(res.successus);
    CREDO_AEQUALIS_I32(res.error, TLS_ERROR_TCP);
    CREDO_NIHIL(res.connexio);

    printf("  TCP error: ");
    si (res.error_descriptio.mensura > 0)
    {
        printf("%.*s", res.error_descriptio.mensura,
            res.error_descriptio.datum);
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
           TlsResultus  res;
    constans character* petitio;
                    i8  buffer[IV * MXXIV];
                   s32  n;
                   b32  misit;

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
    n = tls_recipere(res.connexio, buffer, IV * MXXIV - I);
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

    /* Certificatum hospitis (villa V1): VISIO MUNDI - quod hospes
     * vere praebet, non quod in disco eius iacet. Dies reliqui
     * huius columnae villae fons sunt. */
    {
        TlsCertificatum cert;
        b32 habitum = tls_certificatum_obtinere(res.connexio, &cert);

        CREDO_VERUM(habitum);
        CREDO_VERUM(cert.valida);
        /* fenestra validitatis sana: initium ante finem, et finis
         * post MMXX-01-01 (1577836800) - certificatum vivum est */
        CREDO_VERUM(cert.non_post > cert.non_ante);
        CREDO_VERUM(cert.non_post > (s64)1577836800);
        CREDO_VERUM(cert.subiectum.mensura > 0);
        printf("  Certificatum: %.*s (dies reliqui ~%ld)\n",
               cert.subiectum.mensura, cert.subiectum.datum,
               (longus)((cert.non_post - (s64)time(NIHIL)) / 86400));
    }

    /* Claudere */
    tls_claudere(res.connexio);
    CREDO_FALSUM(tls_est_valida(res.connexio));

    /* post clausuram: recusatio munda, non ruina */
    {
        TlsCertificatum cert;

        CREDO_FALSUM(tls_certificatum_obtinere(res.connexio, &cert));
        CREDO_FALSUM(cert.valida);
    }

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - MITTERE/RECIPERE
 * ======================================================================== */

interior vacuum
probatio_mittere_recipere(Piscina* piscina)
{
           TlsResultus  res;
    constans character* petitio;
                    i8  buffer[IV * MXXIV];
                    i8* totum;
                   i32  totalis;
                   s32  n;

    printf("--- Probans mittere/recipere (LXIV KB integri) ---\n");

    res = tls_connectere("httpbin.org", CDXL + III, piscina);
    si (!res.successus)
    {
        printf("  NOTA: Connexio fallita (rete non disponibilis?)\n");
        printf("\n");
        redde;
    }

    /* Corpus multo maius quam recordum TLS unum (XVI KB) et buffer
     * (IV KB): olim tls_recipere 0 ('clausa') reddebat cum SSLRead
     * errSSLWouldBlock sine octetis dabat - ansa ut haec in medio
     * exibat (lapide briar-feedback bugs/015-016). 'totalis > 0'
     * solum asserere id numquam vidit. */
    petitio = "GET /bytes/65536 HTTP/1.1\r\n"
              "Host: httpbin.org\r\n"
              "Connection: close\r\n"
              "\r\n";

    n = tls_mittere(res.connexio, (constans i8*)petitio,
        (i32)strlen(petitio));
    CREDO_MAIOR_S32(n, 0);

    /* Recipere in loop usque connexio clausa */
    totum = (i8*)piscina_allocare(piscina, (i64)(CXXVIII * M * VIII));
    totalis = 0;
    fac
    {
        n = tls_recipere(res.connexio, buffer, IV * MXXIV);
        si (n > 0 && totalis + (i32)n <= CXXVIII * M * VIII)
        {
            memcpy(totum + totalis, buffer, (size_t)n);
            totalis += (i32)n;
        }
    } dum (n > 0);

    printf("  Totalis receptum: %u bytes (exitus ansae %d)\n", totalis,
           (integer)n);
    CREDO_AEQUALIS_S32(n, 0);   /* EOF mundus, non tempus neque error */

    /* corpus == Content-Length (in eodem responso - contentum ipsum
     * mutari potest, consensus non) */
    {
                       i32  i;
                       i32  finis_capitum  = 0;
                       i32  promissum      = 0;
        constans character* cl;

        per (i = 0; i + III < totalis; i++)
        {
            si (   totum[i]      == '\r' && totum[i + I] == '\n'
                && totum[i + II] == '\r' && totum[i + III] == '\n')
            {
                finis_capitum = i + IV;
                frange;
            }
        }
        CREDO_MAIOR_I32(finis_capitum, 0);
        totum[finis_capitum > 0 ? finis_capitum - I : 0] = '\0';
        cl = strstr((constans character*)totum, "Content-Length: ");
        si (cl == NIHIL)
        {
            cl = strstr((constans character*)totum, "content-length: ");
        }
        CREDO_NON_NIHIL(cl);
        si (cl != NIHIL)
        {
            per (cl += XVI; *cl >= '0' && *cl <= '9'; cl++)
            {
                promissum = promissum * X + (i32)(*cl - '0');
            }
        }
        printf("  Content-Length %u, corpus receptum %u\n", promissum,
               totalis - finis_capitum);
        CREDO_AEQUALIS_I32(promissum, (i32)(XVI * IV * MXXIV));
        CREDO_AEQUALIS_I32(totalis - finis_capitum, promissum);
    }

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
    n = tls_mittere(NIHIL, (constans i8*)"test", IV);
    CREDO_AEQUALIS_S32(n, -1);

    /* tls_recipere cum NIHIL */
    {
        i8 buffer[CXXVIII];
        n = tls_recipere(NIHIL, buffer, CXXVIII);
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
           TlsOptiones  opt;
           TlsResultus  res;
    constans character* petitio;
                    i8  buffer[IV * MXXIV];
                   s32  n;

    printf("--- Probans connectere cum optionibus ---\n");

    /* Optiones cum verificare_certificatum = VERUM (default) */
    opt = tls_optiones_default();
    CREDO_VERUM(opt.verificare_certificatum);

    res = tls_connectere_cum_optionibus("httpbin.org", CDXL + III, &opt,
        piscina);
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

    n = tls_mittere(res.connexio, (constans i8*)petitio,
        (i32)strlen(petitio));
    CREDO_MAIOR_S32(n, 0);

    n = tls_recipere(res.connexio, buffer, IV * MXXIV - I);
    CREDO_MAIOR_S32(n, 0);
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
principale (vacuum)
{
    Piscina* piscina;
        b32  successus;

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
