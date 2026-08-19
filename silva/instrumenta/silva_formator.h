/* silva_formator.h - machina lint formatoris (regulae formandi G1)
 *
 * Regulae: project-specs/formator-regulae.md (nomina stabilia -
 * divergentiae se nominant). Tranche prima = regulae fluminis
 * crudi: R5 (tabulae), R6 (spatium post claves), R12 (spatia
 * caudae), R14 (vexilla), R16 (longitudo). Regulae structurales
 * (arbore egentes) in tranche secunda veniunt.
 *
 * Hic machina sola (probabilis, silici portabilis); modi et CLI
 * in instrumenta/principalia/formator.c - exemplar differre.
 */

#ifndef SILVA_FORMATOR_H
#define SILVA_FORMATOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

nomen structura {
    constans character* regula;      /* nomen stabile regulae */
    constans character* nuntius;     /* descriptio brevis */
                   i32  linea;       /* 1-basata */
                   i32  columna;     /* 1-basata */
                   s32  inventum;    /* valor inventus */
                   s32  exspectatum; /* valor exspectatus */
} FormatorDivergentia;

/* Lint super fontem unum: Xar de FormatorDivergentia (vacuum =
 * conformis; NIHIL solum si piscina fracta). Fons vivere debet
 * quamdiu divergentiae vivunt (visus, nulla copia). */
Xar*
formator_lint (
              Piscina* piscina,
    constans character* fons,
                   i32  mensura);

#endif /* SILVA_FORMATOR_H */
