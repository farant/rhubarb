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

/* ORNATUS proiectionis (T17, gradus V): frons cum dato. Ordinales 'n'
 * SEMPER scribuntur (sententia intra paragraphum, elementa intra
 * sententiam, analyses intra vocabulum) ut planum resolutionis
 * vocabula et analyses nominet sine extentis octetorum; 'lingua'
 * (census annotatoris: latina | anglica) in documento et sententia
 * cum ornatus eam fert. Lector attributa nodi ignorat. */
nomen structura {
      MateriaArborFrons  frons;
     constans character* lingua;   /* NIHIL = non scribitur */
} OratioStmlOrnatus;

/* Consilium cum ornatu: ut oratio_stml_consilium, frons copiata in
 * ornatum cum lingua (ornatus vivat dum consilium adhibetur). */
vacuum
oratio_stml_consilium_ornatum (
           MateriaArborConsilium* consilium,
    constans MateriaLexiconRatum* ratum,
               OratioStmlOrnatus* ornatus,
              constans character* lingua);

#endif /* ORATIO_STML_H */
