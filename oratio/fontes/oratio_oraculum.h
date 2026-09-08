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
    /* per DISTANTIAM socii (T19g bis): vicina = socius ad distantiam
     * I, remota = ultra (regulae cursu fratrum ultra vicinum ligant);
     * sine socio (praelatio) in neutro. Mensura ianuae strictae. */
       i32 vicina;
       i32 vicina_primaria;
       i32 remota;
       i32 remota_primaria;
} OratioOraculumAuctor;

/* ERRATUM decisionis (T19g bis, 2026-09-08): verbum aureum cuius
 * elementum primum decisionem fert (praelatio | impletio | umbra) sed
 * non PRIMARIUM est - tabulatum per (auctor, forma plicata, classis
 * aurea, classis nostra prima, socius) cum numero. Socius = forma
 * vocabuli quocum lectio prima ligata est (umbra impleta lectionis
 * primae: carrier; aliter umbra vicini in hoc vocabulum spectans:
 * implens), vacua pro praelatione. Ex hac tabula regulae emendantur:
 * QUAS formas regula falso decidit, cum QUO ligatas. LEX: summa
 * numerorum auctoris == verba - primaria eius (tabula AUCTOR). */
nomen structura {
           chorda auctor;
           chorda forma;     /* plicata */
    OratioClassis aurea;
    OratioClassis nostra;    /* prima (NUMERUS_CLASSIUM = nulla) */
                      chorda socius;    /* forma socii, vacua si nullus */
              s32 distantia; /* |socius - verbum| in elementis; -I nullus */
              i32 numerus;
} OratioOraculumErratum;

/* ERRATUM CLAUSULAE (T20a, 2026-09-08): verbum aureum positum (causa
 * scripta) cuius clausula aurea non est clausula MAIOR clausulae
 * nostrae (puritas) - tabulatum per (causa, forma plicata, species
 * clausulae nostrae, forma radicis aureae) cum numero. Ex hac tabula
 * strata et semina emendantur. */
nomen structura {
       s32 causa;      /* OratioClausulaCausa */
    chorda forma;      /* plicata */
       s32 species;    /* OratioSpeciesClausulae nostrae */
    chorda radix;      /* forma radicis clausulae aureae (plicata) */
       i32 numerus;
} OratioOraculumErratumClausulae;

/* SEMEN (T20a): census aureus candidati seminis - lemma (SCONJ CCONJ
 * PRON DET ADV) aut forma (PUNCT): occurrentiae, quoties LIMEN
 * clausulae (clausula aurea != clausula verbi praecedentis non
 * interpunctionis non coordinantis; initium sententiae = limen;
 * interpunctio: verba utrimque clausulis diversis), quoties in
 * clausula FINITA, initia, et pro CCONJ: quoties verbum finitum
 * aureum utrimque intra segmentum (ff) et limen tunc. Data listarum
 * ORATIO_SEMINA_* - instrumentum -semina. */
nomen structura {
    chorda textus;     /* lemma aut forma (copia) */
    chorda upos;       /* copia */
       i32 n;
       i32 limes;
       i32 finita;
       i32 initia;
       i32 ff;
       i32 ff_limes;
} OratioOraculumSemen;

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
        /* errata decisionum (T19g bis): Xar de OratioOraculumErratum* +
     * index; pigre creata */
                     Xar* errata;
          TabulaDispersa* errata_index;
    /* CLAUSULAE (T20a, decisio XLVII): PURITAS per causam - verba
     * aurea (non PUNCT, alignata) quorum elementum primum causam
     * fert, et recta (clausula aurea == clausula aurea MAIOR clausulae
     * nostrae); [CAUSA_NUMERUS] = aperta (locus non scriptus).
     * Coactio = posita / iudicata. NUMERUS: sententiae iudicatae,
     * clausulae nostrae, aureae, sententiae paribus numeris. */
        i32 clausulae_verba[ORATIO_CLAUSULA_CAUSA_NUMERUS + I];
        i32 clausulae_rectae[ORATIO_CLAUSULA_CAUSA_NUMERUS + I];
        i32 clausulae_iudicata;
        i32 clausulae_sententiae;
        i32 clausulae_nostrae;
        i32 clausulae_aureae;
        i32 clausulae_pares;
    /* CONCORDIA PARIUM (T20a ter): paria verborum iudicatorum intra
     * sententiam quorum 'eadem clausula' idem iudicat nostrum et aureum
     * (Rand): puritas sola segmentationem nimiam praemiat (clausula
     * unius verbi semper pura), numerus solus ordinem non videt -
     * concordia utrumque punit. Mensura decisionum variationum. */
    i32 clausulae_paria;
    i32 clausulae_paria_concordia;
    Xar* errata_clausularum;   /* OratioOraculumErratumClausulae* */
    TabulaDispersa* errata_clausularum_index;


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

/* Errata auctoris (auctor vacuus = omnium) ordine numeri non
 * crescente, deinde auctoris et formae: Xar de OratioOraculumErratum*
 * in piscina. NIHIL = memoria. */
Xar*
oratio_oraculum_errata (
                          Piscina* piscina,
    constans OratioOraculumCensus* census,
                           chorda  auctor);

/* Errata clausularum (T20a) ordine numeri non crescente, deinde
 * causae et formae: Xar de OratioOraculumErratumClausulae*. NIHIL =
 * memoria. */
Xar*
oratio_oraculum_errata_clausularum (
                          Piscina* piscina,
    constans OratioOraculumCensus* census);

/* Census seminum AUREUS (T20a): super sententias CoNLL-U solas (sine
 * parsura): Xar de OratioOraculumSemen ordine n non crescente, tum
 * textus; lemmata SCONJ CCONJ PRON DET ADV et formae PUNCT solae.
 * NIHIL = memoria. */
Xar*
oratio_oraculum_semina (
    Piscina* piscina,
        Xar* sententiae);

/* Clausula aurea (T20a, decisio XLVII) lexematis k sententiae:
 * positio (in lexematibus, rangis inclusis) radicis clausulae -
 * lexema modo finito (VerbForm=Fin) relatione non aux/cop, aut
 * praedicatum cum aux/cop finito, aut radix sententiae; verbi
 * clausula = maior proximus radix, ipsum inclusum. -I si k ranga
 * aut extra. finita: an radix finita sit. */
s32
oratio_oraculum_clausula_aurea (
                           Piscina* piscina,
    constans OratioConlluSententia* sententia,
                               i32  k,
                               b32* finita);


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
