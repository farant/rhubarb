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

/* Varians SINE monotonia: fortuita recentia quaque vocatione.
 * Gemini eiusdem ms trans partem fortuitam totam differunt - ergo
 * praefixa brevia (tempus 10 + pauca fortuita) eos discernunt,
 * quod monotonia vetat (gemini monotonici solo signo ultimo
 * differunt). Pro stampis capturarum (2026-07-30); ordo
 * generationis NON lexicographicus intra ms. */
vacuum
moneta_ulid_fortuita (
    character* effusio);

/* Octeti fortuiti ex /dev/urandom - SINE RESERVO.
 *
 * Redde FALSUM si fons abest aut deficit; effusio tunc INTACTA.
 * Vocans REFUSAT, non pergit.
 *
 * Cur sine reservo, cum moneta_ulid reservum habeat: ULID
 * UNICITATEM poscit, quam miscela temporis praestare potest;
 * SECRETUM impraevisibilitatem poscit, quam praestare NON potest.
 * Reservum tacitum hic clavem divinabilem pareret - defectus
 * qui ut successus spectat. Ergo duae functiones, non una. */
b32
moneta_octeti_fortuiti (
    i8* effusio,
    i32 mensura);

#endif /* MONETA_H */
