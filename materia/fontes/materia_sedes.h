/* materia_sedes.h - Collatio visionis sedium (porta clientium)
 *
 * Una functio quam probatio STML cuiusque clientis super corpus suum
 * vocat (exemplar: materia_registrum_recens). ORACULUM a scriptore
 * DISIUNCTUM: lineae et columnae ex tabula linearum fontis ipsius,
 * octeti lexematum contra segmentum fontis, tractus nodorum per
 * ambulationem propriam, elementa ordine documenti paria.
 *
 * Quod iudicatur: (1) visio a proiectione nuda NIHIL differt praeter
 * attributa sedes/octeti et 'visio' involucri; (2) elementum quodque
 * sedibus ornatum tractui exspectato congruit, ordine; (3) lector
 * visionem recusat. Recusatur (causa nominata): consilium cum
 * loci_admissi, templa_activa, aut fronte liberos ornante.
 */
#ifndef MATERIA_SEDES_H
#define MATERIA_SEDES_H

#include "latina.h"
#include "piscina.h"
#include "materia_arbor.h"

nomen structura {
                    b32  sana;
                    i32  elementa;   /* elementa sedibus ornata visa */
                    i32  lexemata;   /* lexemata fontis cum segmento */
                    i32  derivata;   /* lexemata derivata (puncta) */
                    i32  nodi;       /* nodi tractu ornati */
     constans character* causa;      /* prima; NIHIL si sana */
                    i32  index;      /* elementi divergentis */
} MateriaSedesRelatio;

/* VERUM = sana. relatio semper impletur. */
b32
materia_sedes_verificare (
                           Piscina* piscina,
             constans MateriaNodus* radix,
    constans MateriaArborConsilium* consilium,
                constans character* fons,
                               i32  mensura,
               MateriaSedesRelatio* relatio);

#endif /* MATERIA_SEDES_H */
