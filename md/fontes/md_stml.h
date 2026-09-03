/* md_stml.h - Consilium proiectionis STML markdown (A8)
 *
 * Proiectio = materia_arbor_scribere_nodum/legere cum consilio hoc:
 * registrum MD, lexicon ratum, grammatica "md", et UNCUS ORIGINIS pro
 * lexematibus DERIVATIS (fons I, spec par. III):
 *
 *  - scriptor: sedes_quaerere ea NON-fontem declarat -> positio
 *    PORTATA (linea/columna; b absens quia byte_offset -I) et f="1";
 *  - lector: radix_quaerere radicem VACUAM semper positam reddit ->
 *    lector positionem derivati non tangit nec cursorem promovet
 *    (materia_arbor.c: 'lexema derivatum octetos non tenet').
 *
 * Trivia nulla, frons nulla: markdown caudam lexematis non habet.
 */

#ifndef MD_STML_H
#define MD_STML_H

#include "latina.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"

vacuum
md_stml_consilium (
           MateriaArborConsilium* consilium,
    constans MateriaLexiconRatum* ratum);

#endif /* MD_STML_H */
