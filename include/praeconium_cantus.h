/* praeconium_cantus.h - eventus 'cantus' libri praeconii (praeco)
 *
 * Agens (Claude) cantilenam ad vicum mittit per librum praeconii, quem
 * vicus iam sequitur: praeco eventum 'cantus' scribit, vicus eum canit
 * (vicus_applicatio: recentissimus, nisi mutus). Seorsum a praeconium.h
 * ne omnes lectores libri (colloquia, admonitiones) cantuale et cantus
 * includant.
 *
 * IUDICIUM ANTE SCRIPTURAM: cantilena fracta numquam scribitur - agens
 * vitium cum offset accipit (praeco caret sub textu), non silentium in
 * vico.
 */
/* <aedilis corpus="lib/praeconium_cantus.c"/> */
#ifndef PRAECONIUM_CANTUS_H
#define PRAECONIUM_CANTUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "praeconium.h"
#include "cantuale.h"

/* cantilenam iudicat (cantionem_legere; <= PRAECONIUM_NUNTIUS_MAXIMUS
 * octeti - liber praecidit, cantilena praecisa mentiretur) et eventum
 * implet: eventus "cantus", genus "intactus" aut "", nuntius = textus.
 * Ceteri campi (tempus, panellus, servitor, via) vocantis. FALSUM:
 * vitium (offset in textu), e intactum */
b32
praeconium_cantum_parare (
                chorda  textus,
                   b32  intactus,
     PraeconiumEventus* e,
               Piscina* piscina,
        CantualeVitium* vitium);

#endif /* PRAECONIUM_CANTUS_H */
