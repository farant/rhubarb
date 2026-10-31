/* scriba_figurae.h - figurae scribae (scriba-plan S2)
 *
 * <purus/>: figurae arborem (data paginae: puncta, titulus) et
 * contextum (folium laboris) legunt. Mensa (prospectus), folium
 * (campus: charta, margo, selectio, cursor, textus), status (titulus:
 * modus colore suo, positio), index paginae (filius status, dextrorsum:
 * "pagina i/n" - visus super librum solum, vicus-latera S2b),
 * nuntius (filius status, colore erroris - S3b-2).
 *
 * Cursor stabilis (sine nictatu: horologium unum - nictatus ex
 * tempore muri vetaretur); quadratum plenum colore status modi sui
 * (COLOR_STATUS_INSERT / _NORMAL - ut verbum modi in linea status;
 * COLOR_CURSOR = aurum inserendi in themate); character sub eo
 * colore chartae superpingitur. Selectio (visualis): lineae totae,
 * ut pagina vetus.
 *
 * TAGS CHRONICAE (chronica C6): in folio libri (sa->liber) <event>,
 * <person>, <place> per chronica_eventa_legere et
 * chronica_nomina_legere super lineas folii TOTIUS leguntur (eventa
 * per lineas fluunt) et colorantur:
 *   eventus bonus    tag aperiens ('<event ...>') et claudens
 *                    ('</event>' / '</>') colore accentus secundi (ut
 *                    #nexus); descriptio textu ordinario
 *   declaratio bona  regio tota colore accentus secundi
 *   vitium           ambitus totus colore erroris (annus deest, non
 *                    clausum, distributio mixta, ...)
 * Lineae selectae non colorantur (textus ibi colore chartae), ut
 * iussa et nexus. Cellula = octetus (visibilis: extra 32-126 spatium),
 * ergo columnae lectoris cellulae sunt. */

#ifndef SCRIBA_FIGURAE_H
#define SCRIBA_FIGURAE_H

/* <aedilis corpus="lib/scriba_figurae.c"/> */

#include "latina.h"
#include "figura.h"
#include "scriba_actiones.h"

nomen structura {
    ScribaActiones* sa;   /* folium laboris (gestus) */
} ScribaFigurae;

vacuum
scriba_figurae_registrare (
    FiguraRegistrum* reg,
                i32  thema,
      ScribaFigurae* ctx);

/* <purus/> */
vacuum
scriba_figura_mensae (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx);

/* <purus/> */
vacuum
scriba_figura_folii (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx);

/* <purus/> */
vacuum
scriba_figura_status (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx);

/* <purus/> index paginae (PARTES_INDEX, filius status): titulus textu
 * in fines suos; fundum status iam pinxit */
vacuum
scriba_figura_paginae (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx);

/* <purus/> nuntius (PARTES_DIALOGUS, filius status): titulus colore
 * erroris in fines suos; fundum status iam pinxit */
vacuum
scriba_figura_nuntii (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx);

#endif /* SCRIBA_FIGURAE_H */
