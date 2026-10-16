/* unitates_arboris.h - sutura ARBORIS VERAE pro lib/unitates
 * (include/unitates.h): viae per 'git ls-files -co --exclude-standard'
 * (tractae + novae non ignoratae - declaratio nondum commissa iam
 * valet), plagulae per filum - utraque sub 'radix' (NIHIL = cwd;
 * 'git -C radix', 'radix/via').
 *
 * Seorsum a nucleo: unitates.c machina pura manet (probationes
 * plagulas in memoria dant); consumptores veri (bin/fabrica quid,
 * oratio vocabula) hanc suturam UNAM communicant.
 *
 * Nomen NON 'unitates_posix': aedilis caput include/unitates.h ad
 * lib/unitates_{macos,posix} PRIUS quam lib/unitates.c resolvit
 * (variantes, aedilis.stml) - adaptator '_posix' nucleum
 * supplantaret. */
#ifndef UNITATES_ARBORIS_H
#define UNITATES_ARBORIS_H

#include "unitates.h"

/* sutura in piscina (Unitates eam servat - provenientia pigre per eam
 * legitur, ergo piscina Unitates superstes esse debet); radix
 * copiatur */
UnitatesSutura*
unitates_sutura_arboris (
    constans character* radix,
               Piscina* piscina);

#endif /* UNITATES_ARBORIS_H */
