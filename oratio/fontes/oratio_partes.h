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
    /* SUFFRAGIA: vocabula quae linguam ferunt (semel per vocabulum et
     * linguam; T18). Census linguae sententiae his fit, non analysibus:
     * 'a' Latinum XX analyses fert, 'the cat sat' Anglica VI - analyses
     * numeratae CCCXVI sententias EWT Latinas, XXI CIRCSE Anglicas
     * iudicabant. */
        i32 vocabula_linguarum[ORATIO_LINGUA_NUMERUS];
        /* T21: vocabula scissa in hospitem et encliticum (-que -ne -ve) */
    i32 enclitica;
    /* T22: vocabula per formam variantem orthographiae mediae annotata */
    i32 orthographia;
    /* T22 b (T30 c): vocabula NOTA quibus lectiones variantes appensae
     * (dialecto medio solo) */
    i32 orthographia_notae;
    /* T22 b: dialectus documenti annotati (OratioDialectus; datus aut
     * praescansione censum) */
    s32 dialectus;
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

/* ENCLITICUM SCINDERE (T21): vocabulum cuius lectio prima ex
 * vocabulario Latino TACKON est (forma tota ignota, forma minus
 * -que/-ne/-ve nota - contractus III vocabularii, ut WORDS) et quod
 * Moby totum nescit (unique, mosque: custodia Anglica) in HOSPITEM et
 * ENCLITICUM dividitur: lexema partitum positione exacta (emissio ==
 * fons manet), cauda hospitis ad encliticum migrat (hospes caudam
 * vacuam tenet), encliticum lectionem UNAM (TACKON: que/ve coordinans,
 * ne particula) cum compendiis accipit; hospes NON annotatus manet -
 * annotatio sequens formam hospitis ipsam quaerit. Vocabulum partibus
 * pluribus (hyphen) aut iam annotatum numquam scinditur. *encliticum =
 * nodus novus (NIHIL = non scissum); vocans eum post vocabulum in
 * listam elementorum inserit (_annotare id facit). FALSUM = memoria. */
b32
oratio_partes_encliticum_scindere (
                          Piscina*  piscina,
                          Piscina*  scratch,
       constans OratioVocabularia*  vocabularia,
                     MateriaNodus*  vocabulum,
               OratioPartesCensus*  census,
                     MateriaNodus** encliticum);

/* Vocabulum unum annotare (scratch pro quaestione, piscina pro
 * arbore); census NIHIL licet. FALSUM = memoria. */
b32
oratio_partes_vocabulum_annotare (
                           Piscina* piscina,
                           Piscina* scratch,
        constans OratioVocabularia* vocabularia,
                      MateriaNodus* vocabulum,
                OratioPartesCensus* census,
                   OratioDialectus  dialectus);

/* Compendia CLASSES/LINGUAE vocabuli ex ordine analysium PRAESENTI
 * reponere (post permutationem resolutionis, gradus V; verbum
 * materia_nodus_reponere); vocabulum sine compendiis (non annotatum
 * aut sine analysi) intactum. FALSUM = memoria. */
b32
oratio_partes_compendia_reponere (
         Piscina* piscina,
    MateriaNodus* vocabulum);

/* REFERENTIAE (T33, 2026-09-10): impletio umbrae et socius alternae
 * REFERENTIAE ad nodum analysis sunt (materia MATERIA_LOCUS_REFERENTIA:
 * permutationem listae supervivunt, remissio ordinalium sublata);
 * ordinales (vocabulum intra elementa, analysis intra analyses) per
 * PATRES derivantur, ut speculum 'n' proiectionis. Referentia scripta
 * cum scopo NIHIL = REVOCATA (olim index -I): scripta, non impleta -
 * proiectio eam recusat ut olim indicem negativum. */

/* Ordinalis nodi intra listam patris (lista prima in qua sedet); -I
 * sine patre aut absens. */
s32
oratio_ordinalis (
    constans MateriaNodus* nodus);

/* Nodus analysis b vocabuli w inter elementa sententiae; NIHIL extra
 * fines aut sine analysi. */
MateriaNodus*
oratio_analysis_ordinalibus (
    MateriaValor elementa,
             i32 w,
             i32 b);

/* An locus referentiae SCRIPTUS sit (impletus aut revocatus). */
b32
oratio_referentia_scripta (
    constans MateriaNodus* nodus,
                      i32  locus);

/* Scopus referentiae loci: nodus analysis; NIHIL si non scripta aut
 * revocata. */
MateriaNodus*
oratio_referentiae_scopus (
    constans MateriaNodus* nodus,
                      i32  locus);

/* Ordinalis vocabuli scopi intra elementa; -I si scopus nullus. */
s32
oratio_referentiae_vocabulum (
    constans MateriaNodus* nodus,
                      i32  locus);

/* Ordinales scopi: *vocabulum intra elementa, *analysis intra analyses
 * vocabuli; FALSUM et ambo -I si scopus nullus aut sine patribus. */
b32
oratio_referentiae_ordinales (
    constans MateriaNodus* nodus,
                      i32  locus,
                      s32* vocabulum,
                      s32* analysis);

/* Arborem totam (documentum aut subarborem quamlibet) annotare;
 * census NIHIL licet. FALSUM = memoria. */
b32
oratio_partes_annotare (
                          Piscina* piscina,
       constans OratioVocabularia* vocabularia,
                     MateriaNodus* radix,
               OratioPartesCensus* census);

/* PRAESCANSIO dialecti (T22 b / T30 c): vocabula subarboris vocabulario
 * Latino quaesita (forma plicata semel per indicem) - verba = vocabula
 * omnia, recuperata = formae ignotae per orthographiam mediam
 * recuperatae (custodiae annotationis eaedem: capitales non, Moby
 * novit non). FALSUM = memoria. */
b32
oratio_partes_praescandere (
                          Piscina* piscina,
       constans OratioVocabularia* vocabularia,
            constans MateriaNodus* radix,
                              i32* recuperata,
                              i32* verba);

/* Dialectus ex censu: MEDIUS si recuperata >= V millesimae verborum
 * (et > 0), aliter CLASSICUS. */
OratioDialectus
oratio_partes_dialectus_censu (
    i32 recuperata,
    i32 verba);

/* Annotare dialecto dato; IGNOTUS = praescansio subarboris ipsius
 * decernit (documentum totum: oraculum thesaurum totum censet et
 * dialectum sententiis dat). census->dialectus scribitur. */
b32
oratio_partes_annotare_dialecto (
                           Piscina* piscina,
        constans OratioVocabularia* vocabularia,
                      MateriaNodus* radix,
                OratioPartesCensus* census,
                   OratioDialectus  dialectus);

#endif /* ORATIO_PARTES_H */
