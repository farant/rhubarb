/*
 * statuae_quadrum.h - quadrum statuae: positura, pictura, dithering
 *
 * Gradus unus quem et latus vici et bin/statuae faciunt
 * (project-specs/statuae-spec.md IV, "A frame"): statua + cyclus +
 * tempus + visus + modus -> imago RGBA parata. Tempus -> positura ->
 * pictura -> dithering in palettam -> charta ubi statua non tegit.
 *
 * Purum et determinatum: eadem petitio idem quadrum dat; nihil
 * inter vocationes servatur.
 */

#ifndef STATUAE_QUADRUM_H
#define STATUAE_QUADRUM_H

/* <aedilis corpus="lib/statuae_quadrum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "imago_typus.h"
#include "statua.h"
#include "statua_pictura.h"

/* modi dithering; ordo = ordo in menu lateris */
nomen enumeratio {
    STATUAE_MODUS_ATKINSON = ZEPHYRUM,
    STATUAE_MODUS_BAYER,
    STATUAE_MODUS_EXEMPLARIA,   /* dithering_exemplaria_velox */
    STATUAE_MODUS_NULLUS,       /* color palettae proximus */
    /* sine paletta: pictura ipsa (solum instrumentum; latus eum non
     * offert) */
    STATUAE_MODUS_CRUDUS,
    STATUAE_MODORUM
} StatuaeModus;

/* titulus modi in statu et in linea mandati: "atkinson", "bayer",
 * "exemplaria", "nullum", "crudum"; NIHIL si extra fines */
constans character*
statuae_modi_titulus (
    StatuaeModus modus);

/* modus ex titulo; FALSUM si ignotus (*modus non mutatur) */
b32
statuae_modum_legere (
          chorda  titulus,
    StatuaeModus* modus);

nomen structura {
             s32 cyclus;      /* -1 = quies */
             f32 tempus;      /* secundae; involvitur */
     StatuaVisus visus;
    StatuaeModus modus;
             i32 latitudo;
             i32 altitudo;
             b32 charta_perlucida;  /* VERUM: pixela sine statua alpha
                                     * 0 habent (latus: fundus suus
                                     * apparet); FALSUM: color chartae
                                     * opacus (plagula PNG) */
} StatuaeQuadrumPetitio;

/* index palettae coloris chartae (Albus) */
#define STATUAE_CHARTA_INDEX V

/*
 * statuae_quadrum_pingere - quadrum unum
 *
 * Omnes XVI colores activi. Pixela quae statua non tegit charta
 * munda sunt, quidquid dithering prope marginem fecit. Latitudo aut
 * altitudo 0: imago vacua (pixela NIHIL), VERUM.
 *
 * Redde: FALSUM si statua aut petitio NIHIL, modus extra fines, aut
 * dithering deficit
 */
b32
statuae_quadrum_pingere (
                   constans Statua* statua,
    constans StatuaeQuadrumPetitio* petitio,
                             Imago* exitus,
                           Piscina* piscina);

#endif /* STATUAE_QUADRUM_H */
