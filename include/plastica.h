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
 * ELEMENTA huius gradus: res (radix, titulus requisitus), grex
 * (transformatio sola), capsa (magnitudo="x y z"). Cetera solida et
 * repetitores in gradu sequenti (plastica-plan T2).
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
