/* saltuarius_penicillus.h - Primitiva pingendi communia (Phase B)
 *
 * UNA via pingendi textum: UNITATES PINGENDAE (runae U6d:
 * graphemata per tessera_graphema_ponere, x per latitudinem
 * promotum), octeti imperii purgati in spatium (\t in cella
 * contractum cratis tesserae frangeret - terminal HT
 * interpretaretur), praecisio ad limitem panni in COLUMNIS: unitas
 * lata quae non capit non ponitur (tessera solum ad marginem CRATIS
 * praecidit).
 * Allocat NIHIL - nulla allocatio per quadrum EX CONSTRUCTIONE.
 * Extractum ex columnae (B2): visum eadem primitiva vult.
 */

#ifndef SALTUARIUS_PENICILLUS_H
#define SALTUARIUS_PENICILLUS_H

#include "latina.h"
#include "chorda.h"
#include "tessera.h"
#include "runae.h"
#include "quadrans.h"

/* Politica runarum operis (TesseraPolitica -> RunaePolitica):
 * mensura saltuarii eadem ac pictura tesserae */
RunaePolitica
saltuarius_pen_politica (
    constans TesseraOpus* opus);

/* Unitatem pingendam UNAM ad (x, y) ponere intra 'reliqua'
 * columnas: octetus imperii (C0/DEL) -> ' '; ceterum
 * tessera_graphema_ponere. Unitas latior quam reliqua NON ponitur:
 * reddit NIHIL. Aliter finem unitatis, latitudinem in *latitudo. */
constans i8*
saltuarius_pen_unitatem (
      TesseraOpus* opus,
              s32  x,
              s32  y,
      constans i8* cursor,
      constans i8* finis,
              i32  reliqua,
    TesseraStilus  stilus,
              i32* latitudo);

/* Textum ponere intra latitudo_max columnas; reddit COLUMNAS
 * positas */
i32
saltuarius_pen_textum (
      TesseraOpus* opus,
              s32  x,
              s32  y,
      constans i8* datum,
              i32  mensura,
              i32  latitudo_max,
    TesseraStilus  stilus);

/* Cellulas quadrantum lat x alt (ordine linearum) ab (x, y) ponere
 * (quadrans Q7): runa -> signum compactum, colores -> stilus */
vacuum
saltuarius_pen_cellulas (
                 TesseraOpus* opus,
                         s32  x,
                         s32  y,
    constans QuadransCellula* cellulae,
                         i32  lat,
                         i32  alt);

/* Literis NUL-terminatis */
vacuum
saltuarius_pen_literis (
           TesseraOpus* opus,
                   s32  x,
                   s32  y,
    constans character* literis,
                   i32  latitudo_max,
         TesseraStilus  stilus);

/* Numerum decimalem ponere; reddit latitudinem scriptam */
i32
saltuarius_pen_numerum (
      TesseraOpus* opus,
              s32  x,
              s32  y,
              s32  valor,
    TesseraStilus  stilus);

/* Digiti decimales valoris */
i32
saltuarius_pen_digiti (
    s32 valor);

#endif /* SALTUARIUS_PENICILLUS_H */
