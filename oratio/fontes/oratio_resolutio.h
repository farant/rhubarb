/* oratio_resolutio.h - Resolutio ambiguitatis per contextum (gradus V,
 * T17, 2026-09-05; spec par. VII, decisiones XXVIII-XXXV)
 *
 * Programma oratio/partes/resolutio.stml = regulae ordine PRIORITATIS
 * (elementa <regula titulus=>), quaeque bracchium EXEMPLAR super
 * proiectionem sententiae annotatae et <consilium> cuius <PER> ordines
 * <praelatio vocabulum= analysis= regula=/> parit (machina exemplarium,
 * lib/stml_macros). Cursus per SENTENTIAM: subarbor sententiae in STML
 * scribitur cum ordinalibus 'n' et lingua documenti
 * (oratio_stml_consilium_ornatum), programma (regulae primae N; -I =
 * omnes) appenditur, documentum unum legitur et expanditur, ordines
 * consilii cuiusque regulae ordine programmatis leguntur - PRIMA
 * REGULA VINCIT per vocabulum, ordines posteriores in vocabulo iam fixo
 * numerantur, non applicantur -, et lista analysium vocabuli permutatur
 * (materia_nodus_lista_permutare: analysis praelata prima, ceterae
 * ordine suo, nihil deletum), compendia classes/linguae reposita
 * (oratio_partes_compendia_reponere). Documentum expansum canoni
 * orationis subiacet (regula/consilium/praelatio declarata): planum
 * iudicabile ante executionem. Sententia cuius compositio, lectio aut
 * expansio fracta est numeratur et praeteritur, nihil mutatum - numquam
 * fractura totius documenti. Ordo malus (attributum absens, ordinalis
 * extra fines, elementum non vocabulum) numeratur 'recusatus'.
 *
 * Idempotens: cursus alter nihil mutat (analysis praelata iam prima).
 */

#ifndef ORATIO_RESOLUTIO_H
#define ORATIO_RESOLUTIO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "materia_nodus.h"
#include "materia_lexicon.h"
#include "oratio_vocabularium.h"
#include "oratio_registrum.h"
#include "oratio_clausula.h"
#include "oratio_partes.h"
#include "tabula_dispersa.h"


/* regula programmatis: titulus (attributum) et textus eius in
 * programmate (extentum elementi <regula>, referens) */
nomen structura {
    chorda titulus;
    chorda textus;
       i32 gradus;    /* GRADUS applicationis (T19d beta): I ordinarius;
                       * regulae gradus II post permutationes gradus I
                       * proiectionem NOVAM vident (umbrae capitis
                       * substantivum iam resolutum sequuntur) */
    chorda lingua;    /* decisio LX: attributum 'lingua' (latina | anglica);
                       * vacua = omnis lingua. Regula linguae alterius in
                       * textum gradus non intrat (~XIII % expansionis) */
} OratioRegula;


nomen structura {
    chorda  textus;    /* programma totum (copia in piscina) */
       Xar* regulae;   /* Xar de OratioRegula ordine documenti */
    /* T32 a (2026-09-09): TABULA FIDUCIAE - titulus regulae -> i32*
     * permille praecisionis arcuum (oratio/probationes/fixa/auctores.tsv,
     * artefactum fixum ex tabulis auctorum plagularum pinnatarum;
     * NIHIL = plagula absens). ORATIO_FIDUCIA=1: regulae graduum II+
     * ordine fiduciae applicantur (petitio fidelior prior). */
    TabulaDispersa* fiducia;
    /* T32 c (2026-09-09): TABULA IUDICIS CONTENTIONUM - clavis 'victor TAB
     * victa' -> Iudicium (numerus, permille victae rectae) ex
     * oratio/probationes/fixa/contentiones.tsv (census contentionum
     * plagularum pinnatarum ante iudicem); NIHIL = plagula absens. */
    TabulaDispersa* contentiones;
    /* T38 b (2026-09-15): TABULA PONDERUM DECRETORIS - clavis 'regula TAB
     * dialectus TAB forma TAB folliculus TAB valor' ('-' absens) ->
     * OratioPondus* ex oratio/probationes/fixa/pondera.tsv (generator
     * oratio/census/pondera.py, plagulae editibiles solae, decisio
     * LVII); NIHIL = plagula absens (decretor = exsecutor). */
    TabulaDispersa* pondera;
} OratioProgramma;

/* PONDUS (T38 b, decisio LVII): contentiones regulae sub contextu et
 * folliculo, rectae, permille = rectae * M / contentiones (integer). */
nomen structura {
    i32 contentiones;
    i32 rectae;
    i32 permille;
} OratioPondus;

/* census per regulam: ordines applicati (vocabula quorum praelatio
 * huic regulae debetur, prima vincente) */
nomen structura {
    chorda titulus;
       i32 numerus;
} OratioResolutioRegulaCensus;

