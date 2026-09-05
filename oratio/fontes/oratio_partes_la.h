/* oratio_partes_la.h - Descriptio universalis analysium Latinarum (T12)
 *
 * Analysis fontis (WORDS stirps + flexio, forma unica, tackon, forma
 * glossarii) in DESCRIPTIONEM universalem vertitur: classis (UD XVII,
 * oratio_registrum.h) et accidentia ut indices enumerationum (-I =
 * non datum: WORDS 'X', persona 0), lemma, nativum (codex fontis
 * VERBATIM: "N 1 1 ACC S C", "V 3 1 PRES PASSIVE SUB 3 P IMPERS ...",
 * "TACKON que", "glossarium verbum"), sensus, fons, lingua. Codex
 * unus classes plures dare potest (stirps N kind N/L = nomen
 * proprium ET substantivum? NON: una vincit - kind decernit), ergo
 * exitus Xar est.
 *
 * TABULA (spec par. V): N -> substantivum (kind N/L -> nomen-proprium;
 * genus ex stirpe si M/F/N, aliter ex flexione; declinatio stirpis);
 * PRON, PACK -> pronomen; ADJ -> adiectivum (gradus); NUM -> numerale
 * (species CARD/ORD/DIST/ADVERB); ADV -> adverbium (gradus); V ->
 * verbum (persona numerus tempus modus vox; INF -> forma infinitivum;
 * stirps DEP/SEMIDEP -> vox deponens; coniugatio stirpis); VPAR ->
 * verbum forma participium (FUT PASSIVE PPL -> gerundivum) cum casu
 * numero genere; SUPINE -> verbum forma supinum; PREP -> adpositio
 * (casus rectus); CONJ -> coniunctio-coordinans, aut -subordinans si
 * lemma in ORATIO_CONIUNCTIONES_SUBORDINANTES; INTERJ -> interiectio;
 * TACKON que/ve -> coniunctio-coordinans, ne -> particula; glossarium:
 * classis entrii ipsa (ignotum-permissum -> ignotum), accidentia
 * formae per titulos enumerationum. Codices ignoti INVENTA sunt (-I),
 * numquam fractura.
 */

#ifndef ORATIO_PARTES_LA_H
#define ORATIO_PARTES_LA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "oratio_registrum.h"
#include "oratio_vocabularium_la.h"

nomen structura {
    OratioClassis classis;
              s32 casus_grammaticus;   /* OratioCasus aut -I ("casus" macro latina.h) */
              s32 numerus;       /* OratioNumerusGrammaticus aut -I */
              s32 genus;         /* OratioGenusGrammaticum aut -I */
              s32 persona;       /* OratioPersona aut -I */
              s32 tempus;        /* OratioTempus aut -I */
              s32 modus;         /* OratioModus aut -I */
              s32 vox;           /* OratioVox aut -I */
              s32 forma_verbi;   /* OratioFormaVerbi aut -I */
              s32 gradus;        /* OratioGradus aut -I */
              s32 species;       /* OratioSpeciesNumeralis aut -I */
              s32 declinatio;    /* I-V aut -I */
              s32 coniugatio;    /* I-IV aut -I */
                      chorda lemma;         /* copia in piscina */
           chorda natura;        /* species stirpis WORDS verbatim ("F L", "DEP"; referens) */
           chorda nativum;       /* codex fontis verbatim (copia) */
           chorda sensus;        /* fontem referens (non copiatur) */
    OratioFonsAnalysis fons;
        OratioLingua lingua;
} OratioDescriptio;

/* Coniunctiones subordinantes (lemma plicatum): ut cum si ne quod quia
 * quoniam dum quamquam etsi quamvis ubi postquam antequam priusquam
 * donec quin nisi ... NIHIL terminata (DATA). */
externus constans character* constans ORATIO_CONIUNCTIONES_SUBORDINANTES[];

/* Regulae SECUNDARIAE ex oraculo UD (T13, 2026-09-04; numeri = verba
 * aurea CIRCSE test / LLCT dev / LLCT test quae WORDS sic non
 * distinguit): descriptio prima manet PRIMARIA, copia classis alterius
 * post eam appenditur (lista candidatorum, ordo gradus V):
 *   AUXILIARIA (sum: AUX CLXVIII / DX / CDXLII): verbum -> + auxiliare
 *   DETERMINANTIA (hic ille meus omnis ...: DET DCLXXXV / MMXCVI /
 *     MCMXIV): pronomen | adiectivum | numerale -> + determinans
 *   PARTICULAE (non haud: PART CXCV / LXXXVI / LXXI): adverbium ->
 *     + particula
 *   SUBORDINANTES (cum qualiter unde quam: SCONJ CCXXII / CDXIV /
 *     CCCLXXXV): adverbium | adpositio cum lemmate listae ->
 *     + coniunctio-subordinans
 *   ordinale (tertius octavus: ADJ): numerale species ordinale ->
 *     + adiectivum
 *   natura N nominis (deus, Manes): nomen-proprium -> + substantivum
 *   CAPITALIS ignotum (Karolus Taenari: PROPN 55 % / 76 % ignota):
 *     nomen-proprium fonte 'regula', nativum "capitalis" */
externus constans character* constans ORATIO_AUXILIARIA[];
externus constans character* constans ORATIO_DETERMINANTIA[];
externus constans character* constans ORATIO_PARTICULAE[];

/* Descriptio vocabuli capitalis ignoti: nomen proprium fonte regula
 * (lemma = forma plicata). */
vacuum
oratio_partes_la_capitalis (
             Piscina* piscina,
              chorda  forma,
    OratioDescriptio* d);

/* Descriptionem vacuam ponere (classis ignotum, accidentia -I). */
vacuum
oratio_descriptio_vacare (
    OratioDescriptio* d);

/* Analysin unam describere: descriptiones in exitus (Xar de
 * OratioDescriptio) APPENDIT (plerumque una). FALSUM = memoria. */
b32
oratio_partes_la_describere (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
          constans OratioAnalysis* analysis,
                              Xar* exitus);

/* Codices WORDS -> indices enumerationum; -I si ignotus aut 'X'. */
s32
oratio_partes_la_casus (
    chorda codex);

s32
oratio_partes_la_numerus (
    chorda codex);

s32
oratio_partes_la_genus (
    chorda codex);

s32
oratio_partes_la_persona (
    chorda codex);

s32
oratio_partes_la_tempus (
    chorda codex);

s32
oratio_partes_la_vox (
    chorda codex);

s32
oratio_partes_la_modus (
    chorda codex);

s32
oratio_partes_la_gradus (
    chorda codex);

s32
oratio_partes_la_species (
    chorda codex);

/* Index tituli in serie titulorum (n tituli); -I si absens. */
s32
oratio_partes_index_tituli (
    constans character* constans* tituli,
                              i32  n,
                           chorda  titulus);

#endif /* ORATIO_PARTES_LA_H */
