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
/* T29 NOTAE (2026-09-09): accidentia praeter casum contra aurum UD -
 * numerus, genus, persona, modus, vox, forma verbi, tempus (ordo
 * tabulae NOTAE_ORACULI in oratio_oraculum.c) */
#define ORATIO_ORACULUM_NOTAE 7

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
    /* T23 CASUS: verba aurea casum ferentia quorum classis prima recta
     * est et lectio prima Latina casum fert; recti = casus lectionis
     * primae == casus aureus (quae regula casum falsum eligit) */
       i32 casus_verba;
       i32 casus_recti;
    /* T32 a (2026-09-09): ARCUS per auctorem - umbrae lectionis primae
     * quas haec regula implevit (auctor umbrae) et rectae earum; vicinae
     * = distantia I. Tabula fiduciae hinc: praecisio arcuum, non
     * accuratio lectionum (differunt: regulae obiecti lectiones 85 %,
     * arcus 55-65 %). */
       i32 ligationes;
       i32 ligationes_rectae;
       i32 ligationes_vicinae;
       i32 ligationes_vicinae_rectae;
} OratioOraculumAuctor;

/* CONTENTIO (T32 b): regula victrix, regula victa, alternae, victae rectae */
nomen structura {
    chorda victor;
    chorda victa;
       i32 numerus;
       i32 victae_rectae;
    /* T32 c: numeri CONIUNCTI - victa recta VICTORE FALSO (flectendum) et
     * victor rectus VICTA FALSA (servandum); arcus ambo eodem capite (subiectum
     * contra obiectum eiusdem verbi) in neutro - contentio lectionis, non arcus */
       i32 victa_sola;
       i32 victor_solus;
    /* T32 d: CASUS - lectio dependentis quam victa et victor proposuerunt
     * contra casum aureum; contentio lectionis in arcu uno (subiectum contra
     * obiectum eiusdem verbi) hic sola discriminat */
       i32 casus_iudicati;      /* casus aureus, victae, victoris omnes noti */
       i32 victa_casu_sola;     /* lectio victae casum aureum fert, victoris non */
       i32 victor_casu_solus;
} OratioOraculumContentio;

/* LIS (T32 e): contentio singula cum notis quas regula videre potuit -
 * materia census notarum (quae nota sortem Senecae findat). Aurum: casus,
 * deprel, caput dependentis; nostrum: casus lectionis victae et victoris,
 * genus lectionis victae, clausula dependentis et in ea nominativi alii
 * (lectione ulla | certi = sine accusativa | concordes numero capitis),
 * accusativi certi; primum clausulae; numerus, persona, vox lectionis
 * primae capitis petiti. Caput = caput a victa petitum; victoris idem =
 * contentio lectionis in arcu uno. */
nomen structura {
    chorda victor;
    chorda victa;
    chorda dependens;            /* forma aurea */
    chorda caput;                /* forma aurea capitis petiti */
    chorda deprel;               /* deprel aureus dependentis */
       b32 ante;                 /* dependens ante caput */
       b32 caput_aureum_idem;    /* caput aureum dependentis == caput petitum */
       b32 caput_victoris_idem;  /* caput victoris == caput petitum */
       s32 casus_aureus;
       s32 casus_victae;
       s32 casus_victoris;
       s32 genus_victae;
       s32 numerus_victae;       /* T32 f: numerus lectionis victae / victoris */
       s32 numerus_victoris;
       s32 clausula;             /* -I aperta */
       b32 primum_clausulae;
       i32 nominativi;
       i32 nominativi_certi;
       i32 nominativi_concordes;
       i32 accusativi_certi;
       s32 numerus_capitis;
       s32 persona_capitis;
       s32 vox_capitis;
} OratioOraculumLis;

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

/* ERRATUM NOTAE (T30, 2026-09-09): verbum aureum classis rectae cuius
 * lectio prima Latina accidens k fert sed valore FALSO (condicio
 * columnae T29) - tabulatum per (accidens, classis, valor aureus primus,
 * valor noster, decisio, auctor, forma plicata, socius, distantia,
 * attingibile) cum numero. ATTINGIBILE = lectio aliqua eiusdem classis
 * Latina valorem aureum fert (electio falsa: ordo lectionum aut
 * concordantia spuria) an nulla (vocabularium aut conventio auri).
 * Socius et distantia ut in ERRATO decisionis. LEX: summa numerorum
 * accidentis k == notae_verba[k] - notae_recti[k]. */
