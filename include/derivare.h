/* derivare.h - Derivatio eventuum PURA ex fluxu crudo
 *
 * Duplex ictus ex TEMPORE eventuum (numquam ex horologio), gradus
 * primus. Tractus (eventus A5; spec D4): pressio + motus ULTRA
 * 'distantia' pixela (in axe utrovis) -> TRACTUS_INCIPIT (x/y =
 * origo pressionis), deinde TRACTUS per motum, TRACTUS_FINIT in
 * liberatione. Tractus ictum primum duplicis NON parat.
 * Hover/focus derivantur in dispensatore post destinationem
 * (id geometricum poscunt). Independens a strategia destinationis.
 *
 * USUS:
 *   Derivator d; derivator_initiare(&d, CCC, IV);
 *   derivare(&d, &eventus, effusio);   effusio = Xar de Eventus
 */

#ifndef DERIVARE_H
#define DERIVARE_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

/* <aedilis corpus="lib/derivare.c"/> */

#include "latina.h"
#include "xar.h"
#include "eventus.h"
#include "mandatum.h"   /* Punctum */


/* ==================================================
 * TYPI
 * ================================================== */

nomen structura {
        s64 tempus_ultimi;
    Punctum ultimus;
        b32 habet_ultimum;
        s64 intervallum_ms;
        i32 distantia;
    /* tractus (A5) */
         b32 armatus;    /* pressio sine liberatione */
         b32 trahens;    /* limen transitum */
     Punctum origo;
mus_botton_t botton;
} Derivator;


/* ==================================================
 * FUNCTIONES
 * ================================================== */

vacuum
derivator_initiare (
    Derivator* d,
          s64  intervallum_ms,
          i32  distantia);

/* Appendit eventum crudum, deinde derivata (DUPLEX, TRACTUS_*) ad
 * effusionem (Xar de Eventus). */
vacuum
derivare (
           Derivator* d,
    constans Eventus* ev,
                 Xar* effusio);

#endif /* DERIVARE_H */
