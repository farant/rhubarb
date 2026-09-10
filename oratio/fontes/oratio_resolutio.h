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
} OratioProgramma;

/* census per regulam: ordines applicati (vocabula quorum praelatio
 * huic regulae debetur, prima vincente) */
nomen structura {
    chorda titulus;
       i32 numerus;
} OratioResolutioRegulaCensus;

nomen structura {
    i32  sententiae;   /* sententiae tractatae */
    i32  fractae;      /* compositio/expansio fracta: praeterita */
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
} OratioResolutioCensus;


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
