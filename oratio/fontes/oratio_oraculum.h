/* oratio_oraculum.h - Oraculum treebank (T13): classes orationis contra UD
 *
 * Sententia CoNLL-U quaeque: textus eius per orationem parsatur et
 * annotatur (oratio_partes), elementa (vocabulum | interpunctio |
 * numerus) extentibus octetorum in textu locantur, lexemata aurea in
 * eodem textu ordine quaeruntur (ranga 'a-b' = superficies una,
 * verba eius sub elemento eodem iudicantur), et elementum quod
 * initium lexematis aurei continet (aut elementa quae extentum eius
 * tangunt) classes suas praebet. MENSURAE per verbum aureum: TECTUM
 * (classis aurea inter classes nostras - COVERAGE, pinnata solum
 * crescens), PRIMARIUM (classis nostra prima == aurea - relatum, ordo
 * gradus V est), LEMMA (lemma aureum plicatum inter lemmata nostra
 * plicata - relatum), IGNOTUM (nostrum 'ignotum'), INALIGNATUM (forma
 * aurea in textu non inventa - inventum lectoris/textus, non
 * classificationis). Tabula per classem auream cum exemplis primis
 * non tectis. UPOS -> classis: NOUN substantivum, PROPN nomen-proprium,
 * VERB verbum, AUX auxiliare, ADJ adiectivum, ADV adverbium, PRON
 * pronomen, DET determinans, ADP adpositio, NUM numerale, CCONJ/SCONJ
 * coniunctiones, PART particula, INTJ interiectio, SYM symbolum, PUNCT
 * interpunctio, X ignotum; alia ('_' rangae ipsius) extra tabulam.
 */

#ifndef ORATIO_ORACULUM_H
#define ORATIO_ORACULUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "oratio_registrum.h"
#include "oratio_conllu.h"
#include "oratio_vocabularia.h"
#include "oratio_resolutio.h"
#include "tabula_dispersa.h"

#define ORATIO_ORACULUM_EXEMPLA 5

/* PARTITIO decisionum (T19g 2026-09-08, decretum SUDOKU decisio XL):
 * verbum aureum quodque per genus decisionis elementi PRIMI sui -
 * praelatio | impletio | umbra (OratioDecisio, locus 'decisio'
 * vocabuli scriptus) | una (lectio una, nemo decidit: soluta) |
 * aperta (lectiones plures, nemo decidit: ordo fontis manet) |
 * nulla (sine lectione). COACTAE = impletio + umbra (testimonium):
 * accuratio earum pinnata solum crescens - decisio coacta falsa est
 * corruptio strati, non error ordinis. Numerus primarii per genus
 * dividit quid testimonium, quid prior, quid fortuna lexici sit. */
#define ORATIO_ORACULUM_PARTITIO_UNA     ((i32)ORATIO_DECISIO_NUMERUS)
#define ORATIO_ORACULUM_PARTITIO_APERTA  ((i32)ORATIO_DECISIO_NUMERUS + 1)
#define ORATIO_ORACULUM_PARTITIO_NULLA   ((i32)ORATIO_DECISIO_NUMERUS + 2)
/* verbum rangae non primum (-que, 's): numquam primarium per
 * constructionem - tectum rangarum (~II % Senecae) visibile */
#define ORATIO_ORACULUM_PARTITIO_RANGA   ((i32)ORATIO_DECISIO_NUMERUS + 3)
#define ORATIO_ORACULUM_PARTITIO_NUMERUS ((i32)ORATIO_DECISIO_NUMERUS + 4)
externus constans character* constans ORATIO_ORACULUM_TITULI_PARTITIONIS[];

/* ACCURATIO per AUCTOREM (T19g): verba aurea quorum elementum primum
 * hunc auctorem fert (titulus regulae decidentis, 'lex-umbrarum' pro
 * lege) et primaria eorum - QUAE regula decisiones falsas facit.
 * Lex summae: verba auctorum omnium == praelatio + impletio + umbra. */
nomen structura {
    chorda titulus;
       i32 verba;
       i32 primaria;
} OratioOraculumAuctor;

nomen structura {
    chorda forma;      /* forma aurea (fontem referens) */
    chorda classes;    /* classes nostrae (copia) aut "ignotum" */
    chorda lemma;      /* lemma aureum */
} OratioOraculumExemplum;

nomen structura {
                       i32 verba;
                       i32 tecta;
                       i32 primaria;
                       i32 lemmata;
                       i32 ignota;
                       i32 inalignata;
                       i32 numerus_exemplorum;
    OratioOraculumExemplum exempla[ORATIO_ORACULUM_EXEMPLA];   /* non tecta prima */
} OratioOraculumClassis;

