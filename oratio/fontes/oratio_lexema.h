/* oratio_lexema.h - Lexator orationis: fluxus totus, sine contextu (T2)
 *
 * Oratio lexari SINE contextu potest (contra markdown): litterae,
 * digiti, spatia, interpunctio, lineae novae ex octetis solis
 * agnoscuntur. Ergo hic lexator fluxus TOTIUS est: valores lexematum
 * concatenati == fons (lex octetorum), positio linea/columna
 * (I-basatae, columna = octetus in linea) in lexemate quoque.
 *
 * REGULAE (spec par. III, limen unicode decisio IV):
 *  - LITTERAE: cursus litterarum ASCII ET octetorum >= 0x80 (sequentiae
 *    UTF-8 litterae sunt, sine validatione aut classibus - historia
 *    unicode rhubarbi postea), EXCEPTIS: NBSP (C2 A0) = SPATIUM;
 *    blocus 'General Punctuation' U+2000..U+206F (E2 80 xx, E2 81 xx):
 *    U+2000..U+200A spatia = SPATIUM, cetera = INTERPUNCTIO (lineae,
 *    citationes curvae, ellipsis), quorum U+2019 (') inter litteras
 *    APOSTROPHUS est.
 *  - DIGITI: cursus '0'..'9'.
 *  - HYPHEN: '-' UNUS inter litteras (well-known); aliter '-' in
 *    INTERPUNCTIONE ('--', ' - ').
 *  - APOSTROPHUS: ''' aut U+2019 inter litteras (don't, Fran's).
 *  - PUNCTUM: '.' solum; cursus punctorum ('...') INTERPUNCTIO.
 *  - INTERPUNCTIO: octetus interpunctionis ASCII unus per lexema, nisi
 *    cursus EIUSDEM octeti ('---', '!!!', '...') lexema unum.
 *  - SPATIUM: cursus ' ' et tabularum. LINEA: "\n" | "\r\n" (crlf ut
 *    vexillum); LINEA_CR: '\r' solum. SIGNUM: octeti ceteri (imperii).
 *  - FINIS ultimum.
 * Nullum lexema "</or-" continet (genera diversa scindunt): forma cruda
 * proiectionis tuta; porta id asserit.
 */

#ifndef ORATIO_LEXEMA_H
#define ORATIO_LEXEMA_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "materia_token.h"

/* Xar de MateriaToken* (fons ZEPHYRUM), FINIS ultimum; NIHIL = memoria. */
Xar*
oratio_lexare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

/* Lexema DERIVATUM (fons I): valor in piscina, byte_offset -I, positio
 * ex origine (sedes portata). */
MateriaToken*
oratio_lexema_derivatum (
                  Piscina* piscina,
                      s32  genus,
                   chorda  valor,
    constans MateriaToken* origo);

/* An octetus litterae sit (ASCII littera aut >= 0x80 extra exceptiones
 * supra); *longitudo = octeti sequentiae (I pro ASCII). */
b32
oratio_littera_est (
    constans character* fons,
                   s32  i,
                   s32  ad,
                   i32* longitudo);

#endif /* ORATIO_LEXEMA_H */
