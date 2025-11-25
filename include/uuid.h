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

/* ==================================================
 * UUIDv5 - Name-Based Unique Identifiers (SHA-1)
 * ================================================== */

/* UUIDv5 format:
 * - SHA-1 hash of namespace UUID + name
 * - 4 bits: version (0101 = 5)
 * - 2 bits: variant (10)
 *
 * Deterministic: same namespace + name = same UUID
 * Useful for scaffolding/migrations where stable IDs needed
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

/* Creare novum UUIDv5 ex namespace et nomine
 * SHA-1(namespace + appellatio) = identificator deterministicus
 *
 * namespace:   Chorda UUID namespace (e.g., UUID_NAMESPACE_DNS)
 * appellatio:  Nomen ad hashing (e.g., "Page:root")
 *
 * Redde: Chorda UUIDv5, vel chorda vacuum si fractura
 */
chorda
uuidv5_creare(
    Piscina*            piscina,
    chorda              namespace_uuid,
    constans character* appellatio);

/* Creare UUIDv5 ex genere et titulo (conveniens pro entitates)
 * Utitur namespace fixo interno, concatenat genus + ":" + titulus
 *
 * genus:   Genus entitatis (e.g., "Page", "Folder")
 * titulus: Titulus entitatis (e.g., "root", "home")
 *
 * Redde: Chorda UUIDv5, vel chorda vacuum si fractura
 */
chorda
uuidv5_ex_genere_et_titulo(
    Piscina*            piscina,
    constans character* genus,
    constans character* titulus);


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

/* Verificare si UUID est UUIDv5
 *
 * Redde: VERUM si versio 5, FALSUM si non
 */
b32
uuid_est_v5(
    chorda uuid_chorda);

#endif /* UUID_H */