/* DISCREPANTIA PRIMARII (T19a, 2026-09-07): verbum aureum TECTUM
 * (classis aurea inter nostras) sed non PRIMUM - classis nostra prima
 * alia. Tabulata per (classis aurea, forma plicata, classis nostra
 * prima) cum numero. Ex hac tabula regulae priorum classium gradus V
 * scribuntur: quae formae quam classem primam falso ferunt, quoties.
 * LEX: summa numerorum classis aureae == tecta - primaria eius. */
nomen structura {
    OratioClassis aurea;
    OratioClassis nostra;    /* classis nostra PRIMA (NUMERUS_CLASSIUM = nulla) */
           chorda forma;     /* plicata (copia in piscina iudicii) */
              i32 numerus;
} OratioOraculumDiscrepantia;


nomen structura {
    i32 sententiae;
    i32 sententiae_fractae;   /* parsura aut annotatio fracta */
    i32 verba;                /* verba aurea (verba rangarum inclusa, rangae ipsae non) */
    i32 rangae;
    i32 tecta;
    i32 primaria;
    i32 lemmata;
    i32 ignota;
        i32 inalignata;
        i32 sententiae_linguae[ORATIO_LINGUA_NUMERUS];   /* census linguae per
                                                      * sententiam (T18):
                                                      * quot Latinae, quot
                                                      * Anglicae iudicatae */
    /* LINGUA DOCUMENTI (T18): thesaurus totus = documentum unum, ut
     * instrumenta documentum totum censent - sententia sola (nomen
     * unum, 'I have.', linea sine verbis) suffragiis paribus Latina
     * caderet; suffragia = vocabula quae linguam ferunt, summata
     * per thesaurum; -I = non censa (sine programmate) */
                                            s32 lingua_documenti;
                                            i32 suffragia_linguarum[ORATIO_LINGUA_NUMERUS];
    /* discrepantiae primarii (T19a): Xar de OratioOraculumDiscrepantia*
     * (cellae stabiles in piscina iudicii) + index clavis
     * 'aurea/nostra/forma'; pigre creata in iudicio, NIHIL ante */
                                          Xar* discrepantiae;
                               TabulaDispersa* discrepantiae_index;
    /* partitio decisionum (T19g): verba et primaria per genus */
        i32 partitio_verba[ORATIO_ORACULUM_PARTITIO_NUMERUS];
        i32 partitio_primaria[ORATIO_ORACULUM_PARTITIO_NUMERUS];
    /* auctores (T19g): Xar de OratioOraculumAuctor* + index per
     * titulum; pigre creati, NIHIL ante */
                     Xar* auctores;
          TabulaDispersa* auctores_index;


    OratioOraculumClassis classes[ORATIO_CLASSIS_NUMERUS_CLASSIUM + I];   /* [NUMERUS] = UPOS extra tabulam */
} OratioOraculumCensus;

/* UPOS -> classis; ORATIO_CLASSIS_NUMERUS_CLASSIUM si extra tabulam. */
OratioClassis
oratio_oraculum_classis_ex_upos (
    chorda upos);

vacuum
oratio_oraculum_census_vacare (
    OratioOraculumCensus* census);

/* Discrepantiae classis aureae ordine NUMERI (maior prior, deinde
 * forma octetim, deinde classis nostra): Xar de
 * OratioOraculumDiscrepantia* in piscina data; vacuus si nullae
 * (census sine iudicio quoque). NIHIL = memoria. */
Xar*
oratio_oraculum_discrepantiae (
                          Piscina* piscina,
    constans OratioOraculumCensus* census,
                    OratioClassis  aurea);

/* Auctores ordine verborum non crescente (T19g): Xar de
 * OratioOraculumAuctor* in piscina; vacuus sine decisionibus. NIHIL =
 * memoria. */
Xar*
oratio_oraculum_auctores (
                          Piscina* piscina,
    constans OratioOraculumCensus* census);


/* Sententias iudicare, censum CUMULARE (vacare prius si novus).
 * vocabularia = tabula Latina cum glossario et Moby (T16: lectiones
 * Anglicae quoque inter classes). FALSUM = memoria. */
b32
oratio_oraculum_iudicare (
                          Piscina* piscina,
       constans OratioVocabularia* vocabularia,
                              Xar* sententiae,
             OratioOraculumCensus* census);

/* Idem cum RESOLUTIONE (T17): sententia quaeque post annotationem
 * programmate resolvitur (regulae primae N; -I omnes; programma NIHIL
 * = sine resolutione), PRIMARIUM tunc ordinem resolutum metitur;
 * lingua sententiae ex censu annotationis eius. */
b32
oratio_oraculum_iudicare_resolutum (
                          Piscina* piscina,
       constans OratioVocabularia* vocabularia,
         constans OratioProgramma* programma,
                              s32  regulae_numerus,
                              Xar* sententiae,
             OratioOraculumCensus* census);

#endif /* ORATIO_ORACULUM_H */