nomen structura {
              i32 nota;        /* k tabulae NOTAE_ORACULI */
    OratioClassis classis;     /* aurea == nostra prima */
              s32 aurea;       /* valor aureus primus scriptus */
              s32 nostra;      /* valor lectionis primae */
              s32 decisio;     /* OratioDecisio; -I nemo */
           chorda auctor;      /* vacua sine decisione */
           chorda forma;       /* plicata */
           chorda socius;      /* forma socii, vacua si nullus */
              s32 distantia;   /* -I nullus */
              b32 attingibile;
              i32 numerus;
} OratioOraculumErratumNotae;

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

/* SENTENTIA OSTENSA (T20a quater, 2026-09-08): sententia cum verbo
 * male posito causae petitae - textus et verba aurea ordine, quodque
 * cum clausula nostra, causa, forma radicis aureae et signo mali
 * (clausula aurea != maior clausulae nostrae). Contextus quem index
 * erratorum aggregatus celat: quota census->ostendendae, filtrum
 * census->causa_ostendenda (-I = quaelibet). */
nomen structura {
    chorda forma;
       s32 nostra;    /* -I aperta */
       s32 causa;     /* -I */
    chorda radix;     /* forma radicis clausulae aureae */
       b32 malum;
} OratioOraculumVerbumOstensum;

