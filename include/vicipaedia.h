/* vicipaedia.h - adaptator Vicipaediae (navigator N1): HTML paginae
 * REST (/api/rest_v1/page/html/<Titulus>, Parsoid) -> Articulus.
 * PURUS: HTML datum, nullum rete. Sectiones per <section>; infobox
 * (table.infobox) -> adiunctum sectionis in qua stat (PAR per
 * lineam); nexus ./Titulus -> URL absoluta ex 'fons'; omittuntur:
 * notae referentiarum (sup.mw-ref), navbox, style/link, sectiones
 * References / Further reading / External links (v1). NIHIL solum
 * memoria deficiente. */

#ifndef VICIPAEDIA_H
#define VICIPAEDIA_H

/* <aedilis corpus="lib/vicipaedia.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "articulus.h"

Articulus*
vicipaedia_articulus (
                 chorda  html,
     constans character* fons,
                Piscina* piscina);

#endif /* VICIPAEDIA_H */
