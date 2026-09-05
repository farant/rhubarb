/* destinatio.h - Destinatio: STRATEGIA una, permutabilis,
 * componentibus ignota
 *
 * Ordinaria: ambulare arborem componentium ULTIMAM intime-primum,
 * summum-primum (ordo pictoris = ordo z), sectionem honorans,
 * translationem/scalam invertens. Captura (motus) vincit
 * geometriam pro eventibus muris positis; claves et rotula (sine
 * puncto in Eventu) ad focum eunt, deinde ascendunt. Componentia
 * destinationem NUMQUAM vocant (lint L9-gemellus: strategia
 * arborem logicam solam legit, numquam mandata).
 */

#ifndef DESTINATIO_H
#define DESTINATIO_H

/* <aedilis corpus="lib/destinatio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "fenestra.h"
#include "mandatum.h"
#include "componens.h"
#include "motus.h"


/* ==================================================
 * Typi
 * ================================================== */

nomen structura {
    /* vacua = nulla captura */
    chorda id_captum;
    /* vacua = nullus ictus / eventus sine puncto */
    chorda id_geometricum;
    /* Xar de Componens*: destinatum primum, radix ultima */
    Xar* ascensus;
    /* punctum in spatio destinati geometrici */
    Punctum punctum_locale;
} Destinatio;

nomen Destinatio (*DestinatioStrategia)(
          Componens* arbor,
    constans Motus* motus,
              chorda focus,
   constans Eventus* ev,
            Piscina* piscina);


/* ==================================================
 * Strategia ordinaria
 * ================================================== */

Destinatio
destinatio_geometrica (
           Componens* arbor,
      constans Motus* motus,
              chorda  focus,
    constans Eventus* ev,
             Piscina* piscina);

/* Componens destinatum: primus ascensus (captum si adest); NIHIL si
 * ascensus vacuus. */
Componens*
destinatio_componens (
    constans Destinatio* d);


/* ==================================================
 * Ordo foci
 * ================================================== */

/* Focusabiles ordine profunditatis; involvit. Focus ignotus aut
 * vacuus: proximum = primus, praecedens = ultimus. Vacua si nemo
 * focusabilis. */
chorda
destinatio_focus_proximum (
     Componens* arbor,
        chorda  focus,
       Piscina* piscina);

chorda
destinatio_focus_praecedens (
     Componens* arbor,
        chorda  focus,
       Piscina* piscina);

#endif /* DESTINATIO_H */
