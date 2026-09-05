/* oratio_vocabularia.h - Vocabularia orationis: tabula Latina cum
 * glossario et tabula Anglica (Moby) una structura (T16, gradus IV)
 *
 * Annotatio (oratio_partes) et oraculum ambo vocabularia consulunt:
 * verbum quodque in AMBOBUS quaeritur, lectiones Latinae primae,
 * Anglicae post appensae (ambiguitas = lista in omni strato; ordo =
 * gradus V). en NIHIL = Latina sola. Onerator unus ex radice
 * repositorii (RHUBARB_RADIX): la.bin, glossarium.stml, mobypos.txt -
 * instrumenta et portae eundem adhibent (nulla copia lectoris per
 * instrumentum).
 */

#ifndef ORATIO_VOCABULARIA_H
#define ORATIO_VOCABULARIA_H

#include "latina.h"
#include "piscina.h"
#include "oratio_vocabularium.h"
#include "oratio_vocabularium_la.h"
#include "oratio_vocabularium_en.h"

nomen structura {
    constans OratioVocabulariumLa* la;   /* WORDS + glossarium */
    constans OratioVocabulariumEn* en;   /* Moby; NIHIL = Latina sola */
} OratioVocabularia;

/* Tria onerare ex radice (NIHIL = "."): oratio/vocabularium/la.bin,
 * oratio/glossarium.stml, oratio/vocabularium/en/mobypos.txt - omnia
 * necessaria. FALSUM cum vitio (plagula nominata, linea, causa);
 * exitus tunc NIHIL ambo. */
b32
oratio_vocabularia_onerare (
                     Piscina* piscina,
          constans character* radix,
           OratioVocabularia* exitus,
    OratioVocabulariumVitium* vitium);

#endif /* ORATIO_VOCABULARIA_H */
