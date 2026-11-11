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
 *
 * QUADRUM (spec IV.6): humus copiata; deinde corpus quodque in visu
 * positum, SOLUM in capsam suam parvam pictum, pixelis post
 * profunditatem quadri iacentibus demptis, exemplariis sub ea larva
 * redactum (statuae_picturam_dithering) et superpositum. Profunditas
 * quadri etiam corpora accipit: propius vincit, ordo corporum nihil
 * mutat. Corpus 'sequens' in medio quadri stat.
 */

#ifndef MUNDUS_PICTURA_H
#define MUNDUS_PICTURA_H

/* <aedilis corpus="lib/mundus_pictura.c"/> */

#include "latina.h"
#include "piscina.h"
#include "statua.h"
#include "statua_pictura.h"
#include "mundus.h"

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


/* ==================================================
 * Corpora et quadrum
 * ================================================== */

/* corpus latius quam hoc (unitates) minuitur etiam si altitudo II non
 * attingitur (aranea lata et humilis) */
#define MUNDUS_CORPORIS_LATITUDO_MAXIMA 3.0f
/* capsa corporis maxima (pixela per latus); maior praeciditur */
#define MUNDUS_CAPSA_MAXIMA             DXII

/* quomodo statua in mundo adhibetur */
nomen structura {
    constans Statua* statua;
                s32  cyclus_ambulandi;  /* -1 = nullus (quies) */
                s32  cyclus_quietis;    /* -1 = positura quietis */
                f32  scala;             /* ad altitudinem II (aut
                                         * latitudinem maximam) */
                f32 imum;              /* y infimum positurae quietis
                                         * (pedes), in spatio statuae */
                f32 medium_x;          /* medium capsae quietis */
                f32 medium_z;
} MundusForma;

/*
 * mundus_forma - formam ex statua derivare
 *
 * cyclus_ambulandi: primus cuius titulus "walk" continet; aliter
 * primus; aliter -1. cyclus_quietis: primus cuius titulus "idle"
 * continet; aliter -1. scala: capsa positurae quietis ad altitudinem
 * MUNDUS_CORPORIS_ALTITUDO, nisi latitudo tunc MAXIMAM excedat.
 * Statua NIHIL aut sine verticibus: scala I, cycli -1.
 *
 * @piscina: memoria temporaria (matrices quietis); NIHIL: capsa per
 *           sphaeram ambientem aestimatur
 */
MundusForma
mundus_forma (
    constans Statua* statua,
            Piscina* piscina);

/*
 * mundus_quadri_origo - origo quadri (pixela mundi) ut medium corporis
 * 'sequens' in medio quadri stet
 *
 * Redde: FALSUM si argumenta NIHIL aut sequens extra fines
 */
b32
mundus_quadri_origo (
    constans Mundus* mundus,
                i32  sequens,
                i32  latitudo,
                i32  altitudo,
                s32* origo_x,
                s32* origo_y);

/*
 * mundus_quadrum_pingere - quadrum: humus + corpora
 *
 * @formae:   corpus c formam formae[c->forma] habet; forma extra
 *            fines aut sine statua: corpus non pingitur
 * @sequens:  index corporis in medio
 * @fundus:   index palettae extra locum
 * @indices:  latitudo * altitudo, a vocante praebiti
 * @piscina:  memoria temporaria quadri
 *
 * Positura corporis: cyclus_ambulandi ad tempus_motus dum movetur;
 * aliter cyclus_quietis ad tempus 0 (aut quies).
 *
 * Redde: FALSUM si argumenta NIHIL, sequens extra fines aut memoria
 * deficit; latitudo aut altitudo 0: VERUM, nihil scribitur
 */
b32
mundus_quadrum_pingere (
             MundusHumus* humus,
         constans Mundus* mundus,
    constans MundusForma* formae,
                     i32  formarum,
                     i32  sequens,
            constans b32* activi,
                      i8  fundus,
                     i32  latitudo,
                     i32  altitudo,
                      i8* indices,
                 Piscina* piscina);

#endif /* MUNDUS_PICTURA_H */
