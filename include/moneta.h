/* moneta.h - Cusio identitatum (ULID)
 *
 * Officina monetae una repositorii totius: tabularium (per
 * scrinium_ulid delegans) et codex (identitates annotationum,
 * frustum B 01KY3D7EJP) ex eodem puteo hauriunt - spatium
 * identitatis UNUM, cusio UNA. Extracta ex lib/scrinium.c
 * 2026-07-21 (scrinium sqlite vendicat - nimis grave pro
 * instrumentis quae solum cudere volunt).
 */

#ifndef MONETA_H
#define MONETA_H

#include "latina.h"

#define MONETA_ULID_MENSURA 27   /* 26 characteres + terminator */

/* ULID: 26 characteres Crockford base32 in effusio (>= 27 octeti);
 * monotona intra ms eundem (fasciculus processus). Fortuita ex
 * /dev/urandom; reservum = miscela temporis (deterioris qualitatis
 * sed numquam duplicata intra processum ob monotoniam). */
vacuum
moneta_ulid (
    character* effusio);

#endif /* MONETA_H */
