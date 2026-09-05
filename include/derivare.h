/* derivare.h - Derivatio eventuum PURA ex fluxu crudo
 *
 * Duplex ictus ex TEMPORE eventuum (numquam ex horologio), gradus
 * primus. Hover/focus derivantur in dispensatore post destinationem
 * (id geometricum poscunt). Independens a strategia destinationis.
 *
 * USUS:
 *   Derivator d; derivator_initiare(&d, CCC, IV);
 *   derivare(&d, &eventus, effusio);   effusio = Xar de Eventus
 */

#ifndef DERIVARE_H
#define DERIVARE_H

/* <aedilis corpus="lib/derivare.c"/> */

#include "latina.h"
#include "xar.h"
#include "fenestra.h"
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
} Derivator;


/* ==================================================
 * FUNCTIONES
 * ================================================== */

vacuum
derivator_initiare (
    Derivator* d,
          s64  intervallum_ms,
          i32  distantia);

/* Appendit eventum crudum, deinde derivata (DUPLEX) ad effusionem
 * (Xar de Eventus). */
vacuum
derivare (
           Derivator* d,
    constans Eventus* ev,
                 Xar* effusio);

#endif /* DERIVARE_H */
