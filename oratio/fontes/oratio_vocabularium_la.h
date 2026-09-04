/* oratio_vocabularium_la.h - Quaesitio Latina in tabula WORDS (T8, spec
 * par. IV)
 *
 * ONUS: tabula la.bin (oratio_vocabularium.h) in memoriam legitur et
 * tabulae dispersae per CLAVEM PLICATAM struuntur (decisio III:
 * minusculae, u/v et i/j iunctae, macra et ligaturae sublatae) - stirpes
 * cum clave stirpis (I-IV; 'zzz' = stirps absens), fines flexionum
 * (finis vacuus seorsum), formae unicae. Octeti fontis in recordis ut
 * sunt; plicatio in clavibus solum.
 *
 * QUAESITIO (ordo WORDS, words_engine-word_package.adb transcriptus):
 *  I.   unica: forma plicata tota == forma unica plicata.
 *  II.  stirps + finis: pro omni scissione (finis 0..VII octetorum,
 *       stirps I..XVIII) flexio quaeque cum eo fine et stirps quaeque
 *       dictionarii cum ea stirpe CONGRUANT: pars (VPAR/SUPINE -> V;
 *       PACK dictionarii flexiones PRON accipit), clavis stirpis (0 in
 *       flexione = quaelibet), declinatio/varians (flexio 0 0 =
 *       quaelibet praeter IX; flexio d 0 = declinatio d), genus nominis
 *       (flexio X = quodlibet, C = M aut F), gradus adiectivi/adverbii
 *       (X utrimque = quilibet), numerale: clavis exacta, praepositio:
 *       casus rectus.
 *  III. tackons (-que -ne -ve: additamenta TACKON basi X) SOLUM si nihil
 *       inventum est (ut WORDS - 'sine' numquam si + ne): forma minus
 *       tackon iterum quaeritur; analysis TACKON ante hospites ponitur,
 *       hospites tackon ferunt. Tackons cum basi (PRON/ADJ/PACK) et
 *       praefixa/suffixa: T8b (data numerata).
 *  Ordo: unica; deinde per stirpem dictionarii ORDINE PLAGULAE (ordo
 *  WORDS ipsius), fine longiore priore, flexione ordine plagulae.
 *  Lista vacua = IGNOTUM = inventum, numquam vitium.
 *
 * LEMMA (v1, 'forma prima'): stirps I + finis flexionis primae clavis I
 * congruentis (N: NOM S; ADJ: NOM S M gradus; V: PRES ACTIVE IND 1 S,
 * deponentia PASSIVE, impersonalia 3 S; indeclinabilia: stirps ipsa) -
 * ex datis WORDS ipsius, non ex tabulis nostris; forma dictionarii
 * plena (partes principales) postea.
 */

#ifndef ORATIO_VOCABULARIUM_LA_H
#define ORATIO_VOCABULARIUM_LA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "oratio_vocabularium.h"

nomen enumeratio {
    ORATIO_ANALYSIS_STIRPS = 0,   /* stirps dictionarii + flexio */
    ORATIO_ANALYSIS_UNICUM,       /* forma unica (UNIQUES.LAT) */
    ORATIO_ANALYSIS_TACKON        /* tackon ipse (que, ne, ve) */
} OratioAnalysisGenus;

nomen structura {
    OratioAnalysisGenus genus;
    s32 stirps;            /* index stirpis (STIRPS) aut -I */
    s32 flexio;            /* index flexionis (STIRPS) aut -I */
    s32 unicum;            /* index unici (UNICUM) aut -I */
    s32 tackon;            /* index additamenti: TACKON ipse, aut hospitis tackon; -I */
    i32 clavis;            /* clavis stirpis congruens (I-IV) */
    i32 mensura_stirpis;   /* scissio in forma plicata (octeti stirpis) */
} OratioAnalysis;

nomen structura OratioVocabulariumLa OratioVocabulariumLa;

/* Tabulam onerare (octeti la.bin, in memoria manent - recorda eos
 * referunt). NIHIL = vitium (recensio) aut memoria. */
OratioVocabulariumLa*
oratio_vocabularium_la_onerare (
                     Piscina* piscina,
                      chorda  tabula,
    OratioVocabulariumVitium* vitium);

/* Forma plicata (copia in piscina): minusculae ASCII, v->u, j->i,
 * ae/oe ex ligaturis, macra/breves sublata; octeti ceteri ut sunt. */
chorda
oratio_vocabularium_la_plicare (
    Piscina* piscina,
     chorda  forma);

/* Analyses formae, ordine WORDS; Xar de OratioAnalysis, vacuus =
 * ignotum. NIHIL = memoria. */
Xar*
oratio_vocabularium_la_quaerere (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
                           chorda  forma);

constans OratioStirps*
oratio_vocabularium_la_stirps (
    constans OratioVocabulariumLa* voc,
                              s32  i);

constans OratioFlexio*
oratio_vocabularium_la_flexio (
    constans OratioVocabulariumLa* voc,
                              s32  i);

constans OratioAdditamentum*
oratio_vocabularium_la_additamentum (
    constans OratioVocabulariumLa* voc,
                              s32  i);

constans OratioUnicum*
oratio_vocabularium_la_unicum (
    constans OratioVocabulariumLa* voc,
                              s32  i);

OratioVocabulariumCensus
oratio_vocabularium_la_census (
    constans OratioVocabulariumLa* voc);

/* numerus tackonum basi X (que ne ve ...) */
i32
oratio_vocabularium_la_tackones (
    constans OratioVocabulariumLa* voc);

/* Lemma v1 (forma prima) stirpis: copia in piscina. */
chorda
oratio_vocabularium_la_lemma (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
                              s32  stirps);

#endif /* ORATIO_VOCABULARIUM_LA_H */
