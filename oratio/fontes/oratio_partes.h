/* oratio_partes.h - Annotatio vocabulorum: analyses, classes, linguae (T12)
 *
 * Cursus post arborem (oratio_arbor_parsare octetos numquam tangit):
 * vocabulum quodque semel quaeritur in UTROQUE vocabulario (tabula
 * Latina cum glossario T8/T9, Moby T15/T16: lectiones Anglicae post
 * Latinas appensae - ambiguitas lista, ordo gradus V),
 * analysis quaeque per oratio_partes_la in descriptionem universalem
 * vertitur, descriptio quaeque NODUS analysis-<classis> fit (genus =
 * classis, loci communes lemma/lingua/fons/nativum/sensus lexemata
 * derivata, accidentia INDEX) et in locum ANALYSES vocabuli ordine
 * fontis appenditur - prima primaria. Compendia: CLASSES (lexema
 * derivatum: tituli classium distincti ordine, spatiis divisi;
 * 'ignotum' sine analysi) et LINGUAE (linguae distinctae ordine;
 * absens sine analysi). Loci SEMEL scribuntur: vocabulum iam annotatum
 * (classes adest) praeteritur. Ignotum = inventum: analyses nullae,
 * classes 'ignotum', numquam fractura.
 *
 * Origo lexematum derivatorum = lexema primum partium vocabuli (sedes
 * portata, f="1").
 */

#ifndef ORATIO_PARTES_H
#define ORATIO_PARTES_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "oratio_registrum.h"
#include "oratio_partes_la.h"
#include "oratio_vocabularium_la.h"
#include "oratio_vocabularia.h"

nomen structura {
    i32 vocabula;     /* vocabula visa */
    i32 annotata;     /* nunc annotata (semel) */
    i32 analyses;     /* nodi analysis structi */
    i32 ignota;       /* vocabula sine analysi */
    i32 classes[ORATIO_CLASSIS_NUMERUS_CLASSIUM];   /* analyses per classem */
    /* analyses per linguam (T16) */
    i32 linguae[ORATIO_LINGUA_NUMERUS];
} OratioPartesCensus;

/* Locus accidentis (titulo) intra genus analysis classis; -I si genus
 * accidens id non fert (ex titulis registri, non ex tabula manu). */
s32
oratio_partes_locus (
           OratioClassis  classis,
      constans character* titulus);

/* Textus vocabuli: partes coniunctae (copia in piscina). */
chorda
oratio_partes_textus_vocabuli (
                  Piscina* piscina,
    constans MateriaNodus* vocabulum);

/* Nodum analysis ex descriptione struere (NIHIL = memoria aut locus). */
MateriaNodus*
oratio_partes_nodum_struere (
                      Piscina* piscina,
    constans OratioDescriptio* d,
        constans MateriaToken* origo);

/* Vocabulum unum annotare (scratch pro quaestione, piscina pro
 * arbore); census NIHIL licet. FALSUM = memoria. */
b32
oratio_partes_vocabulum_annotare (
                          Piscina* piscina,
                          Piscina* scratch,
       constans OratioVocabularia* vocabularia,
                     MateriaNodus* vocabulum,
               OratioPartesCensus* census);

/* Arborem totam (documentum aut subarborem quamlibet) annotare;
 * census NIHIL licet. FALSUM = memoria. */
b32
oratio_partes_annotare (
                          Piscina* piscina,
       constans OratioVocabularia* vocabularia,
                     MateriaNodus* radix,
               OratioPartesCensus* census);

#endif /* ORATIO_PARTES_H */
