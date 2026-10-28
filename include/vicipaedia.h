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
#include "xar.h"

Articulus*
vicipaedia_articulus (
                 chorda  html,
     constans character* fons,
                Piscina* piscina);

/* sedes articuli Vicipaediae: hospes + titulus (+ ancora). titulus
 * DECODICATUS (UTF-8, '_' pro spatio: "Rheum_(plant)", "C++",
 * "AC/DC") - identitas articuli est (hospes, titulus). */
nomen structura {
    chorda hospes;  /* "en.wikipedia.org" (mobilis "en.m." -> "en.") */
    chorda titulus;
    chorda ancora;  /* "Etymology" ex #...; vacua si nulla */
} VicipaediaSedes;

/* URL -> sedes. Accipit https://<lingua>[.m].wikipedia.org/wiki/<T>,
 * /api/rest_v1/page/html/<T> (fons noster) et /w/rest.php/v1/page/
 * <T>/html (quo rest_v1 redirectiones mittit), quaestione neglecta.
 * Spatia -> '_'. FALSUM si hospes non <lingua>.wikipedia.org aut
 * titulus vacuus. */
b32
vicipaedia_sedem_legere (
               chorda  url,
      VicipaediaSedes* exitus,
              Piscina* piscina);

/* URL canonica articuli (sine ancora): https://<hospes>/wiki/<T
 * codificatus> - clavis articuli in bibliotheca (N2.4) et 'fons'
 * vicipaedia_articulus. */
chorda
vicipaedia_url_articuli (
    constans VicipaediaSedes* sedes,
                     Piscina* piscina);

/* URL fontis (REST Parsoid HTML): https://<hospes>/api/rest_v1/
 * page/html/<T codificatus> ('/' tituli -> %2F) */
chorda
vicipaedia_url_fontis (
    constans VicipaediaSedes* sedes,
                     Piscina* piscina);

/* quaestio (REST /w/rest.php/v1/search/page): eventus unus */
nomen structura {
    chorda titulus;      /* "Thomas Aquinas" (ostendendus) */
    chorda clavis;       /* "Thomas_Aquinas" (titulus URL) */
    chorda descriptio;   /* "Italian Dominican friar..."; vacua licet */
} VicipaediaEventus;

/* URL quaestionis: https://<hospes>/w/rest.php/v1/search/page?
 * q=<verba codificata>&limit=<limes> */
chorda
vicipaedia_url_quaestionis (
      chorda  hospes,
      chorda  verba,
         i32  limes,
     Piscina* piscina);

/* JSON responsi -> Xar de VicipaediaEventus (ordine relevantiae);
 * NIHIL si JSON malum; vacuum si nulli */
Xar*
vicipaedia_eventus_legere (
      chorda  json,
     Piscina* piscina);

#endif /* VICIPAEDIA_H */
