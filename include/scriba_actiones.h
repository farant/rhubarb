/* scriba_actiones.h - tractator paginae scribae (scriba-plan S1c)
 *
 * Una actio, "pagina.clavis": claves et textum commissum in vim
 * vertit super FOLIUM LABORIS (gestus applicationis in Motus).
 * Status vim inter claves in insula ephemerarum (cursor, modus,
 * selectio, clavis praecedens, fd, capsa); documentum in cauda
 * (scriba_documentum) - per effusionem gestus solam scribitur.
 *
 * INGRESSUS: characteres imprimibiles EX TEXTU COMMISSO solo (fontes
 * veri etiam DEPRESSUS pro eis mittunt - ignoratur, aliter littera
 * bis); claves nominatae (Esc, Enter, Tab, Backspace, Delete,
 * sagittae, Home, End) et Ctrl-R ex DEPRESSUS. Octeti >= 0x80 v1
 * ignorantur (folium octetorum). Ictus sinister (S3a) cursorem in
 * cellulam ictam ponit: gestus effunditur et insertio clauditur
 * (unitas revocandi nova), modus inserendi manet, visualis ad
 * normalem redit.
 *
 * SERVATIO (Franus): modus normalis/visualis - mutatio clave finita,
 * statim effunditur; inserere - post SCRIBA_QUIES_MS sine clave
 * (dispensator); Esc effundit et insertionem claudit. Insertionis
 * frustum primum actum simplex, cetera CONIUNCTA (historia S1b): 'u'
 * insertionem totam revocat. 'u' / Ctrl-R in modo normali ad
 * historiam, non ad vim; folium laboris ex proiectione reficitur.
 * Horologium unum: fd ex Eventus.tempus.
 *
 * VALORES: nullus vacuus scribitur - absentia = vacuum (scriptor
 * pulcher a="" ut a nudum scribit, relectum "true"). */

#ifndef SCRIBA_ACTIONES_H
#define SCRIBA_ACTIONES_H

/* <aedilis corpus="lib/scriba_actiones.c"/> */

#include "latina.h"
#include "piscina.h"
#include "actio.h"
#include "motus.h"
#include "tabula_characterum.h"
#include "scriba_documentum.h"
#include "scriba_liber.h"

#define SCRIBA_QUIES_MS M   /* insertio in cursu servatur post ~1 s */

nomen structura {
     ScribaDocumentum* doc;
    TabulaCharacterum  laboris;    /* folium laboris (gestus) */
                  b32  inserere;   /* modus post clavem ultimam */
                  b32  insertio_commissa;  /* frustum iam commissum */
    /* R4: status scribae; repo NIHIL = radix repositorii tractatori
     * dati (scriba_actiones_initiare nullat) */
          InsulaRamus ramus;
    /* S2b (vicus-latera): versio documenti (cursor) ex qua folium
     * laboris est - aliter = visus stalus (alius visus commisit) */
                  s64 cursor_laboris;
    /* S2b: liber paginarum; NIHIL = documentum proprium (mos prior,
     * sine navigatione). Ctrl+Shift+Sinister/Dexter paginam priorem/
     * proximam (ultra ultimam nova) - cursor, modus pagina.clavis
     * sunt, ergo hic */
          ScribaLiber* liber;
    /* S2b: clavis plagulae paginae visus ('scriba/visus/<id>') */
               chorda visus;
} ScribaActiones;

/* folium laboris = copia proiectionis documenti */
vacuum
scriba_actiones_initiare (
      ScribaActiones* sa,
    ScribaDocumentum* doc,
             Piscina* piscina);

vacuum
scriba_actiones_registrare (
    ActioRegistrum* reg,
    ScribaActiones* sa);

/* gestum (folium laboris) Motui addicere, quies SCRIBA_QUIES_MS */
vacuum
scriba_gestum_ponere (
             Motus* motus,
    ScribaActiones* sa);

/* <tractator/> */
b32
scriba_pagina_clavis (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

#endif /* SCRIBA_ACTIONES_H */
