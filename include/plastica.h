/*
 * plastica.h - plastica: descriptio textualis in statuam
 *
 * Res (puteus, saepes, saxum) describitur plagula STML brevi quae
 * solida simplicia enumerat; plastica_legere eam in Statuam finitam
 * (statua.h) vertit, eandem quam lector GLB dat. Omnia quae statuam
 * pingunt aut condunt eam sine mutatione accipiunt
 * (project-specs/plastica-spec.md IV.1).
 *
 * Nihil de mundo scit: nullum caput mundus_* hic includitur (spec
 * 9a). Nihil de plagulis aut pixelis.
 *
 * FORMA:
 *
 *   <res titulus="well">
 *     <grex locus="0 0 0" gyrus="0">
 *       <capsa titulus="murus" magnitudo="2 1 2" locus="0 0 0"
 *              color="#8a8f94" signum="solidum"/>
 *     </grex>
 *   </res>
 *
 * MENSURAE: unitas una = unitas mundi (persona II unitates alta); y
 * sursum; humus y = 0. Anguli in gradibus.
 *
 * locus="x y z"   centrum BASIS solidi (res in humo sedet);
 *                 ordinarius 0 0 0
 * gyrus="g"       circa axem erectum
 * rotatio="x y z" versio plena, post gyrum (rara)
 * color="#rrggbb" color planus (aut brevis "#rgb"); ordinarius
 *                 albus
 * signum          "solidum" aut "gressibile"; absens = nullum
 *                 (pars ornamenti: gressum non impedit)
 * invisibilis     "verum": pars non pingitur sed signum tenet
 *                 (obex invisus rei tenuis; spec 14a) - materia
 *                 alpha 0, limen excisionis 0.5
 * titulus         nomen partis (in tabula 'partes')
 *
 * SOLIDA (omnia: titulus, locus, gyrus, rotatio, color, signum,
 * invisibilis; basis in y = 0 spatii sui):
 *
 *   capsa      magnitudo="x y z"
 *   cylindrus  radius, altitudo; radius_summus (ordinarius = radius;
 *              0 = conus); latera (ordinarius VIII; III .. LXIV)
 *   sphaera    radius AUT magnitudo="x y z" (ellipsoides: saxa,
 *              comae arborum); segmenta circum (ordinarius VI;
 *              III .. XXXII; zonae = segmenta / II, minimum II);
 *              asperitas 0 .. 1 cum semine (semen="n"): radius
 *              cuiusque verticis variatur - vertices communes
 *              valorem eundem tenent, rimae nullae
 *   cuneus     magnitudo="x y z": altitudo plena ad marginem -x,
 *              ad nihilum cadit ad +x (tecta, clivi)
 *   extrusio   puncta="x,z x,z ..." (III .. LXIV), altitudo:
 *              lineamentum in plano humi sursum extrusum. Concavum
 *              licet; ordo punctorum uterque (invertitur si opus);
 *              lineamentum se secans recusatur
 *   tornus     puncta="r,y r,y ..." (II .. LXIV), latera: profilum
 *              circa axem erectum tornatum. r >= 0. Superficies ad
 *              DEXTRAM itineris spectat (r dextrorsum, y sursum):
 *              sursum eundo extrorsum, deorsum eundo introrsum -
 *              sic paries interior situlae fit. Fines non
 *              clauduntur nisi profilum ad r = 0 incipit aut desinit
 *
 * GREGES (omnes: titulus, locus, gyrus, rotatio):
 *
 *   grex       transformatio sola
 *   speculum   axis="x" aut "z": liberi ET imago eorum trans planum
 *              ubi coordinata illa 0 est. Imago vera est (vertices
 *              reflexi, ordo verticum inversus), non scala negativa
 *   series     numerus="n" (I .. CCLVI), gressus="x y z",
 *              gyrus_gressus="g": liberi n vicibus, exemplar k per
 *              k gressus motum et k gyros versum
 *   sparsio    numerus="n" (I .. CCLVI), semen="n", area="x z":
 *              liberi n vicibus intra aream circa centrum sparsi;
 *              gyrus_varius="verum" quodque fortuito vertit;
 *              magnitudo_varia="v" (0 .. 1) quodque scala inter
 *              1 - v et 1 + v mutat
 *
 * Fortuita ex semine solo (generator integer proprius): semen idem,
 * res eadem in omni machina.
 *
 * RECUSAT praeterea: latera aut segmenta extra fines; profilum aut
 * lineamentum cum punctis nimis paucis, puncto iterato aut r
 * negativo; numerum 0; rem cuius nodi MMMMXCVI aut triangula CC
 * milia excedunt (repetitores nidificati).
 *
 * Solidum unum = nodus unus cum reti uno; normales per faciem
 * (umbratio plana); facies extrorsum spectant.
 *
 * RECUSAT, causa verbis planis Anglicis et linea plagulae: textum
 * qui STML non est; radicem aliam quam <res>; elementum aut
 * attributum ignotum; magnitudinem absentem, non numericam, nullam
 * aut negativam; signum ignotum; rem sine solidis. Statua recusata
 * NIHIL est - numquam dimidia.
 *
 * DETERMINATA: textus idem statuam eandem dat, octetum pro octeto.
 */

#ifndef PLASTICA_H
#define PLASTICA_H

/* <aedilis corpus="lib/plastica.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "statua.h"

nomen structura {
       b32  successus;
    Statua* statua;   /* finita (statua_finire); NIHIL si recusata */
    chorda* partes;   /* statua->nodorum tituli, ordine nodorum;
                       * vacuus ubi nodus titulum non habet; NIHIL si
                       * recusata. Extra Statuam manet (spec 14b) */
    chorda error;    /* causa recusationis, Anglice, sine linea
                       * nova; vacua si successus */
       i32 linea;    /* linea plagulae (ab I); 0 si ignota aut
                       * successus */
} PlasticaFructus;

/*
 * plastica_legere - descriptionem in statuam vertere
 *
 * @textus:  plagula tota (non mutatur; statua nihil in ea tenet)
 * @piscina: statua, tabula partium et nuntius erroris
 *
 * Redde: successus VERUM cum statua finita et partibus, aut FALSUM
 * cum errore et linea
 */
PlasticaFructus
plastica_legere (
     chorda  textus,
    Piscina* piscina);

#endif /* PLASTICA_H */
