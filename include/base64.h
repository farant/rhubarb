/* base64.h - Base64 Codificatio et Decodificatio
 *
 * Bibliotheca pro Base64 encoding/decoding (RFC 4648).
 * Utilis pro API authentication, embedding binary in JSON, data URIs.
 *
 * USUS:
 *   chorda codificata = base64_codificare(data, mensura, piscina);
 *   Base64Fructus fructus = base64_decodificare(codificata, piscina);
 */

#ifndef BASE64_H
#define BASE64_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* Fructus decodificationis */
nomen structura {
    i8* datum;
    i32 mensura;
} Base64Fructus;


/* ========================================================================
 * FUNCTIONES - CODIFICATIO
 * ======================================================================== */

/* Codificare data binaria ad Base64 chorda
 *
 * datum:    Data binaria ad codificandum
 * mensura:  Longitudo datorum in bytes
 * piscina:  Arena pro allocatio
 *
 * Redde: chorda Base64 codificata
 */
chorda
base64_codificare(
    constans i8* datum,
    i32          mensura,
    Piscina*     piscina);


/* ========================================================================
 * FUNCTIONES - DECODIFICATIO
 * ======================================================================== */

/* Decodificare Base64 chorda ad data binaria
 *
 * codificata: Chorda Base64 ad decodificandum
 * piscina:    Arena pro allocatio
 *
 * Redde: Base64Fructus cum data decodificata
 *        (datum = NIHIL si error)
 */
Base64Fructus
base64_decodificare(
    chorda   codificata,
    Piscina* piscina);


/* ========================================================================
 * FUNCTIONES - UTILITAS
 * ======================================================================== */

/* Calculare longitudo output pro codificatio
 *
 * mensura_originalis: Longitudo datorum originalium
 *
 * Redde: Longitudo chordae Base64 (cum padding)
 */
i32
base64_longitudo_codificata(i32 mensura_originalis);

/* Calculare longitudo output pro decodificatio
 *
 * codificata: Chorda Base64 (pro inspectione padding)
 *
 * Redde: Longitudo datorum decodificatorum
 */
i32
base64_longitudo_decodificata(chorda codificata);


#endif /* BASE64_H */
