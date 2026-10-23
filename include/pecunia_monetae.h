/* pecunia_monetae.h - tabula monetarum GENERATA (internum)
 *
 * Corpus: lib/pecunia_monetae.c, scriptum a
 * tools/pecunia_monetae_generare.sh ex indicibus ISO 4217 fixis.
 * Consumptores pecunia.h solum includunt; hoc caput lib/pecunia.c
 * et probationes legunt.
 */

#ifndef PECUNIA_MONETAE_H
#define PECUNIA_MONETAE_H

#include "latina.h"
#include "pecunia.h"

/* ordine codicum (strcmp) - quaestio binaria */
extern constans PecuniaMonetaInformatio PECUNIA_MONETAE[];
extern constans i32 PECUNIA_MONETAE_NUMERUS;

/* codices ISO sine unitate minore (N.A.: metalla, unitates
 * rationum, XTS XXX) - ut SINE_UNITATE ab IGNOTA discernatur */
extern constans character* constans PECUNIA_MONETAE_SINE_UNITATE[];
extern constans i32 PECUNIA_MONETAE_SINE_UNITATE_NUMERUS;

#endif /* PECUNIA_MONETAE_H */
