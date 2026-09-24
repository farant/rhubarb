/* gesta_inventarium.h - inventaria: tabula ordinum x lentium (PURUM)
 *
 * Inventarium = tabula: ORDINES (membra) x LENTES (columnae). Tabula in
 * statu plicato rei vivit, ex quattuor generibus eventuum aedificata:
 *   ordo-additus   {ordines: [clavis...]}
 *   ordo-remotus   {ordines: [clavis...], causa}
 *   lens-addita    {nomen, genus_valoris, corpus?}
 *   cella-posita   {cellae: [{ordo, lens, valor}...], fons, per?}
 * Valor semper TAGATUS: {genus: "ita-non", valor: ita|non|ignotum} aut
 * {genus: "textus", valor: "..."} aut, in OMNI lente, {genus:
 * "non-applicabile", valor?: causa} - cella expresse vacua quae ut
 * impleta numeratur (decretum ...SD7JR). Provenientia cellae: actor et
 * creatum eventus + fons (manu | derivatum) + per? (opus/commissio).
 *
 * Status plicatus:
 *   ordines: [{clavis, additus, actor}]      (ordine additionis)
 *   lentes:  [{nomen, genus_valoris, corpus?}]
 *   cellae:  {clavis: {lens: {genus, valor, actor, creatum, fons,
 *             per?}}}
 *
 * Plicatura DEFENSIVA (replay numquam fallit: duplicata et cellae sine
 * ordine praetermittuntur); validatio STRICTA ad scripturam, causae
 * OMNES simul. Genus rei (inventarium) hic non videtur - vocans id
 * probat. Spec: project-specs/inventarium-spec.md.
 */
/* <aedilis corpus="gesta/fontes/gesta_inventarium.c"/> */
#ifndef GESTA_INVENTARIUM_H
#define GESTA_INVENTARIUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "json.h"

/* VERUM = unum ex quattuor generibus eventuum inventarii */
b32
gesta_inventarium_eventus_est (
    chorda genus_eventus);

/* eventum in statum (obiectum JSON rei) plicare - PURUM, in loco.
 * actor/creatum = provenientia cellarum. VERUM = status mutatus */
b32
gesta_inventarium_applicare (
     JsonValor* status,
        chorda  genus_eventus,
     JsonValor* datum,
        chorda  actor,
        chorda  creatum,
       Piscina* pn);

/* NIHIL = licet; aliter causae OMNES (una per lineam, "  - ...")
 * contra statum currentem */
constans character*
gesta_inventarium_validare (
     JsonValor* status,
        chorda  genus_eventus,
     JsonValor* datum,
       Piscina* pn);

#endif /* GESTA_INVENTARIUM_H */
