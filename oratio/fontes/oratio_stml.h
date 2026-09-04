/* oratio_stml.h - Consilium proiectionis STML orationis (T4)
 *
 * Proiectio = materia_arbor_scribere_nodum/legere cum consilio hoc:
 * registrum ORATIO, lexicon ratum, grammatica "oratio", et UNCUS
 * ORIGINIS pro lexematibus DERIVATIS (fons I: classes, linguae, lemma,
 * sensus - gradus III): scriptor positionem PORTATAM scribit (linea/
 * columna; b absens quia byte_offset -I) et f="1"; lector radicem
 * VACUAM semper positam reddit, positionem derivati non tangit nec
 * cursorem promovet (ut md_stml.c). Trivia nulla, frons nulla.
 */

#ifndef ORATIO_STML_H
#define ORATIO_STML_H

#include "latina.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"

vacuum
oratio_stml_consilium (
           MateriaArborConsilium* consilium,
    constans MateriaLexiconRatum* ratum);

#endif /* ORATIO_STML_H */
