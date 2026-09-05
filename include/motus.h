/* motus.h - Genus MOBILE: structura in memoria, effusa in quiete
 *
 * "Pictura per eventum, scriptura IN QUIETE" (laboratorium
 * 0007/0015). Scripturae per eventum HUC veniunt (vili); insula
 * ephemerarum solum in quiete tangitur (motus_effundere, sedes
 * <quies/>) aut a scriptura durabili in liberatione superatur.
 * Ictus pendens insulam ephemerarum NUMQUAM tangit - finitus per
 * scripturam durabilem exit.
 *
 * Lint L4: scripturae insulae camporum mobilium solum ex <quies/>.
 */

#ifndef MOTUS_H
#define MOTUS_H

/* <aedilis corpus="lib/motus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "mandatum.h"   /* Punctum */
#include "insula.h"


/* ==================================================
 * Typi
 * ================================================== */

nomen structura {
      chorda  captura;         /* id componentis; vacua = nulla */
         Xar* ictus_pendens;   /* Xar de Punctum */
     Punctum  pan;
         i32  zoom;
         s64  tempus_ultimae_mutationis;
         b32  sordida;
     Piscina* piscina;
} Motus;

nomen vacuum (*MotusMutator)(
    Motus* motus,
    vacuum* ctx);


/* ==================================================
 * Vita et porta
 * ================================================== */

vacuum
motus_initiare (
      Motus* motus,
    Piscina* piscina);

/* PORTA UNICA generis mobilis: mutator currit, tempus notatur,
 * sordida ponitur. Insulam non tangit. */
vacuum
mutare_motum (
            Motus* motus,
     MotusMutator  fn,
           vacuum* ctx,
              s64  tempus);

/* Quies: sordida ET quies_ms elapsa post mutationem ultimam. Nihil
 * sordidum = numquam quieta (nihil effundendum). */
b32
motus_quies (
    constans Motus* motus,
               s64  nunc,
               s64  quies_ms);

/* <quies/> - scriptura UNA camporum persistendorum (pan, zoom) in
 * insulam ephemerarum per mutare_ephemera; sordida tollitur si
 * porta accepit. */
b32
motus_effundere (
                 Motus* motus,
    InsulaRepositorium* repo);


/* ==================================================
 * Captura
 * ================================================== */

vacuum
motus_captura_ponere (
     Motus* motus,
    chorda  id);

vacuum
motus_captura_tollere (
    Motus* motus);

#endif /* MOTUS_H */
