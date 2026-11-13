/*
 * statua_iunctio.h - statuas in unam iungere
 *
 * Statua basis (locus) et statuae addendae (res: puteus, saepes),
 * quaeque in loco suo, statuam UNAM novam dant quam omnia quae
 * statuam legunt sine mutatione accipiunt: tabula gressus mundi,
 * pictor, scriptor GLB (project-specs/plastica-spec.md IV.5).
 *
 * BASIS prima venit et intacta manet: nodi, retia, tractus, vertices,
 * materiae, texturae et cycli eius indices suos servant - cyclus
 * basis eosdem nodos movet. Nihil additum: statua basi par.
 *
 * ADDENDA quaeque sub nodo radicali NOVO pendet qui locum et gyrum
  * eius et scalam fert (parens -1, sine reti, sine signis). Nodi eius
 * sequuntur, indicibus retium, materiarum et texturarum motis. Signa
 * nodorum manent. Gyrus: gradus circa axem erectum, sensu eodem quo
 * 'gyrus' descriptionis plasticae.
 *
 * CYCLI addendarum omittuntur; cycli basis manent.
 *
 * SEMEL TENETUR: statua eadem (index idem) pluries addita retia,
 * tractus, vertices, materias et texturas semel fert - nodi soli
 * iterantur. Textura pixelis par texturae iam praesenti (basis aut
 * addendae prioris) non iteratur: materia ad priorem spectat.
 * Texturae basis inter se non conferuntur (indices eius manent).
 *
 * MEMORIA: statua nova tabulas proprias in piscina tenet, sed pixela
 * texturarum et claves cyclorum ex statuis datis MUTUATUR: illae
 * diutius vivant.
 */

#ifndef STATUA_IUNCTIO_H
#define STATUA_IUNCTIO_H

/* <aedilis corpus="lib/statua_iunctio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "stereometria.h"
#include "statua.h"

/* statua addenda et ubi stet */
nomen structura {
       constans Statua* statua;
    StereometriaVector  locus;    /* ubi origo eius stat */
                   f32  gyrus;    /* gradus circa axem erectum */
                   f32  scala;    /* maior quam 0; I = magnitudo
                                   * propria. Res tota circa originem
                                   * suam crescit */
} StatuaIungenda;

/*
 * statua_coniungere - basim et addendas in statuam novam iungere
 *
 * @basis:    finita
 * @addendae: numerus statuae finitae cum locis; NIHIL licet si
 *            numerus 0
 * @piscina:  statua nova et tabulae eius
 *
 * Redde: statua nova finita (statua_finire); NIHIL si basis aut
 * piscina NIHIL, addendae NIHIL cum numero non 0, statua addenda
 * NIHIL, locus, gyrus aut scala non finita, scala non positiva,
 * memoria deficit aut
 * statua_finire recusat
 */
Statua*
statua_coniungere (
            constans Statua* basis,
    constans StatuaIungenda* addendae,
                        i32  numerus,
                    Piscina* piscina);

#endif /* STATUA_IUNCTIO_H */
