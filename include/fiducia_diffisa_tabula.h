/* fiducia_diffisa_tabula.h - tabula diffisarum ordinaria, GENERATA
 * (tools/fiducia_diffisa_generare.sh -> lib/fiducia_diffisa_tabula.c).
 * Caput separatum ut quisque caput implementationem UNAM habeat
 * (conventio silicis: X.h -> lib/X.c; aedilis idem). */
#ifndef FIDUCIA_DIFFISA_TABULA_H
#define FIDUCIA_DIFFISA_TABULA_H

#include "fiducia_diffisa.h"

/* <aedilis corpus="lib/fiducia_diffisa_tabula.c"/> */

/* ordine (genus, primum, secundum); signum finis non numeratur */
extern constans FiduciaDiffisa fiducia_diffisae[];
extern constans i32 fiducia_diffisae_numerus;
extern constans character* fiducia_diffisae_tempus;   /* "2026-10-21" */

#endif /* FIDUCIA_DIFFISA_TABULA_H */
