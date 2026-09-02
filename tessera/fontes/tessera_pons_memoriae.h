/* tessera_pons_memoriae.h - Pons memoriae: sutura probationum
 * (Phase A)
 *
 * Machina ficta: initus = octeti scripti (probatio eos ponit),
 * exitus = effugia capta (probatio ea legit et asserit), amplitudo
 * mutabilis (semita magnitudinis renovandae probabilis sine
 * SIGWINCH). intrare/exire numerantur - status crudi observabilis.
 */

#ifndef TESSERA_PONS_MEMORIAE_H
#define TESSERA_PONS_MEMORIAE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "tessera_pons.h"

nomen structura {
          TesseraPons  pons;      /* pons.datum = haec structura */
              Piscina* piscina;
          constans i8* initus;    /* scriptum initus (copiatum) */
                  i32  initus_mensura;
                  i32  initus_cursor;
    ChordaAedificator* captum;    /* exitus captus */
                  i32  latitudo;
                  i32  altitudo;
                  b32  intratum;
                  b32  resumendum;   /* proxima interrogatio VERUM dat */
                  i32  numerus_intratum;
                  i32  numerus_exitum;
} TesseraPonsMemoriae;

TesseraPonsMemoriae*
tessera_pons_memoriae_creare (
    Piscina* piscina,
        i32  latitudo,
        i32  altitudo);

/* Scriptum initus ponere (copiatur); cursor ad initium redit */
b32
tessera_pons_memoriae_initum (
    TesseraPonsMemoriae* pm,
            constans i8* octeti,
                    i32  mensura);

/* Visus exitus capti (validus usque ad mutationem proximam) */
chorda
tessera_pons_memoriae_captum (
    TesseraPonsMemoriae* pm);

/* Exitum captum vacare (buffer manet - exemplar reset) */
vacuum
tessera_pons_memoriae_purgare (
    TesseraPonsMemoriae* pm);

/* Amplitudinem mutare (semita renovationis probanda) */
vacuum
tessera_pons_memoriae_amplitudo (
    TesseraPonsMemoriae* pm,
                    i32  latitudo,
                    i32  altitudo);

#endif /* TESSERA_PONS_MEMORIAE_H */
