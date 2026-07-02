/* silva_lexema.h - Lexator totalis silvae
 *
 * Proprietates (vide silva-spec-v2.md §8.2, simulatio ①):
 *   - TOTALIS: quaelibet series octetorum lexatur (OCTETUS_IGNOTUS,
 *     genera IMPERFECTUM) - numquam frangit, numquam octetos perdit
 *   - Scanner triviae UNUS + transitus attachmenti regulis actus
 *     (trailing = trivia eiusdem lineae usque ad primam NOVA_LINEA
 *     inclusive; leading = reliqua)
 *   - initium_lineae = lineae LOGICAE (CONTINUATIO-conscium)
 *   - Laminae \<nl> INTRA lexemata: valor mundus + scissurae
 *   - Fidelitas octetorum: emittere(lexare(x)) == x pro QUOLIBET x
 *   - Verba clausa ANGLICA solum: latina.h per expansorem fluit
 *     (definitiones compilatae), non per lexatorem
 */

#ifndef SILVA_LEXEMA_H
#define SILVA_LEXEMA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"

/* Lexare fontem: reddit Xar de SilvaToken* - lexemata vera (cum EOF ultimo)
 * cum triviis attachatis in spatia_ante/spatia_post.
 * Fons NON copiatur: valores lexematum in textum fontis spectant,
 * ergo textus vivere debet quamdiu lexemata vivunt. */
Xar*
silva_lexare (
    Piscina*            piscina,
    constans character* fons,
    i32                 mensura,
    s32                 fons_index);

/* Fluxus crudus: lexemata vera ET trivia interposita, ordine fontis.
 * Pro probationibus et instrumentis; silva_lexare hoc + attachmentum est. */
Xar*
silva_lexare_cruda (
    Piscina*            piscina,
    constans character* fons,
    i32                 mensura,
    s32                 fons_index);

/* Transitus attachmenti: fluxus crudus -> lexemata vera cum triviis.
 * Functio pura super regulas possessionis - separatim probabilis. */
Xar*
silva_spatia_attachere (
    Piscina* piscina,
    Xar*     cruda);

/* Emittere fluxum attachatum ad octetos - oraculum fidelitatis.
 * Scissurae reinseruntur; trivia verbatim. */
chorda
silva_lexemata_emittere (
    Piscina* piscina,
    Xar*     lexemata);

#endif /* SILVA_LEXEMA_H */
