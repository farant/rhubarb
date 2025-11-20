#ifndef UUID_H
#define UUID_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

/* ==================================================
 * UUIDv7 - Time-Ordered Unique Identifiers
 * ================================================== */

/* UUIDv7 format:
 * - 48 bits: Unix timestamp (milliseconds)
 * - 4 bits: version (0111 = 7)
 * - 12 bits: random
 * - 2 bits: variant (10)
 * - 62 bits: random
 *
 * String format: xxxxxxxx-xxxx-7xxx-yxxx-xxxxxxxxxxxx
 * Mensura: 36 characteres (32 hex + 4 dashes)
 */

#define UUID_MENSURA_CHORDAE 36  /* 32 hex + 4 dashes */


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare novum UUIDv7
 * Tempus currens + randomitas = identificator unicus temporaliter ordinatus
 *
 * Redde: Chorda UUIDv7, vel chorda vacuum si fractura
 */
chorda
uuidv7_creare(
    Piscina* piscina);


/* ==================================================
 * Conversio
 * ================================================== */

/* Convertere UUID ex chorda ad bytes
 * Parse chorda format UUID ad 16 bytes
 *
 * Redde: VERUM si successus, FALSUM si format invalidus
 */
b32
uuid_ex_chorda(
    chorda  uuid_chorda,
    i8      bytes[XVI]);

/* Convertere UUID ex bytes ad chordam
 * Format 16 bytes ad chorda UUID
 *
 * Redde: Chorda UUID, vel chorda vacuum si fractura
 */
chorda
uuid_ad_chordam(
    i8       bytes[XVI],
    Piscina* piscina);


/* ==================================================
 * Validatio
 * ================================================== */

/* Verificare si chorda est UUID validus
 *
 * Redde: VERUM si format validus, FALSUM si non
 */
b32
uuid_est_validus(
    chorda uuid_chorda);

/* Verificare si UUID est UUIDv7
 *
 * Redde: VERUM si versio 7, FALSUM si non
 */
b32
uuid_est_v7(
    chorda uuid_chorda);

#endif /* UUID_H */
