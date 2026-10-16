/* tls13_clavis.h - tabula clavium TLS 1.3 (RFC 8446 §7.1, tls13-spec
 * II.1): HKDF-Extract/Expand (RFC 5869), HKDF-Expand-Label,
 * Derive-Secret, claves traffici, Finished, KeyUpdate, et transcriptum
 * currens nuntiorum pactionis (cum substitutione HelloRetryRequest).
 *
 * Functiones purae: nec fossae nec status globalis; transcriptum solum
 * statum habet, in piscina vocantis. Hash per 'genus' eligitur
 * (TLS13_SHA256 aut TLS13_SHA384) - cifra quaeque suum dat. Capsae
 * exitus vocans praebet: secreta et hash TLS13_HASH_MAXIMUS octetos
 * capiunt.
 *
 * Oraculum: RFC 5869 A.1 et RFC 8448 §3 (textus immutatus,
 * probationes/fixa/tls13/rfc8448.txt) - probatio_tls13_clavis. */
#ifndef TLS13_CLAVIS_H
#define TLS13_CLAVIS_H

#include "latina.h"
#include "piscina.h"

#define TLS13_SHA256        I
#define TLS13_SHA384        II
#define TLS13_HASH_MAXIMUS  XLVIII

/* mensura hash generis: XXXII aut XLVIII; 0 si genus ignotum */
i32
tls13_hash_mensura (
    i32 genus);

/* HKDF-Extract: prk = HMAC(sal, ikm); sal NIHIL = nulli mensurae hash
 * (RFC 5869 2.2); prk capit tls13_hash_mensura(genus) octetos */
vacuum
tls13_hkdf_extrahere (
             i32  genus,
     constans i8* sal,
             i32  sal_n,
     constans i8* ikm,
             i32  ikm_n,
              i8* prk);

/* HKDF-Expand (RFC 5869 2.3): mensura octeti ex prk et info; FALSUM si
 * genus ignotum aut mensura > CCLV * mensura hash */
b32
tls13_hkdf_expandere (
             i32  genus,
     constans i8* prk,
     constans i8* info,
             i32  info_n,
              i8* exitus,
             i32  mensura);

/* HKDF-Expand-Label (RFC 8446 7.1): titulus SINE praefixo "tls13 "
 * (functio id addit); contextus NIHIL cum ctx_n 0 = vacuus; FALSUM si
 * titulus aut contextus longior quam CCLV */
b32
tls13_expandere_titulo (
                   i32  genus,
           constans i8* secretum,
    constans character* titulus,
           constans i8* contextus,
                   i32  ctx_n,
                    i8* exitus,
                   i32  mensura);

/* Derive-Secret(secretum, titulus, Messages): hash_transcripti =
 * Transcript-Hash(Messages) iam computatum; exitus mensurae hash */
vacuum
tls13_secretum_derivare (
                   i32  genus,
           constans i8* secretum,
    constans character* titulus,
           constans i8* hash_transcripti,
                    i8* exitus);

/* claves recordorum ex secreto traffici (RFC 8446 7.3): clavis
 * (clavis_n octeti, "key") et iv (XII octeti, "iv") */
vacuum
tls13_claves_traffici (
             i32  genus,
     constans i8* secretum,
             i32  clavis_n,
              i8* clavis,
              i8* iv);

/* verificatio Finished (RFC 8446 4.4.4): HMAC(finished_key,
 * hash_transcripti), finished_key = Expand-Label(secretum_base,
 * "finished", "", Hash.length) */
vacuum
tls13_finis_calculare (
             i32  genus,
     constans i8* secretum_base,
     constans i8* hash_transcripti,
              i8* verificatio);

/* secretum traffici proximum post KeyUpdate (RFC 8446 7.2):
 * Expand-Label(secretum, "traffic upd", "", Hash.length) */
vacuum
tls13_secretum_proximum (
             i32  genus,
     constans i8* secretum,
              i8* exitus);

/* transcriptum currens: hash nuntiorum pactionis (cum capitibus IV
 * octetorum) ordine addita */
nomen structura Tls13Transcriptum Tls13Transcriptum;

/* NIHIL si genus ignotum aut piscina exhausta */
Tls13Transcriptum*
tls13_transcriptum_creare (
        i32  genus,
    Piscina* piscina);

vacuum
tls13_transcriptum_addere (
    Tls13Transcriptum* t,
          constans i8* octeti,
                  i32  mensura);

/* hash currens in 'hash' (mensura hash); statum non mutat - addere
 * postea pergere licet */
vacuum
tls13_transcriptum_digerere (
    constans Tls13Transcriptum* t,
                            i8* hash);

/* RFC 8446 4.4.1: post HelloRetryRequest ClientHello1 in transcripto
 * fit message_hash (fe 00 00 Hash.length || Hash(CH1)). Vocanda post
 * CH1 additum, ANTE HRR additum. */
vacuum
tls13_transcriptum_hrr (
    Tls13Transcriptum* t);

#endif /* TLS13_CLAVIS_H */
