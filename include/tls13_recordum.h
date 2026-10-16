/* tls13_recordum.h - stratum recordorum TLS 1.3 (RFC 8446 §5.2-5.4,
 * tls13-spec II.1): protectio unius directionis (secretum traffici ->
 * clavis + iv + numerus sequentiae), sigillatio TLSInnerPlaintext in
 * TLSCiphertext et apertio cum verificatione. Cifrae: AES-128-GCM,
 * AES-256-GCM, ChaCha20-Poly1305 (BearSSL, tempore constanti).
 *
 * Nec fossae nec pactio hic: machina (tls13.h) recorda integra
 * tradit et accipit. Protectio in piscina vocantis; numerus
 * sequentiae post sigillationem aut apertionem BONAM progreditur
 * (apertio fracta eum non movet - connexio tunc finitur). */
#ifndef TLS13_RECORDUM_H
#define TLS13_RECORDUM_H

#include "latina.h"
#include "piscina.h"

#define TLS13_AES_128_GCM_SHA256        0x1301
#define TLS13_AES_256_GCM_SHA384        0x1302
#define TLS13_CHACHA20_POLY1305_SHA256  0x1303

/* recordum maximum in filo: caput V + 2^14 + 256 (RFC 8446 5.2) */
#define TLS13_RECORDUM_MAXIMUM  (XVI * MXXIV + CCLVI + V)

/* ContentType (RFC 8446 B.1) */
#define TLS13_CONTENTUM_CCS     XX
#define TLS13_CONTENTUM_ALERTA  XXI
#define TLS13_CONTENTUM_PACTIO  XXII
#define TLS13_CONTENTUM_DATA    XXIII

/* reditus tls13_aperire negativi */
#define TLS13_RECORDUM_MAC    (-I)   /* bad_record_mac */
#define TLS13_RECORDUM_MOLES  (-II)  /* record_overflow */
#define TLS13_RECORDUM_FORMA  (-III) /* decode / unexpected_message */

/* hash cifrae (TLS13_SHA256 / TLS13_SHA384); 0 si cifra ignota */
i32
tls13_cifra_genus (
    i32 cifra);

nomen structura Tls13Protectio Tls13Protectio;

/* protectio ex secreto traffici (mensura hash cifrae); sequentia 0.
 * NIHIL si cifra ignota aut piscina exhausta */
Tls13Protectio*
tls13_protectio_creare (
             i32  cifra,
     constans i8* secretum,
         Piscina* piscina);

/* sigillat data (n <= 2^14) ut genus_contenti: caput 17 03 03 + AEAD in
 * exitus; reddit longitudinem totam, aut 0 si n nimis magnum aut
 * capacitas deficit */
i32
tls13_obsignare (
    Tls13Protectio* protectio,
               i32  genus_contenti,
       constans i8* data,
               i32  n,
                i8* exitus,
               i32  capacitas);

/* aperit recordum totum (caput V + corpus): contentum in exitus,
 * *genus_contenti; reddit longitudinem contenti (>= 0) aut
 * TLS13_RECORDUM_MAC / _MOLES / _FORMA */
s32
tls13_aperire (
    Tls13Protectio* protectio,
       constans i8* recordum,
               i32  n,
                i8* exitus,
               i32  capacitas,
               i32* genus_contenti);

/* KeyUpdate (RFC 8446 7.2): secretum -> secretum proximum, claves
 * novae, sequentia 0 */
vacuum
tls13_protectio_renovare (
    Tls13Protectio* protectio);

#endif /* TLS13_RECORDUM_H */
