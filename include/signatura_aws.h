/* signatura_aws.h - AWS Signature Version 4 (horreum-spec II.2)
 *
 * PURA: tempus a vocante datur, nihil I/O, nihil S3 nisi modus eius -
 * via SEMEL codificatur et numquam normalizatur ("/a/../b", "//", "./"
 * ut data signantur). Secretum numquam in resultu, errore aut capite;
 * clavis signandi derivata (AWS4+secretum -> dies -> regio ->
 * servitium -> aws4_request) post usum zephyratur.
 *
 * Formae canonicae (vectores aws-c-auth v4, probationes/fixa/
 * signatura_aws): titula capitum minuscula et ordinata; valores
 * praecisi, spatia interiora (et lineae continuatae) in unum
 * contracta; titula duplicata valores coniungunt ',' ordine dato;
 * quaestio ordinata per clavem CODIFICATAM deinde valorem, "k=" pro
 * valore vacuo. Codificatio: RFC 3986 unreserved servantur, cetera
 * %XX maiusculae.
 *
 *   SignaturaPetitio sp;  memset(&sp, 0, sizeof sp);  ... campi ...
 *   SignaturaResultus r = signatura_aws_capitibus(&sp, piscina);
 *   si (r.successus) "Authorization: " + r.auctoritas,
 *                    et capita r.capita_addita */
#ifndef SIGNATURA_AWS_H
#define SIGNATURA_AWS_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "fasti.h"   /* MomentumSec */

/* <aedilis corpus="lib/signatura_aws.c"/> */

#define SIGNATURA_AWS_DURATIO_MAXIMA (DCIV * M + DCCC)   /* VII dies */
#define SIGNATURA_AWS_SECRETUM_MAXIMUM CCLVI               /* octeti */

nomen structura {
    chorda titulus;
    chorda valor;
} SignaturaPar;

nomen structura {
    /* "GET", "PUT", ...; Host exactus; via DECODIFICATA ("/a b/x") */
    constans character* methodus;
                chorda  hospes;
                chorda  via;

    /* paria DECODIFICATA; capita praeter Host et x-amz-date */
    constans SignaturaPar* quaestio;
                      i32  quaestio_numerus;
    constans SignaturaPar* capita;
                      i32  capita_numerus;

    /* corpus_sigillum: hex SHA-256 aut "UNSIGNED-PAYLOAD" (vacuum =
     * SHA-256 corporis vacui); corpus_capite: capitibus addit
     * x-amz-content-sha256; servitium "s3"; duratio: quaestione
     * 1..MAXIMA s */
         chorda corpus_sigillum;
            b32 corpus_capite;
    MomentumSec tempus;
         chorda regio;
         chorda servitium;
         chorda clavis_id;
         chorda secretum;
            i32 duratio;
} SignaturaPetitio;

nomen structura {
                   b32 successus;
                chorda error;              /* sine secreto */
                chorda petitio_canonica;
                chorda chorda_signanda;
                chorda signatura;          /* hex minusculae */
                chorda auctoritas;         /* valor Authorization */
                chorda dies_amz;           /* "20150830T123600Z" */
                chorda quaestio_signata;   /* tota codificata, cum
                                             * X-Amz-Signature */
    constans SignaturaPar* capita_addita;   /* x-amz-date [,
                                             * x-amz-content-sha256] */
                   i32 capita_addita_numerus;
} SignaturaResultus;

/* signatio per caput Authorization */
SignaturaResultus
signatura_aws_capitibus (
    constans SignaturaPetitio* petitio,
                      Piscina* piscina);

/* praesignatio: X-Amz-* in quaestione; capita data signantur (Host
 * semper), nulla addita */
SignaturaResultus
signatura_aws_quaestione (
    constans SignaturaPetitio* petitio,
                      Piscina* piscina);

/* YYYYMMDDTHHMMSSZ (UTC) */
chorda
signatura_aws_dies (
    MomentumSec  tempus,
        Piscina* piscina);

/* RFC 3986 unreserved servantur, cetera %XX maiusculae; '/' servatur
 * si servare_virgulam */
chorda
signatura_aws_codificare (
     chorda  s,
        b32  servare_virgulam,
    Piscina* piscina);

#endif /* SIGNATURA_AWS_H */
