/* gesta_expeditio.h - expeditiones: opera per inventarium (PURUM)
 *
 * Expeditio = opus per ordines inventarii factum: PHOTOGRAPHIA ordinum
 * (per lentem forte filtrata) + RUBRICA scripta (quid cuique ordini
 * faciendum). Status in statu plicato rei vivit, ex quinque generibus
 * eventuum aedificatus:
 *   expeditio-photographia {inventarium, ordines: [clavis...], rubrica,
 *                           filtrum?, implenda?}   (semel, in ortu)
 *   rubrica-mutata         {textus}                     (versio + I)
 *   gradus-positus         {ordines: [...], status, per?, nota?}
 *   gradus-promotus        {ordo, opus}
 *   ordines-additi         {ordines: [...]}
 * status gradus: factum | omissum (nota necessaria) | apertum.
 * filtrum = {lens, op: "=" | "!=", valor}; implenda = {lens, genus,
 * valor} (quod 'factum' in inventario scribit - instrumentum id facit).
 *
 * Status plicatus:
 *   inventarium, filtrum?, implenda?
 *   rubrica:  {textus, versio}
 *   ordines:  [{clavis, additus}]
 *   gradus:   {clavis: {status, versio, actor, creatum, per?, nota?,
 *              opus?}}
 * Quisque gradus versionem rubricae CURRENTEM servat (decretum
 * ...D59G3Z): 'ordines sub rubrica vetere facti' quaestio fit.
 *
 * Plicatura DEFENSIVA (replay numquam fallit: ordines ignoti et
 * photographia secunda praetermittuntur); validatio STRICTA ad
 * scripturam, causae OMNES simul. Genus rei (expeditio) hic non
 * videtur - vocans id probat. Spec: project-specs/expeditio-spec.md.
 */
/* <aedilis corpus="gesta/fontes/gesta_expeditio.c"/> */
#ifndef GESTA_EXPEDITIO_H
#define GESTA_EXPEDITIO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "json.h"

/* VERUM = unum ex quinque generibus eventuum expeditionis */
b32
gesta_expeditio_eventus_est (
    chorda genus_eventus);

/* eventum in statum (obiectum JSON rei) plicare - PURUM, in loco.
 * actor/creatum = provenientia graduum. VERUM = status mutatus */
b32
gesta_expeditio_applicare (
     JsonValor* status,
        chorda  genus_eventus,
     JsonValor* datum,
        chorda  actor,
        chorda  creatum,
       Piscina* pn);

/* NIHIL = licet; aliter causae OMNES (una per lineam, "  - ...")
 * contra statum currentem */
constans character*
gesta_expeditio_validare (
     JsonValor* status,
        chorda  genus_eventus,
     JsonValor* datum,
       Piscina* pn);

#endif /* GESTA_EXPEDITIO_H */