nomen structura {
    i32  sententiae;   /* sententiae tractatae */
    i32  fractae;      /* compositio/expansio fracta: praeterita */
    i32 regulae_omissae_lingua;   /* decisio LX: regulae linguae alterius omissae (per gradum, per sententiam) */
    i32  ordines;      /* praelationes lectae (sanae) */
    i32  recusatae;    /* praelationes malae */
    i32  repetitae;    /* in vocabulo iam fixo (prima vincit) */
        i32  applicatae;   /* vocabula permutata (analysis > 0) */
    Xar* per_regulam;  /* OratioResolutioRegulaCensus; NIHIL licet */
    i32  impletae;     /* umbrae ligatae (T19d: ordines impletio; umbra
                        * iam ligata = repetita, prima vincit) */
        /* vocabula lege umbrarum permutata (T19d gamma: lectio umbris a
     * vicinis impletis prima) */
    i32 umbris_ordinata;
        /* vocabula decisa per genus decisionis (T19g: locus 'decisio'
     * vocabuli scriptus - praelatio | impletio | umbra) */
    i32 decisae[ORATIO_DECISIO_NUMERUS];
    /* T31 a LEX CAPITIS: ligationes subiecti/obiecti ad caput socii
     * secutae (socius adiectivum/determinans umbra capitis impleta) */
    i32 ad_caput_secutae;
    /* T32 a: ordines recusati lege CAPITIS UNIUS (dependens iam petitus) */
    i32 recusatae_capitis;
    /* T32 a: petitiones stantes a petitione fideliore revocatae */
    i32 revocatae_capitis;
    /* T32 f: ordines recusati ut contentio LECTIONIS - caput idem ac
     * petitionis stantis (fiducia arcuum eam iudicare nequit; ordo decidit) */
    i32 recusatae_lectionis;
    /* T34 (2026-09-14): ordines recusati quia umbra GRADU PRIORE iam
     * scripta est (impleta aut revocata) - ligari non possunt (LIGATIO
     * umbram scriptam praeterit), ergo nec contendere debent */
    i32 recusatae_scriptae;
    /* T32 b: alternae scriptae (petitiones cedentes servatae) */
    i32 alternae;
        /* T20a: stampa clausularum (strata I-III) ante gradus - census
     * eius (semina, causae, clausulae per speciem) */
    OratioClausulaCensus clausulae;
    /* T20c lex I: ordines impletionis recusati quia carrier et implens
     * clausulis DIVERSIS positi sunt (ambo stratis puris: semen,
     * extentum, unica; relativum ad antecedens exceptum) */
        i32 recusatae_clausulis;
    /* T24: vocabula quorum lectiones prior casuum permutavit */
    i32 prior_casuum;
    /* T38 c DECRETOR (decisio XLVIII): cellulae contestatae (dependens
     * cum candidatis II+), decretae (petitio stans mutata), per habitum;
     * alternae praeoccupatae (ordines T34 servati ut candidatae);
     * decreta = Xar de OratioDecretum per cellulam contestatam (piscina
     * cursus; NIHIL ante) - materia ordinum DECRETUM oraculi */
    i32  cellae_contestatae;
    i32  decretae;
    i32  habitus[ORATIO_HABITUS_NUMERUS];
    i32  alternae_praeoccupatae;
    Xar* decreta;
} OratioResolutioCensus;

/* DECRETUM (T38 c): cellula contestata decisa. Nodi vocabulorum (non
 * ordinales) ut oraculum elementa sua per nodum inveniat; cedens =
 * candidata secunda superstes (petitio stans deposita si mutata). */
nomen structura {
    MateriaNodus* dependens;
    MateriaNodus* caput;
    MateriaNodus* umbra_electa;
    MateriaNodus* analysis_electa;
    MateriaNodus* umbra_cedens;        /* NIHIL si nulla */
    MateriaNodus* analysis_cedens;
    MateriaNodus* caput_cedentis;      /* vocabulum capitis cedentis */
          chorda  regula_electa;
          chorda  regula_cedens;
             s32  relatio;             /* OratioRelatio umbrae electae */
             b32  ante;                /* dependens ante caput */
             s32  numerus_capitis;     /* -I absens */
             s32  numerus_lectionis;
             i32  distantia;
             i32  gradus_electae;      /* scala: I folliculus, II regula,
                                        * III structura, IV fiducia; 0 nulla */
             i32 gradus_cedentis;
             s32 pondus_electae;      /* permille; -I nullum */
             s32 pondus_cedentis;
             s32 habitus;             /* OratioHabitus */
             b32 mutata;
} OratioDecretum;

/* CONTEXTUS DECLARATUS (T38 a, 2026-09-15, decisio LVII): lingua
 * documenti (titulus, ut hodie), dialectus (OratioDialectus, census
 * T30 c) et forma (OratioForma versus | prosa: census formarum
 * paragraphorum in documento, declaratio per identitatem in oraculo).
 * Tabula ponderum decretoris (T38 b/c) his condicionatur; ante T38 c
 * nihil eum legit praeter linguam. */
