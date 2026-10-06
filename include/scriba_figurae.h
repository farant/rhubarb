/* scriba_figurae.h - figurae scribae (scriba-plan S2)
 *
 * <purus/>: figurae arborem (data paginae: puncta, titulus) et
 * contextum (folium laboris) legunt. Mensa (prospectus), folium
 * (campus: charta, margo, selectio, cursor, textus), status (titulus:
 * modus colore suo, positio).
 *
 * Cursor stabilis (sine nictatu: horologium unum - nictatus ex
 * tempore muri vetaretur); quadratum plenum colore status modi sui
 * (COLOR_STATUS_INSERT / _NORMAL - ut verbum modi in linea status;
 * COLOR_CURSOR = aurum inserendi in themate); character sub eo
 * colore chartae superpingitur. Selectio (visualis): lineae totae,
 * ut pagina vetus. */

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

#endif /* SCRIBA_FIGURAE_H */
