/* moneta.h - Cusio identitatum (ULID, nanoid)
 *
 * ULID tempus in fronte fert (ordo creationis gratis, praefixa
 * longa); nanoid fortuita sola (praefixa brevia, ordo nullus);
 * nanoid plagulae pro nominibus plagularum (2026-09-10).
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
    i32  mensura);

#define MONETA_NANOID_LONGITUDO           21
#define MONETA_NANOID_MENSURA             22   /* + terminator */
#define MONETA_NANOID_PLAGULAE_LONGITUDO  12
#define MONETA_NANOID_PLAGULAE_MENSURA    13   /* + terminator */

/* Alphabeta ut macra: vocans eadem signa legit quae moneta scribit
 * (validatio, aut nanoid longitudinis alterius per
 * moneta_nanoid_alphabeto). Ordo signorum distributionem non
 * mutat - solum COPIA signorum. */
#define MONETA_NANOID_ALPHABETUM \
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-"
#define MONETA_NANOID_PLAGULAE_ALPHABETUM \
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

/* Nanoid: 21 signa ex MONETA_NANOID_ALPHABETUM (64 signa, 6 bits
 * singula = 126 bits) in effusio (>= MONETA_NANOID_MENSURA octeti).
 * Contractus moneta_nanoid_alphabeto. */
b32
moneta_nanoid (
    character* effusio);

/* Nanoid plagulae: 12 signa ex 0-9 A-Z (36 signa, ~62 bits) in
 * effusio (>= MONETA_NANOID_PLAGULAE_MENSURA octeti). Pro nominibus
 * plagularum: maiusculae solae, quia systema plagularum macOS casum
 * ordinarie ignorat ('aB' et 'Ab' eadem plagula essent); nec '-'
 * nec '_', ergo nomen numquam ut vexillum imperii incipit.
 * Contractus moneta_nanoid_alphabeto. */
b32
moneta_nanoid_plagulae (
    character* effusio);

/* Nanoid configurabile: longitudo signa ex alphabetum (literae
 * NUL-terminatae, 2..255 signa DISTINCTA) in effusio (>= longitudo
 * + 1 octeti), terminatore addito.
 *
 * UNIFORMITER: octetus fortuitus per larvam tegitur (minima 2^k - 1
 * quae indicem ultimum tegit) et index extra alphabetum ABICITUR -
 * modulus signa prima praeferret (256 = 7 x 36 + 4: in alphabeto 36
 * signorum signa prima quattuor 8/256, cetera 7/256). Signum bis
 * datum recusatur eadem de causa: inclinatio tacita.
 *
 * Fortuita ex moneta_octeti_fortuiti, SINE RESERVO: nanoid tempus
 * non habet quod unicitatem praestet ut ULID, ergo reservum
 * praevisibile identitates geminas pareret. Redde FALSUM si effusio
 * aut alphabetum NIHIL, longitudo 0, alphabetum malum, aut fons
 * deficit; effusio tunc "" (si non NIHIL) - numquam identitas
 * dimidia. */
b32
moneta_nanoid_alphabeto (
             character* effusio,
                   i32  longitudo,
    constans character* alphabetum);

#endif /* MONETA_H */
