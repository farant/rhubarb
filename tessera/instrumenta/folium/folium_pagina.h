/* folium_pagina.h - Involutio et paginatio textus (runae U7)
 *
 * Pars PURA instrumenti folium (spectator corporis Lapidis): textum ad
 * latitudinem involvere, in paginas partiri, paginam in TesseraOpus
 * pingere. Nulla tty, nullus discus - probationes per pontem memoriae.
 *
 * INVOLUTIO NAIVA CONSULTO (non bibliotheca; UAX #14 et lexicon Thai
 * lapides proximi cum quid eos traxerit): paragraphus = linea
 * fasciculi (\n; \r caudae tonsa); ambulatio per runae_unitas_proxima;
 * RUPTURA licet post spatium (U+0020) et inter unitates ubi alterutra
 * NATURA lata (runa prima II: CJK, emoji - non graphema per signum
 * spatians amplificatum, ne verba Hindi intra se frangantur); sine
 * ruptura licita linea ad limitem ipsum frangitur
 * (Thai, verba longa). Unitas latior quam latitudo tota lineam suam
 * solam tenet. Paragraphus vacuus = linea vacua.
 *
 * Extra fontes/ et amalgama consulto: tessera = cratis + initus, in
 * aeternum; dispositio bibliothecae futurae separatae est.
 */

#ifndef FOLIUM_PAGINA_H
#define FOLIUM_PAGINA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "runae.h"
#include "tessera_opus.h"

/* Linea involuta: octeti [initium, finis) textus, latitudo in
 * columnis (spatium caudae inclusum) */
nomen structura {
    i32 initium;
    i32 finis;
    i32 latitudo;
} FoliumLinea;

nomen structura {
    FoliumLinea* lineae;
            i32  numerus;
            i32  latitudo;    /* involutionis */
            /* statisticae textus totius */
            i32 unitates;    /* unitates pingendae */
            i32 columnae;    /* summa latitudinum */
            i32 latae;       /* unitates latitudinis II */
            i32 nullae;      /* unitates latitudinis 0 */
} FoliumInvolutio;

/* Textum ad latitudinem (>= I) involvere; lineae in piscina. FALSUM
 * si allocatio fracta aut latitudo nulla. */
b32
folium_involvere (
             Piscina* piscina,
              chorda  textus,
                 i32  latitudo,
       RunaePolitica  politica,
     FoliumInvolutio* involutio);

/* Lineas [prima, prima + altitudo) ad ordines 0.. pingere (x ab 0);
 * octeti imperii -> ' ' (tabula inclusa). */
vacuum
folium_paginam_pingere (
                 TesseraOpus* opus,
                      chorda  textus,
    constans FoliumInvolutio* involutio,
                         i32  prima,
                         i32  altitudo,
               TesseraStilus  stilus);

#endif /* FOLIUM_PAGINA_H */
