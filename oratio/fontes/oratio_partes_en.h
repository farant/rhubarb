/* oratio_partes_en.h - Mappatio Moby et regularum Anglicarum in
 * descriptiones universales (T16, gradus IV)
 *
 * Analysis Anglica (oratio_vocabularium_en_analysare: recordum exactum
 * aut basis per regulam) -> descriptio(nes) OratioDescriptio:
 *   forma exacta: una descriptio per CLASSEM litterarum codicis, ordine
 *     Moby (praelatio Moby = ordo lectionum); littera 'p' numerum
 *     pluralem addit; litterae eiusdem classis coalescunt; nativum =
 *     codices verbatim ("NVt"); littera extra legendam nihil dat
 *     (recordum totum extra legendam: ignotum).
 *   regula: accidentia ex titulo regulae (DATA ORATIO_REGULAE_EN):
 *     pluralis-*      una per classem BASIS quam regula admittit -
 *                     substantivum: numerus pluralis; verbum: persona
 *                     III, numerus singularis, tempus praesens (tests =
 *                     substantivum pluralis ET verbum)
 *     praeteritum-*   verbum: tempus praeteritum, modus indicativus,
 *                     forma finitum; lectio altera forma participium
 *     participium-*   verbum: forma participium, tempus praesens
 *     possessivum     substantivum: casus genitivus
 *     adverbium-*     adverbium
 *     comparativus-*  adiectivum: gradus comparativus
 *     superlativus-*  adiectivum: gradus superlativus
 *     contractio-*, compositum: classis analysis, sine accidentibus
 *     nativum = "<codices basis> <titulus regulae>" ("NVt pluralis-s")
 *   lemma = basis plicata (forma ipsa aut basis derivata); lingua
 *   anglica; fons vocabularium-en; sensus absens (Moby sensum non
 *   fert).
 */

#ifndef ORATIO_PARTES_EN_H
#define ORATIO_PARTES_EN_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "oratio_partes_la.h"
#include "oratio_vocabularium_en.h"

/* Analysin Anglicam unam describere: descriptiones in exitus (Xar de
 * OratioDescriptio) APPENDIT (una plerumque, plures ubi classes basis
 * aut lectio altera). FALSUM = memoria. */
b32
oratio_partes_en_describere (
                          Piscina* piscina,
    constans OratioVocabulariumEn* en,
        constans OratioAnalysisEn* analysis,
                              Xar* exitus);

/* Cursus mappationis II (oraculum EWT dev, dies primus 2026-09-05:
 * verba aurea per classem / tecta): listae DATA formarum plicatarum,
 * NIHIL terminatae, semel per vocabulum post analyses Anglicas
 * adhibitae - lectio secundaria appensa fonte 'regula', nativum =
 * titulus listae:
 *   AUXILIARIA     (AUX MDLXVII / 0 %): be am is are was were been
 *                  being have has had having do does did done doing
 *                  will would shall should can could may might must
 *                  ought
 *   SUBORDINANTES  (SCONJ CCCXCVII / 0 %): that because since if
 *                  although though while whereas unless until as than
 *                  whether after before when whenever where wherever
 *                  once so lest till like
 *   PARTICULAE     (PART DCXLVII / 0 %): to not
 *   NUMERALIA      (NUM CCCLXXXIII / 79.6 %): zero one two ... trillion
 *   INTERIECTIONES (INTJ CXV / 16.5 %): yes no please welcome hello hi
 *                  hey oh ah wow thanks ok okay sorry yeah yep nope bye
 *                  goodbye
 * Regulae quoque (PART, AUX in rangis contractionum: it's = it + 's):
 *   possessivum      classis basis (pronomen | substantivum) casu
 *                    genitivo, + particula (POS), + auxiliare ('s = is,
 *                    has)
 *   contractio-n't   + particula
 *   contractio-'re/'m/'ll/'ve/'d  + auxiliare
 * Nomen proprium capitale (PROPN MDCCCLXVII / 22.6 %: President, Bush)
 * in annotatore: vocabulum capitale quod nullus fons Latinus novit et
 * quod substantivum (aut nihil) est -> + nomen-proprium (regula). */
externus constans character* constans ORATIO_AUXILIARIA_EN[];
externus constans character* constans ORATIO_SUBORDINANTES_EN[];
externus constans character* constans ORATIO_PARTICULAE_EN[];
externus constans character* constans ORATIO_NUMERALIA_EN[];
externus constans character* constans ORATIO_INTERIECTIONES_EN[];

/* Lectiones secundariae ex listis pro forma (semel per vocabulum):
 * descriptiones appendit fonte 'regula', lingua anglica. FALSUM =
 * memoria. */
b32
oratio_partes_en_secundariae (
    Piscina* piscina,
     chorda  forma,
        Xar* exitus);

#endif /* ORATIO_PARTES_EN_H */
