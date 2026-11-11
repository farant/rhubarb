/*
 * mundus_pictura.h - pictura mundi: humus semel picta, quadrum
 *
 * HUMUS (project-specs/mundus-spec.md IV.5): locus solus, visu
 * parallelo fixo pictus (statua_scaenam_pingere) et dithering
 * Atkinson in palettam redactus, per REGIONES quadratas in craticula
 * pixelorum MUNDI (non lateris). Regio fit cum visus eam primum
 * attingit et deinde servatur: indices palettae et profunditas.
 * Quadrum humum suam ex regionibus copiat.
 *
 * Cur: pictura cruda pixelis integris lapsa eadem est, sed Atkinson
 * denuo currens quintam partem pixelorum mutat (spec III.3) - humus
 * per quadrum redacta toto visu micaret. Semel redacta et lapsa
 * quiescit.
 */

#ifndef MUNDUS_PICTURA_H
#define MUNDUS_PICTURA_H

/* <aedilis corpus="lib/mundus_pictura.c"/> */

#include "latina.h"
#include "piscina.h"
#include "statua.h"
#include "statua_pictura.h"

/* visus mundi fixus */
#define MUNDUS_GYRUS               45.0f
#define MUNDUS_INCLINATIO          35.0f
#define MUNDUS_PIXELA_PER_UNITATEM 40.0f
/* latus regionis humi (pixela) */
#define MUNDUS_REGIO               CCLVI
/* index et profunditas pixeli extra locum intra regionem factam (in
 * copia: fundus vocantis et haec profunditas) */
#define MUNDUS_PROFUNDITAS_VACUA   1.0e30f

nomen structura MundusHumus MundusHumus;

/* visus mundi cum origine data (pixela mundi), illuminatus */
StatuaScaenaVisus
mundus_visus (
    s32 origo_x,
    s32 origo_y);

/*
 * mundus_humus_creare - humum loci parare (nulla regio adhuc fit)
 *
 * @locus:   statua finita; vivere debet dum humus vivit
 * @activi:  colores palettae activi (XVI); copiantur
 * @piscina: regiones in ea servantur; vivere debet dum humus vivit
 *
 * Redde: NIHIL si argumenta NIHIL aut piscina deficit
 */
MundusHumus*
mundus_humus_creare (
    constans Statua* locus,
       constans b32* activi,
            Piscina* piscina);

/*
 * mundus_humus_copiare - rectangulum pixelorum mundi copiare
 *
 * @origo_x, @origo_y: pixelum mundi anguli summi sinistri
 * @fundus:      index scriptus ubi locus non tegit
 * @indices:     latitudo * altitudo, indices palettae
 * @profunditas: latitudo * altitudo aut NIHIL; ubi locus non tegit
 *               MUNDUS_PROFUNDITAS_VACUA
 *
 * Regiones quas rectangulum tangit et quae nondum factae sunt nunc
 * fiunt (semel). Regio tota extra fines loci numquam fit.
 *
 * Redde: FALSUM si h aut indices NIHIL aut piscina deficit
 */
b32
mundus_humus_copiare (
    MundusHumus* h,
            s32  origo_x,
            s32  origo_y,
            i32  latitudo,
            i32  altitudo,
             i8  fundus,
             i8* indices,
            f32* profunditas);

/* regiones iam factae (probationibus et mensurae) */
i32
mundus_humus_regiones (
    constans MundusHumus* h);

#endif /* MUNDUS_PICTURA_H */
