/* radices_interna.h - INTERNA radicum: apices piscinarum temporariarum.
 * NON API - solum lib/radices.c et probationes id includunt;
 * consumptores radices.h solum. Praefixum 'radicum_' (genetivus) ab API
 * 'radix_' / 'radices_' distinguit.
 *
 * Probatio memoriae piscinam vocantis solam metiri non debet: lucrum
 * verum (refectiones in piscinis temporariis) ibi invisibile est
 * (recensio radicum II, L1: tres plantae sine refectione virides). */

#ifndef RADICES_INTERNA_H
#define RADICES_INTERNA_H

#include "radices.h"

/* apices ad nihilum */
vacuum
radicum_apices_purgare (vacuum);

/* apex maximus (octeti, piscina_summa_apex_usus) acervorum VCA ab
 * ultima purgatione */
memoriae_index
radicum_apex_acervi (vacuum);

/* apex maximus piscinarum officinae (status, opus) ab ultima
 * purgatione */
memoriae_index
radicum_apex_officinae (vacuum);

#endif /* RADICES_INTERNA_H */
