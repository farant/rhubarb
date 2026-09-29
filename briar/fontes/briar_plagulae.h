/* briar_plagulae.h - Membra aedificationis: plagulae per elementum
 * bibliothecae adductae (spec par. 3.5, v1.11; lapide
 * feature-requests/015). Elementum STML 'bibliotheca' cum attributo
 * 'via' (relativa contra directorium plagulae IMPORTANTIS, numquam
 * contra directorium currens; absoluta ut data) plagulam aliam
 * nominat; eius regiones C PLANAE (non fragmentum, non munus
 * "probatio", sine methodus) importanti praebentur.
 *
 * Collectio TRANSITIVA: elementa membrorum quoque sequuntur; membra
 * ordine POST-ORDINIS redduntur (folia primum), quodque SEMEL (via
 * absoluta normalizata = identitas); radix ipsa membrum non est.
 * Circulus refutatur - politia, non mechanica: typi per capita genita
 * inter se inclusa non compilarent (spec par. 3.5, nota B6).
 *
 * Refutationes (causa + via plagulae importantis + linea elementi):
 * via absens, suffixum non '.thistle', plagula non exsistens, circulus
 * (catena nominata cum remedio), membrum sine regione C plana,
 * regio membri non parsata (causa silvae eius).
 */

#ifndef BRIAR_PLAGULAE_H
#define BRIAR_PLAGULAE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "materia_nodus.h"
#include "silex.h"

/* membrum aedificationis */
nomen structura {
     constans character* via;              /* absoluta, normalizata */
     constans character* titulus;          /* basis sine ".thistle" */
                 chorda  octeti;           /* plagula tota (clavis) */
           MateriaNodus* documentum;
                    Xar* nexus;             /* BriarNexusRes, silva
                                             * texta */
     constans character* via_importantis;  /* primus qui adduxit */
                    i32  linea_elementi;
} BriarMembrum;

/* refutatio collectionis: causa + ubi (plagula et linea elementi) */
nomen structura {
                 chorda  causa;
     constans character* via;
                    i32  linea;
} BriarMembraCausa;

/* Membra radicis colligere (Xar de BriarMembrum in *membra, ordine
 * post-ordinis). via_radicis relativa contra directorium currens
 * absoluta fit. FALSUM: *causa posita, *membra NIHIL. */
b32
briar_membra_colligere (
                 Piscina*  piscina,
      constans character*  via_radicis,
                     Xar*  nexus_radicis,
     InternamentumChorda*  intern,
      constans SilexFons*  fons,
                     Xar** membra,
        BriarMembraCausa*  causa);

#endif /* BRIAR_PLAGULAE_H */
