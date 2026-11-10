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

/* fines tecti: margo circa pixela tecta, et gradus reticuli ad quem
 * fines aptantur (exemplaria formas suas reticulo octonario ligat:
 * fines non aptati formas inter quadra movent) */
#define STATUAE_FINIUM_MARGO  VIII
#define STATUAE_FINIUM_GRADUS VIII

/* praesupputationes exemplarium in latere: fractiones PIXELORUM
 * STATUAE (larva). Praesupputatio cogens regiones inter quadra eligit
 * et omittit (maximae primum), quod tremorem auget (mensum
 * 2026-11-10: 15 / 30 % statuae -> plus mutationum inter quadra;
 * 45 % numquam cogit). Hachurae 25 %: periculum Frani - statuas
 * multum hachuratas solas tangit (sine limite 12 / 25 / 39 % in
 * lapide / fungo / dracone). Pictogrammata 45 %: non cogit. */
#define STATUAE_PRAESUPPUTATIO_HACHURARUM    0.25
#define STATUAE_PRAESUPPUTATIO_PICTOGRAMMATUM 0.45

/*
 * statuae_fines_tecti - rectangulum minimum circa pixela tecta
 *
 * @tectum: I per pixelum (StatuaPictura.tectum), ordo summus primus
 * @x, @y, @latitudo_finium, @altitudo_finium: rectangulum; margo
 *   STATUAE_FINIUM_MARGO additur, anguli ad multipla
 *   STATUAE_FINIUM_GRADUS extra aptantur, deinde ad picturam coguntur
 *
 * Redde: FALSUM si nihil tectum aut argumentum NIHIL (exitus non
 * mutantur)
 */
b32
statuae_fines_tecti (
    constans i8* tectum,
            i32  latitudo,
            i32  altitudo,
            i32* x,
            i32* y,
            i32* latitudo_finium,
            i32* altitudo_finium);

/*
 * statuae_quadrum_pingere - quadrum unum
 *
 * Omnes XVI colores activi. Pixela quae statua non tegit charta
 * munda sunt, quidquid dithering prope marginem fecit. Latitudo aut
 * altitudo 0: imago vacua (pixela NIHIL), VERUM.
 *
 * Modus exemplaria solum intra statuae_fines_tecti currit (pictura
 * praecisa, deinde reposita): pixela vacua circa statuam tempus non
 * sumunt. Pixela tecta eadem sunt ac si praecisio manu dithering
 * acciperet cum tecto praeciso ut larva
 * (dithering_exemplaria_velox_cum_larva) et praesupputationibus
 * STATUAE_PRAESUPPUTATIO_*; a dithering quadri totius differre licet.
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
