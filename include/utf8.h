/*
 * utf8.h - Bibliotheca UTF-8 decodendi
 *
 * Functiones purae pro decodendo UTF-8 ad codepoints.
 * Nulla allocatio, nulla dependentia praeter latina.h
 */

#ifndef UTF8_H
#define UTF8_H

#include "latina.h"

/*
 * utf8_decodere - Decodere unam runam ex sequentia UTF-8
 *
 * @ptr: Indicator ad indicatorem currentem (promovebitur)
 * @finis: Finis buffer (non legendum)
 *
 * Redde: Codepoint (0-0x10FFFF), vel -1 si invalidum/incompletum
 *
 * Nota: Indicator promovetur ad proximam runam post decodificationem
 */
s32 utf8_decodere(constans i8** ptr, constans i8* finis);

/*
 * utf8_longitudo_byte - Quot bytes hic byte principalis indicat?
 *
 * @byte: Primus byte sequentiae
 *
 * Redde: 1-4 pro validis, 0 pro invalidis
 *
 * Exempla:
 *   0xxxxxxx -> 1 (ASCII)
 *   110xxxxx -> 2
 *   1110xxxx -> 3
 *   11110xxx -> 4
 *   10xxxxxx -> 0 (continuatio, non principalis)
 *   11111xxx -> 0 (invalidum)
 */
s32 utf8_longitudo_byte(i8 byte);

/*
 * utf8_est_continuatio - An hic byte est continuatio? (10xxxxxx)
 *
 * @byte: Byte examinandus
 *
 * Redde: VERUM si continuatio, FALSUM aliter
 */
b32 utf8_est_continuatio(i8 byte);

/*
 * utf8_numerare_runas - Numerare codepoints in buffer
 *
 * @datum: Buffer UTF-8
 * @mensura: Longitudo buffer in bytes
 *
 * Redde: Numerus runarum (codepoints), bytes invalidi numerantur ut 1
 */
s32 utf8_numerare_runas(constans i8* datum, s32 mensura);

/*
 * utf8_proxima_runa - Promovere ad proximam limitem runae
 *
 * @ptr: Indicator currentis
 * @finis: Finis buffer
 *
 * Redde: Indicator novus ad proximam runam
 */
constans i8* utf8_proxima_runa(constans i8* ptr, constans i8* finis);

/*
 * utf8_prior_runa - Retrocedere ad priorem limitem runae
 *
 * @ptr: Indicator currentis
 * @initium: Initium buffer
 *
 * Redde: Indicator novus ad priorem runam
 */
constans i8* utf8_prior_runa(constans i8* ptr, constans i8* initium);

#endif /* UTF8_H */
