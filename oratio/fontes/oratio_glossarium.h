/* oratio_glossarium.h - Glossarium domus (T9, spec par. IV)
 *
 * oratio/glossarium.stml MANU SCRIPTUM, canone
 * oratio/grammatica/glossarium.canon iudicatum (bin/canon_examen):
 * <vocabulum lemma lingua classis nota> cum <sensus> et <forma textus
 * + accidentia universalia (spec par. V: casus numerus genus persona
 * tempus modus vox forma-verbi gradus)>. Classis 'ignotum-permissum' =
 * terminus technicus permissus (offset, index, token ...). Vocabulum
 * sine forma: forma una = lemma.
 *
 * LECTOR: STML legitur (stml_legere), entria et formae in series,
 * tabula dispersa per formam PLICATAM (oratio_vocabularium_la_plicare -
 * eadem plicatio qua tabula WORDS, ut quaestio una utrisque serviat).
 * Canon hic NON iudicat (porta id facit); lector formam minimam
 * poscit (radix glossarium, lemma/lingua/classis, forma textus) et
 * cetera ut sunt fert. FONS PRIMUS: quaesitio Latina
 * (oratio_vocabularium_la) glossarium appensum ante tabulam consulit;
 * analysis glossarii = analysis ut ceterae, fonte notato.
 */

#ifndef ORATIO_GLOSSARIUM_H
#define ORATIO_GLOSSARIUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "oratio_vocabularium.h"

nomen structura {
    chorda lemma;
    chorda lingua;
    chorda classis;
    chorda nota;
    chorda sensus;
       i32 linea;          /* plagulae STML */
       i32 formae_ab;      /* index formae primae */
       i32 formae_numerus;
           b32    permissum;      /* classis == ignotum-permissum */
    chorda contextus;      /* latinus | anglicus | ambo (vacua = ambo) */
    b32    latine;         /* licet in contextu Latino (identificatores, commentaria) */
    b32    anglice;        /* licet in prosa Anglica */
} OratioGlossariumEntrium;


nomen structura {
       s32 entrium;
    chorda textus;
    chorda casus_grammaticus;   /* "casus" macro latina.h est */
    chorda numerus;
    chorda genus;
    chorda persona;
    chorda tempus;
    chorda modus;
    chorda vox;
    chorda forma_verbi;
    chorda gradus;
    chorda nota;
       i32 linea;
} OratioGlossariumForma;

nomen structura OratioGlossarium OratioGlossarium;

/* Glossarium ex fonte STML legere. NIHIL = vitium (plagula 'glossarium',
 * linea, causa) aut memoria. Octeti fontis in memoria manent. */
OratioGlossarium*
oratio_glossarium_legere (
                     Piscina* piscina,
                      chorda  fons,
    OratioVocabulariumVitium* vitium);

/* Formae glossarii quae formae datae (plicatae) congruunt: Xar de s32
 * (indices formarum) ordine plagulae; vacuus = nihil. NIHIL = memoria. */
Xar*
oratio_glossarium_quaerere (
                      Piscina* piscina,
    constans OratioGlossarium* gl,
                       chorda  forma);

/* An forma terminus technicus permissus sit (classis ignotum-permissum)
 * in contextu Latino (latine=VERUM) aut Anglico (FALSUM). */
b32
oratio_glossarium_permissum (
                      Piscina* piscina,
    constans OratioGlossarium* gl,
                       chorda  forma,
                          b32  latine);


constans OratioGlossariumEntrium*
oratio_glossarium_entrium (
    constans OratioGlossarium* gl,
                          s32  i);

constans OratioGlossariumForma*
oratio_glossarium_forma (
    constans OratioGlossarium* gl,
                          s32  i);

i32
oratio_glossarium_numerus_entriorum (
    constans OratioGlossarium* gl);

i32
oratio_glossarium_numerus_formarum (
    constans OratioGlossarium* gl);

#endif /* ORATIO_GLOSSARIUM_H */
