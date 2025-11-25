#ifndef FRIATIO_H
#define FRIATIO_H

#include "latina.h"
#include "chorda.h"

/* ==================================================
 * Friatio - Functiones Friationis (Hash Functions)
 *
 * "Friare" = to crumble, break into pieces
 * "Friatio" = the act of crumbling
 *
 * Continet:
 * - Functiones non-cryptographicae (pro tabulis dispersis)
 * - Functiones cryptographicae (pro UUIDs, checksums, etc.)
 * ================================================== */

/* ==================================================
 * Functiones Non-Cryptographicae (32-bit)
 *
 * Pro tabulis dispersis et distributione rapida.
 * NON securae pro cryptographia!
 * ================================================== */

/* FNV-1a (Fowler-Noll-Vo)
 * Rapida, bona distributio, simplex
 * Offset basis: 2166136261
 * Prime: 16777619
 */
i32
friatio_fnv1a(
    chorda clavis);

/* FNV-1a pro literis C */
i32
friatio_fnv1a_literis(
    constans character* literae,
    i32                 mensura);

/* DJB2 (Dan Bernstein)
 * Classica, rapida, formula: hash * 33 + c
 * Initium: 5381
 */
i32
friatio_djb2(
    chorda clavis);

/* DJB2 pro literis C */
i32
friatio_djb2_literis(
    constans character* literae,
    i32                 mensura);

/* ==================================================
 * SHA-1 (Secure Hash Algorithm 1)
 *
 * Cryptographica (sed OBSOLETA pro securitate!)
 * Producit 160 bits (20 bytes) digest
 * Usus: UUIDv5, checksums legacy, non pro securitate nova
 * ================================================== */

#define SHA1_DIGEST_MENSURA   XX    /* 20 bytes */
#define SHA1_BLOCK_MENSURA    LXIV  /* 64 bytes */

/* Contextus SHA-1 pro hashing incrementale */
nomen structura {
    i32 status[V];            /* A, B, C, D, E */
    i32 numerus[II];          /* Bit count (low, high) */
    i8  buffer[LXIV];         /* Input buffer */
} SHA1Contextus;

/* Initiare contextum SHA-1 */
vacuum
sha1_initiare(
    SHA1Contextus* ctx);

/* Addere data ad hash (potest vocare multiple) */
vacuum
sha1_addere(
    SHA1Contextus*  ctx,
    constans i8*    data,
    i32             mensura);

/* Finire et obtinere digest (20 bytes) */
vacuum
sha1_finire(
    SHA1Contextus* ctx,
    i8             digest[SHA1_DIGEST_MENSURA]);

/* Convenientia: hash data in uno passu */
vacuum
sha1_friare(
    constans i8* data,
    i32          mensura,
    i8           digest[SHA1_DIGEST_MENSURA]);

/* Convenientia: hash chorda in uno passu */
vacuum
sha1_friare_chorda(
    chorda clavis,
    i8     digest[SHA1_DIGEST_MENSURA]);

#endif /* FRIATIO_H */
