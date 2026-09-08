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
 *  IV.  orthographia media (T22): quaesitio SEPARATA
 *       (oratio_vocabularium_la_quaerere_variantes) formarum variantium
 *       per correspondentias mensuratas (e > ae, e > i, h addita, ...),
 *       ab annotatore sola poscita cum forma ipsa ignota est.
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
    ORATIO_ANALYSIS_TACKON,       /* tackon ipse (que, ne, ve) */
    ORATIO_ANALYSIS_GLOSSARIUM    /* forma glossarii domus (T9): fons primus */
} OratioAnalysisGenus;

nomen structura {
    OratioAnalysisGenus genus;
    s32 stirps;            /* index stirpis (STIRPS) aut -I */
    s32 flexio;            /* index flexionis (STIRPS) aut -I */
    s32 unicum;            /* index unici (UNICUM) aut -I */
    s32 tackon;            /* index additamenti: TACKON ipse, aut hospitis tackon; -I */
        s32 glossarium;        /* index formae glossarii (GLOSSARIUM) aut -I */
    i32 clavis;            /* clavis stirpis congruens (I-IV) */
    i32 mensura_stirpis;   /* scissio in forma plicata (octeti stirpis) */
    s32 varians;           /* T22: index correspondentiae orthographiae qua forma varians inventa est; -I = forma ipsa */
} OratioAnalysis;

/* CORRESPONDENTIA orthographiae mediae (T22, contractus IV): forma
 * varians ex forma plicata generatur - modus SUBSTITUTIO: 'a' -> 'b'
 * una occurrentia per vicem (sinistra prima); PRAEFIXUM: 'b' ante
 * formam; INSERTIO: 'b' post quamlibet litteram de 'a'; GEMINATIO:
 * littera quaelibet de 'a' duplicata. 'activa' = interruptor mensurae
 * (quaeque sola thesauris V mensurata, numeri in tabula). */
nomen enumeratio {
    ORATIO_ORTHOGRAPHIA_SUBSTITUTIO = 0,
    ORATIO_ORTHOGRAPHIA_PRAEFIXUM,
    ORATIO_ORTHOGRAPHIA_INSERTIO,
    ORATIO_ORTHOGRAPHIA_GEMINATIO
} OratioOrthographiaModus;

nomen structura {
         constans character* titulus;
    OratioOrthographiaModus  modus;
         constans character* a;
         constans character* b;
                        b32  activa;
} OratioOrthographia;

externus constans OratioOrthographia ORATIO_ORTHOGRAPHIA[];
externus constans i32 ORATIO_ORTHOGRAPHIA_NUMERUS;

nomen structura OratioVocabulariumLa OratioVocabulariumLa;
structura OratioGlossarium;   /* incompleta: oratio_glossarium.h eam definit */


/* Glossarium domus appendere (T9, oratio_glossarium.h - tag structurae
 * hic, ne typedef bis definiatur): quaesitio eius formas PRIMAS reddit
 * (fons primus), inventae numerantur (tackons non tentantur). NIHIL =
 * detrahere. */
vacuum
oratio_vocabularium_la_glossarium_ponere (
    OratioVocabulariumLa* voc,
    constans structura OratioGlossarium* glossarium);

constans structura OratioGlossarium*
oratio_vocabularium_la_glossarium (
    constans OratioVocabulariumLa* voc);


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

/* CONTRACTUS IV (T22, orthographia media): formae VARIANTES formae
 * ignotae ordine tabulae ORATIO_ORTHOGRAPHIA (correspondentiae activae
 * solae) et positionis (sinistra prima) quaeruntur (quaesitio plena,
 * tackons inclusis); varians prima quae analyses reddit vincit, eius
 * analyses 'varians' = index correspondentiae ferunt. Vocans (annotator)
 * eam solum poscit cum forma ipsa nihil reddidit et Moby formam totam
 * nescit - vocabularium ipsum numquam sponte variat (recensio
 * identificatorum formas ipsas iudicat). *varians = index inventae aut
 * -I. Xar vacuus = nihil; NIHIL = memoria. */
Xar*
oratio_vocabularium_la_quaerere_variantes (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
                           chorda  forma,
                              s32* varians);

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
