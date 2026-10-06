/* ludus_tessera_pons.h - Pons tesserae super terminalis, et octeti
 * modorum (modulus 013 A1; project-specs/ludus-tessera-plan.md)
 *
 * TERMINALIS POSSIDET, TESSERA PINGIT (decisio III): terminalis modum
 * crudum, signa et restitutionem tenet; rivus modos initus DECLARAT;
 * tessera cellulas, differentias et effusionem solum. Pons hic tenuis
 * est: scribere et amplitudo ad terminalis; intrare/egredi NIHIL
 * scribunt; legere -1 (lector tesserae in hac via non adhibetur:
 * Eventus a rivo veniunt); resumptum NIHIL (glutinum resumptionem
 * tractat).
 *
 * OCTETI SCRINII CUM MODIS IN UNA INTRATIONE: tractatores signorum
 * terminalis (ruina, TSTP) SOLOS octetos exeundi scribunt quos
 * terminalis_intrare accepit - scrinium alternum a ponte intratum in
 * ruina maneret. Ergo ludus_tessera_modos_componere intrandos et
 * exeundos totos componit, ordine tesserae (tessera_modi.h):
 *   intrandi: ?1049h, deinde modi rivi
 *   exeundi:  ?2026l PRIMUM (quadrum forte apertum), deinde exitus
 *             rivi (ordine inverso), deinde ?1049l, 0m, ?25h
 */

#ifndef LUDUS_TESSERA_PONS_H
#define LUDUS_TESSERA_PONS_H

#include "latina.h"
#include "terminalis.h"
#include "rivus_terminalis.h"
#include "tessera_pons.h"

/* Capacitas bufferorum modorum (= terminalis) */
#define LUDUS_TESSERA_MODI_MAXIMI  TERMINALIS_MODI_MAXIMI

/* Modos (RIVUS_MODUS_*) in rivo declarare et octetos intrandi/
 * exeundi componere; buffers capacitatis LUDUS_TESSERA_MODI_MAXIMI.
 * FALSUM si rivus NIHIL aut modi iam intrati (rivus nihil reddit). */
b32
ludus_tessera_modos_componere (
    RivusTerminalis* rivus,
                i32  modi,
                 i8* intrandi,
                i32* mensura_intrandi,
                 i8* exeundi,
                i32* mensura_exeundi);

/* Pontem super terminalis implere (sine allocatione; datum NIHIL). */
vacuum
ludus_tessera_pontem_initiare (
    TesseraPons* pons);

#endif /* LUDUS_TESSERA_PONS_H */
