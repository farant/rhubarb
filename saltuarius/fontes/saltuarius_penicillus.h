/* saltuarius_penicillus.h - Primitiva pingendi communia (Phase B)
 *
 * UNA via pingendi textum: runae directe in cellas (ambulatu
 * utf8), octeti imperii purgati (\t in cella contractum cratis
 * tesserae frangeret - terminal HT interpretaretur), praecisio ad
 * limitem panni (tessera solum ad marginem CRATIS praecidit).
 * Allocat NIHIL - nulla allocatio per quadrum EX CONSTRUCTIONE.
 * Extractum ex columnae (B2): visum eadem primitiva vult.
 */

#ifndef SALTUARIUS_PENICILLUS_H
#define SALTUARIUS_PENICILLUS_H

#include "latina.h"
#include "chorda.h"
#include "tessera.h"

/* Textum ponere; reddit runas positas */
i32
saltuarius_pen_textum (TesseraOpus* opus, s32 x, s32 y,
    constans i8* datum, i32 mensura, i32 runae_max,
    TesseraStilus stilus);

/* Literis NUL-terminatis */
vacuum
saltuarius_pen_literis (TesseraOpus* opus, s32 x, s32 y,
    constans character* literis, i32 runae_max,
    TesseraStilus stilus);

/* Numerum decimalem ponere; reddit latitudinem scriptam */
i32
saltuarius_pen_numerum (TesseraOpus* opus, s32 x, s32 y,
    s32 valor, TesseraStilus stilus);

/* Digiti decimales valoris */
i32
saltuarius_pen_digiti (s32 valor);

#endif /* SALTUARIUS_PENICILLUS_H */