nomen structura {
    constans character* lingua;
                   s32  dialectus;
                   s32  forma;
} OratioContextus;


/* Programma ex textu legere (stml_legere); regulae = elementa 'regula'
 * radicis ordine. NIHIL cum vitio (plagula "resolutio.stml", linea,
 * causa). Programma sine regulis validum est (nihil facit). */
OratioProgramma*
oratio_resolutio_programma_legere (
                     Piscina* piscina,
         InternamentumChorda* intern,
                      chorda  textus,
    OratioVocabulariumVitium* vitium);

/* Programma ex radice repositorii (NIHIL = "."):
 * oratio/partes/resolutio.stml. NIHIL cum vitio (plagula absens =
 * causa "plagula absens": instrumenta tunc sine resolutione
 * currunt). */
OratioProgramma*
oratio_resolutio_programma_onerare (
                     Piscina* piscina,
         InternamentumChorda* intern,
          constans character* radix,
    OratioVocabulariumVitium* vitium);

vacuum
oratio_resolutio_census_vacare (
    OratioResolutioCensus* census);

/* Programma applicare: radix = documentum, paragraphus aut sententia
 * (materia; patres figendi ante); regulae_numerus = primae N (-I =
 * omnes; s32); lingua = titulus linguae documenti ("latina" | "anglica";
 * NIHIL = non scripta); census NIHIL licet (per_regulam in piscina
 * creatur si NIHIL). FALSUM = memoria sola. */
b32
oratio_resolutio_applicare (
                       Piscina* piscina,
           InternamentumChorda* intern,
  constans MateriaLexiconRatum* ratum,
      constans OratioProgramma* programma,
                           s32  regulae_numerus,
            constans character* lingua,
                  MateriaNodus* radix,
         OratioResolutioCensus* census);

/* Idem cum CONTEXTU DECLARATO (T38 a): oratio_resolutio_applicare
 * delegat cum {lingua, classicus, prosa}. */
b32
oratio_resolutio_applicare_contextu (
                        Piscina* piscina,
            InternamentumChorda* intern,
   constans MateriaLexiconRatum* ratum,
       constans OratioProgramma* programma,
                            s32  regulae_numerus,
       constans OratioContextus* contextus,
                   MateriaNodus* radix,
          OratioResolutioCensus* census);

/* Contextus documenti ex censibus eius (instrumenta verba, arbor):
 * lingua per suffragia annotationis, dialectus ex censu partium
 * (ignotus = classicus), forma per oratio_forma_documenti_censu. */
vacuum
oratio_resolutio_contextus_documenti (
    constans OratioPartesCensus* census,
          constans MateriaNodus* radix,
                OratioContextus* contextus);

/* Tabulam ponderum ex textu legere in programma->pondera (T38 b): linea
 * '#' commentum; prima cetera CAPUT 'regula dialectus forma folliculus
 * valor contentiones rectae permille' NOMINE probatum - titulus alienus
 * refusatur (vitium->causa 'caput ponderum: <titulus>'), ordo latitudine
 * aliena refusatur cum linea ('latitudo ponderum'). Clavis = campi V
 * primi TAB iuncti. Limen NON applicatur (decretor applicat: tabula tota
 * inspectabilis). FALSUM cum vitio aut memoria. */
b32
oratio_resolutio_pondera_legere (
                     Piscina* piscina,
             OratioProgramma* programma,
                      chorda  textus,
    OratioVocabulariumVitium* vitium);

/* Pondus clavis (regula; dialectus, forma, folliculus, valor literis,
 * '-' absens); NIHIL si tabula nulla aut clavis absens. */
constans OratioPondus*
oratio_resolutio_pondus (
    constans OratioProgramma* programma,
                      chorda  regula,
          constans character* dialectus,
          constans character* forma,
          constans character* folliculus,
          constans character* valor);

/* Minimum contentionum ordinis ponderum quo ordo respondet
 * (ORATIO_PONDERA_LIMEN; ordinarium XX). */
i32
oratio_resolutio_pondera_limen (vacuum);

/* Lingua documenti ex censu annotationis: titulus linguae cuius
 * SUFFRAGIA plura (OratioPartesCensus.vocabula_linguarum - vocabula
 * quae linguam ferunt, non analyses: T18 2026-09-07); "latina" in
 * aequalitate aut sine analysi. */
constans character*
oratio_resolutio_lingua_censu (
    constans i32* linguae);

/* Idem, index in OratioLingua (oraculo: sententiae per linguam censae) */
i32
oratio_resolutio_lingua_censu_index (
    constans i32* linguae);

#endif /* ORATIO_RESOLUTIO_H */
