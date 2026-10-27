/* communio.h - acta et massae inter volumina (project-specs/
 * communio-spec.md; plan communio-plan-1 T3)
 *
 * Gradus I sine rete: acta PROPRIA voluminis fontis post cursorem
 * (volumen_acta_propria_legere) in destinationem importantur
 * (volumen_actum_importare) identitate servata; massae quas actum
 * nominat PRIUS mittuntur. Derivata (plagulae checkpoint) domi
 * manent. Idem exportatio/importatio quam protocollum retis (gradus
 * II) portabit.
 */

#ifndef COMMUNIO_H
#define COMMUNIO_H

/* <aedilis corpus="lib/communio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "volumen.h"

/* relatio transitus unius */
nomen structura {
    s64 cursor;        /* = *cursor post transitum */
    i32 importata;     /* VOLUMEN_IMPORTATUM */
    i32 iam_aderant;   /* VOLUMEN_IAM_ADEST */
    i32 massae;        /* massae in destinationem missae */
    i32 refusa;        /* 0 aut I: transitus ad primum refusum sistit */
} CommunioRelatio;

/* acta propria fontis post *cursor (seq locale fontis) in
 * destinationem: derivata praetermittuntur (cursor ea praeterit);
 * massae nominatae prius (quaeque series LXIV characterum hex
 * minusculorum in datum quae massam fontis nominat - regula generalis,
 * nullius applicationis forma); deinde importatio ordine. *cursor
 * solum post acta accepta (importata aut iam praesentia) progreditur;
 * ad primum refusum transitus sistit (cursor ante id) et FALSUM
 * redditur. relatio NIHIL licet. */
b32
communio_transferre (
            Volumen* fons,
            Volumen* destinatio,
                s64* cursor,
    CommunioRelatio* relatio,
            Piscina* piscina);

#endif /* COMMUNIO_H */
