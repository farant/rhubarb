/* rivus_eventuum.h - Server-Sent Events, lector incrementalis
 * (quaestio-rivus-spec II.3; WHATWG "event stream interpretation")
 *
 * Octeti intrant; eventa (nomen, data) per functionem exeunt. Lineae
 * CRLF, LF aut CR (CR ad finem adiectionis + LF in proxima = finis
 * unus); 'event:' nomen (ordinarie "message"), 'data:' (plures lineae
 * '\n' iunctae), 'id:', 'retry:' et campi ignoti neglecti, ':'
 * commentarium, linea vacua dispatchat (sine data: nihil). Spatium
 * unum post ':' tollitur. BOM initialis neglegitur. In fine
 * (rivus_eventuum_finire) eventus imperfectus abicitur. Chordae
 * functioni datae post reditum invalidae sunt: copia si servanda. */

#ifndef RIVUS_EVENTUUM_H
#define RIVUS_EVENTUUM_H

/* <aedilis corpus="lib/rivus_eventuum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

nomen vacuum (*RivusEventuumFunctio)(vacuum* datum, chorda eventus,
                                     chorda data);

nomen structura RivusEventuum RivusEventuum;   /* opacus */

RivusEventuum*
rivus_eventuum_creare (
                 Piscina* piscina,
    RivusEventuumFunctio  functio,
                  vacuum* datum);

vacuum
rivus_eventuum_adicere (
     RivusEventuum* rivus,
       constans i8* octeti,
               i32  n);

vacuum
rivus_eventuum_finire (
    RivusEventuum* rivus);

/* eventa dispatchata hactenus */
i32
rivus_eventuum_numerus (
    constans RivusEventuum* rivus);

#endif /* RIVUS_EVENTUUM_H */
