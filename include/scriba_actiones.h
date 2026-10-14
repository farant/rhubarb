/* scriba_actiones.h - tractator paginae scribae (scriba-plan S1c)
 *
 * Una actio, "pagina.clavis": claves et textum commissum in vim
 * vertit super FOLIUM LABORIS (gestus applicationis in Motus).
 * Status vim inter claves in insula (cursor, modus,
 * selectio, clavis praecedens, fd, capsa); documentum in cauda
 * (scriba_documentum) - per effusionem gestus solam scribitur.
 *
 * INGRESSUS: characteres imprimibiles EX TEXTU COMMISSO solo (fontes
 * veri etiam DEPRESSUS pro eis mittunt - ignoratur, aliter littera
 * bis); claves nominatae (Esc, Enter, Tab, Backspace, Delete,
 * sagittae, Home, End), Ctrl-R et Ctrl-[ (= Esc) ex DEPRESSUS.
 * Octeti >= 0x80 v1 ignorantur (folium octetorum). Ictus sinister
 * (S3a) cursorem in cellulam ictam ponit: gestus effunditur et
 * insertio clauditur (unitas revocandi nova), modus inserendi manet,
 * visualis ad normalem redit.
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
#include "iussum.h"

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
    /* S3b: iussa ('$verbum'); NIHIL = nulla (ictus cursorem solum
     * ponit). Hospes post montationem ponit (ut liber); verba nota
     * colorantur, ictus in iussum id currit */
      IussumRegistrum* iussa;
    /* S7b: ramus registri vim (capsa, capsa_lineae); repo NIHIL =
     * ramus proprius (scriba sola). Hospes post montationem ponit, ut
     * liber et iussa (vicus: radix - registrum unum applicationis) */
          InsulaRamus ramus_capsae;
    /* $question (2026-10-13): iussum DILATUM (effectus.differre) -
     * 0 nullum, I armatum (pulsus proximus picturam sinit), II
     * currendum (pulsus sequens verbum iterum currit) */
                  s32 dilatum;
               Iussum iussum_dilatum;   /* locus et verbum ictus */
    /* opus currens (quaestio-rivus III, 2026-10-14): NIHIL nullum;
     * dum currit pagina CLAUSA (Esc solum), folium = basis + textus */
           IussumOpus* opus;
               Iussum  iussum_operis;     /* locus (relocatum) */
               chorda  caput_operis;      /* in fine applicatur */
                  b32  consumit_operis;
                   i8* basis_cellulae;    /* folium ante opus */
                  s32* basis_indentatio;
              Piscina* piscina_operis;    /* pulsus: vacatur */
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

/* iussum notum sub puncto schirmi in pagina (componens 'pagina'
 * compositus)? FALSUM sine iussis. Hospes ictum primum (latus nondum
 * focatum) ita iudicat: iussum uno ictu currit (vicus-latera,
 * Franus) */
b32
scriba_iussum_ad_punctum (
            ScribaActiones* sa,
        constans Componens* pagina,
                   Punctum  schirmi);

/* pulsus (scriba_reficere): iussum dilatum (si quod) - gradus I ->
 * II (pictura nuntii prius), gradus II: iussum in folio laboris
 * relocatur (linea, initium, verbum idem; aliter nuntius
 * "<verbum>: evanuit"), verbum iterum currit (iussum->iterum), effectus
 * (caput, textus cum '\n', nuntius) in folium, commissio UNA. Redde
 * VERUM si folium aut status mutatum (pictura repetenda) */
b32
scriba_iussum_dilatum_currere (
    ScribaActiones* sa);

/* pulsus (scriba_reficere, ANTE iussum dilatum - pulsus initii nuntium
 * solum pingit): opus currens (si quod) pulsatur, folium = basis +
 * textus hactenus; FINITUM: caput + commissio UNA; IRRITUM: basis.
 * Redde VERUM si folium aut status mutatum */
b32
scriba_opus_pulsare (
    ScribaActiones* sa);

#endif /* SCRIBA_ACTIONES_H */
