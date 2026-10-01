/* modulus.h - Modulus stratus delineandi: cellula in pixelis nostris
 * (project-specs/tessellatio-plan.md D1, D4)
 *
 * Tria spatia: CELLULAE -> PIXELA NOSTRA (coordinatae Mandatorum) ->
 * PIXELA SCHIRMI. Modulus (Vitruvius: unitas ex qua omnes mensurae
 * derivantur) = magnitudo cellulae in pixelis nostris + extensio
 * superficiei + proportio schirmi (reservata: nondum adhibita).
 *
 * SCOPUS DECLARAT, HOSPES ELIGIT: fenestra modulum ex fonte habet
 * (fons_6x8: VI x VIII); terminalis quemvis accipit (ordinarie VI x
 * VIII, ut glyphus fenestrae unus = cellula una).
 *
 * TEXTUS PER SCOPUM MENSURATUR (D4): fons_6x8 runam quamque una
 * cellula pingit (ASCII et Latin-1 sola; ceterae = TOFU, etiam signa
 * combinantia), terminalis unitates runae (latae = II). Ergo "中" in
 * fenestra VI pixela, in terminali XII.
 *
 * DIVISIO: C89 directionem divisionis negativae implementationi
 * relinquit - functiones hic PAVIMENTUM et PROXIMUM manu computant
 * (coordinatae negativae ex translatione et sectione oriuntur).
 *
 * PURUS: nulla allocatio, nullus status. Fines degradant, numquam
 * frangunt: cellula <= 0 -> I.
 */

#ifndef MODULUS_H
#define MODULUS_H

/* <aedilis corpus="lib/modulus.c"/> */

#include "latina.h"
#include "chorda.h"
#include "runae.h"

/* Numerator / denominator (> 0): proportio exacta, nulla fluitans. */
nomen structura {
    s32 numerator;
    s32 denominator;
} ModulusProportio;

nomen structura {
               s32 cellula_latitudo;    /* pixela nostra per cellulam */
               s32 cellula_altitudo;
               s32 extensio_latitudo;   /* superficies, pixela nostra */
               s32 extensio_altitudo;
  ModulusProportio schirmus_x;          /* pixela schirmi per nostrum */
  ModulusProportio schirmus_y;          /* (reservata; ordinarie 1/1) */
} Modulus;

/* Regula latitudinis textus scopi (D4). */
nomen enumeratio {
    MODULUS_MENSOR_FONTIS = 0,   /* runa (aut octetus invalidus) = I */
    MODULUS_MENSOR_RUNARUM       /* unitates runae sub politica */
} ModulusMensorGenus;

nomen structura {
    ModulusMensorGenus genus;
         RunaePolitica politica;    /* RUNARUM solum */
} ModulusMensor;

/* Modulus cum cellula et extensione; proportiones 1/1. Cellula <= 0
 * -> I; extensio < 0 -> 0. */
Modulus
modulus_creare (
    s32 cellula_latitudo,
    s32 cellula_altitudo,
    s32 extensio_latitudo,
    s32 extensio_altitudo);

/* Pixelum nostrum -> columna / linea cellulae: PAVIMENTUM (x = -1 ->
 * columna -1). Positio primitivi (D5). */
s32
modulus_columna (
    constans Modulus* m,
                 s32  x);

s32
modulus_linea (
    constans Modulus* m,
                 s32  y);

/* Margo -> margo cellulae PROXIMUS (dimidium sursum): rectangula
 * impleta et sectio (D5). x = 9, cellula 6 -> 2; x = 8 -> 1. */
s32
modulus_columna_proxima (
    constans Modulus* m,
                 s32  x);

s32
modulus_linea_proxima (
    constans Modulus* m,
                 s32  y);

/* Extensio in cellulis (pavimentum). */
vacuum
modulus_extensio_cellularum (
    constans Modulus* m,
                 s32* columnae,
                 s32* lineae);

/* Textus -> magnitudo in pixelis nostris: latitudo = columnae lineae
 * longissimae x cellula_latitudo; altitudo = lineae x
 * cellula_altitudo. Lineae = I + numerus '\n' (textus vacuus: 0 x 0);
 * '\n' ipsa nullam columnam occupat. */
vacuum
modulus_textum_metiri (
    constans Modulus* m,
       ModulusMensor  mensor,
              chorda  textus,
                 s32* latitudo,
                 s32* altitudo);

#endif /* MODULUS_H */
