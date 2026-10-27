/* hora_hybrida.h - HORA HYBRIDA (hybrid logical clock; genera-
 * datorum VIII-c, D2): signum ordinis eventuum trans machinas.
 *
 * Signum = s64: (Momentum << XVI) | numerator (XVI bita). Ordo
 * numericus = ordo eventuum; (signum, machina) = ordo TOTUS, idem in
 * omni machina (machina nexus ultimos solvit). Prope tempus parietis
 * manet (legibile, intervallis quaeribile), numquam retro currit -
 * horologio retro saliente numerator crescit, non signum minuitur.
 * XLVIII bita millisecundorum: plus quam octo milia annorum.
 *
 * Duo horologia, duo officia: seq voluminis = ordo ADVENTUS (cursor
 * proiectionum); hora = ordo EVENTUS (ultimus scriptor vincit per
 * horam, numquam per seq). */

#ifndef HORA_HYBRIDA_H
#define HORA_HYBRIDA_H

/* <aedilis corpus="lib/hora_hybrida.c"/> */

#include "latina.h"
#include "fasti.h"

#define HORA_HYBRIDA_NUMERATOR_BITA XVI

s64
hora_hybrida_componere (
    Momentum momentum,
         i32 numerator);

Momentum
hora_hybrida_momentum (
    s64 signum);

i32
hora_hybrida_numerator (
    s64 signum);

/* eventus localis: max(physicum << XVI, ultimum + I) */
s64
hora_hybrida_proxima (
         s64 ultimum,
    Momentum physicum);

/* tempus parietis, millisecunda ab epocha */
Momentum
hora_hybrida_physica (
    vacuum);

#endif /* HORA_HYBRIDA_H */