nomen structura {
    chorda  textus;
       Xar* verba;    /* OratioOraculumVerbumOstensum */
} OratioOraculumSententiaOstensa;

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
    /* T23: casus (vide OratioOraculumCensus) */
                       i32 casus_verba;
                       i32 casus_recti;
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
    /* 2026-09-11: proiectiones gradus resolutionis fractae, summa
     * census resolutionis (fractae) per sententias - sententia cuius
     * proiectio in gradu fallit gradum TACITE amittebat (petitio
     * revocata trans gradus improiectabilis: XXV/XLIII/XIX per
     * thesaurum); pinna ZEPHYRUM ne recurrat */
    i32 resolutiones_fractae;
    /* T34 (2026-09-14): ordines recusati quia umbra gradu priore iam
     * scripta (summa census resolutionis) - RELATUM, non pinnatum */
    i32 ordines_scripti_recusati;
    i32 verba;                /* verba aurea (verba rangarum inclusa, rangae ipsae non) */
    i32 rangae;
        i32 tecta;
    i32 primaria;
    i32 lemmata;
    i32 ignota;
        i32 inalignata;
    /* T23 CASUS (2026-09-08): mensura casuum, quam primarium non videt
     * (substantivum casu falso lectum 'primarium' est). Populus = verba
     * aurea 'Case=' ferentia quorum classis prima recta est ET lectio
     * prima Latina locum casus fert; recti = casus lectionis primae ==
     * aureus. Condicionalis classi: casuum disambiguationem sola metitur
     * (lex subiecti T20c IV hic apparet); pinnata solum crescens. */
            i32 casus_verba;
            i32 casus_recti;
    /* T29 NOTAE: per accidens k tabulae - verba aurea accidens ferentia
     * classis rectae cum lectione prima Latina accidens ferente; recti
     * = valor lectionis primae == valor aureus (tabula mappat; tempus
     * ex Tense + Aspect). Numerus et genus pinnati solum crescentes,
     * cetera relata. */
            i32 notae_verba[ORATIO_ORACULUM_NOTAE];
            i32 notae_recti[ORATIO_ORACULUM_NOTAE];
    /* T30: recti per CONVENTIONEM solam - valor noster alter acceptus
     * (vox deponens pro activa/passiva; genus commune pro masculino/
     * feminino: WORDS 'C' = utrumque, aurum unum elegit), non aequalis
     * aureo; pars rectorum relata ne conventio numerum celet */
            i32 notae_conventione[ORATIO_ORACULUM_NOTAE];
    /* T26 LIGATIO (2026-09-08): ligationes nostrae (umbrae impletae
     * lectionis primae verbi alignati, socio alignato) contra capita
     * aurea - umbra capitis: socius == caput aureum carrier; umbra
     * obiecti: carrier == caput aureum socii. PRAECISIO = rectae /
     * nostrae; REVOCATIO = rectae / arcus aurei (verba alignata non
     * interpuncta capite alignato). Per relationem (caput | obiectum)
     * et distantiam (vicina |v - w| = I | remota). */
        i32 ligationes_nostrae;
        i32 ligationes_rectae;
        i32 arcus_aurei;
    /* T31 a: per RELATIONEM umbrae (index OratioRelatio: obiectum caput
     * verbum-finitum subiectum) nostrae et rectae; arcus aurei subiecti
     * = lexemata alignata deprel nsubj* capite alignato (revocatio
     * subiecti = rectae[subiectum] / arcus aurei subiecti) */
        i32 ligationes_nostrae_relationum[ORATIO_RELATIO_NUMERUS];
        i32 ligationes_rectae_relationum[ORATIO_RELATIO_NUMERUS];
        i32 arcus_aurei_subiecti;
        i32 arcus_aurei_obiecti;   /* T31 b: deprel obj alignati */
        i32 ligationes_capitis;
        i32 ligationes_capitis_rectae;
        i32 ligationes_vicinae;
        i32 ligationes_vicinae_rectae;
    /* T32 a: DEPENDENTES CONTESTI - elementa quae umbrae impletae plures
     * (lectionum primarum) ut dependens petunt (caput unum per verbum
     * violatum); petitiones earum et rectae. Census legis capitis unius. */
        i32 dependentes_contesti;
        i32 petitiones_contestae;
        i32 petitiones_contestae_rectae;
    /* T32 b (2026-09-09): TECTUM ARCUUM - umbrae lectionis primae quae
     * petitionem ullam ferunt (prima aut alterna) = petitae; tectae =
     * petitio ulla recta (arcus aureus inter candidatos, ut tectum
     * lectionum contra primarium); alternae numeratae et rectae. */
        i32 ligationes_petitae;
        i32 ligationes_tectae;
        i32 alternae_numerus;
        i32 alternae_rectae;
    /* CONTENTIONES (T32 b): per (victor, victa) numerus alternarum et
     * victae rectae - census iudicis contentionum */
                   Xar* contentiones;
        TabulaDispersa* contentiones_index;
    /* T32 e: lites - Xar de OratioOraculumLis*, una per alternam; pigre
     * creatae, NIHIL ante (instrumentum -lites) */
        Xar* lites;
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
    /* DIALECTUS DOCUMENTI (T22 b / T30 c): thesaurus totus censetur in
     * eodem circuitu ac lingua - recuperata = formae per orthographiam
     * mediam recuperatae, verba = vocabula; dialectus per
     * oratio_partes_dialectus_censu, sententiis datus (ignotus sine
     * programmate: sententia quaeque se ipsa censet) */
                                            s32 dialectus;
                                            i32 dialectus_recuperata;
                                            i32 dialectus_verba;
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
    /* errata notarum (T30): Xar de OratioOraculumErratumNotae* + index;
     * pigre creata */
                     Xar* errata_notarum;
          TabulaDispersa* errata_notarum_index;
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
    /* T20b: ex censu resolutionis - discordiae (margines ligati trans
     * clausulas) et catenatae (verba per catenam mota) */
    i32 catena_discordiae;
    i32 catena_catenatae;

        Xar* errata_clausularum;   /* OratioOraculumErratumClausulae* */
    TabulaDispersa* errata_clausularum_index;
    /* sententiae ostensae (T20a quater): quota (ponenda post vacare;
     * 0 = nullae), filtrum causae, Xar de OratioOraculumSententiaOstensa
     * (pigre) */
    i32  ostendendae;
    s32  causa_ostendenda;
    Xar* ostensae;


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

/* Contentiones (T32 b): Xar de OratioOraculumContentio* ordine numeri
 * non crescente; NIHIL sine contentionibus */
Xar*
oratio_oraculum_contentiones (
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

/* titulus accidentis k tabulae notarum (T29); NIHIL extra tabulam */
constans character*
oratio_oraculum_nota_titulus (
    i32 k);

/* titulus valoris v accidentis k (T30); NIHIL extra tabulam */
constans character*
oratio_oraculum_nota_valor_titulus (
    i32 k,
    s32 v);

/* Errata notarum (T30) accidentis k (k < 0 = omnium) ordine numeri non
 * crescente, deinde accidentis, classis, formae et socii: Xar de
 * OratioOraculumErratumNotae* in piscina. NIHIL = memoria. */
Xar*
oratio_oraculum_errata_notarum (
                          Piscina* piscina,
    constans OratioOraculumCensus* census,
                              s32  nota);

#endif /* ORATIO_ORACULUM_H */
